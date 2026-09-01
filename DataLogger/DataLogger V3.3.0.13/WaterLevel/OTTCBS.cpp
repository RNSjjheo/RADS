//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "OTTCBS.h"
#include "AniThread.h"
#include "System.RegularExpressions.hpp"
//---------------------------------------------------------------------------

#define CBS_OK	     0
#define CBS_ERROR	 1
#define CBS_DATA     2
#define CBS_CONTINUE 3
#define CBS_UNKNOWN 99

#define CBS_DATA_STEP_NOTREADY -1	// NOT READY
#define CBS_DATA_STEP_0         0	// m
#define CBS_DATA_STEP_1         1	// cm
#define CBS_DATA_STEP_2         2	// ft
#define CBS_DATA_STEP_3         3	// Pressure(mbar)
#define CBS_DATA_STEP_4         4	// psi
#define CBS_DATA_STEP_5         5	// Temperature
#define CBS_DATA_STEP_6         6	// checksum

//---------------------------------------------------------------------------
// Constructor
TCBS::TCBS()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status
	m_enWorkingStep= enSensorIdle;

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = OTTCBS_STR;	// WaterLevel Sensor type
	Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;

	AvgWaterLevel  = 0;
	MinWaterLevel  = 0;
	MaxWaterLevel  = 0;
	Temperature    = 0;
	Pressure       = 0;
	Dept           = 0;

	m_pList        = new TList();

	m_Count        = 0;
	m_iIndex       = -1;

	bLeak          = true;	// Leak or not-

	RecvStr        = "";
}

//---------------------------------------------------------------------------
// Destructor
TCBS::~TCBS()
{
	// Resource lease
	if ( m_pList ) {
		STOTTCBSData *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			pData = (STOTTCBSData *) m_pList->Items[i];
			delete pData;
		}
		m_pList->Clear();
		delete m_pList;
	}

	// other resource release
}

//---------------------------------------------------------------------------
// Clear
void TCBS::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	//m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status
	m_enWorkingStep= enSensorIdle;

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = OTTCBS_STR;	// WaterLevel Sensor type
	//Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;

	AvgWaterLevel  = 0;
	MinWaterLevel  = 0;
	MaxWaterLevel  = 0;
	Temperature    = 0;
	Pressure       = 0;
	Dept           = 0;

	m_Count        = 0;
	m_iIndex       = CBS_DATA_STEP_NOTREADY;	// -1
	bLeak          = true;	// Leak or not-

	RecvStr        = "";
}

//---------------------------------------------------------------------------
// ClearList
bool TCBS::ClearList()
{
	if ( m_pList ) {
		STOTTCBSData *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			pData = (STOTTCBSData *) m_pList->Items[i];
			delete pData;
		}
		m_pList->Clear();
	}
	return true;
}

//---------------------------------------------------------------------------
// GetStatus
void TCBS::Init(TApdComPort *pPort)
{
	m_pPort = pPort;
}

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst)
{
	Clear();	// Status Clear

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	
	AnsiString StartCmd = "START\r";
	
	Send(StartCmd, StartCmd.Length());
	
	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	//m_TimeStamp = Now();	// 2017.09.20 TimeStamp는 데이터를 수신하면 갱신한다.
	
    return true;
}

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	//if ( !bResult ) return bResult;

	m_enStep = enSensorIdle;
	m_enWorkingStep = enSensorIdle;

	FormMain->SetWaterLevelIcon(enWaterLevelNormal);
	if ( FormMain->m_pAniThread != NULL ) {
		FormMain->m_pAniThread->m_bWaterLevel = false;
		FormMain->m_pAniThread->m_WaterLevelIndex = enWaterLevelNormal;
	}
	FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

	//::PostMessage(FormMain->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);  // Notify MainForm

	return true;
}

//---------------------------------------------------------------------------
// Data Patten
//[09:11:59] [OTTCBS]<=0
//[09:11:59] [OTTCBS]<=0+0.185
//[09:11:59] [OTTCBS] <= Unknown data..
//[09:11:59] [OTTCBS]<=DATA 0:0 +0.1850+18
//[09:11:59] [OTTCBS]<=+180+0.61
//[09:12:00] [OTTCBS]<=+0.610+18.10
//[09:12:00] [OTTCBS]<=+18.100+0.263
//[09:12:00] [OTTCBS]<=+0.2630+9.5
//[09:12:00] [OTTCBS]<=+9.50+0
//[09:12:00] [OTTCBS]=>STOP
//
//[11:45:29] [OTTCBS]<=DATA 0:0 +0.185+19+0.61+18.18+0.264+15.0+0
// Decode
// OK, ER : xxx, DATA 0:0 xxxx

bool TCBS::Decode(AnsiString RecvMsg, int iLen)
{
//	if ( m_enStep == enSensorIdle ) {
//		LogMessage("[OTTCBS] Sensor is idle status. but data receive...");
//		return false;
//	}

	BYTE Type = CBS_UNKNOWN;

	if ( iLen < 2 ) return false;	// Message too short

	if ( RecvMsg.AnsiPos("OK") > 0)         Type = CBS_OK;
	else if ( RecvMsg.AnsiPos("ER") > 0 )   Type = CBS_ERROR;
	else if ( RecvMsg.AnsiPos("DATA") > 0 ) Type = CBS_DATA;
	else if ( m_iIndex >= 0 )               Type = CBS_CONTINUE;	// Data continue
	else                                    Type = CBS_UNKNOWN;

	switch ( Type ) {
		case CBS_OK :
			{
				if ( m_enStep == enSensorStart ) {
					m_enStep = enSensorWorking;
					m_enWorkingStep = enSensorWorking;
					m_TimeStamp = Now();	// 2017.09.20 TimeStamp는 데이터를 수신하면 갱신한다.
					FormMain->SetWaterLevelIcon(enWaterLevelAni1);
					if ( FormMain->m_pAniThread != NULL ) {
						FormMain->m_pAniThread->m_bWaterLevel = true;
						FormMain->m_pAniThread->m_WaterLevelIndex = enWaterLevelAni1;
					}
					FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);
				}
				if ( m_enStep == enSensorFinish) {
					m_enStep = enSensorIdle;
					m_enWorkingStep = enSensorIdle;
					FormMain->SetWaterLevelIcon(enWaterLevelNormal);
					if ( FormMain->m_pAniThread != NULL ) {
						FormMain->m_pAniThread->m_bWaterLevel = false;
						FormMain->m_pAniThread->m_WaterLevelIndex = enWaterLevelNormal;
					}
					FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);
				}
			}
			break;
		case CBS_ERROR :
			{
				LogMessage("[OTTCBS] <= Error");
			}
			break;
		case CBS_DATA :	// DATA 0:0 +0.1670+17 => 0.167만 취한다.
			{
				m_iIndex = CBS_DATA_STEP_0; 	// Received DATA

				RecvStr += RecvMsg.Trim();	// Trim SPACE, \r\n Character

				int delimiter = 0;
				for ( int i = 1 ; i < RecvStr.Length() ; i++ ) {
					if ( RecvStr.operator[](i) == '+' ||
						 RecvStr.operator[](i) == '-' ) {
						 delimiter++;
					}
				}

				if ( delimiter >= 7 ) {	// Full Text received
					char  Stream[128] = "";
					int   Pos = 0;
					char *Token[10];
					int   TokenPos = 0;
					int   Length = 0;

					Length = RecvStr.Length();
					memcpy(Stream, RecvStr.c_str(), Length);

					for ( int i = 0 ; i <= Length ; i ++ ) {
						if ( Stream[i] == '+' || Stream[i] == '-' ) {
							Token[TokenPos++] = &Stream[i];
						}
					}

					AnsiString DeptStr, DeptCmStr, DeptFtStr;
					AnsiString PressureMbar, PressurePSI;
					AnsiString TemperatureStr;

					//double Dept, Pressure, Temperature;
					Dept = Pressure = Temperature = 0.0f;

					DeptStr        = AnsiString(Token[0], Token[1] - Token[0]);
					DeptCmStr      = AnsiString(Token[1], Token[2] - Token[1]);
					DeptFtStr      = AnsiString(Token[2], Token[3] - Token[2]);
					PressureMbar   = AnsiString(Token[3], Token[4] - Token[3]);
					PressurePSI    = AnsiString(Token[4], Token[5] - Token[4]);
					TemperatureStr = AnsiString(Token[5], Token[6] - Token[5]);

					try {
						Dept        = StrToFloat(DeptStr); 	// meter
						Pressure    = StrToFloat(PressurePSI);	// PSI
						Temperature = StrToFloat(TemperatureStr);	// 'C
					} catch ( Exception &e ) {
						LogMessage("OTTCBS::Decode error");
					}

					AvgWaterLevel = Dept + theEnv.m_WaterOffset;
					MinWaterLevel = Dept + theEnv.m_WaterOffset;
					MaxWaterLevel = Dept + theEnv.m_WaterOffset;

					m_enStep = enSensorFinish;
					m_enWorkingStep = enSensorFinish;
					bLeak    = false;	// Leak or not

					::PostMessage(FormMain->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);	// Notify Main Form

					AnsiString sFinishCmd = "STOP\r";
					Send(sFinishCmd, sFinishCmd.Length());

					m_iIndex == CBS_DATA_STEP_NOTREADY;

				} else {				// continue buffering
					// continue buffering
				}
			}
			break;
		case CBS_CONTINUE :
			{
				m_iIndex = CBS_DATA_STEP_0; 	// Received DATA

				RecvStr += RecvMsg.Trim();	// Trim SPACE, \r\n Character

				int delimiter = 0;
				for ( int i = 1 ; i < RecvStr.Length() ; i++ ) {
					if ( RecvStr.operator[](i) == '+' ||
						 RecvStr.operator[](i) == '-' ) {
						 delimiter++;
					}
				}

				if ( delimiter >= 7 ) {	// Full Text received
					char Stream[128] = "";
					int  Pos = 0;
					char *Token[10];
					int   TokenPos = 0;
					int   Length = 0;

					Length = RecvStr.Length();
					memcpy(Stream, RecvStr.c_str(), Length);

					for ( int i = 0 ; i <= Length ; i ++ ) {
						if ( Stream[i] == '+' || Stream[i] == '-' ) {
							Token[TokenPos++] = &Stream[i];
						}
					}

					AnsiString DeptStr, DeptCmStr, DeptFtStr;
					AnsiString PressureMbar, PressurePSI;
					AnsiString TemperatureStr;

					//double Dept, Pressure, Temperature;
					Dept = Pressure = Temperature = 0.0f;

					DeptStr        = AnsiString(Token[0], Token[1] - Token[0]);
					DeptCmStr      = AnsiString(Token[1], Token[2] - Token[1]);
					DeptFtStr      = AnsiString(Token[2], Token[3] - Token[2]);
					PressureMbar   = AnsiString(Token[3], Token[4] - Token[3]);
					PressurePSI    = AnsiString(Token[4], Token[5] - Token[4]);
					TemperatureStr = AnsiString(Token[5], Token[6] - Token[5]);

					try {
						Dept        = StrToFloat(DeptStr);
						Pressure    = StrToFloat(PressurePSI);
						Temperature = StrToFloat(TemperatureStr);
					} catch ( Exception &e ) {
						LogMessage("OTTCBS::Decode error");
					}

					AvgWaterLevel = Dept + theEnv.m_WaterOffset;
					MinWaterLevel = Dept + theEnv.m_WaterOffset;
					MaxWaterLevel = Dept + theEnv.m_WaterOffset;

					m_enStep = enSensorFinish;
					m_enWorkingStep = enSensorFinish;
					bLeak    = false;	// Leak or not

					::PostMessage(FormMain->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);	// Notify Main Form

					AnsiString sFinishCmd = "STOP\r";
					Send(sFinishCmd, sFinishCmd.Length());

					m_iIndex == CBS_DATA_STEP_NOTREADY;

				} else {				// continue buffering
					// continue buffering
				}
			}
			break;
		case CBS_UNKNOWN :
			{
				LogMessage("[OTTCBS] <= Unknown data..");
			}
			break;
		default :
			{
				// nothing to do..
			}
	} // end of switch

	return true;
}

//---------------------------------------------------------------------------
bool TCBS::Decode(BYTE *pBuffer, int iLen)
{
	bool bResult = true;

	// do something..

	return bResult;
};

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::Receive(AnsiString RecvMsg, int iLen)
{
	//if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_TimeStamp = Now();	// Receive Time Stamp

	m_RecvMsg += RecvMsg;

	if ( m_RecvMsg.Length() > 100 ) { // 100 바이트가 넘으면 이상한 상황으로 간주한다.
		LogMessage("[OTTCBS] Broken data flushing ... data abandon..");
		m_RecvMsg = "";
		return true;
	}

	int iPos = 0;
	iPos = m_RecvMsg.AnsiPos("\r\n");

	AnsiString sCmd;
	int iNextPos = 0;
	while ( iPos > 0  ) {
		sCmd = m_RecvMsg.SubString(1, iPos-1);
		iNextPos = iPos + 2;
		LogMessage("[OTTCBS]<=" + sCmd);

		Decode(sCmd, sCmd.Length());

		m_RecvMsg = m_RecvMsg.SubString(iNextPos, m_RecvMsg.Length() - (iNextPos-1));
		m_RecvMsg = m_RecvMsg.Trim();	// trim \r\n<body>\r\n

		iPos = 0;
		iPos = m_RecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
bool TCBS::Receive(BYTE *pBuffer, int iLen)
{
	bool bResult = true;

	// do something..

	return bResult;
};

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		FormMain->WaterPort->PutString(RecvMsg);
		LogMessage("[OTTCBS]=>" + RecvMsg);
	} catch ( Exception &e ) {
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	Value2DB();	// Value Mapping

	m_RWaterLevel.MeasureDate = MeasureDate;
	m_RWaterLevel.MeasureTime = MeasureTime;
	m_RWaterLevel.WaterKind   = WaterKind;

	bool bResult = false;
	bResult = m_RWaterLevel.Insert();

	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
void TCBS::DB2Value()
{
	AvgWaterLevel         = m_RWaterLevel.AvgWaterLevel;
	MinWaterLevel         = m_RWaterLevel.MinWaterLevel;
	MaxWaterLevel         = m_RWaterLevel.MaxWaterLevel;
	Temperature           = m_RWaterLevel.Value01;
	Pressure              = m_RWaterLevel.Value02;
	Dept                  = m_RWaterLevel.Value03;
	Offset                = theEnv.m_WaterOffset;
}

//---------------------------------------------------------------------------
// GetStatus
void TCBS::Value2DB()
{
	m_RWaterLevel.AvgWaterLevel  = AvgWaterLevel;
	m_RWaterLevel.MinWaterLevel  = MinWaterLevel;
	m_RWaterLevel.MaxWaterLevel  = MaxWaterLevel;
	m_RWaterLevel.Value01        = Temperature;
	m_RWaterLevel.Value02        = Pressure;
	m_RWaterLevel.Value03        = Dept;
	m_RWaterLevel.Value10        = Offset;
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TCBS::GetWaterLevel()
{
	return AvgWaterLevel;
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TCBS::GetArea()
{
	WaterLevel = AvgWaterLevel;

	if ( WaterLevel < m_pRFlowHA->MinH || WaterLevel > m_pRFlowHA->MaxH ) { // 수위 단면적 곡선의 신뢰 구간을 벗어나면 의미 없는 것으로
		Area = 0;
	} else {
		Area = (m_pRFlowHA->A6 * Power(WaterLevel,6)) +
			   (m_pRFlowHA->A5 * Power(WaterLevel,5)) +
			   (m_pRFlowHA->A4 * Power(WaterLevel,4)) +
			   (m_pRFlowHA->A3 * Power(WaterLevel,3)) +
			   (m_pRFlowHA->A2 * Power(WaterLevel,2)) +
			   (m_pRFlowHA->A1 * Power(WaterLevel,1)) +
			   (m_pRFlowHA->A0);
	}

	return Area;
}

//---------------------------------------------------------------------------
// GetStatus
bool TCBS::CalAvgMinMaxWaterLevel()
{
//	double AvgWaterLevel = 0;
//	double MinWaterLevel = 0;
//	double MaxWaterLevel = 0;
//	double Temperature   = 0;
//	double Pressure      = 0;
//	double Dept          = 0;
//
//	if ( m_pList ) {
//		STOTTCBSData *pData = NULL;
//		int iValidCount = 0;
//		bool bFirst = true;
//		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
//			pData = (STOTTCBSData *) m_pList->Items[i];
//
//			if ( pData->Dept < theEnv.m_dWaterLowerBound ||
//				 pData->Dept > theEnv.m_dWaterUpperBound )  {
//
//				UnicodeString Str;
//				Str = Str.sprintf(L"Water Dept Data is out of range. %10.2f, LowerBound: %10.2f Uppderbound: %10.2f", pData->Dept, theEnv.m_dWaterLowerBound,
//								   theEnv.m_dWaterUpperBound );
//				LogMessage(Str);
//				continue;	// 유효범위 이외의 값이 오면 처리하지 않는다.
//			}
//
//			if ( bFirst ) {
//				MinWaterLevel = pData->Dept;
//				MaxWaterLevel = pData->Dept;
//				bFirst = false;
//			}
//
//			Temperature += pData->Temperature;
//			Pressure    += pData->Pressure;
//			Dept        += pData->Dept;
//			iValidCount++;
//
//			if ( MinWaterLevel < pData->Dept ) MinWaterLevel = pData->Dept;
//			if ( MaxWaterLevel > pData->Dept ) MaxWaterLevel = pData->Dept;
//		}
//
//		//if ( m_pList->Count > 0 ) {
//		if ( iValidCount > 0 ) {
//			AvgWaterLevel = (Dept / iValidCount) + theEnv.m_WaterOffset;
//			Temperature   =  Temperature / iValidCount;
//			Pressure      =  Pressure / iValidCount;
//			Dept          =  Dept / iValidCount;
//		} else {
//			// nothing to do..
//		}
//
//	} else {
//		// nothing to do..
//	}
//
//	this->AvgWaterLevel = AvgWaterLevel;
//	this->MinWaterLevel = MinWaterLevel;
//	this->MaxWaterLevel = MaxWaterLevel;
//	this->Temperature   = Temperature;
//	this->Pressure      = Pressure;
//	this->Dept          = Dept;
//	this->Offset        = theEnv.m_WaterOffset;

	return true;
}

//---------------------------------------------------------------------------
// DecodeData
bool TCBS::DecodeData()
{
//	STOTTCBSData *pData = new STLT500Data();
//
//	//pData->Temperature = 0.0;
//	//pData->Pressure    = 0.0;
//	//pData->Dept        = 0.0;	// Decode Protocol Data
//
//	pData->Temperature = Analyze(TempAck);
//	pData->Pressure    = Analyze(PressureAck);
//	pData->Dept        = Analyze(DeptAck);	// Decode Protocol Data
//
//	m_pList->Add(pData);

	return true;
}

//---------------------------------------------------------------------------
// Analyze String data to double data
double TCBS::Analyze(AnsiString sData)
{
//	if ( sData.Length() < 41 ) return 0;
//
//	AnsiString s = sData.SubString(8, 8);	//
//
//	AnsiString sBin = Hex2Bin(s);
//
//	float fValue = 0;
//	fValue = Bin2IEEE745(sBin);
//
//	return fValue;
	return 0;
}

#pragma package(smart_init)
