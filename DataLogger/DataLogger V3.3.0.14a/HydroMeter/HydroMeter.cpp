//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "HydroMeter.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constructor
THydroMeter::THydroMeter()
{
	m_enHydroNo  = enHydroFirst;
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep =  enSensorIdle;	// Sensor Working Status

	m_bBreak      = false;
	m_bBreakResponse = false;
	m_bFlooding   = false; 			// 수신데이다 Flooding 상화
	m_iThrowCount = 0;              // 수신데이터 버려지는 횟수

	MeasureDate  = "";
	MeasureTime  = "";

	HydroKind    = "";	// HydorMeter Type : ChannelMaster, AG Series , SL Series

	m_TimeStamp  = Now();			// 마지막으로 동작한 시간의 TimeStamp
//	m_bIdleCheck = true;			// idle reboot on
//	m_iIdleCheckTime = 3;			// 180 minutes(3hours)

	m_pRFlowIVM  = NULL;
	m_pRFlowHA   = NULL;
	m_pRFlowADVM = NULL;

	m_pVelocityList = new TList();	// List for CellVelocity
}

//---------------------------------------------------------------------------
// Destructor
THydroMeter::~THydroMeter()
{
	if ( m_pVelocityList != NULL ) delete m_pVelocityList;
}

//---------------------------------------------------------------------------
// 유량 산출을 위한 기초 데이터를 지정한다.
void THydroMeter::InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM)
{
	m_pRFlowIVM  = pRFlowIVM;
	m_pRFlowHA   = pRFlowHA;
	m_pRFlowADVM = pRFlowADVM;
}

//---------------------------------------------------------------------------
// GetStatus
EN_SENSOR_STEP THydroMeter::GetStatus()
{
	return m_enStep;
}

//---------------------------------------------------------------------------
// LogMessage
void THydroMeter::LogMessage(UnicodeString sMsg, bool bFile)
{
	FormMain->LogMessage(sMsg, bFile);
}


#pragma package(smart_init)
