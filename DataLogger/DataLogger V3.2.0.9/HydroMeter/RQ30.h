//---------------------------------------------------------------------------

#ifndef RQ30H
#define RQ30H
//---------------------------------------------------------------------------

//#include "../Define.h"
//#include "../UnitMain.h"
//
//#include "AdPort.hpp"
//#include "OoMisc.hpp"

#include "HydroMeter.h"
#include "Airmar.h"
#include "UnitEnv.h"

#define UART_BUFFER		   8192

#define RQ30_MAIN           0x80
#define RQ30_SPECIAL        0x40
#define RQ30_ANALYSIS1		0x20
#define RQ30_ANALYSIS2		0x10

//---------------------------------------------------------------------------
// RQ30 Data Structure
struct STRQ30Data {
	int      SelfCheck;		        // 1  AUX=>SelfCheck
	double   Waterlevel;   		// 2
	double   Velocity;          // 3
	double   Quality;		    // 4
	double   Discharge;		    // 5
	double   Area;     		    // 6
	double   LearnVelocity;		// 7
	double   LearnDischarge;	// 8
	double   OppositeContent;	// 9
	double   SupplyVoltage;		// 10
	int      PeakWidth;         // 11
	int      CSR;               // 12
	int      AreaPeak;          // 13
	int      RmsPic;            // 14 mV
	int      Amp;               // 15
	int      AmpRelation;       // 16
	int      Signal;            // 17
	int      ErrorCode;         // 18
	int      Angle;             // 20
	AnsiString RawData;
	byte     Complete;		// RQ30_MEASUREMENT | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2

	STRQ30Data::STRQ30Data() {
		memset(this, 0x00, sizeof(STRQ30Data));
	}
};


//---------------------------------------------------------------------------
// TRQ30 Class
class TRQ30 : public THydroMeter
{
public:		// Attribute
	STRQ30Data    RQ30Data;			// Airmar Data(Receive)
	STAirmarData  AirmarData;		// for Airmar Data using Airmar Sensor(Direct connection)
	STAirmarData  Airmar;			// for Airmar Data using AirmarLinker

	TList        *m_pRQ30List;			// buffer of airmar data

public:		// Method
	TRQ30();
	~TRQ30();

//Virtual Function
	void   Clear();
	void   Init(TApdComPort *pPort, bool bFirst=true);  	// bFirst=> 하층유속계, false: 상층유속계
	void   SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance);	// AG/SL Series는 반드시 초기 데이터로 지정해야 함.
	bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime);
	bool   Finish();
	bool   Decode(AnsiString RecvMessage, int iLen);
	bool   Decode(BYTE *pBuffer, int iLen);
	bool   Receive(AnsiString RecvMessage, int iLen);
	bool   Receive(BYTE *pBuffer, int iLen);
	bool   Send(AnsiString RecvMsg, int iLen);
	bool   Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void   DB2Value();
	void   Value2DB();
	double GetAvgVelocity();

	void   SetBreak(bool bBreak);
	bool   GetBreakStatus();
	void   SendBreak(int iMiliSecond);

// Method
	void   SetAirmarData(STAirmarData AirmarData);
	bool   CaculateRQ30Data();
	STRQ30Data GetRQ30Data();
};

int __fastcall CompareRQVelocity(void *Item1, void *Item2);

#endif
