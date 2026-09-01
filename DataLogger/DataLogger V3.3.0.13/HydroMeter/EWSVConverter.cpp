//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "EWSVConverter.h"
#include "AniThread.h"
//#include <System.RegularExpressions.hpp>
//#include <System.Classes.hpp>

int __fastcall CompareVelocityx(void *Item1, void *Item2)
{
	STEWSVPayload *A, *B;
	A = (STEWSVPayload *)Item1;
	B = (STEWSVPayload *)Item2;
	if ( A->Data[0].V < B->Data[0].V )       return -1;	// True  => Skip
	else if ( A->Data[0].V == B->Data[0].V ) return  0;
	else                                     return  1;	// false => Swap
}

//---------------------------------------------------------------------------
// Constructor
TEWSVConverter::TEWSVConverter()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status

	MeasureDate = "";
	MeasureTime = "";

	memset(Buffer, 0x00, sizeof(Buffer));
	Pos = 0;
	Length = 0;
	Buffering = false;

	memset(&Payload, 0x00, sizeof(STEWSVPayload));
	memset(&Data, 0x00, sizeof(STEWSVData));
	TQ  = 0.0f;
	TLQ = 0.0f;

	HydroKind   = EWSVCONVERTER_STR;

	m_pList = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TEWSVConverter::~TEWSVConverter()
{
	// Resource release
	STEWSVPayload *pPayload = NULL;
	for ( int i = 0 ; i < m_pList->Count ; i++ ) {
		try {
			pPayload = (STEWSVPayload *) m_pList->Items[i];
			delete pPayload;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pList->Clear();

	delete m_pList;
}

//---------------------------------------------------------------------------
// Clear
void TEWSVConverter::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep = enSensorIdle;
	//m_bBreak     = false;			// <BREAK> false status

	MeasureDate = "";
	MeasureTime = "";

	memset(Buffer, 0x00, sizeof(Buffer));
	Pos = 0;
	Length = 0;
	Buffering = false;

	memset(&Payload, 0x00, sizeof(STEWSVPayload));
	memset(&Data, 0x00, sizeof(STEWSVData));
	TQ  = 0.0f;
	TLQ = 0.0f;

	HydroKind   = EWSVCONVERTER_STR;

	m_RHydroMeter.Clear();
	m_RHydroCellContainer.Clear();

	STEWSVPayload *pPayload = NULL;
	for ( int i = 0 ; i < m_pList->Count ; i++ ) {
		try {
			pPayload = (STEWSVPayload *) m_pList->Items[i];
			delete pPayload;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pList->Clear();
}

//---------------------------------------------------------------------------
// Init
void TEWSVConverter::Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo)
{
	m_pPort = pPort;
	m_enHydroNo = enHydroNo;
	m_RHydroMeter.enHydroNo = enHydroNo;	// HydroMeter1 or not
	m_RHydroCellContainer.m_enHydroNo = enHydroNo;	// HydroMeter1Cell or not

	m_pPort = pPort;
}

//---------------------------------------------------------------------------
void TEWSVConverter::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
//	this->CellBegin    = CellBegin;
//	this->CellEnd      = CellEnd;
//	this->CellNumber   = CellNumber;
//	this->CellNo       = CellNo;
//	this->CellDistance = CellDistance;
}

//---------------------------------------------------------------------------
// Start
bool TEWSVConverter::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	Clear();	// Status Clear

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	bool bResult = false;
	bResult = MeasureStart(theEnv.m_EWSVWorkTime, theEnv.m_EWSVSensorAvg);

	LogMessage((bResult) ? "EWSVConverter measure start ok.." :  "EWSVConverter measure start fail..");

	m_TimeStamp = Now();
	m_enStep = enSensorStart;
	m_enWorkingStep = enSensorStart;

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TEWSVConverter::Finish()
{
	bool bResult = false;
	bResult = Save(MeasureDate, MeasureTime);
	if ( !bResult ) return bResult;

	m_enStep = enSensorFinish;
	m_enWorkingStep = enSensorIdle;	// 2014.09.17

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TEWSVConverter::Decode(AnsiString sCmd, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TEWSVConverter::Decode(BYTE *pBuffer, int iLen)
{
	////////////////////////////////////////////////////////////////////////
	// Decode & Process
	////////////////////////////////////////////////////////////////////////

	int iResult = FrameCheck(pBuffer, iLen);
	if ( iResult == FRAME_STX_ERROR ) {
		LogMessage("EWSV Frame STX Error..");
		return false;
	} else if ( iResult == FRAME_ETX_ERROR ) {
		LogMessage("EWSV Frame ETX Error..");
		return false;
	} else if ( iResult == FRAME_CRC_ERROR ) {
		LogMessage("EWSV Frame CRC Error..");
		return false;
	} else if ( iResult == FRAME_OK ) {
		// nothing to do
	} else {
		// nothing to do
	}

	BYTE Type = *(pBuffer + 1);	// Message Type
	BYTE Ack = 0x00;

	if ( Type == EWSV_MeasureStartAck ) {
		m_enStep = enSensorWorking; 	// Sensor working
		m_enWorkingStep = enSensorStart;	// 2014.09.17

		Ack = *(pBuffer+6);	// MeasureStartAck => 0x01 fixed..
// debug
LogMessage("EWSV MeasureStart acked..");

		FormMain->SetHydroMeter1Icon(enHydroMeterAni1);

		if ( FormMain->m_pAniThread != NULL ) {
			FormMain->m_pAniThread->m_bHydroMeter1 = true;
			FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterAni1;
		} else {}

		FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

	} else if ( Type == EWSV_MeasureDataRes ) {

		m_enStep = enSensorWorking; 	// Sensor working
		m_enWorkingStep = enSensorFinish;	// 2014.09.17

		////////////////////////////////////////////////////////////////////////
		// Data Process
		////////////////////////////////////////////////////////////////////////

		STEWSVPayload *pPayload = new STEWSVPayload();
		memcpy(pPayload, pBuffer+3, Length);

		m_pList->Add(pPayload);	// Buffering

		LogMessage("EWSV Data receive ok..");

		FormMain->SetHydroMeter1Icon(enHydroMeterNormal);

		if ( FormMain->m_pAniThread != NULL ) {
			FormMain->m_pAniThread->m_bHydroMeter1 = false;
			FormMain->m_pAniThread->m_HydroMeter1Index = enHydroMeterNormal;

			FormMain->SendLoggerStatusToViewer(FormMain->m_enStep);

		} else {
			// Frame invalid => nothing to do..
		}
		m_enWorkingStep = enSensorIdle;	// 2014.09.17
	} else {
		// undefine message type => nothing to do..
		LogMessage("EWSV undefine message type..");
	}

	m_TimeStamp = Now();	// TimeStamp refresh

	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TEWSVConverter::Receive(AnsiString RecvMsg, int iLen)
{
	return true;
}


//---------------------------------------------------------------------------
// Receive
bool TEWSVConverter::Receive(BYTE *pBuffer, int iLen)
{
	for ( int i = 0 ; i < iLen ; i++ ) {
		if (Buffering ) {
			Buffer[Pos++] = *(pBuffer+i);
			if ( Pos < 3 ) {
				continue;	// Buffering
			} else if ( Pos == 3 ) {
				Length = Buffer[2];	// Frame Length
			} else {
				if ( Pos >= Length + 5 ) {
					//------------------------------------------------------
					// Main Process
					//------------------------------------------------------
					Decode(Buffer, Pos);

					memset(Buffer, 0x00, sizeof(Buffer));
					Pos = 0;
					Length = 0;
					Buffering = false;
				} else {
					continue;	// Buffering
				}
			}
		} else {
			if ( *(pBuffer+i) == STX ) {	// Frame Start
				memset(Buffer, 0x00, sizeof(Buffer));
				Pos = 0;
				Length = 0;
				Buffering = false;

				Buffer[Pos++] = *(pBuffer+i);
				Buffering = true;
			} else {
				// discard
			}
		}
		// chec overflow
		if ( Pos >= sizeof(Buffer)) {
			memset(Buffer, 0x00, sizeof(Buffer));
			Pos = 0;
			Length = 0;
			Buffering = false;
		}
	}
	return true;
}


//---------------------------------------------------------------------------
// Send
bool TEWSVConverter::Send(AnsiString RecvMsg, int iLen)
{
//	if ( m_pPort == NULL ) return false;
//
//	try {
//		m_pPort->PutString(RecvMsg);
//		LogMessage("[EWSVConverter]=>" + RecvMsg);
//	} catch ( Exception &e ) {
//		LogMessage(e.Message);
//		return false;
//	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TEWSVConverter::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	CaculateVelocity();
	Value2DB();	// Value Mapping

	m_RHydroMeter.MeasureDate = MeasureDate;
	m_RHydroMeter.MeasureTime = MeasureTime;
	m_RHydroMeter.HydroKind   = HydroKind;

	bool bResult = false;
	bResult = m_RHydroMeter.Insert();

	if ( !bResult ) return bResult;

	TRHydroMeterCellContainer HydroCellContainer;
	TRHydroMeterCell *pHydroMeterCell = NULL;

	for ( int i = 1 ; i < Payload.Count ; i++ ) {  	// Detail Table : index start 1 => PAVOS,..

		pHydroMeterCell = new TRHydroMeterCell(m_enHydroNo);

		pHydroMeterCell->MeasureDate = MeasureDate;
		pHydroMeterCell->MeasureTime = MeasureTime;
		pHydroMeterCell->CellNo      = i+1;	// Pavos => 2,3,4
		pHydroMeterCell->Value01     = Payload.Data[i].W;
		pHydroMeterCell->Value02     = Payload.Data[i].V;
		pHydroMeterCell->Value03     = Payload.Data[i].SNR;
		pHydroMeterCell->Value04     = Payload.Data[i].Q;
		pHydroMeterCell->Value05     = TQ;
		pHydroMeterCell->Value06     = Payload.Data[i].LV;
		pHydroMeterCell->Value07     = Payload.Data[i].LQ;
		pHydroMeterCell->Value08     = Payload.Data[i].Opposite;
		pHydroMeterCell->Value09     = Payload.Data[i].Volt;

		pHydroMeterCell->Value19     = TLQ;

		pHydroMeterCell->Value27     = Payload.Data[i].Angle;
		pHydroMeterCell->Value28     = Payload.Data[i].DeviceType;

		HydroCellContainer.Add(pHydroMeterCell);
	}

	bResult = HydroCellContainer.Save(MeasureDate, MeasureTime);

	return bResult; // Container destory될 대 Resource release 됨..
}

//---------------------------------------------------------------------------
// DB2Value
void TEWSVConverter::DB2Value()
{
	// nothing to do..
}

//---------------------------------------------------------------------------
// Value2DB
void TEWSVConverter::Value2DB()
{
	m_RHydroMeter.AvgVelocity   =  AvgVel;  // Average Velocity
	m_RHydroMeter.MinVelocity   =  MinVel;  // Min Velocity
	m_RHydroMeter.MaxVelocity   =  MaxVel;  // Max Velocity
	m_RHydroMeter.Value01	    =  Payload.Data[0].W;       // W
	m_RHydroMeter.Value02	    =  Payload.Data[0].V;       // V
	m_RHydroMeter.Value03	    =  Payload.Data[0].SNR;     // SNR
	m_RHydroMeter.Value04	    =  Payload.Data[0].Q;       // Q
	m_RHydroMeter.Value05	    =  TQ;                      // TQ => (RADS에서 계산 )
	m_RHydroMeter.Value06	    =  Payload.Data[0].LV;      // LV
	m_RHydroMeter.Value07	    =  Payload.Data[0].LQ;      // LQ
	m_RHydroMeter.Value08	    =  Payload.Data[0].Opposite;// Opposite Content
	m_RHydroMeter.Value09	    =  Payload.Data[0].Volt;    // Volt

	m_RHydroMeter.Value19	    =  TLQ;                     // Total LQ ( RADS 에서 계산 )

	m_RHydroMeter.Value27	    =  Payload.Data[0].Angle;   // Inclination
	m_RHydroMeter.Value28	    =  Payload.Data[0].DeviceType;// 0:None, 1:RG-30, 2:PAVOS
}

//---------------------------------------------------------------------------
// 평균유속을 구한다.
double TEWSVConverter::GetAvgVelocity()
{
	double Velocity = 0.0f;


	return Velocity;
}

//---------------------------------------------------------------------------
void TEWSVConverter::SetBreak(bool bBreak)
{
//	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
bool TEWSVConverter::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
void TEWSVConverter::SendBreak(int iMiliSecond)	// Send <BREAK> to ComPort
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
//
// Method 
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// CaculateVelocity
bool TEWSVConverter::CaculateVelocity()
{
	double Avg, Min, Max;
	Avg = Min = Max = 0.0f;

	STEWSVPayload xPayload;
	STEWSVData    xData;
	memset(&xPayload, 0x00, sizeof(STEWSVPayload));
	memset(&xData, 0x00, sizeof(STEWSVData));

	STEWSVPayload *pPayload = NULL;
	STEWSVData *pData = NULL;

	TQ = TLQ = 0.0f;	// TQ, TLQ Clear

	if ( theEnv.m_EWSVAvg == "AVG" ) {	// RADS에서 평균값을 산출하는 방식 => 평균, MID: Sort한 후 중간값
		if ( m_pList != NULL ) {
			int ItemCount = 0;
			for ( int i = 0 ; i < m_pList->Count ; i++ ) {
				pPayload = (STEWSVPayload *) m_pList->Items[i];
				if (pPayload == NULL ) continue;
				ItemCount++;
				xPayload.Count = pPayload->Count;
				for ( int j = 0 ; j < pPayload->Count ; j++ ) {
					// RG-30
					xPayload.Data[j].DeviceType= pPayload->Data[j].DeviceType;
					xPayload.Data[j].W        += pPayload->Data[j].W;
					xPayload.Data[j].V        += pPayload->Data[j].V;
					xPayload.Data[j].SNR      += pPayload->Data[j].SNR;
					xPayload.Data[j].Q        += pPayload->Data[j].Q;
					xPayload.Data[j].LV       += pPayload->Data[j].LV;
					xPayload.Data[j].LQ       += pPayload->Data[j].LQ;
					xPayload.Data[j].Opposite += pPayload->Data[j].Opposite;
					xPayload.Data[j].Volt     += pPayload->Data[j].Volt;
					xPayload.Data[j].Angle    += pPayload->Data[j].Angle;
				}
			}
			if ( ItemCount > 0 ) {
				for ( int j = 0 ; j < xPayload.Count ; j++ ) {
					xPayload.Data[j].W        = xPayload.Data[j].W        / ItemCount;
					xPayload.Data[j].V        = xPayload.Data[j].V        / ItemCount;
					xPayload.Data[j].SNR      = xPayload.Data[j].SNR      / ItemCount;
					xPayload.Data[j].Q        = xPayload.Data[j].Q        / ItemCount;
					xPayload.Data[j].LV       = xPayload.Data[j].LV       / ItemCount;
					xPayload.Data[j].LQ       = xPayload.Data[j].LQ       / ItemCount;
					xPayload.Data[j].Opposite = xPayload.Data[j].Opposite / ItemCount;
					xPayload.Data[j].Volt     = xPayload.Data[j].Volt     / ItemCount;
					xPayload.Data[j].Angle    = xPayload.Data[j].Angle    / ItemCount;
				}
				Avg = Min = Max = xPayload.Data[0].V;	//
				// Caculate TQ, TLQ
				for ( int i = 0 ; i < xPayload.Count ; i++ ) {
					TQ  += xPayload.Data[i].Q;
					TLQ += xPayload.Data[i].LQ;
				}
			} else {
				memset(&xPayload, 0x00, sizeof(STEWSVPayload));
				Avg = Min = Max = xPayload.Data[0].V;	//
			}
		} else {
			// 수신된 데이터가 없다 => 모두 0으로 처리
			memset(&xPayload, 0x00, sizeof(STEWSVPayload));
			Avg = Min = Max = xPayload.Data[0].V;	//
		}
	} else {	// AVG=> "MID" => 소트하여 중간값을 취할 때
		if ( m_pList->Count == 0 ) {	// 리스트에 하나도 없을 때
			memset(&xPayload, 0x00, sizeof(STEWSVPayload));
		} else if ( m_pList->Count == 1 ) {	// 리스트에 하나 있을 때
			pPayload = (STEWSVPayload *) m_pList->Items[0]; // 중간값 Item Select
			memcpy(&xPayload, pPayload, sizeof(STEWSVPayload));
		} else {	// 리스트에 2개 이상 있을 때 => Sort한 후 중간값
			m_pList->Sort(CompareVelocityx);	// Sort
			int iMid = m_pList->Count / 2;
			pPayload = (STEWSVPayload *) m_pList->Items[iMid]; // 중간값 Item Select
			memcpy(&xPayload, pPayload, sizeof(STEWSVPayload));
		}
		Avg = Min = Max = xPayload.Data[0].V;	//
		// Caculate TQ, TLQ
		for ( int i = 0 ; i < xPayload.Count ; i++ ) {
			TQ  += xPayload.Data[i].Q;
			TLQ += xPayload.Data[i].LQ;
		}
	}

	Payload = xPayload;	// Assign
	AvgVel = Avg;
	MinVel = Min;
	MaxVel = Max;

	return true;
}

//---------------------------------------------------------------------------
// Send MeasureStart
bool TEWSVConverter::MeasureStart(int WorkTime, UnicodeString AvgMethod )
{
	bool bResult = false;
	m_enWorkingStep = enSensorStart;	//

	BYTE xBuffer[128] = "";
	int  xPos = 0;
	BYTE crc  = 0x00;

	xBuffer[xPos++] = STX;
	xBuffer[xPos++] = EWSV_MeasureStart; 	// MeasureStart
	xBuffer[xPos++] = 3;
	// Workint Time
	SHORT xWorkTime = WorkTime;
	memcpy(&(xBuffer[xPos]), &xWorkTime, 2);	// 2 byte
	xPos += 2;
	xBuffer[xPos++] = (AvgMethod == "AVG") ? 0x01 : 0x02;	// 1: AVG, 2 : Mid

	for ( int i = 0 ; i < xPos ; i++ ) {
		crc ^= xBuffer[i];
	}
	crc = crc & 0x7F;

	xBuffer[xPos++] = crc;
	xBuffer[xPos++] = ETX;

	if ( m_pPort == NULL ) return false;
	if ( m_pPort->Open == false ) return false;

	try {
		m_pPort->PutBlock(xBuffer, xPos);
		bResult = true;
		LogMessage("EWSV Measure start...");
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		bResult = false;
	}

	return bResult;
}

//---------------------------------------------------------------------------
// Send MeasureDataReq
bool TEWSVConverter::MeasureDataReq()
{
	bool bResult = false;
	m_enWorkingStep = enSensorFinish;	//


	BYTE xBuffer[128] = "";
	int  xPos = 0;
	BYTE crc  = 0x00;

	xBuffer[xPos++] = STX;
	xBuffer[xPos++] = EWSV_MeasureDataReq; 	// MeasureStart
	xBuffer[xPos++] = 0;					// Length
	// Workint Time

	for ( int i = 0 ; i < xPos ; i++ ) {
		crc ^= xBuffer[i];
	}
	crc = crc & 0x7F;

	xBuffer[xPos++] = crc;
	xBuffer[xPos++] = ETX;

	if ( m_pPort == NULL ) return false;
	if ( m_pPort->Open == false ) return false;

	try {
		m_pPort->PutBlock(xBuffer, xPos);
		bResult = true;
// debug
LogMessage("EWSV MeasureDataRes acked..");
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
// FrameCheck
bool TEWSVConverter::FrameCheck(BYTE *pBuffer, int iLen)
{
	if (*(pBuffer+0) != STX)       return FRAME_STX_ERROR;       // STX : 0xFA
	if (*(pBuffer+iLen-1) != ETX ) return FRAME_ETX_ERROR;	   // ETX : 0xF5

	byte crc = 0x00;
	for ( int i = 0 ; i < iLen-2 ; i++ ) {
		crc ^= *(pBuffer+i);
	}
	crc &= 0x7F;

	if ( crc != *(pBuffer+iLen-2)) return FRAME_CRC_ERROR;

	return FRAME_OK;
}

#pragma package(smart_init)
