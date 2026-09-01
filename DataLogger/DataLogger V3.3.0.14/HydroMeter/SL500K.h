//---------------------------------------------------------------------------

#ifndef SL500KH
#define SL500KH
//---------------------------------------------------------------------------

#include "HydroMeter.h"

struct STSLVelocity {
	int   CellNo;
	SHORT VelocityX; 	//Velocity 1/X
	SHORT VelocityY;  	//Velocity 2/Y
	SHORT StdErrVelX;  	//Standard error of velocity 1/X
	SHORT StdErrVelY;   //Standard error of velocity 2/Y
	SHORT SignalBeam1;  //Signal strength (Beam 1)
	SHORT SignalBeam2;  //Signal strength (Beam 2
};


class TSL500K : public THydroMeter
{
public:		// Attribute

	int    CellBegin;  		// Cell Begin
	int    CellEnd;			// Cell End
	int    CellNumber;		// Cell Number ( 0 ~ 200 )
	int    CellNo;		    // Cell Size
	int    CellDistance;	// Cell Blank Distance

	double AvgVel;			// AvgVelocity
	double MinVel;			// MinVelocity
	double MaxVel;			// MaxVelocity
	double NumberOfBeam;
	double Orientation;
	double TemperatureMode;
	double CoordinateSystem;
	double NumberOfCells;
	double CellSize;
	double BlankingDistance;
	double AverageInterval;
	double NumberOfPings;
	double MeanHeading;
	double MeanPitch;
	double MeanRoll;
	double MeanTemperature;
	double MeanPressure;
	double DeviationHeading;
	double DeviationPitch;
	double DeviationRoll;
	double StdTemperature;
	double StdPressure;
	double SoundSpeed;

	//TList *m_pVelocityList;

public:		// Method
	TSL500K();
	~TSL500K();

//Virtual Method
	void   Clear();
	void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo=enHydroFirst);
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

int __fastcall CompareVelocitySL(void *Item1, void *Item2);

#endif
