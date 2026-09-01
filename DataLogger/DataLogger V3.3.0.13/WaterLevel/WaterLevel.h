//---------------------------------------------------------------------------

#ifndef WaterLevelH
#define WaterLevelH
//---------------------------------------------------------------------------

#include "../Define.h"
#include "../UnitMain.h"
#include "../Database/RWaterLevel.h"
#include "../Database/RFlowIVM.h"
#include "../Database/RFlowHA.h"
#include "../Database/RFlowADVM.h"

#include "../common.h"

#include <System.Math.hpp>

#include "AdPort.hpp"
#include "OoMisc.hpp"

#define TROLL_INIT        0x80
#define TROLL_TEMPERATURE 0x40  	// use Ack == TROLL_TEMPERATURE | TROL_PRESSURE | TROL_DEPTH | TROLL_SALINITY
#define TROLL_PRESSURE    0x20
#define TROLL_DEPTH       0x10
#define TROLL_SALINITY    0x08

#define HEX2BIN(x) ((x=='0') ?  0 :  \
		   ((x=='1') ?  1 :  \
		   ((x=='2') ?  2 :  \
		   ((x=='3') ?  3 :  \
		   ((x=='4') ?  4 :  \
		   ((x=='5') ?  5 :  \
		   ((x=='6') ?  6 :  \
		   ((x=='7') ?  7 :  \
		   ((x=='8') ?  8 :  \
		   ((x=='9') ?  9 :  \
		   ((x=='A') ? 10 :  \
		   ((x=='B') ? 11 :  \
		   ((x=='C') ? 12 :  \
		   ((x=='D') ? 13 :  \
		   ((x=='E') ? 14 : 15 )))))))))))))))

#define BIN2HEX(x) ((x== 0) ? '0' : \
		   ((x== 1) ? '1' : \
		   ((x== 2) ? '2' : \
		   ((x== 3) ? '3' : \
		   ((x== 4) ? '4' : \
		   ((x== 5) ? '5' : \
		   ((x== 6) ? '6' : \
		   ((x== 7) ? '7' : \
		   ((x== 8) ? '8' : \
		   ((x== 9) ? '9' : \
		   ((x==10) ? 'A' : \
		   ((x==11) ? 'B' : \
		   ((x==12) ? 'C' : \
		   ((x==13) ? 'D' : \
		   ((x==14) ? 'E' : 'F')))))))))))))))

#pragma pack(1)

// Aqual TROLL 200, Level TROLL 400/500 modubus message header
struct STTROLLHeader {
	byte   Address;           	// Modubus Address
	byte   FunctionCode;		// Function Code
	USHORT DataAddress;			// Data Address
	USHORT RegisterCount;       // Register Count
};

// Level TROLL 400/500 Payload Format
struct STLevelTROLL {
	float  MeasuredValue;
	USHORT ParameterId;
	USHORT UnitsId;
	USHORT DataQualityId;
	float  SentinelValue;
	WORD   AvailableUnits;
};

// Aqua TROLL 200 Payload Foramt
struct STAquaTROLL {
	float  MeasuredValue;
	USHORT ParameterId;
	USHORT UnitsId;
	USHORT DataQualityId;
	float  SentinelValue;
	WORD   AvailableUnits;
};

#pragma pack()


class TWaterLevel
{
public:		// Attribute
	char         m_Buffer[BUFFER_8K];
	bool         m_bBuffering;			// Buffering or not
	AnsiString   m_RecvMsg;

	TApdComPort    *m_pPort;			 // ComPort
	EN_SENSOR_STEP m_enStep;			// Sensor Status ( idle-start-working-finish )
	EN_SENSOR_STEP m_enWorkingStep;		// 2014.09.17 Sensor Working Status (idle-start-finish-start-finish-...)

	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
	UnicodeString WaterKind;			// LT500, AT200, OTTCBS
	double        Offset;				// Offset
	double        WaterLevel;			// 측정한 수심( AverageWaterlevel + Offset )
	double        Area;					// 단면적
	bool          bLeak;				// Leak or not (센서가 등답이 없는 지 여부)

	TDateTime     m_TimeStamp;			// 마지막으로 동작한 시간의 TimeStamp

	TRWaterLevel  m_RWaterLevel;

//	bool          m_bIdleCheck;			// idle pc reboot option =>
//	int           m_iIdleCheckTime;		// default : 180 minutes

	// FlowStream
	TRFlowIVM     *m_pRFlowIVM;
	TRFlowHA      *m_pRFlowHA;
	TRFlowADVM    *m_pRFlowADVM;

public:		// Method
	TWaterLevel();
	virtual ~TWaterLevel();
	virtual void Clear() = 0;
	virtual void Init(TApdComPort *pPort) = 0;
	virtual bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false) = 0;
	virtual bool Finish() = 0;
	virtual bool Decode(AnsiString RecvMsg, int iLen) = 0;
	virtual bool Decode(BYTE *pBuffer, int iLen) = 0;
	virtual bool Receive(AnsiString RecvMsg, int iLen) = 0;
	virtual bool Receive(BYTE *pBuffer, int iLen) = 0;
	virtual bool Send(AnsiString RecvMsg, int iLen) = 0;
	virtual bool Save(UnicodeString MeasureDate, UnicodeString MeasureTime) = 0;
	virtual void DB2Value() = 0;
	virtual void Value2DB() = 0;
	virtual double GetArea() = 0;
	virtual double GetWaterLevel() = 0;

	void InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM);
	EN_SENSOR_STEP GetStatus();
	void LogMessage(UnicodeString sMsg, bool bFile=true);
};

double         SwapDouble(double Value);
float          SwapFloat(float Value);
long           SwapLong(long   Value);
unsigned long  SwapLong(unsigned long Value);
int            SwapInt(int   Value);
unsigned int   SwapUInt(unsigned int Value);
short          SwapShort(short Value);
unsigned short SwapUShort(unsigned short Value);
WORD           SwapWord(WORD Value);

#endif
