//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RnsWater.h"
#include "AniThread.h"
#include "../HydroMeter/Airmar.h"
#include "../Common.h"	// Refer AverageAngle Class
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
TRnsWater::TRnsWater()
{
	memset(Buffer, 0x00, sizeof(Buffer));
	Buffering = false;
	Pos = 0;
	Length = 0; // Payload length

	memset(&RnsWater, 0x00, sizeof(RnsWater));
	memset(&AirmarData, 0x00, sizeof(AirmarData));

	AvgWaterLevel = 0.0f;
	MinWaterLevel = 0.0f;
	MaxWaterLevel = 0.0f;

	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = RNSWATER_STR;	// WaterLevel Sensor type
	Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;
	bLeak          = false;	// Leak or not-

	m_pList        = new TList();
	m_pAirmarList  = new TList();
	m_Count        = 0;
}

//---------------------------------------------------------------------------
// Destructor
TRnsWater::~TRnsWater()
{
	// Resource lease
	if ( m_pList ) {
		STRnsWater *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			pData = (STRnsWater *) m_pList->Items[i];
			delete pData;
		}
		m_pList->Clear();
		delete m_pList;
	}

	if ( m_pAirmarList ) {
		STAirmarData *pData = NULL;
		for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
			pData = (STAirmarData *) m_pList->Items[i];
			delete pData;
		}
		m_pAirmarList->Clear();
		delete m_pAirmarList;
	}

	// other resource release
}

//---------------------------------------------------------------------------
// Clear
void TRnsWater::Clear()
{
	memset(Buffer, 0x00, sizeof(Buffer));
	Buffering = false;
	Pos = 0;
	Length = 0; // Payload length

	memset(&RnsWater, 0x00, sizeof(RnsWater));
	memset(&AirmarData, 0x00, sizeof(AirmarData));

	AvgWaterLevel = 0.0f;
	MinWaterLevel = 0.0f;
	MaxWaterLevel = 0.0f;

	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering   = false;			// Buffering or not
	m_RecvMsg      = "";

	//m_pPort        = NULL;			// ComPort
	m_enStep       = enSensorIdle;	// Sensor Status

	MeasureDate    = "";
	MeasureTime    = "";
	WaterKind      = RNSWATER_STR;	// WaterLevel Sensor type
	//Offset         = 0;
	WaterLevel     = 0;
	Area           = 0;
	bLeak          = false;	// Leak or not-	

	// Resource lease
	if ( m_pList ) {
		STRnsWater *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			pData = (STRnsWater *) m_pList->Items[i];
			delete pData;
		}
		m_pList->Clear();
		//delete m_pList;
	}

	if ( m_pAirmarList ) {
		STAirmarData *pData = NULL;
		for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
			pData = (STAirmarData *) m_pAirmarList->Items[i];
			delete pData;
		}
		m_pAirmarList->Clear();
		//delete m_pAirmarList;
	}

	m_Count        = 0;
}

//---------------------------------------------------------------------------
// ClearList
bool TRnsWater::ClearList()
{
	if ( m_pList ) {
		STRnsWater *pData = NULL;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STRnsWater *) m_pList->Items[i];
				delete pData;
			} catch ( Exception &e) {
             	break;
			}
		}
		m_pList->Clear();
	}

	if ( m_pAirmarList ) {
		STAirmarData *pData = NULL;
		for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
			pData = (STAirmarData *) m_pAirmarList->Items[i];
			delete pData;
		}
		m_pAirmarList->Clear();
	}

	return true;
}

//---------------------------------------------------------------------------
// Init
void TRnsWater::Init(TApdComPort *pPort)
{
	m_pPort = pPort;
}

//---------------------------------------------------------------------------
// Start
bool TRnsWater::Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst)
{
	if ( bFirst ) {	// 처음 실행하면 Atmos, Airmar Clear 시킨다.
		// 처음 시작할 때..
		Clear();		// Status Clear
		ClearList();	// Clear WaterLevel List Data

		// 대기압 Clear
		if ( theEnv.m_bAtmosEnable ) {
			if ( FormMain->m_pAtmos != NULL ) {
				FormMain->m_pAtmos->Clear();
				FormMain->m_pAtmos->Start(MeasureDate, MeasureTime, bFirst);	// 대기압센서를 Start한다.
			}
		}
	}

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;

	bool bResult = false;
	bResult = SendRnsWaterReq();	// Send Data Request Message to RMeasureWL

	USHORT h, n, s, ss;
	Now().DecodeTime(&h, &n, &s, &ss);
	if ( s < 10 ) {	// xx:xx:[0~9] => 매분 정각에 치러헥 처리한다.
		if ( theEnv.m_bAirmarEnable ) {		// RnsWater가 Airmar 데이터를 수집한다면
			bResult = SendAirmarDataReq();	// RnsWater에 Airmar 데이터를 요청한다.
		}
	}

	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TRnsWater::Finish()
{
	if ( theEnv.m_bAtmosEnable ) {
		if ( FormMain->m_pAtmos != NULL ) FormMain->m_pAtmos->Finish();	// 대기압 수집 종료
	}

	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorFinish;

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRnsWater::Decode(AnsiString RecvMsg, int iLen)
{
	// nothing to do
	
	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRnsWater::Decode(BYTE *pBuffer, int iLen)
{
	int iResult = CheckFrame(pBuffer, iLen);
	
	if ( iResult == FRAME_OK ) {
		// nothing to do.
	} else if ( iResult == FRAME_STX_ERROR ) {
		LogMessage("RnsWater STX error..");
		return false;
	} else if ( iResult == FRAME_ETX_ERROR ) {
		LogMessage("RnsWater ETX error..");
		return false;
	} else if ( iResult == FRAME_CRC_ERROR ) {
		LogMessage("RnsWater CRC error..");
		return false;
	} else {
		LogMessage("RnsWater unknown Frame error.. ");
		return false;
	}
	
	// Frame Ok
	BYTE Type = 0x00;
	int  PayloadLength = 0;
	BYTE DeviceType = 0x00;	// WaterLevel Sensor device type
	
	Type          = *(pBuffer+1); 	// Message Type
	PayloadLength = *(pBuffer+2);	// Payload Length
	DeviceType    = *(pBuffer+3);	// WaterLevel Device Type
	
	if ( Type == RnsWaterRes ) {	// 0x11 RnsWaterRes
		if ( DeviceType == 0x00 ) {	// Device Type => 0x00 => DEVICE NONE
			LogMessage("RnsWater no water device..");
		} else if ( DeviceType == 0x01 || DeviceType == 0x02 || DeviceType == 0x03 ||
					DeviceType == 0x04 || DeviceType == 0x05 || DeviceType == 0x06 ||
					DeviceType == 0x07 || DeviceType == 0x08 )  {	// 1: VegaPuls(4~20mA), 2:VegaPuls(485Converter), 3: VegaPuls(Rs-485)
	
			STRnsWater *pRnsWater = new STRnsWater();
			memcpy(pRnsWater, pBuffer+3, PayloadLength);	// Payload Copy
			
			m_pList->Add(pRnsWater);	// Add list 
			
			LogMessage("RnsWater waterlevel added.." + FloatToStrF(pRnsWater->Depth + theEnv.m_WaterOffset, ffFixed, 5, 2 ));
		} else {
			// Unknown device type
			LogMessage("RnsWater unknown device type..");
		}
	} else if ( Type == AirmarDataRes ) {	// Airmar Data 요구에 대한 응답 => Term: 1분 간격으로 요청함.

		STAirmarData *pAirmarData = new STAirmarData();
		memcpy(pAirmarData, pBuffer+3, PayloadLength);

		m_pAirmarList->Add(pAirmarData);

		LogMessage("RnsWater wind added.." + FloatToStrF(pAirmarData->Wind, ffFixed, 5, 2));

	} else if ( Type == RnsWaterSetupRes ) {	// 현재 수위 설정에 대한 응답
		LogMessage("RnsWaterSetupRes.. Acked..");
		// 이거 외에는 할 거 없음
		// 수신되는 Depth는 있지만 => RADS 에서 수위 설정값을 전송하지는 않음.
	} else {
		LogMessage("RnsWater unknown message type..");
	}


	m_TimeStamp = Now();	// 데이터 수신 받은 시간 갱신
	
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRnsWater::Receive(AnsiString RecvMsg, int iLen)
{
	// nothing to do..
	
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRnsWater::Receive(BYTE *pBuffer, int iLen)
{
	static BYTE Length = 0;

	for ( int i = 0 ; i < iLen ; i++ ) {
		if ( Buffering ) {	// Buffering 
			Buffer[Pos++] = *(pBuffer+i);
			if ( Pos < 3 ) {
				// nothing to do.. => continue buffering
			} else if  ( Pos == 3 ) {
				Length = Buffer[2];
			} else {
				if ( Pos == Length + 4 + 1 ) {
					
					Decode(Buffer, Pos);	// Main Process
					
					memset(Buffer, 0x00, sizeof(Buffer));
					Buffering = false;
					Pos = 0;
					Length = 0;
				} else {
					// continue buffering 	
				}
			}
		} else {	// Not Buffering .. 
			if ( *(pBuffer+i) == STX ) {
				memset(Buffer, 0x00, sizeof(Buffer));
				Buffering = false;
				Pos = 0;
				Length = 0;	
				
				Buffer[Pos++] = *(pBuffer+i);	// Frame Buffering start 
				Buffering = true;
			} else {
				// discard data
			}
		}	
		
		if ( Pos >= sizeof(Buffer)) {	// buffer overflow check
			memset(Buffer, 0x00, sizeof(Buffer));
			Buffering = false;
			Pos = 0;
			Length = 0;			
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// Send
bool TRnsWater::Send(AnsiString RecvMsg, int iLen)
{
	bool bResult =  true;
	
	// nothing to do.. 
	
	return bResult;
}

//---------------------------------------------------------------------------
// Send
bool TRnsWater::Send(BYTE *pBuffer, int iLen)
{
	if ( m_pPort == NULL ) return false;
	bool bResult = false;

	try {
		//m_pPort->FlushOutBuffer();	// output buffer clear

		m_pPort->PutBlock(pBuffer, iLen);
		//LogMessage("[RnsWater] data request..");
		bResult = true;
	} catch ( Exception &e ) {
		LogMessage("[RnsWater]=>" + e.Message);
		bResult = false;
	}

	return bResult;
}

//---------------------------------------------------------------------------
// Send Command to Level TROLL Device
bool TRnsWater::Send(char Buffer[], int iLen)
{
	bool bResult =  true;
	
	// nothing to do.. 
	
	return bResult;
}

//---------------------------------------------------------------------------
// Save
bool TRnsWater::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CaculateWaterLevel();	// 평균 수위 등의 자료를 구한다.

	// 대기압 센서에 의한 수위 보정
	if ( theEnv.m_bAtmosEnable ) {	// 대기압 보정이 enable 되어 있으면

		double xWaterLevel, xTemperature, xDepth, xPressure;
		double ATemperature, APressure;
		double ADepth;

		xWaterLevel = xTemperature = xDepth = xPressure = 0;
		ATemperature = APressure = ADepth = 0;

		GetData(xWaterLevel, xTemperature, xDepth, xPressure);   	// RnsWater에서 수집한 수위 데이터(MK21)
		FormMain->m_pAtmos->GetData(ATemperature, APressure);		// 데기압센서에서 수집한 데이터

		// GetData & Write File
		STData Data;
		memset(&Data, 0, sizeof(STData));

		Data.WaterLevel   = xWaterLevel,		// 압력식 수위계의 수위 데이터
		Data.Depth        = xDepth;				// 압력식 수위계의 측정 Depth
		Data.Temperature  = xTemperature;		// 수위계 온도(수온)
		//Data.Pressure     = xPressure;    	// PSI => bar => mbar
		Data.Pressure     = (68.94733 * xPressure) * 0.01;     	// PSI => mbar => dbar      1 PSI => 68.94733 mBar
		Data.Offset       = theEnv.m_WaterOffset;	// 수위 Offset
		Data.ATemperature = ATemperature;		// 측정 대기온도
		Data.APressure    = APressure;			// 측정 대기압
		Data.ABaseAtmos   = theEnv.m_BaseAtmos;	// 기준 대기압

		ComputeDept(Data);	// CPressure, CDept 계산함.

		if ( xPressure != 0 && APressure != 0 ) { // LT400의 기압, 대기압센서가 정상적으로 수집한 경우
			m_RWaterLevel.MeasureDate = MeasureDate;
			m_RWaterLevel.MeasureTime = MeasureTime;
			m_RWaterLevel.WaterKind   = WaterKind;
			m_RWaterLevel.AvgWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.MinWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.MaxWaterLevel = Data.CDepth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.Value01     = Data.Temperature;		// 수온 온도
			m_RWaterLevel.Value02     = Data.CPressure;			// 보정된 압력(새로 계산)
			m_RWaterLevel.Value03     = Data.CDepth; 			// 보정된 수위(새로 계산)
			m_RWaterLevel.Value04     = Data.WaterLevel;		// 센서의 수위 데이터(Depth+Offset)
			m_RWaterLevel.Value05     = Data.Pressure;			// 센서의 압력(PSI)
			m_RWaterLevel.Value06     = Data.Depth;				// 센서의 Depth
			m_RWaterLevel.Value07     = Data.APressure;			// 대기압 (mbar)
			m_RWaterLevel.Value08     = Data.ATemperature;		// 대기상태 온도
			m_RWaterLevel.Value09     = RnsWater.Salinity;		// 염도 데어터
			m_RWaterLevel.Value10     = theEnv.m_WaterOffset;	// 수위 Offset
			m_RWaterLevel.Value11     = theEnv.m_BaseAtmos;		// 기준대기압
			m_RWaterLevel.Value12     = RnsWater.Type;			// 장비 종류, 0:None, 1:VegaPuls(4~20mA), 8:Mk21(하이듸로넷),..
		} else {	// LT400의 기압, 대기압센서가 정상적으로 수집하지 못한 경우 => LT400의 데이터를 그대로 사용한다.
			m_RWaterLevel.MeasureDate = MeasureDate;
			m_RWaterLevel.MeasureTime = MeasureTime;
			m_RWaterLevel.WaterKind   = WaterKind;
			m_RWaterLevel.AvgWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.MinWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.MaxWaterLevel = Data.Depth + theEnv.m_WaterOffset; 	// 보정된 수위
			m_RWaterLevel.Value01     = Data.Temperature;		// 센서의 수온
			m_RWaterLevel.Value02     = Data.Pressure;			// 보정된 압력
			m_RWaterLevel.Value03     = Data.Depth; 			// 보정된 수위
			m_RWaterLevel.Value04     = Data.WaterLevel;		// 센서의 수위 데이터(Depth+Offset)
			m_RWaterLevel.Value05     = Data.Pressure;			// 센서의 압력
			m_RWaterLevel.Value06     = Data.Depth;				// 센서의 Depth
			m_RWaterLevel.Value07     = Data.APressure;			// 대기압 압력
			m_RWaterLevel.Value08     = Data.ATemperature;		// 대기상태 온도
			m_RWaterLevel.Value09     = RnsWater.Salinity;		// 염도
			m_RWaterLevel.Value10     = theEnv.m_WaterOffset;	// 수위 Offset
			m_RWaterLevel.Value11     = theEnv.m_BaseAtmos;		// 기준대기압
			m_RWaterLevel.Value12     = RnsWater.Type;			// 장비 종류, 0:None, 1:VegaPuls(4~20mA), 8:Mk21(하이듸로넷),..
		}
	} else {	// 대기압 보정 없이 저장
		Value2DB();
	}

	// Airmar Check
	if ( theEnv.m_bAirmarEnable ) {

		CaculateAirmar();	// 버퍼링된 Airmar 데이트를 평균한다.

		m_RWaterLevel.Value13 = AirmarData.Direction;	// Direction
		m_RWaterLevel.Value14 = AirmarData.Wind;		// Wind Speed
		m_RWaterLevel.Value15 = AirmarData.WindGust;	// WindGust
		m_RWaterLevel.Value16 = AirmarData.Pressure;	// Pressure(Atmospere)(bar)
		m_RWaterLevel.Value17 = AirmarData.Temperature;	// Temperature(c)
		m_RWaterLevel.Value18 = AirmarData.Humidity;	// Humidity(%)
	} else {
		m_RWaterLevel.Value13 = 0.0f;	// Direction
		m_RWaterLevel.Value14 = 0.0f;	// Wind Speed
		m_RWaterLevel.Value15 = 0.0f;	// WindGust
		m_RWaterLevel.Value16 = 0.0f;	// Pressure(Atmospere)(bar)
		m_RWaterLevel.Value17 = 0.0f;	// Temperature(c)
		m_RWaterLevel.Value18 = 0.0f;	// Humidity(%)
	}

	bool bResult = false;
	m_RWaterLevel.MeasureDate = MeasureDate;
	m_RWaterLevel.MeasureTime = MeasureTime;
	bResult = m_RWaterLevel.Insert();

	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
void TRnsWater::DB2Value()
{
//	AvgWaterLevel         = m_RWaterLevel.AvgWaterLevel;
//	MinWaterLevel         = m_RWaterLevel.MinWaterLevel;
//	MaxWaterLevel         = m_RWaterLevel.MaxWaterLevel;
//	Temperature           = m_RWaterLevel.Value01;
//	Pressure              = m_RWaterLevel.Value02;
//	Depth                 = m_RWaterLevel.Value03;
//	Offset                = theEnv.m_WaterOffset;
}

//---------------------------------------------------------------------------
// GetStatus
void TRnsWater::Value2DB()
{
	m_RWaterLevel.WaterKind      = this->WaterKind;
	m_RWaterLevel.AvgWaterLevel  = AvgWaterLevel;	// Depth + Offset
	m_RWaterLevel.MinWaterLevel  = MinWaterLevel;	// Depth + Offset
	m_RWaterLevel.MaxWaterLevel  = MaxWaterLevel;   // Depth + Offset
	m_RWaterLevel.Value01        = RnsWater.Temperature;
	m_RWaterLevel.Value02        = RnsWater.Pressure;	// CPressure
	m_RWaterLevel.Value03        = RnsWater.Depth;		// CDepth
	m_RWaterLevel.Value04        = RnsWater.Depth + theEnv.m_WaterOffset;	// WaterLevel : Depth + Offset
	m_RWaterLevel.Value05        = RnsWater.Pressure;	// Pressure
	m_RWaterLevel.Value06        = RnsWater.Depth;		// Depth
	m_RWaterLevel.Value07        = 0.0f;				// 대기압
	m_RWaterLevel.Value08        = 0.0f;				// 대기온도
	m_RWaterLevel.Value09        = RnsWater.Salinity; 	// Salinity
	m_RWaterLevel.Value10        = theEnv.m_WaterOffset;	// Offset
	m_RWaterLevel.Value11        = theEnv.m_BaseAtmos;		// 대긱압 기준대기압
	m_RWaterLevel.Value12        = RnsWater.Type;	// WaterLevel Sensor Device Type => 0,1,2,3,..
	m_RWaterLevel.Value13        = 0.0f;
	m_RWaterLevel.Value14        = 0.0f;
	m_RWaterLevel.Value15        = 0.0f;
	m_RWaterLevel.Value16        = 0.0f;
	m_RWaterLevel.Value17        = 0.0f;
	m_RWaterLevel.Value18        = 0.0f;
	m_RWaterLevel.Value19        = 0.0f;
	m_RWaterLevel.Value20        = 0.0f;
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TRnsWater::GetWaterLevel()
{
	//WaterLevel = AvgWaterLevel + Offset; 	//2014.10.01

	return AvgWaterLevel;	// AvgWaterLevel => Dept + Offset
}

//---------------------------------------------------------------------------
// GetArea() 단면적을 가져온다.
double TRnsWater::GetArea()
{
//	WaterLevel = AvgWaterLevel + Offset;
//	WaterLevel = AvgWaterLevel;
//
//	if ( WaterLevel < m_pRFlowHA->MinH || WaterLevel > m_pRFlowHA->MaxH ) { // 수위 단면적 곡선의 신뢰 구간을 벗어나면 의미 없는 것으로
//		Area = 0;
//	} else {
//		Area = (m_pRFlowHA->A6 * Power(WaterLevel,6)) +
//			   (m_pRFlowHA->A5 * Power(WaterLevel,5)) +
//			   (m_pRFlowHA->A4 * Power(WaterLevel,4)) +
//			   (m_pRFlowHA->A3 * Power(WaterLevel,3)) +
//			   (m_pRFlowHA->A2 * Power(WaterLevel,2)) +
//			   (m_pRFlowHA->A1 * Power(WaterLevel,1)) +
//			   (m_pRFlowHA->A0);
//	}
	Area = 0.0f;
	return Area;
}

//---------------------------------------------------------------------------
// SendRnsWaterReq
bool TRnsWater::SendRnsWaterReq()
{
	BYTE SendBuffer[128] = "";

	SendBuffer[0] = STX;
	SendBuffer[1] = RnsWaterReq;	// 0x01 RnsWaterReq
	SendBuffer[2] = 0x00;			// Length
	
	BYTE crc = 0x00;
	for ( int i = 0 ; i < 3 ; i++ ) {
		crc ^= SendBuffer[i];
	}
	
	crc = crc & 0x7F;
	
	SendBuffer[3] = crc;
	SendBuffer[4] = ETX;
	
	bool bResult = false;
	bResult = Send(SendBuffer, 5);
	LogMessage("[RnsWater] Waterlevel data request..");

	return bResult;
}

//---------------------------------------------------------------------------
// SendAirmarDataReq
bool TRnsWater::SendAirmarDataReq()
{
	BYTE SendBuffer[128] = "";

	SendBuffer[0] = STX;
	SendBuffer[1] = AirmarDataReq;	// 0x02 AirmarDataReq
	SendBuffer[2] = 0x00;			// Length

	BYTE crc = 0x00;
	for ( int i = 0 ; i < 3 ; i++ ) {
		crc ^= SendBuffer[i];
	}

	crc = crc & 0x7F;

	SendBuffer[3] = crc;
	SendBuffer[4] = ETX;

	bool bResult = false;
	bResult = Send(SendBuffer, 5);
	LogMessage("[RnsWater] Airmar data request..");

	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
bool TRnsWater::CaculateWaterLevel()
{
	BYTE   Type = 0x00;
	double AvgWaterLevel = 0.0f;
	double MinWaterLevel = 0.0f;
	double MaxWaterLevel = 0.0f;
	double Temperature   = 0.0f;
	double Pressure      = 0.0f;
	double Depth         = 0.0f;
	double Salinity      = 0.0f;

	if ( m_pList ) {
		STRnsWater *pData = NULL;
		int iValidCount = 0;
		bool bFirst = true;
		for ( int i = 0 ; i < m_pList->Count ; i++ ) {
			try {
				pData = (STRnsWater *) m_pList->Items[i];
			} catch ( Exception &e){
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
			
			Type         = pData->Type;
			Temperature += pData->Temperature;
			Pressure    += pData->Pressure;
			Depth       += pData->Depth;
			Salinity    += pData->Salinity;
			iValidCount++;

			if ( MinWaterLevel < pData->Depth ) MinWaterLevel = pData->Depth;
			if ( MaxWaterLevel > pData->Depth ) MaxWaterLevel = pData->Depth;
		} // end of for

		if ( iValidCount > 0 ) {
			AvgWaterLevel = (Depth / iValidCount) + theEnv.m_WaterOffset;
			MinWaterLevel = MinWaterLevel + theEnv.m_WaterOffset;
			MaxWaterLevel = MaxWaterLevel + theEnv.m_WaterOffset;
			Temperature   = Temperature  / iValidCount;	// Avg Temperature
			Pressure      = Pressure     / iValidCount;	// Avg Pressure
			Depth         = Depth        / iValidCount;	// Avg Depth
			Salinity      = Salinity     / iValidCount;	// Avg Salinity
		} else {
			// nothing to do..
		}

	} else {
		// nothing to do..
	}

	this->AvgWaterLevel = AvgWaterLevel;
	this->MinWaterLevel = MinWaterLevel;
	this->MaxWaterLevel = MaxWaterLevel;
	this->RnsWater.Type        = Type;
	this->RnsWater.Temperature = Temperature;
	this->RnsWater.Pressure    = Pressure;
	this->RnsWater.Depth       = Depth;
	this->RnsWater.Salinity    = Salinity;
	this->Offset               = theEnv.m_WaterOffset;

	return true;
}

//---------------------------------------------------------------------------
bool TRnsWater::CaculateAirmar()
{
	STAirmarData xAirmarData;
	memset(&xAirmarData, 0x00, sizeof(STAirmarData));	// Clear

	AverageAngle AA(AverageAngle::DEGREES);
	AA.reset();

	if ( m_pAirmarList ) {
		STAirmarData *pData = NULL;
		int iValidCount = 0;
		bool bFirst = true;
		for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
			try {
				pData = (STAirmarData *) m_pAirmarList->Items[i];
			} catch ( Exception &e){
				return false;
			}

			if ( bFirst ) {
				xAirmarData.WindGust = pData->WindGust;	// 최대 풍속 기억
			}

			xAirmarData.Wind           += pData->Wind;
			xAirmarData.WindUnit        = pData->WindUnit;
			//xAirmarData.WindGust      = pData->WindGust;
			xAirmarData.Direction      += pData->Direction;	// 단위 변환 Degree => Radian
			AA.add(pData->Direction);
			xAirmarData.DirectionType   = pData->DirectionType;
			xAirmarData.Temperature    += pData->Temperature;
			xAirmarData.TemperatureUnit = pData->TemperatureUnit;
			xAirmarData.Humidity       += pData->Humidity;
			xAirmarData.Pressure       += pData->Pressure;
			xAirmarData.PressureUnit    = pData->PressureUnit;

			if ( xAirmarData.WindGust > pData->WindGust ) xAirmarData.WindGust = pData->WindGust;	// 최대 풍속 갱신

			iValidCount++;
		} // end of for

		if ( iValidCount > 0 ) {
			AirmarData.Wind            = xAirmarData.Wind / iValidCount;          	// iValidCount;
			AirmarData.WindUnit        = xAirmarData.WindUnit;
			AirmarData.WindGust        = xAirmarData.WindGust;			// 최대 유속
			AirmarData.Direction       = AA.getAverage();				// 풍향(0~360)
			AirmarData.DirectionType   = xAirmarData.DirectionType;
			AirmarData.Temperature     = xAirmarData.Temperature / iValidCount;       // iValidCount;
			AirmarData.TemperatureUnit = xAirmarData.TemperatureUnit;
			AirmarData.Humidity        = xAirmarData.Humidity / iValidCount;
			AirmarData.Pressure        = xAirmarData.Pressure / iValidCount;          // iValidCount;
			AirmarData.PressureUnit    = xAirmarData.PressureUnit;
		} else {
			memset(&xAirmarData, 0x00, sizeof(STAirmarData));
		}

	} else {
		// nothing to do..
	}
	return true;
}

//---------------------------------------------------------------------------
int TRnsWater::CheckFrame(byte *pBuffer, int Length)
{
	if ( *(pBuffer+0) != STX )          return FRAME_STX_ERROR;	// STX Error
	if ( *(pBuffer+(Length-1)) != ETX ) return FRAME_ETX_ERROR;	// ETX Error

	BYTE crc = 0x00;
	for ( int i = 0 ; i < Length-2 ; i++ )
		crc ^= *(pBuffer + i);

	crc = crc & 0x7F;

	if ( *(pBuffer+(Length-2)) != crc ) return FRAME_CRC_ERROR;	// CRC Error

	return FRAME_OK;
}

//---------------------------------------------------------------------------
void TRnsWater::GetData(double &W, double &T, double &D,  double &P)
{
	W  = RnsWater.Depth + theEnv.m_WaterOffset;	// Unit: meter
	T  = RnsWater.Temperature;	// Unit: 'C
	D  = RnsWater.Depth;		// Unit: Meter
	P  = RnsWater.Pressure; 	// Unit: PSI
}

#pragma package(smart_init)
