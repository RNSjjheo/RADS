//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "Atmos.h"
#include "UnitMain.h"

//---------------------------------------------------------------------------
TAtmos::TAtmos()
{
	Temperature    = 0;
	Pressure       = 0;

	MeasureDate    = "";
	MeasureTime    = "";
	m_RecvMsg      = "";

	m_pComPort     = NULL;
	m_pList        = new TList();

	Count          = 0;
	memset(m_Buffer, 0x00, BUFFER_1K);

	Status         = enSensorIdle;
}

//---------------------------------------------------------------------------
TAtmos::~TAtmos()
{
	if ( m_pList ) {
		STAtmos *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STAtmos *) m_pList->Items[i];
				delete pData;
			} catch ( Exception &e ) {
				break;
			}
		}
		m_pList->Clear();
		delete m_pList;
	}
}

//---------------------------------------------------------------------------
void TAtmos::Clear()
{
	Temperature    = 0;
	Pressure       = 0;

	MeasureDate    = "";
	MeasureTime    = "";
	m_RecvMsg      = "";

	Count          = 0;
	memset(m_Buffer, 0x00, BUFFER_1K);

	Status         = enSensorIdle;

	//ClearList(); 	// 2021.11.16
}

//---------------------------------------------------------------------------
void TAtmos::Init(TApdComPort *pPort)
{
	m_pComPort = pPort;	// Serial Port
}

//---------------------------------------------------------------------------
bool TAtmos::Start(UnicodeString Date, UnicodeString Time, bool bFirst)
{
	Clear();
	if ( bFirst ) ClearList();

	this->MeasureDate = Date;
	this->MeasureTime = Time;
	this->m_RecvMsg   = "";

	Status = enSensorStart;
	return true;
}

//---------------------------------------------------------------------------
bool TAtmos::Finish()
{
	Status = enSensorFinish;

	Caculate();	// Average Temperature, Pressure

	Status = enSensorIdle;
	return true;
}

//---------------------------------------------------------------------------
bool TAtmos::Decode(AnsiString RecvMsg, int iLen)
{
	RecvMsg = Trim(RecvMsg);	// \r\n25.11,1007.14\r\n => 25.11,1007.14

	int iPos = 0;
	AnsiString sTemperature, sPressure;

	TStringList *pStrLst = new TStringList();

	pStrLst->Delimiter     = ',';
	pStrLst->DelimitedText = RecvMsg;

//	iPos = RecvMsg.AnsiPos(",");
//	if ( iPos == 0 ) return false;
//
//	sTemperature = RecvMsg.SubString(1, iPos-1);
//	sPressure    = RecvMsg.SubString(iPos+1, RecvMsg.Length() - iPos);
	if ( pStrLst->Count < 2 ) {
		LogMessage("[TAtmos:Decode] Item is too short..");
		delete pStrLst;
		return false;
	}

	try {
		sTemperature = pStrLst->Strings[0];
		sPressure    = pStrLst->Strings[1];
	} catch ( Exception &e ) {
		delete pStrLst;
		return false;
	}

	try {
		Temperature = StrToFloat(sTemperature);
		Pressure    = StrToFloat(sPressure);
	} catch ( Exception &e ) {
		delete pStrLst;
		LogMessage(UnicodeString("[TAtmos:Decode]") + e.Message);
		return false;
	}

	STAtmos *pAtmos = new STAtmos();
	pAtmos->Temperature = Temperature;
	pAtmos->Pressure    = Pressure;

	m_pList->Add(pAtmos);

	delete pStrLst;

	return true;
}

//---------------------------------------------------------------------------
bool TAtmos::Receive(AnsiString RecvMsg, int iLen)
{
	if ( Status == enSensorFinish || Status == enSensorIdle ) {	// Idle Status => Discard
		//LogMessage(RecvMsg + " Discard...", false);
		return true;	// not receive status
	}

	m_RecvMsg += RecvMsg;

	int iPos = 0;
	iPos = m_RecvMsg.AnsiPos("\r\n");

	AnsiString sCmd;
	int iNextPos = 0;
	while ( iPos > 0  ) {
		sCmd = m_RecvMsg.SubString(1, iPos-1);
		iNextPos = iPos + 2;
		LogMessage("[Atmos]<=" + sCmd);

		Decode(sCmd, sCmd.Length());

		m_RecvMsg = m_RecvMsg.SubString(iNextPos, m_RecvMsg.Length() - (iNextPos-1));
		m_RecvMsg = m_RecvMsg.Trim();	// trim \r\n<body>\r\n

		iPos = 0;
		iPos = m_RecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
bool TAtmos::Send(AnsiString RecvMsg, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
void TAtmos::Caculate()
{
	double T, P;
	double MinT, AvgT, MaxT;
	double MinP, AvgP, MaxP;

	T = P = 0;
	MinT = AvgT = MaxT = 0;
	MinP = AvgP = MaxP = 0;

	if ( m_pList ) {
		STAtmos *pData = NULL;
		int iValidCount = 0;
		bool bFirst = true;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STAtmos *) m_pList->Items[i];
			} catch ( Exception &e ) {
				break;
			}

			if ( bFirst ) {
				MaxT = pData->Temperature;
				MaxP = pData->Pressure;
				bFirst = false;
			}

			T += pData->Temperature;
			P += pData->Pressure;
			iValidCount++;

			if ( MinT < pData->Temperature ) MinT = pData->Temperature;
			if ( MinP > pData->Pressure    ) MinP = pData->Pressure;
		}

		//if ( m_pList->Count > 0 ) {
		if ( iValidCount > 0 ) {
			AvgT  = T / iValidCount;
			AvgP  = P / iValidCount;
		} else {
			// nothing to do..
		}

	} else {
		// nothing to do..
	}

	Temperature = AvgT;
	Pressure    = AvgP;
}

//---------------------------------------------------------------------------
void TAtmos::GetData(double &T, double &P)
{
	T = this->Temperature;
	P = this->Pressure;

//	double T, P;
//	double MinT, AvgT, MaxT;
//	double MinP, AvgP, MaxP;
//
//	T = P = 0;
//	MinT = AvgT = MaxT = 0;
//	MinP = AvgP = MaxP = 0;
//
//	if ( m_pList ) {
//		STAtmos *pData = NULL;
//		int iValidCount = 0;
//		bool bFirst = true;
//		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
//			pData = (STAtmos *) m_pList->Items[i];
//
//			if ( bFirst ) {
//				MaxT = pData->Temperature;
//				MaxP = pData->Pressure;
//				bFirst = false;
//			}
//
//			T += pData->Temperature;
//			P += pData->Pressure;
//			iValidCount++;
//
//			if ( MinT < pData->Temperature ) MinT = pData->Temperature;
//			if ( MinP > pData->Pressure    ) MinP = pData->Pressure;
//		}
//
//		//if ( m_pList->Count > 0 ) {
//		if ( iValidCount > 0 ) {
//			AvgT  = T / iValidCount;
//			AvgP  = P / iValidCount;
//		} else {
//			// nothing to do..
//		}
//
//	} else {
//		// nothing to do..
//	}
//
//	Temperature = AvgT;
//	Pressure    = AvgP;
}

//---------------------------------------------------------------------------
bool TAtmos::ClearList()
{
	if ( m_pList ) {
		STAtmos *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STAtmos *) m_pList->Items[i];
				delete pData;
			} catch ( Exception &e ) {
				break;
			}
		}
		m_pList->Clear();
	}
	return true;
}

//---------------------------------------------------------------------------
void TAtmos::LogMessage(UnicodeString sMsg, bool bFile)
{
	FormMain->LogMessage(sMsg, bFile);
}

#pragma package(smart_init)
