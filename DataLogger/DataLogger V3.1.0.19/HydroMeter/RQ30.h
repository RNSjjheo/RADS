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
	double   AUX;		        // 0
	double   Waterlevel;   		// 1
	double   Velocity;          // 2
	double   Quality;		    // 3
	double   Discharge;		    // 4
	double   Area;     		    // 5
	double   LearnVelocity;		// 6
	double   LearnDischarge;	// 7
	double   OppositeContent;	// 8
	double   SupplyVoltage;		// 9
	int      PeakWidth;         // 10
	int      CSR;               // 11
	int      AreaPeak;          // 12
	int      RmsPic;            // 13 mV
	int      Amp;               // 14
	int      AmpRelation;       // 15
	int      Signal;            // 16
	int      ErrorCode;         // 17
	AnsiString RawData;
	byte     Complete;		// RQ30_MEASUREMENT | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2
};

//---------------------------------------------------------------------------
// TRQ30 Class
class TRQ30 : public THydroMeter
{
public:		// Attribute
	STRQ30Data    RQ30Data;			// Airmar Data(Receive)
	STAirmarData  AirmarData;

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
