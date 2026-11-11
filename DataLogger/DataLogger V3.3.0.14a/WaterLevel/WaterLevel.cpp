//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "WaterLevel.h"
//---------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Swap Function
///////////////////////////////////////////////////////////////////////////////
double SwapDouble(double Value)
{
    byte x[8];
    byte c;
    memcpy(x, &Value, 8);

    c = x[7];   x[7] = x[0];   x[0] = c;
    c = x[6];   x[6] = x[1];   x[1] = c;
    c = x[5];   x[5] = x[2];   x[2] = c;
    c = x[4];   x[4] = x[3];   x[3] = c;

    memcpy(&Value, x, 8);
    return Value;
}
//---------------------------------------------------------------------------
float SwapFloat(float Value)
{
    byte x[4];
    byte c;
    memcpy(x, &Value, 4);

    c = x[3];   x[3] = x[0];   x[0] = c;
    c = x[2];   x[2] = x[1];   x[1] = c;

    memcpy(&Value, x, 4);
    return Value;
}
//---------------------------------------------------------------------------
long SwapLong(long   Value)
{
    byte x[4];
    byte c;
    memcpy(x, &Value, 4);

    c = x[3];   x[3] = x[0];   x[0] = c;
    c = x[2];   x[2] = x[1];   x[1] = c;

    memcpy(&Value, x, 4);
    return Value;
}
//---------------------------------------------------------------------------
unsigned long SwapULong(unsigned long Value)
{
    byte x[4];
    byte c;
    memcpy(x, &Value, 4);

    c = x[3];   x[3] = x[0];   x[0] = c;
    c = x[2];   x[2] = x[1];   x[1] = c;

    memcpy(&Value, x, 4);
    return Value;
}
//---------------------------------------------------------------------------
int SwapInt(int Value)
{
    byte x[2];
    byte c;
    memcpy(x, &Value, 2);

    c = x[1];   x[1] = x[0];   x[0] = c;

    memcpy(&Value, x, 2);
    return Value;
}
//---------------------------------------------------------------------------
unsigned int SwapUInt(unsigned int Value)
{
    byte x[2];
    byte c;
    memcpy(x, &Value, 2);

    c = x[1];   x[1] = x[0];   x[0] = c;

    memcpy(&Value, x, 2);
    return Value;
}
//---------------------------------------------------------------------------
short SwapShort(short Value)
{
    byte x[2];
    byte c;
    memcpy(x, &Value, 2);

    c = x[1];   x[1] = x[0];   x[0] = c;

    memcpy(&Value, x, 2);
    return Value;
}
//---------------------------------------------------------------------------
unsigned short SwapUShort(unsigned short Value)
{
    byte x[2];
    byte c;
    memcpy(x, &Value, 2);

    c = x[1];   x[1] = x[0];   x[0] = c;

    memcpy(&Value, x, 2);
    return Value;
}
//---------------------------------------------------------------------------
WORD SwapWord(WORD Value)   // 2 byte
{
    byte x[2];
    byte c;
    memcpy(x, &Value, 2);

    c = x[1];   x[1] = x[0];   x[0] = c;

    memcpy(&Value, x, 2);
    return Value;
}


//---------------------------------------------------------------------------
// Constructor
TWaterLevel::TWaterLevel()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep =  enSensorIdle;	// Sensor Working Status

	MeasureDate = "";
	MeasureTime = "";
	WaterKind   = "";
	Offset      = 0;
	WaterLevel  = 0;
	Area        = 0;

	bLeak = false;	// Leak or not

	m_TimeStamp = Now();
//	m_bIdleCheck = true;			// idle reboot on
//	m_iIdleCheckTime = 3;			// 180 minutes(3hours)
}

//---------------------------------------------------------------------------
// Destructor
TWaterLevel::~TWaterLevel()
{
	// nothing to do..
}

//---------------------------------------------------------------------------
// 유량 산출을 위한 기초 데이터를 지정한다.
void TWaterLevel::InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM)
{
	m_pRFlowIVM  = pRFlowIVM;
	m_pRFlowHA   = pRFlowHA;
	m_pRFlowADVM = pRFlowADVM;
}

//---------------------------------------------------------------------------
// GetStatus
EN_SENSOR_STEP TWaterLevel::GetStatus()
{
	return m_enStep;
}

//---------------------------------------------------------------------------
// LogMessage
void TWaterLevel::LogMessage(UnicodeString sMsg, bool bFile)
{
	FormMain->LogMessage(sMsg, bFile);
}


#pragma package(smart_init)
