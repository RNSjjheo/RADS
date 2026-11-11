//---------------------------------------------------------------------------
/*
	Thrdqueue
  by Song, Deuk Bo
*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Contnrs.hpp>
#include "MultipleEvents.h"

#ifndef ThrdqueueH
#define ThrdqueueH
////////////////////////////////////////////////////////////////////////////////
//큐 데이터 구조체
struct TpbNormalQData
{
	char *pbPacket;
};
struct TNormalQData //UnicodeString 기본 구조체
{
	UnicodeString sText;
};
struct TReceiveTimeQData //일시 포함
{
	UnicodeString sDateTime;
	UnicodeString sText;
};
struct TWriteLogFileData
{
    bool bFileWrite;
    bool bDBWrite;
    bool bLogMessage;
	UnicodeString sDate;
	UnicodeString sText;
};
struct TFTPOutQData //ftp 전송용 데이터 구조체
{
    UnicodeString sMeasureDate;
    UnicodeString sMeasureTime;
    UnicodeString sLocalPathFile; //local 폴더/파일명
    UnicodeString sFTPRootPathFile; //ftp연결시 대상 폴더/파일명
};
struct TSendToViewers
{
    int iSize;
    char *sBuffer;
};
//---------------------------------------------------------------------------
struct TQueueNode
{
	void *Data;
	TQueueNode *Next;
};
//---------------------------------------------------------------------------
class TThrdqueue
{
private:
    TQueue *q;
	TCriticalSection *csThrdqueue;
	TMultipleEvents *MultipleEvents;
	UnicodeString sEventID;
public:
	TThrdqueue(TMultipleEvents *meEvents, UnicodeString sEventID, TCriticalSection *csGlobal);
	~TThrdqueue();
	int Count();
	void Push(UnicodeString sEventID, TQueueNode *aNode);
	TQueueNode* Pop();
};
//---------------------------------------------------------------------------
extern TThrdqueue *qSendToViewers; //뷰어에 패킷 전송 큐
extern TThrdqueue *qWriteLogDBnFile; //로그 텍스트 파일저장 큐
extern TThrdqueue *qVthResponse; //vth모달폼을 위해 vth의 리턴값이 따로 저장되는 큐
extern TThrdqueue *qFTPOutput; //ftp 전송용 자료를 저장하는 큐
extern TThrdqueue *qVTHOutput; //vth시리얼 전송을 위한 데이터 큐
extern TThrdqueue *qFTPQutputRun; //ftp스레드의 전송작업을 시작하게 하는 큐
extern TThrdqueue *qVTHInput; //vth 시리얼로부터 수신신호 저장 큐
extern TThrdqueue *qDataViewerInput; //DataViewer로부터 수신한 신호저장 큐
extern TThrdqueue *qGuardnet; //가드넷 호출 큐
extern TThrdqueue *qimgFTPStatus;
extern TThrdqueue *qimgGuardnetStatus;
extern TThrdqueue *qimgLineStatus;
extern TThrdqueue *qimgVthStatus;
extern TThrdqueue *qimgLoggerStatus;
extern TThrdqueue *qimgWdStatus;
extern TThrdqueue *qimgViewerStatus;
//---------------------------------------------------------------------------
#endif
