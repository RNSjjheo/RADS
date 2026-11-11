//---------------------------------------------------------------------------

#ifndef UnitLogH
#define UnitLogH

#include <vcl.h>

//---------------------------------------------------------------------------

class TLog {
private:
	UnicodeString m_sHomeDir;
	int           m_iYear;
	int           m_iMonth;
	int           m_iDay;
public:
	TLog();
	TLog(UnicodeString sHomeDir);
	~TLog();
	void SetHomeDir(UnicodeString sHomeDir);
	bool LogMessage(UnicodeString sMsg);
	bool MakeDir(UnicodeString sDir);
	bool MakeFile(UnicodeString sFile);
	bool IsExistDir(UnicodeString sDir);
	bool IsExistFile(UnicodeString sFile);
};

extern TLog theLog;

#endif
