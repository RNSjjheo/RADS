//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "AG1500K.h"
#include "AniThread.h"
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------

int __fastcall CompareVelocityAG(void *Item1, void *Item2)
{
	STAGVelocity *A, *B;
	A = (STAGVelocity *)Item1;
	B = (STAGVelocity *)Item2;
	if ( A->VelocityX < B->VelocityX )       return -1;	// True  => Skip
	else if ( A->VelocityX == B->VelocityX ) return  0;
	else                                     return  1;	// false => Swap
}


//---------------------------------------------------------------------------
// Constructor
TAG1500K::TAG1500K()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = AGSERIES_STR;

	CellBegin        = 0;
	CellEnd          = 0;
	CellNumber       = 0;
	CellNo           = 0;
	CellDistance     = 0;

	AvgVel           = 0;	// AvgVelocity
	MinVel           = 0;	// MinVelocity
	MaxVel           = 0;	// MaxVelocity
	V1               = 0;	// Velocity component 1
	V2               = 0;	// Velocity component 2
	V3               = 0;	// Velocity component 3
	ErrorOfVelocity1 = 0;	// Standard error of velocity 1
	ErrorOfVelocity2 = 0;	// Standard error of velocity 2
	ErrorOfVelocity3 = 0;	// Standard error of velocity 3
	SignalStrength1  = 0;	// Signal Strength 1
	SignalStrength2  = 0;	// Signal Strength 2
	SignalStrength3  = 0;	// Signal Strength 3
	PercentGoodPings = 0;	// PercentGoodPings
	Heading          = 0;	// Heading
	Pitch            = 0;	// Pitch
	Roll             = 0;	// Roll
	DeviationHeading = 0;	// Standard deviation heading
	DeviationPitch   = 0;	// Standard deviation pitch
	DeviationRoll    = 0;	// Standard deviation roll
	MeanTemperature  = 0;	// Mean Temperature
	MeanPressure     = 0;	// Mean Pressure
	DeviationPressure= 0;	// Standard deviation of pressure
	InputPowerLevel  = 0;	// Input power Level
	StartLocation    = 0;	// Starting location of sampling volume
	EndLocation      = 0;	// Ending location of sampling volume
	NoiseLevel1      = 0;	// Noise Level(Beam 1)
	NoiseLevel2      = 0;	// Noise Level(Beam 2)
	NoiseLevel3      = 0;	// Noise Level(Beam 3)

	//m_pVelocityList = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TAG1500K::~TAG1500K()
{
	STAGVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STAGVelocity *) m_pVelocityList->Items[i];
		delete pVelocity;
	}
	m_pVelocityList->Clear();

	//delete m_pVelocityList;  //Delete THydroMeter Class
}

//---------------------------------------------------------------------------
// Clear
void TAG1500K::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	//m_bBreak     = false;			// <BREAK> false status
	m_bBreakResponse = false;		// Break Response Clear
	m_bFlooding   = false; 			// 수신데이다 Flooding 상화
	m_iThrowCount = 0;              // 수신데이터 버려지는 횟수

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = AGSERIES_STR;

	AvgVel           = 0;	// AvgVelocity
	MinVel           = 0;	// MinVelocity
	MaxVel           = 0;	// MaxVelocity
	V1               = 0;	// Velocity component 1
	V2               = 0;	// Velocity component 2
	V3               = 0;	// Velocity component 3
	ErrorOfVelocity1 = 0;	// Standard error of velocity 1
	ErrorOfVelocity2 = 0;	// Standard error of velocity 2
	ErrorOfVelocity3 = 0;	// Standard error of velocity 3
	SignalStrength1  = 0;	// Signal Strength 1
	SignalStrength2  = 0;	// Signal Strength 2
	SignalStrength3  = 0;	// Signal Strength 3
	PercentGoodPings = 0;	// PercentGoodPings
	Heading          = 0;	// Heading
	Pitch            = 0;	// Pitch
	Roll             = 0;	// Roll
	DeviationHeading = 0;	// Standard deviation heading
	DeviationPitch   = 0;	// Standard deviation pitch
	DeviationRoll    = 0;	// Standard deviation roll
	MeanTemperature  = 0;	// Mean Temperature
	MeanPressure     = 0;	// Mean Pressure
	DeviationPressure= 0;	// Standard deviation of pressure
	InputPowerLevel  = 0;	// Input power Level
	StartLocation    = 0;	// Starting location of sampling volume
	EndLocation      = 0;	// Ending location of sampling volume
	NoiseLevel1      = 0;	// Noise Level(Beam 1)
	NoiseLevel2      = 0;	// Noise Level(Beam 2)
	NoiseLevel3      = 0;	// Noise Level(Beam 3)

	STAGVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STAGVelocity *) m_pVelocityList->Items[i];
		delete pVelocity;
	}
	m_pVelocityList->Clear();

	m_RHydroCellContainer.Clear();
}

//---------------------------------------------------------------------------
// Init
void TAG1500K::Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo)
{
	this->m_enHydroNo = enHydroNo;
	m_RHydroMeter.enHydroNo = enHydroNo;	// HydroMeter1 or not
	m_RHydroCellContainer.m_enHydroNo = enHydroNo;	// HydroMeter1Cell or not

	m_pPort = pPort;
}

//---------------------------------------------------------------------------
void TAG1500K::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
	this->CellBegin    = CellBegin;
	this->CellEnd      = CellEnd;
	this->CellNumber   = CellNumber;
	this->CellNo       = CellNo;
	this->CellDistance = CellDistance;
}

//---------------------------------------------------------------------------
// Start
bool TAG1500K::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	Clear();	// Status Clear
	SetBreak(false);	// <BREAK> Release

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	AnsiString sSendMsg = "O\r";
	bool bResult = false;

	bResult = Send(sSendMsg.c_str(), sSendMsg.Length());
	if ( !bResult ) {
		LogMessage("[AG1500K] Start error => send error");
		return false;
	}

	//m_TimeStamp = Now();
	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	if ( m_enHydroNo == enHydroFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterAni3);
	else                               FormMain->SetHydroMeter2Icon(enHydroMeterAni3);

	if ( FormMain->m_pAniThread != NULL ) {
		if ( m_enHydroNo == enHydroFirst ) {
			FormMain->m_pAniThread->m_bHydroMeter1 = true;
			FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterAni3;
		} else {
			FormMain->m_pAniThread->m_bHydroMeter2 = true;
			FormMain->m_pAniThread->m_HydroMeter2Index = enHydroMeterAni3;
		}
	} else {}
	FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TAG1500K::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;

	return bResult;
}

//---------------------------------------------------------------------------
// Decode
bool TAG1500K::Decode(AnsiString sCmd, int iLen)
{
	if ( m_enStep == enSensorIdle ) { // 에러 상황으로 Next Step로 넘어가기 위해 finish 메세지를 준다.
		::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_enHydroNo == enHydroFirst) ?  enHydroMeter1Finish :
													((m_enHydroNo == enHydroSecond) ? enHydroMeter2Finish : enHydroMeter3Finish)), 0);
		return true;	// not receive status
	}

	if ( sCmd.IsEmpty() ) {    	// 에러 상황으로 Next Step로 넘어가기 위해 finish 메세지를 준다.
		LogMessage("AG1500K decode.. but data is empty..");
		::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_enHydroNo == enHydroFirst) ?  enHydroMeter1Finish :
													((m_enHydroNo == enHydroSecond) ? enHydroMeter2Finish : enHydroMeter3Finish)), 0);
		return true;
	}

	//LogMessage(AnsiString("[AG1500K] Decode() <=") + sCmd);

	TStringList *StrLst = new TStringList();
	TRegEx      *Ex     = new TRegEx("\r\n");
	AnsiString   ss;

	try {
		ss = Ex->Replace(sCmd, "  ");
		delete Ex;
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		delete Ex;
	}

	m_RHydroMeter.RawData = ss;	// heaven2 2016.06.07 Raw Data Save

	// 구분자를 ','로 설정한다.
	StrLst->Delimiter     = ' '; 	// [Space], "," 가능
	StrLst->DelimitedText = ss;

	//구분자가 콤마(',')인 경우 위 2줄 코딩을 빼고 아래처럼..
	//StrLst->CommaText = sCmd;
	// To Do here ....
	//int a = StrLst->Strings[0];
	if ( StrLst->Count > 30 ) {
		try {
			V1				  = StrToInt(Trim(StrLst->Strings[6]));
			V2				  = StrToInt(Trim(StrLst->Strings[7]));
			V3				  = StrToInt(Trim(StrLst->Strings[8]));
			ErrorOfVelocity1  = StrToInt(Trim(StrLst->Strings[9]));
			ErrorOfVelocity2  = StrToInt(Trim(StrLst->Strings[10]));
			ErrorOfVelocity3  = StrToInt(Trim(StrLst->Strings[11]));
			SignalStrength1	  = StrToInt(Trim(StrLst->Strings[12]));
			SignalStrength2	  = StrToInt(Trim(StrLst->Strings[13]));
			SignalStrength3	  = StrToInt(Trim(StrLst->Strings[14]));
			PercentGoodPings  = StrToInt(Trim(StrLst->Strings[15]));
			Heading			  = StrToInt(Trim(StrLst->Strings[16]));
			Pitch			  = StrToInt(Trim(StrLst->Strings[17]));
			Roll			  = StrToInt(Trim(StrLst->Strings[18]));
			DeviationHeading  = StrToInt(Trim(StrLst->Strings[19]));
			DeviationPitch	  = StrToInt(Trim(StrLst->Strings[20]));
			DeviationRoll	  = StrToInt(Trim(StrLst->Strings[21]));
			MeanTemperature	  = StrToInt(Trim(StrLst->Strings[22]));
			MeanPressure      = StrToInt(Trim(StrLst->Strings[23]));
			DeviationPressure = StrToInt(Trim(StrLst->Strings[24]));
			InputPowerLevel   = StrToInt(Trim(StrLst->Strings[25]));
			StartLocation     = StrToInt(Trim(StrLst->Strings[26]));
			EndLocation       = StrToInt(Trim(StrLst->Strings[27]));
			NoiseLevel1       = StrToInt(Trim(StrLst->Strings[28]));
			NoiseLevel2       = StrToInt(Trim(StrLst->Strings[29]));
			NoiseLevel3       = StrToInt(Trim(StrLst->Strings[30]));
		} catch (Exception &e ) {
			LogMessage(e.Message);
		}
	} else {
		// nothing to do.. 수신한 데이터가 짧거나 비정상적인 데이터로 간주됨.
		LogMessage("AG1500K decode.. data is too short or invalid data..");
	}

	int iCellNo   = CellNo; 	// Cell No in Setup
	int ItemCount = StrLst->Count;	// String item count
	int Pos       = 31;	// First CellNo
	int iIndex    = 0;

	STAGVelocity *pVelocity = NULL;
	TRHydroMeterCell *pCell = NULL;
	//while ( i < CellNo ) {
	while ( Pos < ItemCount ) {
		pVelocity = new STAGVelocity();
		pCell     = new TRHydroMeterCell(m_RHydroMeter.enHydroNo);

		try {
			pVelocity->CellNo      = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->VelocityX   = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->VelocityY   = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->StdErrVelX  = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->StdErrVelY  = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->SignalBeam1 = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->SignalBeam2 = StrToInt(Trim(StrLst->Strings[Pos++]));

			//pVelocity->CellNo      = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 0]));
			//pVelocity->VelocityX   = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 1]));
			//pVelocity->VelocityY   = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 2]));
			//pVelocity->StdErrVelX  = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 3]));
			//pVelocity->StdErrVelY  = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 4]));
			//pVelocity->SignalBeam1 = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 5]));
			//pVelocity->SignalBeam2 = StrToInt(Trim(StrLst->Strings[31 + (i*7) + 6]));

			pCell->MeasureDate     = MeasureDate;
			pCell->MeasureTime     = MeasureTime;
			pCell->CellNo          = pVelocity->CellNo;
			pCell->Value01         = pVelocity->VelocityX;
			pCell->Value02         = pVelocity->VelocityY;
			pCell->Value03         = pVelocity->StdErrVelX;
			pCell->Value04         = pVelocity->StdErrVelY;
			pCell->Value05         = pVelocity->SignalBeam1;
			pCell->Value06         = pVelocity->SignalBeam2;

			m_RHydroCellContainer.Add(pCell);

		} catch ( Exception &e ) {
			LogMessage(e.Message);
			break;
		}

		m_pVelocityList->Add(pVelocity);
		iIndex++;

		if ( iIndex >= iCellNo ) break;	// Exit parsing routine
		// 왜 이러나면 Data가 'x,x,x,x,'형태로 끝나서 ItemCount가 하나가 많아서 이렇게 처리함.

	}

	m_enWorkingStep = enSensorFinish;
	if ( m_enHydroNo == enHydroFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterNormal);
	else                               FormMain->SetHydroMeter2Icon(enHydroMeterNormal);

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

	delete StrLst;

	m_TimeStamp = Now();	// TimeStamp refresh

	LogMessage("AG1500K Decode finished.. ");

	::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_enHydroNo == enHydroFirst) ?  enHydroMeter1Finish : \
												((m_enHydroNo == enHydroSecond) ? enHydroMeter2Finish : enHydroMeter3Finish)), 0);

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TAG1500K::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAG1500K::Receive(AnsiString RecvMsg, int iLen)
{
	//if ( m_enStep == enSensorIdle ) return true;	// not receive status
	//if ( m_bBreak ) return true;					// BREAK Status

	if ( m_bBreak ) {
		// BREAK 상황에서 "Sontek", ">" 스트링이 들어오면 BREAK Response가 완료된 것으로 간주함.
		if ( m_bFlooding ) return true;	// 수신된 횟수가 홍수된 상황 => 버리고 리턴한다.
		m_iThrowCount++;
		if ( m_iThrowCount > 10 ) { m_bFlooding = true;  return true; }
		LogMessage("[AG1500K] <BREAK> before START. RecvMsg => " + RecvMsg);
		m_RecvMsg += RecvMsg;
		if ( m_bBreakResponse == false ) {
			AnsiString x = m_RecvMsg.UpperCase();
			//if ( (x.AnsiPos("SONTEK") > 0) && (x.AnsiPos(">") > 0) ) {	// heaven2 2021.11.05
			if (x.AnsiPos("SONTEK") > 0) {	// heaven2 2021.11.05
				m_bBreakResponse = true;	// Break Response string found => Last Character
				LogMessage("[AG1500K] <BREAK> response is ok..");
				//m_bBreak = false;	// Break Status clear .
			}
		}
		return true;					// BREAK Status
	}

	if ( m_enStep == enSensorIdle ) {
		if ( m_bFlooding ) return true;	// 수신된 횟수가 홍수된 상황 => 버리고 리턴한다.
		m_iThrowCount++;
		if ( m_iThrowCount > 10 ) { m_bFlooding = true;  return true; }
		LogMessage("[AG1500K] Idle Status RecvMsg => " + RecvMsg );
		return true;	// not receive status
	}
	// 2014.06.11 SL/AG 여러번 전송하는 형태로 파악되어 이렇게만 하고 타이머에서 Read Expire 시킴
	m_RecvMsg += RecvMsg;

//	int iPos = 0;
//	iPos = m_RecvMsg.AnsiPos("\r\n");
//
//	AnsiString sCmd;
//	int iNextPos = 0;
//	while ( iPos > 0 ) {
//		if ( iPos > 0 ) {	// \r\n
//			sCmd = m_RecvMsg.SubString(1, iPos-1);
//			iNextPos = iPos + 2;
//			LogMessage("[AG1500K]<=" + sCmd);
//		}
//
//		Decode(sCmd, sCmd.Length());
//
//		m_RecvMsg = m_RecvMsg.SubString(iNextPos, m_RecvMsg.Length() - (iNextPos-1));
//		if ( !m_RecvMsg.IsEmpty() ) LogMessage(m_RecvMsg, false);
//		//m_RecvMsg = m_RecvMsg.Trim();	// trim \r\n<body>\r\n
//
//		iPos = 0;
//		iPos = m_RecvMsg.AnsiPos("\r\n");
//	}

	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAG1500K::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Send
bool TAG1500K::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		m_pPort->PutString(RecvMsg);
		LogMessage("[AG1500K]=>" + RecvMsg);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TAG1500K::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
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
void TAG1500K::DB2Value()
{
	AvgVel              = m_RHydroMeter.AvgVelocity; // AvgVelocity
	MinVel              = m_RHydroMeter.MinVelocity; // MinVelocity
	MaxVel              = m_RHydroMeter.MaxVelocity; // MaxVelocity
	V1                  = m_RHydroMeter.Value01;	 // Velocity component 1
	V2                  = m_RHydroMeter.Value02;	 // Velocity component 2
	V3                  = m_RHydroMeter.Value03;	 // Velocity component 3
	ErrorOfVelocity1    = m_RHydroMeter.Value04;	 // Standard error of velocity 1
	ErrorOfVelocity2    = m_RHydroMeter.Value05;	 // Standard error of velocity 2
	ErrorOfVelocity3    = m_RHydroMeter.Value06;	 // Standard error of velocity 3
	SignalStrength1     = m_RHydroMeter.Value07;	 // Signal Strength 1
	SignalStrength2     = m_RHydroMeter.Value08;	 // Signal Strength 2
	SignalStrength3     = m_RHydroMeter.Value09;	 // Signal Strength 3
	PercentGoodPings    = m_RHydroMeter.Value10;	 // PercentGoodPings
	Heading             = m_RHydroMeter.Value11;	 // Heading
	Pitch               = m_RHydroMeter.Value12;	 // Pitch
	Roll                = m_RHydroMeter.Value13;	 // Roll
	DeviationHeading    = m_RHydroMeter.Value14;	 // Standard deviation heading
	DeviationPitch      = m_RHydroMeter.Value15; 	 // Standard deviation pitch
	DeviationRoll       = m_RHydroMeter.Value16;	 // Standard deviation roll
	MeanTemperature     = m_RHydroMeter.Value17;	 // Mean Temperature
	MeanPressure        = m_RHydroMeter.Value18;	 // Mean Pressure
	DeviationPressure   = m_RHydroMeter.Value19;	 // Standard deviation of pressure
	InputPowerLevel     = m_RHydroMeter.Value20;	 // Input power Level
	StartLocation       = m_RHydroMeter.Value21;	 // Starting location of sampling volume
	EndLocation         = m_RHydroMeter.Value22;	 // Ending location of sampling volume
	NoiseLevel1         = m_RHydroMeter.Value23;	 // Noise Level(Beam 1)
	NoiseLevel2         = m_RHydroMeter.Value24;	 // Noise Level(Beam 2)
	NoiseLevel3         = m_RHydroMeter.Value25;	 // Noise Level(Beam 3)
}

//---------------------------------------------------------------------------
// Value2DB
void TAG1500K::Value2DB()
{
	m_RHydroMeter.AvgVelocity   =  AvgVel;              // AvgVelocity
	m_RHydroMeter.MinVelocity   =  MinVel;              // MinVelocity
	m_RHydroMeter.MaxVelocity   =  MaxVel;              // MaxVelocity
	m_RHydroMeter.Value01	    =  V1;                  // Velocity component 1
	m_RHydroMeter.Value02	    =  V2;                  // Velocity component 2
	m_RHydroMeter.Value03	    =  V3;                  // Velocity component 3
	m_RHydroMeter.Value04	    =  ErrorOfVelocity1;    // Standard error of velocity 1
	m_RHydroMeter.Value05	    =  ErrorOfVelocity2;    // Standard error of velocity 2
	m_RHydroMeter.Value06	    =  ErrorOfVelocity3;    // Standard error of velocity 3
	m_RHydroMeter.Value07	    =  SignalStrength1;     // Signal Strength 1
	m_RHydroMeter.Value08	    =  SignalStrength2;     // Signal Strength 2
	m_RHydroMeter.Value09	    =  SignalStrength3;     // Signal Strength 3
	m_RHydroMeter.Value10	    =  PercentGoodPings;    // PercentGoodPings
	m_RHydroMeter.Value11	    =  Heading;             // Heading
	m_RHydroMeter.Value12	    =  Pitch;               // Pitch
	m_RHydroMeter.Value13	    =  Roll;                // Roll
	m_RHydroMeter.Value14	    =  DeviationHeading;    // Standard deviation heading
	m_RHydroMeter.Value15 	    =  DeviationPitch;      // Standard deviation pitch
	m_RHydroMeter.Value16	    =  DeviationRoll;       // Standard deviation roll
	m_RHydroMeter.Value17	    =  MeanTemperature;     // Mean Temperature
	m_RHydroMeter.Value18	    =  MeanPressure;        // Mean Pressure
	m_RHydroMeter.Value19	    =  DeviationPressure;   // Standard deviation of pressure
	m_RHydroMeter.Value20	    =  InputPowerLevel;     // Input power Level
	m_RHydroMeter.Value21	    =  StartLocation;       // Starting location of sampling volume
	m_RHydroMeter.Value22	    =  EndLocation;         // Ending location of sampling volume
	m_RHydroMeter.Value23	    =  NoiseLevel1;         // Noise Level(Beam 1)
	m_RHydroMeter.Value24	    =  NoiseLevel2;         // Noise Level(Beam 2)
	m_RHydroMeter.Value25	    =  NoiseLevel3;         // Noise Level(Beam 3)
	//m_RHydroMeter.RawData       =  m_RecvMsg;			// heaven2 2016.06.07
}

//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TAG1500K::GetAvgVelocity()
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

		m_pVelocityList->Sort(CompareVelocityAG);	// Sort

		int iMid = m_pVelocityList->Count / 2;
		STAGVelocity *pVelocity = NULL;
		pVelocity = (STAGVelocity *)m_pVelocityList->Items[iMid];
		Velocity  = (bReverse) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;

	} else {	// Cell이 5 이하=> 평균값 사용

		double Sum = 0;
		STAGVelocity *pVelocity = NULL;

		for ( int i = StartCell-1 ; i < EndCell - 1 ; i++ ) {
			pVelocity = (STAGVelocity *)m_pVelocityList->Items[i];
			Sum += (bReverse) ? (pVelocity->VelocityX * -1) : (pVelocity->VelocityX);
		}

		Velocity = Sum / CellCount;
	}

	return Velocity * 0.001;	// cm/s => m/s로 변환하여 변환
								// 0.01을 곱해야 하나 AG 시리즈 결과 자체가 정수 형태로 표출( 271=> 21.1cm/s를 의미해서 0.001을 곱합
}

//---------------------------------------------------------------------------
void TAG1500K::SetBreak(bool bBreak)
{
	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
bool TAG1500K::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
void TAG1500K::SendBreak(int iMiliSecond)	// Send <BREAK> to ComPort
{
	if ( m_pPort == NULL ) return;
	if ( m_pPort->Open == false ) return;

	m_pPort->SetBreak(true);
	SleepMsg(iMiliSecond);	// 900ms
	m_pPort->SetBreak(false);
	//SleepMsg(100);			// 100ms

// 2016.05.25 heaven2

//	SleepMsg(1000);	// Wait for while
//
//	AnsiString sStart = "START\r";
//	bool bResult = false;
//
//	bResult = Send(sStart.c_str(), sStart.Length());
}

//---------------------------------------------------------------------------
// DecodeFrame
bool TAG1500K::CalAvgMinMaxVelocity()
{
	double Avg, Min, Max, Sum;
	Avg = Min = Max = Sum = 0;
	bool bFirst = true;

	int StartCell, EndCell;
	StartCell = EndCell = 0;
	StartCell = ((m_enHydroNo == enHydroFirst)  ? theEnv.m_StartCell1 :
				((m_enHydroNo == enHydroSecond) ? theEnv.m_StartCell2 : theEnv.m_StartCell3));
	EndCell   = ((m_enHydroNo == enHydroFirst)  ? theEnv.m_EndCell1   :
				((m_enHydroNo == enHydroSecond) ? theEnv.m_EndCell2   : theEnv.m_EndCell3));
	if ( StartCell == 0 ) StartCell =  1;	// 만약을 위한 코드(Default: 1 ~ 10 cell)
	if ( EndCell   == 0 ) EndCell   = 10;   // 만약을 위한 코드

	STAGVelocity *pVelocity = NULL;
	int iValidCellCount = 0;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STAGVelocity *) m_pVelocityList->Items[i];

		if ( i+1 >= StartCell && i+1 <= EndCell ) {

			if ( pVelocity->VelocityX < theEnv.m_dHydroLowerBound ||
				 pVelocity->VelocityX > theEnv.m_dHydroUpperBound ) continue;	// 유효범위 이외의 값이 오면 처리하지 않는다.

			if ( bFirst ) {
				Min = pVelocity->VelocityX;
				Max = pVelocity->VelocityX;
				bFirst = false;
			}

			if ( pVelocity != NULL ) {
				Sum += pVelocity->VelocityX;
				iValidCellCount++;
			}

			if ( abs(pVelocity->VelocityX) < abs(Min) ) {
				Min = pVelocity->VelocityX;
			} else {}

			if ( abs(pVelocity->VelocityX) > abs(Max) ) {
				Max = pVelocity->VelocityX;
			} else {}

		} else {
			// 유효셀 범위 밖이므로 Avg/Min/Max에서 제외한다.
		}
	}

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
