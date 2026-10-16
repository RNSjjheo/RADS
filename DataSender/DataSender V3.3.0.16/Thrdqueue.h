#ifndef ThrdqueueH
#define ThrdqueueH

#include <vcl.h>
#include "MultipleEvents.h"

////////////////////////////////////////////////////////////////////////////////
//큐 데이터 구조체

struct TpbNormalQData
{
	char *pbPacket;
};

struct TNormalQData 	//UnicodeString 기본 구조체
{
	UnicodeString sText;
};

struct TReceiveTimeQData 	//일시 포함
{
	UnicodeString sDateTime;
	UnicodeString sText;
};

struct TWriteLogFileData
{
    bool bFileWrite;
    bool bDBWrite;
	bool bLogMessage; 		//true면 로그메모, false면 가드넷메모
	UnicodeString sDate;
	UnicodeString sText;
};

struct TFTPOutQData 		//ftp 전송용 데이터 구조체
{
	bool          bReal; 			//real인지 유무
    UnicodeString sMeasureDate;
    UnicodeString sMeasureTime;
	UnicodeString sLocalPathFile; 	//local 폴더/파일명
    UnicodeString sFTPRootPathFile; //ftp연결시 대상 폴더/파일명
};

struct TSendToViewers
{
	int   iSize;
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
	TQueue           *q;
	TCriticalSection *csThrdqueue;
	TMultipleEvents  *MultipleEvents;
	UnicodeString     sEventID;
public:
	TThrdqueue(TMultipleEvents *meEvents, UnicodeString sEventID, TCriticalSection *csGlobal);
	~TThrdqueue();
	int         Count();
	void        Push(UnicodeString sEventID, TQueueNode *aNode);
	TQueueNode *Pop();
};

//---------------------------------------------------------------------------

//extern TThrdqueue *qWriteLogDBnFile; 	//로그 텍스트 파일저장 큐
extern TThrdqueue *qFTPOutput; 			//ftp 전송용 자료를 저장하는 큐
extern TThrdqueue *qFTPQutputRun; 		//ftp스레드의 전송작업을 시작하게 하는 큐
extern TThrdqueue *qGuardnet; 			//가드넷 호출 큐

//---------------------------------------------------------------------------

#endif


