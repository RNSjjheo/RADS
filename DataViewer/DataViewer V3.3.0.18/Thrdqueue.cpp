#pragma hdrstop

#include "Thrdqueue.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TThrdqueue *qSendToViewers; //뷰어에 패킷 전송 큐
TThrdqueue *qDataViewerInput; //DataViewer로부터 수신한 신호저장 큐

TThrdqueue *qWriteLogDBnFile; //로그 텍스트 파일저장 큐

TThrdqueue *qFTPOutput; //ftp 전송용 자료를 저장하는 큐
TThrdqueue *qFTPQutputRun; //ftp 자료를 전송하도록 스레드를 시작하는 큐

TThrdqueue *qVthResponse; //vth모달폼을 위해 vth의 리턴값이 따로 저장되는 큐
TThrdqueue *qVTHOutput; //vth시리얼 전송을 위한 큐
TThrdqueue *qVTHInput; //vth 시리얼로부터 수신신호 저장 큐

TThrdqueue *qGuardnet; //가드넷 호출 큐
//이미지 관련 큐
TThrdqueue *qimgFTPStatus;
TThrdqueue *qimgGuardnetStatus;
TThrdqueue *qimgLineStatus;
TThrdqueue *qimgVthStatus;
TThrdqueue *qimgLoggerStatus;
TThrdqueue *qimgWdStatus;
TThrdqueue *qimgViewerStatus;
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
