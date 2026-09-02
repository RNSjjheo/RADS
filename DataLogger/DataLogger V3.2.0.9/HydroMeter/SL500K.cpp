//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "SL500K.h"
#include "AniThread.h"
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------

int __fastcall CompareVelocitySL(void *Item1, void *Item2)
{
	STSLVelocity *A, *B;
	A = (STSLVelocity *)Item1;
	B = (STSLVelocity *)Item2;
	if ( A->VelocityX < B->VelocityX )       return -1;	// True  => Skip
	else if ( A->VelocityX == B->VelocityX ) return  0;
	else                                     return  1;	// false => Swap
}


//---------------------------------------------------------------------------
// Constructor
TSL500K::TSL500K()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = SLSERIES_STR;

	CellBegin        = 0;
	CellEnd          = 0;
	CellNumber       = 0;
	CellNo           = 0;
	CellDistance     = 0;

	AvgVel           = 0;
	MinVel			 = 0;
	MaxVel			 = 0;
	NumberOfBeam     = 0;
	Orientation      = 0;
	TemperatureMode  = 0;
	CoordinateSystem = 0;
	NumberOfCells    = 0;
	CellSize         = 0;
	BlankingDistance = 0;
	AverageInterval  = 0;
	NumberOfPings    = 0;
	MeanHeading      = 0;
	MeanPitch        = 0;
	MeanRoll         = 0;
	MeanTemperature  = 0;
	MeanPressure     = 0;
	DeviationHeading = 0;
	DeviationPitch   = 0;
	DeviationRoll    = 0;
	StdTemperature   = 0;
	StdPressure      = 0;
	SoundSpeed       = 0;

	//m_pVelocityList = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TSL500K::~TSL500K()
{
	STSLVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STSLVelocity *) m_pVelocityList->Items[i];
		delete pVelocity;
	}
	m_pVelocityList->Clear();

	//delete m_pVelocityList; // Delete THydroMeter Class
}

//---------------------------------------------------------------------------
// Clear
void TSL500K::Clear()
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

	HydroKind   = SLSERIES_STR;

	AvgVel           = 0;
	MinVel			 = 0;
	MaxVel			 = 0;
	NumberOfBeam     = 0;
	Orientation      = 0;
	TemperatureMode  = 0;
	CoordinateSystem = 0;
	NumberOfCells    = 0;
	CellSize         = 0;
	BlankingDistance = 0;
	AverageInterval  = 0;
	NumberOfPings    = 0;
	MeanHeading      = 0;
	MeanPitch        = 0;
	MeanRoll         = 0;
	MeanTemperature  = 0;
	MeanPressure     = 0;
	DeviationHeading = 0;
	DeviationPitch   = 0;
	DeviationRoll    = 0;
	StdTemperature   = 0;
	StdPressure      = 0;
	SoundSpeed       = 0;

	STSLVelocity *pVelocity = NULL;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STSLVelocity *) m_pVelocityList->Items[i];
		delete pVelocity;
	}
	m_pVelocityList->Clear();

	m_RHydroCellContainer.Clear();
}

//---------------------------------------------------------------------------
// Init
void TSL500K::Init(TApdComPort *pPort, bool bFirst)
{
	m_bFirst = bFirst;
	m_RHydroMeter.First = bFirst;	// HydroMeter1 or not

	m_pPort = pPort;
}

//---------------------------------------------------------------------------
void TSL500K::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
	this->CellBegin    = CellBegin;
	this->CellEnd      = CellEnd;
	this->CellNumber   = CellNumber;
	this->CellNo       = CellNo;
	this->CellDistance = CellDistance;
}

//---------------------------------------------------------------------------
// Start
bool TSL500K::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
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
		LogMessage("[SL500K] Start error => Send Error");
		return false;
	}

	//m_TimeStamp = Now();	// Save Current Time
	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	if ( m_bFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterAni3);
	else            FormMain->SetHydroMeter2Icon(enHydroMeterAni3);

	if ( FormMain->m_pAniThread != NULL ) {
		if ( m_bFirst ) {
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
bool TSL500K::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;

	return true;
}

//---------------------------------------------------------------------------
// Decode
//data type, se number,pro number,      year,     month,       day,      hour,    minute,    second,beams nums,orientation,temp mode,coo system,  cell num, cell size,blank dist,   avg int,  ping num,   heading,     pitch, roll(0.1),temp(0.01),  pressure, s-heading,   s-pitch,    s-roll,temp(0.01), pressure, speed(0.1),                                                                                                                                                                       vol(0.2),  cell num,        VX,        VY,       SD1,       SD2,       Ap1,       Ap2,
//       16,      C687,       912,      2010,         7,        20,         0,         0,         7,         2,         2,         1,         1,        20,       200,       200,        30,        99,      2176,        34,        -8,      2449,      5743,         0,         0,         0,         1,         5,     14947,         0,         0,         0,         0,        82,       179,        83,       180,        19,        20,         0,         0,       145,       250,         0,        53,         1,       183,        67,        23,        10,       159,       158,         2,       203,        47,        25,        10,       152,       150,         3,       225,       -23,        23,        11,       146,       146,         4,       292,        34,        21,        10,       141,       141,         5,       275,       -10,        25,        11,       136,       137,         6,       330,        19,        25,        11,       135,       133,         7,       406,        50,        28,        11,       130,       130,         8,       407,        57,        25,        11,       128,       126,         9,       434,        26,        25,        10,       125,       124,        10,       493,        52,        27,        13,       122,       121,        11,       500,        28,        26,        12,       121,       120,        12,       569,        52,        26,        11,       119,       117,        13,       538,        49,        27,        11,       117,       115,        14,       464,        30,        26,        11,       114,       117,        15,       431,        48,        25,        10,       113,       121,        16,       163,       164,        21,         9,       112,       122,        17,       398,        39,        20,         8,       110,       127,        18,       365,        49,        24,        11,       111,       123,        19,       198,       109,        17,         8,       108,       139,        20,        87,       212,        21,         9,       106,       139,
bool TSL500K::Decode(AnsiString sCmd, int iLen)
{
	if ( m_enStep == enSensorIdle ) {	// idle 상태일 때
		::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_bFirst) ?  enHydroMeter1Finish : enHydroMeter2Finish), 0);
		return true;	// not receive status
	}

	if ( sCmd.IsEmpty() ) {	// Error 상황으로 수신한 데이터가 없을 때 Skip하기 위해
		::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_bFirst) ?  enHydroMeter1Finish : enHydroMeter2Finish), 0);
		return false;
	}

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

	if ( StrLst->Count > 28 ) { 	// 수신된 Item이 최소한 28개 이상은 되어야 처리함.
		try {
			NumberOfBeam      = StrToInt(Trim(StrLst->Strings[ 9]));
			Orientation       = StrToInt(Trim(StrLst->Strings[10]));
			TemperatureMode   = StrToInt(Trim(StrLst->Strings[11]));
			CoordinateSystem  = StrToInt(Trim(StrLst->Strings[12]));
			NumberOfCells     = StrToInt(Trim(StrLst->Strings[13]));
			CellSize          = StrToInt(Trim(StrLst->Strings[14]));
			BlankingDistance  = StrToInt(Trim(StrLst->Strings[15]));
			AverageInterval   = StrToInt(Trim(StrLst->Strings[16]));
			NumberOfPings     = StrToInt(Trim(StrLst->Strings[17]));
			MeanHeading       = StrToInt(Trim(StrLst->Strings[18]));
			MeanPitch         = StrToInt(Trim(StrLst->Strings[19]));
			MeanRoll          = StrToInt(Trim(StrLst->Strings[20]));
			MeanTemperature   = StrToInt(Trim(StrLst->Strings[21]));
			MeanPressure      = StrToInt(Trim(StrLst->Strings[22]));
			DeviationHeading  = StrToInt(Trim(StrLst->Strings[23]));
			DeviationPitch    = StrToInt(Trim(StrLst->Strings[24]));
			DeviationRoll     = StrToInt(Trim(StrLst->Strings[25]));
			StdTemperature    = StrToInt(Trim(StrLst->Strings[26]));
			StdPressure       = StrToInt(Trim(StrLst->Strings[27]));
			SoundSpeed        = StrToInt(Trim(StrLst->Strings[28]));
		} catch (Exception &e ) {
			LogMessage(e.Message);
		}
	} else {
		// nothing to do.. 수신한 데이터가 짧거나 비정상적인 데이터로 간주됨.
	}

	int iCellNo   = StrToInt(Trim(StrLst->Strings[13])); 	// Cell No in Protocol
	int ItemCount = StrLst->Count;	// String item count
	int Pos       = 45;	// First CellNo
	int iIndex    = 0;  // Cell Index ( 0 ~ CellNo-1 )

	STSLVelocity *pVelocity = NULL;
	TRHydroMeterCell *pCell = NULL;
	//while ( i < CellNo ) {
	while ( Pos < ItemCount ) {
		pVelocity = new STSLVelocity();
		pCell     = new TRHydroMeterCell(m_RHydroMeter.First);

		try {
			pVelocity->CellNo      = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->VelocityX   = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->VelocityY   = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->StdErrVelX  = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->StdErrVelY  = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->SignalBeam1 = StrToInt(Trim(StrLst->Strings[Pos++]));
			pVelocity->SignalBeam2 = StrToInt(Trim(StrLst->Strings[Pos++]));

			//pVelocity->CellNo      = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 0]));
			//pVelocity->VelocityX   = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 1]));
			//pVelocity->VelocityY   = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 2]));
			//pVelocity->StdErrVelX  = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 3]));
			//pVelocity->StdErrVelY  = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 4]));
			//pVelocity->SignalBeam1 = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 5]));
			//pVelocity->SignalBeam2 = StrToInt(Trim(StrLst->Strings[45 + (iIndex*7) + 6]));

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
	if ( m_bFirst ) FormMain->SetHydroMeter1Icon(enHydroMeterNormal);
	else            FormMain->SetHydroMeter2Icon(enHydroMeterNormal);

	if ( FormMain->m_pAniThread != NULL ) {
		if ( m_bFirst ) {
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

	LogMessage("SL500K Decode finished.. ");

	::PostMessage(FormMain->Handle, UM_MESSAGE, ((m_bFirst) ?  enHydroMeter1Finish : enHydroMeter2Finish), 0);

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TSL500K::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TSL500K::Receive(AnsiString RecvMsg, int iLen)
{
	//if ( m_enStep == enSensorIdle ) return true;	// not receive status
	//if ( m_bBreak ) return true;					// BREAK Status

	if ( m_bBreak ) {
		// BREAK 상황에서 "Sontek", ">" 스트링이 들어오면 BREAK Response가 완료된 것으로 간주함.
		if ( m_bFlooding ) return true;	// 수신된 횟수가 홍수된 상황 => 버리고 리턴한다.
		m_iThrowCount++;
		if ( m_iThrowCount > 10 ) { m_bFlooding = true;  return true; }
		LogMessage("[SL500] <BREAK> before START. RecvMsg => " + RecvMsg);
		m_RecvMsg += RecvMsg;
		if ( m_bBreakResponse == false ) {
			AnsiString x = m_RecvMsg.UpperCase();
			if ( (x.AnsiPos("SONTEK") > 0) && (x.AnsiPos(">") > 0) ) {
				m_bBreakResponse = true;	// Break Response string found => Last Character
				LogMessage("[SL500] <BREAK> response is ok..");
				//m_bBreak = false;	// Break Status clear .
			}
		}
		return true;					// BREAK Status
	}

	if ( m_enStep == enSensorIdle ) {
		if ( m_bFlooding ) return true;	// 수신된 횟수가 홍수된 상황 => 버리고 리턴한다.
		m_iThrowCount++;
		if ( m_iThrowCount > 10 ) { m_bFlooding = true;  return true; }
		LogMessage("[SL500] Idle Status RecvMsg => " + RecvMsg );
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
//			LogMessage("[SL500k]<=" + sCmd);
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
bool TSL500K::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Send
bool TSL500K::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		m_pPort->PutString(RecvMsg);
		LogMessage("[SL500k]=>" + RecvMsg);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TSL500K::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
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
void TSL500K::DB2Value()
{
	AvgVel           = m_RHydroMeter.AvgVelocity;
	MinVel			 = m_RHydroMeter.MinVelocity;
	MaxVel			 = m_RHydroMeter.MaxVelocity;
	NumberOfBeam     = m_RHydroMeter.Value01;
	Orientation      = m_RHydroMeter.Value02;
	TemperatureMode  = m_RHydroMeter.Value03;
	CoordinateSystem = m_RHydroMeter.Value04;
	NumberOfCells    = m_RHydroMeter.Value05;
	CellSize         = m_RHydroMeter.Value06;
	BlankingDistance = m_RHydroMeter.Value07;
	AverageInterval  = m_RHydroMeter.Value08;
	NumberOfPings    = m_RHydroMeter.Value09;
	MeanHeading      = m_RHydroMeter.Value10;
	MeanPitch        = m_RHydroMeter.Value11;
	MeanRoll         = m_RHydroMeter.Value12;
	MeanTemperature  = m_RHydroMeter.Value13;
	MeanPressure     = m_RHydroMeter.Value14;
	DeviationHeading = m_RHydroMeter.Value15;
	DeviationPitch   = m_RHydroMeter.Value16;
	DeviationRoll    = m_RHydroMeter.Value17;
	StdTemperature   = m_RHydroMeter.Value18;
	StdPressure      = m_RHydroMeter.Value19;
	SoundSpeed       = m_RHydroMeter.Value20;
}

//---------------------------------------------------------------------------
// Value2DB
void TSL500K::Value2DB()
{
	m_RHydroMeter.AvgVelocity  = AvgVel;
	m_RHydroMeter.MinVelocity  = MinVel;
	m_RHydroMeter.MaxVelocity  = MaxVel;
	m_RHydroMeter.Value01      = NumberOfBeam;
	m_RHydroMeter.Value02      = Orientation;
	m_RHydroMeter.Value03      = TemperatureMode;
	m_RHydroMeter.Value04      = CoordinateSystem;
	m_RHydroMeter.Value05      = NumberOfCells;
	m_RHydroMeter.Value06	   = CellSize;
	m_RHydroMeter.Value07	   = BlankingDistance;
	m_RHydroMeter.Value08	   = AverageInterval;
	m_RHydroMeter.Value09	   = NumberOfPings;
	m_RHydroMeter.Value10	   = MeanHeading;
	m_RHydroMeter.Value11	   = MeanPitch;
	m_RHydroMeter.Value12	   = MeanRoll;
	m_RHydroMeter.Value13	   = MeanTemperature;
	m_RHydroMeter.Value14	   = MeanPressure;
	m_RHydroMeter.Value15	   = DeviationHeading;
	m_RHydroMeter.Value16	   = DeviationPitch;
	m_RHydroMeter.Value17	   = DeviationRoll;
	m_RHydroMeter.Value18	   = StdTemperature;
	m_RHydroMeter.Value19	   = StdPressure;
	m_RHydroMeter.Value20 	   = SoundSpeed;
	//m_RHydroMeter.RawData      = m_RecvMsg;
}

//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TSL500K::GetAvgVelocity()
{
	double Velocity  = 0;
	int    StartCell = 0;
	int    EndCell   = 0;
	bool   bReverse  = false;

	if ( m_bFirst ) {
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

		m_pVelocityList->Sort(CompareVelocitySL);	// Sort

		int iMid = m_pVelocityList->Count / 2;
		STSLVelocity *pVelocity = NULL;
		pVelocity = (STSLVelocity *)m_pVelocityList->Items[iMid];
		Velocity  = (bReverse) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;

	} else {	// Cell이 5 이하=> 평균값 사용

		double Sum = 0;
		STSLVelocity *pVelocity = NULL;

		for ( int i = StartCell-1 ; i < EndCell - 1 ; i++ ) {
			pVelocity = (STSLVelocity *)m_pVelocityList->Items[i];
			Sum += (bReverse) ? (pVelocity->VelocityX * -1) : (pVelocity->VelocityX);
		}

		Velocity = Sum / CellCount;
	}

	return Velocity * 0.001;	// cm/sec => m/sec 로 변환하여 Return
}

//---------------------------------------------------------------------------
void TSL500K::SetBreak(bool bBreak)
{
	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
bool TSL500K::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
void TSL500K::SendBreak(int iMiliSecond)	// Send <BREAK> to ComPort
{
	if ( m_pPort == NULL ) return;
	if ( m_pPort->Open == false ) return;

	m_pPort->SetBreak(true);
	SleepMsg(iMiliSecond);	// 900ms => 500ms
	m_pPort->SetBreak(false);
	//Sleep(100);			// 100ms

// 2016.05.25 heaven2

//	SleepMsg(1000);	// Wait 1 Second.
//
//	AnsiString sStart = "START\r";
//	bool bResult = false;
//
//	bResult = Send(sStart.c_str(), sStart.Length());
}

//---------------------------------------------------------------------------
// DecodeFrame
bool TSL500K::CalAvgMinMaxVelocity()
{
	double Avg, Min, Max, Sum;
	Avg = Min = Max = Sum = 0;
	bool bFirst = true;

	int StartCell, EndCell;
	StartCell = EndCell = 0;
	StartCell = (m_bFirst) ? theEnv.m_StartCell1 : theEnv.m_StartCell2;
	EndCell   = (m_bFirst) ? theEnv.m_EndCell1 : theEnv.m_EndCell2;
	if ( StartCell == 0 ) StartCell =  1;	// 만약을 위한 코드(Default: 1 ~ 10 cell)
	if ( EndCell   == 0 ) EndCell   = 10;   // 만약을 위한 코드

	STSLVelocity *pVelocity = NULL;
	int iValidCellCount = 0;
	for ( int i = 0 ; i < m_pVelocityList->Count ; i++ ) {
		pVelocity = (STSLVelocity *) m_pVelocityList->Items[i];

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
