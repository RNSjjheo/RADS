//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RnsWater.h"
#include "AniThread.h"
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
	Clear();	// Status Clear
	if ( bFirst ) ClearList();	// Clear WaterLevel List Data

//	if ( m_pPort != NULL ) {
//		m_pPort->FlushInBuffer();	// Input buffer clear
//		m_pPort->FlushOutBuffer();	// Output buffer clear
//	}

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;

	bool bResult = false;
	bResult = SendRnsWaterReq();	// Send Data Request Message to RMeasureWL

	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	return bResult;
}

//---------------------------------------------------------------------------
// Finish 
bool TRnsWater::Finish()
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
		} else if ( DeviceType == 0x01 || DeviceType == 0x02 || DeviceType == 0x03 )  {	// 1: VegaPuls(4~20mA), 2:VegaPuls(485Converter), 3: VegaPuls(Rs-485)
	
			STRnsWater *pRnsWater = new STRnsWater();
			memcpy(pRnsWater, pBuffer+3, PayloadLength);	// Payload Copy
			
			m_pList->Add(pRnsWater);	// Add list 
			
			LogMessage("RnsWater waterlevel added.." + FloatToStrF(pRnsWater->Depth + theEnv.m_WaterOffset, ffFixed, 5, 2 ));
		} else {
			// Unknown device type
			LogMessage("RnsWater unknown device type..");
		}
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
	for ( int i = 0 ; i < iLen ; i++ ) {
		if ( Buffering ) {	// Buffering 
			Buffer[Pos++] = *(pBuffer+i);
			if ( Pos < 3 ) {
				// nothing to do.. => continue buffering
			} else if  ( Pos == 3 ) {
				Length = *(pBuffer+2);
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
		LogMessage("[RnsWater] data request..");
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
	
	Value2DB();

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
	m_RWaterLevel.AvgWaterLevel  = AvgWaterLevel;	// Depth + Offset
	m_RWaterLevel.MinWaterLevel  = MinWaterLevel;
	m_RWaterLevel.MaxWaterLevel  = MaxWaterLevel;
	m_RWaterLevel.Value01        = RnsWater.Temperature;
	m_RWaterLevel.Value02        = RnsWater.Pressure;
	m_RWaterLevel.Value03        = RnsWater.Depth;
	m_RWaterLevel.Value04        = RnsWater.Depth + theEnv.m_WaterOffset;	// Depth + Offset
	m_RWaterLevel.Value05        = 0.0f;
	m_RWaterLevel.Value06        = 0.0f;
	m_RWaterLevel.Value07        = RnsWater.Salinity;
	m_RWaterLevel.Value08        = 0.0f;
	m_RWaterLevel.Value09        = 0.0f;
	m_RWaterLevel.Value10        = theEnv.m_WaterOffset;	// Offset
	m_RWaterLevel.Value11        = RnsWater.Type;	// WaterLevel Sensor Device Type => 0,1,2,3,..
	m_RWaterLevel.Value12        = 0.0f;
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
	
	return bResult;
}

//---------------------------------------------------------------------------
// GetStatus
bool TRnsWater::CaculateWaterLevel()
{
	BYTE Type = 0x00;
	double AvgWaterLevel = 0.0f;
	double MinWaterLevel = 0.0f;
	double MaxWaterLevel = 0.0f;
	double Temperature   = 0.0f;
	double Pressure      = 0.0f;
	double Depth         = 0.0f;

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
			iValidCount++;

			if ( MinWaterLevel < pData->Depth ) MinWaterLevel = pData->Depth;
			if ( MaxWaterLevel > pData->Depth ) MaxWaterLevel = pData->Depth;
		} // end of for 

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
	this->RnsWater.Type        = Type;
	this->RnsWater.Temperature = Temperature;
	this->RnsWater.Pressure    = Pressure;
	this->RnsWater.Depth       = Depth;
	this->Offset               = theEnv.m_WaterOffset;

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

#pragma package(smart_init)
