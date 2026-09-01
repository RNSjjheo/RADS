//---------------------------------------------------------------------------

#ifndef RSetupH
#define RSetupH
//---------------------------------------------------------------------------

#include "RBase.h"

class TRSetup : public TRBase
{
public:
	int           PK1;
	int           PK2;
	UnicodeString Value01;
	UnicodeString Value02;
	UnicodeString Value03;
	UnicodeString Value04;
	UnicodeString Value05;
	UnicodeString Value06;
	UnicodeString Value07;
	UnicodeString Value08;
	UnicodeString Value09;
	UnicodeString Value10;
	UnicodeString Value11;
	UnicodeString Value12;
	UnicodeString Value13;
	UnicodeString Value14;
	UnicodeString Value15;
	UnicodeString Value16;
	UnicodeString Value17;
	UnicodeString Value18;
	UnicodeString Value19;
	UnicodeString Value20;
	UnicodeString Des;
public:
	TRSetup();
	~TRSetup();
	void Clear();
	bool Insert();
	bool Update();
	bool Update(UnicodeString sColumnName);
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
};

#endif
