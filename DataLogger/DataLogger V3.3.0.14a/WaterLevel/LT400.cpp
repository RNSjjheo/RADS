//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "LT400.h"
#include "AniThread.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
TLT400::TLT400()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = LT400_STR;	// WaterLevel Sensor type
	Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;

	AvgWaterLevel  = 0;
	MinWaterLevel  = 0;
	MaxWaterLevel  = 0;
	Temperature    = 0;
	Pressure       = 0;
	Depth          = 0;

	WaterKind      = LT400_STR;	// WaterLevel Sensor type

	m_pList        = new TList();

	bStartAck      = false;
	RecvFlag       = 0x00;

	StartAck       = "";
	TempAck        = "";
	PressureAck    = "";
	DeptAck        = "";

	m_Status       = enLT400Idle;
	m_Count        = 0;

	bLeak = true;	// Leak or not-
}

//---------------------------------------------------------------------------
// Destructor
TLT400::~TLT400()
{
	// Resource lease
	if ( m_pList ) {
		STLT400Data *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			pData = (STLT400Data *) m_pList->Items[i];
			delete pData;
		}
		m_pList->Clear();
		delete m_pList;
	}

	// other resource release
}

//---------------------------------------------------------------------------
// Clear
void TLT400::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	//m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = LT400_STR;	// WaterLevel Sensor type
	//Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;

	AvgWaterLevel  = 0;
	MinWaterLevel  = 0;
	MaxWaterLevel  = 0;
	Temperature    = 0;
	Pressure       = 0;
	Depth          = 0;

	WaterKind      = LT400_STR;	// WaterLevel Sensor type

	// Resource lease
//	if ( m_pList ) {
//		STLT500Data *pData = NULL;
//		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
//			pData = (STLT500Data *) m_pList->Items[i];
//			delete pData;
//		}
//		m_pList->Clear();
//	}

	bStartAck      = false;
	RecvFlag       = 0x00;

	StartAck       = "";
	TempAck        = "";
	PressureAck    = "";
	DeptAck        = "";

	m_Status       = enLT400Idle;
	m_Count        = 0;

	bLeak = true;	// Leak or not-
}

//---------------------------------------------------------------------------
// ClearList
bool TLT400::ClearList()
{
	if ( m_pList ) {
		STLT400Data *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STLT400Data *) m_pList->Items[i];
				delete pData;
			} catch ( Exception &e) {
             	break;
			}
		}
		m_pList->Clear();
	}
	return true;
}

//---------------------------------------------------------------------------
// GetStatus
void TLT400::Init(TApdComPort *pPort)
{
	m_pPort = pPort;
}

//---------------------------------------------------------------------------
// GetStatus
bool TLT400::Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst)
{
	Clear();	// Status Clear
	if ( bFirst ) ClearList();	// Clear WaterLevel List Data

	if ( m_pPort != NULL ) {
		m_pPort->FlushInBuffer();	// Input buffer clear
		m_pPort->FlushOutBuffer();	// Output buffer clear
	}

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;

	// Atmosperic Sensor Start
	if ( FormMain->m_pAtmos != NULL )
		FormMain->m_pAtmos->Start(MeasureDate, MeasureTime, bFirst);	// 대기압센서를 Start한다.

	int  i = 0;
	bool bResult = false;
	bool bStart  = false;
	while ( i++ < 5 ) {	// Try 5 times
		m_Status = enLT400Start;
		m_enStep = enSensorStart;

		bResult = SendStart();
		if ( !bResult ) {
			LogMessage("[LT400] SendStart() => Send error");
			return false;
		}

		//m_TimeStamp = Now();	// 2017.09.20 TimeStamp는 데이터를 수신하면 갱신한다.
		// heaven2 2021.10.26 300ms => 500ms retry 5 times ..
		SleepMsg(500);	// Sleep 300ms with Process Window Message

		if ( bStartAck ) {	// bStartAck = true when Receive Event
			LogMessage("LT400 Started..");
			m_Count++;
			bStart = true;
			break;
		}
	}
	return bStart;
}

//---------------------------------------------------------------------------
// Finish
bool TLT400::SendStart()
{
	bool bResult = false;

	char Buffer[128] = "";
	byte Address = 0x00;
	byte FunctionCode = 0x00;
	byte CRC = 0x00;

	// Init  -------------------------------------------------------------------
	Address = 0x01;
	FunctionCode = 0x11;
	CRC = ((Address + FunctionCode) ^ 0xFF) + 0x01;

	byte Stream[16] = "";
	Stream[0] = Address;
	Stream[1] = FunctionCode;
	Stream[2] = CRC;

	Buffer[0] = ':';
	int Pos = 1;
	byte x,y;
	for ( int i = 0 ; i < 3 ; i++ ) {
		x = (Stream[i] & 0xF0) >> 4;
		y = (Stream[i] & 0x0F) >> 0;
		Buffer[Pos+0] = BIN2HEX(x);
		Buffer[Pos+1] = BIN2HEX(y);
		Pos += 2;
	}
	Buffer[Pos++] = '\r';
	Buffer[Pos++] = '\n';

	bResult = Send(AnsiString(Buffer), Pos);

	return bResult;
}


//---------------------------------------------------------------------------
// Finish
bool TLT400::SendTemperature()
{
	bool bResult = false;
	char Buffer[128] = "";

	STTROLLHeader TROLLHeader;
	int Length = 0;

	TROLLHeader.Address       = 0x01;
	TROLLHeader.FunctionCode  = 0x03;
	TROLLHeader.DataAddress   = 45;      // 0046 Temperature
	TROLLHeader.RegisterCount = 8;

	Length = MakeStream(Buffer, TROLLHeader);

	bResult = Send(Buffer, Length);

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TLT400::SendPressure()
{
	bool bResult = false;
	char Buffer[128] = "";

	STTROLLHeader TROLLHeader;
	int Length = 0;

	TROLLHeader.Address       = 0x01;
	TROLLHeader.FunctionCode  = 0x03;
	TROLLHeader.DataAddress   = 37;      // 0037 Pressure
	TROLLHeader.RegisterCount = 8;

	Length = MakeStream(Buffer, TROLLHeader);

	bResult = Send(Buffer, Length);

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TLT400::SendDepth()
{
	bool bResult = false;
	char Buffer[128] = "";

	STTROLLHeader TROLLHeader;
	int Length = 0;

	TROLLHeader.Address       = 0x01;
	TROLLHeader.FunctionCode  = 0x03;
	TROLLHeader.DataAddress   = 53;      // 0053 Depth
	TROLLHeader.RegisterCount = 8;

	Length = MakeStream(Buffer, TROLLHeader);

	bResult = Send(Buffer, Length);

	return bResult;
}

//---------------------------------------------------------------------------
// Make modubus header
int TLT400::MakeStream(char Buffer[], STTROLLHeader &Header)
{
    int Length = 0;

    byte Stream[64];
    int  Pos = 0;
    byte CRC = 0x00;

	Header.DataAddress   = SwapUInt(Header.DataAddress);
	Header.RegisterCount = SwapUInt(Header.RegisterCount);

	memcpy(Stream, &Header, sizeof(Header));

	Length = sizeof(STTROLLHeader);

	for ( int i = 0 ; i < Length ; i++ ) {
        CRC += Stream[i];
    }
    CRC = (CRC ^ 0xFF) + 0x01;	// complement by 2
	Stream[Length++] = CRC;		// Length => 7

    Buffer[0] = ':'; // prefix
    Pos = 1;
	byte x,y;
	for ( int i = 0 ; i < Length ; i++ ) {   // Binary => Hex String
		x = (Stream[i] & 0xF0) >> 4;
		y = (Stream[i] & 0x0F) >> 0;
		Buffer[Pos+0] = BIN2HEX(x);
		Buffer[Pos+1] = BIN2HEX(y);
		Pos += 2;
	}
	Buffer[Pos++] = '\r';   // postfix
	Buffer[Pos++] = '\n';   // postfix

	return Pos;
}

//---------------------------------------------------------------------------
// Make modubus data to structure data
int TLT400::MakeBinaryStream(byte *pStream, char *pBuffer, int Length)
{
    byte x,y;
    int Pos = 0;
	for ( int i = 1; i < Length ; i+=2 ) {  //":xxxx" => ':' except..
    	x = HEX2BIN(*(pBuffer+i));
    	y = HEX2BIN(*(pBuffer+i+1));
    	*(pStream+Pos) = (x << 4) | y ;
    	Pos++;
    }
	return Pos;
}

//---------------------------------------------------------------------------
// Finish
bool TLT400::Finish()
{
	if ( FormMain->m_pAtmos != NULL ) FormMain->m_pAtmos->Finish();

	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;
	m_Status = enLT400Idle;

	//::PostMessage(FormMain->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);

	return true;
}

//---------------------------------------------------------------------------
// GetStatus
bool TLT400::Decode(AnsiString RecvMsg, int iLen)
{
	if ( m_Status == enLT400Start ) {               // =>:0111EE
		if ( bStartAck ) return true;
		if ( RecvMsg.AnsiPos(":0111") > 0 ) {  // <=:0111XXXXXXXXXXXXXXXXXXX...
			bStartAck = true;
			RecvFlag |= TROLL_INIT;
			m_Status  = enLT400Temp;	// Go Next Step
			LogMessage("LT400 Start acked..");

			// Animation start
			FormMain->SetWaterLevelIcon(enWaterLevelAni1);
			if ( FormMain->m_pAniThread != NULL ) {
				FormMain->m_pAniThread->m_bWaterLevel = true;
				FormMain->m_pAniThread->m_WaterLevelIndex = enWaterLevelAni1;
			}
			FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

			SleepMsg(100);
			SendTemperature();
			m_Status = enLT400Temp;
			return true;
		}
	}

	bool bResult = false;
	int Length = 0;

	float          MeasuredValue = 0;
	unsigned short ParameterID = 0;
	unsigned short UnitID = 0;
	unsigned short DataQualityID = 0;
	float          SentinelValue = 0;
	unsigned short Units = 0;

	byte Stream[256] = "";

	Length = MakeBinaryStream(Stream, RecvMsg.c_str(), RecvMsg.Length());

	bResult = CheckCRC(Stream, Length);
	if ( !bResult ) {
		LogMessage("CRC Error");
		return false;
	}

	memcpy(&MeasuredValue, Stream +  3, 4);
	memcpy(&ParameterID,   Stream +  7, 2);
	memcpy(&UnitID,        Stream +  9, 2);
	memcpy(&DataQualityID, Stream + 11, 2);
	memcpy(&SentinelValue, Stream + 13, 4);
	memcpy(&Units,         Stream + 17, 2);

	MeasuredValue = SwapFloat(MeasuredValue);
	ParameterID   = SwapUInt(ParameterID);
	UnitID        = SwapUInt(UnitID);
	DataQualityID = SwapUInt(DataQualityID);
	SentinelValue = SwapFloat(SentinelValue);
	Units         = SwapUInt(Units);

	switch ( ParameterID ) {
		case 1 :     // Temperature
			{
				Temperature  = MeasuredValue;
				RecvFlag |= TROLL_TEMPERATURE;
				SleepMsg(100); // wait for while => must wait ..
				SendPressure();
				m_Status = enLT400Pressure;
			}
			break;
		case 2 : // Pressure
			{
				Pressure = MeasuredValue;
				RecvFlag |= TROLL_PRESSURE;
				SleepMsg(100); // wait for while => must wait ..
				SendDepth();
				m_Status = enLT400Depth;
			}
			break;
		case 3 : // level dept(default)
		case 4 : // level top of casing
		case 5 : // level elevation
			{
				Depth      = MeasuredValue;
				RecvFlag  |= TROLL_DEPTH;
				WaterLevel = MeasuredValue + Offset;

				if ( RecvFlag == ( TROLL_INIT | TROLL_TEMPERATURE | TROLL_PRESSURE | TROLL_DEPTH )) {

					m_TimeStamp = Now();	// TimeStamp refresh

					STLT400Data *pData = new STLT400Data();
					pData->Temperature = Temperature;
					pData->Pressure    = Pressure;
					pData->Depth       = Depth;
					m_pList->Add(pData);
					LogMessage("LT400 data is buffered..");
				}  else {
					LogMessage("LT400 is not complete data set..");
				}
				m_Status = enLT400Idle;

				// Animation stop
				FormMain->SetWaterLevelIcon(enWaterLevelNormal);
				if ( FormMain->m_pAniThread != NULL ) {
					FormMain->m_pAniThread->m_bWaterLevel = false;
					FormMain->m_pAniThread->m_WaterLevelIndex = enWaterLevelNormal;
				}
				FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);
			}
			break;
		case 9 :     // Actual Conductivity
			{
				//WaterLevel.Conductivity = MeasuredValue;
			}
			break;
		case 12:     // Salinity
			{
				//WaterLevel.Salinity      = MeasuredValue;
			}
			break;
	}
	return true;
}

//---------------------------------------------------------------------------
bool TLT400::Decode(BYTE *pBuffer, int iLen)
{
	bool bResult = true;

	// do something..

	return bResult;
};

//---------------------------------------------------------------------------
// GetStatus
bool TLT400::Receive(AnsiString RecvMsg, int iLen)
{
	if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_RecvMsg += RecvMsg;

	int iPos = 0;
	iPos = m_RecvMsg.AnsiPos("\r\n");

	AnsiString sCmd;
	int iNextPos = 0;
	while ( iPos > 0  ) {
		sCmd = m_RecvMsg.SubString(1, iPos-1);
		iNextPos = iPos + 2;
		LogMessage("[LT400]<=" + sCmd);

		sCmd = TrimHEX(sCmd);

		Decode(sCmd, sCmd.Length());

		m_RecvMsg = m_RecvMsg.SubString(iNextPos, m_RecvMsg.Length() - (iNextPos-1));
		m_RecvMsg = m_RecvMsg.Trim();	// trim \r\n<body>\r\n

		iPos = 0;
		iPos = m_RecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
bool TLT400::Receive(BYTE *pBuffer, int iLen)
{
	bool bResult = true;

	// do something..

	return bResult;
};

//---------------------------------------------------------------------------
// GetStatus
bool TLT400::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;
	bool bResult = false;

	try {
		FormMain->WaterPort->FlushOutBuffer();	// output buffer clear

		FormMain->WaterPort->PutString(RecvMsg);
		LogMessage("[LT400]=>" + RecvMsg);
		bResult = true;
	} catch ( Exception &e ) {
		LogMessage("[LT400]=>" + e.Message);
		bResult = false;
	}

	return bResult;
}

//---------------------------------------------------------------------------
// Send Command to Level TROLL Device
bool TLT400::Send(char Buffer[], int iLen)
{
	if ( m_pPort == NULL ) return false;
	bool bResult = false;

	try {
		FormMain->WaterPort->PutString(Buffer);
		LogMessage("[LT400]=>" + AnsiString(Buffer, iLen));
		bResult = true;
	} catch ( Exception &e ) {
		LogMessage("[LT400]=>" + e.Message);
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
bool TLT400::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CalAvgMinMaxWaterLevel();	// 평균 수위 등의 자료를 구한다.

	// 보정된 수위 정보를 구한다.

	double xWaterLevel, xTemperature, xDepth, xPressure;
	double ATemperature, APressure;
	double ADepth;

	xWaterLevel = xTemperature = xDepth = xPressure = 0;
	ATemperature = APressure = ADepth = 0;

	GetData(xWaterLevel, xTemperature, xDepth, xPressure);   	// LT400에서 수집한 데이터
	FormMain->m_pAtmos->GetData(ATemperature, APressure);		// 데기압센서에서 수집한 데이터

	// GetData & Write File
	STData Data;
	memset(&Data, 0, sizeof(STData));

	Data.WaterLevel   = xWaterLevel,
	Data.Depth        = xDepth;
	Data.Temperature  = xTemperature;
	//Data.Pressure     = xPressure;    	// PSI => bar => mbar
	Data.Pressure     = (68.94733 * xPressure) * 0.01;     	// PSI => mbar => dbar      1 PSI => 68.94733 mBar
	Data.Offset       = theEnv.m_WaterOffset;
	Data.ATemperature = ATemperature;
	Data.APressure    = APressure;
	Data.ABaseAtmos   = theEnv.m_BaseAtmos;

	ComputeDept(Data);	// CPressure, CDept 계산함.

	if ( xPressure != 0 && APressure != 0 ) { // LT400의 기압, 대기압센서가 정상적으로 수집한 경우
		m_RWaterLevel.MeasureDate = MeasureDate;
		m_RWaterLevel.MeasureTime = MeasureTime;
		m_RWaterLevel.WaterKind   = WaterKind;
		m_RWaterLevel.AvgWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.MinWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.MaxWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.Value01     = Data.Temperature;		// LT400의 온도
		m_RWaterLevel.Value02     = Data.CPressure;			// 보정된 압력
		m_RWaterLevel.Value03     = Data.CDepth; 			// 보정된 수위
		m_RWaterLevel.Value04     = Data.WaterLevel;		// LT400 수위 데이터(이미 Offset값을 적용한 값)
		m_RWaterLevel.Value05     = Data.Pressure;			// LT400 압력
		m_RWaterLevel.Value06     = Data.Depth;				// LT400 Dept
		m_RWaterLevel.Value07     = Data.APressure;			// 대기압 압력
		m_RWaterLevel.Value08     = Data.ATemperature;		// 대기상태 온도
		//m_RWaterLevel.Value09     = ;
		m_RWaterLevel.Value10     = theEnv.m_WaterOffset;	// 수위 Offset
		m_RWaterLevel.Value11     = theEnv.m_BaseAtmos;		// 기준대기압
	} else {	// LT400의 기압, 대기압센서가 정상적으로 수집하지 못한 경우 => LT400의 데이터를 그대로 사용한다.
		m_RWaterLevel.MeasureDate = MeasureDate;
		m_RWaterLevel.MeasureTime = MeasureTime;
		m_RWaterLevel.WaterKind   = WaterKind;
		m_RWaterLevel.AvgWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.MinWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.MaxWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
		m_RWaterLevel.Value01     = Data.Temperature;		// LT400의 온도
		m_RWaterLevel.Value02     = Data.Pressure;			// 보정된 압력
		m_RWaterLevel.Value03     = Data.Depth; 			    // 보정된 수위
		m_RWaterLevel.Value04     = Data.WaterLevel;		// LT400 수위 데이터
		m_RWaterLevel.Value05     = Data.Pressure;			// LT400 압력
		m_RWaterLevel.Value06     = Data.Depth;				// LT400 Dept
		m_RWaterLevel.Value07     = Data.APressure;			// 대기압 압력
		m_RWaterLevel.Value08     = Data.ATemperature;		// 대기상태 온도
		//m_RWaterLevel.Value09     = ;
		m_RWaterLevel.Value10     = theEnv.m_WaterOffset;	// 수위 Offset
		m_RWaterLevel.Value11     = theEnv.m_BaseAtmos;		// 기준대기압
	}

	bool bResult = false;
	bResult = m_RWaterLevel.Insert();

	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
void TLT400::DB2Value()
{
	AvgWaterLevel         = m_RWaterLevel.AvgWaterLevel;
	MinWaterLevel         = m_RWaterLevel.MinWaterLevel;
	MaxWaterLevel         = m_RWaterLevel.MaxWaterLevel;
	Temperature           = m_RWaterLevel.Value01;
	Pressure              = m_RWaterLevel.Value02;
	Depth                 = m_RWaterLevel.Value03;
	Offset                = theEnv.m_WaterOffset;
}

//---------------------------------------------------------------------------
// GetStatus
void TLT400::Value2DB()
{
//	m_RWaterLevel.AvgWaterLevel  = AvgWaterLevel;
//	m_RWaterLevel.MinWaterLevel  = MinWaterLevel;
//	m_RWaterLevel.MaxWaterLevel  = MaxWaterLevel;
//	m_RWaterLevel.Value01        = Temperature;
//	m_RWaterLevel.Value02        = Pressure;
//	m_RWaterLevel.Value03        = Dept;
//	m_RWaterLevel.Value10        = Offset;
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TLT400::GetWaterLevel()
{
	//WaterLevel = AvgWaterLevel + Offset; 	//2014.10.01

	return AvgWaterLevel;	// AvgWaterLevel => Dept + Offset
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TLT400::GetArea()
{
	//WaterLevel = AvgWaterLevel + Offset;
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
bool TLT400::CalAvgMinMaxWaterLevel()
{
	double AvgWaterLevel = 0;
	double MinWaterLevel = 0;
	double MaxWaterLevel = 0;
	double Temperature   = 0;
	double Pressure      = 0;
	double Depth         = 0;

	if ( m_pList ) {
		STLT400Data *pData = NULL;
		int iValidCount = 0;
		bool bFirst = true;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STLT400Data *) m_pList->Items[i];
			}catch ( Exception &e){
				return false;
			}

			if ( pData->Depth < theEnv.m_dWaterLowerBound ||
				 pData->Depth > theEnv.m_dWaterUpperBound )  {

				UnicodeString Str;
				Str = Str.sprintf(L"Water Dept Data is out of range. %10.2f, LowerBound: %10.2f Uppderbound: %10.2f", pData->Depth, theEnv.m_dWaterLowerBound,
								   theEnv.m_dWaterUpperBound );
				LogMessage(Str);
				continue;	// 유효범위 이외의 값이 오면 처리하지 않는다.
			}

			if ( bFirst ) {
				MinWaterLevel = pData->Depth;
				MaxWaterLevel = pData->Depth;
				bFirst = false;
			}

			Temperature += pData->Temperature;
			Pressure    += pData->Pressure;
			Depth       += pData->Depth;
			iValidCount++;

			if ( MinWaterLevel < pData->Depth ) MinWaterLevel = pData->Depth;
			if ( MaxWaterLevel > pData->Depth ) MaxWaterLevel = pData->Depth;
		}

		//if ( m_pList->Count > 0 ) {
		if ( iValidCount > 0 ) {
			AvgWaterLevel = (Depth / iValidCount) + theEnv.m_WaterOffset;
			MinWaterLevel = MinWaterLevel + theEnv.m_WaterOffset;
			MaxWaterLevel = MaxWaterLevel + theEnv.m_WaterOffset;
			Temperature   =  Temperature / iValidCount;
			Pressure      =  Pressure / iValidCount;
			Depth         =  Depth / iValidCount;
		} else {
			// nothing to do..
		}

	} else {
		// nothing to do..
	}

	this->AvgWaterLevel = AvgWaterLevel;
	this->MinWaterLevel = MinWaterLevel;
	this->MaxWaterLevel = MaxWaterLevel;
	this->Temperature   = Temperature;
	this->Pressure      = Pressure;
	this->Depth         = Depth;
	this->Offset        = theEnv.m_WaterOffset;

	return true;
}

//---------------------------------------------------------------------------
// DecodeData
bool TLT400::DecodeData()
{
	STLT400Data *pData = new STLT400Data();

	//pData->Temperature = 0.0;
	//pData->Pressure    = 0.0;
	//pData->Dept        = 0.0;	// Decode Protocol Data

	pData->Temperature = Analyze(TempAck);
	pData->Pressure    = Analyze(PressureAck);
	pData->Depth       = Analyze(DeptAck);	// Decode Protocol Data

	m_pList->Add(pData);

	return true;
}

//---------------------------------------------------------------------------
// Analyze String data to double data
double TLT400::Analyze(AnsiString sData)
{
	if ( sData.Length() < 41 ) return 0;

	AnsiString s = sData.SubString(8, 8);	//

	AnsiString sBin = Hex2Bin(s);

	float fValue = 0;
	fValue = Bin2IEEE745(sBin);

	return fValue;
}

//---------------------------------------------------------------------------
void TLT400::GetData(double &W, double &T, double &D,  double &P)
{
	W  = this->AvgWaterLevel;
	T  = this->Temperature;
	D  = this->Depth;
	P  = this->Pressure;
}


//---------------------------------------------------------------------------
bool TLT400::CheckCRC(byte *pBuffer, int Length)
{
	bool bResult = false;
	byte CRC = 0x00;
	for ( int i = 0 ; i < Length-1 ; i++ ) {
		CRC += *(pBuffer+i);
	}
	CRC = (CRC ^ 0xFF) + 0x01;

	if ( CRC == *(pBuffer+(Length-1))) {
		bResult = true;
	} else {
		bResult = false;
	}
	return bResult;
}

#pragma package(smart_init)
