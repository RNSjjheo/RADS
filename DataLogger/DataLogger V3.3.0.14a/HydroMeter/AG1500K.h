//---------------------------------------------------------------------------

#ifndef AG1500KH
#define AG1500KH
//---------------------------------------------------------------------------

#include "HydroMeter.h"

struct STAGVelocity {
	int   CellNo;		//CellNo
	SHORT VelocityX; 	//Velocity 1/X
	SHORT VelocityY;  	//Velocity 2/Y
	SHORT StdErrVelX;  	//Standard error of velocity 1/X
	SHORT StdErrVelY;   //Standard error of velocity 2/Y
	SHORT SignalBeam1;  //Signal strength (Beam 1)
	SHORT SignalBeam2;  //Signal strength (Beam 2
};


class TAG1500K : public THydroMeter
{
public:		// Attribute

	int    CellBegin;  		// Cell Begin
	int    CellEnd;			// Cell End
	int    CellNumber;		// Cell Number ( 0 ~ 200 )
	int    CellNo;		    // Cell Size
	int    CellDistance;		// Cell Blank Distance

	double AvgVel;			// AvgVelocity
	double MinVel;			// MinVelocity
	double MaxVel;			// MaxVelocity
	double V1;				// Velocity component 1
	double V2;				// Velocity component 2
	double V3;				// Velocity component 3
	double ErrorOfVelocity1;// Standard error of velocity 1
	double ErrorOfVelocity2;// Standard error of velocity 2
	double ErrorOfVelocity3;// Standard error of velocity 3
	double SignalStrength1;	// Signal Strength 1
	double SignalStrength2;	// Signal Strength 2
	double SignalStrength3;	// Signal Strength 3
	double PercentGoodPings;// PercentGoodPings
	double Heading;			// Heading
	double Pitch;			// Pitch
	double Roll;			// Roll
	double DeviationHeading;// Standard deviation heading
	double DeviationPitch;	// Standard deviation pitch
	double DeviationRoll;	// Standard deviation roll
	double MeanTemperature;	// Mean Temperature
	double MeanPressure;	// Mean Pressure
	double DeviationPressure;// Standard deviation of pressure
	double InputPowerLevel;	// Input power Level
	double StartLocation;	// Starting location of sampling volume
	double EndLocation;		// Ending location of sampling volume
	double NoiseLevel1;		// Noise Level(Beam 1)
	double NoiseLevel2;		// Noise Level(Beam 2)
	double NoiseLevel3;		// Noise Level(Beam 3)

	//TList *m_pVelocityList;

public:		// Method
	TAG1500K();
	~TAG1500K();

//Virtual Method
	void   Clear();
	void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo = enHydroFirst);
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

//Public Method
	bool CalAvgMinMaxVelocity();
};

int __fastcall CompareVelocityAG(void *Item1, void *Item2);


#endif
