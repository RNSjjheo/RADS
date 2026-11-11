//---------------------------------------------------------------------------

#ifndef RCheckReportH
#define RCheckReportH
//---------------------------------------------------------------------------


#include "RBase.h"

class TRCheckReport : public TRBase
{
public:

public:
	TRCheckReport();
	~TRCheckReport();
	void Clear();
	bool Insert();
	bool Update();
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
};

#endif
