//---------------------------------------------------------------------------
#pragma hdrstop

#include "MultipleEvents.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMultipleEvents *meFtp = NULL;
TMultipleEvents *meGuardnet = NULL;
TMultipleEvents *meLog = NULL;
//---------------------------------------------------------------------------
TMultipleEvents::TMultipleEvents(UnicodeString sEventNames)
{
	iEventCount = GetSepItemCount(sEventNames, ",");
	Events = new HANDLE[iEventCount];
	slEventNames = new TStringList();
	for(int i = 0; i < iEventCount; i++)
	{
		Events[i] = CreateEvent(NULL, false, false, UnicodeString(GetSepItem(sEventNames, i, ",")).c_str());
		slEventNames->Add(GetSepItem(sEventNames, i, ","));
  } // for i
}
//---------------------------------------------------------------------------
TMultipleEvents::~TMultipleEvents()
{
    delete[] Events;
	delete slEventNames;
}
//---------------------------------------------------------------------------
UnicodeString TMultipleEvents::RunEvent(UnicodeString sEventName)
{
	UnicodeString sErrorMessage = "";

	int idx = slEventNames->IndexOf(sEventName);
	if (idx == -1)
	{
		return "이벤트 명이 없습니다.";
	} // if
	try
	{
		ResetEvent(Events[idx]);
    	SetEvent(Events[idx]);
	} catch(Exception &e)
	{
        sErrorMessage = e.Message;
	} // catch

	return sErrorMessage;
}
//---------------------------------------------------------------------------
HANDLE *&TMultipleEvents::GetEventHandles()
{
	return Events;
}
//---------------------------------------------------------------------------
