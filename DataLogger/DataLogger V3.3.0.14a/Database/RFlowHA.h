//---------------------------------------------------------------------------

#ifndef RFlowHAH
#define RFlowHAH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRFlowHA : public TRBase
{
public:
	UnicodeString PointCode;
	UnicodeString UseDate;
	double        MinH;
	double        MaxH;
	double        A6;
	double        A5;
	double        A4;
	double        A3;
	double        A2;
	double        A1;
	double        A0;
public:
	TRFlowHA();
	~TRFlowHA();
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
