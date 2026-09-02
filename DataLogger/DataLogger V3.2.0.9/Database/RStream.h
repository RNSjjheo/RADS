//---------------------------------------------------------------------------

#ifndef RStreamH
#define RStreamH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRStream : public TRBase
{
public:
	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
	double        AvgStream;
	double        MinStream;	// not use
	double        MaxStream;	// not use
	double        WaterLevel;	// WaterLevel of measure point
	double        Area;			// Area of measure point
	double        Velocity;		// Average Velocity
public:
	TRStream();
	~TRStream();
	void Clear();
	bool Insert();
	bool Update();
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
};


#endif
