//---------------------------------------------------------------------------

#ifndef RFlowADVMH
#define RFlowADVMH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRFlowADVM : public TRBase
{
public:
	UnicodeString PointCode;
	UnicodeString UseDate;

	double AXadvm;
	double AZadvm;
	double APan;
	double ATilt;
	double APhi;
	double ASubmersion;
	int    AReverseMode;
	int    AUnitVelocity;
	double AVMax;
	double AVMin;
	double AStartCell;
	double AEndCell;
	double BXadvm;
	double BZadvm;
	double BPan;
	double BTilt;
	double BPhi;
	double BSubmersion;
	int    BReverseMode;
	int    BUnitVelocity;
	double BVMax;
	double BVMin;
	double BStartCell;
	double BEndCell;
	UnicodeString BType;

public:
	TRFlowADVM();
	~TRFlowADVM();
	void Clear();
	bool Insert();
	bool Update();
	bool UpdatePointCode(UnicodeString sPointCode);
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
	bool GetData(UnicodeString sPointCode);
};



#endif
