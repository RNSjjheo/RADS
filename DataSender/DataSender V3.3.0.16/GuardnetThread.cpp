//---------------------------------------------------------------------------
#pragma hdrstop

//#include <math.h>
#include "GuardnetThread.h"
#include "UnitMain.h"
#include "LockeCommon.h"
//#include "Env.h"
//#include "Thrdqueue.h"

//#include <iphlpapi.h>		// Routing 정보
//#pragma comment(lib, "iphlpapi.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TGuardnetThread *thrdGuardnet;

//---------------------------------------------------------------------------
__fastcall TGuardnetThread::TGuardnetThread() : TThread(false)
{
	bTesting        = false;
    dtWirelessStart = Now(); //무선 시작시간
	iNextStep       = 0;
	iRetryCnt       = 0; //iPingCnt의 N회 재시도 회수
	iGWResetCnt     = 0; //g/w reset 회수
	iPCRebootCnt    = 0; //1일 pc reboot회수
	Priority        = tpLower;
	sPingError      = "";
	sWireIP         = "";
	sWirelessIP     = "";

	tmrPing           = new TTimer(Application);
	tmrPing->Enabled  = false;
	tmrPing->Interval = 1;
	tmrPing->OnTimer  = tmrPingTimer;

	FreeOnTerminate = true;
}

//---------------------------------------------------------------------------
__fastcall TGuardnetThread::~TGuardnetThread()
{
	//delete tmrPing;
	//TThread::~TThread();
}

//---------------------------------------------------------------------------
bool TGuardnetThread::PerformLine2Wireless()
//유선->무선 전환
{
	gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF); //현재의 네트웍 상태
	//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF); //현재의 네트웍 상태
	switch(gnStatus.cPrimary)
	{
        case lkLine: //현재 유선우선이면
            if (gnStatus.bWirelessEnabled == true)
			{
				_ChangeRoutingTable(iLineIF, iWirelessIF, true);
				//_ChangeRoutingTable(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF, true);
				gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF);
				//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF);
				switch(gnStatus.cPrimary)
                {
                    case lkWireless:
                        dtWirelessStart = Now(); //무선 시작시간
                        frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유선->무선 전환 성공", false);
                        break;
                    default:
                        frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유선->무선 전환 실패", false);
                        break;
                } // switch
            } else frmMain->LogMessage(CbFileWrite, !CbDBWrite, "무선 라우팅 테이블이 없어서 전환 실패", false);
            break;
        case lkWireless: //무선
            frmMain->LogMessage(CbFileWrite, !CbDBWrite, "이미 무선상태입니다.", false);
            break;
	} // if

	UpdateMainFormIcon(gnStatus);	// MainForm Update

    return gnStatus.cPrimary == lkWireless;
}

//---------------------------------------------------------------------------
bool TGuardnetThread::PerformWireless2Line()
//무선->유선 전환
{
	gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF); //현재의 네트웍 상태
	//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF); //현재의 네트웍 상태
	switch(gnStatus.cPrimary)
	{
        case lkWireless: //현재 무선우선이면
            if (gnStatus.bLineEnabled == true)
			{
				_ChangeRoutingTable(iLineIF, iWirelessIF, false);
				//_ChangeRoutingTable(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF, false);
				gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF);
				//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF);
				switch(gnStatus.cPrimary)
				{
                    case lkLine:
                        frmMain->LogMessage(CbFileWrite, !CbDBWrite, "무선->유선 전환 성공", false);
						break;
                    default:
                        frmMain->LogMessage(CbFileWrite, !CbDBWrite, "무선->유선 전환 실패", false);
                        break;
                } // switch
            } else frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유선 라우팅 테이블이 없어서 전환 실패", false);
            break;
        case lkLine: //유선
            frmMain->LogMessage(CbFileWrite, !CbDBWrite, "이미 유선상태입니다.", false);
			break;
	} // if

	UpdateMainFormIcon(gnStatus);	// MainForm Update

    return gnStatus.cPrimary == 0x1;
}

//---------------------------------------------------------------------------
void __fastcall TGuardnetThread::tmrPingTimer(TObject *Sender)
{
	tmrPing->Enabled = false;
	try
	{
		//sPingError = IcmpPing(sPingIP);	// heaven2 2017.10.17
		UnicodeString sLocalIP = "";
		sLocalIP = (gnStatus.cPrimary == 0x01) ? sWireIP : sWirelessIP;	// 유선LAN Card IP 또는 무선 Lan Card IP
//debug
UnicodeString Message =  (gnStatus.cPrimary == 0x00) ? "0 Unknown.."   :     \
						((gnStatus.cPrimary == 0x01) ? "1 유선 테스트" :     \
						((gnStatus.cPrimary == 0x02) ? "2 무선 테스트" : "발생할 수 없는 상황"));
frmMain->LogMessage(CbFileWrite, !CbDBWrite, Message, false);
		sPingError = IcmpPing(sPingIP, sLocalIP);
	} catch(Exception &e)
    {
		sPingError = e.Message;
    } // catch
	bPingRan = true;
}

//---------------------------------------------------------------------------
bool TGuardnetThread::PingOk(int iServerNo)
//서버에 핑 테스트
{
    bool bResult;

    UnicodeString sPingServer = (iServerNo == 1) ? frmMain->DBEnvInfo.sGNPingServer1 : frmMain->DBEnvInfo.sGNPingServer2;
    int iPingPort = (iServerNo == 1) ? frmMain->DBEnvInfo.iGNPingServer1Port : frmMain->DBEnvInfo.iGNPingServer2Port;
    sPingIP = Domain2IP(sPingServer);

    sPingError = "";
	bPingRan = false;
    int iTickStart = GetTickCount();
	tmrPing->Enabled = true;
	while(!bPingRan)
	{
		Sleep(100);
        int iTickNow = abs(GetTickCount() - iTickStart);
		if (iTickNow > 5000) break; //5초 초과시 실패로 간주
	} // while

	bResult = sPingError == "";
	if (!bResult) frmMain->LogMessage(CbFileWrite, !CbDBWrite, "핑 에러: " + sPingError, false);

	return bResult;
}

//---------------------------------------------------------------------------
// MainForm의 네트워크 상태 Icon을 Update한다.
void TGuardnetThread::UpdateMainFormIcon(TGuardnetStatus GuardnetStatus)
{
	switch(GuardnetStatus.cPrimary)
    {
        case lkLine:
			frmMain->fLineStatus = gniLine;
			break;
		case lkWireless:
			frmMain->fLineStatus = gniWireless;
			break;
	} // switch

	Synchronize(SetLineIcon);
}

//---------------------------------------------------------------------------
void __fastcall TGuardnetThread::Execute()
{
	iPCRebootCnt = frmMain->DBEnvInfo.iPCRebootCnt;
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "금일 PC reboot 누적회수: " + IntToStr(iPCRebootCnt), false);
	UnicodeString sToday = Now().FormatString("yyyymmdd");

	// heaven2 2026.02.13
	iLineIF     = GetIFByDescription(frmMain->DBEnvInfo.sGNNetworkWiredIFDesc);  	// 유선 IF 번호를 구한다.
	iWirelessIF = GetIFByDescription(frmMain->DBEnvInfo.sGNNetworkWirelessIFDesc);	// 무선 IF 번호를 구한다.

	try
	{
		TWaitResult Result;
		STGuardnetMessage *pGuardnetMessage = NULL;
		while ( !Terminated ) {
			bTesting = false;
			Result = pGGuardnetEvent->WaitFor(1000);
			bTesting = true;
			if ( Result == wrSignaled || Result == wrTimeout ) {
				while ( pGGuardnetQueue->Count() ) {
					pGGuardnetSection->Enter();
					pGuardnetMessage = (STGuardnetMessage *)pGGuardnetQueue->Pop();
					pGGuardnetSection->Leave();

					int iJobType = 0;
					if (pGuardnetMessage->sText == CsGNPingTest) iJobType = 1;
					if (pGuardnetMessage->sText == CsGNChange2W) iJobType = 2;
					if (pGuardnetMessage->sText == CsGNChange2L) iJobType = 3;

					iLineIF     = GetIFByDescription(frmMain->DBEnvInfo.sGNNetworkWiredIFDesc);  	// 유선 IF 번호를 구한다.
					iWirelessIF = GetIFByDescription(frmMain->DBEnvInfo.sGNNetworkWirelessIFDesc);	// 무선 IF 번호를 구한다.

					switch(iJobType)
					{
						case 1:
							frmMain->fGuardNetStatus = gniWorking;
							Synchronize(SetGuardnetIcon);

							PerformGuardnetTest(); //일반적인 가드넷 테스트
							break;
						case 2:
							PerformLine2Wireless(); //유선->무선 전환
							break;
						case 3:
							PerformWireless2Line(); //무선->유선 전환
							break;
					} // switch
					delete pGuardnetMessage;
				} // end of while
			} else if ( Result == wrAbandoned ) {	// Event Object was destored
													// wrError: An error occured while waiting.
													// Check the LastError property for an error
													// code giving more informaiton.
			} else {
				// nothing to do..
			}
		} // end of while

		frmMain->tmrWD->Enabled = false; //watchdog 전송중지
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "GuardnetThread 종료", false);
	} // try
	catch (Exception &e)
	{
		frmMain->tmrWD->Enabled = false; //watchdog 전송중지
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "GuardnetThread 오류: " + e.Message, false);
	} // catch
}

//---------------------------------------------------------------------------
TGuardnetStatus TGuardnetThread::GetGuardnetStatus()
//현재의 라우팅테이블 정보를 리턴
{
	gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF);
	//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF);
	return gnStatus;
}

//---------------------------------------------------------------------------
bool TGuardnetThread::IsTesting()
//가드넷 테스트 중인지 여부를 리턴
{
    return bTesting;
}

//---------------------------------------------------------------------------
void TGuardnetThread::DoPing(TLineKind lKind, int iPingServerNo)
{
    bool bOk = false;

    //회선변경
    if (gnStatus.cPrimary != lKind && gnStatus.bLineEnabled && gnStatus.bWirelessEnabled)
    {
        switch(gnStatus.cPrimary)
        {
            case lkLine: //유선
                PerformLine2Wireless(); //무선으로 전환
                break;
            case lkWireless: //무선
                PerformWireless2Line(); //유선으로 전환
                break;
        } // switch
		gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF); //현재의 네트웍 상태
		//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF); //현재의 네트웍 상태
		UpdateMainFormIcon(gnStatus);	// MainForm Update
	} // if

    iRetryCnt++;
    frmMain->LogMessage(CbFileWrite, !CbDBWrite, "단계 " + IntToStr(iNextStep + 1) + " 재시도 회수:" + IntToStr(iRetryCnt), false);
	for(int i = 0; i < frmMain->DBEnvInfo.iGNRetryCount; i++)
    {
		if (PingOk(iPingServerNo))
        {
            frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Server" + IntToStr(iPingServerNo) + " Ping #" + IntToStr(i + 1) + " 성공", false);
            bOk = true;
			iNextStep = 0;
            iRetryCnt = 0;
            break;
        } else frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Server" + IntToStr(iPingServerNo) + " Ping #" + IntToStr(i + 1) + " 실패", false);
    } // for i
	if (bOk) frmMain->fGuardNetStatus = gniOk;
	else     frmMain->fGuardNetStatus = gniError;
	Synchronize(SetGuardnetIcon);

    if (bOk == false && iRetryCnt == frmMain->DBEnvInfo.iGNPingRetryCount)
    {
        iRetryCnt = 0;
        iNextStep++;
    } // if
}

//---------------------------------------------------------------------------
void TGuardnetThread::PerformGuardnetTest()
//일반적인 가드넷 테스트
{
    bTesting = true;
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Guardnet 테스트 시작", false);

	gnStatus = _GetRoutingTableInfo(iLineIF, iWirelessIF); //현재의 네트웍 상태
	//gnStatus = _GetRoutingTableInfo(frmMain->DBEnvInfo.iGNNetworkWiredIF, frmMain->DBEnvInfo.iGNNetworkWirelessIF); //현재의 네트웍 상태
	UpdateMainFormIcon(gnStatus);	// MainForm Update
	// heaven2 2026.03.02
	sWireIP     = GetIPByIF(iLineIF); 		// heaven2 2017.10.17
	sWirelessIP = GetIPByIF(iWirelessIF);

	// heaven2 2015.05.19 라우팅 정보가 맞지 않는다면 Ping Test를 하지 않고 Skip한다.
	if ( gnStatus.cPrimary == 0x00 ) {
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "설정의 IF정보와 시스템의 IF 정보가 일치하지 않습니다. Ping Test를 Skip합니다. ", false);
		return;
	}
	// end

	if (gnStatus.cPrimary == lkWireless && gnStatus.bLineEnabled) //현재 무선인데 유선이 활성화된 상태면 무선사용시간 체크해서 유선으로 전환
	{
		if ((Now() - dtWirelessStart).FormatString("hhnn").ToInt() >= frmMain->DBEnvInfo.iGNWirelessUseMinute)
		{
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "무선 사용시간이 만료되어 유선으로 자동 전환합니다.", false);
			PerformWireless2Line();
        } // if
    } // if

    switch(gnStatus.cPrimary)
    {
        case lkLine:
            frmMain->LogMessage(CbFileWrite, !CbDBWrite, "현재 유선", false);
            break;
        case lkWireless:
            frmMain->LogMessage(CbFileWrite, !CbDBWrite, "현재 무선", false);
            break;
    } // switch

    bool bOk = false;
	switch(iNextStep)
    {
        case 0: //유선
            if (gnStatus.cPrimary == lkWireless && gnStatus.bLineEnabled == false) iNextStep = 1; //무선만 있을 경우 무선테스트 핑1서버
            if (gnStatus.bLineEnabled == false && gnStatus.bWirelessEnabled == false) iNextStep = 4; //유무선 둘 다 없으면 테스트 제외
            if (gnStatus.bLineEnabled == true && gnStatus.bWirelessEnabled == true && gnStatus.cPrimary == lkWireless) iNextStep = 1; //양쪽 회선이 다 있고 무선이면 1차를 건너띄고 2차 무선테스트부터 진행
            break;
        case 1: //무선
            if (gnStatus.cPrimary == lkLine && gnStatus.bWirelessEnabled == false) iNextStep = 3; //유선만 있을 경우 유선테스트 핑2서버
            if (gnStatus.bLineEnabled == false && gnStatus.bWirelessEnabled == false) iNextStep = 4; //유무선 둘 다 없으면 테스트 제외
            break;
        case 2: //무선
            if (gnStatus.cPrimary == lkLine && gnStatus.bWirelessEnabled == false) iNextStep = 3; //유선만 있을 경우 유선테스트 핑2서버
            if (gnStatus.bLineEnabled == false && gnStatus.bWirelessEnabled == false) iNextStep = 4; //유무선 둘 다 없으면 테스트 제외
            break;
        case 3: //유선
            if (gnStatus.cPrimary == lkWireless && gnStatus.bLineEnabled == false) iNextStep = 4; //무선만 있을 경우 무선테스트 핑1서버
            if (gnStatus.bLineEnabled == false && gnStatus.bWirelessEnabled == false) iNextStep = 4; //유무선 둘 다 없으면 테스트 제외
            break;
    } // switch

    switch(iNextStep)
    {
        case 0: //서버1 ping 1차
            DoPing(lkLine, 1);
            break;
        case 1: //서버1 ping 2차
            DoPing(lkWireless, 1);
            break;
        case 2: //서버2 ping 1차
            if (frmMain->DBEnvInfo.bGNPingServer2TestOn) DoPing(lkWireless, 2);
            else
            {
                frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Ping Server 2 테스트가 비활성화되어 3단계를 스킵합니다.", false);
                iNextStep = 4;
			} // else
            if (iNextStep != 4) break;
        case 3: //서버2 ping 2차
            if (iNextStep == 3)
            {
                if (frmMain->DBEnvInfo.bGNPingServer2TestOn) DoPing(lkLine, 2);
                else
                {
                    frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Ping Server 2 테스트가 비활성화되어 4단계를 스킵합니다.", false);
                    iNextStep = 4;
                } // else
            } // if
            if (iNextStep != 4) break;
        case 4: //g/w reset
            iGWResetCnt++;
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "G/W reset 누적 " + IntToStr(iGWResetCnt) + " 회 신호전송", false);
			frmMain->GiGWResetFrom = 2;
			frmMain->SendVTH(AnsiString("!"));
			//PostMessage(frmMain->Handle, WM_USER + tmtWriteToVTH, '!', 0); //g/w reset 신호 전송

			frmMain->fGuardNetStatus = gniReboot;
			Synchronize(SetGuardnetIcon);

			if (iGWResetCnt == frmMain->DBEnvInfo.iGNGatewayRebootMaxCount && (iPCRebootCnt + 1) <= frmMain->DBEnvInfo.iGNRebootMaxCount)
			{
				iPCRebootCnt++;
				iGWResetCnt = 0;
				iRetryCnt = 0;

				//ini에 pc 부팅회수 저장
				WriteIni(ExtractFilePath(Application->ExeName) + "DataSender.ini", "ETC", CsGNDayPcRebootCnt, Now().FormatString("yyyymmdd") + "," + IntToStr(iPCRebootCnt));

				//pc재부팅
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[GuardnetThread] PC 재부팅", false);
				ForcePCReboot();
			} // if
			iNextStep = 0;
			break;
	} // switch

	bTesting = false;
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "Guardnet 테스트 종료", false);
}

////---------------------------------------------------------------------------
//MIB_IPFORWARDTABLE *TGuardnetThread::_GetForwardTable()
//{
//    MIB_IPFORWARDTABLE *prTable = NULL;
//    DWORD dwTableSize = 0;
//    prTable = (MIB_IPFORWARDTABLE*)malloc(sizeof(MIB_IPFORWARDTABLE));
//	if (GetIpForwardTable(prTable, &dwTableSize, false) == ERROR_INSUFFICIENT_BUFFER)
//    {
//        free(prTable);
//        prTable = (MIB_IPFORWARDTABLE*)malloc(dwTableSize);
//    } // if
//    DWORD dwRetVal = GetIpForwardTable(prTable, &dwTableSize, false);
//    if (dwRetVal != NO_ERROR && prTable != NULL)
//    {
//        free(prTable);
//        prTable = NULL;
//    } // else
//    return prTable;
//}

////---------------------------------------------------------------------------
//void TGuardnetThread::_ChangeRoutingTable(int iLineIF, int iWirelessIF, bool bWire2Wireless)
//{
//    int iLineMetric = 0;
//    int iWirelessMetric = 0;
//	MIB_IPFORWARDROW iprowLine, *piprowLine = NULL;
//	MIB_IPFORWARDROW iprowWireless, *piprowWireless = NULL;
//
//	MIB_IPFORWARDTABLE *prTable = _GetForwardTable();
//
//	if (prTable != NULL)
//    {
//        for (int i = 0; i < prTable->dwNumEntries; i++)
//        {
//            MIB_IPFORWARDROW aTable = prTable->table[i];
//            if (aTable.dwForwardType != MIB_IPROUTE_TYPE_INDIRECT) //MIB_IPROUTE_TYPE_INDIRECT=a remote destination(공유기 자체)
//				continue;
//            if (aTable.dwForwardIfIndex == iLineIF) //유선
//            {
//                iLineMetric = aTable.dwForwardMetric1;
//                iprowLine = aTable;
//                piprowLine = prTable->table + i;
//            } // if
//            if (aTable.dwForwardIfIndex == iWirelessIF) //무선
//            {
//				iWirelessMetric = aTable.dwForwardMetric1;
//                iprowWireless = aTable;
//				piprowWireless = prTable->table + i;
//            } // if
//		} // for i
//
//		if (  piprowLine == NULL ||  piprowWireless == NULL ) {
//			free(prTable);
//			return;
//		}
//
//		DWORD dwStatus;
//
//		if ( bWire2Wireless ) {	// Wired => Wireless first
//			piprowLine->dwForwardMetric1 = iWirelessMetric+1;
//			dwStatus = SetIpForwardEntry(piprowLine);
//
//			//piprowWireless->dwForwardMetric1 = 10;
//			//dwStatus = SetIpForwardEntry(piprowWireless);
//		} else {				// Wireless => Wired first
//			piprowLine->dwForwardMetric1 = iWirelessMetric-1;
//			dwStatus = SetIpForwardEntry(piprowLine);
//
//			//piprowWireless->dwForwardMetric1 = 40;
//			//dwStatus = SetIpForwardEntry(piprowWireless);
//		}
//	} // if
//
//	if (prTable != NULL) free(prTable);
//}
//
////---------------------------------------------------------------------------
//TGuardnetStatus TGuardnetThread::_GetRoutingTableInfo(int iLineIF, int iWirelessIF)
//{
//	MIB_IPFORWARDTABLE *prTable = _GetForwardTable();
//
//    TGuardnetStatus gsResult;
//    gsResult.cPrimary = 0x0;
//    gsResult.bLineEnabled = false;
//    gsResult.bWirelessEnabled = false;
//    gsResult.iLineMetric = 10000;
//    gsResult.iWirelessMetric = 10000;
//
//    if (prTable != NULL)
//    {
//        for (int i = 0; i < prTable->dwNumEntries; i++)
//        {
//            MIB_IPFORWARDROW aTable = prTable->table[i];
//            if (aTable.dwForwardType != MIB_IPROUTE_TYPE_INDIRECT) //MIB_IPROUTE_TYPE_INDIRECT=a remote destination(공유기 자체)
//                continue;
//            if (aTable.dwForwardIfIndex == iLineIF) //유선
//			{
//                gsResult.bLineEnabled = true;
//                gsResult.iLineMetric = aTable.dwForwardMetric1;
//            } // if
//            if (aTable.dwForwardIfIndex == iWirelessIF) //무선
//            {
//                gsResult.bWirelessEnabled = true;
//                gsResult.iWirelessMetric = aTable.dwForwardMetric1;
//            } // if
//		} // for i
////        gsResult.cPrimary = (gsResult.iLineMetric == gsResult.iWirelessMetric) ? 0x0 :
////                            (gsResult.iLineMetric < gsResult.iWirelessMetric) ? 0x1 :
////                            0x2; //metric(connection cost to the internet) 값이 작은게 최우선임
//		if ( !gsResult.bLineEnabled && !gsResult.bWirelessEnabled ) {
//			free(prTable);
//			return gsResult;	// 2015.05.19 양쪽 Metric 정보가 맞지 않는 상태 => 아무 짓도 하지 않는다.
//		} else {
//			gsResult.cPrimary = (gsResult.iLineMetric <= gsResult.iWirelessMetric) ? 0x1 : 0x2; //metric(connection cost to the internet) 값이 작은게 최우선임
//		}
//	} // if
//
//    //유/무선 상태 화면 표시
//    switch(gsResult.cPrimary)
//    {
//        case lkLine:
//			frmMain->fLineStatus = gniLine;
//            break;
//        case lkWireless:
//			frmMain->fLineStatus = gniWireless;
//            break;
//	} // switch
//
//	Synchronize(SetLineIcon);
//
//	if (prTable != NULL) free(prTable);
//
//    return gsResult;
//}

//---------------------------------------------------------------------------
void __fastcall TGuardnetThread::SetGuardnetIcon()
{
	frmMain->ChangeGuardNetIcon(0);
}

//---------------------------------------------------------------------------
void __fastcall TGuardnetThread::SetLineIcon()
{
	frmMain->ChangeLineIcon(0);
}

