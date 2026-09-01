//---------------------------------------------------------------------------

#ifndef LogThreadH
#define LogThreadH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include "UnitLog.h"	// Log Class

//class TLog;

//---------------------------------------------------------------------------
struct STLogMessage
{
	TDateTime     LogTime;
	UnicodeString LogMessage;
	bool          bFile;
};


//---------------------------------------------------------------------------
class TLogThread : public TThread
{
public:
	TLog m_Log; 	// Log Write class
private:
	UnicodeString m_sUILogMessage;
	void __fastcall UpdateLogMemo();
protected:
	void __fastcall Execute();
	void LogWrite(STLogMessage *pLogMessage);
public:
	__fastcall TLogThread(bool CreateSuspended);
	void InitThread(UnicodeString sHomeDir);
};
//---------------------------------------------------------------------------
#endif
