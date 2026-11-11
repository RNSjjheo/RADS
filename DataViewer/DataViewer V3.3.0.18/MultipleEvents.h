//---------------------------------------------------------------------------
#include <vcl.h>

#ifndef MultipleEventsH
#define MultipleEventsH
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
extern TMultipleEvents *meVTHEvents;
extern TMultipleEvents *meGuardnet;
extern TMultipleEvents *meLog;
//---------------------------------------------------------------------------
#endif
