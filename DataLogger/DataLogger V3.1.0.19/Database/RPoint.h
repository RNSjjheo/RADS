//---------------------------------------------------------------------------

#ifndef RPointH
#define RPointH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRPoint : public TRBase
{
public:
	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
	UnicodeString HydroMeter1Flag;
	UnicodeString HydroMeter2Flag;
	UnicodeString WaterLevelFlag;
	UnicodeString FileFlag;
	UnicodeString StreamFlag;
	UnicodeString ServerFlag;
	UnicodeString RnseaFlag;
public:
	TRPoint();
	~TRPoint();
	void Clear();
	bool Insert();
	bool Update();
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
	bool UpdateWaterLevel(EN_POINT_STATUS enPointStatus);
	bool UpdateHydroMeter1(EN_POINT_STATUS enPointStatus);
	bool UpdateHydroMeter2(EN_POINT_STATUS enPointStatus);
	bool UpdateFlowStream(EN_POINT_STATUS enPointStatus);
	bool UpdateStatus(UnicodeString sColumn, UnicodeString sStatus);
};

UnicodeString PointStatus2Str(EN_POINT_STATUS enPointStatus);

#endif
