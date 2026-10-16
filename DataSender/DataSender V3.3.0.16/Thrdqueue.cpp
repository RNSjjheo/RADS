//---------------------------------------------------------------------------
#pragma hdrstop

#include "Thrdqueue.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//TThrdqueue *qWriteLogDBnFile; 	//로그 텍스트 파일저장 큐

TThrdqueue *qFTPOutput; 		//ftp 전송용 자료를 저장하는 큐
TThrdqueue *qFTPQutputRun; 		//ftp 자료를 전송하도록 스레드를 시작하는 큐

TThrdqueue *qGuardnet; 			//가드넷 호출 큐

//---------------------------------------------------------------------------
TThrdqueue::TThrdqueue(TMultipleEvents *meEvents, UnicodeString sEID, TCriticalSection *csGlobal)
{
    q = new TQueue();
    csThrdqueue = csGlobal;
	MultipleEvents = meEvents;
	sEventID = sEID;
} // constructor

//---------------------------------------------------------------------------
TThrdqueue::~TThrdqueue()
{
    delete q;
	delete csThrdqueue;
} // destructor

//---------------------------------------------------------------------------
void TThrdqueue::Push(UnicodeString sEventID, TQueueNode *qNode)
{
    csThrdqueue->Acquire();
    try
    {
        q->Push(qNode);
    } catch(...)
    {
    } // catch
    if (MultipleEvents != NULL && sEventID != "") MultipleEvents->RunEvent(sEventID);
    csThrdqueue->Release();
} // Push

//---------------------------------------------------------------------------
TQueueNode* TThrdqueue::Pop()
{
   	TQueueNode *qNode = NULL;
	csThrdqueue->Acquire();
    try
    {
        if (q->Count() > 0) qNode = (TQueueNode *)q->Pop();
    } catch(...)
    {
    } // catch
  	csThrdqueue->Release();

	return qNode;
} // Pop

//---------------------------------------------------------------------------
int TThrdqueue::Count()
{
	csThrdqueue->Acquire();
	int iCount = q->Count();
	csThrdqueue->Release();
	return iCount;
} // Count
//---------------------------------------------------------------------------

