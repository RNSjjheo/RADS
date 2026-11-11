//---------------------------------------------------------------------------

#ifndef RFlowIVMH
#define RFlowIVMH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRFlowIVM : public TRBase
{
public:
	UnicodeString PointCode;
	UnicodeString UseDate;
	double        AMinVi;
	double        AMaxVi;
	double        AA;
	double        AB;
	double        AC;
	double        BMinVi;
	double        BMaxVi;
	double        BA;
	double        BB;
	double        BC;
public:
	TRFlowIVM();
	~TRFlowIVM();
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
