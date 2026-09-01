//---------------------------------------------------------------------------
//#include <vcl.h>
//#include <ipmib.h>
//#pragma hdrstop
//#include "Env.h"

#ifndef GuardnetThreadH
#define GuardnetThreadH

#include <vcl.h>
//#include <ipmib.h>		// 이 library를 사용할 것
//#include <Iprtrmib.h>	// MSDN은 이것이지만 ipmib.h를 사용할 것 .. 이 해더를 쓰면 AsyncPro와 Conflict
#include "Env.h"

enum TLineKind { lkLine = 1, lkWireless = 2 };

//---------------------------------------------------------------------------
class TGuardnetThread : public TThread
{
private:
	bool          bTesting;
	TDateTime     dtWirelessStart;
	int           iNextStep;
	int           iRetryCnt; //iPingCnt의 N회 재시도 회수
	int           iGWResetCnt; //g/w reset 회수
	int           iPCRebootCnt; //1일 pc reboot회수
	TTimer        *tmrPing;
	UnicodeString sPingError;
	UnicodeString sPingIP;
	UnicodeString sWireIP;
	UnicodeString sWirelessIP;
	bool          bPingRan;
	TGuardnetStatus gnStatus;
	int             iLineStatus;

	int            iLineIF;		// 유선 IF 번호
	int            iWirelessIF;	// 무선 IF 번호

//	MIB_IPFORWARDTABLE *_GetForwardTable();
//	void            _ChangeRoutingTable(int iLineIF, int iWirelessIF, bool bWire2Wireless=true);
//	TGuardnetStatus _GetRoutingTableInfo(int iLineIF, int iWirelessIF);
	void            PerformGuardnetTest(); //일반적인 가드넷 테스트
	bool            PerformLine2Wireless(); //유선->무선 전환
	bool            PerformWireless2Line(); //무선->유선 전환
	bool            PingOk(int iServerNo);  //핑
	void            DoPing(TLineKind lKind, int iPingServerNo);
	void            UpdateMainFormIcon(TGuardnetStatus GuardnetStatus);
protected:
	void __fastcall Execute();
	void __fastcall SetGuardnetIcon();	//Synchronize function
	void __fastcall SetLineIcon();    	//Synchronize function
public:
	void __fastcall tmrPingTimer(TObject *Sender);
	__fastcall      TGuardnetThread();
	__fastcall      ~TGuardnetThread();
	TGuardnetStatus GetGuardnetStatus();
	bool            IsTesting();
};
//---------------------------------------------------------------------------
extern PACKAGE TGuardnetThread *thrdGuardnet;
//---------------------------------------------------------------------------
#endif
