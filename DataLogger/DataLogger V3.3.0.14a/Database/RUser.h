//---------------------------------------------------------------------------

#ifndef RUserH
#define RUserH
//---------------------------------------------------------------------------


#include "RBase.h"

class TRUser : public TRBase
{
public:
	UnicodeString UserID;
	UnicodeString UserName;
	UnicodeString Password;
	UnicodeString Privilege;
	UnicodeString Pos;
	UnicodeString Company;
	UnicodeString Phone;

public:
	TRUser();
	~TRUser();
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
