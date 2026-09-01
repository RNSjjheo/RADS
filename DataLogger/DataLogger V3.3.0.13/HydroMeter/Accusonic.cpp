//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "Accusonic.h"
#include "AniThread.h"
//#include "System.RegularExpressions.hpp"

//---------------------------------------------------------------------------

//int __fastcall CompareVelocity(void *Item1, void *Item2)
//{
//	STAccusonic *A, *B;
//	A = (STAccusonic *)Item1;
//	B = (STAccusonic *)Item2;
//	if ( A->LDV1 < B->LDV1 )       return -1;	// True  => Skip
//	else if ( A->LDV1 == B->LDV1 ) return  0;
//	else                           return  1;	// false => Swap
//}

//---------------------------------------------------------------------------
// Constructor
TAccusonic::TAccusonic()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = ACCUSONIC_A_STR;
	Count = 0;

	memset(Buffer, 0x00, sizeof(Buffer));
	Pos = 0;
	memset(&stAccusonic, 0x00, sizeof(stAccusonic));
	memset(AccusonicArray, 0x00, sizeof(STAccusonic)*MAX_ACCUSONIC_COUNT);
	Count = 0;
	enVT100Step = enVT100Idle;	// idle, EscStart, EscFinish, EscData
	LastSpacePos = 0;

	//m_pAccuList     = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TAccusonic::~TAccusonic()
{
	// Resource release
//	STAccusonic *pAccusonic = NULL;
//	for ( int i = 0 ; i < m_pAccuList->Count ; i++ ) {
//		pAccusonic = (STAccusonic *) m_pAccuList->Items[i];
//		delete pAccusonic;
//	}
//	m_pAccuList->Clear();
//
//	delete m_pAccuList;
}

//---------------------------------------------------------------------------
// Clear
void TAccusonic::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	//m_bBreak     = false;			// <BREAK> false status

	MeasureDate = "";
	MeasureTime = "";

//	HydroKind   = ACCUSONIC_A_STR;
//	memset(Buffer, 0x00, sizeof(Buffer));
	Count = 0;

	//memset(Buffer, 0x00, sizeof(Buffer));   // Buffer는 Clear하지 않는다. 기존에 수신한 데이터가 있을 수 있으므로..
	//Pos = 0;
	//enVT100Step = enVT100Idle;	// idle, EscStart, EscFinish, EscData

	m_RHydroMeter.Clear();
//	m_RHydroCellContainer.Clear();

	memset(&stAccusonic, 0x00, sizeof(stAccusonic));
	memset(AccusonicArray, 0x00, sizeof(STAccusonic)*MAX_ACCUSONIC_COUNT);
	Count = 0;
	LastSpacePos = 0;

//	STAccusonic *pAccusonic = NULL;
//	for ( int i = 0 ; i < m_pAccuList->Count ; i++ ) {
//		pAccusonic = (STAccusonic *) m_pAccuList->Items[i];
//		delete pAccusonic;
//	}
//	m_pAccuList->Clear();

//	m_RHydroCellContainer.Clear();
}

//---------------------------------------------------------------------------
// Init
void TAccusonic::Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo)
{
	m_pPort = pPort;
	m_enHydroNo = enHydroNo;
	m_RHydroMeter.enHydroNo = enHydroNo;	// HydroMeter1 or not
	m_RHydroCellContainer.m_enHydroNo = enHydroNo;	// HydroMeter1Cell or not

	m_pPort = pPort;
}

//---------------------------------------------------------------------------
void TAccusonic::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
//	nothing to do..
}

//---------------------------------------------------------------------------
// Start
bool TAccusonic::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	//Clear();	// Status Clear	=> Finish()에서 함. Count 변수가 초기화 되지 않게

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	if ( FormMain->m_pAniThread != NULL ) {	// Animation
		if ( m_enHydroNo == enHydroFirst ) {
			FormMain->m_pAniThread->m_bHydroMeter1 = true;
			FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterAni3;
		} else {
			FormMain->m_pAniThread->m_bHydroMeter2 = true;
			FormMain->m_pAniThread->m_HydroMeter2Index = enHydroMeterAni3;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Finish
bool TAccusonic::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	Clear();
	if ( !bResult ) return bResult;
	LogMessage("Save and Finished..Cleared..");

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;	// 2014.09.17

	if ( FormMain->m_pAniThread != NULL ) {  	// Animation Stop
		if ( m_enHydroNo == enHydroFirst ) {
			FormMain->m_pAniThread->m_bHydroMeter1 = false;
			FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterNormal;
		} else {
			FormMain->m_pAniThread->m_bHydroMeter2 = false;
			FormMain->m_pAniThread->m_HydroMeter2Index = enHydroMeterNormal;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TAccusonic::Decode(AnsiString sCmd, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Decode => Accusonic Data
bool TAccusonic::Decode(BYTE *pBuffer, int iLen)
{
	AnsiString RecvStream = "";
	bool bComplete = false;

	if ( LastSpacePos == iLen-1 ) {	// 하나의 Frame으로 완결한 데이터
		bComplete = true;
		RecvStream = AnsiString((char *)pBuffer, iLen);
		memset(pBuffer, 0x00, BUFFER_4K);
		Pos = 0;
	} else if ( LastSpacePos < iLen-1 ) { // 하나의 Frame외에 여분의 데이터가 더 들어왔다.
		bComplete = false;
		RecvStream = AnsiString((char *)pBuffer, LastSpacePos+1);	// LastSpacePos => 배열 첨자이므로 Length = Pos + 1
		// Buffer의 남은 데이터를 앞쪽으로 이동한다.
		BYTE Temp[1024] = "";
		memcpy(Temp, pBuffer+(LastSpacePos+1), iLen-(LastSpacePos+1));
		memset(pBuffer, 0x00, BUFFER_4K);
		memcpy(pBuffer, Temp, iLen-(LastSpacePos+1));	// 버퍼의 뒤에 남은 부분을 앞부분으로 이동한다.
		Pos = iLen-(LastSpacePos+1);					// 버퍼 Position 정보도 이동 시킨다.
	} else {
		// 논리오류 : 마지막 Space가 iLen보다 클 수 없다.
		return false;
	}

//debug
//LogMessage(RecvStream);

	TStringList *StrList = new TStringList();
	StrList->DelimitedText = ' ';	// Space가 구분자이다.
	StrList->DelimitedText = RecvStream;

	AnsiString DataStream = "";
	AnsiString Token = "";
	for ( int i = 0 ; i < StrList->Count ; i++ ) {
		Token = StrList->Strings[i];
		if ( Token.AnsiPos("LDF") > 0 ) {
			DataStream = Token;
			break;
		}
	}

	delete StrList;	// Release

	if ( DataStream.IsEmpty() ) return false;	// Data Stream 없음.
//debug
//LogMessage(DataStream);

	TStringList *DataStrList = new TStringList();
	DataStrList->Delimiter = '#';
	DataStrList->DelimitedText = DataStream;
	AnsiString FieldStr;
	int        Index;
	AnsiString DataStr;
	//STAccusonic *pAccusonic = NULL;
	//pAccusonic = new STAccusonic();
	STAccusonic Accusonic;
	memset(&Accusonic, 0x00, sizeof(STAccusonic));
	bool bLDF = false;
	bool bLDB = false;

	for ( int i = 0 ; i < DataStrList->Count ; i++ ) {
		Token = DataStrList->Strings[i];

		if ( Token.IsEmpty() ) continue;
		if ( Token.AnsiPos("LD") == 0 ) {
			continue;
		}

		if ( Token.AnsiPos("LD") != 1 ) {	// LD 앞에 뭔가 Prefix가 붙었으면
			Index = Token.AnsiPos("LD");
			Token = Token.SubString(Index, Token.Length()-2);	// Prefix를 때어낸다.
		}

		try {
			FieldStr = Token.SubString(1,3);				  //LDF116005 => LDF
			Index    = StrToInt(Token.SubString(4,1));        //LDF116005 => 1
			DataStr  = Token.SubString(5,Token.Length()-5+1); //LDF116005 => 16005
			if (      FieldStr == "LDF" && Index == 1 ) {
				Accusonic.LDF1 = StrToFloat(DataStr)/1000;
				bLDF = true;
			}
			else if ( FieldStr == "LDO" && Index == 1 ) Accusonic.LDO1 = StrToInt(DataStr);
			else if ( FieldStr == "LDS" && Index == 1 ) Accusonic.LDS1 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDT" && Index == 1 ) Accusonic.LDT1 = StrToInt(DataStr);
			else if ( FieldStr == "LDE" && Index == 1 ) Accusonic.LDE1 = StrToInt(DataStr);

			else if ( FieldStr == "LDV" && Index == 1 ) Accusonic.LDV1 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 2 ) Accusonic.LDV2 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 3 ) Accusonic.LDV3 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 4 ) Accusonic.LDV4 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 5 ) Accusonic.LDV5 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 6 ) Accusonic.LDV6 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 7 ) Accusonic.LDV7 = StrToFloat(DataStr)/1000;
			else if ( FieldStr == "LDV" && Index == 8 ) Accusonic.LDV8 = StrToFloat(DataStr)/1000;

			else if ( FieldStr == "LDG" && Index == 1 ) Accusonic.LDG1 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 2 ) Accusonic.LDG2 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 3 ) Accusonic.LDG3 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 4 ) Accusonic.LDG4 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 5 ) Accusonic.LDG5 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 6 ) Accusonic.LDG6 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 7 ) Accusonic.LDG7 = StrToInt(DataStr);
			else if ( FieldStr == "LDG" && Index == 8 ) Accusonic.LDG8 = StrToInt(DataStr);

			else if ( FieldStr == "LDC" && Index == 1 ) Accusonic.LDC1 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 2 ) Accusonic.LDC2 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 3 ) Accusonic.LDC3 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 4 ) Accusonic.LDC4 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 5 ) Accusonic.LDC5 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 6 ) Accusonic.LDC6 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 7 ) Accusonic.LDC7 = StrToInt(DataStr);
			else if ( FieldStr == "LDC" && Index == 8 ) Accusonic.LDC8 = StrToInt(DataStr);

			else if ( FieldStr == "LDB" && Index == 1 ) Accusonic.LDB1 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 2 ) Accusonic.LDB2 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 3 ) Accusonic.LDB3 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 4 ) Accusonic.LDB4 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 5 ) Accusonic.LDB5 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 6 ) Accusonic.LDB6 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 7 ) Accusonic.LDB7 = StrToInt(DataStr);
			else if ( FieldStr == "LDB" && Index == 8 ) {
				Accusonic.LDB8 = StrToInt(DataStr);
				bLDB = true;
			} else {
				// nothing to do..
			}
		} catch ( Exception &e ) {
			LogMessage(e.Message);
			continue;
		}
	} // end for

	if ( bLDF && bLDB )	{
		//if ( m_pAccuList->Count > 240 ) {	// 너무 많이 Buffering 되면 메모리 부담으로 인해 더이상 Buffering하지 않는다. => max => 40분
		if ( Count >= MAX_ACCUSONIC_COUNT ) {	// 너무 많이 Buffering 되면 메모리 부담으로 인해 더이상 Buffering하지 않는다. => max => 40분
			LogMessage("Too many accusonic data added. no more accusonic data added.." + IntToStr(Count));
		} else {
			//m_pAccuList->Add(pAccusonic);
			memcpy(&AccusonicArray[Count], &Accusonic, sizeof(STAccusonic));
			Count++;
			m_enStep = enSensorWorking;
			LogMessage(HydroKind + " data set added.." + IntToStr(Count));
		}
	} else {
		//delete pAccusonic;
		LogMessage("Accusonic data set is not complete");
	}

	m_TimeStamp = Now();	// TimeStamp Refresh

	delete DataStrList;	// Release

	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAccusonic::Receive(AnsiString RecvMsg, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAccusonic::Receive(BYTE *pBuffer, int iLen)
{
	bool bResult = false;

	for ( int i = 0 ; i < iLen ; i++ ) {
		if ( enVT100Step == enVT100Idle ) {
			if ( *(pBuffer+i) == ESC_START ) {  	// 0x1B => Ctrl+[ => Escape Start
				enVT100Step = enVT100EscStart;
			} else {
				// nothing to do .. => Skip Data 그냥 버린다. ESC가 시작되어야먄 Parsing하기 시작한다.
			}
		} else if ( enVT100Step == enVT100EscStart ) {
			if ( *(pBuffer+i) == ESC_FINISH ) {		// 0x48 => 'H' => Escape Finish
				enVT100Step = enVT100Data;	// 이제 데이터를 수신하다.
			} else {
				// nothing to do.. => ESC Data이기 때문에 버린다.
			}
		} else if ( enVT100Step == enVT100EscFinish ) {
			// nothing to do
		} else if ( enVT100Step == enVT100Data ) {
			if ( *(pBuffer+i) == ESC_START ) { // 0x1B => ESC Sequence code
				enVT100Step = enVT100EscStart;	     // E
				// nothing to do..
			} else if ( *(pBuffer+i) >= ASCII_SPACE && *(pBuffer+i) <= ASCII_BRACE ) {	// 0x20 <= Data <= 0x7D => Buffering
				Buffer[Pos++] = *(pBuffer+i);
			} else {	// ASCII 문자 영역이 아닐 경우 => 깨진 것으로 간주하여 버린다.
				// nothing to do..
				// 수신데이터 중에 0x00도 Buffering에서 뺀다.
			}
		} else {
			// nothing to do
		}
	}
	// Space가 16개 있는 지 체크한다.
	int SpaceCnt = 0;
	bool bComplete = false;
	for ( int i = 0 ; i < Pos ; i++ ) {
		if ( Buffer[i] == ASCII_SPACE ) SpaceCnt++;
		else                            SpaceCnt = 0;
		if ( SpaceCnt >= 13 ) {	// SPace가 연속으로 13개 이상 나오면 데이터 1Set가 완료된 것으로 본다.
			LastSpacePos = i; 	// 마지막 Space의 Index를 기억한다.
			bComplete = true;
			break;
		}
	}

	if ( bComplete ) Decode((BYTE *)Buffer, Pos);

	return bResult;
}

//---------------------------------------------------------------------------
// Send
bool TAccusonic::Send(AnsiString RecvMsg, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Save
bool TAccusonic::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CalAvgMinMaxVelocity();
	Value2DB();	// Value Mapping

//debug
UnicodeString ResultStream;
	ResultStream = ResultStream.sprintf(L"Result => LDF1=%10.3f LDO1=%10ld LDS1=%10.3f LDT1=%10d LDE1=%10d "
										L"LDV1-8 %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f "
										L"LDG1-8 %10d %10d %10d %10d %10d %10d %10d %10d "
										L"LDC1-8 %10d %10d %10d %10d %10d %10d %10d %10d "
										L"LDB1-8 %10d %10d %10d %10d %10d %10d %10d %10d ",
										stAccusonic.LDF1,
										stAccusonic.LDO1,
										stAccusonic.LDS1,
										stAccusonic.LDT1,
										stAccusonic.LDE1,
										stAccusonic.LDV1,
										stAccusonic.LDV2,
										stAccusonic.LDV3,
										stAccusonic.LDV4,
										stAccusonic.LDV5,
										stAccusonic.LDV6,
										stAccusonic.LDV7,
										stAccusonic.LDV8,
										stAccusonic.LDG1,
										stAccusonic.LDG2,
										stAccusonic.LDG3,
										stAccusonic.LDG4,
										stAccusonic.LDG5,
										stAccusonic.LDG6,
										stAccusonic.LDG7,
										stAccusonic.LDG8,
										stAccusonic.LDC1,
										stAccusonic.LDC2,
										stAccusonic.LDC3,
										stAccusonic.LDC4,
										stAccusonic.LDC5,
										stAccusonic.LDC6,
										stAccusonic.LDC7,
										stAccusonic.LDC8,
										stAccusonic.LDB1,
										stAccusonic.LDB2,
										stAccusonic.LDB3,
										stAccusonic.LDB4,
										stAccusonic.LDB5,
										stAccusonic.LDB6,
										stAccusonic.LDB7,
										stAccusonic.LDB8 );
// debug
LogMessage(ResultStream);

	//heaven2 => 화면과 파일에 쓰는 로직

	m_RHydroMeter.MeasureDate = MeasureDate;
	m_RHydroMeter.MeasureTime = MeasureTime;
	m_RHydroMeter.HydroKind   = HydroKind;

	//m_RHydroMeterCell.MeasureDate = MeasureDate;
	//m_RHydroMeterCell.MeasureTime = MeasureTime;

	bool bResult = false;
	bResult = m_RHydroMeter.Insert();

//	if ( !bResult ) return bResult;        	// Accusonic은 HydroCell Table에 데이터가 없다.
//
//	bResult = m_RHydroCellContainer.Save(MeasureDate, MeasureTime);

	return bResult;
}

//---------------------------------------------------------------------------
// DB2Value
void TAccusonic::DB2Value()
{
	AvgVel = m_RHydroMeter.AvgVelocity;
	MinVel = m_RHydroMeter.AvgVelocity;
	MaxVel = m_RHydroMeter.MaxVelocity;
	stAccusonic.LDF1 = m_RHydroMeter.Value01;
	stAccusonic.LDO1 = m_RHydroMeter.Value02;
	stAccusonic.LDS1 = m_RHydroMeter.Value03;
	stAccusonic.LDT1 = m_RHydroMeter.Value04;
	stAccusonic.LDE1 = m_RHydroMeter.Value05;
	stAccusonic.LDV1 = m_RHydroMeter.Value06;
	stAccusonic.LDV2 = m_RHydroMeter.Value07;
	stAccusonic.LDV3 = m_RHydroMeter.Value08;
	stAccusonic.LDV4 = m_RHydroMeter.Value09;
	stAccusonic.LDV5 = m_RHydroMeter.Value10;
	stAccusonic.LDV6 = m_RHydroMeter.Value11;
	stAccusonic.LDV7 = m_RHydroMeter.Value12;
	stAccusonic.LDV8 = m_RHydroMeter.Value13;
	stAccusonic.LDG1 = m_RHydroMeter.Value14;
	stAccusonic.LDG2 = m_RHydroMeter.Value15;
	stAccusonic.LDG3 = m_RHydroMeter.Value16;
	stAccusonic.LDG4 = m_RHydroMeter.Value17;
	stAccusonic.LDG5 = m_RHydroMeter.Value18;
	stAccusonic.LDG6 = m_RHydroMeter.Value19;
	stAccusonic.LDG7 = m_RHydroMeter.Value20;
	stAccusonic.LDG8 = m_RHydroMeter.Value21;
	stAccusonic.LDC1 = m_RHydroMeter.Value22;
	stAccusonic.LDC2 = m_RHydroMeter.Value23;
	stAccusonic.LDC3 = m_RHydroMeter.Value24;
	stAccusonic.LDC4 = m_RHydroMeter.Value25;
	stAccusonic.LDC5 = m_RHydroMeter.Value26;
	stAccusonic.LDC6 = m_RHydroMeter.Value27;
	stAccusonic.LDC7 = m_RHydroMeter.Value28;
	stAccusonic.LDC8 = m_RHydroMeter.Value29;
	stAccusonic.LDB1 = m_RHydroMeter.Value30;
	stAccusonic.LDB2 = m_RHydroMeter.Value31;
	stAccusonic.LDB3 = m_RHydroMeter.Value32;
	stAccusonic.LDB4 = m_RHydroMeter.Value33;
	stAccusonic.LDB5 = m_RHydroMeter.Value34;
	stAccusonic.LDB6 = m_RHydroMeter.Value35;
	stAccusonic.LDB7 = m_RHydroMeter.Value36;
	stAccusonic.LDB8 = m_RHydroMeter.Value37;
	Count            = m_RHydroMeter.Value40;
}

//---------------------------------------------------------------------------
// Value2DB
void TAccusonic::Value2DB()
{
	m_RHydroMeter.AvgVelocity   =  AvgVel;  // Average Velocity
	m_RHydroMeter.MinVelocity   =  MinVel;  // Min Velocity
	m_RHydroMeter.MaxVelocity   =  MaxVel;  // Max Velocity
	m_RHydroMeter.Value01 = stAccusonic.LDF1;
	m_RHydroMeter.Value02 = stAccusonic.LDO1;
	m_RHydroMeter.Value03 = stAccusonic.LDS1;
	m_RHydroMeter.Value04 = stAccusonic.LDT1;
	m_RHydroMeter.Value05 = stAccusonic.LDE1;
	m_RHydroMeter.Value06 = stAccusonic.LDV1;
	m_RHydroMeter.Value07 = stAccusonic.LDV2;
	m_RHydroMeter.Value08 = stAccusonic.LDV3;
	m_RHydroMeter.Value09 = stAccusonic.LDV4;
	m_RHydroMeter.Value10 = stAccusonic.LDV5;
	m_RHydroMeter.Value11 = stAccusonic.LDV6;
	m_RHydroMeter.Value12 = stAccusonic.LDV7;
	m_RHydroMeter.Value13 = stAccusonic.LDV8;
	m_RHydroMeter.Value14 = stAccusonic.LDG1;
	m_RHydroMeter.Value15 = stAccusonic.LDG2;
	m_RHydroMeter.Value16 = stAccusonic.LDG3;
	m_RHydroMeter.Value17 = stAccusonic.LDG4;
	m_RHydroMeter.Value18 = stAccusonic.LDG5;
	m_RHydroMeter.Value19 = stAccusonic.LDG6;
	m_RHydroMeter.Value20 = stAccusonic.LDG7;
	m_RHydroMeter.Value21 = stAccusonic.LDG8;
	m_RHydroMeter.Value22 = stAccusonic.LDC1;
	m_RHydroMeter.Value23 = stAccusonic.LDC2;
	m_RHydroMeter.Value24 = stAccusonic.LDC3;
	m_RHydroMeter.Value25 = stAccusonic.LDC4;
	m_RHydroMeter.Value26 = stAccusonic.LDC5;
	m_RHydroMeter.Value27 = stAccusonic.LDC6;
	m_RHydroMeter.Value28 = stAccusonic.LDC7;
	m_RHydroMeter.Value29 = stAccusonic.LDC8;
	m_RHydroMeter.Value30 = stAccusonic.LDB1;
	m_RHydroMeter.Value31 = stAccusonic.LDB2;
	m_RHydroMeter.Value32 = stAccusonic.LDB3;
	m_RHydroMeter.Value33 = stAccusonic.LDB4;
	m_RHydroMeter.Value34 = stAccusonic.LDB5;
	m_RHydroMeter.Value35 = stAccusonic.LDB6;
	m_RHydroMeter.Value36 = stAccusonic.LDB7;
	m_RHydroMeter.Value37 = stAccusonic.LDB8;
	m_RHydroMeter.Value40 = Count;
}


//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TAccusonic::GetAvgVelocity()
{
	return AvgVel * 0.1;   	// cm/s => m/s로 변환(맞나?)
}

//---------------------------------------------------------------------------
void TAccusonic::SetBreak(bool bBreak)
{
//	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
bool TAccusonic::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
void TAccusonic::SendBreak(int iMiliSecond)	// Send <BREAK> to ComPort
{
//	if ( m_pPort == NULL ) return;
//	if ( m_pPort->Open == false ) return;
//
//	m_pPort->SetBreak(true);
//	SleepMsg(iMiliSecond);	// 900ms => 500ms
//	m_pPort->SetBreak(false);
//	//Sleep(100);			// 100ms
}

//---------------------------------------------------------------------------
// CalAvgMinMaxVelocity
bool TAccusonic::CalAvgMinMaxVelocity()
{
	double Avg, Min, Max, Sum;
	double x ,y;
	Avg = Min = Max = Sum = 0;
	bool bFirst = true;

	//STAccusonic *pAccusonic = NULL;
	int iValidCellCount = 0;
	for ( int i = 0 ; i < Count ; i++ ) {
		//pAccusonic = (STAccusonic *) m_pAccuList->Items[i];

		//if ( pAccusonic == NULL ) continue;

		// 유효범위값 Check
//		if ( pVelocity->Velocity1 < theEnv.m_dHydroLowerBound ||
//			 pVelocity->Velocity1 > theEnv.m_dHydroUpperBound ) continue;	// 유효범위 이외의 값이 오면 처리하지 않는다.

		stAccusonic.LDF1 += AccusonicArray[i].LDF1;
		stAccusonic.LDO1 += AccusonicArray[i].LDO1;
		stAccusonic.LDS1 += AccusonicArray[i].LDS1;
		stAccusonic.LDT1 += AccusonicArray[i].LDT1;
		stAccusonic.LDE1 += AccusonicArray[i].LDE1;
		stAccusonic.LDV1 += AccusonicArray[i].LDV1;
		stAccusonic.LDV2 += AccusonicArray[i].LDV2;
		stAccusonic.LDV3 += AccusonicArray[i].LDV3;
		stAccusonic.LDV4 += AccusonicArray[i].LDV4;
		stAccusonic.LDV5 += AccusonicArray[i].LDV5;
		stAccusonic.LDV6 += AccusonicArray[i].LDV6;
		stAccusonic.LDV7 += AccusonicArray[i].LDV7;
		stAccusonic.LDV8 += AccusonicArray[i].LDV8;
		stAccusonic.LDG1 += AccusonicArray[i].LDG1;
		stAccusonic.LDG2 += AccusonicArray[i].LDG2;
		stAccusonic.LDG3 += AccusonicArray[i].LDG3;
		stAccusonic.LDG4 += AccusonicArray[i].LDG4;
		stAccusonic.LDG5 += AccusonicArray[i].LDG5;
		stAccusonic.LDG6 += AccusonicArray[i].LDG6;
		stAccusonic.LDG7 += AccusonicArray[i].LDG7;
		stAccusonic.LDG8 += AccusonicArray[i].LDG8;
		stAccusonic.LDC1 += AccusonicArray[i].LDC1;
		stAccusonic.LDC2 += AccusonicArray[i].LDC2;
		stAccusonic.LDC3 += AccusonicArray[i].LDC3;
		stAccusonic.LDC4 += AccusonicArray[i].LDC4;
		stAccusonic.LDC5 += AccusonicArray[i].LDC5;
		stAccusonic.LDC6 += AccusonicArray[i].LDC6;
		stAccusonic.LDC7 += AccusonicArray[i].LDC7;
		stAccusonic.LDC8 += AccusonicArray[i].LDC8;
		stAccusonic.LDB1 += AccusonicArray[i].LDB1;
		stAccusonic.LDB2 += AccusonicArray[i].LDB2;
		stAccusonic.LDB3 += AccusonicArray[i].LDB3;
		stAccusonic.LDB4 += AccusonicArray[i].LDB4;
		stAccusonic.LDB5 += AccusonicArray[i].LDB5;
		stAccusonic.LDB6 += AccusonicArray[i].LDB6;
		stAccusonic.LDB7 += AccusonicArray[i].LDB7;
		stAccusonic.LDB8 += AccusonicArray[i].LDB8;

		x = (AccusonicArray[i].LDV1 + AccusonicArray[i].LDV2 + AccusonicArray[i].LDV3 + AccusonicArray[i].LDV4 +
			 AccusonicArray[i].LDV5 + AccusonicArray[i].LDV6 + AccusonicArray[i].LDV7 + AccusonicArray[i].LDV8 ) / 8;
		y = (AccusonicArray[i].LDV1 + AccusonicArray[i].LDV2 + AccusonicArray[i].LDV3 + AccusonicArray[i].LDV4 +
			 AccusonicArray[i].LDV5 + AccusonicArray[i].LDV6 + AccusonicArray[i].LDV7 + AccusonicArray[i].LDV8 ) / 8;

		if ( bFirst ) {
			Min = x;
			Max = y;
			bFirst = false;
		}

		iValidCellCount++;

		if ( abs(x) < Min ) Min = x; 	// -값이라 하더라도 거꾸로 설치된 것일 수 있으므로 절대값으로 계산
		if ( abs(y) > Max ) Max = y; 	// -값이라 하더라도 거꾸로 설치된 것일 수 있으므로 절대값으로 계산
	}

	// 2014.09.17 : 처음에는 셀 전체의 평균값을 사용하였으나
	// 유효범위 안의 셀만을 이용하여 셀유속을 구한다.
	if ( iValidCellCount > 0 ) {
		stAccusonic.LDF1 /= iValidCellCount;
		stAccusonic.LDO1 /= iValidCellCount;
		stAccusonic.LDS1 /= iValidCellCount;
		stAccusonic.LDT1 /= iValidCellCount;
		stAccusonic.LDE1 /= iValidCellCount;
		stAccusonic.LDV1 /= iValidCellCount;
		stAccusonic.LDV2 /= iValidCellCount;
		stAccusonic.LDV3 /= iValidCellCount;
		stAccusonic.LDV4 /= iValidCellCount;
		stAccusonic.LDV5 /= iValidCellCount;
		stAccusonic.LDV6 /= iValidCellCount;
		stAccusonic.LDV7 /= iValidCellCount;
		stAccusonic.LDV8 /= iValidCellCount;
		stAccusonic.LDG1 /= iValidCellCount;
		stAccusonic.LDG2 /= iValidCellCount;
		stAccusonic.LDG3 /= iValidCellCount;
		stAccusonic.LDG4 /= iValidCellCount;
		stAccusonic.LDG5 /= iValidCellCount;
		stAccusonic.LDG6 /= iValidCellCount;
		stAccusonic.LDG7 /= iValidCellCount;
		stAccusonic.LDG8 /= iValidCellCount;
		stAccusonic.LDC1 /= iValidCellCount;
		stAccusonic.LDC2 /= iValidCellCount;
		stAccusonic.LDC3 /= iValidCellCount;
		stAccusonic.LDC4 /= iValidCellCount;
		stAccusonic.LDC5 /= iValidCellCount;
		stAccusonic.LDC6 /= iValidCellCount;
		stAccusonic.LDC7 /= iValidCellCount;
		stAccusonic.LDC8 /= iValidCellCount;
		stAccusonic.LDB1 /= iValidCellCount;
		stAccusonic.LDB2 /= iValidCellCount;
		stAccusonic.LDB3 /= iValidCellCount;
		stAccusonic.LDB4 /= iValidCellCount;
		stAccusonic.LDB5 /= iValidCellCount;
		stAccusonic.LDB6 /= iValidCellCount;
		stAccusonic.LDB7 /= iValidCellCount;
		stAccusonic.LDB8 /= iValidCellCount;
		Count = iValidCellCount;
	} else {
		Count = 0;
	}

	AvgVel = (stAccusonic.LDV1 + stAccusonic.LDV2 + stAccusonic.LDV3 + stAccusonic.LDV4 +
			  stAccusonic.LDV5 + stAccusonic.LDV6 + stAccusonic.LDV7 + stAccusonic.LDV8 ) / 8;
	MinVel = Min;
	MaxVel = Max;

	// m/s => mm/s로 환산한다. => Chart를 그리기 위해서 이렇게 한다.
	AvgVel = AvgVel * 1000;
	MinVel = MinVel * 1000;
	MaxVel = MaxVel * 1000;

	return true;
}

#pragma package(smart_init)
