//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "ChannelMaster.h"
#include "AniThread.h"
#include "System.RegularExpressions.hpp"

//---------------------------------------------------------------------------

int __fastcall CompareVelocity(void *Item1, void *Item2)
{
	STVelocity *A, *B;
	A = (STVelocity *)Item1;
	B = (STVelocity *)Item2;
	if ( A->Velocity1 < B->Velocity1 )       return -1;	// True  => Skip
	else if ( A->Velocity1 == B->Velocity1 ) return  0;
	else                                     return  1;	// false => Swap
}

//---------------------------------------------------------------------------
// Constructor
TChannelMaster::TChannelMaster()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = CHANNELMASTER_STR;

	CellBegin        = 0;
	CellEnd          = 0;
	CellNumber       = 0;
	CellNo           = 0;
	CellDistance     = 0;

	AvgVel      = 0;	// Average Velocity
	MinVel      = 0;	// Min Velocity
	MaxVel      = 0;	// Max Velocity
	NB          = 0;	// Number of BEAMS
	WN          = 0;	// Number of Cells
	WP          = 0;	// Pings Per Ensemble
	WS          = 0;	// Range Cell Length
	WF          = 0;	// Blank after Transmit
	WC          = 0;	// Low Corr Thresh
	WE          = 0;	// Error Velocity Maximum
	TP          = 0;	// TPP Hundredths
	EX          = 0;	// Coordinate Transform
	EZ          = 0;	// Sensor Source
	BIN1        = 0;	// Bin1Distance
	WA          = 0;	// False Target Thresh
	WB          = 0;	// System Bandwidth
	EC          = 0;	// Speed of Sound
	ED          = 0; 	// Depth of Transducer
	Pitch       = 0;	// Pitch
	ROLL        = 0;	// Roll
	ES          = 0;	// Salinity
	ET          = 0;	// Temperature
	MMinute     = 0;	// MPT Minutes
	MSecond     = 0;	// MPT Second
	MHundredths = 0;	// MPT Hundredths
	PITCHSTD    = 0;	// PITCH STD
	ROLLSTD     = 0;	// ROLL STD
	BatVolt     = 0;	// Battery Voltage in Volts
	Pressure    = 0;	// Pressure

	//m_pVelocityList = new TList();
	m_pEchoList     = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TChannelMaster::~TChannelMaster()
{
	// Resource release
	STVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		try {
			pVelocity = (STVelocity *) m_pVelocityList->Items[i];
			delete pVelocity;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pVelocityList->Clear();

	//delete m_pVelocityList; // delete THydroMeter Class

	STEcho *pEcho = NULL;
	for ( int i = 0 ; i < m_pEchoList->Count ; i++ ) {
		try {
			pEcho = (STEcho *) m_pEchoList->Items[i];
			delete pEcho;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pEchoList->Clear();

	delete m_pEchoList;
}

//---------------------------------------------------------------------------
// Clear
void TChannelMaster::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	//m_bBreak     = false;			// <BREAK> false status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = CHANNELMASTER_STR;

	m_RHydroMeter.Clear();
	m_RHydroCellContainer.Clear();

	AvgVel      = 0;	// Average Velocity
	MinVel      = 0;	// Min Velocity
	MaxVel      = 0;	// Max Velocity
	NB          = 0;	// Number of BEAMS
	WN          = 0;	// Number of Cells
	WP          = 0;	// Pings Per Ensemble
	WS          = 0;	// Range Cell Length
	WF          = 0;	// Blank after Transmit
	WC          = 0;	// Low Corr Thresh
	WE          = 0;	// Error Velocity Maximum
	TP          = 0;	// TPP Hundredths
	EX          = 0;	// Coordinate Transform
	EZ          = 0;	// Sensor Source
	BIN1        = 0;	// Bin1Distance
	WA          = 0;	// False Target Thresh
	WB          = 0;	// System Bandwidth
	EC          = 0;	// Speed of Sound
	ED          = 0; 	// Depth of Transducer
	Pitch       = 0;	// Pitch
	ROLL        = 0;	// Roll
	ES          = 0;	// Salinity
	ET          = 0;	// Temperature
	MMinute     = 0;	// MPT Minutes
	MSecond     = 0;	// MPT Second
	MHundredths = 0;	// MPT Hundredths
	PITCHSTD    = 0;	// PITCH STD
	ROLLSTD     = 0;	// ROLL STD
	BatVolt     = 0;	// Battery Voltage in Volts
	Pressure    = 0;	// Pressure

	STVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		try {
			pVelocity = (STVelocity *) m_pVelocityList->Items[i];
			delete pVelocity;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pVelocityList->Clear();

	STEcho *pEcho = NULL;
	for ( int i = 0 ; i < m_pEchoList->Count ; i++ ) {
		try {
			pEcho = (STEcho *) m_pEchoList->Items[i];
			delete pEcho;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pEchoList->Clear();

	m_RHydroCellContainer.Clear();
}

//---------------------------------------------------------------------------
// Init
void TChannelMaster::Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo)
{
	this->m_enHydroNo = enHydroNo;
	m_RHydroMeter.enHydroNo = enHydroNo;	// HydroMeter1 or not
	m_RHydroCellContainer.m_enHydroNo = enHydroNo;	// HydroMeter1Cell or not

	m_pPort = pPort;
}

//---------------------------------------------------------------------------
void TChannelMaster::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
	this->CellBegin    = CellBegin;
	this->CellEnd      = CellEnd;
	this->CellNumber   = CellNumber;
	this->CellNo       = CellNo;
	this->CellDistance = CellDistance;
}

//---------------------------------------------------------------------------
// Start
bool TChannelMaster::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	Clear();	// Status Clear
	SetBreak(false);	// <BREAK> Release

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	AnsiString sSendMsg = "CS\r";
	bool bResult = false;

	bResult = Send(sSendMsg.c_str(), sSendMsg.Length());
	if ( !bResult ) {
		LogMessage("[ChannelMaster] Start error => Send error ");
		return false;
	}

	//m_TimeStamp = Now();
	m_enStep = enSensorStart;

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TChannelMaster::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;	// 2014.09.17

	//EN_LOGGER_STEP enLoggerStep;
	//if ( m_bFirst ) enLoggerStep = enHydroMeter1Finish;
	//else            enLoggerStep = enHydroMeter2Finish;
	//
	//::PostMessage(FormMain->Handle, UM_MESSAGE, enLoggerStep, 0);

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TChannelMaster::Decode(AnsiString sCmd, int iLen)
{

	if ( sCmd == "CS" ) {

		m_enStep = enSensorWorking; 	// Sensor working
		m_enWorkingStep = enSensorStart;	// 2014.09.17

		if ( m_enHydroNo == enHydroFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterAni1);
		else                               FormMain->SetHydroMeter2Icon(enHydroMeterAni1);

		if ( FormMain->m_pAniThread != NULL ) {
			if (  m_enHydroNo == enHydroFirst ) {
				FormMain->m_pAniThread->m_bHydroMeter1 = true;
				FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterAni1;
			} else {
				FormMain->m_pAniThread->m_bHydroMeter2 = true;
				FormMain->m_pAniThread->m_HydroMeter2Index = enHydroMeterAni1;
			}
		} else {}

		FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

	} else {

		m_enStep = enSensorWorking; 	// Sensor working
		m_enWorkingStep = enSensorFinish;	// 2014.09.17

		TRegEx *ExCS      = new TRegEx("CS");
		TRegEx *ExNewLine = new TRegEx("\r\n");
		TRegEx *ExCarrage = new TRegEx("\r");
		TRegEx *ExLine    = new TRegEx("\n");
		TRegEx *ExLess    = new TRegEx(">");
		TRegEx *ExSpace   = new TRegEx(" ");
		TRegEx *ExSharp   = new TRegEx("#");
		//TRegEx *ExQuest   = new TRegEx("\?");

		//sCmd = ">CS7F7F0183054D810328408F120C3850123C840581083401AB823D481C20834081B2B0B510A750710\r\n";

		sCmd = ExCS->Replace(sCmd, "");   		// "CS"
		sCmd = ExNewLine->Replace(sCmd, "");	// "\r\n"
		sCmd = ExCarrage->Replace(sCmd, "");	// "\r"
		sCmd = ExLine->Replace(sCmd, "");		// "\n"
		sCmd = ExLess->Replace(sCmd, "");		// ">"
		sCmd = ExSpace->Replace(sCmd, "");		// " "
		sCmd = ExSharp->Replace(sCmd, "");		// "#"
		//sCmd = ExQuest->Replace(sCmd, "");		// "?"

		//sCmd = "7F7F0183054D810328408F120C3850123C840581083401AB823D481C20834081B2B0B510A750710";

		sCmd = Trim(sCmd);	// Left, Rigth => Space, \r\n, control character trim 2022.07.14

		//m_RecvMsg = sCmd;
		m_RHydroMeter.RawData       =  sCmd;	// Save RawData

		delete ExCS;
		delete ExNewLine;
		delete ExCarrage;
		delete ExLine;
		delete ExLess;
		delete ExSpace;
		delete ExSharp;
		//delete ExQuest;

		int iPosSTX = sCmd.AnsiPos("7F7F");	// heaven2 2022.07.14
		if ( iPosSTX > 1 ) {
			sCmd = sCmd.SubString(iPosSTX, sCmd.Length()-(iPosSTX-1));
			iPosSTX = sCmd.AnsiPos("7F7F");
		}

		bool bDecodeResult = false;

		if ( iPosSTX == 1 ) {

			// 일단 나중에 처리
			bDecodeResult = DecodeBBensemble(sCmd);
			if ( bDecodeResult ) {
				LogMessage("ChannelMaster Decode Routine DecodeBBensemble is complete..");
			} else {
				LogMessage("ChannelMaster DecodeBBensemble has error. Decode is not complete..");
			}

			if (  m_enHydroNo == enHydroFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterNormal);
			else                                FormMain->SetHydroMeter2Icon(enHydroMeterNormal);

			if ( FormMain->m_pAniThread != NULL ) {
				if ( m_enHydroNo == enHydroFirst ) {
					FormMain->m_pAniThread->m_bHydroMeter1 = false;
					FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterNormal;
				} else {
					FormMain->m_pAniThread->m_bHydroMeter2 = false;
					FormMain->m_pAniThread->m_HydroMeter2Index = enHydroMeterNormal;
				}
			} else {}
			FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

			::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_enHydroNo == enHydroFirst) ?  enHydroMeter1Finish :
														((m_enHydroNo == enHydroSecond) ? enHydroMeter2Finish : enHydroMeter3Finish)), 0);

			//Finish();

		} else {
			// Frame invalid => nothing to do..
		}
	}

	m_TimeStamp = Now();	// TimeStamp refresh

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TChannelMaster::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TChannelMaster::Receive(AnsiString RecvMsg, int iLen)
{
	if ( m_enStep == enSensorIdle ) return true;	// not receive status
	if ( m_bBreak ) return true;					// BREAK Status

	m_RecvMsg += RecvMsg;

	int iPos = 0;
	bool br, bn;
	br = bn = false;
	iPos = m_RecvMsg.AnsiPos("\r\n");
	if ( iPos == 0 ) {
		iPos = m_RecvMsg.AnsiPos("\r");
		if ( iPos > 0 ) {
			br = true;	// \r만 있는 상황
			bn = false;
		} else {
			br = false;
			bn = false;
			int Pos = m_RecvMsg.AnsiPos("CS");
			if ( Pos > 0 ) {
				AnsiString sCmd = m_RecvMsg.SubString(Pos, 2);
				Decode(sCmd, sCmd.Length());
				m_RecvMsg = m_RecvMsg.SubString(Pos+2, m_RecvMsg.Length() - (Pos+2));
				AnsiString Prefix = "";
				Prefix = ((m_enHydroNo == enHydroFirst)  ? "[CM1]<=" :   \
						 ((m_enHydroNo == enHydroSecond) ? "[CM2]<=" : "[CM3]<="));
				LogMessage(Prefix + sCmd);
			}
		}
	} else {
		br = bn = true;	// \r\n모두 있는 상황
	}

	AnsiString sCmd;
	int iNextPos = 0;
	while ( iPos > 0 ) {
		if ( iPos > 0 ) {	// \r\n
			sCmd = m_RecvMsg.SubString(1, iPos-1);
			if ( br && bn ) {
				iNextPos = iPos + 2;
			} else if ( br ) {
				iNextPos = iPos + 1;
			} else {
				// nothing to do.. no \r no \n
				break;
			}
			AnsiString Prefix = "";
			Prefix = ((m_enHydroNo == enHydroFirst)  ? "[CM1]<=" :   \
					 ((m_enHydroNo == enHydroSecond) ? "[CM2]<=" : "[CM3]<="));
			LogMessage(Prefix + sCmd);
		}

		Decode(sCmd, sCmd.Length());

		m_RecvMsg = m_RecvMsg.SubString(iNextPos, m_RecvMsg.Length() - (iNextPos-1));  // abc\r\n>
		//if ( !m_RecvMsg.IsEmpty() ) LogMessage(m_RecvMsg, false);
		//m_RecvMsg = m_RecvMsg.Trim();	// trim \r\n<body>\r\n
		iPos = m_RecvMsg.AnsiPos("\r\n");
		if ( iPos == 0 ) {
			iPos = m_RecvMsg.AnsiPos("\r");
			if ( iPos > 0 ) {
				br = true;	// \r만 있는 상황
				bn = false;
			} else {
				br = false;
				bn = false;
				int Pos = m_RecvMsg.AnsiPos("CS");
				if ( Pos > 0 ) {
					AnsiString sCmd = m_RecvMsg.SubString(Pos, 2);
					Decode(sCmd, sCmd.Length());
					m_RecvMsg = m_RecvMsg.SubString(Pos+2, m_RecvMsg.Length() - (Pos+2));
					AnsiString Prefix = "";
					Prefix = ((m_enHydroNo == enHydroFirst)  ? "[CM1]<=" :   \
							 ((m_enHydroNo == enHydroSecond) ? "[CM2]<=" : "[CM3]<="));
					LogMessage(Prefix + sCmd);
					break;
				}
			}
		} else {
			br = bn = true;	// \r\n 존재
		}
	}

	return true;
}


//---------------------------------------------------------------------------
// Receive
bool TChannelMaster::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}


//---------------------------------------------------------------------------
// Send
bool TChannelMaster::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		m_pPort->PutString(RecvMsg);
		LogMessage("[ChannelMaster]=>" + RecvMsg);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TChannelMaster::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CalAvgMinMaxVelocity();
	Value2DB();	// Value Mapping

	m_RHydroMeter.MeasureDate = MeasureDate;
	m_RHydroMeter.MeasureTime = MeasureTime;
	m_RHydroMeter.HydroKind   = HydroKind;

	//m_RHydroMeterCell.MeasureDate = MeasureDate;
	//m_RHydroMeterCell.MeasureTime = MeasureTime;

	bool bResult = false;
	bResult = m_RHydroMeter.Insert();

	if ( !bResult ) return bResult;

	bResult = m_RHydroCellContainer.Save(MeasureDate, MeasureTime);

	return bResult;
}

//---------------------------------------------------------------------------
// DB2Value
void TChannelMaster::DB2Value()
{
	AvgVel      = m_RHydroMeter.AvgVelocity;// Average Velocity
	MinVel      = m_RHydroMeter.MinVelocity;// Min Velocity
	MaxVel      = m_RHydroMeter.MaxVelocity;// Max Velocity
	NB          = m_RHydroMeter.Value01;	// Number of BEAMS
	WN          = m_RHydroMeter.Value02;	// Number of Cells
	WP          = m_RHydroMeter.Value03;	// Pings Per Ensemble
	WS          = m_RHydroMeter.Value04;	// Range Cell Length
	WF          = m_RHydroMeter.Value05;	// Blank after Transmit
	WC          = m_RHydroMeter.Value06;	// Low Corr Thresh
	WE          = m_RHydroMeter.Value07;	// Error Velocity Maximum
	TP          = m_RHydroMeter.Value08;	// TPP Hundredths
	EX          = m_RHydroMeter.Value09;	// Coordinate Transform
	EZ          = m_RHydroMeter.Value10;	// Sensor Source
	BIN1        = m_RHydroMeter.Value11;	// Bin1Distance
	WA          = m_RHydroMeter.Value12;	// False Target Thresh
	WB          = m_RHydroMeter.Value13;	// System Bandwidth
	EC          = m_RHydroMeter.Value14;	// Speed of Sound
	ED          = m_RHydroMeter.Value15; 	// Depth of Transducer
	Pitch       = m_RHydroMeter.Value16;	// Pitch
	ROLL        = m_RHydroMeter.Value17;	// Roll
	ES          = m_RHydroMeter.Value18;	// Salinity
	ET          = m_RHydroMeter.Value19;	// Temperature
	MMinute     = m_RHydroMeter.Value20;	// MPT Minutes
	MSecond     = m_RHydroMeter.Value21;	// MPT Second
	MHundredths = m_RHydroMeter.Value22;	// MPT Hundredths
	PITCHSTD    = m_RHydroMeter.Value23;	// PITCH STD
	ROLLSTD     = m_RHydroMeter.Value24;	// ROLL STD
	BatVolt     = m_RHydroMeter.Value25;	// Battery Voltage in Volts
	Pressure    = m_RHydroMeter.Value26;	// Pressure
}

//---------------------------------------------------------------------------
// Value2DB
void TChannelMaster::Value2DB()
{
	m_RHydroMeter.AvgVelocity   =  AvgVel;  // Average Velocity
	m_RHydroMeter.MinVelocity   =  MinVel;  // Min Velocity
	m_RHydroMeter.MaxVelocity   =  MaxVel;  // Max Velocity
	m_RHydroMeter.Value01	    =  NB;      // Number of BEAMS
	m_RHydroMeter.Value02	    =  WN;      // Number of Cells
	m_RHydroMeter.Value03	    =  WP;      // Pings Per Ensemble
	m_RHydroMeter.Value04	    =  WS;      // Range Cell Length
	m_RHydroMeter.Value05	    =  WF;      // Blank after Transmit
	m_RHydroMeter.Value06	    =  WC;      // Low Corr Thresh
	m_RHydroMeter.Value07	    =  WE;      // Error Velocity Maximum
	m_RHydroMeter.Value08	    =  TP;      // TPP Hundredths
	m_RHydroMeter.Value09	    =  EX;      // Coordinate Transform
	m_RHydroMeter.Value10	    =  EZ;      // Sensor Source
	m_RHydroMeter.Value11	    =  BIN1;    // Bin1Distance
	m_RHydroMeter.Value12	    =  WA;      // False Target Thresh
	m_RHydroMeter.Value13	    =  WB;      // System Bandwidth
	m_RHydroMeter.Value14	    =  EC;      // Speed of Sound
	m_RHydroMeter.Value15 	    =  ED;      // Depth of Transducer
	m_RHydroMeter.Value16	    =  Pitch;   // Pitch
	m_RHydroMeter.Value17	    =  ROLL;    // Roll
	m_RHydroMeter.Value18	    =  ES;      // Salinity
	m_RHydroMeter.Value19	    =  ET;      // Temperature
	m_RHydroMeter.Value20	    =  MMinute; // MPT Minutes
	m_RHydroMeter.Value21	    =  MSecond; // MPT Second
	m_RHydroMeter.Value22	    =  MHundredths;// MPT Hundredthshs;
	m_RHydroMeter.Value23	    =  PITCHSTD;// PITCH STD
	m_RHydroMeter.Value24	    =  ROLLSTD; // ROLL STD
	m_RHydroMeter.Value25	    =  BatVolt; // Battery Voltage in Volts
	m_RHydroMeter.Value26	    =  Pressure;// Pressure
//	m_RHydroMeter.RawData       =  m_RecvMsg;
}


//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TChannelMaster::GetAvgVelocity()
{
	double Velocity  = 0;
	int    StartCell = 0;
	int    EndCell   = 0;
	bool   bReverse  = false;

	if ( m_enHydroNo == enHydroFirst ) {
		StartCell =  m_pRFlowADVM->AStartCell;
		EndCell   =  m_pRFlowADVM->AEndCell;
		bReverse  = (m_pRFlowADVM->AReverseMode == 0 ) ? true : false;
	} else {
		StartCell =  m_pRFlowADVM->BStartCell;
		EndCell   =  m_pRFlowADVM->BEndCell;
		bReverse  = (m_pRFlowADVM->BReverseMode == 0 ) ? true : false;
	}

	if ( EndCell > m_pVelocityList->Count ) EndCell = m_pVelocityList->Count;

	int CellCount = EndCell - StartCell;

	if ( CellCount > 5 ) {	// Cell이 5 이상 => 중간값 사용

		m_pVelocityList->Sort(CompareVelocity);	// Sort

		int iMid = m_pVelocityList->Count / 2;
		STVelocity *pVelocity = NULL;
		pVelocity = (STVelocity *)m_pVelocityList->Items[iMid];
		Velocity = (bReverse) ? pVelocity->Velocity1 * -1 : pVelocity->Velocity1;

	} else {	// Cell이 5 이하=> 평균값 사용

		double Sum = 0;
		STVelocity *pVelocity = NULL;

		for ( int i = StartCell-1 ; i < EndCell - 1 ; i++ ) {
			try {
				pVelocity = (STVelocity *)m_pVelocityList->Items[i];
				Sum += (bReverse) ? (pVelocity->Velocity1 * -1) : (pVelocity->Velocity1);
			} catch ( Exception &e ) {
				break;
			}
		}

		Velocity = Sum / CellCount;
	}

	return Velocity * 0.001;   	// mm/s => m/s로 변환하여 Return
}

//---------------------------------------------------------------------------
void TChannelMaster::SetBreak(bool bBreak)
{
	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
bool TChannelMaster::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
void TChannelMaster::SendBreak(int iMiliSecond)	// Send <BREAK> to ComPort
{
	if ( m_pPort == NULL ) return;
	if ( m_pPort->Open == false ) return;

	m_pPort->SetBreak(true);
	SleepMsg(iMiliSecond);	// 900ms => 500ms
	m_pPort->SetBreak(false);
	//Sleep(100);			// 100ms
}

//---------------------------------------------------------------------------
// DecodeFrame
bool TChannelMaster::DecodeBBensemble(AnsiString sFrame)
{

	INT nADCPLen = 0;
	BYTE RcvBuff[UART_BUFFER];
	memset(RcvBuff,0x00,UART_BUFFER);
	BYTE nHexRow[3];
	AnsiString HexStr;
	bool bErrorFlag = false;	// Error Flag

	for( int i = 0 ; i < sFrame.Length() ; i += 2 )
	{
		HexStr = sFrame.SubString(i+1,2);
		HexStr = "0x" + HexStr;
		try {
			RcvBuff[nADCPLen] = StrToInt(HexStr);
		} catch ( Exception &e ) {
			LogMessage("DecodeBBensemble()=>" + e.Message);
			bErrorFlag = true;
			break;
		}
		//RcvBuff[nADCPLen] = strtol((CHAR*)nHexRow,NULL, 16);
		nADCPLen++;
	}

	if ( bErrorFlag ) return false;	// decoding error 2018.01.08

	unsigned long fDepth = 0;
	INT nWN = 0;
	//short nTemp = 0;

	int nIndex = 0;
	while(nIndex < nADCPLen  )
	{
		unsigned short nCMD = 0;
		memcpy(&nCMD,&RcvBuff[nIndex],2);

		switch( nCMD )
		{
			case 0x7F7F: //ADCPHeader
				{
					int numData = RcvBuff[nIndex + 5];
					nIndex = (numData * 2) + 6;

				}
				break;
			case 0X0000: //ADCPFixedLeader
				{
					NB = RcvBuff[nIndex+8];				// Number of Beams(NB)
					WN = RcvBuff[nIndex+9];				// number of cell(WN)
					nWN = WN;
					memcpy(&WP, &RcvBuff[nIndex+10],2);	// Pings perensemble(WP)
					memcpy(&WS, &RcvBuff[nIndex+12],2);	// Range Cell Length(WS)
					memcpy(&WF, &RcvBuff[nIndex+14],2);	// Blank after Transmit(WF)
					WC = RcvBuff[nIndex+17]; 			// Low corr thresh(WC)
					memcpy(&WE, &RcvBuff[nIndex+20],2);	// Error Velocity Maximum(WE)
					TP = RcvBuff[nIndex+24];			// TPP Hundredths(TP)
					EX = RcvBuff[nIndex+25];			// Coordinate Transform(EX)
					EZ = RcvBuff[nIndex+30];			// Sensor Source(EZ)
					memcpy(&BIN1, &RcvBuff[nIndex+32],2);	// Bin 1 Distance
					WA = RcvBuff[nIndex+38];			// False Target Thresh(WA)
					WB = RcvBuff[nIndex+50];			// System Bandwith(WB)

					//nWN =  RcvBuff[nIndex + 9];
					//CString sWN;
					//sWN.Format("%d",nWN);
					//AfxMessageBox(sWN);
					nIndex += 58;
				}
				break;
			case 0x0080 : //ADCPVariableLeader
				{
					memcpy(&EC,    &RcvBuff[nIndex+14],2);	// Speed of Sound(EC)
					memcpy(&ED,    &RcvBuff[nIndex+16],2);	// Depth of Transducer
					memcpy(&Pitch, &RcvBuff[nIndex+20],2);	// Pitch
					memcpy(&ROLL,  &RcvBuff[nIndex+22],2);	// ROLL
					memcpy(&ES,    &RcvBuff[nIndex+24],2);	// Salinity(ES)
					memcpy(&ET,    &RcvBuff[nIndex+26],2);	// Temperature(ET)
					MMinute = RcvBuff[nIndex+28];			// MPT Minutes
					MSecond = RcvBuff[nIndex+29];			// MPT Seconds
					MHundredths = RcvBuff[nIndex+30];		// MPT Hundredths
					PITCHSTD = RcvBuff[nIndex+32];			// Pitch Std
					ROLLSTD  = RcvBuff[nIndex+33];			// Roll Std
					BatVolt  = RcvBuff[nIndex+35];			// Battery Voltage in Volts
					memcpy(&Pressure, &RcvBuff[nIndex+48],4);	// Pressure


					//memcpy(&nTemp,&RcvBuff[nIndex + 26],2);
					//CString sTemp;
					//sTemp.Format("%3.2f",nTemp * 0.01);
					//AfxMessageBox(sTemp);
					nIndex += 60;
				}
				break;
			case 0x0100: //ADCPVelocityData
				{
					// Header
					nIndex += 2;

					for (int i = 0; i < nWN ; i++)
					{
						STVelocity *pVelocity = new STVelocity();
						pVelocity->Velocity1 = 0;
						pVelocity->Velocity2 = 0;

						memcpy(&(pVelocity->Velocity1), &RcvBuff[nIndex],2);
						nIndex += 2;
						memcpy(&(pVelocity->Velocity2), &RcvBuff[nIndex],2);
						nIndex += 2;
						//reserved
						nIndex += 4;

						m_pVelocityList->Add(pVelocity);

						//stVelocity tVeloctiy;
						//tVeloctiy.nVelocity1 = 0;
						//tVeloctiy.nVelocity2 = 0;
						//
						//memcpy(&tVeloctiy.nVelocity1,&RcvBuff[nIndex],2);
						//nIndex += 2;
						//memcpy(&tVeloctiy.nVelocity2,&RcvBuff[nIndex],2);
						//nIndex += 2;
						//// Reserved
						//nIndex += 4;
						//
						//m_ListVelocity.AddTail(tVeloctiy);

					}

				}
				break;
			case 0x0200: //ADCPCorrelationData
				{
					nIndex += 2;
					for (int i = 0; i < nWN ; i++)
					{
						nIndex += 4;
					}
				}
				break;
			case 0x0300: //ADCPEchoIntensityData
				{
					// Header
					nIndex += 2;
					for (int i = 0; i < nWN ; i++)
					{
						STEcho *pEcho = new STEcho();
						pEcho->Echo1 = RcvBuff[nIndex];
						nIndex++;
						pEcho->Echo2 = RcvBuff[nIndex];
						nIndex++;
						//Reserved
						nIndex += 2;

						m_pEchoList->Add(pEcho);

						//stEcho tEcho;
						//tEcho.EchoIntensity1 = RcvBuff[nIndex];
						//nIndex++;
						//tEcho.EchoIntensity2 = RcvBuff[nIndex];
						//nIndex++;
						//
						//// Reserved
						//nIndex += 2;
						//
						//m_ListEcho.AddTail(tEcho);
					}
				}
				break;

			case 0x0400: //ADCPPercentGoodData
				{
					nIndex += 2;
					for (int i = 0; i < nWN ; i++)
					{
						nIndex += 4;
					}

				}
				break;
			case 0x0500: //ADCPStatusData
				{
					nIndex += 2;
					for (int i = 0; i < nWN ; i++)
					{
						nIndex += 4;
					}
				}
				break;
			case 0x4002: //ADCPSurfaceTrackAmplitude
				{
					// Header
					nIndex += 2;

					// Pings in Burst (NPing)
					byte nping = RcvBuff[nIndex];
					nIndex++;

					// Bin Location of Surface (Avg.)
					nIndex += 2;

					// Filter Evaluation Amp (Avg.)
					nIndex++;

					// Amplitude at Surface (Avg.)
					nIndex++;

					// W Filter Bin Location of Surface (Avg.)
					nIndex += 2;

					// W Filter Evaluation Amp (Avg.)
					nIndex++;

					// W Filter Amplitude at Surface (Avg.)
					nIndex++;

					// Leading Edge Filter Bin Location of Surface (Avg.)
					nIndex += 2;

					// Leading Edge Filter Evaluation Amp (Avg.)
					nIndex++;

					// Leading Edge Amplitude at Surface (Avg.)
					nIndex++;

					// W Filter Bin Location (Indivisual Pings)
					nIndex += (2 * nping);

					// W Filter Evaluation Amplitude (Indivisual Pings)
					nIndex +=nping;

					// W Filter Amplitude (Indivisual Pings)
					nIndex +=nping;

					// Status of Indivisual Pings
					nIndex +=nping;

				}
				break;
			case 0x4001: //ADCPSurfaceTrackCommands
				{
					// Header
					nIndex += 2;

					// VM_Blank
					nIndex += 2;

					// VM_Pings
					nIndex += 2;

					// VM_BW
					nIndex++;

					// VM_DetectMode
					nIndex++;

					// VM_PressScreen
					nIndex++;

					// VM_RangeScreen
					nIndex++;

					// VM_EdgeDetectThres
					nIndex++;

					// VM_EdgeDetectDelta
					nIndex += 2;

					// VM_RcvrGain
					nIndex++;

					// VM_OffsetTenthsMM
					nIndex += 2;

					// VM_ScalePPM
					nIndex += 2;

					// VM_MinRange
					nIndex += 2;

					// VM_MaxRange
					nIndex += 2;

					// VM_XmtLength
					nIndex += 2;

					// VM_WThreshold
					nIndex++;

					// VM_WWidth
					nIndex += 2;

					// VM_XmitPwr
					nIndex++;

				}
				break;
			case 0x4000: //ADCPSurfaceTrackStatus
				{
					// Header
					nIndex += 2;

					// Depth (Corrected Surface Track)
					memcpy(&fDepth,&RcvBuff[nIndex],4);
					nIndex += 4;

					// Depth (Uncorrected Surface Track)
					nIndex += 4;

					// Evaluation Amplitude
					nIndex++;

					// Amplitude at the Surface
					nIndex++;

					// Good of Surface Track
					nIndex++;

					// STD Surface Track
					nIndex += 4;

					// Min Surface Track
					nIndex += 4;

					// Max Surface Track
					nIndex += 4;

					// Pressure Depth Correction
					nIndex += 4;

					// Depth (Uncorrected Pressure)
					nIndex += 4;

					// Percent Good of Depth based on Pressure
					nIndex++;

					// STD of Depth based on Corrected Pressure
					nIndex += 4;

					// Min of Depth based on Corrected Pressure
					nIndex += 4;

					// Max of Depth based on Corrected Pressure
					nIndex += 4;

				}
				break;

			case 0x0002: //ADCPCorrelationData
				{
					nIndex += 2;

					for (int i = 0; i < nWN ; i++)
					{
						nIndex += 4;
					}
				}
				break;
			case 0x0012: //ADCPFirmwareVersion
				{
					// Header
					nIndex += 2;

					// Line Feed
					nIndex++;

					// Product Version
					nIndex += 8;

					// CPU Firmware
					nIndex += 6;

					// FPGA Firmware
					nIndex += 9;

					// Sensor Boot
					nIndex += 6;

					// Sensor Firmware
					nIndex += 6;

					if ( RcvBuff[nIndex] == 0x00)
					{
						// Terminal NULL
						nIndex++;
						break;
					}

					// SDI-12 Boot
					nIndex += 6;

					// SDI-12 Firmware
					nIndex += 5;

					// Terminal NULL
					nIndex++;
				}
				break;
			default:
				nIndex = nADCPLen ;
				break;
		}
	}

	// HydroMeterCell Value
	STVelocity *pVelocity = NULL;
	STEcho     *pEcho     = NULL;
	TRHydroMeterCell *pCell = NULL;
	for ( int i = 0 ; i < nWN ; i++ ) {
		try {
			pVelocity = (STVelocity *)m_pVelocityList->Items[i];
			pEcho     = (STEcho     *)m_pEchoList->Items[i];
		} catch ( Exception &e) {
			break;
		}

		pCell = new TRHydroMeterCell(m_RHydroMeter.enHydroNo);
		pCell->MeasureDate = MeasureDate;
		pCell->MeasureTime = MeasureTime;
		pCell->CellNo      = i+1;
		pCell->Value01 = pVelocity->Velocity1;
		pCell->Value02 = pVelocity->Velocity2;
		pCell->Value03 = pEcho->Echo1;
		pCell->Value04 = pEcho->Echo2;

		m_RHydroCellContainer.Add(pCell);
	}

	return true;
}


//---------------------------------------------------------------------------
// CalAvgMinMaxVelocity
bool TChannelMaster::CalAvgMinMaxVelocity()
{
	double Avg, Min, Max, Sum;
	Avg = Min = Max = Sum = 0;
	bool bFirst = true;

	int StartCell, EndCell;
	StartCell = EndCell = 0;
	StartCell = (m_enHydroNo == enHydroFirst)  ? theEnv.m_StartCell1 :
				(m_enHydroNo == enHydroSecond) ? theEnv.m_StartCell2 : theEnv.m_StartCell3;
	EndCell   = (m_enHydroNo == enHydroFirst)  ? theEnv.m_EndCell1   :
				(m_enHydroNo == enHydroSecond) ? theEnv.m_StartCell2  : theEnv.m_StartCell3;
	if ( StartCell == 0 ) StartCell =  1;	// 만약을 위한 코드(Default: 1 ~ 10 cell)
	if ( EndCell   == 0 ) EndCell   = 10;   // 만약을 위한 코드

	STVelocity *pVelocity = NULL;
	int iValidCellCount = 0;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		try {
			pVelocity = (STVelocity *) m_pVelocityList->Items[i];
		} catch ( Exception &e ) {
			break;
		}

		if ( i+1 >= StartCell && i+1 <= EndCell ) {

			if ( pVelocity->Velocity1 < theEnv.m_dHydroLowerBound ||
				 pVelocity->Velocity1 > theEnv.m_dHydroUpperBound ) continue;	// 유효범위 이외의 값이 오면 처리하지 않는다.

			if ( bFirst ) {
				Min = pVelocity->Velocity1;
				Max = pVelocity->Velocity1;
				bFirst = false;
			}

			if ( pVelocity != NULL ) {
				Sum += pVelocity->Velocity1;
				iValidCellCount++;
			}

			if ( abs(pVelocity->Velocity1) < abs(Min) ) {   	// -값이라 하더라도 거꾸로 설치된 것일 수 있으므로 절대값으로 계산
				Min = pVelocity->Velocity1;
			} else {}

			if ( abs(pVelocity->Velocity1) > abs(Max) ) {   // -값이라 하더라도 거꾸로 설치된 것일 수 있으므로 절대값으로 계산
				Max = pVelocity->Velocity1;
			} else {}

		} else {
			// 유효셀 범위 밖이므로 Avg/Min/Max에서 제외한다.
		}
	}

	// 2014.09.17 : 처음에는 셀 전체의 평균값을 사용하였으나
	// 유효범위 안의 셀만을 이용하여 셀유속을 구한다.
	if ( iValidCellCount > 0 ) {
		Avg = Sum / iValidCellCount;
	} else {
		Avg = 0;
	}

	AvgVel = Avg;
	MinVel = Min;
	MaxVel = Max;

	return true;
}




#pragma package(smart_init)
