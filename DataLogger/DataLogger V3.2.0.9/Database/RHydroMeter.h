//---------------------------------------------------------------------------

#ifndef RHydroMeterH
#define RHydroMeterH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRHydroMeter : public TRBase
{
public:
	bool          First;
	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
	UnicodeString HydroKind;
	double        AvgVelocity;
	double        MinVelocity;
	double        MaxVelocity;
	double        Value01;
	double        Value02;
	double        Value03;
	double        Value04;
	double        Value05;
	double        Value06;
	double        Value07;
	double        Value08;
	double        Value09;
	double        Value10;
	double        Value11;
	double        Value12;
	double        Value13;
	double        Value14;
	double        Value15;
	double        Value16;
	double        Value17;
	double        Value18;
	double        Value19;
	double        Value20;
	double        Value21;
	double        Value22;
	double        Value23;
	double        Value24;
	double        Value25;
	double        Value26;
	double        Value27;
	double        Value28;
	double        Value29;
	double        Value30;
	double        Value31;
	double        Value32;
	double        Value33;
	double        Value34;
	double        Value35;
	double        Value36;
	double        Value37;
	double        Value38;
	double        Value39;
	double        Value40;
	UnicodeString RawData;

public:
	TRHydroMeter(bool bFirst=true);
	~TRHydroMeter();
	void Clear();
	bool Insert();
	bool Update();
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();

	UnicodeString GetTable();
};


#endif
