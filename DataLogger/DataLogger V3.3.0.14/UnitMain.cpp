//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitMain.h"

#include "HydroMeter.h"
#include "ChannelMaster.h"
#include "AG1500K.h"
#include "SL500K.h"
#include "Accusonic.h"
#include "RQ30.h"
#include "RQ30d.h"
#include "./HydroMeter/EWSVConverter.h"
#include "WaterLevel.h"
#include "LT500.h"
#include "LT400.h"
#include "AT200.h"
#include "OTTCBS.h"
#include "RnsWater.h"
//#include "Atmos.h"
#include "Airmar.h"
#include "FlowStream.h"
#include "RPoint.h"
#include "AniThread.h"
#include "LogThread.h"
#include "UnitSetup.h"
#include "./Database/RLoggerLog.h"

#include "./HydroSetup/UnitHydroSetup.h"
#include "./HydroSetup/UnitHydroSetupCM.h"
#include "./HydroSetup/UnitHydroSetupAG.h"
#include "./HydroSetup/UnitHydroSetupSL.h"


//#include "./Database/RFlowIVM.h"
//#include "./Database/RFlowHA.h"
//#include "./Database/RFlowADVM.h"

#include <typeinfo.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdPort"
#pragma link "OoMisc"
#pragma link "OoMisc"
#pragma link "OoMisc"
#pragma link "OoMisc"
#pragma resource "*.dfm"

#include <psapi.h>
#pragma comment(lib, "psapi")

TFormMain *FormMain;

TCriticalSection *pGLogSection = new TCriticalSection(); 	// LogMessage Critical Section
TCriticalSection *pGMasterPortSection = new TCriticalSection(); 	// MasterPort  Critical Section
TCriticalSection *pGSlavePortSection  = new TCriticalSection(); 	// SlavePort  Critical Section

TQueue *GLogQueue = new TQueue();	// global serail queue
TEvent *GLogEvent = new TEvent(NULL, false, false, NULL, false);	// global log event
TEvent *GAniEvent = new TEvent(NULL, false, false, NULL, false);	// global Ani event

//---------------------------------------------------------------------------
SIZE_T GetProcessWorkingSetSize()
//현재의 메모리 사용량 리턴
{
	SIZE_T ulResult = 0;

	DWORD ProcessID = GetCurrentProcessId();
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) ulResult = pmc.WorkingSetSize / 1024;

	CloseHandle(hProcess);
	return ulResult;
}

//---------------------------------------------------------------------------
BYTE MakeCRC(BYTE *pBuff, int iLen)
{
	BYTE crc = 0x00;
	for ( int i = 0 ; i < iLen ; i++ )
		crc ^= *(pBuff + i);

	return crc & 0x7F;
}

//---------------------------------------------------------------------------
int  CheckFrame(BYTE *pBuff, int iLen)
{
	if ( *(pBuff+0) != STX )      return -1;	// STX Error
	if ( *(pBuff+(iLen-1)) != ETX ) return -2;	// ETX Error

	BYTE crc = 0x00;
	for ( int i = 0 ; i < iLen-2 ; i++ )
		crc ^= *(pBuff + i);

	crc = crc & 0x7F;

	if ( *(pBuff+(iLen-2)) != crc ) return -3;	// CRC Error

	return 0;
}

int  GetTimeSpanBySecond(TDateTime SpanTime)	// 시간의 경과를 초로 환산한다.
{
	USHORT year, month, day;
	USHORT hour, minute, sec, msec;

	SpanTime.DecodeDate(&year, &month, &day);
	SpanTime.DecodeTime(&hour, &minute, &sec, &msec);

	int iTotalSecond = 0;

	iTotalSecond = hour * 60 * 60 + minute * 60 + sec;

	return iTotalSecond;
}

//---------------------------------------------------------------------------
//
// TFormMain
//
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
	m_enStep      = enIdle;	// default

	m_CurrentDir  = "";		// Default directory

	m_StartTime   = 0;
	m_MeasureDate = "";     // MeasureDate/time => PK in Database
	m_MeasureTime = "";

	memset(m_PreWaterLevel,  0x00, sizeof(m_PreWaterLevel));
	memset(m_PreAtmos,       0x00, sizeof(m_PreAtmos));
	memset(m_PreAirmar,      0x00, sizeof(m_PreAirmar));
	memset(m_PreHydroMeter1, 0x00, sizeof(m_PreHydroMeter1));
	memset(m_PreHydroMeter2, 0x00, sizeof(m_PreHydroMeter2));
	memset(m_PreHydroMeter3, 0x00, sizeof(m_PreHydroMeter3));
	memset(m_ViewerBuffer,   0x00, sizeof(m_ViewerBuffer));
	m_RecvIndex = 0;
	memset(m_SenderBuffer, 0x00, sizeof(m_SenderBuffer));
	m_SenderIndex = 0;

	m_FormSetup = NULL;

	m_bExit  = false;
	m_iDBErrorCnt = 0;

	m_pHydroMeter1 = NULL;
	m_pHydroMeter2 = NULL;
	m_pHydroMeter3 = NULL;
	m_pWaterLevel  = NULL;
	m_pAtmos       = NULL;
	m_pAirmar      = NULL;
	m_pFlowStream  = NULL;

	m_bHydroBreak  = false;	// 유속계에 Break를 보냈는 지 여부(Logging 시작할 때 false로 만든다.) SL/AG에만 해당
	m_iRemainBreak = 0;		// BREAK보내기까지 남은 시간(초)

	m_pAniThread   = NULL;
	m_bAniThread   = false;

	m_pLogThread   = NULL;		// log Thread
	m_bLogThread   = false;

	m_iViewerCount = 0;

	m_pHydroSetup1 = NULL;
	m_pHydroSetup2 = NULL;
	m_pHydroSetup3 = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormActivate(TObject *Sender)
{
	static bool bFirst = false;

	if ( bFirst ) return;	// 최초 한 번만 실행하고 그 다음 이벤트일 때는 아래 스크립트를 발생하지 않는다.

#ifdef _DEBUG
	OutputDebugString(L"FormActivate Event..\r\n");
	ButtonTest->Visible = true;		// Debug mode => Show
#else
	ButtonTest->Visible = false;	// Release mode => Hide
#endif

	// 2014.06.19 Unhandled Exception 시 O/S 레벨의 디버그 창 표시를 Disable 시킴
	::SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS );

	this->Height = 140;

	bool bResult = false;

	UnicodeString Version = VersionInfo("FileVersion");
	lbVersion->Caption = Version;

	m_CurrentDir = GetCurrentDir();

	bResult = theEnv.LoadINI(m_CurrentDir + "\\DataLogger.ini");
	if ( !bResult ) {
		m_bExit = true;
		Close();
	}
								// 127.0.0.1:D:\RADS\Database\RADS.FDB, rads, rads2014
	bResult = DM->Open(theEnv.m_DBIP + ":" + theEnv.m_DBName, "",  theEnv.m_Account, "rads2014");
	if ( !bResult ) {
		m_bExit = true;
		Close();
	}

	bResult = theEnv.LoadEnv();
	if ( !bResult ) return;

	// Log ---------------------------------------------------------------------
	theLog.SetHomeDir(theEnv.m_DataLoggerHome + "\\Log");

	// Log Thread --------------------------------------------------------------
	m_pLogThread = new TLogThread(true);	// supended
	m_pLogThread->InitThread(theEnv.m_DataLoggerHome + "\\Log");
	m_bLogThread = true;
	m_pLogThread->Start();	// Start Thread

	// TrayTimer Enable
	TimerTray->Interval = 1000;	// 1sec
	TimerTray->Enabled = true;

	// InitLogger Start --------------------------------------------------------

	InitLogger();

	// InitLogger Finish -------------------------------------------------------

	ShowSetupInfo();	// Show Setup information

	SetWorkingIcon(EN_LOGGER_STEP::enIdle); 	// Working  icon init

	LogMessage("DataLogger Started.......................................................................", bGCSFileLog, bGCSDBLog);

	bFirst = true;	// 한번만 실행하도록 한다.
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
{
#ifdef _DEBUG
	OutputDebugString(L"FormShow Event..\r\n");
#endif

	LocateRightButton();
}

//---------------------------------------------------------------------------
// Before Close Event
void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
// CloseQuery Event
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
#ifdef _DEBUG

	LogMessage("DataLogger is closed...................................", bGCSFileLog, bGCSDBLog);

	//------------------------------------------------------------------
	//
	// Program Terminate ( Resource Release )
	//
	//------------------------------------------------------------------

	// Timere Stop
	TimerGeneral->Enabled  = false;
	TimerHydro1->Enabled   = false;
	TimerHydro2->Enabled   = false;
	TimerHydro3->Enabled   = false;
	TimerWater->Enabled    = false;
	TimerWatchDog->Enabled = false;
	TimerSender->Enabled   = false;

	// Serial port close
	try {
		HydroPort1->Open   = false;
		HydroPort2->Open   = false;
		HydroPort3->Open   = false;
		WaterPort->Open    = false;
		AtmosPort->Open    = false;
		AirmarPort->Open   = false;
		RQSlavePort->Open  = false;
	} catch (...) {
		// nothing to do..
	}

	if ( m_pHydroMeter1 != NULL ) delete m_pHydroMeter1;
	if ( m_pHydroMeter2 != NULL ) delete m_pHydroMeter2;
	if ( m_pHydroMeter3 != NULL ) delete m_pHydroMeter3;
	if ( m_pWaterLevel  != NULL ) delete m_pWaterLevel;
	if ( m_pAtmos       != NULL ) delete m_pAtmos;
	if ( m_pAirmar      != NULL ) delete m_pAirmar;
	if ( m_pFlowStream  != NULL ) delete m_pFlowStream;

	// Socket Release
	try {
		ServerSocket->Active   = false; 	// for viewer
		SenderSocket->Active   = false;		// for sender
		WatchDogSocket->Active = false;		// for watchdog
	} catch (...) {
		// nothing to do..
	}

	// Message pumping
	for ( int i = 0 ; i < 10 ; i++ ) {
		Application->ProcessMessages();
	}

	if ( m_pAniThread != NULL ) {
		m_bAniThread = false;
		GAniEvent->SetEvent();
		Sleep(100);		// Wait for while )  2025.02.19 200ms => 100ms
		delete m_pAniThread;
	}

	if ( m_pLogThread != NULL ) {
		m_bLogThread = false;
		GLogEvent->SetEvent();	//
		Sleep(100); 	// Wait for while )  2025.02.19 200ms => 100ms
		delete m_pLogThread;
	}

	// RQ-30d class resource destory
	delete MasterSection;		// for Master Port Thread
	delete SlaveSection;		// for Slave Port Thread
	delete MasterQueue;			// for Master Port Thread
	delete SlaveQueue;			// for Slave Port Thread
	delete MasterEvent;			// for Master Port Thread
	delete SlaveEvent;			// fro Slave Port Thread

	// Main Form class resource destory
	delete pGLogSection;
	delete pGMasterPortSection;
	delete pGSlavePortSection;
	delete GLogQueue;
	delete GLogEvent;
	delete GAniEvent;

	Action = caFree;
	return;

#endif

	///////////////////////////////////////////////////////////////////////////
	//
	// Release Mode
	//
	///////////////////////////////////////////////////////////////////////////

	if ( !m_bExit ) {
		Action = caNone;

		GoToSystemTray();

		return;

	} else {

		LogMessage("DataLogger is closed...................................", bGCSFileLog, bGCSDBLog);

		//------------------------------------------------------------------
		//
		// Program Terminate ( Resource Release )
		//
		//------------------------------------------------------------------

		// Timere Stop
		TimerGeneral->Enabled  = false;
		TimerHydro1->Enabled   = false;
		TimerHydro2->Enabled   = false;
		TimerHydro3->Enabled   = false;
		TimerWater->Enabled    = false;
		TimerWatchDog->Enabled = false;
		TimerSender->Enabled   = false;

		// Serial port close
		try {
			HydroPort1->Open   = false;
			HydroPort2->Open   = false;
			HydroPort3->Open   = false;
			WaterPort->Open    = false;
			AtmosPort->Open    = false;
			AirmarPort->Open   = false;
			RQSlavePort->Open  = false;
		} catch (...) {
			// nothing to do..
		}

		if ( m_pHydroMeter1 != NULL ) delete m_pHydroMeter1;
		if ( m_pHydroMeter2 != NULL ) delete m_pHydroMeter2;
		if ( m_pHydroMeter3 != NULL ) delete m_pHydroMeter3;
		if ( m_pWaterLevel  != NULL ) delete m_pWaterLevel;
		if ( m_pAtmos       != NULL ) delete m_pAtmos;
		if ( m_pAirmar      != NULL ) delete m_pAirmar;
		if ( m_pFlowStream  != NULL ) delete m_pFlowStream;

		// Socket Release
		try {
			ServerSocket->Active   = false; 	// for viewer
			SenderSocket->Active   = false;		// for sender
			WatchDogSocket->Active = false;		// for watchdog
		} catch (...) {
			// nothing to do..
		}

		// Message pumping
		for ( int i = 0 ; i < 10 ; i++ ) {
			Application->ProcessMessages();
		}

		if ( m_pAniThread != NULL ) {
			m_bAniThread = false;
			GAniEvent->SetEvent();
			Sleep(100);		// Wait for while )  2025.02.19 200ms => 100ms
			delete m_pAniThread;
		}

		if ( m_pLogThread != NULL ) {
			m_bLogThread = false;
			GLogEvent->SetEvent();	//
			Sleep(100); 	// Wait for while )  2025.02.19 200ms => 100ms
			delete m_pLogThread;
		}

		// RQ-30d class resource destory
		delete MasterSection;		// for Master Port Thread
		delete SlaveSection;		// for Slave Port Thread
		delete MasterQueue;			// for Master Port Thread
		delete SlaveQueue;			// for Slave Port Thread
		delete MasterEvent;			// for Master Port Thread
		delete SlaveEvent;			// fro Slave Port Thread

		// Main Form Resource destory
		delete pGLogSection;
		delete pGMasterPortSection;
		delete pGSlavePortSection;
		delete GLogQueue;
		delete GLogEvent;
		delete GAniEvent;

		Action = caFree;
		return;
	}
}

//---------------------------------------------------------------------------
// Form Destory Event => Close Sequence
void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
//TrayIcon->Visible = false;
}

//---------------------------------------------------------------------------
//▼▲
void __fastcall TFormMain::cbOpenCloseClick(TObject *Sender)
{
	if ( cbOpenClose->Caption == "▲" ) {
		SetUIExtend(true);
//		this->Height = 570;
//		cbOpenClose->Caption = "▼";
//		this->Top = Screen->WorkAreaHeight - this->Height;
	} else {
		SetUIExtend(false);
//		this->Height = 140;
//		cbOpenClose->Caption = "▲";
//		this->Top = Screen->WorkAreaHeight - this->Height;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerGeneralTimer(TObject *Sender)
{
	TDateTime now = Now();

	UnicodeString sNow = now.FormatString("hh:nn:ss");
	lbTime->Caption = sNow;

	USHORT hour, minute, sec, msec;
	now.DecodeTime(&hour, &minute, &sec, &msec);

	///////////////////////////////////////////////////////////////////////
	// Logger is not working
	///////////////////////////////////////////////////////////////////////

	if ( !IsWorking() ) {	// Logger 작업 중이 아니면

		if ( IsStartTime(now) ) {

			lbPTime->Caption = "00:00";
			::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enStart, 0);

		} else {
			// idle time => wait for FIRE Time
			// Logger가 다음 실행 시간을 위해 쉬고 있는 시간
			int iRemainSec = theEnv.GetRemainIdleTime(now);
			UnicodeString sRemainTime;
			sRemainTime = sRemainTime.sprintf(L"-%02d:%02d", iRemainSec / 60 , iRemainSec % 60);
			lbPTime->Caption = sRemainTime;

//			if ( sec == 0 ) {	// 매 분 0초이면 => RQ-30/RQ-30d 측정 시작
//				if (m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TRQ30) || typeid(*m_pHydroMeter1) == typeid(TRQ30D))) {
//					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Start, 0);
//				}
//			}
//
//			if ( sec == 35 ) {	// 매문 35초이면 => RQ-30/RQ-30d 측정 종료
//				if (m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TRQ30) || typeid(*m_pHydroMeter1) == typeid(TRQ30D))) {
//					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Finish, 0);
//				}
//			}
		}

	///////////////////////////////////////////////////////////////////////
	// Logger is working
	///////////////////////////////////////////////////////////////////////

	} else {			// Logger 작업 중

		TDateTime SpanTime = now - m_StartTime;
		int iSpentTime = GetTimeSpanBySecond(SpanTime);

		ProgressBar->Position = CalProgress(iSpentTime);	// 0% ~ 100%
		lbPTime->Caption = SpentTimeToCaption(iSpentTime); // 00:00 ~ 08:30
		// Animation 처리 => 나중.

		// 2022.02.02 OTT WaterLevel 센서 가끔 데이터 깨지는 증상에 대한 처리(8분10초까지 끝나지 않으면 강제로 종료한다.)
		if ((iSpentTime >= 490) && (m_pWaterLevel != NULL) && (typeid(*m_pWaterLevel) == typeid(TWaterLevel)) &&
			((m_pWaterLevel->m_enStep == enSensorStart || m_pWaterLevel->m_enStep == enSensorWorking))) {
			::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);  // 강제로 OTT Level Sensor를 종료한다.
		}

		// 동작 Expire Time 설정
		if ( (m_enStep > enIdle) && (m_enStep < enFinish) && (IsExpireTime(iSpentTime)) ) {	// 센서에서 차체적으로 끝내지 못하고 ExpireTime에 다가 왔을 때
			::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enFinish, 0);
		} else {
			// Logger Working
		}

		////////////////////////////////////////////////////////////////////////
		// RQ-30d 측정 Start/Finish
		////////////////////////////////////////////////////////////////////////

		// 2026.02.12 xx:x5:00 측정 종료, xx:x9:00 측정 시작
		if (((minute % 10) == 9) && (sec == 0 || sec == 1)) { // 측정시작은 onStart에서 시작됨.
			if (m_pHydroMeter1 != NULL &&  (typeid(*m_pHydroMeter1) == typeid(TRQ30D))) {
				if ( m_pHydroMeter1->m_enStep == enSensorStart || m_pHydroMeter1->m_enStep == enSensorWorking ) {	// 동작중일 때만
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Start, 0);
				} else {
					// Sensor idle
				}
			}
		}

		// 2026.02.12 xx:x8:30 (1번재 측정 종료), xx:x2:30 (2번째 측정 종료)
		if  ((((minute % 10) == 8) && (sec == 30 || sec == 31)) || (((minute % 10) == 2) && (sec == 30 || sec == 31))) {
			if (m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TRQ30D))) {
				if ( m_pHydroMeter1->m_enStep == enSensorStart || m_pHydroMeter1->m_enStep == enSensorWorking ) {	// 동작중일 때만
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Finish, 0);
				} else {
					// Sensor Idle
				}
			}
		}

		////////////////////////////////////////////////////////////////////////
		// EWSV Measure Start/Finish
		////////////////////////////////////////////////////////////////////////
		// EWSV Action : 데이터 요구 : xx:x8:50, xx:x2:50
		if ((((minute % 10) == 8) && (sec == 50 || sec == 51)) || (((minute % 10) == 2) && (sec == 50 || sec == 51))) {
			if (m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TEWSVConverter))) {
				::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enEWSVFinish, 0);
			}
		}

		// EWSV Action : 측적 시작 : xx:x5:00(OnHydroMeterStart에서 처리), xx:x9:00 => PostMessage
		if (((minute % 10) == 9) && (sec == 0 || sec == 1)) {	// 두번째 측정 시작을 보낸다 ( 첫번째는 HydroMeter.Start에서 시작
			if (m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TEWSVConverter))) {
				::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enEWSVStart, 0);
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::Open1Click(TObject *Sender)
{
	RestoreFromTray();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::Close1Click(TObject *Sender)
{
	m_bExit = true;
	Close();
}

//---------------------------------------------------------------------------
bool TFormMain::GoToSystemTray()
{
	// Hide the window and set its state variable to wsMinimized.
	Hide();
	WindowState = wsMinimized;

	// Show the animated tray icon and also a hint balloon.
	TrayIcon->Visible = true;
	//TrayIcon1->Animate = true;
	//TrayIcon1->ShowBalloonHint();
	return true;
}

//---------------------------------------------------------------------------
bool TFormMain::RestoreFromTray()
{
	// Hide the tray icon and show the window,
	// setting its state property to wsNormal.
	TrayIcon->Visible = false;
	Show();
	WindowState = wsNormal;
	Application->BringToFront();

	LocateRightButton();

	return true;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerTrayTimer(TObject *Sender)
{
	TimerTray->Enabled = false;
	GoToSystemTray();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
//	m_CurrentDir = GetCurrentDir();
//
//	bResult = theEnv.LoadINI(m_CurrentDir + "\\DataLogger.ini");
//	if ( !bResult ) return;		//
//
//								// 127.0.0.1:D:\RADS\Database\RADS.FDB, rads, rads2014
//	bResult = DM->Open(theEnv.m_DBIP + ":" + theEnv.m_DBName, "",  theEnv.m_Account, "rads2014");
//	if ( !bResult ) {
//
//		m_pLogThread = new TLogThread(true);	// supended
//		m_pLogThread->InitThread("D:\\RADS\\DataSender\\Log");
//		m_bLogThread = true;
//		m_pLogThread->Start();	// Start Thread
//
//		LogMessage("Database connection error. DataLogger is shutdown..");
//
//		::PostMessage(this->Handle, UM_CLOSE, 0, 0);
//	}
}

//---------------------------------------------------------------------------
bool TFormMain::LocateRightButton()
{
	Forms::TMonitor *pMon = NULL;

	for ( int i = 0 ; i < Screen->MonitorCount ; i++ ) {
		Forms::TMonitor *m = Screen->Monitors[i];
		if ( m->Primary == true ) {
			pMon = m;
			break;
		}
	}

	if ( pMon == NULL ) return false;

	this->Left = Screen->WorkAreaWidth - this->Width;
	this->Top  = Screen->WorkAreaHeight - this->Height;

	return true;
}

//---------------------------------------------------------------------------
// WaterLevel Timer ( 20 Second )
void __fastcall TFormMain::TimerWaterTimer(TObject *Sender)
{
	if ( m_pWaterLevel == NULL )  return;

	TDateTime now = Now();
	TDateTime Span = now - m_StartTime;

	USHORT hour, minute, sec, msec;
	Span.DecodeTime(&hour, &minute, &sec, &msec);	// 얼마나 경과 되었나?
	int SpanSec = minute * 60 + sec ;


	// 07:55 ~ 08:05 => WaterLevel Stop
	if ( SpanSec > theEnv.GetWorkingTime() - 30 - 5) {	// 7분 55초 이상이면 Water Timer을 종료한다.

		TimerWater->Enabled = false;
		::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);

	} else {	// 측정 중

		// 10초 타이머 끝날 때까지 안 끝나면 강제로 끝낸다.
		if ( m_pWaterLevel->m_enStep == enSensorStart ||
			 m_pWaterLevel->m_enStep == enSensorWorking ) {
			// 강제로 끝낸다. 별다른 작업 없음
			// 그냥 다시 시작한다.
		}
		m_pWaterLevel->Start(m_MeasureDate, m_MeasureTime, false);	// 측정 중에 다시 시작함.
	}
}

//---------------------------------------------------------------------------
// 하층유속계 : SL/AG 유속계의 경우 유속계의 Timer에서 Decoding 로직을 호출한다.
void __fastcall TFormMain::TimerHydro1Timer(TObject *Sender)
{
	TimerHydro1->Enabled = false;	// 유속계 읽기 작업에 대한 Timer이므로 종료한다.

	// 예외 상황 처리

	if ( m_pHydroMeter1 == NULL ) return;

	if ( m_pHydroMeter1->GetStatus() == enSensorIdle ) return;

	if ( typeid(*m_pHydroMeter1) != typeid(TSL500K) && typeid(*m_pHydroMeter1) != typeid(TAG1500K) &&
		 typeid(*m_pHydroMeter1) != typeid(TRQ30D) ) return;

	// only SL/AG device => decode
	if ( typeid(*m_pHydroMeter1) == typeid(TSL500K) || typeid(*m_pHydroMeter1) == typeid(TAG1500K) ) {
		m_pHydroMeter1->Decode(m_pHydroMeter1->m_RecvMsg, m_pHydroMeter1->m_RecvMsg.Length());
	} else if (typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) { // only RQ-30/RQ-30d => FinishRQ() // 1단위 측정 종료
		// Only SL/AG에 대한 decode 호출
		((TRQ30D *)m_pHydroMeter1)->FinishForceRQ();
	} else {
		// nothing to do..
	}
}

//---------------------------------------------------------------------------
// 상층유속계 : SL/AG 유속계의 경우 유속계의 Timer에서 Decoding 로직을 호출한다.
void __fastcall TFormMain::TimerHydro2Timer(TObject *Sender)
{
	TimerHydro2->Enabled = false;	// 유속계 읽기 작업에 대한 Timer이므로 종료한다.

	// 예외 상황 처리

	if ( m_pHydroMeter2 == NULL ) return;
	if ( m_pHydroMeter2->GetStatus() == enSensorIdle ) return;
	if ( typeid(*m_pHydroMeter2) != typeid(TSL500K) && typeid(*m_pHydroMeter2) != typeid(TAG1500K) ) return;

	// Only SL/AG에 대한 decode 호출

	m_pHydroMeter2->Decode(m_pHydroMeter2->m_RecvMsg, m_pHydroMeter2->m_RecvMsg.Length());
}

//---------------------------------------------------------------------------
// 3번 유속계 Timer : ChannelMaster, Sontek 장비만 유효
void __fastcall TFormMain::TimerHydro3Timer(TObject *Sender)
{
	TimerHydro3->Enabled = false;	// 유속계 읽기 작업에 대한 Timer이므로 종료한다.

	// 예외 상황 처리

	if ( m_pHydroMeter3 == NULL ) return;
	if ( m_pHydroMeter3->GetStatus() == enSensorIdle ) return;
	if ( typeid(*m_pHydroMeter3) != typeid(TSL500K) && typeid(*m_pHydroMeter2) != typeid(TAG1500K) ) return;

	// Only SL/AG에 대한 decode 호출

	m_pHydroMeter3->Decode(m_pHydroMeter3->m_RecvMsg, m_pHydroMeter3->m_RecvMsg.Length());
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// WatchDog Timer Event
void __fastcall TFormMain::TimerWatchDogTimer(TObject *Sender)
{
	//heaven2 2014.07.02 Test code for shutdown request code
	//static int iCnt = 0;
	//iCnt++;

	if ( WatchDogSocket->Socket->Connected ) {

		BYTE Action = ACTION_HEARTBEAT;

		////////////////////////////////////////////////////////////////////////
		// DB connection error check => 더미로 DB의 날짜/시간을 Select해 본다.
		////////////////////////////////////////////////////////////////////////

		TDateTime x = 0;
		x = DM->GetCurrentDateTime();  	// Get DB DateTime
		if ( (double)x == 0.0 ) {
			// DB Datatime get error => something wrong
			m_iDBErrorCnt++;
			if ( m_iDBErrorCnt >= 6 ) {		// it take 3 minutes..
				Action = ACTION_SHUTDOWN_REQUEST;
				LogMessage("DB Error excceded limitation. DataLogger request shutown to WatchDog");
				m_iDBErrorCnt = 0;	// clear
			} else {
				// DB Error Counter++
			}
		} else {
			m_iDBErrorCnt = 0;	// DB normal
		}

		////////////////////////////////////////////////////////////////////////
		// Memory Check
		////////////////////////////////////////////////////////////////////////

		if ( Action == ACTION_HEARTBEAT ) {

			SIZE_T Size = GetProcessWorkingSetSize();

			//if ( Size > 50 * 1024 || iCnt > 10 ) { // Working Memory Size : 50 Mbytes를 초과할 때 => WatchDog에 Shutdown Request를 전송한다.
			if ( Size > 50 * 1024 ) { // Working Memory Size : 50 Mbytes를 초과할 때 => WatchDog에 Shutdown Request를 전송한다.
				Action = ACTION_SHUTDOWN_REQUEST;
				UnicodeString sMsg;
				sMsg = sMsg.sprintf(L"DatLogger memory excceded limitation(%d MBytes). DataLogger request shutdown to WatchDog..", Size);
				LogMessage(sMsg);
			}
		}

		////////////////////////////////////////////////////////////////////////
		// TimeStamp Check
		////////////////////////////////////////////////////////////////////////

		if ( Action == ACTION_HEARTBEAT ) {

			TDateTime now = Now();
			TDateTime TimeSpan;
			TDateTime Threshold = TDateTime(3,0,0,0);  	// 3시간 이상 차이나면 Reset하도록 함.

			if ( m_pWaterLevel != NULL ) {
				if ( theEnv.m_bIdleCheckWater ) {
					Threshold = TDateTime(theEnv.m_iIdleCheckWaterTime, 0,0,0);
					TimeSpan = now - m_pWaterLevel->m_TimeStamp;
					if ( TimeSpan > Threshold ) {	// 3시간 이상 시간이 벌어지면
						Action = ACTION_SHUTDOWN_REQUEST;
						LogMessage("WaterLevel Sensor is not working above 3 hours. DataLogger request shutdown to WatchDog..");
					}
				} else {
					// idle check off
					// nothing to do
				}
			}

			if ( Action == ACTION_HEARTBEAT && m_pHydroMeter1 != NULL ) {
				if ( theEnv.m_bIdleCheck1 ) {
					Threshold = TDateTime(theEnv.m_iIdleCheckTime1, 0,0,0);
					TimeSpan = now - m_pHydroMeter1->m_TimeStamp;
					if ( TimeSpan > Threshold ) {	// 3시간 이상 시간이 벌어지면
						Action = ACTION_SHUTDOWN_REQUEST;
						LogMessage("HydroMeter1 Sensor is not working above 3 hours. DataLogger request shutdown to WatchDog..");
					}
				} else {
					// idle check off
					// nothing to do
				}
			}

			if ( Action == ACTION_HEARTBEAT && m_pHydroMeter2 != NULL ) {
				if ( theEnv.m_bIdleCheck2 ) {
					Threshold = TDateTime(theEnv.m_iIdleCheckTime2, 0,0,0);
					TimeSpan = now - m_pHydroMeter2->m_TimeStamp;
					if ( TimeSpan > Threshold ) {	// 3시간 이상 시간이 벌어지면
						Action = ACTION_SHUTDOWN_REQUEST;
						LogMessage("HydroMeter2 Sensor is not working above 3 hours. DataLogger request shutdown to WatchDog..");
					}
				} else {
					// idle check off
					// nothing to do
				}
			}
		}

		SendHeartBeatToWatchDog(Action);

	} else {	// Retry to connect Watchdog
		try {
			//WatchDogSocket->Socket->Close();
			//Sleep(100);
			WatchDogSocket->Active = true;
			LogMessage("Try to connect to WatchDog..");
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerSenderTimer(TObject *Sender)
{
	if ( SenderSocket->Socket->Connected ) {

		SendLoggerStatusToSender(m_enStep);

		//LogMessage("Send Logger Event to DataSender..");

	} else {	// Retry to connect Watchdog
		try {
			SenderSocket->Active = false;
			Sleep(50);
			SenderSocket->Active = true;
			LogMessage("Try to connect to DataSender..");
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerViewerTimer(TObject *Sender)
{
	SendLoggerStatusToViewer(m_enStep);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TrayIconDblClick(TObject *Sender)
{
	RestoreFromTray();
}

//---------------------------------------------------------------------------
void TFormMain::SetSenderSocketIcon(EN_SOCKET_ACTION enSocketAction)
{
	int iIndex = 0;
	if ( enSocketAction == enSocketConnected ) iIndex = 3;
	else                                       iIndex = 2;

	TBitmap *pBitmap = new TBitmap();
	ImageListSocket->GetBitmap(iIndex,pBitmap);
	ImageSender->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TFormMain::SetWatchDogSocketIcon(EN_SOCKET_ACTION enSocketAction)
{
	int iIndex = 0;
	if ( enSocketAction == enSocketConnected ) iIndex = 5;
	else                                       iIndex = 4;

	TBitmap *pBitmap = new TBitmap();
	ImageListSocket->GetBitmap(iIndex,pBitmap);
	ImageWatchDog->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TFormMain::SetViewerSocketIcon(EN_SOCKET_ACTION enSocketAction)
{
	int iIndex = 0;
	//int iCount = ServerSocket->Socket->ActiveConnections;
	int iCount = m_iViewerCount;

	if ( iCount == 0 ) iIndex = 6;
	else               iIndex = 8 + ( (iCount-1) * 2 ) + 1;	// 6,7,8,9(1),10,11(2),12,13(3),14,15(4),16,17(5)

	if ( iIndex > 17 ) iIndex = 17;	// 너무 많이 붙으면 5로 고정함.

	TBitmap *pBitmap = new TBitmap();
	ImageListSocket->GetBitmap(iIndex,pBitmap);
	ImageViewer->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TFormMain::SetWorkingIcon(EN_LOGGER_STEP enStep)
{
	if ( enStep == enIdle ) {

		// FlowStream -------------------------------------------------------

		if ( m_pFlowStream != NULL ) {
			TBitmap *pBitmap = new TBitmap();
			ImageListFlow->GetBitmap(10,pBitmap);  	// normal status
			ImageFlow->Picture->Bitmap = pBitmap;
			delete pBitmap;
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListFlow->GetBitmap(9,pBitmap);	// siable status
			ImageFlow->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}

		// HydroMeter1 ------------------------------------------------------

		if ( m_pHydroMeter1 != NULL ) {
			if ( m_pHydroMeter1->HydroKind == CHANNELMASTER_STR ) {
				TBitmap *pBitmap = new TBitmap();
				ImageListCM->GetBitmap(12,pBitmap);  	// normal status
				ImageHydro1->Picture->Bitmap = pBitmap;
				delete pBitmap;
			} else {
				TBitmap *pBitmap = new TBitmap();
				ImageListSONTEK->GetBitmap(12,pBitmap);  	// normal status
				ImageHydro1->Picture->Bitmap = pBitmap;
				delete pBitmap;
			}
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListCM->GetBitmap(11,pBitmap);	// diable status
			ImageHydro1->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}

		// HydroMeter2 ------------------------------------------------------

		if ( m_pHydroMeter2 != NULL ) {
			if ( m_pHydroMeter2->HydroKind == CHANNELMASTER_STR ) {
				TBitmap *pBitmap = new TBitmap();
				ImageListCM->GetBitmap(12,pBitmap);  	// normal status
				ImageHydro2->Picture->Bitmap = pBitmap;
				delete pBitmap;
			} else {
				TBitmap *pBitmap = new TBitmap();
				ImageListSONTEK->GetBitmap(12,pBitmap);  	// normal status
				ImageHydro2->Picture->Bitmap = pBitmap;
				delete pBitmap;
			}
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListCM->GetBitmap(11,pBitmap);	// diable status
			ImageHydro2->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}

		// WaterLevel -------------------------------------------------------

		if ( m_pWaterLevel != NULL ) {
			TBitmap *pBitmap = new TBitmap();
			ImageListWaterLevel->GetBitmap(5,pBitmap);  	// normal status
			ImageWaterLevel->Picture->Bitmap = pBitmap;
			delete pBitmap;
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListWaterLevel->GetBitmap(4,pBitmap);	// siable status
			ImageWaterLevel->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}
	}
}

//---------------------------------------------------------------------------
void TFormMain::SetUIExtend(bool bEnlarge)
{
	if ( bEnlarge ) {
		this->Height = 570;
		cbOpenClose->Caption = "▼";
		this->Top = Screen->WorkAreaHeight - this->Height;
	} else {
		this->Height = 140;
		cbOpenClose->Caption = "▲";
		this->Top = Screen->WorkAreaHeight - this->Height;
	}
}

//---------------------------------------------------------------------------
void TFormMain::LogMessage(UnicodeString sMsg, bool bFile, bool bDB)
{
	// File, UI Log
	STLogMessage *pLogMessage = new STLogMessage();

	pLogMessage->LogTime = Now();
	pLogMessage->LogMessage = Trim(sMsg);
	pLogMessage->bFile = bFile;

	pGLogSection->Enter();
	GLogQueue->Push(pLogMessage);
	pGLogSection->Leave();

	// DB Log

	if ( bDB ) {
		TLoggerLog LogMessage;
		LogMessage.LogDateTime = Now();
		LogMessage.LogMessage  = sMsg;

		LogMessage.Insert();
	}

//	if ( Memo->Lines->Count > 100 ) Memo->Lines->Clear();
//
//	TDateTime t = Now();
//	//USHORT iYear, iMonth, iDay;
//	USHORT iHour, iMinute, iSec, iMSec;
//
//	//t.DecodeDate(&iYear, &iMonth, &iDay);
//	t.DecodeTime(&iHour, &iMinute, &iSec, &iMSec);
//
//	AnsiString sLogMsg;
//	sLogMsg = sLogMsg.sprintf("[%02d:%02d:%02d] ",iHour, iMinute, iSec);
//	sLogMsg += sMsg;
//
//	Memo->Lines->Insert(0, sLogMsg);
//
//	if ( bFile ) {
//		theLog.LogMessage(sLogMsg);
//	}

}

//---------------------------------------------------------------------------
//
// Window Message Process ( User message )
//	=> 모든 메세지는 여기서 처리, 모든 Logic의 흐름은 이곳에서 분기한다.
//
//---------------------------------------------------------------------------
void TFormMain::OnMessage(TMessage &Message)
{
	//Application->ProcessMessages();	// Message pumpping => 혹시 처리못한 message가 있다면 처리

	bool bResult = false;

	EN_LOGGER_STEP enStep = (EN_LOGGER_STEP)Message.WParam;

	UnicodeString MeasureDate, MeasureTime;

	switch ( enStep ) {
		// ---------------------------------------------------------------------
		// Logger Idle
		// ---------------------------------------------------------------------
		case enIdle :
			{
				m_enStep      = enIdle;
				m_MeasureDate = "";
				m_MeasureTime = "";
				m_RPoint.Clear();

				ProgressBar->Position = 0;
				lbPTime->Caption      = "00:00";

				if ( m_pWaterLevel != NULL )  m_pWaterLevel->Clear();
				if ( m_pHydroMeter1 != NULL ) m_pHydroMeter1->Clear();
				if ( m_pHydroMeter2 != NULL ) m_pHydroMeter2->Clear();
				if ( m_pHydroMeter3 != NULL ) m_pHydroMeter3->Clear();

				SetWorkingIcon(EN_LOGGER_STEP::enIdle);	// working icon init

				LogMessage("Logger is idle status..");
			}
			break;
		// ---------------------------------------------------------------------
		// Logger Start
		// ---------------------------------------------------------------------
		case enStart :
			{
				if ( m_enStep >= enStart ) break;	// 뭔가 잘못된 상태 => 이미 시작 중인데 Start가 옴.

				if ( m_bHydroBreak || TimerBREAK->Enabled == true) { 	// SendBreak가 걸려있으면 이를 해제한다.
					m_bHydroBreak  = false;
					m_iRemainBreak = 0;
					TimerBREAK->Enabled = false;

					// HydroMeter1
					if (  m_pHydroMeter1 != NULL &&
						  (typeid(*m_pHydroMeter1) == typeid(TChannelMaster) || typeid(*m_pHydroMeter1) == typeid(TSL500K) || typeid(*m_pHydroMeter1) == typeid(TAG1500K)) &&
						  m_pHydroMeter1->GetBreakStatus() ) {
						// 만약 BREAK 상황이면 SL/AG는 START 명령으로 센서를 시작한다.
						if ((typeid(*m_pHydroMeter1) == typeid(TSL500K) || typeid(*m_pHydroMeter1) == typeid(TAG1500K)) && m_pHydroMeter1->GetBreakStatus()) {
							if ( m_pHydroMeter1->m_bBreakResponse ) {	// 이미 <BREAK>에 대한 Ack를 못 받은 상황
								AnsiString sStart = "START\r";
								m_pHydroMeter1->Send(sStart.c_str(), sStart.Length());
								LogMessage("SL500 is start with START Command");
							} else {	// <BREAK>에 대한 Ack를 받지 못한 상황
								AnsiString sStart = "START\r";
								m_pHydroMeter1->Send(sStart.c_str(), sStart.Length());
								LogMessage("SL500 BREAK command is not complete. Anyway SL500 is start with START Command");
							}
						}
						m_pHydroMeter1->SetBreak(false);	// BREAK Release
					}

					// HydroMeter2
					if (  m_pHydroMeter2 != NULL &&
						  (typeid(*m_pHydroMeter2) == typeid(TChannelMaster) || typeid(*m_pHydroMeter2) == typeid(TSL500K) || typeid(*m_pHydroMeter2) == typeid(TAG1500K)) &&
						  m_pHydroMeter2->GetBreakStatus() ) {
						if ((typeid(*m_pHydroMeter2) == typeid(TSL500K) || typeid(*m_pHydroMeter2) == typeid(TAG1500K)) && m_pHydroMeter2->GetBreakStatus()) {
							AnsiString sStart = "START\r";
							m_pHydroMeter2->Send(sStart.c_str(), sStart.Length());
						}
						m_pHydroMeter2->SetBreak(false);	// BREAK Release
					}
					// HydroMeter3
					if (  m_pHydroMeter3 != NULL &&
						  (typeid(*m_pHydroMeter3) == typeid(TChannelMaster) || typeid(*m_pHydroMeter3) == typeid(TSL500K) || typeid(*m_pHydroMeter3) == typeid(TAG1500K)) &&
						  m_pHydroMeter3->GetBreakStatus() ) {
						if ((typeid(*m_pHydroMeter3) == typeid(TSL500K) || typeid(*m_pHydroMeter3) == typeid(TAG1500K)) && m_pHydroMeter3->GetBreakStatus()) {
							AnsiString sStart = "START\r";
							m_pHydroMeter3->Send(sStart.c_str(), sStart.Length());
						}
						m_pHydroMeter3->SetBreak(false);	// BREAK Release
					}
				} // end if => 시작 전 전처리 끝.

				m_enStep = enStart;
				m_StartTime = Now();	// Logging Start Time
				GetMeasureDateTime(m_StartTime, MeasureDate, MeasureTime);

				//TRPoint RPoint;
				m_RPoint.Clear();
				m_RPoint.MeasureDate = MeasureDate;
				m_RPoint.MeasureTime = MeasureTime;

				bool bResult = false;
				bResult= m_RPoint.Insert();
				if ( !bResult ) break;

				m_MeasureDate = MeasureDate;
				m_MeasureTime = MeasureTime;

//				// Animmation Start
				m_pAniThread = new TAniThread(false);	// not suspend
				m_bAniThread = true;

				// WaterLevel Start --------------------------------------------

				if ( m_pWaterLevel != NULL ) {

					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelStart, 0);

				} else { // 수위계 없고 SL/AG Series일 때는 바로 SL/AG Series 시작한다.

					if ( m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TAG1500K) ||
													typeid(*m_pHydroMeter1) == typeid(TSL500K)) ) {
						::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter1Start, 0);
					}

					Sleep(50);	// Wait for while

					if ( m_pHydroMeter2 != NULL && (typeid(*m_pHydroMeter2) == typeid(TAG1500K) ||
													typeid(*m_pHydroMeter2) == typeid(TSL500K)) ) {
						::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter2Start, 0);
					}
				}

				Sleep(50);	// Wait For While

				// HydroMeter1(ChannelMaster) ----------------------------------

				if ( m_pHydroMeter1 != NULL && (typeid(*m_pHydroMeter1) == typeid(TChannelMaster) ||
												typeid(*m_pHydroMeter1) == typeid(TAccusonic)     ||
												typeid(*m_pHydroMeter1) == typeid(TRQ30)          ||
												typeid(*m_pHydroMeter1) == typeid(TRQ30D))       ||
												typeid(*m_pHydroMeter1) == typeid(TEWSVConverter)) {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter1Start, 0);
				}

				Sleep(50);	// Wait for while

				// HydroMeter2(ChannelMaster) ----------------------------------

				if ( m_pHydroMeter2 != NULL && (typeid(*m_pHydroMeter2) == typeid(TChannelMaster) ||
												typeid(*m_pHydroMeter2) == typeid(TAccusonic)     ||
												typeid(*m_pHydroMeter2) == typeid(TRQ30))) {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter2Start, 0);
				}

				// HydroMeter3(ChannelMaster) ----------------------------------

				if ( m_pHydroMeter3 != NULL && (typeid(*m_pHydroMeter3) == typeid(TChannelMaster) ||
												typeid(*m_pHydroMeter3) == typeid(TAccusonic)     ||
												typeid(*m_pHydroMeter3) == typeid(TRQ30))) {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter3Start, 0);
				}

				SendLoggerStatusToSender(enStart);
				SendLoggerStatusToViewer(enStart);

//				// Animmation Start
//				m_pAniThread = new TAniThread(false);	// not suspend
//				m_bAniThread = true;

				// 가장 최근 Logging 시작한 시간 "
				UnicodeString s = Now().FormatString("yyyy-mm-dd hh:nn:ss");
				lbLastLogTime->Caption = "로깅시간: " + s + " started";

				LogMessage("Logger Logging start..");
			}
			break;
		// ---------------------------------------------------------------------
		// WaterLevel Start
		// ---------------------------------------------------------------------
		case enWaterLevelStart :
			{
				if ( m_enStep == enWaterLevelStart ) break;	// 이미 그 상태이면 Skip

				m_enStep = enWaterLevelStart;
				if ( m_pWaterLevel != NULL ) {
					m_pWaterLevel->Start(m_MeasureDate, m_MeasureTime, true);	// first Start
					if ( m_pWaterLevel->WaterKind == LT500_STR || m_pWaterLevel->WaterKind == LT400_STR ||
						 m_pWaterLevel->WaterKind == AT200_STR || m_pWaterLevel->WaterKind == RNSWATER_STR) {
						TimerWater->Interval = 10 * 1000;	// 10 Sec interval
						TimerWater->Enabled  = true;		// Water Timer enable
					} else {
						// WaterLevel Timer not necessary
						//TimerWater->Interval = 10 * 1000;	// 10 Sec interval
						TimerWater->Enabled  = false;		// Water Timer enable
					}
				} else {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enWaterLevelFinish, 0);
				}
				LogMessage("WaterLevel start..");
			}
			break;
		case enWaterLevelWorking :
			{
			}
			break;
		// ---------------------------------------------------------------------
		// WaterLevel Finish
		// ---------------------------------------------------------------------
		case enWaterLevelFinish :
			{
				if ( m_enStep == enWaterLevelFinish ) break;	// 이미 그 상태이면 Skip

				m_enStep = enWaterLevelFinish;
				if ( m_pWaterLevel != NULL ) {
					m_pWaterLevel->Finish();
					if ( m_pWaterLevel->bLeak ) m_RPoint.UpdateWaterLevel(EN_POINT_STATUS::enPointLeak);	// 결측이 있는 상황
					else                        m_RPoint.UpdateWaterLevel(EN_POINT_STATUS::enPointFinish);	// 결측이 없는 상황
				}
				GoNextStep(enWaterLevelFinish);
				LogMessage("WaterLevel finished..");
			}
			break;
		// ---------------------------------------------------------------------
		// HydroMeter1 Start
		// ---------------------------------------------------------------------
		case enHydroMeter1Start :
			{
				if ( m_enStep == enHydroMeter1Start ) {
					LogMessage("m_enStep == enHydroMeter1Start.. already Start Status..break");
					break;	// 이미 그 상태이면 Skip
				}
				if ( m_pHydroSetup1 != NULL ) {
					LogMessage("m_pHydroSetup1 is not null.. HydroSetup Form Opened. break");
					break;	// 설정 화면이 열려 있으면 Skip한다.
				}
				if ( m_pHydroMeter1->GetBreakStatus() ) {
					LogMessage("m_pHydroMeter1 Break Status on...break");
					break;
				}

				m_enStep = enHydroMeter1Start;
				if ( m_pHydroMeter1 != NULL ) {
					m_pHydroMeter1->Start(m_MeasureDate, m_MeasureTime); 	// 유속 장비별로 측정 시작
					if (typeid(*m_pHydroMeter1) == typeid(TSL500K) || typeid(*m_pHydroMeter1) == typeid(TAG1500K)) {
						// Read Timer Enable
						TimerHydro1->Interval = 5 * 1000;	// 5 sec timer
						TimerHydro1->Enabled  = true;
					} else if ( typeid(*m_pHydroMeter1) == typeid(TRQ30) ) {	// RQ-30
						if ( m_pAirmar != NULL ) {
							m_pAirmar->Start(m_MeasureDate, m_MeasureTime);
						} else {
							// nothing to do..
						}
					} else if ( typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) {	// RQ-30d
						if ( m_pAirmar != NULL ) {
							m_pAirmar->Start(m_MeasureDate, m_MeasureTime);
						} else {
							// nothing to do..
						}
					} else {
						// ChannelMaster => nothing to do..
						// RQ-30,Accusonic, others
					}
				} else {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter1Finish, 0);
				}
				LogMessage("HydroMeter1 started..");
			}
			break;
		case enHydroMeter1Working :
			{
			}
			break;
		// ---------------------------------------------------------------------
		// HydroMeter1 Finish
		// ---------------------------------------------------------------------
		case enHydroMeter1Finish :
			{
				if ( m_enStep == enHydroMeter1Finish ) break;	// 이미 그 상태이면 Skip

				m_enStep = enHydroMeter1Finish;
				if ( m_pHydroMeter1 != NULL ) {
					m_pHydroMeter1->Finish();
					m_RPoint.UpdateHydroMeter1(EN_POINT_STATUS::enPointFinish);
				}
				GoNextStep(enHydroMeter1Finish);
				LogMessage("HydroMeter1 finished..");
			}
			break;
		// ---------------------------------------------------------------------
		// HydroMeter2 Start
		// ---------------------------------------------------------------------
		case enHydroMeter2Start :
			{
				if ( m_enStep == enHydroMeter2Start ) break;	// 이미 그 상태이면 Skip
				if ( m_pHydroSetup2 != NULL ) break;	// 설정 화면이 열려 있으면 Skip한다.
				if ( m_pHydroMeter2->GetBreakStatus()) break;

				m_enStep = enHydroMeter2Start;
				if ( m_pHydroMeter2 != NULL ) {
					m_pHydroMeter2->Start(m_MeasureDate, m_MeasureTime);
					if (typeid(*m_pHydroMeter2) == typeid(TSL500K) || typeid(*m_pHydroMeter2) == typeid(TAG1500K)) {
						// Read Timer Enable
						TimerHydro2->Interval = 5 * 1000;	// 5 sec timer
						TimerHydro2->Enabled  = true;
					} else {
						// ChannelMaster => nothing to do..
						// RQ-30, Accusonic, others
					}
				} else {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter2Finish, 0);
				}
				LogMessage("HydroMeter2 started..");
			}
			break;
		case enHydroMeter2Working :
			{
			}
			break;
		// ---------------------------------------------------------------------
		// HydroMeter2 Finish
		// ---------------------------------------------------------------------
		case enHydroMeter2Finish :
			{
				if ( m_enStep == enHydroMeter2Finish ) break;	// 이미 그 상태이면 Skip

				m_enStep = enHydroMeter2Finish;
				if ( m_pHydroMeter2 != NULL ) {
					m_pHydroMeter2->Finish();
					m_RPoint.UpdateHydroMeter2(EN_POINT_STATUS::enPointFinish);
				}
				GoNextStep(enHydroMeter2Finish);
				LogMessage("HydroMeter2 finished..");
			}
			break;

		// ---------------------------------------------------------------------
		// HydroMeter3 Start
		// ---------------------------------------------------------------------
		case enHydroMeter3Start :
			{
				if ( m_enStep == enHydroMeter3Start ) break;	// 이미 그 상태이면 Skip
				if ( m_pHydroSetup3 != NULL ) break;	// 설정 화면이 열려 있으면 Skip한다.
				if ( m_pHydroMeter3->GetBreakStatus()) break;

				m_enStep = enHydroMeter3Start;
				if ( m_pHydroMeter3 != NULL ) {
					m_pHydroMeter3->Start(m_MeasureDate, m_MeasureTime);
					if (typeid(*m_pHydroMeter3) == typeid(TSL500K) || typeid(*m_pHydroMeter3) == typeid(TAG1500K)) {
						// Read Timer Enable
						TimerHydro3->Interval = 5 * 1000;	// 5 sec timer
						TimerHydro3->Enabled  = true;
					} else {
						// ChannelMaster => nothing to do..
						// RQ-30, Accusonic, others
					}
				} else {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter3Finish, 0);
				}
				LogMessage("HydroMeter3 started..");
			}
			break;
		case enHydroMeter3Working :
			{
			}
			break;

		// ---------------------------------------------------------------------
		// HydroMeter3 Finish
		// ---------------------------------------------------------------------
		case enHydroMeter3Finish :
			{
				if ( m_enStep == enHydroMeter3Finish ) break;	// 이미 그 상태이면 Skip

				m_enStep = enHydroMeter3Finish;
				if ( m_pHydroMeter3 != NULL ) {
					m_pHydroMeter3->Finish();
					m_RPoint.UpdateHydroMeter3(EN_POINT_STATUS::enPointFinish);
				}
				GoNextStep(enHydroMeter3Finish);
				LogMessage("HydroMeter3 finished..");
			}
			break;

		// ---------------------------------------------------------------------
		// RQ-30 Start (1분 단위 측정 시작 )
		// ---------------------------------------------------------------------
		case enRQ30Start :
			{
				if ( m_pHydroMeter1 == NULL ) break;

				if ( typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) {
					if ( ((TRQ30D *)m_pHydroMeter1)->m_enWorkingStep != enSensorIdle ) {	// 이미 시작 되었음
						// Nothing to do..
					} else {	// Idle 상태에서만 출발한다.
						((TRQ30D *)m_pHydroMeter1)->StartRQ();
					}
				} else {
					// LogMessage("Other sensor not need RQ30start..");
				}
			}
			break;

		// ---------------------------------------------------------------------
		// RQ-30 Finish (1분 단위 측정 종료 )
		// ---------------------------------------------------------------------
		case enRQ30Finish :
			{
				if ( m_pHydroMeter1 == NULL ) break;

				if ( typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) {
					if ( ((TRQ30D *)m_pHydroMeter1)->m_enWorkingStep == enSensorIdle ) break;
					if ( ((TRQ30D *)m_pHydroMeter1)->m_enWorkingStep == enSensorFinish ) break;	// 중복 메세지 => 이미 Finish를 했는데 또 들어옴.

					if ( ((TRQ30D *)m_pHydroMeter1)->m_enWorkingStep == enSensorStart ||  // 시작/ 측정 중에만 -> Finish
						 ((TRQ30D *)m_pHydroMeter1)->m_enWorkingStep == enSensorWorking ) {
						((TRQ30D *)m_pHydroMeter1)->FinishRQ();  	// 1분 단위 측정 종료
					} else {	// 출발을 해야만 Finish가 가능한다.
						// noting to do..
					}
				} else {
					// LogMessage("Other sensor not need RQ30start..");
				}
			}
			break;

		// ---------------------------------------------------------------------
		// EWSVConverter Start
		// ---------------------------------------------------------------------
		case enEWSVStart :
			{
				if ( m_pHydroMeter1 == NULL ) break;

				if ( typeid(*m_pHydroMeter1) == typeid(TEWSVConverter) ) {
					if ( ((TEWSVConverter *)m_pHydroMeter1)->m_enWorkingStep == enSensorIdle ) {	// 이미 시작 되었음
						((TEWSVConverter *)m_pHydroMeter1)->MeasureStart(theEnv.m_EWSVWorkTime, theEnv.m_EWSVSensorAvg);
					} else {
						// working status => nothing to do..
					}
				} else {
					// LogMessage("..");
				}
			}
			break;

		// ---------------------------------------------------------------------
		// EWSVConverter Finish
		// ---------------------------------------------------------------------
		case enEWSVFinish :
			{
				if ( m_pHydroMeter1 == NULL ) break;

				if ( typeid(*m_pHydroMeter1) == typeid(TEWSVConverter) ) {
					if ( ((TEWSVConverter *)m_pHydroMeter1)->m_enWorkingStep != enSensorIdle ) {
						((TEWSVConverter *)m_pHydroMeter1)->MeasureDataReq();	// 데이터 요구
					} else {	//
						// idle status => noting to do..
					}
				} else {
					// LogMessage("...");
				}
			}
			break;
		// ---------------------------------------------------------------------
		// Logger Finish
		// ---------------------------------------------------------------------
		case enFinish :
			{
				if ( m_enStep == enFinish ) break;	// 이미 그 상태이면 Skip

				m_enStep = enFinish;

				// if not finished sensor, force to finish the sensor.
				if ( m_pWaterLevel != NULL && (m_pWaterLevel->GetStatus() == enSensorStart ||
											   m_pWaterLevel->GetStatus() == enSensorWorking)) {
					m_pWaterLevel->Finish();
					m_RPoint.UpdateWaterLevel(EN_POINT_STATUS::enPointLeak); 	// 2025.04.08 enPointLeak => enPointFinish update
				}

				if ( m_pHydroMeter1 != NULL && (m_pHydroMeter1->GetStatus() == enSensorStart ||
												m_pHydroMeter1->GetStatus() == enSensorWorking)) {
					// RQ-30 측정 Finish
					if ( typeid(*m_pHydroMeter1) == typeid(TRQ30) ) {	// RQ-30 => Airmar Finish
						STAirmarData AirmarData;
						memset(&AirmarData, 0x00, sizeof(STAirmarData));
						if ( m_pAirmar != NULL ) {
							m_pAirmar->Finish();
							AirmarData = m_pAirmar->GetAirmarData();
						}
						((TRQ30 *)m_pHydroMeter1)->SetAirmarData(AirmarData);
					}

					// RQ-30d 측정 Finish
					if ( typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) {	// RQ-30d	=> Airmar Finish
						STAirmarData AirmarData;
						memset(&AirmarData, 0x00, sizeof(STAirmarData));
						if ( m_pAirmar != NULL ) {
							m_pAirmar->Finish();
							AirmarData = m_pAirmar->GetAirmarData();
						}
						((TRQ30D *)m_pHydroMeter1)->SetAirmarData(AirmarData);
					}

					m_pHydroMeter1->Finish();	// Accusonic은 여기서 측정이 종료된다. RQ-30도 여기서 종료된다.  RQ-30D도 여기서 종료된다.
					if ( typeid(*m_pHydroMeter1) != typeid(TAccusonic)) {
						m_RPoint.UpdateHydroMeter1(EN_POINT_STATUS::enPointFinish);
					}
				}

				if ( m_pHydroMeter2 != NULL && (m_pHydroMeter2->GetStatus() == enSensorStart ||
												m_pHydroMeter2->GetStatus() == enSensorWorking)) {

					m_pHydroMeter2->Finish();	// Accusonic은 여기서 측정이 종료된다. RQ-30 여기서 종료힌다.
					if ( typeid(*m_pHydroMeter2) != typeid(TAccusonic)) {
						m_RPoint.UpdateHydroMeter2(EN_POINT_STATUS::enPointFinish);
					}
				}

				if ( m_pHydroMeter3 != NULL && (m_pHydroMeter3->GetStatus() == enSensorStart ||
												m_pHydroMeter3->GetStatus() == enSensorWorking)) {

					m_pHydroMeter3->Finish();	// Accusonic은 여기서 측정이 종료된다. RQ-30 여기서 종료힌다.
					if ( typeid(*m_pHydroMeter3) != typeid(TAccusonic)) {
						m_RPoint.UpdateHydroMeter3(EN_POINT_STATUS::enPointFinish);
					}
				}

				//--------------------------------------------------------------
				// 유량산출
				//--------------------------------------------------------------
				if ( m_pFlowStream != NULL && theEnv.m_FlowType > 0 ) { // 유량을 산출하여야 한다.
					if ( m_pHydroMeter1 == NULL )  { 	// 유량산출 불가
						// 유량 산출 불가능
					} else if ( typeid(*m_pHydroMeter1) == typeid(TRQ30) ) {
						// RQ30 => 장비에서 유량 산정 함.
						// 유량 산정 불가능
					} else if ( typeid(*m_pHydroMeter1) == typeid(TAccusonic) ) {
						// Accusonic 장비는 유량 산출을 지원하지 않음
						// 유량 산출 불가능
					} else if ( m_pHydroMeter2 != NULL && typeid(*m_pHydroMeter2) == typeid(TAccusonic) ) {
						// Accusonic 장비는 유량 산출을 지원하지 않음
						// 유량 산출 불가능
					} else { 	// ChannelMaster, Sontek 장비만 가능함.
						m_pFlowStream->m_enWorkingStep = enSensorStart;
						SetFlowStreamIcon(enFlowStreamAni4);	//유량산출 아이콘 처리
						if ( FormMain->m_pAniThread != NULL ) {
							FormMain->m_pAniThread->m_bFlowStream = true;
							FormMain->m_pAniThread->m_FlowStreamIndex = enFlowStreamAni4;
						}
						//SendLoggerStatusToViewer(enHydroMeter2Finish);

						m_pFlowStream->SetMeasureTime(m_MeasureDate, m_MeasureTime);
						m_pFlowStream->CaculateFlowStream();	// 유량 산출함.
						m_RPoint.UpdateFlowStream(EN_POINT_STATUS::enPointFinish);
						LogMessage("FlowStream finished... ");

						m_pFlowStream->m_enWorkingStep = enSensorFinish;
						SetFlowStreamIcon(enFlowStreamNormal); 	//유량산출 아이콘 처리
						if ( FormMain->m_pAniThread != NULL ) {
							FormMain->m_pAniThread->m_bFlowStream = false;
							FormMain->m_pAniThread->m_FlowStreamIndex = enFlowStreamNormal;
						}
						//SendLoggerStatusToViewer(enHydroMeter2Finish);
					}
				}
				SendLoggerStatusToSender(enFinish);
				SendLoggerStatusToViewer(enFinish);

				// Animation Stop
				m_bAniThread = false;
				GAniEvent->SetEvent();
				Sleep(100);	// Wait ani therad terminate
				delete m_pAniThread;
				m_pAniThread = NULL;

				// 가장 최근 Logging 한 시간을 화면에 적는다."
				UnicodeString s = Now().FormatString("yyyy-mm-dd hh:nn:ss");
				lbLastLogTime->Caption = "로깅시간: " + s + " ok..";

				// 모든 처리가 끊나면 idle Message
				::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enIdle, 0);
				LogMessage("Logger Logging finished..[" + m_MeasureDate + "] [" + m_MeasureTime + "]");

				// Garbase Collection 2025.09.16
				// 10분 단위 작업이 끝나면 모든 메모리를 정리한다.
				MemoryGarbageCollection();	// 사용한 메모리를 O/S에 즉시 반납한다. -> 최대한 메모리 내용을 줄힌다.

			}
			break;
	} // switch
}

//---------------------------------------------------------------------------
//
// Basic Functions
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool TFormMain::InitLogger()	// Init WaterLevel/HydroMeter1/2,...
{
	// FlowStream Data Load
	m_FlowIVM.PointCode = theEnv.m_PointCode;
	m_FlowIVM.GetData();

	m_FlowHA.PointCode = theEnv.m_PointCode;
	m_FlowHA.GetData();

	m_FlowADVM.PointCode = theEnv.m_PointCode;
	m_FlowADVM.GetData();

	// WaterLevel Create
	if ( !theEnv.m_WaterType.IsEmpty() && theEnv.m_WaterType != NONE_STR ) {
		if ( theEnv.m_WaterType == LT500_STR ) {			// LT500
			m_pWaterLevel = new TLT500();
			m_pWaterLevel->Init(WaterPort);
			m_pWaterLevel->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pWaterLevel->Offset = theEnv.m_WaterOffset;
			m_pWaterLevel->Clear();
		} else if ( theEnv.m_WaterType == AT200_STR ) {		// AT200
			m_pWaterLevel = new TAT200();
			m_pWaterLevel->Init(WaterPort);
			m_pWaterLevel->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pWaterLevel->Offset = theEnv.m_WaterOffset;
			m_pWaterLevel->Clear();
		} else if ( theEnv.m_WaterType == LT400_STR ) {		// LT400
			m_pWaterLevel = new TLT400();
			m_pWaterLevel->Init(WaterPort);
			m_pWaterLevel->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pWaterLevel->Offset = theEnv.m_WaterOffset;
			m_pWaterLevel->Clear();

			m_pAtmos = new TAtmos();
			m_pAtmos->Init(AtmosPort);
		} else if ( theEnv.m_WaterType == OTTCBS_STR ) {	// OTTCBS
			m_pWaterLevel = new TCBS();
			m_pWaterLevel->Init(WaterPort);
			m_pWaterLevel->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pWaterLevel->Offset = theEnv.m_WaterOffset;
			m_pWaterLevel->Clear();
		} else if ( theEnv.m_WaterType == RNSWATER_STR ) {	// 2024.07.25 RnsWater Sensor => RNSWATER
			m_pWaterLevel = new TRnsWater();
			m_pWaterLevel->Init(WaterPort);
			m_pWaterLevel->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pWaterLevel->Offset = theEnv.m_WaterOffset;
			m_pWaterLevel->Clear();

			if ( theEnv.m_bAtmosEnable ) { 	// 대기압 데이터를 받을 수 있다.
				m_pAtmos = new TAtmos();
				m_pAtmos->Init(AtmosPort);
			} else {
				// RnsWater Sensor 이지만 대기압 보정을 하지 않는다.
				m_pAtmos = NULL;
			}
		} else {
			m_pWaterLevel = NULL;
		}

		try {
			WaterPort->ComNumber = ConvertComPort(theEnv.m_WaterPort);
			WaterPort->Baud      = theEnv.m_WaterBaudRate;
			WaterPort->Open      = true;

			if ( theEnv.m_WaterType == LT400_STR ) {   	// LT400일 때 대기압센서 포트 지정
				if ( ! theEnv.m_AtmosPort.IsEmpty() ) { // 대기압센서 포트 설정
					AtmosPort->ComNumber = ConvertComPort(theEnv.m_AtmosPort);
					AtmosPort->Baud      = 9600;
					AtmosPort->Open      = true;	// 대기압 센서 포트 Open
				} else {
					ShowMessage("수위계가 LT400일 경우 대기압센서가 설정되어 있어야 합니다. ");
				}
			}

			if ( theEnv.m_WaterType == RNSWATER_STR ) {	// RnsWater일 때(VegaPuls, Mk21) 대기압 보정 수행
				if ( theEnv.m_bAtmosEnable && !theEnv.m_AtmosPort.IsEmpty() ) { // 대기압 보정 ON, 대기압포트 지정되어 있을 때.
					AtmosPort->ComNumber = ConvertComPort(theEnv.m_AtmosPort);
					AtmosPort->Baud      = 9600;
					AtmosPort->Open      = true;	// 대기압 센서 포트 Open
				} else {
					//ShowMessage("대기압 보정을 하지 않음");
				}
			}
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	} // end waterlevel

	// HydroMeter1 Create
	if ( !theEnv.m_HydroType1.IsEmpty() && theEnv.m_HydroType1 != NONE_STR ) {
		if ( theEnv.m_HydroType1 == CHANNELMASTER_STR ) {
			m_pHydroMeter1 = new TChannelMaster();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();
		} else if ( theEnv.m_HydroType1 == AGSERIES_STR ) {
			m_pHydroMeter1 = new TAG1500K();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->SetCellInfo(theEnv.m_CellBegin1, theEnv.m_CellEnd1, theEnv.m_CellNumber1, theEnv.m_CellSize1, theEnv.m_CellBlank1);
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();
		} else if ( theEnv.m_HydroType1 == SLSERIES_STR ) {
			m_pHydroMeter1 = new TSL500K();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->SetCellInfo(theEnv.m_CellBegin1, theEnv.m_CellEnd1, theEnv.m_CellNumber1, theEnv.m_CellSize1, theEnv.m_CellBlank1);
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();
		} else if ( theEnv.m_HydroType1 == ACCUSONIC_A_STR || theEnv.m_HydroType1 == ACCUSONIC_B_STR ) { 	// heaven2 2017.11.01
			m_pHydroMeter1 = new TAccusonic();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->HydroKind = theEnv.m_HydroType1;
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();
		} else if ( theEnv.m_HydroType1 == RQ30_STR ) {
			m_pHydroMeter1 = new TRQ30();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->HydroKind = theEnv.m_HydroType1;
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();

			// heaven2 2023.06.17
			if ( !theEnv.m_bAirmarEnable ) {	// 아직 수위계(RnsWater)에서 Airmar 관련 설정이 없다면 => 유속계 1에서 Airmar 데이터를 수집한다. m_pAirmar
				if ( !theEnv.m_HydroAirmarPort1.IsEmpty() && theEnv.m_HydroAirmarPort1 != "NONE" ) {
					m_pAirmar = new TAirmar();
					m_pAirmar->m_bLinker = theEnv.m_AirmarLinker;	// AirmarLinker 사용 여부 ( true: AirmarLinker 사용)
				} else {
					m_pAirmar = NULL;	// Airmar is not ready
				}
			}
//			int ComNumber = ConvertComPort(theEnv.m_HydroAirmarPort1);
//			int Baudrate  = theEnv.m_HydroAirmarBaudRate1;
//			m_pAirmar->Init(AirmarPort, CumNumber, Baudrate);
		} else if ( theEnv.m_HydroType1 == RQ30D_STR ) {
			m_pHydroMeter1 = new TRQ30D();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->HydroKind = theEnv.m_HydroType1;
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			((TRQ30D *)m_pHydroMeter1)->SetSlaveCount(theEnv.m_SlaveCount);
			((TRQ30D *)m_pHydroMeter1)->MasterStatus.SetDeviceAddress(theEnv.m_HydroMasterAddress);

			// Slave Port Assign
			if ( theEnv.m_HydroUseSlavePort == ON_STR ) {    	// SlavePortThrad가 사용할 Port를 지정한다.
				((TRQ30D *)m_pHydroMeter1)->SetSlavePort(RQSlavePort);
				((TRQ30D *)m_pHydroMeter1)->SlaveStatus.SetDeviceAddress(theEnv.m_HydroSlaveAddress);
			}
			m_pHydroMeter1->Clear();

			((TRQ30D *)m_pHydroMeter1)->CreateThread();	// RQ30D이 사용할 Thread를 생성한다.

			// heaven2 2023.06.17
			if ( !theEnv.m_bAirmarEnable ) {	// 아직 수위계(RnsWater)에서 Airmar 관련 설정이 없다면 => 유속계 1에서 Airmar 데이터를 수집한다. m_pAirmar
				if ( !theEnv.m_HydroAirmarPort1.IsEmpty() && theEnv.m_HydroAirmarPort1 != "NONE" ) {
					m_pAirmar = new TAirmar();
					m_pAirmar->m_bLinker = theEnv.m_AirmarLinker;	// AirmarLinker 사용 여부 ( true: AirmarLinker 사용)
				} else {
					m_pAirmar = NULL;	// Airmar is not ready
				}
			}
//			int ComNumber = ConvertComPort(theEnv.m_HydroAirmarPort1);
//			int Baudrate  = theEnv.m_HydroAirmarBaudRate1;
//			m_pAirmar->Init(AirmarPort, CumNumber, Baudrate);
		} else if ( theEnv.m_HydroType1 == EWSVCONVERTER_STR ) {
			m_pHydroMeter1 = new TEWSVConverter();
			m_pHydroMeter1->Init(HydroPort1, enHydroFirst);
			m_pHydroMeter1->HydroKind = theEnv.m_HydroType1;
			m_pHydroMeter1->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter1->Clear();
//			int ComNumber = ConvertComPort(theEnv.m_HydroAirmarPort1);
//			int Baudrate  = theEnv.m_HydroAirmarBaudRate1;
//			m_pAirmar->Init(AirmarPort, CumNumber, Baudrate);
		} else {
			m_pHydroMeter1 = NULL;
		}

		try {
			HydroPort1->ComNumber = ConvertComPort(theEnv.m_HydroPort1);
			HydroPort1->Baud      = theEnv.m_HydroBaudRate1;
			try {
				HydroPort1->Open      = true;
			} catch ( Exception &e ) {
				LogMessage(e.Message);
			}

			if ( theEnv.m_HydroType1 == RQ30_STR || theEnv.m_HydroType1 == RQ30D_STR ) {   // RQ-30/RQ-30d => Airmar port open
				// RQ-30d Slave Port Open
				if ( theEnv.m_HydroType1 == RQ30D_STR ) {
					if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
						try {
							RQSlavePort->ComNumber = ConvertComPort(theEnv.m_HydroSlavePort);	// COM01,COM02,...
							RQSlavePort->Baud      = theEnv.m_HydroSlaveBaudRate;	// default: 9600
							try {
								RQSlavePort->Open  = true;
							} catch ( Exception &e ) {
								LogMessage(e.Message);
							}
						} catch ( Exception &e ) {
							LogMessage("RQ30D Slave Port open error.." + e.Message);
						}
					}
				}
				// Airamr Port open
				if ( !theEnv.m_bAirmarEnable ) { // 수위계(RnsWater)에서 기상 데이터를 수집하지 않으면
					if ( !theEnv.m_HydroAirmarPort1.IsEmpty() && theEnv.m_HydroAirmarPort1.SubString(1,3) == "COM") {
						int ComNumber = ConvertComPort(theEnv.m_HydroAirmarPort1);
						int Baudrate  = theEnv.m_HydroAirmarBaudRate1;

						m_pAirmar->Init(AirmarPort, ComNumber, Baudrate);

						//AirmarPort->ComNumber = ConvertComPort(theEnv.m_HydroAirmarPort1);
						//AirmarPort->Baud      = theEnv.m_HydroAirmarBaudRate1;
						//AirmarPort->Open      = true;
					} else {
						// Airmar port is not defined
						LogMessage("RQ-30d Device Airmar port is not defined..");
					}
				} else {
					// nothing to do .. 기상데이터를 수위계에서 수집
				}
			} else {
				// nothing to do..
			}
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	} // end Hydro1

	// HydroMeter2 Create
	if ( !theEnv.m_HydroType2.IsEmpty() && theEnv.m_HydroType2 != NONE_STR ) {
		if ( theEnv.m_HydroType2 == CHANNELMASTER_STR ) {
			m_pHydroMeter2 = new TChannelMaster();
			m_pHydroMeter2->Init(HydroPort2, enHydroSecond);
			m_pHydroMeter2->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter2->Clear();
		} else if ( theEnv.m_HydroType2 == AGSERIES_STR ) {
			m_pHydroMeter2 = new TAG1500K();
			m_pHydroMeter2->Init(HydroPort2, enHydroSecond);
			m_pHydroMeter2->SetCellInfo(theEnv.m_CellBegin2, theEnv.m_CellEnd2, theEnv.m_CellNumber2, theEnv.m_CellSize2, theEnv.m_CellBlank2);
			m_pHydroMeter2->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter2->Clear();
		} else if ( theEnv.m_HydroType2 == SLSERIES_STR ) {
			m_pHydroMeter2 = new TSL500K();
			m_pHydroMeter2->Init(HydroPort2, enHydroSecond);
			m_pHydroMeter2->SetCellInfo(theEnv.m_CellBegin2, theEnv.m_CellEnd2, theEnv.m_CellNumber2, theEnv.m_CellSize2, theEnv.m_CellBlank2);
			m_pHydroMeter2->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter2->Clear();
		} else if ( theEnv.m_HydroType2 == ACCUSONIC_A_STR || theEnv.m_HydroType2 == ACCUSONIC_B_STR ) { 	// heaven2 2017.11.01
			m_pHydroMeter2 = new TAccusonic();
			m_pHydroMeter2->Init(HydroPort2, enHydroSecond);
			m_pHydroMeter2->HydroKind = theEnv.m_HydroType2;
			m_pHydroMeter2->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter2->Clear();
		} else if ( theEnv.m_HydroType2 == RQ30_STR ) { 	// heaven2 2021.12.06
			m_pHydroMeter2 = new TRQ30();
			m_pHydroMeter2->Init(HydroPort2, enHydroSecond);
			m_pHydroMeter2->HydroKind = theEnv.m_HydroType2;
			m_pHydroMeter2->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter2->Clear();

			// HydroMeter2 ( RQ30) is not support Airmar Sensor
		} else {
			m_pHydroMeter2 = NULL;
		}

		try {
			HydroPort2->ComNumber = ConvertComPort(theEnv.m_HydroPort2);
			HydroPort2->Baud      = theEnv.m_HydroBaudRate2;
			try {
				HydroPort2->Open      = true;
			} catch ( Exception &e ) {
				LogMessage(e.Message);
			}
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	}

	// HydroMeter3 Create
	if ( !theEnv.m_HydroType3.IsEmpty() && theEnv.m_HydroType3 != NONE_STR ) {
		if ( theEnv.m_HydroType3 == CHANNELMASTER_STR ) {
			m_pHydroMeter3 = new TChannelMaster();
			m_pHydroMeter3->Init(HydroPort3, enHydroThird);
			m_pHydroMeter3->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter3->Clear();
		} else if ( theEnv.m_HydroType3 == AGSERIES_STR ) {
			m_pHydroMeter3 = new TAG1500K();
			m_pHydroMeter3->Init(HydroPort3, enHydroThird);
			m_pHydroMeter3->SetCellInfo(theEnv.m_CellBegin3, theEnv.m_CellEnd3, theEnv.m_CellNumber3, theEnv.m_CellSize3, theEnv.m_CellBlank3);
			m_pHydroMeter3->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter3->Clear();
		} else if ( theEnv.m_HydroType3 == SLSERIES_STR ) {
			m_pHydroMeter3 = new TSL500K();
			m_pHydroMeter3->Init(HydroPort3, enHydroThird);
			m_pHydroMeter3->SetCellInfo(theEnv.m_CellBegin3, theEnv.m_CellEnd3, theEnv.m_CellNumber3, theEnv.m_CellSize3, theEnv.m_CellBlank3);
			m_pHydroMeter3->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
			m_pHydroMeter3->Clear();
		} else {
			m_pHydroMeter3 = NULL;
		}

		try {
			HydroPort3->ComNumber = ConvertComPort(theEnv.m_HydroPort3);
			HydroPort3->Baud      = theEnv.m_HydroBaudRate3;
			try {
				HydroPort3->Open      = true;
			} catch ( Exception &e ) {
				LogMessage(e.Message);
			}
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
	} // end Hydro3

	// FlowStream Create
	if ( theEnv.m_FlowType != FLOWSTREAM_OFF ) {
		m_pFlowStream = new TFlowStream(theEnv.m_FlowType);
		//m_pFlowStream->m_FlowType = theEnv.m_FlowType;	// 0:None, 1: 유량사업단, 2:RNSEA 자체
		m_pFlowStream->InitFlowStream(&m_FlowIVM, &m_FlowHA, &m_FlowADVM);
		m_pFlowStream->InitWaterHydroMeter(m_pWaterLevel, m_pHydroMeter1, m_pHydroMeter2);
	}

	// GeneralTimer Enable
	TimerGeneral->Interval = 1000;	// 1sec
	TimerGeneral->Enabled = true;

	// DataViewer Server Socket Open ------------------------------------------
	try {
		ServerSocket->Port   = theEnv.m_DataLoggerPort;
		ServerSocket->Active = true;
	} catch ( Exception &e ) {
		LogMessage("[ServerSocket]" + e.Message);
	}

	// WatchDog Socket connect ------------------------------------------------
	try {
		WatchDogSocket->Host   = "127.0.0.1";
		WatchDogSocket->Port   = theEnv.m_WatchDogPort;
		WatchDogSocket->Active = true;
	} catch ( Exception &e ) {
		LogMessage("[WatchDog]" + e.Message);
	}

	// DataSender Socket connect ----------------------------------------------
	try {
		SenderSocket->Host   = theEnv.m_DataSenderIP;
		SenderSocket->Port   = theEnv.m_DataSenderFromLoggerPort;
		SenderSocket->Active = true;
	} catch ( Exception &e ) {
		LogMessage("[DataSender]" + e.Message);
	}

	// WatchDog Timer Enable
	TimerWatchDog->Interval = 30 * 1000;	// 30 sec
	TimerWatchDog->Enabled  = true;

	Sleep(50); // Wait for while

	// DataSender Timer Enable
	TimerSender->Interval = 30 * 1000;	// 30 sec
	TimerSender->Enabled  = true;

	Sleep(50); // Wait for while

	// DataViewer Timer Enable
	TimerViewer->Interval = 30 * 1000;	// 30 sec
	TimerViewer->Enabled  = true;

	return true;
}


//---------------------------------------------------------------------------
// 설정 정보의 변경(자체 또는 네트워크)으로 인해 현재의 설정 정보를 모두 무시한다.
// 무시한 후 DB 에서 다시 Reload하여 설정한다.
void TFormMain::ClearLogger()	// Clear Logger Resoure and Reload Logger
{
	// if Animation go, stop Animation
	if ( m_bAniThread ) {
		m_bAniThread = false;
		GAniEvent->SetEvent();
		Sleep(200);	// Wait for while
		delete m_pAniThread;
		m_pAniThread = NULL;
	}

	LogMessage("The setup changed. all setup information will reloaded  and logging is stopping...");

	// Socket Close
	try {
		SenderSocket->Socket->Close();
		WatchDogSocket->Socket->Close();
		ServerSocket->Close();
	} catch ( Exception &e ) {
		LogMessage(e.Message);
	}

	// Serial Port Close
	try {
		if ( WaterPort->Open  )   WaterPort->Open   = false;
		if ( AtmosPort->Open  )   AtmosPort->Open   = false;
		if ( AirmarPort->Open )   AirmarPort->Open  = false;
		if ( RQSlavePort->Open)   RQSlavePort->Open = false;
		if ( HydroPort1->Open )   HydroPort1->Open  = false;
		if ( HydroPort2->Open )   HydroPort2->Open  = false;
		if ( HydroPort3->Open )   HydroPort3->Open  = false;
	} catch ( Exception &e ) {
		LogMessage(e.Message);
	}

	// Timer Clear
	TimerGeneral->Enabled    = false;
	TimerWatchDog->Enabled   = false;
	TimerSender->Enabled     = false;
	TimerViewer->Enabled     = false;
	TimerWater->Enabled      = false;
	TimerHydro1->Enabled     = false;
	TimerHydro2->Enabled     = false;
	TimerHydro3->Enabled     = false;

	Sleep(100);	// Wait for while (Resource clear)

	// Hydro/WaterLevel Cleared
	if ( m_pHydroMeter1 != NULL ) { delete m_pHydroMeter1; m_pHydroMeter1 = NULL; }
	if ( m_pHydroMeter2 != NULL ) { delete m_pHydroMeter2; m_pHydroMeter2 = NULL; }
	if ( m_pHydroMeter3 != NULL ) { delete m_pHydroMeter3; m_pHydroMeter3 = NULL; }
	if ( m_pWaterLevel  != NULL ) { delete m_pWaterLevel;  m_pWaterLevel  = NULL; }
	if ( m_pAtmos       != NULL ) { delete m_pAtmos;       m_pAtmos       = NULL; }
	if ( m_pAirmar      != NULL ) { delete m_pAirmar;      m_pAirmar      = NULL; }
	if ( m_pFlowStream  != NULL ) { delete m_pFlowStream;  m_pFlowStream  = NULL; }

	// FlowStream Clear
	m_FlowIVM.Clear();
	m_FlowHA.Clear();
	m_FlowADVM.Clear();
}

//---------------------------------------------------------------------------
bool TFormMain::IsWorking()
{
	if ( m_enStep == enIdle ) return false;	// idle status
	else                      return true;	// Working status
}

//---------------------------------------------------------------------------
// 현재 시간이 측정을 시작해야 할 시간인 지 판정한다.
// m_enStep == enIdle 상태이고, 시작할 시간에 다가왔을 때.
bool TFormMain::IsStartTime(TDateTime now)
{
	USHORT hour, minute, second, msecond;
	now.DecodeTime(&hour, &minute, &second, &msecond);

	bool bStartMinute = false;
	bStartMinute = theEnv.IsStartMinute(minute);

	if ( bStartMinute && m_enStep == enIdle ) return true;	// Start 시간이고 아직 Start하지 않음
	else                                      return false; // Idle 시간이거나 Start 시간이고 이미 Start한 상태

/*
	int StartTime[60];
	int Len = 0;
	int Interval  = 10;	// 10:00 minutes
	int Start = theEnv.m_StartTime;	// 0 ~ 9Minutes

	USHORT hour, minute, second, msecond;
	now.DecodeTime(&hour, &minute, &second, &msecond);

//	if ( second != 0 ) return false;	// 매 0초마다 평가한다.

	try {
		Interval = StrToInt(theEnv.m_Interval.SubString(1,2));  	// 10:00
	} catch ( Exception &e ) {
		Interval = 10;
	}

	// make Start Time Table
	while ( Start < 60 ) {
		StartTime[Len++] = Start;
		Start += Interval;
	}

	bool bFound = false;
	for ( int i = 0 ; i < Len ; i++ ) {
		if ( minute == StartTime[i] && m_enStep == enIdle ) {
			bFound = true;
			break;
		}
	}

	return bFound;	// True: 시작해야 하는 시간, false: idle 또는 시작해야할 시간이나 이미 시작한 상태..
*/
}

//---------------------------------------------------------------------------
// 각 센서의 동작으로 측정이 끝나지 않을 때 특정 시간이 되면 강제로 끝낸다. ( 08:30 + 10초)
bool TFormMain::IsExpireTime(int iSpanTime)
{
	UnicodeString WorkTime = "08:30";	// default , 2025.02.18 "09:00"으로 설정 ( 360초 )
	int iMinute, iSecond;
	if ( theEnv.m_WorkTime.Length() != 5 ) WorkTime = "08:30";

	try {
		iMinute = StrToInt(theEnv.m_WorkTime.SubString(1,2));	// 08:30
		iSecond = StrToInt(theEnv.m_WorkTime.SubString(4,2));	// 08:30
	} catch ( Exception &e ) {
		iMinute = 8;	// default
		iSecond = 30;	//default
	}

	int iExpireSecond = iMinute * 60 + iSecond + 10;   // 08:30 + 10Second ( 10Second: Extra Time to Expire )

	if ( iSpanTime >= iExpireSecond ) return true;
	else                              return false;
}

//---------------------------------------------------------------------------
// 각 센서의 동작이 끝났을 때 다음 진행해야 Step을 판정하고 진행한다.
void TFormMain::GoNextStep(EN_LOGGER_STEP enStep)	// 사용 시점: WaterLevelFinish, HydroMeter1Finish, HydroMeter2Finish
{
	bool bWorking = false;

	switch ( enStep ) {

		case enWaterLevelFinish :	// 수위계 작업 종료 시점
			{
				// 수위계 끝나고 유속계가 SONTEK인 경우 이루틴을 따른다.
				if ( m_pHydroMeter1 != NULL &&
					( (typeid(*m_pHydroMeter1) == typeid(TSL500K)) || (typeid(*m_pHydroMeter1) == typeid(TAG1500K))) ) {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter1Start, 0);
					bWorking = true;
				}

				//::Sleep(50);	// Wait for While

				if ( m_pHydroMeter2 != NULL &&
					( (typeid(*m_pHydroMeter2) == typeid(TSL500K)) || (typeid(*m_pHydroMeter2) == typeid(TAG1500K))) ) {
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enHydroMeter2Start, 0);
					bWorking = true;
				}

				if ( !bWorking ) {	// 작업 중이 아닌 경우 Logger에 전체 Logging Finish 명령을 내린다.
					if ( IsAllComplete() ) {
						::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enFinish, 0);
					}
				}

			}
			break;

		case enHydroMeter1Finish :	// 유속계1 작업 종료 시점
			{
				if ( IsAllComplete() ) {	// 모든 작업이 끝나면 Finish로 이동한다.
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enFinish, 0);
				}
			}
			break;

		case enHydroMeter2Finish :	// 유속계2 작업 종료 시점
			{
				if ( IsAllComplete() ) {	// 모든 작업이 끝나면 Finish로 이동한다.J
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enFinish, 0);
				}
			}
			break;

		case enHydroMeter3Finish :	// 유속계3 작업 종료 시점
			{
				if ( IsAllComplete() ) {	// 모든 작업이 끝나면 Finish로 이동한다.J
					::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enFinish, 0);
				}
			}
			break;
	}
}

//---------------------------------------------------------------------------
// 시간을 기준으로 측정 시간(PK값)을 계산한다. ( 00:00:00, 00:10:00, 00:20:00,... 23:50:00 )
// 기준 시간 => 매분 0초일 때마다 Fire
void TFormMain::GetMeasureDateTime(TDateTime now, UnicodeString &Date, UnicodeString &Time)
{
	USHORT hour, minute, second, msecond;
	now.DecodeTime(&hour, &minute, &second, &msecond);

	int addmin = 10 - (minute % 10);	// 05분 => 10분, 12분 => 20분 , 23:53:00 => 다음날 00:00:00으로 만들어 줌.

	TDateTime add = TDateTime(0,addmin,0,0);
	TDateTime newtime = 0;

	newtime = now + add;

	Date = newtime.FormatString("yyyymmdd");
	Time = newtime.FormatString("hhnn00");
}

//---------------------------------------------------------------------------
bool TFormMain::IsComplete()						// All Sensor is complete or not
{
	if ( m_pWaterLevel  != NULL && m_pWaterLevel->m_enStep  != enIdle ) return false;
	if ( m_pHydroMeter1 != NULL && m_pHydroMeter1->m_enStep != enIdle ) return false;
	if ( m_pHydroMeter2 != NULL && m_pHydroMeter2->m_enStep != enIdle ) return false;

	return true;	// All Sensor complete
}

//---------------------------------------------------------------------------
void TFormMain::ShowSetupInfo()
{
	TListItem *pItem = NULL;
	TStrings  *p;

	pItem = ListView->Items->Item[0];
	pItem->Caption = "지점코드";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_PointCode;
	p->Strings[1] = theEnv.m_PointName;
	p->Strings[2] = "";
	p->Strings[3] = "";

	pItem = ListView->Items->Item[1];
	pItem->Caption = "수위계";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_WaterType;
	if ( theEnv.m_WaterType == LT400_STR ) {
		p->Strings[0] = theEnv.m_WaterType + "+Atmos";
	}
	p->Strings[1] = theEnv.m_WaterPort;
	p->Strings[2] = theEnv.m_WaterHz;
	p->Strings[3] = theEnv.m_WaterSN;

	pItem = ListView->Items->Item[2];
	pItem->Caption = "1번유속계";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_HydroType1;
	p->Strings[1] = theEnv.m_HydroPort1;
	p->Strings[2] = theEnv.m_HydroHz1;
	p->Strings[3] = theEnv.m_HydroSN1;

	pItem = ListView->Items->Item[3];
	pItem->Caption = "2번유속계";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_HydroType2;
	p->Strings[1] = theEnv.m_HydroPort2;
	p->Strings[2] = theEnv.m_HydroHz2;
	p->Strings[3] = theEnv.m_HydroSN2;

	pItem = ListView->Items->Item[4];
	pItem->Caption = "3번유속계";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_HydroType3;
	p->Strings[1] = theEnv.m_HydroPort3;
	p->Strings[2] = theEnv.m_HydroHz3;
	p->Strings[3] = theEnv.m_HydroSN3;

	pItem = ListView->Items->Item[5];
	pItem->Caption = "측정/동작/시작";
	p = pItem->SubItems;
	p->Strings[0] = theEnv.m_Interval;
	p->Strings[1] = theEnv.m_WorkTime;
	p->Strings[2] = IntToStr(theEnv.m_StartTime) + "분";
	p->Strings[3] = "";

	//ListView->Columns->Items[0]->Width = 80;
	//ListView->Columns->Items[1]->Width = 80;
	//ListView->Columns->Items[2]->Width = 80;
}

//---------------------------------------------------------------------------
int  TFormMain::CalProgress(int iSpentTime)
{
	int Pos = 0;

	if ( theEnv.GetWorkingTime() == 0 )
		Pos = 0;
	else
		Pos = ((double)iSpentTime / (double)theEnv.GetWorkingTime()) * 100;

	return ( Pos < 0 ) ? 0 : (( Pos > 100 ) ? 100 : Pos);
}

//---------------------------------------------------------------------------
UnicodeString TFormMain::SpentTimeToCaption(int iSpentTime)	// 90초 => "01:30"
{
	int iMin, iSec;
	UnicodeString x = "";
	iMin = iSpentTime / 60;
	iSec = iSpentTime % 60;
	x = x.sprintf(L"%02d:%02d",iMin, iSec);

	return x;
}

//---------------------------------------------------------------------------
// Logger Status
bool TFormMain::IsAllComplete()
{
	if ( m_pWaterLevel != NULL ) {
		if ( m_pWaterLevel->m_enStep == enSensorStart ||
			 m_pWaterLevel->m_enStep == enSensorWorking ) return false;	// not finish
	}

	if ( m_pHydroMeter1 != NULL ) {
		if ( m_pHydroMeter1->m_enStep == enSensorStart ||
			 m_pHydroMeter1->m_enStep == enSensorWorking ) return false;
	}

	if ( m_pHydroMeter2 != NULL ) {
		if ( m_pHydroMeter2->m_enStep == enSensorStart ||
			 m_pHydroMeter2->m_enStep == enSensorWorking ) return false;
	}

	if ( m_pHydroMeter3 != NULL ) {
		if ( m_pHydroMeter3->m_enStep == enSensorStart ||
			 m_pHydroMeter3->m_enStep == enSensorWorking ) return false;
	}

	//if ( m_pFlowStream != NULL ) {
	//	if ( m_pFlowStream->m_enStep == enSensorStart ||
	//		 m_pFlowStream->m_enStep == enSensorWorking ) return false;
	//}

	return true;	// All Sensor Finish
}

//---------------------------------------------------------------------------
// Logger Status [WaterLevel][HydroMeter1][HydroMeter2][FlowStream]
WORD TFormMain::GetLoggerStatus()
{
	BYTE Water, HydroMeter1, HydroMeter2, FlowStream;

	Water       = m_pWaterLevel->m_enStep;
	HydroMeter1 = m_pHydroMeter1->m_enStep;
	HydroMeter2 = m_pHydroMeter2->m_enStep;
	FlowStream  = m_pFlowStream->m_enStep;

	WORD w = 0x0000;

	w = w | (((WORD)Water)       << 12);
	w = w | (((WORD)HydroMeter1) <<  8);
	w = w | (((WORD)HydroMeter2) <<  4);
	w = w | (((WORD)FlowStream)  <<  0);

	return w;
}

//---------------------------------------------------------------------------
// Logger의 상태를 DataSender에 보낸다.
void TFormMain::SendLoggerStatusToSender(EN_LOGGER_STEP enStep)
{
	if ( SenderSocket->Socket->Connected ) {
		BYTE Data[1024];
		memset(Data, 0x00, 1024);
		int Pos = 0;

		Data[0] = STX;
		Data[1] = MSG_TYPE_LoggerActionEvent;	// 0x01
		Data[2] = 20;	// 20Byte

		// Logging Status
		switch ( enStep )
		{
			case enIdle :
				Data[3] = 0x00;	// Idle
				break;
			case enStart:
				Data[3] = 0x01;	// Loggger Start
				break;
			case enWaterLevelStart:
			case enWaterLevelWorking:
			case enWaterLevelFinish:
			case enHydroMeter1Start:
			case enHydroMeter1Working:
			case enHydroMeter1Finish:
			case enHydroMeter2Start:
			case enHydroMeter2Working:
			case enHydroMeter2Finish:
				{
					Data[3] = 0x02;	// Logger logging ( Working )
					// HydroMeter1
					if ( m_pHydroMeter1 == NULL ) Data[4] = 0x00;	// HydroMeter1 Status
					else                          Data[4] = m_pHydroMeter1->m_enStep;	// 0x00:idle-Start-Working-Finish
					// HydroMeter2
					if ( m_pHydroMeter2 == NULL ) Data[5] = 0x00;	// HydroMeter2 Status
					else                          Data[5] = m_pHydroMeter2->m_enStep;	// 0x00:idle-Start-Working-Finish
					// WaterLevel
					if ( m_pWaterLevel == NULL )  Data[6] = 0x00;	// WaterLevel Status
					else                          Data[6] = m_pWaterLevel->m_enStep;	// 0x00:idle-Start-Working-Finish
				}
				break;
			case enFinish:
				Data[3] = 0x03;	// Finish
				break;
		}


		// Data and Time
		TDateTime now = Now();

		AnsiString sDay, sTime;
		sDay = now.FormatString("yyyymmdd");
		sTime = now.FormatString("hhnnss");

		memcpy(&Data[7], sDay.c_str(), 8);
		memcpy(&Data[15], sTime.c_str(), 6);

		Data[21] = 0x00;	//Reserved
		Data[22] = 0x00;	//Reserved

		Data[23] = MakeCRC(Data,23);
		Data[24] = ETX;

		SenderSocket->Socket->SendBuf(Data, 25);
		LogMessage("Send Logger status to DataSender");
	}
}

//---------------------------------------------------------------------------
// Logger의 상태를 DataViewer에 보낸다.
void TFormMain::SendLoggerStatusToViewer(EN_LOGGER_STEP enStep)
{
	if ( ServerSocket->Socket->ActiveConnections == 0 ) return;	// no connection
	// Make Frame Data

	BYTE Data[1024];
	memset(Data, 0x00, 1024);
	int Pos = 0;

	Data[0] = STX;
	Data[1] = MSG_TYPE_LoggerActionEvent;	// 0x01
	Data[2] = 8;	// 8Byte

	// Logging Status
	switch ( enStep )
	{
		case enIdle :
			Data[3] = 0x00;	// Idle
			break;
		case enStart:
			Data[3] = 0x01;	// Loggger Start
			break;
		case enWaterLevelStart:
		case enWaterLevelWorking:
		case enWaterLevelFinish:
		case enHydroMeter1Start:
		case enHydroMeter1Working:
		case enHydroMeter1Finish:
		case enHydroMeter2Start:
		case enHydroMeter2Working:
		case enHydroMeter2Finish:
			{
				Data[3] = 0x02;	// Logger logging ( Working )
				// HydroMeter1
				if ( m_pHydroMeter1 == NULL ) Data[4] = 0x00;	// HydroMeter1 Status
				else                          Data[4] = SENSOR_STATUS_TO_NETWORK(m_pHydroMeter1->m_enWorkingStep);	// 0x00:idle-Start-Working-Finish
				// HydroMeter2
				if ( m_pHydroMeter2 == NULL ) Data[5] = 0x00;	// HydroMeter2 Status
				else                          Data[5] = SENSOR_STATUS_TO_NETWORK(m_pHydroMeter2->m_enWorkingStep);	// 0x00:idle-Start-Working-Finish
				// WaterLevel
				if ( m_pWaterLevel == NULL )  Data[6] = 0x00;	// WaterLevel Status
				else                          Data[6] = SENSOR_STATUS_TO_NETWORK(m_pWaterLevel->m_enWorkingStep);	// 0x00:idle-Start-Working-Finish
				// FlowStream
				if ( m_pFlowStream == NULL )  Data[7] = 0x00;	// FlowStream Status
				else                          Data[7] = SENSOR_STATUS_TO_NETWORK(m_pFlowStream->m_enWorkingStep);	// 0x00:idle-Start-Working-Finish
			}
			break;
		case enFinish:
			Data[3] = 0x03;	// Finish
			break;
	}

	Data[ 8] = 0x00; 	// Reserved
	Data[ 9] = 0x00; 	// Reserved
	Data[10] = 0x00; 	// Reserved

	Data[11] = MakeCRC(Data,11);
	Data[12] = ETX;

	int iViewerCount = 0;
	int iSendCount = 0;

	for ( int i = 0 ; i < ServerSocket->Socket->ActiveConnections ; i++ ) {
		iSendCount = ServerSocket->Socket->Connections[i]->SendBuf(Data, 13);
		if ( iSendCount >= 0 ) iViewerCount++;
	}
	m_iViewerCount = iViewerCount;	// Alive Viewer Count
	SetViewerSocketIcon(EN_SOCKET_ACTION::enSocketConnected);

	LogMessage("Send Logger status to DataViewer");
}

//---------------------------------------------------------------------------
bool TFormMain::SendPing()
{
	if ( SenderSocket->Socket->Connected ) {
		BYTE Data[1024];
		memset(Data, 0x00, 1024);
		int Pos = 0;

		Data[0] = STX;
		Data[1] = MSG_TYPE_Ping;	// 0x01
		Data[2] = 0;	// Len
		Data[3] = MakeCRC(Data,3);
		Data[4] = ETX;

		SenderSocket->Socket->SendBuf(Data, 5);
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFormMain::SendPingAck()
{
	if ( SenderSocket->Socket->Connected ) {
		BYTE Data[1024];
		memset(Data, 0x00, 1024);
		int Pos = 0;

		Data[0] = STX;
		Data[1] = MSG_TYPE_PingAck;	// 0x01
		Data[2] = 0;	// Len
		Data[3] = MakeCRC(Data,3);
		Data[4] = ETX;

		SenderSocket->Socket->SendBuf(Data, 5);
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFormMain::SendRebootReq()
{
	if ( SenderSocket->Socket->Connected ) {
		BYTE Data[1024];
		memset(Data, 0x00, 1024);
		int Pos = 0;

		Data[0] = STX;
		Data[1] = MSG_TYPE_RebootReq;	// 0x01
		Data[2] = 0;	// Len
		Data[3] = MakeCRC(Data,3);
		Data[4] = ETX;

		SenderSocket->Socket->SendBuf(Data, 5);
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFormMain::SendRebootRes()
{
	if ( SenderSocket->Socket->Connected ) {
		BYTE Data[1024];
		memset(Data, 0x00, 1024);
		int Pos = 0;

		Data[0] = STX;
		Data[1] = MSG_TYPE_RebootRes;	// 0x01
		Data[2] = 0;	// Len
		Data[3] = MakeCRC(Data,3);
		Data[4] = ETX;

		SenderSocket->Socket->SendBuf(Data, 5);
		return true;
	} else {
		return false;
	}
}

//#define MSG_TYPE_Ping				   0x20		// 2017.09.21 heaven2
//#define MSG_TYPE_PingAck               0x21		// 2017.09.21 heaven2
//#define MSG_TYPE_RebootReq             0x22		// 2017.09.21 heaven2
//#define MSG_TYPE_RebootRes             0x23		// 2017.09.21 heaven2

//---------------------------------------------------------------------------
// Process Sender Message
bool TFormMain::ProcessSenderMessage(BYTE *pData, int Len)
{
	bool bResult = false;
	BYTE Type = *(pData + 1);	// Message Type

	//////////////////////////////////////////////////////////////////////
	// Ping
	if ( Type ==  MSG_TYPE_Ping ) {				// Ping Request
		bResult = SendPingAck();
		if ( !bResult ) LogMessage("Ping ack fail..");
		else            LogMessage("Ping acked..");

	//////////////////////////////////////////////////////////////////////
	// PingAck
	} else if ( Type == MSG_TYPE_PingAck ) {	// Ping Ack
		if ( m_FormSetup != NULL ) {
			m_FormSetup->DataSenderMessage(pData, Len);
		} else {
			LogMessage("PingAck received. SetupForm is not ready..");
		}

	//////////////////////////////////////////////////////////////////////
	// RebootReq
	} else if ( Type == MSG_TYPE_RebootReq ) {	// Reboot request
		bResult = SendRebootRes();
		if ( !bResult ) LogMessage("RebootRes failed..");
		else            LogMessage("RebootRes ok..");
		LogMessage("Logger PC reboot now ...!!");

		SleepMsg(1000);	// wait for while

#ifdef _DEBUG
		//ForcePCReboot();	// reboot PC => debug mode
#else
		ForcePCReboot();	// reboot PC => Release mode
#endif

	//////////////////////////////////////////////////////////////////////
	// RebootRes
	} else if ( Type == MSG_TYPE_RebootRes ) {	// Reboot response
		if ( m_FormSetup != NULL ) {
			m_FormSetup->DataSenderMessage(pData, Len);
		} else {
			LogMessage("RebootRes received. but SetupForm is not ready..");
		}

	//////////////////////////////////////////////////////////////////////
	// Unknown Message
	} else {
		LogMessage("[ProcessSenderMessage] Unknown message");
	}
	return true;
}

//---------------------------------------------------------------------------
//
//
// TServerSocket Event
//
//
//---------------------------------------------------------------------------


void __fastcall TFormMain::ServerSocketAccept(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataViewer accepted..");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataViewer connected..");

	m_iViewerCount++;

	SetViewerSocketIcon(EN_SOCKET_ACTION::enSocketConnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataViewer disconnected..");

	m_iViewerCount--;
	if ( m_iViewerCount < 0 ) m_iViewerCount = 0;

	SetViewerSocketIcon(EN_SOCKET_ACTION::enSocketDisconnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
	Socket->Close();	// Socket Handle close
	LogMessage("DataViewer Socket Error");

	SetViewerSocketIcon(EN_SOCKET_ACTION::enSocketError);
}


//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket)
{
	//LogWatchDog("Socket Read Event");
	static bool bBuffering = true;

	BYTE Data[2048] = "";

	int iRecvSize = Socket->ReceiveBuf(Data, 2048);

	for ( int i = 0 ; i < iRecvSize ; i++ ) {

		if ( Data[i] == STX ) {			// STX ---------------------------------
			if ( bBuffering ) {
				m_ViewerBuffer[m_RecvIndex++] = Data[i];
			} else {
				m_RecvIndex = 0;
				m_ViewerBuffer[m_RecvIndex++] = Data[i];
				bBuffering = true;
			}

		} else if ( Data[i] == ETX ) {	// ETX ---------------------------------
			if ( !bBuffering ) continue;
			m_ViewerBuffer[m_RecvIndex++] = Data[i];
			int iLen = m_ViewerBuffer[2];	// Length

			if ( m_RecvIndex == iLen + 5 ) {	// Match

				ViewerMessage(m_ViewerBuffer, m_RecvIndex);

				memset(m_ViewerBuffer, 0x00, BUFFER_8K);
				bBuffering = false;
				m_RecvIndex = 0;

			} else if ( m_RecvIndex > iLen + 5 ) {	// Frame is too long

				LogMessage("Frame is too long");
				UnicodeString sTemp = "";
				UnicodeString sMsg = "";
				for ( int i = 0 ; i < m_RecvIndex ; i++ ) {
					sTemp = sTemp.sprintf(L"%02X ", m_ViewerBuffer[i]);
					sMsg += sTemp;
				}
				LogMessage(sMsg);

				memset(m_ViewerBuffer, 0x00, BUFFER_8K);
				bBuffering = false;
				m_RecvIndex = 0;

			} else {								// Frame is too Short

				LogMessage("Frame is too short.. ETX in payload.. continue buffering..");

			}

		} else {						// Data(Payload) -----------------------

			if ( bBuffering ) {
				m_ViewerBuffer[m_RecvIndex++] = Data[i];      // continue buffering
			} else {
				// unknown contidion => nothing to do
				memset(m_ViewerBuffer, 0x00, BUFFER_8K);
				bBuffering = false;
				m_RecvIndex = 0;
			}
		}

		// Overflow check
		if ( m_RecvIndex >= BUFFER_8K ) {	// Buffer Overflow
			memset(m_ViewerBuffer, 0x00, BUFFER_8K);
			bBuffering = false;
			m_RecvIndex = 0;
		}
	} // end of for
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketListen(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataViewer port is listening..");
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Socket Event
void TFormMain::ViewerMessage(BYTE *pBuffer, int iLen)
{
	int iResult = FRAME_OK;
	iResult = CheckFrame(pBuffer, iLen);
	if ( iResult != FRAME_OK ) {
		if ( iResult == FRAME_STX_ERROR )      LogMessage("ViewerSocket STX Error");
		else if ( iResult == FRAME_ETX_ERROR ) LogMessage("ViewerSocket ETX Error");
		else if ( iResult == FRAME_CRC_ERROR ) LogMessage("ViewerSocket CRC Error");
		else                                   LogMessage("ViewerSocket Unknown Error");
		return;
	}

	BYTE MsgType = *(pBuffer+1);	// MessageType
	bool bResult = false;

	if ( MsgType == MSG_TYPE_LoggerSetupChange ) {   //	 Viewer에서 Setup 정보의 변경에 대한 알림.

		//LogMessage("The DataViewer request Logger Setup data changed. All Setup data is reloading..");
		LogMessage("The DataViewer notify Logger Setup data changed. DataSender will be restart..");

		BYTE Action = ACTION_SHUTDOWN_REQUEST;

		SendHeartBeatToWatchDog(Action);

		// heaven2 아래의 코드는 만약 WatchDog이 가용한 상태가 아니면 Logger가 Restart하지 못하므로
		// 다시 제원을 Reload하도록 시도 한다.
		// 위함한 코드이므로 일단 주석으로 막는다.
/*
		// Logger의 동작을 Clear시킨다.
		ClearLogger();

		// DB Config Reload
		bResult = theEnv.LoadEnv();
		if ( !bResult ) return;

		// Init Procedure
		InitLogger();

		ShowSetupInfo();	// Show Setup information

		SetWorkingIcon(EN_LOGGER_STEP::enIdle); 	// Working  icon init

		LogMessage("The setup is reloaded..");

		// Logger 상태를 Idle 상태로 만든다.
		::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enIdle, 0);
*/

	} else if ( MsgType == MSG_TYPE_LoggerUIOpen ) {	// SystemTray에 있다면 UI를 Open한다.

			RestoreFromTray();
			SetUIExtend(true);

			HWND hWnd = this->Handle;
			::SetForegroundWindow( hWnd );
			::SetFocus( hWnd );

	} else {
		// currently undefined..
	}
}

//---------------------------------------------------------------------------
//
//
// DataSender Socket Event
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataSender connected..");

	SetSenderSocketIcon(EN_SOCKET_ACTION::enSocketConnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("DataSender disconnected..");

	SetSenderSocketIcon(EN_SOCKET_ACTION::enSocketDisconnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderSocketError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
	Socket->Close();	// Socket Handle close
	//LogMessage("DataViewer error");

	SetSenderSocketIcon(EN_SOCKET_ACTION::enSocketError);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderSocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
	//LogWatchDog("Socket Read Event");
	static bBuffering = true;

	BYTE Data[1024] = "";

	int iRecvSize = Socket->ReceiveBuf(Data, 1024);

	for ( int i = 0 ; i < iRecvSize ; i++ ) {

		if ( Data[i] == STX ) {			// STX ---------------------------------
			if ( bBuffering ) {
				m_SenderBuffer[m_SenderIndex++] = Data[i];
			} else {
				m_SenderIndex = 0;
				m_SenderBuffer[m_SenderIndex++] = Data[i];
				bBuffering = true;
			}

		} else if ( Data[i] == ETX ) {	// ETX ---------------------------------
			if ( !bBuffering ) continue;
			m_SenderBuffer[m_SenderIndex++] = Data[i];
			int iLen = m_SenderBuffer[2];	// Length

			if ( m_SenderIndex == iLen + 5 ) {	// Match

				////////////////////////////////////////////////////////////////
				// Process Message
				////////////////////////////////////////////////////////////////
				ProcessSenderMessage(m_SenderBuffer, m_SenderIndex);

				memset(m_SenderBuffer, 0x00, sizeof(m_SenderBuffer));
				bBuffering = false;
				m_SenderIndex = 0;

			} else if ( m_SenderIndex > iLen + 5 ) {	// Frame is too long

				LogMessage("Frame is too long");
				UnicodeString sTemp = "";
				UnicodeString sMsg = "";
				for ( int i = 0 ; i < m_SenderIndex ; i++ ) {
					sTemp = sTemp.sprintf(L"%02X ", m_SenderBuffer[i]);
					sMsg += sTemp;
				}
				LogMessage(sMsg);

				memset(m_SenderBuffer, 0x00, sizeof(m_SenderBuffer));
				bBuffering = false;
				m_SenderIndex = 0;

			} else {								// Frame is too Short

				LogMessage("Frame is too short.. ETX in payload.. continue buffering..");

			}

		} else {						// Data(Payload) -----------------------

			if ( bBuffering ) {
				m_SenderBuffer[m_SenderIndex++] = Data[i];
			} else {
				// unknown contidion
				// nothing to do..
				memset(m_SenderBuffer, 0x00, sizeof(m_SenderBuffer));
				bBuffering = false;
				m_SenderIndex = 0;
			}
		}
	} // end of for
}


//---------------------------------------------------------------------------
//
//
// WatchDog Socket Event
//
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TFormMain::WatchDogSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("WatchDog connected..");

	SetWatchDogSocketIcon(EN_SOCKET_ACTION::enSocketConnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::WatchDogSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	LogMessage("WatchDog disconnected..");

	SetWatchDogSocketIcon(EN_SOCKET_ACTION::enSocketDisconnected);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::WatchDogSocketError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
	Socket->Close();	// Socket Handle Close
	//LogMessage("WatchDog socket error..");

	SetWatchDogSocketIcon(EN_SOCKET_ACTION::enSocketError);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::WatchDogSocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
	// Read Event
}

//---------------------------------------------------------------------------
//
//
// HydroMeter1 Port
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort1PortOpen(TObject *Sender)
{
	LogMessage("HydroMeter1 Port Opened");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort1PortClose(TObject *Sender)
{
	LogMessage("HydroMeter1 Port Closed");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort1TriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreHydroMeter1,0x00, sizeof(m_PreHydroMeter1));

	try {
		pGMasterPortSection->Enter();
		HydroPort1->GetBlock(m_PreHydroMeter1, Count);
		pGMasterPortSection->Leave();
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[HydroPort1TriggerAvail]") + e.Message);
		return;
	}

	AnsiString sRecvMsg = m_PreHydroMeter1;
	//LogMessage(AnsiString("HydroMeter1 Msg : ") + m_PreHydroMeter1);

	if ( m_pHydroSetup1 != NULL ) {	// 설정화면이 열려 있다면 그 쪽으로 데이터를 보낸다.
		m_pHydroSetup1->Receive(sRecvMsg);
		return;
	}

	if ( m_pHydroMeter1 != NULL ) {
		if ( typeid(*m_pHydroMeter1) == typeid(TAccusonic) || typeid(*m_pHydroMeter1) == typeid(TEWSVConverter) )	// Accusonic 장비일 때는 Binary Stream 데이터로 전달한다.
			m_pHydroMeter1->Receive((BYTE *)m_PreHydroMeter1, Count);	// Binary Stream 중간에 0x00이 데이터로 들어 있어서 이렇개 한다.
		else
			m_pHydroMeter1->Receive(sRecvMsg, Count);	// 그외 데이터는 AnsiString 형태로 전달한다.
	}
}

//---------------------------------------------------------------------------
//
//
// HydroMeter2 Port
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort2PortOpen(TObject *Sender)
{
	LogMessage("HydroMeter2 Port Opened");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort2PortClose(TObject *Sender)
{
	LogMessage("HydroMeter2 Port Closed");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort2TriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreHydroMeter2,0x00, sizeof(m_PreHydroMeter2));

	try {
		HydroPort2->GetBlock(m_PreHydroMeter2, Count);
		//LogMessage(AnsiString("HydroMeter2 Msg : ") + m_PreHydroMeter2);
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[HydroPort2TriggerAvail]") + e.Message);
		return;
	}

	AnsiString sRecvMsg = m_PreHydroMeter2;

	if ( m_pHydroSetup2 != NULL ) {	// 설정화면이 열려 있다면 그 쪽으로 데이터를 보낸다.
		m_pHydroSetup2->Receive(sRecvMsg);
		return;
	}

	if ( m_pHydroMeter2 != NULL ) {
		if ( typeid(*m_pHydroMeter2) == typeid(TAccusonic) )
			m_pHydroMeter2->Receive((BYTE *)m_PreHydroMeter2, Count);
		else
			m_pHydroMeter2->Receive(sRecvMsg, Count);
	}
}

//---------------------------------------------------------------------------
//
//
// HydroMeter3 Port
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort3PortOpen(TObject *Sender)
{
	LogMessage("HydroMeter3 Port Opened");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort3PortClose(TObject *Sender)
{
	LogMessage("HydroMeter3 Port Closed");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort3TriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreHydroMeter3,0x00, sizeof(m_PreHydroMeter3));

	try {
		HydroPort3->GetBlock(m_PreHydroMeter3, Count);
		//LogMessage(AnsiString("HydroMeter2 Msg : ") + m_PreHydroMeter2);
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[HydroPort3TriggerAvail]") + e.Message);
		return;
	}

	AnsiString sRecvMsg = m_PreHydroMeter3;

	if ( m_pHydroSetup3 != NULL ) {	// 설정화면이 열려 있다면 그 쪽으로 데이터를 보낸다.
		m_pHydroSetup3->Receive(sRecvMsg);
		return;
	}

	if ( m_pHydroMeter3 != NULL ) {
		if ( typeid(*m_pHydroMeter3) == typeid(TAccusonic) )
			m_pHydroMeter3->Receive((BYTE *)m_PreHydroMeter3, Count);
		else
			m_pHydroMeter3->Receive(sRecvMsg, Count);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::HydroPort3TriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	if ( HydroPort3->LineError != leNoError ) {
//		HydroPort2->FlushInBuffer();		// 일단 검증되지 않으면 보류.. 데이터 중에 일부분만 날아갈 수 있다. (버퍼에 있는 놈만)
//		HydroPort2->FlushOutBuffer();
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
// WaterLevel Port
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::WaterPortPortOpen(TObject *Sender)
{
	LogMessage("WaterLevel Port Opened");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::WaterPortPortClose(TObject *Sender)
{
	LogMessage("WaterLevel Port Closed");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::WaterPortTriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreWaterLevel,0x00, sizeof(m_PreWaterLevel));

	try {
		WaterPort->GetBlock(m_PreWaterLevel, Count);
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[WaterPortTriggerAvail]") + e.Message);
		return;
	}

	if ( typeid(*m_pWaterLevel) == typeid(TRnsWater)) {	// RnsWater 수위계=> Binary Stream
		m_pWaterLevel->Receive((BYTE *)m_PreWaterLevel, Count);
	} else {											// 기존 수위 데이터 => Text Stream
		AnsiString sRecvMsg = m_PreWaterLevel;
		if ( m_pWaterLevel != NULL ) {
			m_pWaterLevel->Receive(sRecvMsg, Count);
		}
	}
}

//---------------------------------------------------------------------------
// Logger의 운영 환경을 설정한다.
void __fastcall TFormMain::cbSetupClick(TObject *Sender)
{
	TModalResult mrResult;
	//TFormSetup *pSetup = new TFormSetup(this);
	m_FormSetup = new TFormSetup(this);
	bool bResult = false;

	LogMessage("Setup form open..", bGCSFileLog, bGCSDBLog);

	mrResult = m_FormSetup->ShowModal();

	LogMessage("Setup form closed..",bGCSFileLog, bGCSDBLog);

	if ( mrResult != mrOk )  {
		delete m_FormSetup;
		m_FormSetup = NULL;
		return;	// 변경이 없으므로
	}

	if ( m_FormSetup->m_bChanged ) {

		// Logger의 동작을 Clear시킨다.
		ClearLogger();

		// DB Config Reload
		bResult = theEnv.LoadEnv();
		if ( !bResult ) return;

		// Init Procedure
		InitLogger();

		ShowSetupInfo();	// Show Setup information

		SetWorkingIcon(EN_LOGGER_STEP::enIdle); 	// Working  icon init

		LogMessage("The setup is reloaded..");

		// Logger 상태를 Idle 상태로 만든다.
		::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enIdle, 0);
	}

	delete m_FormSetup;
	m_FormSetup = NULL;
}

//---------------------------------------------------------------------------
// Sensor(CM/SL/AG)의 운영 설정한다(장비 자체의 설정을 조정한다.)
void __fastcall TFormMain::cbSensorSetupClick(TObject *Sender)
{
	TListItem *pItem = ListView->Selected;
	if ( pItem == NULL ) {
		ShowMessage("유속계 장비를 선택하십시오.");
		return;
	}

	UnicodeString Devision, Sensor, Port;
	Devision = pItem->Caption;	// Devision
	TStrings *Sub = pItem->SubItems;
	Sensor = Sub->Strings[0];
	Port   = Sub->Strings[1];

	if ( Devision == "수위계" ) {
		ShowMessage("수위계의 설정은 장비 제조사의 설정 프로그램을 이용하여 주십시오.");
		return;
	}

	if ( Devision != "1번유속계" && Devision != "2번유속계" ) {
		ShowMessage("유속계 장비설정을 지원합니다. 일반 설정은 환경 설정 메뉴을 이용하십시오.");
		return;
	}

	THydroSetup *pHydroSetup = NULL;
	THydroMeter *pHydroMeter = NULL;
	TApdComPort *pComPort    = NULL;
	bool         bFirstHydro = false;

	if ( Devision == "1번유속계" ) {
		pHydroMeter = m_pHydroMeter1;
		pComPort    = HydroPort1;
		bFirstHydro = true;
	} else if ( Devision == "2번유속계") {
		pHydroMeter = m_pHydroMeter2;
		pComPort    = HydroPort2;
		bFirstHydro = false;
	} else {
		ShowMessage("유속계가 설정되어 있지 않습니다.");
		return;
	}

	if ( UpperCase(Sensor) == UpperCase("NONE")) {
		ShowMessage("설정되지 않은 장비입니다.");
		return;
	}

	if ( Sensor == CHANNELMASTER_STR ) {
		pHydroSetup = new THydroSetupCM(this);
		pHydroSetup->Init( pComPort, pHydroMeter, bFirstHydro);
	} else if ( Sensor == AGSERIES_STR ) {
		pHydroSetup = new THydroSetupAG(this);
		pHydroSetup->Init( pComPort, pHydroMeter, bFirstHydro);
	} else if ( Sensor == SLSERIES_STR ) {
		pHydroSetup = new THydroSetupSL(this);
		pHydroSetup->Init( pComPort, pHydroMeter, bFirstHydro);
	} else {
		ShowMessage("지원하지 않는 모델입니다.");
		return;
	}

	if ( bFirstHydro ) m_pHydroSetup1 = pHydroSetup;
	else               m_pHydroSetup2 = pHydroSetup;

	LogMessage("Sensor setup form open..", bGCSFileLog, bGCSDBLog);

	TModalResult Result;
	Result = pHydroSetup->ShowModal();

	LogMessage("Sensor setup form closed..", bGCSFileLog, bGCSDBLog);

	EN_EXIT_KIND enExitKind = pHydroSetup->m_enExit;

	//if ( Result == mrOk || Result == mrClose || Result == mrYes ) {
	if ( enExitKind == enOk ) {
		pHydroMeter->Clear();
		pHydroMeter->MeasureDate = m_MeasureDate;
		pHydroMeter->MeasureTime = m_MeasureTime;
		if ( m_bHydroBreak ) {
			pHydroMeter->SetBreak(true);
			LogMessage("[cbSensorSetupClick] HydroMeter Set Break");
		} else {}

	} else {
		//nothing to do => Cancel로 빠져나옴.
		// 동작 중에 Setup 화면을 열었다가 그냥나오면 이렇게 함.
	}

	delete pHydroSetup;

	if ( bFirstHydro ) m_pHydroSetup1 = NULL;
	else               m_pHydroSetup2 = NULL;
}

//---------------------------------------------------------------------------
void TFormMain::SendHeartBeatToWatchDog(BYTE Action)
{
	if ( WatchDogSocket->Socket->Connected ) {

		BYTE Data[1024];
		BYTE crc = 0x00;
		int  Pos = 0;
		memset(Data, 0x00, 1024);

		Data[Pos++] = STX;
		Data[Pos++] = MSG_TYPE_HeartbeatSignal;
		Data[Pos++] = 4;		// Length
		Data[Pos++] = 0x01;		// 0x01: Logger, 0x02: Datasender, 0x03: DataViewer
		Data[Pos++] = Action;	// Action:0x00: Normal HeartBeat, 0xFF: Shutdown Request
		Data[Pos++] = 0;		// Reserved
		Data[Pos++] = 0;		// Reserved

		crc = MakeCRC(Data, Pos);
		Data[Pos++] = crc;
		Data[Pos++] = ETX;

		try {
			WatchDogSocket->Socket->SendBuf(Data, Pos);
		} catch ( Exception &e ) {
			LogMessage(e.Message);
		}
		LogMessage("Send HeartBeat to WatchDog..");
	} else {
		LogMessage("Try to send HeartBeat to WatchDog.. But WatchDog Socket is not connected.");
	}
}

//---------------------------------------------------------------------------
// SL/AG Series의 설정 화면에서 닫았을 때 다시 Logging 시작 바로 전에 다시 시작함.
void __fastcall TFormMain::TimerBREAKTimer(TObject *Sender)
{
	if ( m_bHydroBreak ) {

		m_iRemainBreak = theEnv.GetRemainIdleTime(Now());

		if ( m_iRemainBreak <= 10 ) {	// 로깅 시작 10초전에 <BREAK> Command를 전송한다.
			TimerBREAK->Enabled = false;
			//m_bHydroBreak  = false;
			m_iRemainBreak = 0;

			if (  m_pHydroMeter1 != NULL &&
				  (typeid(*m_pHydroMeter1) == typeid(TChannelMaster) || typeid(*m_pHydroMeter1) == typeid(TSL500K) || typeid(*m_pHydroMeter1) == typeid(TAG1500K)) &&
				  m_pHydroMeter1->GetBreakStatus() ) {
				m_pHydroMeter1->SendBreak(500);	// It's take 1000 msecond ( 900 + 100 )
				LogMessage("HydroMeter1 started with <BREAK> command.");
			} else {}

			if (  m_pHydroMeter2 != NULL &&
				  (typeid(*m_pHydroMeter1) == typeid(TChannelMaster) || typeid(*m_pHydroMeter2) == typeid(TSL500K) || typeid(*m_pHydroMeter2) == typeid(TAG1500K)) &&
				  m_pHydroMeter2->GetBreakStatus() ) {
				m_pHydroMeter2->SendBreak(500);	// It's take 1000 msecond ( 900 + 100 )
				LogMessage("HydroMeter2 started with <BREAK> command.");
			} else {}

		} else {
			if ( m_iRemainBreak % 10 == 0 ) {
				UnicodeString sMsg;
				sMsg = sMsg.sprintf(L"HydroMeter will start after %d seconds later with <BREAK> Command.", m_iRemainBreak - 10);
				LogMessage(sMsg);
			} else {
				// nothing to do..
			}
		}
	}
}

//---------------------------------------------------------------------------
void TFormMain::SetFlowStreamIcon(EN_FLOWSTREAM_ICON enFlowStreamIcon)
{
	TBitmap *pBitmap = new TBitmap();
	ImageListFlow->GetBitmap(enFlowStreamIcon,pBitmap);	// siable status
	ImageFlow->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
void TFormMain::SetHydroMeter1Icon(EN_HYDROMETER_ICON enHydroMeterIcon)
{
	if ( m_pHydroMeter1 != NULL ) {
		if ( m_pHydroMeter1->HydroKind == CHANNELMASTER_STR ) {
			TBitmap *pBitmap = new TBitmap();
			ImageListCM->GetBitmap(enHydroMeterIcon,pBitmap);  	// normal status
			ImageHydro1->Picture->Bitmap = pBitmap;
			delete pBitmap;
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListSONTEK->GetBitmap(enHydroMeterIcon,pBitmap);  	// normal status
			ImageHydro1->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}
	}
}

//---------------------------------------------------------------------------
void TFormMain::SetHydroMeter2Icon(EN_HYDROMETER_ICON enHydroMeterIcon)
{
	if ( m_pHydroMeter2 != NULL ) {
		if ( m_pHydroMeter2->HydroKind == CHANNELMASTER_STR ) {
			TBitmap *pBitmap = new TBitmap();
			ImageListCM->GetBitmap(enHydroMeterIcon,pBitmap);  	// normal status
			ImageHydro2->Picture->Bitmap = pBitmap;
			delete pBitmap;
		} else {
			TBitmap *pBitmap = new TBitmap();
			ImageListSONTEK->GetBitmap(enHydroMeterIcon,pBitmap);  	// normal status
			ImageHydro2->Picture->Bitmap = pBitmap;
			delete pBitmap;
		}
	}
}

//---------------------------------------------------------------------------
void TFormMain::SetWaterLevelIcon(EN_WATERLEVEL_ICON enWaterLevelIcon)
{
	TBitmap *pBitmap = new TBitmap();
	ImageListWaterLevel->GetBitmap(enWaterLevelIcon,pBitmap);	// siable status
	ImageWaterLevel->Picture->Bitmap = pBitmap;
	delete pBitmap;
}

//---------------------------------------------------------------------------
// 매 분마다 Garbage Collection 루틴을 수행한다.
void TFormMain::MemoryGarbageCollection()		// Check Garbage Collection
{
    HANDLE LHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (LHandle != NULL)
	{
        // SetProcessWorkingSetSize to -1 means trim as much as possible
        SetProcessWorkingSetSize(LHandle, (SIZE_T)-1, (SIZE_T)-1);
        CloseHandle(LHandle);
	}
}

//---------------------------------------------------------------------------
// Atmospheric Sensor opened
void __fastcall TFormMain::AtmosPortPortOpen(TObject *Sender)
{
	LogMessage("Atmospheric Sensor opened..");
}

//---------------------------------------------------------------------------
// Atmospheric Sensor closed
void __fastcall TFormMain::AtmosPortPortClose(TObject *Sender)
{
	LogMessage("Atmospheric Sensor closed..");
}

//---------------------------------------------------------------------------
// Atmospheric Sensor data received
void __fastcall TFormMain::AtmosPortTriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreAtmos,0x00, sizeof(m_PreAtmos));

	try {
		AtmosPort->GetBlock(m_PreAtmos, Count);
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[AtmosPortTriggerAvail]") + e.Message);
		return;
	}

	AnsiString sRecvMsg = m_PreAtmos;
	if ( m_pAtmos != NULL ) {
		m_pAtmos->Receive(sRecvMsg, Count);
	}
}

//---------------------------------------------------------------------------
// WaterPort Line error detech event      2021.12.01
void __fastcall TFormMain::WaterPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	if ( WaterPort->LineError != leNoError ) {
		WaterPort->FlushInBuffer();
		WaterPort->FlushOutBuffer();
	}
}

//---------------------------------------------------------------------------
// Airmar Port Open
void __fastcall TFormMain::AirmarPortPortOpen(TObject *Sender)
{
	LogMessage("Airmar Port opened..");
}

//---------------------------------------------------------------------------
// Airmar Port Closed
void __fastcall TFormMain::AirmarPortPortClose(TObject *Sender)
{
	LogMessage("Airmar Port closed..");
}

//---------------------------------------------------------------------------
// Airmar Port received Event
void __fastcall TFormMain::AirmarPortTriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreAirmar,0x00, sizeof(m_PreAirmar));

	try {
		AirmarPort->GetBlock(m_PreAirmar, Count);
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[AirmarReadEvent]") + e.Message);
		return;
	}

	if ( m_pAirmar->m_bLinker ) {	// AirmarLinker를 통해 받으면
		m_pAirmar->ReceiveLinker(m_PreAirmar, Count);
	} else {							// Airmar Sensor에서 직접 받으면
		AnsiString sRecvMsg = (char *)m_PreAirmar;
		if ( m_pAirmar != NULL ) {
			m_pAirmar->Receive(sRecvMsg, Count);
		}
	}
}

//---------------------------------------------------------------------------
// Airmar Port Line Error Event
void __fastcall TFormMain::AirmarPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	// 2021.12.06 heaven2
	if ( AirmarPort->LineError != leNoError ) {
		AirmarPort->FlushInBuffer();
		AirmarPort->FlushOutBuffer();
	}
}

//---------------------------------------------------------------------------
// HydroPort1 Line Error
void __fastcall TFormMain::HydroPort1TriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	// 2021.12.06 heaven2
	if ( HydroPort1->LineError != leNoError ) {
//		HydroPort1->FlushInBuffer();     	// 일단 검증되지 않으면 보류.. 데이터 중에 일부분만 날아갈 수 있다. (버퍼에 있는 놈만)
//		HydroPort1->FlushOutBuffer();
	}
}

//---------------------------------------------------------------------------
// HydroPort2 Line error
void __fastcall TFormMain::HydroPort2TriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	// 2021.12.06 heaven2
	if ( HydroPort2->LineError != leNoError ) {
//		HydroPort2->FlushInBuffer();		// 일단 검증되지 않으면 보류.. 데이터 중에 일부분만 날아갈 수 있다. (버퍼에 있는 놈만)
//		HydroPort2->FlushOutBuffer();
	}
}

//---------------------------------------------------------------------------
// Atmos Port Line Error
void __fastcall TFormMain::AtmosPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	// 2021.12.06 heaven2
	if ( AtmosPort->LineError != leNoError ) {
		AtmosPort->FlushInBuffer();
		AtmosPort->FlushOutBuffer();
	}
}

//---------------------------------------------------------------------------
// Function Test
void __fastcall TFormMain::ButtonTestClick(TObject *Sender)
{
//	if ( !AirmarPort->Open ) {
//		LogMessage("Airmar Port is not open");
//		return;
//	}
//
//	BYTE Buffer[128] = { 0, };
//	Buffer[0] = STX;
//	Buffer[1] = 0x01;	// AirmarDataReq
//	Buffer[2] = 0x00;	// Length
//	BYTE crc = 0x00;
//	for ( int i = 0 ; i < 3 ; i++ ) {
//		crc ^= Buffer[i];
//	}
//	crc = crc & 0x7F;
//	Buffer[3] = crc;
//	Buffer[4] = ETX;
//
//	try {
//		AirmarPort->PutBlock(Buffer, 5);
//		LogMessage("AirmarLinker data request..");
//	} catch ( Exception &e ) {
//		LogMessage("Airmar Port Send Error :" + e.Message);
//	}

//	::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enStart, 0);

//	::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Start, 0);
//	::PostMessage(this->Handle, UM_MESSAGE, EN_LOGGER_STEP::enRQ30Finish, 0);
//
//
//	theEnv.m_HydroUseSlavePort    = "ON";
//	theEnv.m_HydroMasterAddress   = "5,4,3,2,1";
//	theEnv.m_HydroMasterPortCount = 5;
//	theEnv.m_HydroSlaveAddress    = "6,7,8,9,10";
//	theEnv.m_HydroSlavePortCount  = 5;
//
//	THydroMeter *pHydro = new TRQ30D();
//	TRQ30D *pRQ30D = (TRQ30D *)pHydro;
//	pRQ30D->MasterStatus.SetDeviceAddress(theEnv.m_HydroMasterAddress);
//	pRQ30D->SlaveStatus.SetDeviceAddress(theEnv.m_HydroSlaveAddress);
//
//	STRQ30DData *pData = NULL;
//	TList *pList = NULL;
//
//	// MasterPort --------------------------------------------------------------
//
//	// Address 5
//	pList = pRQ30D->GetMasterListByAddress(5); // Address 5
//	pData = new STRQ30DData();
//	pData->Address = 5;
//	pData->Velocity = -99.00f;
//	pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 5; pData->Velocity = -99.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 5; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 5; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 5; pData->Velocity =   1.00f; pList->Add(pData);
//
//	// Address 4
//	pList = pRQ30D->GetMasterListByAddress(4); // Address 4
//	pData = new STRQ30DData(); pData->Address = 4; pData->Velocity =   0.10f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 4; pData->Velocity =   0.20f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 4; pData->Velocity =   0.05f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 4; pData->Velocity =   0.30f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 4; pData->Velocity =   0.50f; pList->Add(pData);
//
//	// Address 3
//	pList = pRQ30D->GetMasterListByAddress(3); // Address 3
//	pData = new STRQ30DData(); pData->Address = 3; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 3; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 3; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 3; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 3; pData->Velocity =   1.00f; pList->Add(pData);
//
//	// Address 2
//	pList = pRQ30D->GetMasterListByAddress(2); // Address 2
//	pData = new STRQ30DData(); pData->Address = 2; pData->Velocity = -99.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 2; pData->Velocity = -99.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 2; pData->Velocity = -99.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 2; pData->Velocity = -99.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 2; pData->Velocity = -99.00f; pList->Add(pData);
//
//	// Address 1
//	pList = pRQ30D->GetMasterListByAddress(1); // Address 1
//	pData = new STRQ30DData(); pData->Address = 1; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 1; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 1; pData->Velocity =   0.50f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 1; pData->Velocity =   1.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 1; pData->Velocity =   0.00f; pList->Add(pData);
//
//	// SlavePort --------------------------------------------------------------
//
//	// Address 6
////	pList = pRQ30D->GetSlaveListByAddress(6); // Address 5
////	pData = new STRQ30d(); pData->Address = 5; pData->Velocity = -99.00f; pList->Add(pData);
////	pData = new STRQ30d(); pData->Address = 5; pData->Velocity = -99.00f; pList->Add(pData);
////	pData = new STRQ30d(); pData->Address = 5; pData->Velocity =   0.00f; pList->Add(pData);
////	pData = new STRQ30d(); pData->Address = 5; pData->Velocity =   0.00f; pList->Add(pData);
////	pData = new STRQ30d(); pData->Address = 5; pData->Velocity =   1.00f; pList->Add(pData);
//
//	// Address 7
//	pList = pRQ30D->GetSlaveListByAddress(7); // Address 7
//	pData = new STRQ30DData(); pData->Address = 7; pData->Velocity = -99.00f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address = 7; pData->Velocity = -99.00f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address = 7; pData->Velocity = -99.00f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address = 7; pData->Velocity = -99.00f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address = 7; pData->Velocity = -99.00f; pList->Add(pData);
//
//	// Address 8
//	pList = pRQ30D->GetSlaveListByAddress(8); // Address 8
//	pData = new STRQ30DData(); pData->Address = 8; pData->Velocity =   0.10f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 8; pData->Velocity =   0.09f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 8; pData->Velocity =   0.08f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 8; pData->Velocity =   0.15f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 8; pData->Velocity =   0.13f; pList->Add(pData);
//
//	// Address 9
//	pList = pRQ30D->GetSlaveListByAddress(9); // Address 9
//	pData = new STRQ30DData(); pData->Address = 9; pData->Velocity =   0.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 9; pData->Velocity =   1.00f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 9; pData->Velocity =   1.30f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 9; pData->Velocity =   1.20f; pList->Add(pData);
//	pData = new STRQ30DData(); pData->Address = 9; pData->Velocity = -99.00f; pList->Add(pData);
//
//	// Address 10
//	pList = pRQ30D->GetSlaveListByAddress(10); // Address 10
//	pData = new STRQ30DData(); pData->Address =10; pData->Velocity =   1.00f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address =10; pData->Velocity =   0.20f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address =10; pData->Velocity =   0.05f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address =10; pData->Velocity =   0.30f; pList->Add(pData);
//	//pData = new STRQ30d(); pData->Address =10; pData->Velocity =   0.50f; pList->Add(pData);
//
//	pRQ30D->CaculateRQ30DData();
//
//	AnsiString Str = "";
//	AnsiString Temp = "";
//	STRQ30DSet *pSet = &(pRQ30D->RQ30DFinalSet);
//
//	for ( int i = 0 ; i < 10 ; i++  ) {
//		Str = IntToStr(i) + "=>Address:" + IntToStr(pSet->RQ30DData[i].Address) + " =>Velocity:" + FloatToStrF(pSet->RQ30DData[i].Velocity, ffFixed, 5,2) ;
//		LogMessage(Str);
//	}
//	Str = "TotalQ:" + FloatToStrF(pSet->TotalQ, ffFixed, 5,2);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::RQSlavePortPortOpen(TObject *Sender)
{
	LogMessage("RQ Slave Port opened..");
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::RQSlavePortPortClose(TObject *Sender)
{
	LogMessage("RQ Slave Port closed..");
}

//---------------------------------------------------------------------------
// RQ30d Slave Port receive event
void __fastcall TFormMain::RQSlavePortTriggerAvail(TObject *CP, WORD Count)
{
	memset(m_PreRQBuffer,0x00, sizeof(m_PreRQBuffer));

	try {
		pGSlavePortSection->Enter();
		RQSlavePort->GetBlock(m_PreRQBuffer, Count);
		pGSlavePortSection->Leave();
	} catch ( Exception &e ) {
		LogMessage(UnicodeString("[RQSlavePortTriggerAvail]") + e.Message);
		return;
	}

	AnsiString sRecvMsg = m_PreRQBuffer;
	//LogMessage(AnsiString("HydroMeter1 Msg : ") + m_PreHydroMeter1);

	if ( m_pHydroMeter1 != NULL ) {
		if ( typeid(*m_pHydroMeter1) == typeid(TRQ30D) ) {	// RQSlavePort는 RQ-30d에만 해당된다.
			((TRQ30D *)m_pHydroMeter1)->ReceiveSlave(sRecvMsg, Count);	// 그외 데이터는 AnsiString 형태로 전달한다.
		} else {
			// nothing to do..
		}
	}
}

//---------------------------------------------------------------------------
// RQ30d Slave Port Error event
void __fastcall TFormMain::RQSlavePortTriggerLineError(TObject *CP, WORD Error, bool LineBreak)
{
	if ( HydroPort1->LineError != leNoError ) {
//		HydroPort1->FlushInBuffer();     	// 일단 검증되지 않으면 보류.. 데이터 중에 일부분만 날아갈 수 있다. (버퍼에 있는 놈만)
//		HydroPort1->FlushOutBuffer();
	}
}


