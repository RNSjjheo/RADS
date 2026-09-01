//---------------------------------------------------------------------------

#ifndef ChannelMasterH
#define ChannelMasterH
//---------------------------------------------------------------------------

#include "HydroMeter.h"

#define UART_BUFFER		8192

struct STVelocity {
	SHORT Velocity1;
	SHORT Velocity2;
};

struct STEcho {
	int Echo1;
	int Echo2;
};


class TChannelMaster : public THydroMeter
{
public:		// Attribute

	int    CellBegin;  		// Cell Begin
	int    CellEnd;			// Cell End
	int    CellNumber;		// Cell Number ( 0 ~ 200 )
	int    CellNo;		    // Cell Size
	int    CellDistance;	// Cell Blank Distance

	double AvgVel;		// Average Velocity
	double MinVel;		// Min Velocity
	double MaxVel;		// Max Velocity
	BYTE   NB; 			// Number of BEAMS
	BYTE   WN;			// Number of Cells
	SHORT  WP;			// Pings Per Ensemble
	SHORT  WS;			// Range Cell Length
	SHORT  WF;			// Blank after Transmit
	BYTE   WC;			// Low Corr Thresh
	SHORT  WE;			// Error Velocity Maximum
	BYTE   TP;			// TPP Hundredths
	BYTE   EX;			// Coordinate Transform
	BYTE   EZ;			// Sensor Source
	SHORT  BIN1;		// Bin1Distance
	BYTE   WA;			// False Target Thresh
	BYTE   WB;			// System Bandwidth
	SHORT  EC;			// Speed of Sound
	SHORT  ED; 			// Depth of Transducer
	SHORT  Pitch;		// Pitch
	SHORT  ROLL;		// Roll
	SHORT  ES;			// Salinity
	SHORT  ET;			// Temperature
	BYTE   MMinute;		// MPT Minutes
	BYTE   MSecond;		// MPT Second
	BYTE   MHundredths;	// MPT Hundredths
	BYTE   PITCHSTD;	// PITCH STD
	BYTE   ROLLSTD;		// ROLL STD
	BYTE   BatVolt;		// Battery Voltage in Volts
	INT    Pressure;	// Pressure
	AnsiString RawData;

	//TList *m_pVelocityList;
	TList *m_pEchoList;

public:		// Method
	TChannelMaster();
	~TChannelMaster();

//Virtual Function
	void   Clear();
	void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo);  	// bFirst=> 하층유속계, false: 상층유속계
	void   SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance);
	bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime);
	bool   Finish();
	bool   Decode(AnsiString sCmd, int iLen);
	bool   Decode(BYTE *pBuffer, int iLen);
	bool   Receive(AnsiString RecvMsg, int iLen);
	bool   Receive(BYTE *pBuffer, int iLen);
	bool   Send(AnsiString RecvMsg, int iLen);
	bool   Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void   DB2Value();
	void   Value2DB();
	double GetAvgVelocity();

	void   SetBreak(bool bBreak);
	bool   GetBreakStatus();
	void   SendBreak(int iMiliSecond);	// Send <BREAK> to ComPort

// Function
	bool DecodeBBensemble(AnsiString sFrame);
	bool CalAvgMinMaxVelocity();

};

int __fastcall CompareVelocity(void *Item1, void *Item2);

#endif
