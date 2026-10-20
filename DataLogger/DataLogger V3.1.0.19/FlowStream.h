//---------------------------------------------------------------------------

#ifndef FlowStreamH
#define FlowStreamH
//---------------------------------------------------------------------------

#include "./Database/RFlowIVM.h"
#include "./Database/RFlowHA.h"
#include "./Database/RFlowADVM.h"
#include "./Database/RStream.h"

#include "./HydroMeter/HydroMeter.h"
#include "./WaterLevel/WaterLevel.h"

struct STFlowVelocity {
	int 	HydroNo;	// 0:하층 유속계
	int 	CellNo;		// 0,1,2,...255
	double  Velocity;	// 유속
};

class TFlowStream
{
public:
	EN_SENSOR_STEP m_enStep;			// Sensor Status ( idle-start-working-finish )
	EN_SENSOR_STEP m_enWorkingStep;		// 2014.09.17 Sensor Working Status (idle-start-finish-start-finish-...)

	int            m_FlowType;	// 0: Not FlowStream 1: 유량사업단, 2:RNSEA itself
	UnicodeString  m_MeasureDate;
	UnicodeString  m_MeasureTime;

	TRFlowIVM     *m_pRFlowIVM;
	TRFlowHA      *m_pRFlowHA;
	TRFlowADVM    *m_pRFlowADVM;

	TWaterLevel  *m_pWaterLevel;
	THydroMeter  *m_pHydroMeter1;
	THydroMeter  *m_pHydroMeter2;

	TRStream      m_RStream;	// RStream Table

public:
	TFlowStream();
	TFlowStream(int iFlowtype);
	~TFlowStream();
	void Clear();
	void Init(int iFlowType);	//0: Not FlowStream 1: 유량사업단, 2:RNSEA itself
	void InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM);
	void InitWaterHydroMeter( TWaterLevel *pWaterLevel, THydroMeter *pHydro1, THydroMeter *pHydro2);

	void SetMeasureTime(UnicodeString sMeasureDate, UnicodeString sMeasureTime);
	bool CaculateFlowStream();
	bool CaculateFlowStreamByRNSEA();

	double GetVelocityBySourthNorth();

	void LogMessage(UnicodeString sMsg, bool bFile=true);
};

int __fastcall CompareFlowVelocity(void *Item1, void *Item2);

#endif
