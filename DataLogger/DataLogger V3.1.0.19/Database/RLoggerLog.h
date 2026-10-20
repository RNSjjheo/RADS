//---------------------------------------------------------------------------

#ifndef RLoggerLogH
#define RLoggerLogH
//---------------------------------------------------------------------------


#include "RBase.h"

class TLoggerLog : public TRBase
{
public:
	TDateTime     LogDateTime;
	UnicodeString LogMessage;
public:
	TLoggerLog();
	~TLoggerLog();
	void Clear();
	bool Insert();
	bool Update();
	bool Delete();
	bool IsExist();
	bool Check(int iAction);
	bool GetData();
};

#endif
