//---------------------------------------------------------------------------
#ifndef LogThreadH
#define LogThreadH

#include <vcl.h>
//#include "Thrdqueue.h"
#include "fbTable.h"
#include "LockeCommon.h"

//---------------------------------------------------------------------------
class TLogThread : public TThread
{
private:
    TfbTable *RSenderLog;
    void WriteDBLog(UnicodeString sDateTime, UnicodeString sText);
    void WriteFileLog(UnicodeString sPathFile, UnicodeString sDate, UnicodeString sText);
    void __fastcall PerformLog();
protected:
	void __fastcall Execute();
public:
	__fastcall TLogThread();
	__fastcall ~TLogThread();

	STLogMessage *m_pLogMessage;

	bool bWorking;	// Thread working or idle
};
//---------------------------------------------------------------------------
extern PACKAGE TLogThread *thrdLog;
//---------------------------------------------------------------------------
#endif
