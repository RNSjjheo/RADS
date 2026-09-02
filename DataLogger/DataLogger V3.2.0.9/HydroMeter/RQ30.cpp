//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RQ30.h"
#include "AniThread.h"
#include "System.RegularExpressions.hpp"

int __fastcall CompareRQVelocity(void *Item1, void *Item2)
{
	STRQ30Data *A, *B;
	A = (STRQ30Data *)Item1;
	B = (STRQ30Data *)Item2;
	if ( A->Velocity < B->Velocity )       return -1;	// True  => Skip
	else if ( A->Velocity == B->Velocity ) return  0;
	else                                   return  1;	// false => Swap
}

//---------------------------------------------------------------------------
// Constructor
TRQ30::TRQ30()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = RQ30_STR;

	memset(&RQ30Data, 0x00, sizeof(STRQ30Data));
	m_pRQ30List  = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TRQ30::~TRQ30()
{
	STRQ30Data *pRQ30Data = NULL;
	for ( int i = 0 ; i < m_pRQ30List->Count ; i++ ) {
		try {
			pRQ30Data = (STRQ30Data *) m_pRQ30List->Items[i];
			delete pRQ30Data;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pRQ30List->Clear();

	delete m_pRQ30List;
}

//---------------------------------------------------------------------------
// Clear
void TRQ30::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = RQ30_STR;

	memset(&RQ30Data, 0x00, sizeof(STRQ30Data));
	memset(&AirmarData, 0x00, sizeof(STAirmarData));

	STRQ30Data *pRQ30Data = NULL;
	for ( int i = 0 ; i < m_pRQ30List->Count ; i++ ) {
		try {
			pRQ30Data = (STRQ30Data *) m_pRQ30List->Items[i];
			delete pRQ30Data;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pRQ30List->Clear();
}

//---------------------------------------------------------------------------
// Init
void TRQ30::Init(TApdComPort *pPort, bool bFirst)
{
	m_pPort  = pPort;
	m_bFirst = bFirst;
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
	// nothing to do..
}

//---------------------------------------------------------------------------
// Start
bool TRQ30::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	bool bResult = false;

	Clear();	// Status Clear

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	m_TimeStamp = Now();

	LogMessage("RQ30 started..");

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TRQ30::Finish()
{
	bool bResult = false;
	m_enStep = enSensorFinish;

	bResult = Save(MeasureDate, MeasureTime);

	m_enStep = enSensorIdle;

	LogMessage("RQ30 finished..");

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRQ30::Decode(AnsiString RecvMessage, int iLen)
{
// Sommer Protocol(old)
// #M0001G00se00      15|01    1.35|02 -99.000|03  -18.90|04 -153.85|05    1.94|2827;	// Measurement value
//#M0001G01se06 -99.000|07 -153.85|08     350|09   12.46|2815;                          // Special value
//#M0001G02se10       0|11       1|12       0|13      12|14    3494|15       8|3856;    // Analysis value #1
//#M0001G03se16       0|17       0|1899999998|19    29.4|20    21.5|BEF6;               // Analysis value #2

	LogMessage(RecvMessage);

	RecvMessage = Trim(RecvMessage);	// 앞 뒤의 Space를 지운다.

	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = '|';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = RecvMessage;

	if ( pStrList->Count <= 0 ) {
		LogMessage("RQ30 lack of token..");
		delete pStrList;	// release
		return false;
	}

	AnsiString Prefix = pStrList->Strings[0];	//#M0001G00 => Main Value, #M0001G01 => Special Value
	AnsiString x = "";
	try {
		x = Prefix.SubString(7,3);
	} catch ( Exception &e ) {
		delete pStrList;
		return false;
	}

	AnsiString Token;
	AnsiString No[6];
	AnsiString Value[6];

	// RawData Add
	RQ30Data.RawData += RecvMessage+"\r\n";

// Sommer Protocol(new)
//#M0001G01se0199999998|02   -0.03|03    0.00|04  -30.99|05    0.00|06    0.00|A247;
//#M0001G02se07    0.00|08    0.00|09     638|10   18.76|3D21;
//#M0001G03se11       0|12    2823|13       0|14      10|15    4809|16     -10|C279;
//#M0001G04se17       6|18       0|1999999998|20    55.8|21    12.8|CFEE;

	if ( x == "G01" ) { 	// Main Value   #M0001G01se0199999998|02   -0.03|03    0.00|04  -30.99|05    0.00|06    0.00|A247;
		if ( pStrList->Count < 6 ) {
			LogMessage("RQ30 Lack of token..main value");
			return false;
		}

		Token    = pStrList->Strings[0];	//#M0001G01se0199999998
		No[0]    = Token.SubString(12,2);
		Value[0] = Token.SubString(14, Token.Length()-14+1);

		Token    = pStrList->Strings[1];   	//02   -0.03	Measured WaterLevel
		No[1]    = Token.SubString(1,2);
		Value[1] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[2]; 	//03    0.00	Measured Velocity
		No[2]    = Token.SubString(1,2);
		Value[2] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[3]; 	//04  -30.99	SNR
		No[3]    = Token.SubString(1,2);
		Value[3] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[4]; 	//05    0.00	Measured Discharge
		No[4]    = Token.SubString(1,2);
		Value[4] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[5]; 	//06    0.00	Area
		No[5]    = Token.SubString(1,2);
		Value[5] = Token.SubString(3, Token.Length()-3+1);

		int iNo = 0;
		int  iValue = 0;
		double dValue = 0.0f;
		for ( int i = 0 ; i < 6 ; i++ ) {
			try {
				iNo = StrToInt(No[i]);
			} catch ( ...  ) {
				iNo = 0;
			}
			if ( i == 0 ) {	// i => 0 : SelfCheck
				try {
					iValue = StrToInt(Value[i]);
				} catch ( ... ) {
					iValue = 0;
				}
			} else {		// i => 1,2,3,4,5 => Water, Velocity,Quality, Discharge, Area
				try {
					dValue = StrToFloat(Value[i]);
				} catch (...) {
					dValue = 0.0f;
				}
			}

			if ( iNo == 1 ) RQ30Data.SelfCheck  = iValue;
			if ( iNo == 2 ) RQ30Data.Waterlevel = dValue;
			if ( iNo == 3 ) RQ30Data.Velocity   = dValue;
			if ( iNo == 4 ) RQ30Data.Quality    = dValue;
			if ( iNo == 5 ) RQ30Data.Discharge  = dValue;
			if ( iNo == 6 ) RQ30Data.Area       = dValue;
		}
		RQ30Data.Complete |= RQ30_MAIN;

	} else if ( x == "G02" ) {	// Special Vaue  #M0001G02se07    0.00|08    0.00|09     638|10   18.76|3D21;
		if ( pStrList->Count < 4 ) {
			LogMessage("RQ30 Lack of token..special value..");
			return false;
		}

		Token    = pStrList->Strings[0];	//#M0001G02se07    0.00 	Learned Velocity
		No[0]    = Token.SubString(12,2);
		Value[0] = Token.SubString(14, Token.Length()-14+1);

		Token    = pStrList->Strings[1];   	//08    0.00	Learned Discharge
		No[1]    = Token.SubString(1,2);
		Value[1] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[2]; 	//09     638	Opposite Content
		No[2]    = Token.SubString(1,2);
		Value[2] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[3]; 	//10   18.76	Supply Voltage
		No[3]    = Token.SubString(1,2);
		Value[3] = Token.SubString(3, Token.Length()-3+1);

		int iNo = 0;
		double dValue = 0;
		for ( int i = 0 ; i < 4 ; i++ ) {
			try {
				iNo = StrToInt(No[i]);
			} catch (...) {
				iNo = 0;
			}
			try {
				dValue = StrToFloat(Value[i]);
			} catch (...) {
				dValue = 0.0f;
			}

			if ( iNo == 7 ) RQ30Data.LearnVelocity   = dValue;
			if ( iNo == 8 ) RQ30Data.LearnDischarge  = dValue;
			if ( iNo == 9 ) RQ30Data.OppositeContent = dValue;
			if ( iNo == 10) RQ30Data.SupplyVoltage   = dValue;
		}
		RQ30Data.Complete |= RQ30_SPECIAL;

	} else if (x == "G03") { // Analysis Value 01 => #M0001G03se11       0|12    2823|13       0|14      10|15    4809|16     -10|C279;
		if ( pStrList->Count < 6 ) {
			LogMessage("RQ30 Lack of token..special value..");
			return false;
		}

		Token    = pStrList->Strings[0];	//M0001G03se11       0 	Peak Width
		No[0]    = Token.SubString(12,2);
		Value[0] = Token.SubString(14, Token.Length()-14+1);

		Token    = pStrList->Strings[1];   	//12    2823	CSR
		No[1]    = Token.SubString(1,2);
		Value[1] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[2]; 	//13       0	AreaPeak
		No[2]    = Token.SubString(1,2);
		Value[2] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[3]; 	//14      10	RMSPic
		No[3]    = Token.SubString(1,2);
		Value[3] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[4]; 	//15    4809	Amplification
		No[4]    = Token.SubString(1,2);
		Value[4] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[5]; 	//16     -10   	AmplificationRelation
		No[5]    = Token.SubString(1,2);
		Value[5] = Token.SubString(3, Token.Length()-3+1);

		int iNo = 0;
		int  iValue = 0;
		for ( int i = 0 ; i < 6 ; i++ ) {
			try {
				iNo = StrToInt(No[i]);
			} catch (...) {
				iNo = 0;
			}
			try {
				iValue = StrToInt(Value[i]);
			} catch (...) {
				iValue = 0;
			}

			if ( iNo == 11 ) RQ30Data.PeakWidth   = iValue;
			if ( iNo == 12 ) RQ30Data.CSR         = iValue;
			if ( iNo == 13 ) RQ30Data.AreaPeak    = iValue;
			if ( iNo == 14 ) RQ30Data.RmsPic      = iValue;
			if ( iNo == 15 ) RQ30Data.Amp         = iValue;
			if ( iNo == 16 ) RQ30Data.AmpRelation = iValue;
		}
		RQ30Data.Complete |= RQ30_ANALYSIS1;

	} else if (x == "G04") { // Analysis Value 02 => #M0001G04se17       6|18       0|1999999998|20    55.8|21    12.8|CFEE;
		if ( pStrList->Count < 5 ) {
			LogMessage("RQ30 Lack of token..special value..");
			return false;
		}

		Token    = pStrList->Strings[0];	//#M0001G04se17       6   	SignalRelation
		No[0]    = Token.SubString(12,2);
		Value[0] = Token.SubString(14, Token.Length()-14+1);

		Token    = pStrList->Strings[1];   	//18       0    	ErrorCode
		No[1]    = Token.SubString(1,2);
		Value[1] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[2]; 	//1999999998
		No[2]    = Token.SubString(1,2);
		Value[2] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[3]; 	//20    55.8
		No[3]    = Token.SubString(1,2);
		Value[3] = Token.SubString(3, Token.Length()-3+1);

		Token    = pStrList->Strings[4]; 	//21    12.8
		No[4]    = Token.SubString(1,2);
		Value[4] = Token.SubString(3, Token.Length()-3+1);

		int iNo = 0;
		int  iValue = 0;
		double dValue = 0.0f;
		for ( int i = 0 ; i < 5 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				if ( iNo < 20 ) {
					try {
						iValue = StrToInt(Value[i]);
					} catch ( ... ) {
						iValue = 0;
					}
				} else { // iNo == 20, 21 = double
					try {
						dValue = StrToFloat(Value[i]);
					} catch ( ... ) {
						dValue = 0;
					}
				}

			if ( iNo == 17 ) RQ30Data.Signal    = iValue;
			if ( iNo == 18 ) RQ30Data.ErrorCode = iValue;
			if ( iNo == 20 ) RQ30Data.Angle     = dValue;
		}
		RQ30Data.Complete |= RQ30_ANALYSIS2;

	} else {
		// unknown command
	}

	if (RQ30Data.Complete == (RQ30_MAIN | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2)) {	// Data Complete => Send Message complete
		STRQ30Data *pRQ30Data = new STRQ30Data();
		memcpy(pRQ30Data, &RQ30Data, sizeof(STRQ30Data));
		m_pRQ30List->Add(pRQ30Data);				// Added
		LogMessage("RQ30 data buffered..");

		memset(&RQ30Data, 0x00, sizeof(STRQ30Data));	// structure clear

		// AirmarLinker에 데이터를 요청한다.(AirmarLinker가 있다면)
		if ( FormMain->m_pAirmar != NULL && FormMain->m_pAirmar->m_bLinker ) {
			FormMain->m_pAirmar->SendAirmarLinker();
		} else {
			// AirmarLinker에 대한 작업은 없음.
		}
	}

	m_TimeStamp = Now();	// TimeStamp refresh

	delete pStrList;
	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRQ30::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRQ30::Receive(AnsiString RecvMsg, int iLen)
{
	if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_RecvMsg += RecvMsg;

	int Pos = 0;
	AnsiString RecvMessage = "";

	Pos = m_RecvMsg.AnsiPos("\r\n");
	while ( Pos > 0 ) {

		RecvMessage = m_RecvMsg.SubString(1, Pos-1);
		RecvMessage = Trim(RecvMessage);

		Decode(RecvMessage, RecvMessage.Length());

		m_RecvMsg = m_RecvMsg.SubString(Pos+2, m_RecvMsg.Length() - (Pos+1));
		Pos = m_RecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRQ30::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Send
bool TRQ30::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		m_pPort->FlushOutBuffer();
		SleepMsg(100);

		m_pPort->PutString(RecvMsg);
		LogMessage("[RQ30]=>" + RecvMsg);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TRQ30::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CaculateRQ30Data();   	// RQ30 data
	Value2DB();	// Value Mapping ( include Airmar Data)

	m_RHydroMeter.MeasureDate = MeasureDate;
	m_RHydroMeter.MeasureTime = MeasureTime;
	m_RHydroMeter.HydroKind   = HydroKind;

	bool bResult = false;
	bResult = m_RHydroMeter.Insert();

	return bResult;
}

//---------------------------------------------------------------------------
// DB2Value
void TRQ30::DB2Value()
{
	// RQ30 Data
	RQ30Data.Waterlevel 	   = m_RHydroMeter.Value01;   // WaterLevel
	RQ30Data.Velocity		   = m_RHydroMeter.Value02;   // Velocity
	RQ30Data.Quality		   = m_RHydroMeter.Value03;	  // SNR
	RQ30Data.Discharge   	   = m_RHydroMeter.Value04;	  // Discharge
	RQ30Data.Area              = m_RHydroMeter.Value05;	  // Area
	RQ30Data.LearnVelocity	   = m_RHydroMeter.Value06;	  // LVelocity
	RQ30Data.LearnDischarge    = m_RHydroMeter.Value07;	  // LDischarge
	RQ30Data.OppositeContent   = m_RHydroMeter.Value08;	  // OppDirection
	RQ30Data.SupplyVoltage	   = m_RHydroMeter.Value09;	  // Voltage
	RQ30Data.PeakWidth         = m_RHydroMeter.Value10;	  // PeakWidth
	RQ30Data.CSR               = m_RHydroMeter.Value11;	  // CSR
	RQ30Data.AreaPeak          = m_RHydroMeter.Value12;	  // AreaPeak
	RQ30Data.RmsPic            = m_RHydroMeter.Value13;	  // RMS
	RQ30Data.Amp               = m_RHydroMeter.Value14;	  // Amp
	RQ30Data.AmpRelation       = m_RHydroMeter.Value15;	  // AmpRelation
	RQ30Data.Signal            = m_RHydroMeter.Value16;	  // AmpRelation
	RQ30Data.ErrorCode         = m_RHydroMeter.Value17;	  // AmpRelation

	// Airmar Data
	AirmarData.Direction       = m_RHydroMeter.Value20;	  // Degree
	AirmarData.Wind		       = m_RHydroMeter.Value21;	  // meter/sec
	AirmarData.WindGust        = m_RHydroMeter.Value22;	  // meter/sec
	AirmarData.Pressure        = m_RHydroMeter.Value23;	  // bar
	AirmarData.Temperature     = m_RHydroMeter.Value24;	  // 'C
	AirmarData.Humidity        = m_RHydroMeter.Value25;	  // %

	RQ30Data.SelfCheck         = m_RHydroMeter.Value26;   // SelfCheck
	RQ30Data.Angle             = m_RHydroMeter.Value27;   // Angle

	RQ30Data.RawData           = m_RHydroMeter.RawData;   // RQ30 RawData
}

//---------------------------------------------------------------------------
// Value2DB
void TRQ30::Value2DB()
{
	// RQ30 Data
	m_RHydroMeter.AvgVelocity = RQ30Data.Velocity;		   // Velocity
	m_RHydroMeter.MinVelocity = RQ30Data.Velocity;		   // Velocity
	m_RHydroMeter.MaxVelocity = RQ30Data.Velocity;		   // Velocity
	m_RHydroMeter.Value01     = RQ30Data.Waterlevel;   	   // WaterLevel
	m_RHydroMeter.Value02	  = RQ30Data.Velocity;         // Velocity
	m_RHydroMeter.Value03	  = RQ30Data.Quality;		   // SNR
	m_RHydroMeter.Value04	  = RQ30Data.Discharge;		   // Discharge
	m_RHydroMeter.Value05	  = RQ30Data.Area;     		   // Area
	m_RHydroMeter.Value06	  = RQ30Data.LearnVelocity;	   // LVelocity
	m_RHydroMeter.Value07	  = RQ30Data.LearnDischarge;   // LDischarge
	m_RHydroMeter.Value08	  = RQ30Data.OppositeContent;  // OppDirection
	m_RHydroMeter.Value09	  = RQ30Data.SupplyVoltage;	   // Voltage
	m_RHydroMeter.Value10	  = RQ30Data.PeakWidth;        // PeakWidth
	m_RHydroMeter.Value11	  = RQ30Data.CSR;              // CSR
	m_RHydroMeter.Value12	  = RQ30Data.AreaPeak;         // AreaPeak
	m_RHydroMeter.Value13	  = RQ30Data.RmsPic;           // RMS
	m_RHydroMeter.Value14	  = RQ30Data.Amp;              // Amp
	m_RHydroMeter.Value15	  = RQ30Data.AmpRelation;      // AmpRelation
	m_RHydroMeter.Value16	  = RQ30Data.Signal;           // Signal
	m_RHydroMeter.Value17	  = RQ30Data.ErrorCode;        // ErrorCode
	//m_RHydroMeter.Value18	  = theEnv.m_WaterOffset;	   // Offset
	m_RHydroMeter.Value18	  = 0.0f;					   // Offset 2022.05.11 RQ-30은 Offset을 적용하지 않음.

	// Airmar Data
	m_RHydroMeter.Value20	  = AirmarData.Direction;	   // Degree
	m_RHydroMeter.Value21	  = AirmarData.Wind;		   // meter/sec
	m_RHydroMeter.Value22	  = AirmarData.WindGust;       // meter/sec
	m_RHydroMeter.Value23	  = AirmarData.Pressure;       // bar
	m_RHydroMeter.Value24	  = AirmarData.Temperature;    // 'C
	m_RHydroMeter.Value25	  = AirmarData.Humidity;       // %

	m_RHydroMeter.Value26	  = RQ30Data.SelfCheck;        // SelfCheck
	m_RHydroMeter.Value27	  = RQ30Data.Angle;            // Angle

	m_RHydroMeter.RawData     = RQ30Data.RawData;          // RQ30 RawData
}

//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TRQ30::GetAvgVelocity()
{
	return 0.0f;
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30::SetBreak(bool bBreak)
{
	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
// Virtual Function
bool TRQ30::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30::SendBreak(int iMiliSecond)
{
	if ( m_pPort == NULL ) return;
	if ( m_pPort->Open == false ) return;

	m_pPort->SetBreak(true);
	SleepMsg(iMiliSecond);	// 900ms => 500ms
	m_pPort->SetBreak(false);
	//Sleep(100);			// 100ms
}

//---------------------------------------------------------------------------
// Set Airmar Data
void  TRQ30::SetAirmarData(STAirmarData AirmarData)
{
	this->AirmarData = AirmarData;
}

//---------------------------------------------------------------------------
// RQ30 데이터 중에 사용할 데이터를 정한다.
bool TRQ30::CaculateRQ30Data()
{
	int Count = 0;
	Count = m_pRQ30List->Count;

	memset(&RQ30Data, 0x00, sizeof(STRQ30Data));

	STRQ30Data *pRQ30Data = NULL;
	double V = 0.0f;

	if ( Count >= 2 ) {   	// 수집된 데이터가 2개 이상이면 정렬하여 중간값을 취한다. 단 -99일 때는 Normal Value을 따로 모아 중간값을 취한다.
		m_pRQ30List->Sort(CompareRQVelocity);	// Sort

		int iMid = m_pRQ30List->Count / 2;
		pRQ30Data = (STRQ30Data *)m_pRQ30List->Items[iMid];	// 정렬된 리스트에서 중간값을 취한다.
		RQ30Data = *pRQ30Data;

		//-------------------------------------------------------------------
		//RQ-30 데이터 선택 전략 ( 중간값이 -99일 때 이를 회피하기 위한 전략)
		// 2024.01.08 -99 => 0.0으로 변경함. 센서 설정 시 Invalid Velocity => -99 => 0.0f로 Replace함
		V = RQ30Data.Velocity;
		//if ( RQ30Data.Velocity <= -90.0f || RQ30Data.Velocity == 0.0f ) {
		if ( RQ30Data.Velocity <= -90.0f || (abs((int)(V * 100)) == 0) ) {
			int NormalCount = 0;
			STRQ30Data *pData = NULL;
			STRQ30Data *pDataArray = new STRQ30Data[m_pRQ30List->Count];   	// 임시 저장 공간 확보..

			for ( int i = 0 ; i < m_pRQ30List->Count ; i++ ) { 	// Normal Value값만 모든다.
				pData = (STRQ30Data *) m_pRQ30List->Items[i];
				V = pData->Velocity;
				//if ( pData->Velocity > -90.0f && pData->Velocity != 0.0f ) {	// Normal Value, Tide 옵션이면 -Velocity가 나올 수 있다.
				if ( pData->Velocity > -90.0f && (abs((int)(V * 100)) != 0) ) {	// Normal Value, Tide 옵션이면 -Velocity가 나올 수 있다.
					pDataArray[NormalCount++] = *pData;
				}
			}
			if ( NormalCount > 0 ) {
				int Index = NormalCount / 2;
				RQ30Data = pDataArray[Index];	// Normal value 중에 중간값을 선택한다.
			} else { // Normal Count = 0
				// 전부 -99 or 0.0f 이므로 중간값을 그대로 사요한다.
			}
			delete [] pDataArray; // Array Release
		} else {	// Velocity != -99.0f ( Normal Velocity
			// Normal 데이터로 간주한다 => 이 데이터 그대로 쓴다.
		}
	} else if ( Count == 1 ) {  	// 수집한 데이터가 1건일 때는 그냥 그걸 쓴다.
		pRQ30Data = (STRQ30Data *)m_pRQ30List->Items[0];
		RQ30Data = *pRQ30Data;
	} else {
		// Init Value
		RQ30Data.SelfCheck      = 0;     // 0
		RQ30Data.Waterlevel     = 0.0f;     // 1
		RQ30Data.Velocity       = 0.0f;     // 2
		RQ30Data.Quality        = 0.0f;	    // 3
		RQ30Data.Discharge      = 0.0f;	    // 4
		RQ30Data.Area           = 0.0f;     // 5
		RQ30Data.LearnVelocity  = 0.0f;	    // 6
		RQ30Data.LearnDischarge = 0.0f;	    // 7
		RQ30Data.OppositeContent= 0.0f;	    // 8
		RQ30Data.SupplyVoltage  = 0.0f;		  // 9
		RQ30Data.PeakWidth      = 0.0f;        // 10
		RQ30Data.CSR            = 0;           // 11
		RQ30Data.AreaPeak       = 0;           // 12
		RQ30Data.RmsPic         = 0;           // 13
		RQ30Data.Amp            = 0;           // 14
		RQ30Data.AmpRelation    = 0;           // 15
		RQ30Data.Signal         = 0;           // 16
		RQ30Data.ErrorCode      = 0;           // 17
		RQ30Data.Angle          = 0;           // 20
		RQ30Data.RawData        = "";
	}
	return true;
}

//---------------------------------------------------------------------------
// Get RQ30 Result data
STRQ30Data TRQ30::GetRQ30Data()
{
	return RQ30Data;
}

#pragma package(smart_init)


