//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.DateUtils.hpp>
//#include <IniFiles.hpp>
//#include <Psapi.h>
#pragma hdrstop

#include "UnitMain.h"
#include "VTHThread.h"
#include "FtpThread.h"
#include "GuardnetThread.h"
#include "LogThread.h"
#include "UnitVth.h"
#include "UnitConfig.h"

#include "MultipleEvents.h"
#include "RVTHLogger.h"
#include "RWaterLevel.h"
#include "RHydroMeter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdPort"
#pragma link "OoMisc"
//#pragma link "SBSimpleSftp"	// 반드시 여기에 놓을 것.. (Main에서 SFTP를 추가할 때)
#pragma resource "*.dfm"

TfrmMain *frmMain = NULL;

TEvent *pGLogEvent  = new TEvent(NULL, false, false, NULL, false);
TEvent *pGFTPEvent  = new TEvent(NULL, false, false, NULL, false);
TEvent *pGGuardnetEvent = new TEvent(NULL, false, false, NULL, false);

TCriticalSection *pGLogSection = new TCriticalSection(); 	// LogMessage Critical Section
TCriticalSection *pGFTPSection = new TCriticalSection();	// FTP Queue Critical Section
TCriticalSection *pGGuardnetSection = new TCriticalSection();	// Guardnet Queue Critical Section

TQueue *pGLogQueue = new TQueue();     						// Log Queue
TQueue *pGFTPQueue = new TQueue();							// FTP Queue
TQueue *pGGuardnetQueue = new TQueue();						// Guardnet Queue

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	GiGWResetFrom   = 0;
	GbVthDlgRequest = false;
	GbGWPowerOff    = false;
	GsMeasureDate   = "";
	GsMeasureTime   = "";
	GiLogOperation  = loIdle;

	fFTPStatus      = fsIdle; //ftp상태
	fFTPImageStatus = fipEnabled;
	fGuardNetStatus = gniIdle;
	fLineStatus     = 0;
	fVTHCharge      = 0;
	fVTHTemperature = 0;
	fVTHHumidity    = 0;
	fVTHBattery     = 0;
	f5V             = 0;
	f3V             = 0;
	fAIN0           = 0;
	fAIN1           = 0;
	fAIN2           = 0;
	fAIN3           = 0;
	sVTHMsg         = "";

	m_bExit         = false;

	shpProgress->Brush->Color = clLime;
	shpProgress->Width = 0;

	m_LoggerTime = Now();
	m_VTHTimeStamp = Now();	// VTHLogger Get TimeStamp
	m_bVTHRequest  = false;	// VTHLogger Data request flag => prevent data flooding
	m_VTHMsg     = "";
//heaven2 SecureBlackBox License Key
//#error Please pick the evaluation license key from <SecureBlackbox>\LicenseKey.txt file and place it here. If the evaluation key expires, you can request an extension using the form on http://www.secureblackbox.com/order/keyreq/
SetLicenseKey((AnsiString)"91A071FEDAD312FA39410BB2A39E13EF7011BAD5A172A4F68C33B88FBF1979C9" +
                          "029D327BDED248CEAE24B0E22C65638A2CE3605F7EFD1DF3AE2237EC85A24FBE" +
                          "1F5253E8F76BB34E16F25DC75F453C1CA5A16CFF294DBA8B71228E77BFDD3FCA" +
                          "BF91DA93A2B3669115146301A35D3882BFA9B8F86F52D3E851E4F99E6C484EEA" +
                          "E31E120729778C30F89E90C9E78248480A4BA2E289430E2A76B3992BADFE26F1" +
                          "E572D502F279B52B9940AAD498CF3662CBDB0B57B944E6F9489ABC38D0B029FE" +
                          "E19A3FE8588B60A5CC0198B18060C3808CE79008B86F2AC5102BE517EA8F3E6D" +
                          "B623A23B844F1267F1FB2350D16595B015930B7153583DEBF3128B1E14C58DAF");
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnWindowSizeToggleClick(TObject *Sender)
{
	if (btnWindowSizeToggle->Caption == "▲")
	{
		SetUIExtend(true);
		//this->Height = 660;
		//btnWindowSizeToggle->Caption = "▼";
		//this->Top = Screen->WorkAreaHeight - this->Height;
	} else
	{
		SetUIExtend(false);
		//this->Height = 200;
		//btnWindowSizeToggle->Caption = "▲";
		//this->Top = Screen->WorkAreaHeight - this->Height;
	} // else
}

//---------------------------------------------------------------------------
TDateTime GetStartTime()
{
    unsigned short iY, iM, iD, iH, iN, iS, iMs;
    DecodeDate(Now(), iY, iM, iD);
    DecodeTime(Now(), iH, iN, iS, iMs);
    if ((iN % 10) >= 5) iN = iN / 10 * 10 + 5;
    else
    {
        if (iN >= 5) iN = iN / 10 * 10 - 5;
        else
        {
            iH -= 1;
            iN = 55;
        } // else
    } // else
    iS = iMs = 0;
    TDateTime dtResult = EncodeDate(iY, iM, iD) + EncodeTime(iH, iN, iS, iMs);
    return dtResult;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrDisplayTimeTimer(TObject *Sender)
{
	static bool bGuardnetTimer = false;
	static bool bCheckDBForFTP = false;

	TDateTime now = Now();
	USHORT h,m,s,ss;
	now.DecodeTime(&h, &m, &s, &ss);

	//현재의 시분초 표시
	lbTime->Caption = now.FormatString("hh:nn:ss");

	//logger 진행시간 표시
	//static TDateTime dtStart = Now(); // = GetStartTime();
	switch(GiLogOperation)
	{
		case loIdle: //유휴
			{
//              dtStart = GetStartTime();
                //GiLogOperation = loLogging;
                break;
            }
		case loStart: //시작
			{
				GiLogOperation = loLogging; //진행중
				//dtStart = Now() - 1.157407407407407e-5; //1초 뺌
				m_LoggerTime = now;
			}
            break;
		case loLogging: //진행중
			{
				lbPTime->Caption = FormatDateTime("nn:ss", now - m_LoggerTime);
				break;
			}
		case loFinish: //완료
			{
				lbPTime->Caption = "00:00";
				break;
			}
	} // switch

	//가드넷 검사시간 설정
    if (DBEnvInfo.bGNEnable)
    {
        if (DBEnvInfo.cGNType == 0x0) //0x0=normal(특정시간에 pc부팅)
        {
            if (DBEnvInfo.sGNRebootTime.Trim() == now.FormatString("hh:nn:ss"))
            {
				GiGWResetFrom = 0;
				SendVTH("!"); //g/w reset 신호 전송
				ForcePCReboot(); //pc재부팅
			} else {
				// 예약된 시간이 아니므로 Skip한다.
			}// if
		}
		else
		{
			//if (tmrGuardnet->Enabled == false)
			if ( !bGuardnetTimer ) // GuardNet Timer가 Enable되어 있지 않으면 이 루틴을 탄다.
			{
				tmrGuardnet->Interval = DBEnvInfo.iGNPingInterval * 1000;
				if (DBEnvInfo.sGNPingStartTime.Trim().UpperCase() == "CURRENT")
				{
					tmrGuardnet->Enabled = true; //가드넷 타이머 바로 작동
					tmrGuardnetTimer(NULL);
					bGuardnetTimer = true;
				}
				else //매분 0초에 실행
				{
					if (now.FormatString("ss") == "00")
					{
						tmrGuardnet->Enabled = true; //가드넷 타이머 작동
						tmrGuardnetTimer(NULL);
						bGuardnetTimer = true;
					} else {
						// 0초가 아니므로 그냥 Skip한다.
					}// if
				} // else
			} // if
		} // else
	}
	else
	{
		//SetBitmapFromImageList(imgGuardnetStatus, gniDisabled);
		//SetBitmapFromImageList(imgLineStatus, gniDisabled);
	} // else

	// Background FTP Check [ xx:x[1,5,7,9]:00 에 수행
	// 1분, 5분, 7분, 9분에 시행. 3분은 REAL Data 전송으로 Skip한다.
	if ((((m%10) == 1) || ((m%10) == 5) || ((m%10) == 7) || ((m%10) == 9)) && (s == 0)) {
		::PostMessage(this->Handle, UM_MESSAGE, PAST_FILE_CHECK_JOB, 0);
	}

//	if ( s == 15 || s == 45 ) {	// Every XX:XX:[15,45], DB Connection check.. 2022.04.27 DB Connection Check job
	if ( s == 15 ) {	// 2024.08.22 통일대교: 45초의 Health Check 시점에 DataLogger Finish Event와 동일시점에서 발생 : 동시성 제어 문제 발생 => 45초 이벤트 빼고 처리함.
		::PostMessage(this->Handle, UM_MESSAGE, DB_CONNECTION_CHECK, 0);
	}

	// Gargage Collection => xx:00:00 매 시간마다 한 번씩 Garbage Collecton 수행
//	if ( m == 0 && s == 0 ) {	// 매 시간 정각에 수행함.
//		::PostMessage(this->Handle, UM_MESSAGE, GARBAGE_COLLECTION, 0);
//	}

//	//backgroound ftp 검사시간 설정
//	//if (!tmrCheckDBForFTP->Enabled)
//	if ( !bCheckDBForFTP )  	// 2마다 동작하는 Backgroud FTP Timer가 아직 동작중이 아니면 .
//	{
//		int iMinuteRest = m % 2;
////		if (iMinuteRest + (DBEnvInfo.iLGStartTime % 2) == 1 && now.FormatString("ss") == "00")
//		if ((iMinuteRest == 1) && (s == 0))	// 1,3,5,7,9,11,13,..분에 Fire됨.
//		{
//			bCheckDBForFTP = true;
//			tmrCheckDBForFTP->Enabled = true; //매2분마다 실행됨
//			tmrCheckDBForFTPTimer(NULL);
//		} // if
//	} // if

	// VTHLogger Data Check   2015.08.19
	if (((m%2) == 0) && (s == 0)) SendVTH("@"); // Send VTHData Request // 짝수분 0,2,4,6,8분에 Request한다. (매 분 Request하면 너무 자주함. )

}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (!m_bExit)
	{
		Action = caNone;
		GoToSystemTray();
		return;
	} else
	{
		Action = caFree;
		return;
	} // else
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Open1Click(TObject *Sender)
{
	RestoreFromTray();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Close1Click(TObject *Sender)
{
	m_bExit = true;
	Close();
}

//---------------------------------------------------------------------------
void TfrmMain::GoToSystemTray()
{
	Hide();
	WindowState = wsMinimized;
	TrayIcon1->Visible = true;
}

//---------------------------------------------------------------------------
void TfrmMain::RestoreFromTray()
{
	TrayIcon1->Visible = false;
	this->Height = 175;
	btnWindowSizeToggle->Caption = "▲";
	LocateRightButton();
	Show();
	WindowState = wsNormal;
	Application->BringToFront();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrGoToTrayTimer(TObject *Sender)
{
	tmrGoToTray->Enabled = false;
	GoToSystemTray();
}

//---------------------------------------------------------------------------
void TfrmMain::LocateRightButton()
{
	Forms::TMonitor *pMon = NULL;
	for(int i = 0; i < Screen->MonitorCount; i++)
	{
		Forms::TMonitor *m = Screen->Monitors[i];
		if(m->Primary == true)
		{
			pMon = m;
			break;
		} // if
	} // for i

	if (pMon == NULL) return;

	this->Left = Screen->WorkAreaWidth - this->Width;
	this->Top  = Screen->WorkAreaHeight - this->Height;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::TrayIcon1DblClick(TObject *Sender)
{
	RestoreFromTray();
}

//---------------------------------------------------------------------------
void TfrmMain::ReadIniInfo()
{
	UnicodeString sIniFile = ExtractFilePath(Application->ExeName) + "DataSender.ini";
	IniInfo.sIP      = ReadIni(sIniFile, "DATABASE", "DatabaseIP", "127.0.0.1");
	IniInfo.sName    = ReadIni(sIniFile, "DATABASE", "DatabaseName", "RADS");
	IniInfo.sAccount = ReadIni(sIniFile, "DATABASE", "DatabaseAccount", "rads");
    //pc 리부팅 회수 가져오기; ini의 값을 읽어 일자가 당일과 일치할때만 회수를 가져온다
	UnicodeString sText = ReadIni(sIniFile, "ETC", CsGNDayPcRebootCnt, "");
    UnicodeString sDay = GetSepItem(sText, 0, ",").Trim();
    int iCnt = GetSepItem(sText, 1, ",").ToIntDef(0);
	if (sDay == Now().FormatString("yyyymmdd")) DBEnvInfo.iPCRebootCnt = iCnt;
	else                                        DBEnvInfo.iPCRebootCnt = 0;
}

//---------------------------------------------------------------------------
void TfrmMain::LogMessage(bool bFileWrite, bool bDBWrite, UnicodeString sMessage, bool bLogMessage)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	pGLogSection->Enter();

	STLogMessage *pLogMessage = new STLogMessage();

	pLogMessage->bFileWrite  = bFileWrite;
	pLogMessage->bDBWrite    = bDBWrite;
	pLogMessage->bLogMessage = bLogMessage;
	pLogMessage->sText       = sMessage;
	pLogMessage->sDate       = Now().FormatString("yyyy-mm-dd hh:nn:ss.zzz");

	pGLogQueue->Push(pLogMessage);

	if ( !thrdLog->bWorking ) pGLogEvent->SetEvent();	// Send Event to LogThread

//	//로그를 파일에 저장하기위해 큐에 넣음
//	TWriteLogFileData *nqData = (TWriteLogFileData*)GlobalAlloc(GPTR, sizeof(TWriteLogFileData));
//	CheckAllocateMemory(nqData);	// heaven2
//	nqData->bFileWrite = bFileWrite;
//	nqData->bDBWrite = bDBWrite;
//	nqData->bLogMessage = bLogMessage;
//	nqData->sText = sMessage;
//	nqData->sDate = Now().FormatString("yyyy-mm-dd hh:nn:ss.zzz");
//	TQueueNode *aNode = (TQueueNode*)GlobalAlloc(GPTR, sizeof(TQueueNode));
//	CheckAllocateMemory(aNode);	// heaven2
//	aNode->Data = nqData;
//	qWriteLogDBnFile->Push(CsWriteLogDBnFile, aNode);

	pGLogSection->Leave();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	UnicodeString sFilePath = PathStrAdd(DBEnvInfo.sPubSenderHome, CsLogPath) + "\\" +
                                         DBEnvInfo.sPointCode + "_" + Now().FormatString("yyyymm") + "\\" +
                                         DBEnvInfo.sPointCode + "_" + Now().FormatString("yyyymmdd") + ".log";
	UnicodeString sTime = "           " + Now().FormatString("hh:nn:ss");

    WriteFileLog(sFilePath, sTime, "GuardnetThread 종료");
    WriteFileLog(sFilePath, sTime, "VTHThread 종료");
    WriteFileLog(sFilePath, sTime, "FtpThread 종료");
    WriteFileLog(sFilePath, sTime, "DataSender 프로그램 종료");

    TfbTable *RSenderLog = new TfbTable(SQLConnection, "RSENDERLOG");
    RSenderLog->ClearParams();
    RSenderLog->SetFieldParam("LOGDATETIME", FormatDateTime("yyyy-mm-dd hh:nn:ss", Now()));
	RSenderLog->SetFieldParam("LOGMESSAGE", "DataSender 프로그램 종료");
    RSenderLog->Insert();
	delete RSenderLog;

	// Close Serial Port
	if (ApdComPort->Open) ApdComPort->Open = false;

	// Close DataViewer Socket
	lstViewer->Clear();
	delete lstViewer;

	// Queue Release
	while ( pGGuardnetQueue->Count() ) delete (STGuardnetMessage *)pGGuardnetQueue->Pop();
	while ( pGFTPQueue->Count() )      delete (STFTPMessage *)pGFTPQueue->Pop();
	while ( pGLogQueue->Count() )      delete (STLogMessage *)pGLogQueue->Pop();

    //스레드 해제
	thrdGuardnet->Terminate();	// GuardnetThread Terminate
	pGGuardnetEvent->SetEvent();
	thrdFTP->Terminate();		// FTPThread Terminate
	pGFTPEvent->SetEvent();
	thrdLog->Terminate();		// LogThread Terminate
	pGLogEvent->SetEvent();

	SleepMsg(300);	// wait 100ms with process window message

	//delete thrdGuardnet;
	//delete thrdFTP;
	//delete thrdLog;

	// Packet Resource
	delete pbLogger;
	delete pbViewer;
	//heaven2 2015.02.03

	delete SearchDBRPoint;

	// Event
	delete pGLogEvent;
	delete pGFTPEvent;
	delete pGGuardnetEvent;

	// Critical Section
	delete pGLogSection; 		// LogMessage Critical Section
	delete pGFTPSection;		// FTP Queue Critical Section
	delete pGGuardnetSection;	// Guardnet Queue Critical Section

	// Queue
	delete pGLogQueue;
	delete pGFTPQueue;
	delete pGGuardnetQueue;

	//Sleep(300);
	//SleepMsg(100); // Wait for while  for remain window message
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ViewerServerConnect(TIdContext *AContext)
{
	lstViewer->Add(AContext);
	int iCount = ViewerServer->Contexts->Count();
	if (iCount > 0 && iCount <= 5) //SetBitmapFromImageList(imgViewerStatus, asViewEnabled + iCount * 2);
		ChangeViewerIcon(asViewEnabled + iCount * 2);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoggerServerExecute(TIdContext *AContext)
{
	try
	{
		if (AContext->Connection->Socket->InputBufferIsEmpty())
        {
            Sleep(50);
            return;
        } // if
		while(!AContext->Connection->Socket->InputBufferIsEmpty())
        {
        	char ch = AContext->Connection->Socket->ReadByte();
			pbLogger->WriteByte(ch);
        } // while
	} // try
	catch(Exception &e)
	{
		if (!m_bExit) //프로그램 종료가 아닐떄
		{
			LogMessage(CbFileWrite, !CbDBWrite, "DataLogger 신호수신 오류: " + e.Message);
		} // if
		return;
	} // catch

	char Packet[1024];
	memset(Packet, 0x00, 1024);
	int iSize = 0;

	//if (pbLogger->GetPacket(Packet, iSize)) // heaven2 2020.07.14
	while (pbLogger->GetPacket(Packet, iSize))
	{
		UnicodeString sError = SockPacketValid(Packet, iSize);
		if (sError == "")
		{
			if ( Packet[1] == MSG_TYPE_LoggerActionEvent ) {
				//logging status
				switch(Packet[3])
				{
					case loIdle:	//Idle
						{
							LogMessage(CbFileWrite, !CbDBWrite, "DataLogger Idle 신호 수신");
							break;
						}
					case loStart:	//Start
						{
							LogMessage(CbFileWrite, !CbDBWrite, "DataLogger Start 신호 수신");
							shpProgress->Brush->Color = clLime;
							shpProgress->Tag          = 0;
							shpProgress->Width        = 0;
							GiLogOperation            = loStart;
							m_LoggerTime              = Now();
							break;
						}
					case loLogging:	//Logging
						{
							LogMessage(CbFileWrite, !CbDBWrite, "DataLogger Logging 신호 수신");
							if ( GiLogOperation == loStart ) {	// Logging 중간에 실행된 상태
								m_LoggerTime = Now();
							}
							shpProgress->Brush->Color = clLime;
							shpProgress->Tag          = shpProgress->Tag + 1;
							shpProgress->Width        = (shpBG1->Width / 17) * shpProgress->Tag;
							GiLogOperation            = loLogging;
							break;
						}
					case loFinish:	//Finish
						{
							LogMessage(CbFileWrite, !CbDBWrite, "DataLogger Finish 신호 수신");

							shpProgress->Brush->Color = clLime;
							shpProgress->Width        = shpBG1->Width;
							GiLogOperation            = loFinish;

							char szDate[9] = { 0, }, szTime[7] = { 0, };
							strncpy(szDate, Packet + 7, 8);
							GsMeasureDate = szDate;
							strncpy(szTime, Packet + 15, 6);
							strncpy(szTime + 3, "000", 3); 							//분-초 모두 ?0:00로 rpoint키 생성
							GsMeasureTime = szTime;

							SendVTH("@");	// Send VTH to Request

							//PostMessage(Handle, WM_USER + tmtWriteToVTH, '@', 0); 	//VTH에 상태 정보 요청
							//finish신호후 자료를 전송할 자료파일 생성/전송

							//Sleep(5000);	// VTHLogger Thread가 데이터를 정리할 동안 기다린다.
							//				// VTHLogger가 응답이 느릴 경우 최장 3초 정도가 소요된다.
							//HWait(5000);	// 5초간 기다림.

							SleepMsg(5 * 1000);	// 2015.01.29 5초간 기다림.

							SaveVTHData();

							SearchDBMakeFtpFiles(GsMeasureDate, GsMeasureTime); //DB를 쿼리하여 ftp파일이 생성되지않은 자료가 있으면 생성

							if (DBEnvInfo.bServerEnable || DBEnvInfo.bRnseaEnable)
								SendFtpFiles(true); //DB에서 자료 쿼리를 쿼리하여 ftp output큐에 전송

							shpProgress->Width = 0;

							// VTHLogger TimeStamp Check
							if ( ApdComPort->Open ) { // VTH Comport가 열려 있으면 데이터 수신 유무를 Check한다.
								TDateTime Span = Now() - m_VTHTimeStamp;
								if ( (double)Span >= TDateTime(1,0,0,0) ) {   // 1시간 동안 (xxx20분) 동안 VTHLogger 데이터를 수집하지 못함 => Watchdog에 Restart 요청
									// WatchDog에 Restart 요청함.
									LogMessage(CbFileWrite, !CbDBWrite, "VTHLogger 데이터 수집 못하여 WatchDog에 Restart 요청함.");
									BYTE Action = ACTION_SHUTDOWN_REQUEST;
									SendWatchDogMessage(Action);
								}
							}
							break;
						}
				} // switch
			} else if ( Packet[1] == MSG_TYPE_Ping      || Packet[1] == MSG_TYPE_PingAck  ||
						Packet[1] == MSG_TYPE_RebootReq || Packet[1] == MSG_TYPE_RebootRes ) {

				ProcessLoggerMessage(Packet, iSize);	// Ping,PingAck,RebootReq,RebootRes Message

			} else {
				LogMessage(CbFileWrite, !CbDBWrite, "Logger Message unknown..");
			}
		} else LogMessage(CbFileWrite, !CbDBWrite, sError);
		// clear
		memset(Packet, 0x00, 1024);
		iSize = 0;
	} // ene of while
	//} // if.. end of while
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrWDTimer(TObject *Sender)
{
	//Shutdown Request Test 하기 위한 임시 코드
	//static int iCnt = 0;
	//iCnt++;

	if (WatchDogClient->Connected()) WatchDogClient->CheckForGracefulDisconnect(false);
	if (!WatchDogClient->Connected())
	{
		try
		{
			WatchDogClient->Host = DBEnvInfo.sWatchDogIP;
			WatchDogClient->Port = DBEnvInfo.iWatchDogPort;
			WatchDogClient->Connect();
			LogMessage(CbFileWrite, !CbDBWrite, "WatchDog 연결 시도.. ");
		} // try
		catch(Exception &e)
		{
			LogMessage(CbFileWrite, !CbDBWrite, "WatchDog 연결실패: " + e.Message);
			//SetBitmapFromImageList(imgWdStatus, asWdDisabled); //watchdog 상태표시
			ChangeWatchDogIcon(asWdDisabled);
		} // catch
	} // if
	if (WatchDogClient->Connected())
	{
		BYTE Action = ACTION_HEARTBEAT;

		SIZE_T MemorySize = GetProcessWorkingSetSize();

		//if ( MemorySize > 50 * 1024 || iCnt >= 10 ) { // 50Mbyte 보다 크면
		if ( MemorySize > 50 * 1024 ) { // 50Mbyte 보다 크면
			Action = ACTION_SHUTDOWN_REQUEST;
			UnicodeString sMsg;
			sMsg = sMsg.sprintf(L"DataSender Memory excceded limitation(%d MBytes). Sender request shutdown to WatchDog..", MemorySize);
			LogMessage(CbFileWrite, !CbDBWrite, sMsg);
		}

		SendWatchDogMessage(Action);
	}
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::ViewerServerExecute(TIdContext *AContext)
{
	try
	{
        if (AContext->Connection->Socket->InputBufferIsEmpty())
		{
            Sleep(50);
            return;
        } // if
        while(!AContext->Connection->Socket->InputBufferIsEmpty())
		{
        	char ch = AContext->Connection->Socket->ReadByte();
			pbViewer->WriteByte(ch);
        } // while
	} // try
	catch(Exception &e)
	{
		if (!m_bExit) //프로그램 종료가 아닐떄
		{
			if (e.Message.Pos("Connection Closed Gracefully") == 0) LogMessage(CbFileWrite, !CbDBWrite, "DataViewer 신호수신 오류발생: " + e.Message);
		} // if
		return;
	} // catch

	char Packet[1024];
	memset(Packet, 0x00, 1024);
	int   iSize    = 0;

	while (pbViewer->GetPacket(Packet, iSize))
	{
		UnicodeString sError = SockPacketValid(Packet, iSize);
		if (sError == "")
		{
			BYTE cType = *(Packet+1);
			int  iLen  = *(Packet+2);

			if ( cType == MSG_TYPE_SenderSetupChange ) {
				LogMessage(CbFileWrite, !CbDBWrite, "DataViewer 설정정보변경 신호수신, Send Shutdown message to WatchDog..");

				BYTE Action = ACTION_SHUTDOWN_REQUEST;
				SendWatchDogMessage(Action);

			} else if ( cType == MSG_TYPE_SenderUIOpen ) {
				LogMessage(CbFileWrite, !CbDBWrite, "DataViewer UI Open Request received");

				RestoreFromTray();
				SetUIExtend(true);

				HWND hWnd = this->Handle;
				::SetForegroundWindow( hWnd );
				::SetFocus( hWnd );

			} else {
				LogMessage(CbFileWrite, !CbDBWrite, "Unknown Message Type from DataViewer..");
			}
		} else LogMessage(CbFileWrite, !CbDBWrite, sError);
	} // while
}

//---------------------------------------------------------------------------
bool TfrmMain::ConnectDB()
{
	bool bResult = true;

	SQLConnection->Close();
	SQLConnection->Params->Values["Database"] = IniInfo.sIP + ":" + IniInfo.sName;
	SQLConnection->Params->Values["User_Name"] = IniInfo.sAccount;
	try
	{
		SQLConnection->Open();
	} // try
	catch(Exception *e)
	{
		bResult = false;
	} // catch

	return bResult;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ApdComPortTriggerAvail(TObject *CP, WORD Count)
{
	// 2015.01.09 buffering이 너무 길면 Clear 시킨다.
	if ( m_VTHMsg.Length() > 200 ) {	// Buffering이 길어지만 Clear 시킨다.
		m_VTHMsg = ""; // Buffer Clear
		LogMessage(CbFileWrite, !CbDBWrite, "VTHMsg is too long.. VTHMsg cleared.. and Continue..");
	}

	char PreBuffer[1024];
	memset(PreBuffer, 0x0, sizeof(PreBuffer));

	try {
		ApdComPort->GetBlock(PreBuffer, Count);
	} catch ( Exception &e ) {
		LogMessage(CbFileWrite, !CbDBWrite, "VTH Error " + e.Message);
		return;
	}

	// just for test ( 2015.01.09 )
	//LogMessage(CbFileWrite, !CbDBWrite, "VTH Receive <= " + AnsiString(PreBuffer));

	m_VTHMsg += AnsiString(PreBuffer);

	int iPos = 0;
	bool bCRLF = false;
	bool bCR = false;
	iPos = m_VTHMsg.AnsiPos("\r\n");
	if ( iPos > 0 ) {
		bCR = false;     bCRLF = true;
	} else 	{
		iPos = m_VTHMsg.AnsiPos("\r");
		if ( iPos > 0 ) {
			bCR = true;  bCRLF = false;
		} else {
			bCR = false, bCRLF = false;
		}
	}
	AnsiString sMsg = "";
	//while ( iPos > 0 ) {
	while ( bCRLF || bCR ) {
		sMsg = m_VTHMsg.SubString(1, iPos-1);	//abcd\r\nabce\r\n

		////////////////////////////////////////////////////////////////////////
		// Main Process
		VTHProcess(Trim(sMsg));
		////////////////////////////////////////////////////////////////////////

		//m_VTHTimeStamp = Now();		// Save Last VTHLogger TimeStamp

		if ( bCRLF ) {	// bCRLF == true
			m_VTHMsg = m_VTHMsg.SubString(iPos+2, m_VTHMsg.Length() - ( iPos + 1));
		} else 	{ // bCR == true
			m_VTHMsg = m_VTHMsg.SubString(iPos+1, m_VTHMsg.Length() - ( iPos ));
		}

		iPos = m_VTHMsg.AnsiPos("\r\n");
		if ( iPos > 0 ) {
			bCR = false;     bCRLF = true;
		} else 	{
			iPos = m_VTHMsg.AnsiPos("\r");
			if ( iPos > 0 ) {
				bCR = true;  bCRLF = false;
			} else {
				bCR = false, bCRLF = false;
			}
		}
	}

	//2015.01.09, 2015.01.12 Comment
	//if (GetSepItemCount(m_VTHMsg, ",") == CiVthItemCount) {
	//	VTHProcess(Trim(m_VTHMsg));
	//	m_VTHMsg = "";
	//	LogMessage(CbFileWrite, !CbDBWrite, "VTH Error : CR LF 없음 => 처리함 ");
	//}

	// 2015.01.09 buffering이 너무 길면 Clear 시킨다.
	if ( m_VTHMsg.Length() > 200 ) {	// Buffering이 길어지만 Clear 시킨다.
		m_VTHMsg = ""; // Buffer Clear
		LogMessage(CbFileWrite, !CbDBWrite, "VTH Error : Buffering Data Length > 200, Buffer Clear..");
	}
}

//---------------------------------------------------------------------------
void TfrmMain::VTHProcess(AnsiString sMsg)	// VTHLogger 수신한 내역의 처리
{
	if ( m_bVTHRequest ) {
		LogMessage(CbFileWrite, !CbDBWrite, "VTH 수신: " + sMsg.Trim());
		m_bVTHRequest = false;
	}

	m_VTHTimeStamp = Now();		// VTHLogger TimeStamp Refresh

	if ( GbVthDlgRequest ) {	// VTH Dialog가 열려 있을 때

		if ( frmVth != NULL ) {
			frmVth->Receive(sMsg);
		} else {
			//	Unknown Status
		}

	} else {   					// Main 화면에서 받았을 대

		if (GetSepItemCount(sMsg, ",") == CiVthItemCount) {	// VTH Data Receive

			m_VTHTimeStamp = Now();		// Save Last VTHLogger TimeStamp

//			TfbTable *RPoint;
//			TfbTable *RVTHLogger;
//
//			RPoint     = new TfbTable(frmMain->SQLConnection, "RPOINT");
//			RVTHLogger = new TfbTable(frmMain->SQLConnection, "RVTHLOGGER");

			// heaven2  2014.06.25
			fVTHCharge      = StrToFloat(GetSepItem(sMsg, 5, ","));
			fVTHTemperature = StrToFloat(GetSepItem(sMsg, 0, ","));
			fVTHHumidity    = StrToFloat(GetSepItem(sMsg, 1, ","));
			fVTHBattery     = StrToFloat(GetSepItem(sMsg, 2, ","));
			f5V             = StrToFloat(GetSepItem(sMsg, 3, ","));  // heaven2 2015.08.19
			f3V             = StrToFloat(GetSepItem(sMsg, 4, ","));
			fAIN0           = StrToFloat(GetSepItem(sMsg, 6, ","));
			fAIN1           = StrToFloat(GetSepItem(sMsg, 7, ","));
			fAIN2           = StrToFloat(GetSepItem(sMsg, 8, ","));
			fAIN3           = StrToFloat(GetSepItem(sMsg, 9, ","));
			sVTHMsg         = sMsg;

			ChangeVTHValue();	// UI Update
/*
			bool bExist;
			RPoint->ClearParams();
			RPoint->SetWhereParam("MEASUREDATE", GsMeasureDate);
			RPoint->SetWhereParam("MEASURETIME", GsMeasureTime);

			try
			{
				bExist = RPoint->Exists();
			} // try
			catch(Exception &e)
			{
				LogMessage(CbFileWrite, !CbDBWrite, "DB 쿼리 오류: " + e.Message);
				LogMessage(CbFileWrite, !CbDBWrite, RPoint->SqlText);
			} // catch

			if (!bExist)
			{
				RPoint->ClearParams();
				RPoint->SetFieldParam("MEASUREDATE", GsMeasureDate);
				RPoint->SetFieldParam("MEASURETIME", GsMeasureTime);
				RPoint->SetFieldParam("FILEFLAG",   "N");
				RPoint->SetFieldParam("SERVERFLAG", "N");
				RPoint->SetFieldParam("RNSEAFLAG",  "N");
				UnicodeString sErrorMessage = RPoint->Insert();
				if (sErrorMessage != "")
				{
					LogMessage(CbFileWrite, !CbDBWrite, "RPoint DB저장 오류: " + sErrorMessage);
					LogMessage(CbFileWrite, !CbDBWrite, RPoint->SqlText);
				} // if
			} // if

			//RVTHLOGGER에 insert
			//UnicodeString sVolt = (StrToFloat(GetSepItem(sMsg, 5, ",")) == 0.0f) ? "0" : "220"; //dc charge=0이면 전압도 0
			UnicodeString sVolt;
			if ( DBEnvInfo.sACOpt.IsEmpty() || DBEnvInfo.sACOpt.UpperCase() == "BATTERY" ) {
				sVolt = (StrToFloat(GetSepItem(sMsg, 2, ",")) < 10.0f) ? "0" : "220"; //DC Battery 10Volt 이하면 AC Fail
			} else {
				sVolt = (StrToFloat(GetSepItem(sMsg, 6, ",")) < 10.0f) ? "0" : "220"; //AIN0 10volt 이하이면 AC Fail
			}

			RVTHLogger->ClearParams();
			RVTHLogger->SetFieldParam("MEASUREDATE", GsMeasureDate); 	//일자
			RVTHLogger->SetFieldParam("MEASURETIME", GsMeasureTime); 	//시간
			RVTHLogger->SetFieldParam("VTHKIND", "VTH Logger"); 					//"VTH Logger"
			RVTHLogger->SetFieldParam("VOLT",        sVolt); 						//전압
			RVTHLogger->SetFieldParam("TEMPERATURE", GetSepItem(sMsg, 0, ",")); 	//온도
			RVTHLogger->SetFieldParam("HUMIDITY",    GetSepItem(sMsg, 1, ",")); 	//습도
			RVTHLogger->SetFieldParam("DCCHARGE",    GetSepItem(sMsg, 5, ",")); 	//dc charge
			RVTHLogger->SetFieldParam("DCBATTERY",   GetSepItem(sMsg, 2, ",")); 	//dc battery
			RVTHLogger->SetFieldParam("Value01",     GetSepItem(sMsg, 6, ",")); 	//AIN0
			RVTHLogger->SetFieldParam("Value02",     GetSepItem(sMsg, 7, ",")); 	//AIN1
			RVTHLogger->SetFieldParam("Value03",     GetSepItem(sMsg, 8, ",")); 	//AIN2
			RVTHLogger->SetFieldParam("Value04",     GetSepItem(sMsg, 9, ",")); 	//AIN3
			RVTHLogger->SetFieldParam("RAWDATA",     sMsg); 						//raw data

			UnicodeString sErrorMessage = RVTHLogger->Insert();
			if (sErrorMessage != "")
			{
				LogMessage(CbFileWrite, !CbDBWrite, "VTH DB저장 오류: " + sErrorMessage);
				LogMessage(CbFileWrite, !CbDBWrite, RVTHLogger->SqlText);
			} // if

			delete RPoint;    	// Resource release
			delete RVTHLogger;	// Resource release
*/
		} else if ((sMsg.UpperCase()).Pos("POWER OFF") > 0) {

			//LogMessage(CbFileWrite, !CbDBWrite, "VTH 수신: " + sMsg.Trim());

		} else if ((sMsg.UpperCase()).Pos("POWER ON") > 0) {

			//LogMessage(CbFileWrite, !CbDBWrite, "VTH 수신: " + sMsg.Trim());

		} else {

			//LogMessage(CbFileWrite, !CbDBWrite, "VTH 수신: " + sMsg.Trim());

		}
	}
}

//---------------------------------------------------------------------------
void TfrmMain::OpenVTH()
{
	try
	{
		if (ApdComPort->Open) ApdComPort->Open = false;
		if ( DBEnvInfo.iComNumber > 0 ) {
			ApdComPort->ComNumber = DBEnvInfo.iComNumber;
			ApdComPort->Baud = DBEnvInfo.iBaud;
			LogMessage(CbFileWrite, !CbDBWrite, "DB VTH COM port=" + IntToStr(ApdComPort->ComNumber));
			LogMessage(CbFileWrite, !CbDBWrite, "DB VTH baud rate=" + IntToStr(ApdComPort->Baud));
			ApdComPort->Open = true;
		} else {
			LogMessage(CbFileWrite, !CbDBWrite, "VTH Comport is not set... VTH Port is not open...");
		}
	} // try
	catch(Exception &e)
	{
		LogMessage(CbFileWrite, !CbDBWrite, "VTH Logger 에러: " + e.Message);
	} // catch
}

//---------------------------------------------------------------------------
void TfrmMain::GetDBEnvInfo()
//필요한 DB관련 정보를 읽기
{
	// heaven2 2014.07.24 제원 정보창에 보여주기 위해서 사용.
	UnicodeString sPointCode, sPointName;
	UnicodeString sWater, sWaterPort, sWaterHz, sWaterSN;
	UnicodeString sHydro1, sHydroPort1, sHydroHz1, sHydroSN1;
	UnicodeString sHydro2, sHydroPort2, sHydroHz2, sHydroSN2;
	UnicodeString sHydro3, sHydroPort3, sHydroHz3, sHydroSN3;
	UnicodeString sVTHPort;
	UnicodeString sInterval, sWorkTime, sStartTime;

	LogMessage(CbFileWrite, !CbDBWrite, "DB에서 설정 읽어옴");

	TfbTable *RSetup = new TfbTable(SQLConnection, "RSETUP");
    //DataSender 정보: 수집서버
    UnicodeString sErrorMessage;
    //지점정보
	RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "1");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnvInfo.sPointCode    = RSetup->ValueByName("Value01"); //지점코드
        DBEnvInfo.sPointName    = RSetup->ValueByName("Value02"); //지점명
        DBEnvInfo.sPointAddress = RSetup->ValueByName("Value03"); //지점 주소
		DBEnvInfo.sPointZipCode = RSetup->ValueByName("Value04"); //지점 우편번호
        DBEnvInfo.sPointManager = RSetup->ValueByName("Value05"); //지점 담당자
        DBEnvInfo.sPointTel     = RSetup->ValueByName("Value06"); //담당자 연락처
        DBEnvInfo.sPointEtc     = RSetup->ValueByName("Value07"); //특이사항
    } // if
	else
	{
		LogMessage(CbFileWrite, !CbDBWrite, "지점정보 읽어오는중 DB에러=" + sErrorMessage);
		LogMessage(CbFileWrite, !CbDBWrite, RSetup->SqlText);
	} // else

    //공통사항
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "2");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnvInfo.sPubRadsHome    = RSetup->ValueByName("Value01"); //Rads Home 기본폴더
        DBEnvInfo.sPubLoggerHome  = RSetup->ValueByName("Value02"); //logger home
        DBEnvInfo.sPubSenderHome  = RSetup->ValueByName("Value03"); //sender home
        DBEnvInfo.sPubViewerHome  = RSetup->ValueByName("Value04"); //viewer home
        DBEnvInfo.iDataLoggerPort = RSetup->ValueByName("Value09").ToIntDef(5021);
		DBEnvInfo.iDataViewerPort = RSetup->ValueByName("Value10").ToIntDef(5011);
        DBEnvInfo.sWatchDogIP     = "127.0.0.1";
        DBEnvInfo.iWatchDogPort   = RSetup->ValueByName("Value11").ToIntDef(5031);
    } // if
	else
	{
		LogMessage(CbFileWrite, !CbDBWrite, "공통사항 읽어오는중 DB에러=" + sErrorMessage);
		LogMessage(CbFileWrite, !CbDBWrite, RSetup->SqlText);
	} // else

	//Logger 정보: 수위계 정보
	RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
    RSetup->SetWhereParam("PK2", "4");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		DBEnvInfo.sWaterKind    = RSetup->ValueByName("Value01");	// NONE, LT300, .. RnsWater, ..
		DBEnvInfo.sAirmarEnable = RSetup->ValueByName("Value12");	// ON, OFF ( RnsWater일 때 Airmar를 받는 지 여부)
		DBEnvInfo.fWLOffset     = RSetup->FloatByNameDef("Value04", 0.0f);
		DBEnvInfo.sWaterUseFlag = RSetup->ValueByName("Value13");	// 수위 사요: 유속계/수위계, HydroMeter/WaterLevel
		if ( DBEnvInfo.sWaterUseFlag.IsEmpty() )  DBEnvInfo.sWaterUseFlag = WATERLEVEL_STR;	// (Default) 수위센서를 기본으로 한다.
	} // if

//장비제원 화면에 표시
	//mmSpec->Lines->Clear();
	//mmSpec->Lines->Add("지점코드: " + DBEnvInfo.sPointCode + " " + DBEnvInfo.sPointName); //지점
	sPointCode = DBEnvInfo.sPointCode;
	sPointName = DBEnvInfo.sPointName;

    //유속계1; 하층유속계
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
    RSetup->SetWhereParam("PK2", "2");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		DBEnvInfo.sHydroKind1 = RSetup->ValueByName("Value01");  //NONE/ChannelMaster/AGSeries/SLSerice/RQ30,...
		DBEnvInfo.sAirmarPort = RSetup->ValueByName("Value16");  //AirmarPort : NONE, COM01, COM02,..
		if ( DBEnvInfo.sHydroKind1 == AGSERIES_STR || DBEnvInfo.sHydroKind1 == SLSERIES_STR ) {
			DBEnvInfo.iAG1_WN = RSetup->ValueByName("Value07").ToIntDef(0); //        WN, ->    측정 셀 총개수
			DBEnvInfo.fAG1_WS = RSetup->FloatByNameDef("Value08", 0.0f);    //        WS, ->    측정 셀 크기(cm)
			DBEnvInfo.fAG1_WF = RSetup->FloatByNameDef("Value09", 0.0f);    //        WF, ->    공백거리셀
		}
		DBEnvInfo.iSlaveCount = RSetup->IntByNameDef("Value13",0);      // 2022.12.15    SlaveCount => RQ-30d ( Slave RG-30d Count)
		//mmSpec->Lines->Add("하층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		sHydro1     = RSetup->ValueByName("Value01");
		sHydroPort1 = RSetup->ValueByName("Value02");
		sHydroHz1   = RSetup->ValueByName("Value11");
		sHydroSN1   = RSetup->ValueByName("Value12");
		// 2024.10.02
		if ( DBEnvInfo.sHydroKind1 == RQ30D_STR ) {	// Device => RQ30D
			DBEnvInfo.sHydroMasterAddress = RSetup->ValueByName("Value05");	// Master Port Device Address (ex)5,4,3,2,1
			DBEnvInfo.iHydroMasterCnt     = HowManyDeviceAddress(DBEnvInfo.sHydroMasterAddress);      // Master Port Device Count
			DBEnvInfo.sHydroUseSlave      = RSetup->ValueByName("Value06");       // ON/OFF Slave Port를 사용할지 말 지 Flag
			if ( DBEnvInfo.sHydroUseSlave == ON_STR ) {
				DBEnvInfo.sHydroSlaveAddress  = RSetup->ValueByName("Value09");   // Slave Port Device Address (ex) 6,7,8,9,10
				DBEnvInfo.iHydroSlaveCnt      = HowManyDeviceAddress(DBEnvInfo.sHydroSlaveAddress);       // Slave Port Device Count
			} else {
				DBEnvInfo.sHydroSlaveAddress  = "";   // Slave Port Device Address (ex) 6,7,8,9,10
				DBEnvInfo.iHydroSlaveCnt      = HowManyDeviceAddress(DBEnvInfo.sHydroSlaveAddress);       // Slave Port Device Count
			}

			// AddressOrder[] 배열에 좌안부터의 DeviceAddress를 정의한다.
			SetDeviceAddressOrder(DBEnvInfo.DeviceAddress, DBEnvInfo.sHydroMasterAddress, DBEnvInfo.sHydroSlaveAddress);

		} else if ( DBEnvInfo.sHydroKind1 == RQ30_STR ) {
			DBEnvInfo.sHydroMasterAddress = "1";	   // RQ-30 Address : 1 fixed
			DBEnvInfo.iHydroMasterCnt     = 1;         // Master Port Device Count
			DBEnvInfo.sHydroUseSlave      = "OFF";     // ON/OFF Slave Port를 사용할지 말 지 Flag => "OFF" (fixed)
			DBEnvInfo.sHydroSlaveAddress  = "";        // Slave Port Device Address (ex) 6,7,8,9,10
			DBEnvInfo.iHydroSlaveCnt      = 0;         // Slave Port Device Count

			// AddressOrder[] 배열에 좌안부터의 DeviceAddress를 정의한다.
			SetDeviceAddressOrder(DBEnvInfo.DeviceAddress, DBEnvInfo.sHydroMasterAddress, DBEnvInfo.sHydroSlaveAddress);
		} else {

		}
	} // if

	//유속계2; 상층유속계
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "3");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		DBEnvInfo.sHydroKind2 = RSetup->ValueByName("Value01");  //ChannelMaster/AGSeries/SLSerice/RQ30,...
		DBEnvInfo.iAG2_WN = RSetup->ValueByName("Value07").ToIntDef(0); //        WN, ->    측정 셀 총개수
		DBEnvInfo.fAG2_WS = RSetup->FloatByNameDef("Value08", 0.0f);    //        WS, ->    측정 셀 크기(cm)
		DBEnvInfo.fAG2_WF = RSetup->FloatByNameDef("Value09", 0.0f);    //        WF, ->    공백거리셀
		//mmSpec->Lines->Add("상층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		sHydro2     = RSetup->ValueByName("Value01");
		sHydroPort2 = RSetup->ValueByName("Value02");
		sHydroHz2   = RSetup->ValueByName("Value12");
		sHydroSN2   = RSetup->ValueByName("Value13");
	} // if

	//유속계3; 3번유속계
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "5");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		DBEnvInfo.sHydroKind3 = RSetup->ValueByName("Value01");  //ChannelMaster/AGSeries/SLSerice/RQ30,...
		DBEnvInfo.iAG3_WN = RSetup->ValueByName("Value07").ToIntDef(0); //        WN, ->    측정 셀 총개수
		DBEnvInfo.fAG3_WS = RSetup->FloatByNameDef("Value08", 0.0f);    //        WS, ->    측정 셀 크기(cm)
		DBEnvInfo.fAG3_WF = RSetup->FloatByNameDef("Value09", 0.0f);    //        WF, ->    공백거리셀
		//mmSpec->Lines->Add("상층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		sHydro3     = RSetup->ValueByName("Value01");
		sHydroPort3 = RSetup->ValueByName("Value02");
		sHydroHz3   = RSetup->ValueByName("Value12");
		sHydroSN3   = RSetup->ValueByName("Value13");
	} // if

	//수위계
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "4");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		//mmSpec->Lines->Add("수위계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		sWater     = RSetup->ValueByName("Value01");
		sWaterPort = RSetup->ValueByName("Value02");
		sWaterHz   = RSetup->ValueByName("Value05");
		sWaterSN   = RSetup->ValueByName("Value06");
	} // if
    //Logger 정보: 동작 시간 설정
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
    RSetup->SetWhereParam("PK2", "1");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.sLGInterval      = RSetup->ValueByName("Value01"); //측정주기(nn:ss)
        DBEnvInfo.sLGWorkingPeriod = RSetup->ValueByName("Value02"); //WorkingPeriod: 동작주기(08:30초 동안 동작하여 데이터 수집)
		DBEnvInfo.iLGStartTime     = RSetup->ValueByName("Value03").ToIntDef(5); //시작시간(매5,15,25,35,45,55분에 시작하여 10분동안 자료 수집함
		sInterval  = RSetup->ValueByName("Value01");
		sWorkTime  = RSetup->ValueByName("Value02");
		sStartTime = RSetup->ValueByName("Value03");
	} // if
	//DataSender 정보: 수집서버
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "1");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnvInfo.bServerEnable   = RSetup->ValueByName("Value01").UpperCase().Trim() == "ON"; //수집서버 전송 Flag
        DBEnvInfo.sServerDomain   = RSetup->ValueByName("Value02").Trim(); //수집서버 Domain/IP
        DBEnvInfo.iServerPort     = RSetup->ValueByName("Value03").ToIntDef(0); //수집서버 Port
        DBEnvInfo.sServerAccount  = RSetup->ValueByName("Value04").Trim(); //서버 id
        DBEnvInfo.sServerPassword = RSetup->ValueByName("Value05").Trim(); //서버 pw
		DBEnvInfo.sServerRealPath = RSetup->ValueByName("Value06").Trim(); //서버 실시간 저장폴더
        DBEnvInfo.sServerPastPath = RSetup->ValueByName("Value07").Trim(); //서버 과거 저장폴더
        DBEnvInfo.bRnseaEnable    = RSetup->ValueByName("Value08").UpperCase().Trim() == "ON"; //rnsea ftp 서버 전송 유무
        DBEnvInfo.sRnseaDomain    = RSetup->ValueByName("Value09").Trim(); //rnsea ftp 서버 도메인/ip
        DBEnvInfo.iRnseaPort      = RSetup->ValueByName("Value10").ToIntDef(0); //rnsea ftp 서버 Port
        DBEnvInfo.sRnseaAccount   = RSetup->ValueByName("Value11").Trim(); //rnsea ftp 서버 id
        DBEnvInfo.sRnseaPassword  = RSetup->ValueByName("Value12").Trim(); //rnsea ftp 서버 pw
		DBEnvInfo.sRnseaRealPath  = RSetup->ValueByName("Value13").Trim(); //rnsea ftp 서버 실시간 저장폴더
        DBEnvInfo.sRnseaPastPath  = RSetup->ValueByName("Value14").Trim(); //rnsea ftp 서버 과거 저장폴더
		DBEnvInfo.sFtpFileFormat  = RSetup->ValueByName("Value15").Trim(); //ftp file format(ADVM/UVM/UVM(Hydrovision))
		//heaven2
		DBEnvInfo.sFTPMode        = RSetup->ValueByName("Value16").Trim(); // Active / Passive
		DBEnvInfo.sRnseaFTPMode   = RSetup->ValueByName("Value17").Trim(); // Active / Passive
		if (DBEnvInfo.sFTPMode.IsEmpty())      DBEnvInfo.sFTPMode = "Active";   // Active/Passive
		if (DBEnvInfo.sRnseaFTPMode.IsEmpty()) DBEnvInfo.sFTPMode = "Active";	// Active/Passive
		//heaven2
		UnicodeString FTPStr;
		FTPStr = RSetup->ValueByName("Value18").Trim();	// Server 1
		if ( FTPStr == "FTP") {					// FTPType => FTP
			DBEnvInfo.sFTPType        = "FTP";
			DBEnvInfo.sPrivateKey     = "";
		} else if ( FTPStr.Pos("SFTP") == 1 ) {	// FTPType => SFTP (ex) SFTP|C:\Program Files\RADS\privatekey.ras
			DBEnvInfo.sFTPType        = "SFTP";
			DBEnvInfo.sPrivateKey     = (FTPStr.Length() > 4) ? FTPStr.SubString(6, FTPStr.Length()-5) : UnicodeString("");
		} else {
			DBEnvInfo.sFTPType        = "FTP";
			DBEnvInfo.sPrivateKey     = "";
		}
		FTPStr = RSetup->ValueByName("Value19").Trim();	// Server 2
		if ( FTPStr == "FTP") {					// FTPType => FTP
			DBEnvInfo.sFTPType2       = "FTP";
			DBEnvInfo.sPrivateKey2    = "";
		} else if ( FTPStr.Pos("SFTP")== 1 ) {	// FTPType => SFTP (ex) SFTP|C:\Program Files\RADS\privatekey.ras
			DBEnvInfo.sFTPType2       = "SFTP";
			DBEnvInfo.sPrivateKey2    = (FTPStr.Length() > 4) ? FTPStr.SubString(6, FTPStr.Length()-5) : UnicodeString("");
		} else {
			DBEnvInfo.sFTPType2       = "FTP";
			DBEnvInfo.sPrivateKey2    = "";
		}
		//DBEnvInfo.sFTPType        = RSetup->ValueByName("Value18").Trim(); // FTP/SFTP
		//DBEnvInfo.sPrivateKey     = RSetup->ValueByName("Value19").Trim(); // FTP/SFTP
	} // if
	else
	{
		LogMessage(CbFileWrite, !CbDBWrite, "수집서버 정보 읽어오는중 DB에러=" + sErrorMessage);
		LogMessage(CbFileWrite, !CbDBWrite, RSetup->SqlText);
	} // else
    //DataSender 정보: 동작 변수, VTHLogger 정보
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "2");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.iRetryCount       = RSetup->ValueByName("Value01").ToIntDef(3); //FTP 전송 실패 시 재시도 횟수
        DBEnvInfo.bRebootFlag       = RSetup->ValueByName("Value02").UpperCase().Trim() == "ON"; //FTP 실패시 Gateway/PC Reboot할 것인 지에 대한 옵션
        DBEnvInfo.iMaxSendFile      = RSetup->ValueByName("Value03").ToIntDef(10); //최대 업로드 개수
		DBEnvInfo.iRebootRetryCount = RSetup->ValueByName("Value04").ToIntDef(3); //FTP 전송 연속으로 몇 번 실패할 때 Gateway Reboot/PC Reboot할 것인 지 횟수
		DBEnvInfo.iComNumber        = GetDecimalStr(RSetup->ValueByName("Value05")).ToIntDef(-1);
		DBEnvInfo.iBaud             = RSetup->ValueByName("Value06").ToIntDef(9600);
		DBEnvInfo.sACOpt            = RSetup->ValueByName("Value07");	// BATTERY / AIN
		//mmSpec->Lines->Add("VTH Logger: " + RSetup->ValueByName("Value05"));
		sVTHPort = RSetup->ValueByName("Value05");
	} // if
    //DataSender 정보: GuardNet 동작 변수
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "3");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.bGNEnable                  = RSetup->ValueByName("Value01").UpperCase() == "ON"; //가드넷 사용유무
        DBEnvInfo.cGNType                    = RSetup->ValueByName("Value02").UpperCase() == "NORMAL" ? 0x0 : 0x1; //0x0=normal(특정시간에 pc부팅), 0x1=네트웍 상황에 따라 pc부팅
        DBEnvInfo.sGNRebootTime              = RSetup->ValueByName("Value03"); //normal일때 pc부팅시간(hh:nn:ss)
        DBEnvInfo.iGNPingInterval            = RSetup->ValueByName("Value04").ToIntDef(60); //핑 주기(초)
        DBEnvInfo.sGNPingStartTime           = RSetup->ValueByName("Value05"); //ping을 시작할 시간(=current면 프로그램 시간한 시간부터/0: 매분(0초 마다))
        DBEnvInfo.sGNPingServer1             = RSetup->ValueByName("Value06"); //ping server 1
        DBEnvInfo.iGNPingServer1Port         = RSetup->ValueByName("Value07").ToIntDef(80); //ping server 1 port
        DBEnvInfo.bGNPingServer2TestOn       = RSetup->ValueByName("Value08").UpperCase() == "ON" ? true : false; //핑2 테스트 유무
        DBEnvInfo.sGNPingServer2             = RSetup->ValueByName("Value09"); //ping server 2
        DBEnvInfo.iGNPingServer2Port         = RSetup->ValueByName("Value10").ToIntDef(80); //ping server 2 port
        DBEnvInfo.iGNRetryCount              = RSetup->ValueByName("Value11").ToIntDef(5); //ping실패시 재시도 횟수(1~5)
        DBEnvInfo.iGNPingRetryCount          = RSetup->ValueByName("Value12").ToIntDef(2); //1분당 시도 시 재시도 횟수(연속실패횟수)
        DBEnvInfo.iGNWirelessUseMinute       = RSetup->ValueByName("Value13").ToIntDef(30); //무선 사용 시간; 무선을 전환시 최대운영 시간(Expire되면 유선으로 다시 전환하여 다시 시도)
        DBEnvInfo.iGNGatewayRebootMaxCount   = RSetup->ValueByName("Value14").ToIntDef(5); //gw reset max
		DBEnvInfo.iGNRebootMaxCount          = RSetup->ValueByName("Value15").ToIntDef(10); //1일 PC 최대 Reboot 할 수 있는 횟수(1,2,3,4,5,6,7,8,9,10)
		// heaven2 2026.03.02
		//DBEnvInfo.iGNNetworkWiredIF        = RSetup->ValueByName("Value16").ToIntDef(13); //NetworkWiredIF : Network Interface No
		//DBEnvInfo.iGNNetworkWirelessIF     = RSetup->ValueByName("Value17").ToIntDef(14); //NetworkWirelessIF : Network Interface No
		DBEnvInfo.sGNNetworkWiredIFDesc      = RSetup->ValueByName("Value16"); //NetworkWiredIF : Network Interface Description
		DBEnvInfo.sGNNetworkWirelessIFDesc   = RSetup->ValueByName("Value17"); //NetworkWirelessIF : Network Interface Description
		DBEnvInfo.iGNDelayMinuteAfterGWReset = RSetup->ValueByName("Value18").ToIntDef(1); //g/w reset후 대기시간 분
	} // if

	//mmSpec->Lines->Add("측정주기/동작주기/시작시간= " + GetSepItem(DBEnvInfo.sLGInterval, 0, ":") + ":" + GetSepItem(DBEnvInfo.sLGInterval, 1, ":") +
	//                   "/" + GetSepItem(DBEnvInfo.sLGWorkingPeriod, 0, ":") + ":" + GetSepItem(DBEnvInfo.sLGWorkingPeriod, 1, ":") +
	//                   "/" + IntToStr(DBEnvInfo.iLGStartTime));
	//mmSpec->SelStart = 0;
	//mmSpec->SelLength = 0;

//유효범위 가져오기
    //vth logger
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "10");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.iAC_Min = RSetup->ValueByName("VALUE01").ToInt();
        DBEnvInfo.iAC_Max = RSetup->ValueByName("VALUE02").ToInt();

        DBEnvInfo.fDC_Charge_Min = RSetup->FloatByNameDef("VALUE03", 0.0f);
        DBEnvInfo.fDC_Charge_Max = RSetup->FloatByNameDef("VALUE04", 0.0f);

        DBEnvInfo.fDC_Battery_Min = RSetup->FloatByNameDef("VALUE05", 0.0f);
        DBEnvInfo.fDC_Battery_Max = RSetup->FloatByNameDef("VALUE06", 0.0f);

        DBEnvInfo.fTemp_Sys_Min = RSetup->FloatByNameDef("VALUE07", 0.0f);
        DBEnvInfo.fTemp_Sys_Max = RSetup->FloatByNameDef("VALUE08", 0.0f);

        DBEnvInfo.fHr_Sys_Min = RSetup->FloatByNameDef("VALUE09", 0.0f);
        DBEnvInfo.fHr_Sys_Max = RSetup->FloatByNameDef("VALUE10", 0.0f);
	} // if

    //수위계
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "11");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.fWaterLevel_Min = RSetup->FloatByNameDef("VALUE01", 0.0f);
        DBEnvInfo.fWaterLevel_Max = RSetup->FloatByNameDef("VALUE02", 0.0f);

        DBEnvInfo.fSalinity_Min = RSetup->FloatByNameDef("VALUE03", 0.0f);
        DBEnvInfo.fSalinity_Max = RSetup->FloatByNameDef("VALUE04", 0.0f);
	} // if

    //유속계
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "12");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
        DBEnvInfo.fTemp_Water_Min = RSetup->FloatByNameDef("VALUE01", 0.0f);
        DBEnvInfo.fTemp_Water_Max = RSetup->FloatByNameDef("VALUE02", 0.0f);

    //    ???수압 = SimpleDataSet->FieldByName("VALUE03")->AsString;
    //    ???수압 = SimpleDataSet->FieldByName("VALUE04")->AsString;
    //todo: adv파일 만들때 수압은 사용안함

        DBEnvInfo.fPitch_Min = RSetup->FloatByNameDef("VALUE05", 0.0f);
        DBEnvInfo.fPitch_Max = RSetup->FloatByNameDef("VALUE06", 0.0f);

		DBEnvInfo.fRoll_Min = RSetup->FloatByNameDef("VALUE07", 0.0f);
        DBEnvInfo.fRoll_Max = RSetup->FloatByNameDef("VALUE08", 0.0f);

		//유속ew
		DBEnvInfo.fV_EW_Min = RSetup->FloatByNameDef("VALUE09", 0.0f);
		DBEnvInfo.fV_EW_Max = RSetup->FloatByNameDef("VALUE10", 0.0f);
		//유속ns
		DBEnvInfo.fV_NS_Min = RSetup->FloatByNameDef("VALUE11", 0.0f);
		DBEnvInfo.fV_NS_Max = RSetup->FloatByNameDef("VALUE12", 0.0f);
	} // if

	delete RSetup;

	//----------------------------------------------------------------------
	// Show System Info to List View
	//----------------------------------------------------------------------
	TListItem *pItem = NULL;
	TStrings  *p;

	pItem = ListView->Items->Item[0];
	pItem->Caption = "지점코드";
	p = pItem->SubItems;
	p->Strings[0] = sPointCode;
	p->Strings[1] = sPointName;
	p->Strings[2] = "";
	p->Strings[3] = "";

	pItem = ListView->Items->Item[1];
	pItem->Caption = "수위계";
	p = pItem->SubItems;
	p->Strings[0] = sWater;
	if ( sWater == LT400_STR )	p->Strings[0] = sWater + "+Atmos";
	p->Strings[1] = sWaterPort;
	p->Strings[2] = sWaterHz;
	p->Strings[3] = sWaterSN;

	pItem = ListView->Items->Item[2];
	pItem->Caption = "1번유속계";
	p = pItem->SubItems;
	p->Strings[0] = sHydro1;
	p->Strings[1] = sHydroPort1;
	p->Strings[2] = sHydroHz1;
	p->Strings[3] = sHydroSN1;

	pItem = ListView->Items->Item[3];
	pItem->Caption = "2번유속계";
	p = pItem->SubItems;
	p->Strings[0] = sHydro2;
	p->Strings[1] = sHydroPort2;
	p->Strings[2] = sHydroHz2;
	p->Strings[3] = sHydroSN2;

	pItem = ListView->Items->Item[4];
	pItem->Caption = "3번유속계";
	p = pItem->SubItems;
	p->Strings[0] = sHydro3;
	p->Strings[1] = sHydroPort3;
	p->Strings[2] = sHydroHz3;
	p->Strings[3] = sHydroSN3;

	pItem = ListView->Items->Item[5];
	pItem->Caption = "VTHLogger";
	p = pItem->SubItems;
	p->Strings[0] = "";
	p->Strings[1] = sVTHPort;
	p->Strings[2] = "";
	p->Strings[3] = "";

	pItem = ListView->Items->Item[6];
	pItem->Caption = "측정/동작/시작";
	p = pItem->SubItems;
	p->Strings[0] = sInterval;
	p->Strings[1] = sWorkTime;
	p->Strings[2] = sStartTime;
	p->Strings[3] = "";
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrCheckDBForFTPTimer(TObject *Sender)
//매 2분마다 FTP에 전송할 파일이 DB에 있는지 검사하여 있으면 파일생성 루틴호출
{
// 일단 매 2분마다 무조건 File Check 하는 것으로
//	USHORT hour, min, sec, msec;
//	Now().DecodeTime(&hour, &min, &sec, &msec);
//	if ( min % 10 == 2 || min % 10 == 3 || min % 10 == 4 ) {  // 매 2,3,4분은 Background Processing이 돌지 않도록 한다. (동시성 제어 때문..)
//		LogMessage(CbFileWrite, !CbDBWrite, "SKIP DB Check Timer => " + IntToStr(min) + " minute Event");
//		return;
//	}

	SearchDBMakeFtpFiles(); //DB를 쿼리하여 ftp파일이 생성되지않은 자료가 있으면 생성

	if (DBEnvInfo.bServerEnable ||
		DBEnvInfo.bRnseaEnable) SendFtpFiles(false); //DB에서 ftp로 전송되지않은 자료 쿼리를 쿼리하여 ftp output큐에 전송
}

//---------------------------------------------------------------------------
void TfrmMain::SearchDBMakeFtpFiles(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
//DB를 쿼리하여 ftp파일이 생성되지않은 자료가 있으면 생성
{
	static int iDBErrorCnt = 0;	// DB Query Error Count ( in this Function )
	UnicodeString sError = "";

	//adv 파일 생성
	//파일로 생성되지 않은 자료 쿼리
	SearchDBRPoint->ClearParams();
	if (sMeasureDate != "")
	{
		SearchDBRPoint->SetWhereParam("MEASUREDATE", sMeasureDate);
		SearchDBRPoint->SetWhereParam("MEASURETIME", sMeasureTime);
	} // if
	SearchDBRPoint->SetWhereParam("FILEFLAG", "N");
	SearchDBRPoint->SetOrderParam("MEASUREDATE", okAsc);
	SearchDBRPoint->SetOrderParam("MEASURETIME", okAsc);
	sError = SearchDBRPoint->Open();

	if ( !sError.IsEmpty() ) {	// heaven2 => Open()에서 Error String 이 넘겨오면
		iDBErrorCnt++;
		LogMessage(CbFileWrite, !CbDBWrite, "DB UPDATE 에러: " + sError);
		if ( iDBErrorCnt >= 6 ) {	// 10분에 3번 * 2 = 6번 (20분 이상 DB Query Error가 발생한다면)
			// WatchDog에 Restart를 요청한다.
			BYTE Action = ACTION_SHUTDOWN_REQUEST;
			SendWatchDogMessage(Action);
			iDBErrorCnt = 0;	// 혹시 WatchDog이 실행중이 아니면 계속 요청하기 때문에 대응 코드
			return;
		}
	}

	int iRowCount = 0;
	int iRecordCount = SearchDBRPoint->DataSet()->RecordCount;
	while(!SearchDBRPoint->DataSet()->Eof)
	{
		//현재시간보다 미래는 처리하지않고 스킵
		UnicodeString sRecordDate = SearchDBRPoint->ValueByName("MEASUREDATE");
		UnicodeString sRecordTime = SearchDBRPoint->ValueByName("MEASURETIME");
		int iYear   = sRecordDate.SubString(1, 4).ToIntDef(1901),
			iMonth  = sRecordDate.SubString(5, 2).ToIntDef(1),
			iDay    = sRecordDate.SubString(7, 2).ToIntDef(1);
		int iHour   = sRecordTime.SubString(1, 2).ToIntDef(0),
			iMinute = sRecordTime.SubString(3, 2).ToIntDef(0),
			iSec    = sRecordTime.SubString(5, 2).ToIntDef(0);
		TDateTime dtRecord = EncodeDate(iYear, iMonth, iDay) +
							 EncodeTime(iHour, iMinute, iSec, 0);
		if (dtRecord > Now())
		{
			SearchDBRPoint->DataSet()->Next();
			continue;
		} // if

		//BG에선 마지막 1건(real)을 제외
		if (sMeasureDate == "" && iRowCount == (iRecordCount - 1))
		{
			SearchDBRPoint->DataSet()->Next();
			continue;
		} // if

		bool bSuccess = DoMakeDB2Adv(SearchDBRPoint->ValueByName("MEASUREDATE"),
									 SearchDBRPoint->ValueByName("MEASURETIME"));

		//DB업데이트(fileflag = 'Y')
		if (bSuccess)
		{
			SearchDBRPoint->ClearParams();
			SearchDBRPoint->SetFieldParam("FILEFLAG", "Y");
			SearchDBRPoint->SetWhereParam("MEASUREDATE", SearchDBRPoint->ValueByName("MEASUREDATE"));
			SearchDBRPoint->SetWhereParam("MEASURETIME", SearchDBRPoint->ValueByName("MEASURETIME"));
			UnicodeString sError = SearchDBRPoint->Update();
			if (sError != "")
			{
				LogMessage(CbFileWrite, !CbDBWrite, "DB UPDATE 에러: " + sError);
				LogMessage(CbFileWrite, !CbDBWrite, SearchDBRPoint->SqlText);
			} // if
		} // if
		iRowCount++;
		if (iRowCount == DBEnvInfo.iMaxSendFile) break;
		SearchDBRPoint->DataSet()->Next();
	} // while

//	//----------------------------------------------------------------------
//	bWorking = false;	// Working ..
//	//----------------------------------------------------------------------
}

//---------------------------------------------------------------------------
void TfrmMain::SendFtpFiles(bool bReal)
//true면 real폴더로 false면 past폴더로
//DB에서 ftp로 전송되지않은 자료 쿼리를 쿼리하여 ftp output큐에 전송
{
	UnicodeString sFtpFolder  = (bReal) ? DBEnvInfo.sServerRealPath : DBEnvInfo.sServerPastPath;	// SERVER_IRDIMS_FIRST  : 수신서버1의 Path
	UnicodeString sFtpFolder2 = (bReal) ? DBEnvInfo.sRnseaRealPath  : DBEnvInfo.sRnseaPastPath;		// SERVER_IRDIMS_SECOND : 수신서버2의 Path

	// ---------------------------------------------------------------------
	// REAL Data 전송 ( FTP Queue에 Push )
	// ---------------------------------------------------------------------

	if (bReal)
	{
		STFTPMessage *pFTPMessage = new STFTPMessage();

		pFTPMessage->sMeasureDate = GsMeasureDate;
		pFTPMessage->sMeasureTime = GsMeasureTime;
		pFTPMessage->bEsvAdvFlag  = ( IsFileFormat() == FORMAT_EWSV_ADVM)     ? true : false;	// ADVM+EWSV 일 때만 true로 설정
		pFTPMessage->enFileType   = ((IsFileFormat() == FORMAT_ADVM)          ? enAdvFile : \
									((IsFileFormat() == FORMAT_UVM_ACCUSONIC) ? enUvmFile : \
									((IsFileFormat() == FORMAT_EWSV )         ? enEsvFile : \
									((IsFileFormat() == FORMAT_EWSV_ADVM )    ? enEsvFile : enAdvFile))));  // ADVM+EWSV일 때 esv를 먼저 보낸다.

		UnicodeString sFileName = "";
		UnicodeString sFileName2 = "";
		if ( IsFileFormat() == FORMAT_ADVM ) {
			sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
						GsMeasureDate + GsMeasureTime.SubString(1, 4) + //년월일시분
						".adv";
		} else if ( IsFileFormat() == FORMAT_UVM_ACCUSONIC ) {
			sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
						GsMeasureDate + GsMeasureTime.SubString(1, 4) + //년월일시분
						".uvm";
		} else if ( IsFileFormat() == FORMAT_EWSV ) {
			sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
						GsMeasureDate + GsMeasureTime.SubString(1, 4) + //년월일시분
						".esv";
		} else if ( IsFileFormat() == FORMAT_EWSV_ADVM ) {	// EWSV, ADVM 혼용 사이트
			sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
						GsMeasureDate + GsMeasureTime.SubString(1, 4) + //년월일시분
						".esv";
			sFileName2 = DBEnvInfo.sPointCode + "_" + //지점코드
						GsMeasureDate + GsMeasureTime.SubString(1, 4) + //년월일시분
						".adv";
		} else {
			return;
		}
		pFTPMessage->sLocalPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" + //local 폴더/파일명
								DBEnvInfo.sPointCode + "_" + //지점코드
								GsMeasureDate.SubString(1, 6) + "\\" + //년월
								DBEnvInfo.sPointCode + "_" + //지점코드
								GsMeasureDate.SubString(1, 8) + "\\" + //년월일
								sFileName;

		if(!FileExists(pFTPMessage->sLocalPathFile))
		{
			bool bSuccess = DoMakeDB2Adv(pFTPMessage->sMeasureDate, pFTPMessage->sMeasureTime); //파일이 없으면 재생성
			if ( !bSuccess ) {
				LogMessage(CbFileWrite, !CbDBWrite, "file create error");
				return;
			}
		} // if


		pFTPMessage->sFTPRootPathFile  = sFtpFolder  + "/" + sFileName; //ftp연결시 대상 폴더/파일명
		pFTPMessage->sFTPRootPathFile2 = sFtpFolder2 + "/" + sFileName; //ftp연결시 대상 폴더/파일명
		pFTPMessage->bReal = bReal;

		pGFTPSection->Enter();
		pGFTPQueue->Push(pFTPMessage); //FTP에 전송할 자료를 큐에 푸시
		pGFTPSection->Leave();

		//LogMessage(CbFileWrite, !CbDBWrite, UnicodeString("FTP Queue Push=>") + pFTPMessage->sLocalPathFile);

		if ( !thrdFTP->bWorking ) pGFTPEvent->SetEvent();

		// EWSV+ADVM에 추가되는 ADVM File 처리
		if ( IsFileFormat() == FORMAT_EWSV_ADVM ) {

			STFTPMessage *pFTPMessage = new STFTPMessage();

			pFTPMessage->sMeasureDate = GsMeasureDate;
			pFTPMessage->sMeasureTime = GsMeasureTime;
			pFTPMessage->bEsvAdvFlag  = true;			// adv를 보낼 차레다.
			pFTPMessage->enFileType   = enAdvFile;		// ADVM+EWSV에서 esv, adv를 순서대로 보낸다. (이번에는 adv 파일)

			pFTPMessage->sLocalPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" + //local 폴더/파일명
									DBEnvInfo.sPointCode + "_" + //지점코드
									GsMeasureDate.SubString(1, 6) + "\\" + //년월
									DBEnvInfo.sPointCode + "_" + //지점코드
									GsMeasureDate.SubString(1, 8) + "\\" + //년월일
									sFileName2;	// Adv File

			pFTPMessage->sFTPRootPathFile  = sFtpFolder  + "/" + sFileName2; //ftp연결시 대상 폴더/파일명
			pFTPMessage->sFTPRootPathFile2 = sFtpFolder2 + "/" + sFileName2; //ftp연결시 대상 폴더/파일명
			pFTPMessage->bReal = bReal;

			pGFTPSection->Enter();
			pGFTPQueue->Push(pFTPMessage); //FTP에 전송할 자료를 큐에 푸시
			pGFTPSection->Leave();

			if ( !thrdFTP->bWorking ) {	pGFTPEvent->SetEvent();	} else { } // nothing to do..
		} else {
			// nothing to do..
		}

	// ---------------------------------------------------------------------
	// PAST Data 전송 ( FTP Queue에 Push )
	// ---------------------------------------------------------------------

	} else {

		// Queue 검사
		int cnt = 0;

		pGFTPSection->Enter();
		cnt = pGFTPQueue->Count();
		pGFTPSection->Leave();

		if ( cnt >= DBEnvInfo.iMaxSendFile ) {
			// 아직 FTP Queue에 보내야할 파일이 MaxSendFile갯수를 초과하므로 Skip한다.
			return;
		}

		TfbTable *RPoint = new TfbTable(SQLConnection, "RPOINT");
		RPoint->ClearParams();
		RPoint->SetWhereParam("FILEFLAG", "Y");
		RPoint->SetWhereParam("SERVERFLAG", "N");
		RPoint->SetWhereParam("MEASUREDATE", FormatDateTime("yyyymmdd", Now() - 30 * 6), wkBiggerOrEqual); //현재로부터 6개월 이전까지만 쿼리하도록
		RPoint->SetOrderParam("MEASUREDATE", okAsc);
		RPoint->SetOrderParam("MEASURETIME", okAsc);
		RPoint->Open();

		int iCnt = 0;
		STFTPMessage *pFTPMessage = NULL;
		while(!RPoint->DataSet()->Eof)
		{
			//미래시간은 처리하지않음
			UnicodeString sDate = RPoint->ValueByName("MEASUREDATE");
			UnicodeString sTime = RPoint->ValueByName("MEASURETIME");
			TDateTime dtRecord = GetStrToDateTime(sDate, sTime);
			if (dtRecord > Now())
			{
				RPoint->DataSet()->Next();
				continue;
			} // if

			/////////////////////////////////////////////////////////////////

			pFTPMessage = new STFTPMessage();

			pFTPMessage->sMeasureDate = RPoint->ValueByName("MEASUREDATE");
			pFTPMessage->sMeasureTime = RPoint->ValueByName("MEASURETIME");
			pFTPMessage->bEsvAdvFlag  = ( IsFileFormat() == FORMAT_EWSV_ADVM)     ? true : false;	// ADVM+EWSV 일 때만 true로 설정
			pFTPMessage->enFileType   = ((IsFileFormat() == FORMAT_ADVM)          ? enAdvFile : \
										((IsFileFormat() == FORMAT_UVM_ACCUSONIC) ? enUvmFile : \
										((IsFileFormat() == FORMAT_EWSV )         ? enEsvFile : \
										((IsFileFormat() == FORMAT_EWSV_ADVM )    ? enEsvFile : enAdvFile))));  // ADVM+EWSV일 때 esv를 먼저 보낸다.

			UnicodeString sFileName = "";
			UnicodeString sFileName2 = "";
			if ( IsFileFormat() == FORMAT_ADVM ) {
				sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
							pFTPMessage->sMeasureDate + pFTPMessage->sMeasureTime.SubString(1, 4) + //년월일시분
							".adv";
			} else if ( IsFileFormat() == FORMAT_UVM_ACCUSONIC ) {
				sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
							pFTPMessage->sMeasureDate + pFTPMessage->sMeasureTime.SubString(1, 4) + //년월일시분
							".uvm";
			} else if ( IsFileFormat() == FORMAT_EWSV ) {
				sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
							pFTPMessage->sMeasureDate + pFTPMessage->sMeasureTime.SubString(1, 4) + //년월일시분
							".esv";
			} else if ( IsFileFormat() == FORMAT_EWSV_ADVM ) { 	// xxx.esv, xxx.adv 파일 두개 생성
				sFileName = DBEnvInfo.sPointCode + "_" + //지점코드
							pFTPMessage->sMeasureDate + pFTPMessage->sMeasureTime.SubString(1, 4) + //년월일시분
							".esv";
				sFileName2 = DBEnvInfo.sPointCode + "_" + //지점코드
							pFTPMessage->sMeasureDate + pFTPMessage->sMeasureTime.SubString(1, 4) + //년월일시분
							".adv";
			} else {
				return ;
			}

			pFTPMessage->sLocalPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" + //local 폴더/파일명
									DBEnvInfo.sPointCode + "_" + //지점코드
									pFTPMessage->sMeasureDate.SubString(1, 6) + "\\" + //년월
									DBEnvInfo.sPointCode + "_" + //지점코드
									pFTPMessage->sMeasureDate + "\\" + //년월일
									sFileName;

			if(!FileExists(pFTPMessage->sLocalPathFile))
			{
				bool bSuccess = DoMakeDB2Adv(pFTPMessage->sMeasureDate, pFTPMessage->sMeasureTime); //파일이 없으면 재생성
				if ( !bSuccess ) {
					LogMessage(CbFileWrite, !CbDBWrite, "adv(batch) file create error");
					continue;
				}
			} // if

			pFTPMessage->sFTPRootPathFile  = sFtpFolder  + "/" + sFileName; //ftp연결시 대상 폴더/파일명
			pFTPMessage->sFTPRootPathFile2 = sFtpFolder2 + "/" + sFileName; //ftp연결시 대상 폴더/파일명
			pFTPMessage->bReal = bReal;

			if ( IsExistFTPQueue(pFTPMessage) ) {
				LogMessage(CbFileWrite, !CbDBWrite, "FTPQueue에 이미 동일한 파일이 있습니다. Push Queue => Skip함");
				continue;
			}

			pGFTPSection->Enter();
			pGFTPQueue->Push(pFTPMessage);
			pGFTPSection->Leave();

			if ( IsFileFormat() == FORMAT_EWSV_ADVM ) {	// EWSV+ADVM에서 추가되는 ADVM 파일을 처리함

				pFTPMessage = new STFTPMessage();

				pFTPMessage->sMeasureDate = RPoint->ValueByName("MEASUREDATE");
				pFTPMessage->sMeasureTime = RPoint->ValueByName("MEASURETIME");
				pFTPMessage->bEsvAdvFlag  = true;			// adv를 보낼 차레다.
				pFTPMessage->enFileType   = enAdvFile;		// ADVM+EWSV에서 esv, adv를 순서대로 보낸다. (이번에는 adv 파일)

				pFTPMessage->sLocalPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" + //local 폴더/파일명
										DBEnvInfo.sPointCode + "_" + //지점코드
										pFTPMessage->sMeasureDate.SubString(1, 6) + "\\" + //년월
										DBEnvInfo.sPointCode + "_" + //지점코드
										pFTPMessage->sMeasureDate + "\\" + //년월일
										sFileName2;

				pFTPMessage->sFTPRootPathFile  = sFtpFolder  + "/" + sFileName2; //ftp연결시 대상 폴더/파일명
				pFTPMessage->sFTPRootPathFile2 = sFtpFolder2 + "/" + sFileName2; //ftp연결시 대상 폴더/파일명
				pFTPMessage->bReal = bReal;

				if ( IsExistFTPQueue(pFTPMessage) ) {
					LogMessage(CbFileWrite, !CbDBWrite, "FTPQueue에 이미 동일한 파일이 있습니다. Push Queue => Skip함");
					continue;
				}

				pGFTPSection->Enter();
				pGFTPQueue->Push(pFTPMessage);
				pGFTPSection->Leave();
			}

			//LogMessage(CbFileWrite, !CbDBWrite, UnicodeString("FTP Queue Push=>") + pFTPMessage->sLocalPathFile);

			iCnt++;	// Send File Count

			////////////////////////////////////////////////////////////////

			RPoint->DataSet()->Next();

			if (iCnt >= DBEnvInfo.iMaxSendFile) break;
		} // while

		if ( !thrdFTP->bWorking ) pGFTPEvent->SetEvent();

		delete RPoint;

	} // else

	// Gargabe Collection
	MemoryGarbageCollection();
	LogMessage(CbFileWrite, !CbDBWrite, "Garbage Collection..");
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrGWResetDelayTimer(TObject *Sender)
//vth에서 power off신호 수신이후 대기시간 후 실행되어 다시 상태를 (true->false) 변경
{
	tmrGWResetDelay->Enabled = false;
    GbGWPowerOff = false;
	LogMessage(CbFileWrite, !CbDBWrite, "G/W 리셋 대기시간 종료(FtpThread, GuardnetThread는 재활성화 됩니다.", (GiGWResetFrom <= 1) ? true : false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoggerServerConnect(TIdContext *AContext)
{
	//SetBitmapFromImageList(imgLoggerStatus, asLoggerEnabled); //logger 상태표시
	ChangeLoggerIcon(asLoggerEnabled);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoggerServerDisconnect(TIdContext *AContext)
{
	ChangeLoggerIcon(asLoggerDisabled);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::WatchDogClientConnected(TObject *Sender)
{
	ChangeWatchDogIcon(asWdEnabled);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::WatchDogClientDisconnected(TObject *Sender)
{
	ChangeWatchDogIcon(asWdDisabled);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ViewerServerDisconnect(TIdContext *AContext)
{
	int idx = lstViewer->IndexOf(AContext);
	if (idx >= 0) lstViewer->Delete(idx);

	int iCount = lstViewer->Count;
	if (iCount > 0 && iCount <= 5) //SetBitmapFromImageList(imgViewerStatus, asViewEnabled + iCount * 2); //viewer 상태표시
		ChangeViewerIcon(asViewEnabled + iCount * 2);
	else if (iCount == 0) //SetBitmapFromImageList(imgViewerStatus, asViewDisabled);
		ChangeViewerIcon(asViewDisabled);
}

//---------------------------------------------------------------------------
bool TfrmMain::DoMakeDB2Adv(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
{
	bool bResult = true;
	UnicodeString sFullPathFile = "";
	UnicodeString sFullPathFile2 = "";

	if ( DBEnvInfo.sFtpFileFormat == FORMAT_ADVM_STR ) {   	// ADVM Format
		sFullPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" +
								   DBEnvInfo.sPointCode + "_" + sMeasureDate.SubString(1, 6) + "\\" + //지점코드_yyyymm
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + "\\" + //지점코드_yyyymmdd
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + sMeasureTime.SubString(1, 4) + //지점코드_yyyymmddhhnn
								   ".adv";
	} else if ( DBEnvInfo.sFtpFileFormat == FORMAT_UVM_ACCUSONIC_STR ) {  	// UVM Format
		sFullPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" +
								   DBEnvInfo.sPointCode + "_" + sMeasureDate.SubString(1, 6) + "\\" + //지점코드_yyyymm
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + "\\" + //지점코드_yyyymmdd
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + sMeasureTime.SubString(1, 4) + //지점코드_yyyymmddhhnn
								   ".uvm";
	} else if ( DBEnvInfo.sFtpFileFormat == FORMAT_EWSV_STR ) {  	// EWSV Format
		sFullPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" +
								   DBEnvInfo.sPointCode + "_" + sMeasureDate.SubString(1, 6) + "\\" + //지점코드_yyyymm
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + "\\" + //지점코드_yyyymmdd
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + sMeasureTime.SubString(1, 4) + //지점코드_yyyymmddhhnn
								   ".esv";
	} else if ( DBEnvInfo.sFtpFileFormat == FORMAT_EWSV_ADVM_STR ) {  	// EWSV+ADVM Format
		sFullPathFile = PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" +
								   DBEnvInfo.sPointCode + "_" + sMeasureDate.SubString(1, 6) + "\\" + //지점코드_yyyymm
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + "\\" + //지점코드_yyyymmdd
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + sMeasureTime.SubString(1, 4) + //지점코드_yyyymmddhhnn
								   ".esv";
		// 두번째 파일
		sFullPathFile2= PathStrAdd(DBEnvInfo.sPubSenderHome, CsDataPath) + "\\" +
								   DBEnvInfo.sPointCode + "_" + sMeasureDate.SubString(1, 6) + "\\" + //지점코드_yyyymm
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + "\\" + //지점코드_yyyymmdd
								   DBEnvInfo.sPointCode + "_" + sMeasureDate + sMeasureTime.SubString(1, 4) + //지점코드_yyyymmddhhnn
								   ".adv";
	} else {	// FORMAT_UVM_HYDROVISION_STR => not support
		// return false;
	}

    Winapi::Windows::THandle hFile;

    try
    {
		//폴더생성
		UnicodeString usFileText = "";
		ForceDirectories(ExtractFilePath(sFullPathFile));
		if (FileExists(sFullPathFile)) DeleteFile(sFullPathFile);
		hFile = FileCreate(sFullPathFile, fmOpenWrite | fmShareDenyNone);

		if ( IsFileFormat() == FORMAT_ADVM ) {		// ADVM Text를 만든다.
			AnsiString sAdvHeaderText = CsAdvHeaderText;
			FileWrite(hFile, sAdvHeaderText.c_str(), sAdvHeaderText.Length());
			usFileText = GetAdvTextFromDB(sMeasureDate, sMeasureTime);

		} else if ( IsFileFormat() == FORMAT_UVM_ACCUSONIC ) {
			AnsiString sUvmHeaderText = CsUvmHeaderText;
			FileWrite(hFile, sUvmHeaderText.c_str(), sUvmHeaderText.Length());
			usFileText = GetAdvTextFromDB(sMeasureDate, sMeasureTime);

		} else if ( IsFileFormat() == FORMAT_EWSV ) {	// EWSV Text를 만든다.
			AnsiString sEsvHeaderText = CsEsvHeaderText;
			FileWrite(hFile, sEsvHeaderText.c_str(), sEsvHeaderText.Length());
			usFileText = GetEsvTextFromDB(sMeasureDate, sMeasureTime);

		} else if ( IsFileFormat() == FORMAT_EWSV_ADVM ) {	// EWSV+ADVM => 일단 EWSV Text를 만든다.
			AnsiString sEsvHeaderText = CsEsvHeaderText;
			FileWrite(hFile, sEsvHeaderText.c_str(), sEsvHeaderText.Length());
			usFileText = GetEsvTextFromDB(sMeasureDate, sMeasureTime);

		} else {}

		//UnicodeString usFileText = GetAdvTextFromDB(sMeasureDate, sMeasureTime);
		AnsiString sFileText = usFileText;
		FileWrite(hFile, sFileText.c_str(), sFileText.Length());
		if (sFileText == "") bResult = false;
		LogMessage(CbFileWrite, !CbDBWrite, ExtractFileName(sFullPathFile) + " 생성됨");

		if (hFile != -1) FileClose(hFile);

		if ( IsFileFormat() == FORMAT_EWSV_ADVM ) {   	// EWSV+ADVM일 때 ADV 파일을 만드는 추가 로직
			//폴더생성
			UnicodeString usFileText2 = "";
			ForceDirectories(ExtractFilePath(sFullPathFile2));
			if (FileExists(sFullPathFile2)) DeleteFile(sFullPathFile2);
			hFile = FileCreate(sFullPathFile2, fmOpenWrite | fmShareDenyNone);

			AnsiString sAdvHeaderText = CsAdvHeaderText;
			FileWrite(hFile, sAdvHeaderText.c_str(), sAdvHeaderText.Length());

			usFileText = GetAdvTextFromDB2(sMeasureDate, sMeasureTime);	// HydroMeter2/3으로 adv file을 만든다.
			AnsiString sFileText = usFileText;
			FileWrite(hFile, sFileText.c_str(), sFileText.Length());
			if (sFileText == "") bResult = false;
			LogMessage(CbFileWrite, !CbDBWrite, ExtractFileName(sFullPathFile2) + " 생성됨");

			if (hFile != -1) FileClose(hFile);
		}
	} // try
    catch(Exception &e)
	{
		// heaven2
		UnicodeString FileErrorStr = ( IsFileFormat() == FORMAT_ADVM) ?          "ADV File Error:" :
									 ((IsFileFormat() == FORMAT_UVM_ACCUSONIC) ? "UVM File Error:" :
									 ((IsFileFormat() == FORMAT_EWSV ) ?         "ESV File Error:" :
									 ((IsFileFormat() == FORMAT_EWSV_ADVM ) ?    "ESV,ADV File Error:" : "Data File Error")));
		LogMessage(CbFileWrite, !CbDBWrite, FileErrorStr + e.Message);
		//LogMessage(CbFileWrite, !CbDBWrite, "ADV 파일 에러: " + e.Message);
		bResult = false;
	} // catch


	return bResult;
}

//---------------------------------------------------------------------------
UnicodeString TfrmMain::GetAdvTextFromDB(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
//db의 값을 읽어와서 adv output text를 리턴
//판단=(0=정상/1=오측/2=결측)
{
    UnicodeString sResult = "";
    AnsiString sFormat;

    //vth
	TRVTHLogger *RVTHLogger = new TRVTHLogger(SQLConnection);
	RVTHLogger->ClearParams();
	RVTHLogger->SetWhereParam("MEASUREDATE", sMeasureDate);
	RVTHLogger->SetWhereParam("MEASURETIME", sMeasureTime);
	RVTHLogger->Open();
	AnsiString sVTH;
	char cDecisionVTH = RVTHLogger->GetAdvText(sVTH); //Deci_All의 vth 결과값
	delete RVTHLogger;

	//수위계
	AnsiString sWL = "";
	char cDecisionWL = 0x00;
	if ( DBEnvInfo.sWaterUseFlag == WATERLEVEL_STR ) {
		TRWaterLevel *RWaterLevel = new TRWaterLevel(SQLConnection, DBEnvInfo.fWLOffset);
		RWaterLevel->SetMeasureDateTime(sMeasureDate, sMeasureTime);	// heaven2 2021.12.10
		RWaterLevel->ClearParams();
		RWaterLevel->SetWhereParam("MEASUREDATE", sMeasureDate);
		RWaterLevel->SetWhereParam("MEASURETIME", sMeasureTime);
		RWaterLevel->Open();
		cDecisionWL = RWaterLevel->GetAdvText(sWL); //Deci_All의 wl 결과값
		delete RWaterLevel;
	} else {	// sWaterUseFlag == HYDROMETER_STR => 유속계1의 수위 데이터(RQ-30d의 수위데이터를 사용 )
		TRHydroMeter *HydroMeter = new TRHydroMeter(SQLConnection, 1);
		HydroMeter->ClearParams();
		HydroMeter->SetWhereParam("MEASUREDATE", sMeasureDate);
		HydroMeter->SetWhereParam("MEASURETIME", sMeasureTime);
		HydroMeter->Open();
		cDecisionWL = HydroMeter->GetWaterAdvText(sWL); //Deci_All의 wl 결과값
		delete HydroMeter;
	}

	//유속계1(하층유속계)
	TRHydroMeter *RHydroMeter1 = new TRHydroMeter(SQLConnection, 1);
	RHydroMeter1->ClearParams();
	RHydroMeter1->SetWhereParam("MEASUREDATE", sMeasureDate);
	RHydroMeter1->SetWhereParam("MEASURETIME", sMeasureTime);
	RHydroMeter1->Open();
	AnsiString sHD1 = "";
	char cDecl_HD1;
	int  iFileFormat = IsFileFormat();
	if ( iFileFormat == FORMAT_ADVM ) {
		cDecl_HD1 = RHydroMeter1->GetAdvText(sHD1); //Deci_All의 hydrometer 결과값
	} else if ( iFileFormat == FORMAT_UVM_ACCUSONIC ) {
		cDecl_HD1 = RHydroMeter1->GetUvmText(sHD1); //Deci_All의 hydrometer 결과값
	} else {}

	delete RHydroMeter1;

    //유속계2(상층유속계)
	TRHydroMeter *RHydroMeter2 = new TRHydroMeter(SQLConnection, 2);
	RHydroMeter2->ClearParams();
	RHydroMeter2->SetWhereParam("MEASUREDATE", sMeasureDate);
	RHydroMeter2->SetWhereParam("MEASURETIME", sMeasureTime);
	RHydroMeter2->Open();
	AnsiString sHD2 = "";
	char cDecl_HD2;
	if ( iFileFormat == FORMAT_ADVM ) {
		cDecl_HD2 = RHydroMeter2->GetAdvText(sHD2); //Deci_All의 hydrometer 결과값
	} else if ( iFileFormat == FORMAT_UVM_ACCUSONIC ) {
		cDecl_HD2 = RHydroMeter2->GetUvmText(sHD2); //Deci_All의 hydrometer 결과값
	} else {}

	delete RHydroMeter2;

//text 처리
    AnsiString sDeci_All = ""; //todo:  Deci_All, -> 기기별 정보판단(vth, 수위계, 1번 유속계, 2번 유속계)
    sDeci_All += (char)cDecisionVTH;
    sDeci_All += (char)cDecisionWL;
    sDeci_All += (char)cDecl_HD1;
    sDeci_All += (char)cDecl_HD2;

    //St_code,
    sResult += DBEnvInfo.sPointCode + ","; //지점코드
    //YYYYMMDDhhmm, -> 년월일 + 시분
    sResult += sMeasureDate + sMeasureTime.SubString(1, 4) + ",";
    sResult += sFormat.sprintf("%10s", sDeci_All) + ",";
    sResult += sVTH;
    sResult += sWL;
    sResult += sHD1;
    sResult += sHD2;

    return sResult;
}

//---------------------------------------------------------------------------
// Only use: EWSV+ADVM일 때 ADVM 자료를 만들 때 사용됨
UnicodeString TfrmMain::GetAdvTextFromDB2(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
//db의 값을 읽어와서 adv output text를 리턴
//판단=(0=정상/1=오측/2=결측)
{
    UnicodeString sResult = "";
    AnsiString sFormat;

    //vth
	TRVTHLogger *RVTHLogger = new TRVTHLogger(SQLConnection);
	RVTHLogger->ClearParams();
	RVTHLogger->SetWhereParam("MEASUREDATE", sMeasureDate);
	RVTHLogger->SetWhereParam("MEASURETIME", sMeasureTime);
	RVTHLogger->Open();
	AnsiString sVTH;
	char cDecisionVTH = RVTHLogger->GetAdvText(sVTH); //Deci_All의 vth 결과값
	delete RVTHLogger;

	//수위계
	AnsiString sWL = "";
	char cDecisionWL = 0x00;
	if ( DBEnvInfo.sWaterUseFlag == WATERLEVEL_STR ) {
		TRWaterLevel *RWaterLevel = new TRWaterLevel(SQLConnection, DBEnvInfo.fWLOffset);
		RWaterLevel->SetMeasureDateTime(sMeasureDate, sMeasureTime);	// heaven2 2021.12.10
		RWaterLevel->ClearParams();
		RWaterLevel->SetWhereParam("MEASUREDATE", sMeasureDate);
		RWaterLevel->SetWhereParam("MEASURETIME", sMeasureTime);
		RWaterLevel->Open();
		cDecisionWL = RWaterLevel->GetAdvText(sWL); //Deci_All의 wl 결과값
		delete RWaterLevel;
	} else {	// sWaterUseFlag == HYDROMETER_STR => 유속계1의 수위 데이터(RQ-30d의 수위데이터를 사용 )
		TRHydroMeter *HydroMeter = new TRHydroMeter(SQLConnection, 1);
		HydroMeter->ClearParams();
		HydroMeter->SetWhereParam("MEASUREDATE", sMeasureDate);
		HydroMeter->SetWhereParam("MEASURETIME", sMeasureTime);
		HydroMeter->Open();
		cDecisionWL = HydroMeter->GetWaterAdvText(sWL); //Deci_All의 wl 결과값
		delete HydroMeter;
	}

//	//유속계1(하층유속계) => RQ-30d 설치로 ADVM 형식을 지원하지 않는다 => 결측으로 처리
//	TRHydroMeter *RHydroMeter1 = new TRHydroMeter(SQLConnection, 1);
//	RHydroMeter1->ClearParams();
//	RHydroMeter1->SetWhereParam("MEASUREDATE", sMeasureDate);
//	RHydroMeter1->SetWhereParam("MEASURETIME", sMeasureTime);
//	RHydroMeter1->Open();
//	AnsiString sHD1 = "";
//	char cDecl_HD1;
	int  iFileFormat = IsFileFormat();
//	if ( iFileFormat == FORMAT_ADVM ) {
//		cDecl_HD1 = RHydroMeter1->GetAdvText(sHD1); //Deci_All의 hydrometer 결과값
//	} else if ( iFileFormat == FORMAT_UVM_ACCUSONIC ) {
//		cDecl_HD1 = RHydroMeter1->GetUvmText(sHD1); //Deci_All의 hydrometer 결과값
//	} else {}
//	delete RHydroMeter1;

    //유속계2(상층유속계)
	TRHydroMeter *RHydroMeter2 = new TRHydroMeter(SQLConnection, 2);
	RHydroMeter2->SetEwsvAdvmFlag(true);	// EWSV+ADVM인 경우만 해당
	RHydroMeter2->ClearParams();
	RHydroMeter2->SetWhereParam("MEASUREDATE", sMeasureDate);
	RHydroMeter2->SetWhereParam("MEASURETIME", sMeasureTime);
	RHydroMeter2->Open();
	AnsiString sHD2 = "";
	char cDecl_HD2;
	if ( iFileFormat == FORMAT_EWSV_ADVM ) {	// EWSV+ADVM일 때만 동작함
		cDecl_HD2 = RHydroMeter2->GetAdvText(sHD2); //Deci_All의 hydrometer 결과값
//	} else if ( iFileFormat == FORMAT_UVM_ACCUSONIC ) {
//		cDecl_HD2 = RHydroMeter2->GetUvmText(sHD2); //Deci_All의 hydrometer 결과값
	} else {}
	delete RHydroMeter2;

	//유속계3(3번 유속계)
	TRHydroMeter *RHydroMeter3 = new TRHydroMeter(SQLConnection, 3);
	RHydroMeter3->SetEwsvAdvmFlag(true);	// EWSV+ADVM인 경우만 해당
	RHydroMeter3->ClearParams();
	RHydroMeter3->SetWhereParam("MEASUREDATE", sMeasureDate);
	RHydroMeter3->SetWhereParam("MEASURETIME", sMeasureTime);
	RHydroMeter3->Open();
	AnsiString sHD3 = "";
	char cDecl_HD3;
	if ( iFileFormat == FORMAT_EWSV_ADVM ) {	// EWSV+ADVM일 때만 동작함
		cDecl_HD3 = RHydroMeter3->GetAdvText(sHD3); //Deci_All의 hydrometer 결과값
//	} else if ( iFileFormat == FORMAT_UVM_ACCUSONIC ) {
//		cDecl_HD2 = RHydroMeter2->GetUvmText(sHD2); //Deci_All의 hydrometer 결과값
	} else {}
	delete RHydroMeter3;

//text 처리
    AnsiString sDeci_All = ""; //todo:  Deci_All, -> 기기별 정보판단(vth, 수위계, 1번 유속계, 2번 유속계)
    sDeci_All += (char)cDecisionVTH;
    sDeci_All += (char)cDecisionWL;
	//sDeci_All += (char)cDecl_HD1;
	//sDeci_All += (char)'2';		// 1번 유속계는 RQ-30d 설치로 advm data는 결측으로 간주함.
	sDeci_All += (char)cDecl_HD2;
	sDeci_All += (char)cDecl_HD3;

	//St_code,
	sResult += DBEnvInfo.sPointCode + ","; //지점코드
	//YYYYMMDDhhmm, -> 년월일 + 시분
	sResult += sMeasureDate + sMeasureTime.SubString(1, 4) + ",";
	sResult += sFormat.sprintf("%10s", sDeci_All) + ",";
	sResult += sVTH;
	sResult += sWL;
	//sResult += sHD1;	// 1번 유속계는 RQ-30d 설치로 advm data는 결측으로 간주함.
	sResult += sHD2;	// 2번 유속계
	sResult += sHD3;	// 3번 유속계

	return sResult;
}

//---------------------------------------------------------------------------
// EWSV Format Data
UnicodeString TfrmMain::GetEsvTextFromDB(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
{
    UnicodeString sResult = "";
    AnsiString sFormat;

    //vth
	TRVTHLogger *RVTHLogger = new TRVTHLogger(SQLConnection);
	RVTHLogger->ClearParams();
	RVTHLogger->SetWhereParam("MEASUREDATE", sMeasureDate);
	RVTHLogger->SetWhereParam("MEASURETIME", sMeasureTime);
	RVTHLogger->Open();
	AnsiString sVTH;
	char cDecisionVTH = RVTHLogger->GetAdvText(sVTH); //Deci_All의 vth 결과값
	delete RVTHLogger;

	//수위계
	AnsiString sWL = "";
	char cDecisionWL = 0x00;
	if ( DBEnvInfo.sWaterUseFlag == WATERLEVEL_STR ) {
		TRWaterLevel *RWaterLevel = new TRWaterLevel(SQLConnection, DBEnvInfo.fWLOffset);
		RWaterLevel->SetMeasureDateTime(sMeasureDate, sMeasureTime);	// heaven2 2021.12.10
		RWaterLevel->ClearParams();
		RWaterLevel->SetWhereParam("MEASUREDATE", sMeasureDate);
		RWaterLevel->SetWhereParam("MEASURETIME", sMeasureTime);
		RWaterLevel->Open();
		cDecisionWL = RWaterLevel->GetAdvText(sWL); //Deci_All의 wl 결과값
		delete RWaterLevel;
	} else {	// sWaterUseFlag == HYDROMETER_STR => 유속계1의 수위 데이터(RQ-30d의 수위데이터를 사용 )
		TRHydroMeter *HydroMeter = new TRHydroMeter(SQLConnection, 1);
		HydroMeter->ClearParams();
		HydroMeter->SetWhereParam("MEASUREDATE", sMeasureDate);
		HydroMeter->SetWhereParam("MEASURETIME", sMeasureTime);
		HydroMeter->Open();
		cDecisionWL = HydroMeter->GetWaterAdvText(sWL); //Deci_All의 wl 결과값
		delete HydroMeter;
	}

	// 풍향 풍속계(RQ-30 => RHydroMeter1 Table) --------------------------------
	// 풍향계 데이터를 얻는 설정이 유속계=> 유속계에서 얻는다.

	AnsiString sWind = "";
	char cDecisionWind = 0x00;

	if ( DBEnvInfo.sWaterKind == RNSWATER_STR && DBEnvInfo.sAirmarEnable == ON_STR ) {
		// 풍속 데이터 => RWaterLevel에 있음
		TRWaterLevel *RWind = new TRWaterLevel(SQLConnection, DBEnvInfo.fWLOffset);
		RWind->ClearParams();
		RWind->SetWhereParam("MEASUREDATE", sMeasureDate);
		RWind->SetWhereParam("MEASURETIME", sMeasureTime);
		RWind->Open();
		cDecisionWind = RWind->GetWindText(sWind);	// 풍속계판단, 풍향, 풍속, 온도, 습도, 기압
		delete RWind;
	} else if ((DBEnvInfo.sHydroKind1 == RQ30_STR || DBEnvInfo.sHydroKind1 == RQ30D_STR) && // RQ-30/RQ-30d일때만 동작
			   (!DBEnvInfo.sAirmarPort.IsEmpty() && DBEnvInfo.sAirmarPort != NONE_STR )) {
		// 유속계(RQ-30d)에서 Airmar 데이터 수집
		TRHydroMeter *RWind = new TRHydroMeter(SQLConnection, 1);
		RWind->ClearParams();
		RWind->SetWhereParam("MEASUREDATE", sMeasureDate);
		RWind->SetWhereParam("MEASURETIME", sMeasureTime);
		RWind->Open();
		cDecisionWind = RWind->GetWindText(sWind);	// 풍속계판단, 풍향, 풍속, 온도, 습도, 기압
		delete RWind;
	} else {
		// Airmar 데이터 수집이 없다. => 0으로 채운다.
		sResult = "";

		double fDegree   = 0.0f;	// 풍향
		double fWind     = 0.0f;	// 풍속
		double fTemp     = 0.0f;	// 온도
		double fHumidity = 0.0f;	// 습도
		double fAtmos    = 0.0f;	// 대기압

		AnsiString sDeci_Wind = "22222";	// 결측 데이터
		sWind += sFormat.sprintf("%10s",   sDeci_Wind ) + ","; // Deci_Wind,  -> 풍속계 판단
		sWind += sFormat.sprintf("%10.2f", fDegree )    + ","; // Degree,     -> 풍향
		sWind += sFormat.sprintf("%10.3f", fWind )      + ","; // Wind,       -> 풍속
		sWind += sFormat.sprintf("%10.3f", fTemp )      + ","; // Temp,       -> 온도
		sWind += sFormat.sprintf("%10.2f", fHumidity )  + ","; // Humidity,   -> 습도
		sWind += sFormat.sprintf("%10d", (int)fAtmos )  + ","; // Atmos,      -> 대기압
	}

	//유속계1(하층유속계) => RQ-30D Data
	TRHydroMeter *RHydroMeter1 = new TRHydroMeter(SQLConnection, 1);
	RHydroMeter1->ClearParams();
	RHydroMeter1->SetWhereParam("MEASUREDATE", sMeasureDate);
	RHydroMeter1->SetWhereParam("MEASURETIME", sMeasureTime);
	RHydroMeter1->Open();
	AnsiString sHD1 = "";
	char cDecl_HD1;
	cDecl_HD1 = RHydroMeter1->GetEsvText(sHD1); //Deci_All의 hydrometer 결과값
	delete RHydroMeter1;

	//유속계2(상층유속계) => EWSV Format에서 상층유속계(유속계 2는 없음). 단, EWSV+ADVM에서는 상층 유속계를 포함해야 한다.

//text 처리
    AnsiString sDeci_All = ""; //todo:  Deci_All, -> 기기별 정보판단(vth, 수위계, 1번 유속계, 2번 유속계)
	sDeci_All += (char)cDecisionVTH;
	sDeci_All += (char)cDecisionWL;
	sDeci_All += (char)cDecisionWind;
	sDeci_All += (char)cDecl_HD1;
	//sDeci_All += (char)cDecl_HD2; // EWSV에서는 하지 않음.

    //St_code,
	sResult += DBEnvInfo.sPointCode + ","; //지점코드
	//YYYYMMDDhhmm, -> 년월일 + 시분
    sResult += sMeasureDate + sMeasureTime.SubString(1, 4) + ",";
    sResult += sFormat.sprintf("%10s", sDeci_All) + ",";
    sResult += sVTH;
	sResult += sWL;
	sResult += sWind;
	sResult += sHD1;
	//sResult += sHD2;

    return sResult;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::miVTHClick(TObject *Sender)
//vth표시창 열기
{
    ShowVth(this);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	static bool bFirst = false;

#ifdef _DEBUG
	Button1->Visible = true;
#else
	Button1->Visible = false;  	// Release mode => button hide
#endif

	if ( bFirst ) return;	// taskbar icon에서 창이 뜰떄 매번 실행되므로 처음 1회만
	bFirst = true;

	UnicodeString Version = VersionInfo("FileVersion");
	lbVersion->Caption = Version;

	mmLog->Hint = "프로그램 실행시간: " + FormatDateTime("yyyy-mm-dd hh:nn:ss.zzz", Now());
	Application->ProcessMessages();
	tmrGoToTray->Enabled = true;
	this->Height = 175;
	LocateRightButton();

	SearchDBRPoint = new TfbTable(SQLConnection, "RPOINT");

	lstViewer = new TList;

	pbLogger = new TPacketBuffer;
	pbViewer = new TPacketBuffer;

	thrdLog = new TLogThread(); //로그스레드 먼저 생성
	LogMessage(CbFileWrite, CbDBWrite, "DataSender 프로그램 시작");

	ReadIniInfo();

	if (!ConnectDB())
	{
		//ShowMessage("DB에 연결할 수 없습니다.\n프로그램이 종료됩니다.");
		Application->Terminate();
		return;
	} // if

	TrayIconRefresh();	// System Tray Icon Refresh 2015.10.02

	GetDBEnvInfo();

	// FTP Mode
	FTP->Passive      = (DBEnvInfo.sFTPMode       == "Active" ) ? false : true;
	ftpRnsea->Passive = ( DBEnvInfo.sRnseaFTPMode == "Active" ) ? false : true;

//	FTP->ConnectTimeout      = 10 * 1000;	// 10 Second for Connection time out
//	FTP->ReadTimeout         =  6 * 1000;	//  6 Second for Read Time out
//	ftpRnsea->ConnectTimeout = 10 * 1000;	// 10 Second for Connection time out
//	ftpRnsea->ReadTimeout    =  6 * 1000;	//  6 Second for Read Time out

	//if (DBEnvInfo.iComNumber == 0) DBEnvInfo.iComNumber = 99; //컴포틑 설정이 없으면 기본으로 99설정 heaven2 99=> -1

	OpenVTH();

	// Timer Enable
	tmrDisplayTime->Interval = 1000;	// General Timer
	tmrDisplayTime->Enabled = true;
	Sleep(200);	//Wait for while

	tmrSendToViewers->Interval = 30 * 1000;	// 30second Timer => viewer Timer
	tmrSendToViewers->Enabled = true;
	tmrSendToViewersTimer(this);

	Sleep(200); // Wait for while

	tmrWD->Interval = 30 * 1000;	// 30second timer => WatchDog Timer
	tmrWD->Enabled  = true;
	tmrWDTimer(this);				// Try to connect WatchDog S/W

	//스레드 시작
	//thrdLog      = new TLogThread(); //로그스레드 먼저 생성 ( 앞에서 먼저 생성함 => 로그를 적기 위해)
	thrdFTP      = new TFtpThread();
	thrdGuardnet = new TGuardnetThread();

	m_VTHTimeStamp = Now();	// VTHLogger Get TimeStamp 2015.03.03
	m_DBCheckTimeStamp = Now();	// DBCheckTimeStamp 2024.10.05

	ResetSocketServer();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnL2WClick(TObject *Sender)
{
	//가드넷 유선->무선 전환
	if (thrdGuardnet->IsTesting())
	{
        LogMessage(!CbFileWrite, !CbDBWrite, "가드넷 테스트 중에는 전환할 수 없습니다.", false);
        return;
	} // if

	if (!DBEnvInfo.bGNEnable) return;

	pGGuardnetSection->Enter();

	STGuardnetMessage *pGuardnetMessage = new STGuardnetMessage();
	pGuardnetMessage->sText = CsGNChange2W;
	pGGuardnetQueue->Push(pGuardnetMessage);

	pGGuardnetSection->Leave();

	if ( !thrdGuardnet->IsTesting() )
		pGGuardnetEvent->SetEvent();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnW2LClick(TObject *Sender)
{
    //가드넷 무선->유선 전환
    if (thrdGuardnet->IsTesting())
    {
        LogMessage(!CbFileWrite, !CbDBWrite, "가드넷 테스트 중에는 전환할 수 없습니다.", false);
        return;
	} // if

	if (!DBEnvInfo.bGNEnable) return;

	pGGuardnetSection->Enter();

	STGuardnetMessage *pGuardnetMessage = new STGuardnetMessage();
	pGuardnetMessage->sText = CsGNChange2L;
	pGGuardnetQueue->Push(pGuardnetMessage);

	pGGuardnetSection->Leave();

	if ( !thrdGuardnet->IsTesting() )
		pGGuardnetEvent->SetEvent();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrGuardnetTimer(TObject *Sender)
//가드넷 타이머; 테스트를 위해서 가드넷 큐에 request push
{
	if (thrdGuardnet->IsTesting()) return;

	if (!DBEnvInfo.bGNEnable) return;

	pGGuardnetSection->Enter();

	STGuardnetMessage *pGuardnetMessage = new STGuardnetMessage();
	pGuardnetMessage->sText = CsGNPingTest;
	pGGuardnetQueue->Push(pGuardnetMessage);

	pGGuardnetSection->Leave();

	if ( !thrdGuardnet->IsTesting() )
		pGGuardnetEvent->SetEvent();
}

//---------------------------------------------------------------------------
// 30초마다 Viewer에 DataSender의 상태를 알린다.
void __fastcall TfrmMain::tmrSendToViewersTimer(TObject *Sender)
{
	SendSenderStatusToViewer();
}

//---------------------------------------------------------------------------
// DataSender의 상태를 Viewer에 Broadcasting한다.
void TfrmMain::SendSenderStatusToViewer()	// DataSender의 상태를 Viewer에 보낸다.
{
	BYTE cFTPStatus, cGuardNetStatus;
	DWORD  Temperature, Humidity, DCBattery, DCCharge;

	cFTPStatus      = fFTPStatus;
	cGuardNetStatus = fGuardNetStatus;
	Temperature     = htonl(((int)(fVTHTemperature * 1000)));  	// Big endian
	Humidity        = htonl(((int)(fVTHHumidity * 1000)));     	// Big endian
	DCBattery       = htonl(((int)(fVTHBattery * 1000)));		// Big endian
	DCCharge        = htonl(((int)(fVTHCharge * 1000)));			// Big endian

	BYTE sData[1024] = "";
	int  iSize = 0;

	sData[ 0] = STX;
	sData[ 1] = MSG_TYPE_SenderActionEvent;	// 0x02
	sData[ 2] = 20;						// Payload : 20 byte
	sData[ 3] = cFTPStatus;
	sData[ 4] = cGuardNetStatus;
	memcpy(&sData[ 5], &Temperature, 4);	// Temperature
	memcpy(&sData[ 9], &Humidity,    4);	// Humidity
	memcpy(&sData[13], &DCBattery,   4);	// DC Battery
	memcpy(&sData[17], &DCCharge,    4);	// DC Charge
	sData[21] = 0x00; 					// Reserved
	sData[22] = 0x00; 					// Reserved
	sData[23] = MakeCRC(sData, 23);		// Make CRC
	sData[24] = ETX;

	iSize = 25;

	int iCnt     = lstViewer->Count;
	int iSuccess = 0;
	TMemoryStream *ms = new TMemoryStream;
	ms->Size = 0;
	ms->Write(sData, iSize);
	for(int i = iCnt - 1; i >= 0; i--)
	{
		try {
			ms->Position = 0;
			if (((TIdContext *)lstViewer->List[i])->Connection->Socket->Connected()) ((TIdContext *)lstViewer->List[i])->Connection->Socket->CheckForDisconnect(false);
			if (((TIdContext *)lstViewer->List[i])->Connection->Socket->Connected())
			{
				((TIdContext *)lstViewer->List[i])->Connection->Socket->Write(ms, iSize);
				iSuccess++;
			} // if
		} catch(Exception &e) {
			//전송실패한 클라이언트는 연결을 끊고 리스트에서 제거
			lstViewer->Delete(i);
			LogMessage(CbFileWrite, !CbDBWrite, "전송실패한 클라이언트 제거: " + e.Message);
		}// catch
	}// end of for

	delete ms;

	LogMessage(CbFileWrite, !CbDBWrite, "DataViewer에 상태신호 전송");
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnConfigClick(TObject *Sender)
//설정버튼
{
	ShowConfig(ciPoint | ciSender | ciGuardnet | ciRange | ciEtc, //표시할 설정 페이지
               ciSender); //디폴트 페이지
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ApplicationEventsException(TObject *Sender, Exception *E)
{
	LogMessage(CbFileWrite, !CbDBWrite, "[ApplicationEvents] " + E->Message);
    ApplicationEvents->CancelDispatch();
}

//---------------------------------------------------------------------------
void TfrmMain::DoReset()
//환경 리셋
{
	GetDBEnvInfo();
	FTP->Passive      = (DBEnvInfo.sFTPMode       == "Active" ) ? false : true;
	ftpRnsea->Passive = ( DBEnvInfo.sRnseaFTPMode == "Active" ) ? false : true;

    //소켓서버 리셋
	ResetSocketServer();
  	LogMessage(CbFileWrite, !CbDBWrite, "소켓서버 리셋");
    //watch dog client
    if (WatchDogClient->Connected()) WatchDogClient->Disconnect(); //타이머가 설정을 최신으로 하여 재연결 함
  	LogMessage(CbFileWrite, !CbDBWrite, "watch dog 클라이언트 연결 해제");
    //vth로거 리셋
    OpenVTH();
}

//---------------------------------------------------------------------------
void TfrmMain::ResetSocketServer()
{
	//환경구조체에서 읽은 포트번호로 소켓서버 동적 바인딩
    if (LoggerServer->Active) LoggerServer->Active = false;
	LoggerServer->DefaultPort = DBEnvInfo.iDataLoggerPort;
    Sleep(100);
	LoggerServer->Bindings->Clear();
	LoggerServer->Bindings->Add()->SetBinding("0.0.0.0"  , DBEnvInfo.iDataLoggerPort, ID_DEFAULT_IP_VERSION);
	//LoggerServer->Bindings->Add()->SetBinding("127.0.0.1", DBEnvInfo.iDataLoggerPort, ID_DEFAULT_IP_VERSION);
    try
    {
    	LoggerServer->Active = true;
    } catch(Exception &e)
    {
      	LogMessage(CbFileWrite, !CbDBWrite, "[LoggerServer] " + e.Message);
    } // catch

    if (ViewerServer->Active) ViewerServer->Active = false;
    Sleep(100);
	ViewerServer->DefaultPort = DBEnvInfo.iDataViewerPort;
	ViewerServer->Bindings->Clear();
	ViewerServer->Bindings->Add()->SetBinding("0.0.0.0"  , DBEnvInfo.iDataViewerPort, ID_DEFAULT_IP_VERSION);
	//ViewerServer->Bindings->Add()->SetBinding("127.0.0.1", DBEnvInfo.iDataViewerPort, ID_DEFAULT_IP_VERSION);
	try
	{
		ViewerServer->Active = true;
	} catch(Exception &e)
	{
		LogMessage(CbFileWrite, !CbDBWrite, "[ViewerServer] " + e.Message);
	} // catch
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeFTPIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.
	if ( idx < 0 || idx > 5 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ImageListFTP->GetBitmap(idx, pBitmap);
	this->imgFTPStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeGuardNetIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	idx = fGuardNetStatus;

	if ( idx < 0 || idx > 7 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ImageListGuardNet->GetBitmap(idx, pBitmap);
	this->imgGuardnetStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeLineIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	idx = fLineStatus;

	if ( idx < 6 || idx > 7 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ImageListGuardNet->GetBitmap(idx, pBitmap);
	this->imgLineStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeBatteryIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	if ( idx < 0 || idx > 1 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ImageListVTH->GetBitmap(idx, pBitmap);
	this->imgVthStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeLoggerIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	if ( idx < 0 || idx > 1 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	ilStatus->GetBitmap(idx, pBitmap);
	imgLoggerStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeWatchDogIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.

	if ( idx < 2 || idx > 3 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ilStatus->GetBitmap(idx, pBitmap);
	this->imgWdStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeViewerIcon(int idx)
{
	if ( Application->Terminated ) return; // Main UI terminated.
	if ( idx < 4 || idx > 15 ) return;	// idx out of range

	TBitmap *pBitmap = new TBitmap();
	this->ilStatus->GetBitmap(idx, pBitmap);
	this->imgViewerStatus->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TfrmMain::ChangeVTHValue()
{
	if ( Application->Terminated ) return; // Main UI terminated.

	pnlDCCharge->Caption    = FormatFloat("#0.00", fVTHCharge);
	pnlDCBattery->Caption   = FormatFloat("#0.00", fVTHBattery);
	pnlTemperature->Caption = FormatFloat("#0.00", fVTHTemperature);

//	if ( fVTHCharge < 12.0 || fVTHBattery < 12.0 ) {
	if ( fVTHCharge < 11.5 || fVTHBattery < 11.5 ) {	// 2015.08.19
		ChangeBatteryIcon(0); 	// Alarm Icon
	} else {
		ChangeBatteryIcon(1);	// Normal Icon
	}
}

//---------------------------------------------------------------------------
void TfrmMain::SendWatchDogMessage(BYTE Action)	// 0x00: Normal HeartBeat, 0xFF: Shutdown Request
{
	BYTE Data[1024] = "";
	int  iSize = 0;

	Data[ 0] = STX;
	Data[ 1] = MSG_TYPE_HeartbeatSignal;	// 0x90
	Data[ 2] = 4;							// Payload : 4 Byte
	Data[ 3] = 0x02;						// DATASENDER
	Data[ 4] = Action;						// 0x00: Normal HeartBeat, 0xFF: Shutdown
	Data[ 5] = 0x00;						// Reserved
	Data[ 6] = 0x00;						// Reserved
	Data[ 7] = MakeCRC(Data, 7);			// Make CRC
	Data[ 8] = ETX;

	iSize = 9;

	TByteDynArray ABuffer;
	int           ALength;
	int           AOffset;

	ABuffer.set_length(9);

	for ( int i = 0 ; i < 9 ; i++ )
		ABuffer[i] = Data[i];
	ALength = 9;
	AOffset = 0;

	try {
		WatchDogClient->IOHandler->Write(ABuffer, ALength, AOffset);
	} catch ( Exception &e ) {
		LogMessage(CbFileWrite, !CbDBWrite, "WatchDog Heartbeat 전송 에러");
	}

	if ( Action == 0x00 ) {
		LogMessage(CbFileWrite, !CbDBWrite, "WatchDog Heartbeat 전송");
	} else if ( Action == 0xFF ) {
		LogMessage(CbFileWrite, !CbDBWrite, "Shutdown Request Message send to WatchDog");
	} else {}

}

//---------------------------------------------------------------------------
void TfrmMain::SetUIExtend(bool bEnlarge)
{
	if (bEnlarge)
	{
		this->Height = 750;
		btnWindowSizeToggle->Caption = "▼";
		this->Top = Screen->WorkAreaHeight - this->Height;
	} else
	{
		this->Height = 175;
		btnWindowSizeToggle->Caption = "▲";
		this->Top = Screen->WorkAreaHeight - this->Height;
	} // else
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ApdComPortPortClose(TObject *Sender)
{
	LogMessage(CbFileWrite, !CbDBWrite, "COM Port is closed..");
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ApdComPortPortOpen(TObject *Sender)
{
	LogMessage(CbFileWrite, !CbDBWrite, "COM Port is opened..");
}

//---------------------------------------------------------------------------
void TfrmMain::CheckAllocateMemory(void *p)  			// GlobalAlloc()함수가 Fail이 나면 WatchDog에 Restart를 요청한다.
{				 						// GlobalAlloc()에서 메모리 Get이 Error 5번 나면 Restart한다.
	static int iCount = 0;

	if ( p != NULL ) return;

	iCount++;
	UnicodeString sMsg;
	sMsg = sMsg.sprintf(L"GlobalAlloc() is fail %d times. if count >=5, datasender will restart by watchdog.", iCount);
	LogMessage(CbFileWrite, !CbDBWrite, sMsg);

	if ( iCount >= 5 ) {
		BYTE Action = ACTION_SHUTDOWN_REQUEST;
		SendWatchDogMessage(Action);
	}
}

//---------------------------------------------------------------------------
// Send Command to VTHLogger
bool TfrmMain::SendVTH(AnsiString sMsg)
{
	if ( ApdComPort->Open == false ) return false;

	try {

		if ( sMsg == "!" ) {
			GbGWPowerOff = true;
			tmrGWResetDelay->Interval = 1000 * 60 * DBEnvInfo.iGNDelayMinuteAfterGWReset; //대기시간
			tmrGWResetDelay->Enabled = true;
			LogMessage(CbFileWrite, !CbDBWrite, "G/W 리셋시작(FtpThread, GuardnetThread는 대기합니다.", (GiGWResetFrom <= 1) ? true : false);
		}

		ApdComPort->PutString(sMsg);
		LogMessage(CbFileWrite, !CbDBWrite, AnsiString("VTH Send => ") + sMsg);
		m_bVTHRequest = true;

	} catch ( Exception &e ) {
		LogMessage(CbFileWrite, !CbDBWrite, AnsiString("VTH Send error : ") + sMsg);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Save VTHLogger Data ( call only Logger Finish Event )
void TfrmMain::SaveVTHData()
{
	TfbTable *RPoint;
	TfbTable *RVTHLogger;

	RPoint     = new TfbTable(frmMain->SQLConnection, "RPOINT");
	RVTHLogger = new TfbTable(frmMain->SQLConnection, "RVTHLOGGER");

	//ChangeVTHValue();	// UI Update

	bool bExist;
	RPoint->ClearParams();
	RPoint->SetWhereParam("MEASUREDATE", GsMeasureDate);
	RPoint->SetWhereParam("MEASURETIME", GsMeasureTime);

	try
	{
		bExist = RPoint->Exists();
	} // try
	catch(Exception &e)
	{
		LogMessage(CbFileWrite, !CbDBWrite, "DB 쿼리 오류: " + e.Message);
		LogMessage(CbFileWrite, !CbDBWrite, RPoint->SqlText);
	} // catch

	if (!bExist)
	{
		RPoint->ClearParams();
		RPoint->SetFieldParam("MEASUREDATE", GsMeasureDate);
		RPoint->SetFieldParam("MEASURETIME", GsMeasureTime);
		RPoint->SetFieldParam("FILEFLAG",   "N");
		RPoint->SetFieldParam("SERVERFLAG", "N");
		RPoint->SetFieldParam("RNSEAFLAG",  "N");
		UnicodeString sErrorMessage = RPoint->Insert();
		if (sErrorMessage != "")
		{
			LogMessage(CbFileWrite, !CbDBWrite, "RPoint DB저장 오류: " + sErrorMessage);
			LogMessage(CbFileWrite, !CbDBWrite, RPoint->SqlText);
		} // if
	} // if

	//RVTHLOGGER에 insert
	//UnicodeString sVolt = (StrToFloat(GetSepItem(sMsg, 5, ",")) == 0.0f) ? "0" : "220"; //dc charge=0이면 전압도 0
	UnicodeString sVolt;
	if ( DBEnvInfo.sACOpt.IsEmpty() || DBEnvInfo.sACOpt.UpperCase() == "BATTERY" ) {
		//sVolt = (StrToFloat(GetSepItem(sMsg, 2, ",")) < 10.0f) ? "0" : "220"; //DC Battery 10Volt 이하면 AC Fail
		sVolt = (fVTHBattery < 10.0f) ? "0" : "220"; //DC Battery 10Volt 이하면 AC Fail
	} else {
		//sVolt = (StrToFloat(GetSepItem(sMsg, 6, ",")) < 10.0f) ? "0" : "220"; //AIN0 10volt 이하이면 AC Fail
		sVolt = (fAIN0 < 10.0f) ? "0" : "220"; //AIN0 10volt 이하이면 AC Fail
	}

	RVTHLogger->ClearParams();
	RVTHLogger->SetFieldParam("MEASUREDATE", GsMeasureDate); 	//일자
	RVTHLogger->SetFieldParam("MEASURETIME", GsMeasureTime); 	//시간
	RVTHLogger->SetFieldParam("VTHKIND", "VTH Logger"); 					//"VTH Logger"
	RVTHLogger->SetFieldParam("VOLT",        sVolt); 						//전압
	RVTHLogger->SetFieldParam("TEMPERATURE", UnicodeString::FloatToStrF(fVTHTemperature, ffFixed, 5, 2)); 	//온도
	RVTHLogger->SetFieldParam("HUMIDITY",    UnicodeString::FloatToStrF(fVTHHumidity,    ffFixed, 5, 2)); 	//습도
	RVTHLogger->SetFieldParam("DCCHARGE",    UnicodeString::FloatToStrF(fVTHCharge,      ffFixed, 5, 2)); 	//dc charge
	RVTHLogger->SetFieldParam("DCBATTERY",   UnicodeString::FloatToStrF(fVTHBattery,     ffFixed, 5, 2)); 	//dc battery
	RVTHLogger->SetFieldParam("Value01",     UnicodeString::FloatToStrF(fAIN0, ffFixed, 5, 2)); 	//AIN0
	RVTHLogger->SetFieldParam("Value02",     UnicodeString::FloatToStrF(fAIN1, ffFixed, 5, 2)); 	//AIN1
	RVTHLogger->SetFieldParam("Value03",     UnicodeString::FloatToStrF(fAIN2, ffFixed, 5, 2)); 	//AIN2
	RVTHLogger->SetFieldParam("Value04",     UnicodeString::FloatToStrF(fAIN3, ffFixed, 5, 2));	    //AIN3
	RVTHLogger->SetFieldParam("RAWDATA",     sVTHMsg); 						//raw data

//	RVTHLogger->SetFieldParam("TEMPERATURE", GetSepItem(sMsg, 0, ",")); 	//온도
//	RVTHLogger->SetFieldParam("HUMIDITY",    GetSepItem(sMsg, 1, ",")); 	//습도
//	RVTHLogger->SetFieldParam("DCCHARGE",    GetSepItem(sMsg, 5, ",")); 	//dc charge
//	RVTHLogger->SetFieldParam("DCBATTERY",   GetSepItem(sMsg, 2, ",")); 	//dc battery
//	RVTHLogger->SetFieldParam("Value01",     GetSepItem(sMsg, 6, ",")); 	//AIN0
//	RVTHLogger->SetFieldParam("Value02",     GetSepItem(sMsg, 7, ",")); 	//AIN1
//	RVTHLogger->SetFieldParam("Value03",     GetSepItem(sMsg, 8, ",")); 	//AIN2
//	RVTHLogger->SetFieldParam("Value04",     GetSepItem(sMsg, 9, ",")); 	//AIN3
//	RVTHLogger->SetFieldParam("RAWDATA",     sMsg); 						//raw data

	UnicodeString sErrorMessage = RVTHLogger->Insert();
	if (sErrorMessage != "")
	{
		LogMessage(CbFileWrite, !CbDBWrite, "VTH DB저장 오류: " + sErrorMessage);
		LogMessage(CbFileWrite, !CbDBWrite, RVTHLogger->SqlText);
	} // if

	delete RPoint;    	// Resource release
	delete RVTHLogger;	// Resource release
}

//---------------------------------------------------------------------------
// Modem Line Error
void __fastcall TfrmMain::ApdComPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
//
}

class TMyQueue : public TQueue {
public:
	TList* GetList() {
		return TOrderedList::List;
	}
};

//---------------------------------------------------------------------------
// FTP Queue에 중복된 파일이 있는 지 검사한다.
bool TfrmMain::IsExistFTPQueue(STFTPMessage *pFTPMessage)
{
	STFTPMessage *p = NULL;
	int Count = 0;
	bool bExist = false;
	TList *pList = NULL;

	pGFTPSection->Enter();
	Count = pGFTPQueue->Count();
	pList = ((TMyQueue *)pGFTPQueue)->GetList();  	// Type Casting => protected member => public member
	for ( int i = 0 ; i < Count ; i++ ) {
		p = (STFTPMessage *) pList->Items[i];
		if ( p == NULL ) break;
		if ( p->sLocalPathFile == pFTPMessage->sLocalPathFile ) {
			bExist = true;
			break;
		}
	}
	pGFTPSection->Leave();

	return bExist;
}

//---------------------------------------------------------------------------
// heaven2 2017.09.26
int TfrmMain::IsFileFormat()
{
	if ( DBEnvInfo.sFtpFileFormat == FORMAT_ADVM_STR )                 return FORMAT_ADVM;				// 1
	else if ( DBEnvInfo.sFtpFileFormat == FORMAT_UVM_ACCUSONIC_STR )   return FORMAT_UVM_ACCUSONIC;		// 2
	else if ( DBEnvInfo.sFtpFileFormat == FORMAT_UVM_HYDROVISION_STR ) return FORMAT_UVM_HYDROVISION;	// 3
	else if ( DBEnvInfo.sFtpFileFormat == FORMAT_EWSV_STR )            return FORMAT_EWSV;				// 4
	else if ( DBEnvInfo.sFtpFileFormat == FORMAT_EWSV_ADVM_STR )       return FORMAT_EWSV_ADVM;			// 5
	else                                                               return FORMAT_ADVM;				// 1
}

//---------------------------------------------------------------------------
// heaven2 2017.09.26
bool TfrmMain::SendPing()
{
	BYTE Data[1024];
	memset(Data, 0x00, 1024);

	Data[0] = STX;
	Data[1] = MSG_TYPE_Ping;	// 0x01
	Data[2] = 0;	// Len
	Data[3] = MakeCRC(Data,3);
	Data[4] = ETX;

	bool bResult = false;
	bResult = SendMessageToLogger(Data, 5);

	if ( bResult) LogMessage(CbFileWrite, !CbDBWrite, "Ping..");
	else          LogMessage(CbFileWrite, !CbDBWrite, "Ping failed..");

	return bResult;
}

//---------------------------------------------------------------------------
bool TfrmMain::SendPingAck()
{
	BYTE Data[1024];
	memset(Data, 0x00, 1024);

	Data[0] = STX;
	Data[1] = MSG_TYPE_PingAck;	// 0x01
	Data[2] = 0;	// Len
	Data[3] = MakeCRC(Data,3);
	Data[4] = ETX;

	bool bResult = false;
	bResult = SendMessageToLogger(Data, 5);

	if ( bResult) LogMessage(CbFileWrite, !CbDBWrite, "Ping acked..");
	else          LogMessage(CbFileWrite, !CbDBWrite, "Ping ack failed..");

	return bResult;
}

//---------------------------------------------------------------------------
bool TfrmMain::SendRebootReq()
{
	BYTE Data[1024];
	memset(Data, 0x00, 1024);
	int Pos = 0;

	Data[0] = STX;
	Data[1] = MSG_TYPE_RebootReq;	// 0x01
	Data[2] = 0;	// Len
	Data[3] = MakeCRC(Data,3);
	Data[4] = ETX;

	bool bResult = false;
	bResult = SendMessageToLogger(Data, 5);

	if ( bResult) LogMessage(CbFileWrite, !CbDBWrite, "RebootReq to DataLogger");
	else          LogMessage(CbFileWrite, !CbDBWrite, "RebootReq failed..");

	return bResult;
}

//---------------------------------------------------------------------------
bool TfrmMain::SendRebootRes()
{
	BYTE Data[1024];
	memset(Data, 0x00, 1024);
	int Pos = 0;

	Data[0] = STX;
	Data[1] = MSG_TYPE_RebootRes;	// 0x01
	Data[2] = 0;	// Len
	Data[3] = MakeCRC(Data,3);
	Data[4] = ETX;

	bool bResult = false;
	bResult = SendMessageToLogger(Data, 5);

	if ( bResult) LogMessage(CbFileWrite, !CbDBWrite, "RebootRes to DataLogger");
	else          LogMessage(CbFileWrite, !CbDBWrite, "RebootRes fail..");

	return bResult;
}

//---------------------------------------------------------------------------
bool TfrmMain::SendMessageToLogger(BYTE *pData, int Len)
{
	TList *pList = LoggerServer->Contexts->LockList();
	int iCount = pList->Count;
	if ( iCount == 0 ) return false;		// not connected DatLogger

	TMemoryStream *MemoryStream = new TMemoryStream;
	MemoryStream->Size = 0;
	MemoryStream->Write(pData, 5);

	bool bResult = true;
	for ( int i = 0 ; i < iCount ; i++ ) {
		try {
			MemoryStream->Position = 0;
			if (((TIdContext *)pList->List[i])->Connection->Socket->Connected()) ((TIdContext *)pList->List[i])->Connection->Socket->CheckForDisconnect(false);
			if (((TIdContext *)pList->List[i])->Connection->Socket->Connected())
			{
				((TIdContext *)pList->List[i])->Connection->Socket->Write(MemoryStream, 5);
			} // if
			bResult = true;
		} catch ( Exception &e ) {
			// nothing to do..
			bResult = false;
		}
	}
	LoggerServer->Contexts->UnlockList();
	delete MemoryStream;
	return bResult;
}

//#define MSG_TYPE_Ping				   0x20		// 2017.09.21 heaven2
//#define MSG_TYPE_PingAck               0x21		// 2017.09.21 heaven2
//#define MSG_TYPE_RebootReq             0x22		// 2017.09.21 heaven2
//#define MSG_TYPE_RebootRes             0x23		// 2017.09.21 heaven2
//---------------------------------------------------------------------------
// Process Sender Message
bool TfrmMain::ProcessLoggerMessage(BYTE *pData, int Len)
{
	bool bResult = false;
	BYTE Type = *(pData + 1);	// Message Type

	//////////////////////////////////////////////////////////////////////
	// Ping
	if ( Type ==  MSG_TYPE_Ping ) {				// Ping Request
		bResult = SendPingAck();
		if ( !bResult ) LogMessage(CbFileWrite, !CbDBWrite, "Ping ack failed..");
		else            LogMessage(CbFileWrite, !CbDBWrite, "Ping acked..");

	//////////////////////////////////////////////////////////////////////
	// PingAck
	} else if ( Type == MSG_TYPE_PingAck ) {	// Ping Ack
		if ( FormConfig != NULL ) {
			FormConfig->DataLoggerMessage(pData, Len);
		} else {
			LogMessage(CbFileWrite, !CbDBWrite, "Ping acked. but SetupForm is not ready..");
		}

	//////////////////////////////////////////////////////////////////////
	// RebootReq
	} else if ( Type == MSG_TYPE_RebootReq ) {	// Reboot request
		bResult = SendRebootRes();
		if ( !bResult ) LogMessage(CbFileWrite, !CbDBWrite, "RebootRes failed..");
		else            LogMessage(CbFileWrite, !CbDBWrite, "RebootRes ok..");
		LogMessage(CbFileWrite, !CbDBWrite, "Sender PC will reboot now...!!");

		SleepMsg(1000);	// wait for while
#ifdef _DEBUG
		//ForcePCReboot();	// reboot PC => debug mode => just test
#else
		ForcePCReboot();	// reboot PC => Release mode
#endif

	//////////////////////////////////////////////////////////////////////
	// RebootRes
	} else if ( Type == MSG_TYPE_RebootRes ) {	// Reboot response
		if ( FormConfig != NULL ) {
			FormConfig->DataLoggerMessage(pData, Len);
		} else {
			LogMessage(CbFileWrite, !CbDBWrite, "RebootRes received. but SetupForm is not ready..");
		}

	//////////////////////////////////////////////////////////////////////
	// Unknown Message
	} else {
		LogMessage(CbFileWrite, !CbDBWrite, "[ProcessLoggerMessage] Unknown message");
	}

	return true;
}

//---------------------------------------------------------------------------
// Check Database Connection by query ( Query Time ) 2022.04.27
void TfrmMain::CheckDBConnection()
{
	static int iDBFailCount = 0;

	// DBCheckTimeStamp Check -----------------------------------------------

	long Second = SecondsBetween(Now(), m_DBCheckTimeStamp);
	if ( Second >= 180 ) {	// 만약 3분 이상 TimeStamp가 차이가 나면
		LogMessage(CbFileWrite, !CbDBWrite, "DB health timestamp is over.. send RESET message to WatchDog.");
		BYTE Action = ACTION_SHUTDOWN_REQUEST;
		SendWatchDogMessage(Action);  		// Send Shutdown Message to WatchDog
		iDBFailCount = 0;
		m_DBCheckTimeStamp = Now();	// DBCheckTimeStamp refresh..만약 WatchDog이 연결되지 않아서 Reset되지 않으면
									// 계속 Reset Message를 전송하는 것을 방지하기 위해서 이렇게 함.
	}

	// Query DateTime from Database
	// if occur exception, increment iDBFailCount
	// if iDBFailCount >= 2, Send Shutdown message to WatchDog

	UnicodeString Err = "";
	TDateTime DatabaseDateTime;

	Err = GetDatabaseDateTime(frmMain->SQLConnection, DatabaseDateTime);

	if ( !Err.IsEmpty() ) {	// DB Connection has problem..
		iDBFailCount++;
		if ( iDBFailCount >= 2 ) {
			LogMessage(CbFileWrite, !CbDBWrite, "DB Connection has problem.. over threshold..");
			BYTE Action = ACTION_SHUTDOWN_REQUEST;
			SendWatchDogMessage(Action);  		// Send Shutdown Message to WatchDog
			iDBFailCount = 0;
		} else {
			LogMessage(CbFileWrite, !CbDBWrite, "DB Connection has problem.. not reach threshold..");
		}
	} else {
		iDBFailCount = 0;
		LogMessage(CbFileWrite, !CbDBWrite, "DB Connection is health..");
		m_DBCheckTimeStamp = Now();	// DBCheckTimeStamp 갱신
	}
}

//---------------------------------------------------------------------------
//
// Window Message Process ( User message )
//	=> 모든 메세지는 여기서 처리, 모든 Logic의 흐름은 이곳에서 분기한다.
//
//---------------------------------------------------------------------------
void TfrmMain::OnMessage(TMessage &Message)
{
	//Application->ProcessMessages();	// Message pumpping => 혹시 처리못한 message가 있다면 처리

	bool bResult = false;

	int Job = (int)Message.WParam;

	switch ( Job ) {
		// ---------------------------------------------------------------------
		// PAST_FILE_CHECK_JOB ( 1, 5, 7, 9 Minute => PAST FILE CHECK JOB ) 3분은 REAL Data 전송용으로 쉰다.
		// ---------------------------------------------------------------------
		case PAST_FILE_CHECK_JOB :
			{
				tmrCheckDBForFTPTimer(NULL);
			}
			break;

		case DB_CONNECTION_CHECK :	// DB Connection Check Job ( every 30 second 2022.04.27
			{
				CheckDBConnection();
			}
			break;
//		case GARBAGE_COLLECTION : // Garbage Collection
//			{
//				MemoryGarbageCollection();
//				LogMessage(CbFileWrite, !CbDBWrite, "Garbage Collection..ok");
//			}
//			break;
	}
}

//void __fastcall TfrmMain::SFTPClientKeyValidate(TObject *Sender, TElSSHKey *ServerKey,
//          bool &Validate)
//{
//	Validate = true;
//}
////---------------------------------------------------------------------------
//
//void __fastcall TfrmMain::SFTPClient2KeyValidate(TObject *Sender, TElSSHKey *ServerKey,
//          bool &Validate)
//{
//	Validate = true;
//}
////---------------------------------------------------------------------------

void __fastcall TfrmMain::Button1Click(TObject *Sender)
{
	// Test code
	UnicodeString sMeasureDate = "20260312";
	UnicodeString sMeasureTime = "100000";
	//UnicodeString sMeasureDate = "20241004";
	//Unicodtring sMeasureTime = "084000";

	SearchDBMakeFtpFiles(sMeasureDate, sMeasureTime); //DB를 쿼리하여 ftp파일이 생성되지않은 자료가 있으면 생성

	if (DBEnvInfo.bServerEnable ||
		DBEnvInfo.bRnseaEnable) SendFtpFiles(false); //DB에서 ftp로 전송되지않은 자료 쿼리를 쿼리하여 ftp output큐에 전송

//	static int iDBFailCount = 0;
//	UnicodeString Err = "";
//	TDateTime DatabaseDateTime;
//
//	Err = GetDatabaseDateTime(frmMain->SQLConnection, DatabaseDateTime);
//
//	if ( !Err.IsEmpty() ) {	// DB Connection has problem..
//		iDBFailCount++;
//		if ( iDBFailCount >= 2 ) {
//			LogMessage(CbFileWrite, !CbDBWrite, "DB Connection has problem.. over threshold..");
//			BYTE Action = ACTION_SHUTDOWN_REQUEST;
//			SendWatchDogMessage(Action);  		// Send Shutdown Message to WatchDog
//			iDBFailCount = 0;
//		} else {
//			LogMessage(CbFileWrite, !CbDBWrite, "DB Connection has problem.. not reach threshold..");
//		}
//	} else {
//		iDBFailCount = 0;
//		LogMessage(CbFileWrite, !CbDBWrite, "DB Connection is health..");
//	}
}
//---------------------------------------------------------------------------

