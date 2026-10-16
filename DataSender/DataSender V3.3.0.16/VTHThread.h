//---------------------------------------------------------------------------
#include <vcl.h>
#include "Thrdqueue.h"

#ifndef VTHThreadH
#define VTHThreadH

#include "fbTable.h"
//---------------------------------------------------------------------------
class TVTHThread : public TThread
{
private:
    TfbTable *RPoint;
    TfbTable *RVTHLogger;
protected:
	void __fastcall Execute();
    void DataViewerReceive();
    void VTHSend();
    void VTHReceive();
public:
	__fastcall TVTHThread();
    __fastcall ~TVTHThread();
};
//---------------------------------------------------------------------------
extern PACKAGE TVTHThread *thrdVTH;
//---------------------------------------------------------------------------
#endif
