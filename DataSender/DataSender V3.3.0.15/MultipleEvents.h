//---------------------------------------------------------------------------
#ifndef MultipleEventsH
#define MultipleEventsH

#include <vcl.h>

//---------------------------------------------------------------------------
class TMultipleEvents
{
private:
	HANDLE *Events;
	TStringList *slEventNames;
public:
	int iEventCount;
	TMultipleEvents(UnicodeString sEventNames);
	~TMultipleEvents();
	UnicodeString RunEvent(UnicodeString sEventName);
	HANDLE *&GetEventHandles();
};
//---------------------------------------------------------------------------
extern TMultipleEvents *meFtp;
extern TMultipleEvents *meGuardnet;
extern TMultipleEvents *meLog;
//---------------------------------------------------------------------------
#endif
