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
	UnicodeString StreamFlag;
	UnicodeString FileFlag;
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
	bool GetData(int pk1, int pk2);
};

#endif
