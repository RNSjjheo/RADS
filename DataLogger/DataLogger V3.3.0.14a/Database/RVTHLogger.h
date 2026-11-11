//---------------------------------------------------------------------------

#ifndef RVTHLoggerH
#define RVTHLoggerH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRVTHLogger : public TRBase
{
public:
	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
	double        Volt;
	double        Temperature;
	double        Humidity;
	double        DCCharge;
	double        DCBattery;
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
	UnicodeString RawData;
public:
	TRVTHLogger();
	~TRVTHLogger();
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
