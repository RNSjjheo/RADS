//---------------------------------------------------------------------------

#include <vcl.h>
#include <Windows.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "UnitMain.h"
#include "DvEnv.h"
#include "UnitLeft.h"
#include "UnitLogin.h"
#include "fbTable.h"
#include "LockeCommon.h"
#include "PacketBuffer.h"
#include "UnitConfig.h"
#include "UnitHydroMeter.h"
#include "UnitAbout.h"
#include "UnitLog.h"
#include "UnitUser.h"
#include "UnitReport.h"
#include "UnitFlow.h"
#include "UnitWaterLevel.h"
#include "UnitVTH.h"
#include "UnitSearchData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormMain *FormMain;


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

//---------------------------------------------------------------------------
//
// TFormMain
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
    iClickedTick = GetTickCount();
	bLoggedIn    = true;
	bRunning     = false;

	m_enLoggerStatus = enLoggerIdle;
	m_enSenderStatus = enSenderIdle;

	m_bLoggerConnect = false;
	m_bSenderConnect = false;

	memset(m_LoggerBuffer, 0x00, 1024);
	memset(m_SenderBuffer, 0x00, 1024);
	m_LoggerIndex = 0;
	m_SenderIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrDisplayTimeTimer(TObject *Sender)
{
	UnicodeString sNow = Now().FormatString("hh:nn:ss");
	stTime->Caption = sNow;
	//stTime->Caption = DBEnv.sLoggedUserID + " " + sNow;
	//stTime->Left = ClientWidth - stTime->Width - 5;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormResize(TObject *Sender)
{
	// Minium Size : 1260 * 768
	if ( this->Width  < 1260 ) this->Width  = 1260;
	if ( this->Height <  768 ) this->Height =  768;

    ArrangeChildren();
}
//---------------------------------------------------------------------------
bool TFormMain::DoLogin()
//사용자 로그인
{
    UnicodeString sLoggedID = ShowLogin(DBEnv.sLoggedUserID);
    if (sLoggedID != "")
    {
        iClickedTick = GetTickCount();
        WriteLog(!CbFileWrite, CbDBWrite, DBEnv.sLoggedUserID + " 로그인하였습니다.");
    } // if
    return bLoggedIn;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SplitterMoved(TObject *Sender)
{
	SendMessage(this->Handle, WM_SIZE, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
//  2014.09.30 Session Expiration 안 하기로 함 => 주석 처리.
//	if (FormLogin) //로그인창이 떠있으면 메시지 필터링 안함
//    {
//        Handled = False;
//        return;
//    } // if
//    if (Msg.message == WM_LBUTTONDOWN || Msg.message == WM_LBUTTONUP)
//    {
//        bool bLogged = False;
//        int iCurrent = (GetTickCount() - iClickedTick) / 1000;
//        int iLast = DBEnv.iLoginDelayMinute * 60;
//        if (iCurrent > iLast) //대기시간 초과시 로그인창 띄움
//        {
//            Handled = True;
//            if (!DoLogin()) return;
//        } // if
//        iClickedTick = GetTickCount();
//    } // if
//	if (Msg.message == WM_USER) QueryAllChildren();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SplitterCanResize(TObject *Sender, int &NewSize, bool &Accept)
{
    if (NewSize == 1) NewSize = 2;
    Accept = True;
}
//---------------------------------------------------------------------------
void TFormMain::ReadIni()
{
    UnicodeString sIniFile = ExtractFilePath(Application->ExeName) + "DataViewer.ini";
    TIniFile *ini = new TIniFile(sIniFile);

    DBEnv.sDatabaseIP      = ini->ReadString("DATABASE", "DatabaseIP"     , "127.0.0.1");
    DBEnv.sDatabaseName    = ini->ReadString("DATABASE", "DatabaseName"   , "RADS");
    DBEnv.sDatabaseAccount = ini->ReadString("DATABASE", "DatabaseAccount", "rads");

	SenderClient->Host = DBEnv.sDatabaseIP;
    LoggerClient->Host = DBEnv.sDatabaseIP;

    delete ini;
}
//---------------------------------------------------------------------------
bool TFormMain::ConnectDB()
{
    bool bResult = True;

	SQLConnection->Close();
	SQLConnection->Params->Values["Database"] = DBEnv.sDatabaseIP + ":" + DBEnv.sDatabaseName;
	SQLConnection->Params->Values["User_Name"] = DBEnv.sDatabaseAccount;
	try
	{
		SQLConnection->Open();
	} // try
	catch(Exception *e)
	{
		bResult = False;
	} // catch

    return bResult;
}
//---------------------------------------------------------------------------
//void TFormMain::GetDBEnvInfo()
bool TFormMain::GetDBEnvInfo()
//필요한 DB관련 정보를 읽기
{
	if ( !SQLConnection->Connected ) return false;

    TfbTable *RSetup = new TfbTable(SQLConnection, "rsetup");
    UnicodeString sErrorMessage;

    //지점정보
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "1");
	RSetup->Open();
    DBEnv.sPointCode    = RSetup->ValueByName("Value01"); //지점코드
    DBEnv.sPointName    = RSetup->ValueByName("Value02"); //지점명
    DBEnv.sPointAddress = RSetup->ValueByName("Value03"); //지점 주소
    DBEnv.sPointZipCode = RSetup->ValueByName("Value04"); //지점 우편번호
    DBEnv.sPointManager = RSetup->ValueByName("Value05"); //지점 담당자
    DBEnv.sPointTel     = RSetup->ValueByName("Value06"); //담당자 연락처
    DBEnv.sPointEtc     = RSetup->ValueByName("Value07"); //특이사항

    //공통사항
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "2");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnv.sPubRadsHome    = RSetup->ValueByName("Value01"); //Rads Home 기본폴더
        DBEnv.sPubLoggerHome  = RSetup->ValueByName("Value02"); //logger home
        DBEnv.sPubSenderHome  = RSetup->ValueByName("Value03"); //sender home
        DBEnv.sPubViewerHome  = RSetup->ValueByName("Value04"); //viewer home
        DBEnv.sDataSenderIP   = RSetup->ValueByName("Value06");
        DBEnv.iDataSenderPort = RSetup->ValueByName("Value10").ToIntDef(5011);
        DBEnv.sDataLoggerIP   = RSetup->ValueByName("Value05");
        DBEnv.iDataLoggerPort = RSetup->ValueByName("Value08").ToIntDef(5001);
    } // if

    //화면 graph 옵션
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "4");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnv.bMDIWaterFlow   = RSetup->ValueByName("Value01").UpperCase() == "ON"; //유량산출
        DBEnv.bMDIWaterLevel  = RSetup->ValueByName("Value02").UpperCase() == "ON"; //수위계
        DBEnv.bMDIHydroMeter1 = RSetup->ValueByName("Value03").UpperCase() == "ON"; //하층유속계
        DBEnv.bMDILog         = RSetup->ValueByName("Value04").UpperCase() == "ON"; //로그창
        DBEnv.bMDIVth         = RSetup->ValueByName("Value05").UpperCase() == "ON"; //vth logger
    } // if
    //이메일 정보
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "5");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnv.bEmailAutoSend      = RSetup->ValueByName("Value01").UpperCase() == "ON"; //자동전송 유무
        DBEnv.sEmailServerAddress = RSetup->ValueByName("Value02"); //smtp 메일서버 주소
        DBEnv.sEmailAccount       = RSetup->ValueByName("Value03"); //id
        DBEnv.sEmailPassword      = RSetup->ValueByName("Value04"); //pw
        DBEnv.sEmailDest1         = RSetup->ValueByName("Value05"); //수신자1 -> 자신 rads@rnsea.com
        DBEnv.sEmailDest2         = RSetup->ValueByName("Value06"); //수신자2
        DBEnv.sEmailDest3         = RSetup->ValueByName("Value07"); //수신자3
        DBEnv.sEmailDest4         = RSetup->ValueByName("Value08"); //수신자4
        DBEnv.sEmailDest5         = RSetup->ValueByName("Value09"); //수신자5
    } // if

    //운영정보
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "1");
    RSetup->SetWhereParam("PK2", "3");
	sErrorMessage = RSetup->Open();
    if (sErrorMessage == "")
    {
        DBEnv.sRStreamMethod    = RSetup->ValueByName("Value02"); //유량산출방식
        DBEnv.iMSL              = RSetup->ValueByName("Value03").ToIntDef(0); //해발표고
        DBEnv.sCctvUrl          = RSetup->ValueByName("Value04"); //cctv url
        DBEnv.iLoginDelayMinute = RSetup->ValueByName("Value05").ToIntDef(1); //로그인 대기시간
    } // if

//장비제원 화면에 표시
	//FormLeft->mmSpec->Clear();
	//FormLeft->mmSpec->Lines->Add("지점코드: " + DBEnv.sPointCode + " " + DBEnv.sPointName); //지점
	FormLeft->SetPointInfo(DBEnv.sPointCode, DBEnv.sPointName);
    //유속계1
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
    RSetup->SetWhereParam("PK2", "2");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		//FormLeft->mmSpec->Lines->Add("하층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		UnicodeString sHydro, sPort, sHz, sSN;
		sHydro = RSetup->ValueByName("Value01");
		sPort  = RSetup->ValueByName("Value02");
		sHz    = RSetup->ValueByName("Value11");
		sSN    = RSetup->ValueByName("Value12");
		FormLeft->SetHydroMeter(1, sHydro, sPort, sHz, sSN);
		DBEnv.sHD1Kind  = RSetup->ValueByName("Value01");
		DBEnv.iAG1_WN   = RSetup->ValueByName("Value07").ToIntDef(40); //하층유속계 셀 개수
		DBEnv.bReverse1 = RSetup->ValueByName("Value10") == "ON"; //하층유속계 리버스 옵션
	} // if
    //유속계2
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
    RSetup->SetWhereParam("PK2", "3");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		//FormLeft->mmSpec->Lines->Add("상층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		UnicodeString sHydro, sPort, sHz, sSN;
		sHydro = RSetup->ValueByName("Value01");
		sPort  = RSetup->ValueByName("Value02");
		sHz    = RSetup->ValueByName("Value12");
		sSN    = RSetup->ValueByName("Value13");
		FormLeft->SetHydroMeter(2, sHydro, sPort, sHz, sSN);
		DBEnv.sHD2Kind  = RSetup->ValueByName("Value01");
		DBEnv.iAG2_WN   = RSetup->ValueByName("Value07").ToIntDef(40); //상층유속계 셀 개수
        DBEnv.bReverse2 = RSetup->ValueByName("Value10") == "ON"; //상층유속계 리버스 옵션
	} // if
	//유속계3
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "5");  	// 3번 유속계
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		//FormLeft->mmSpec->Lines->Add("상층유속계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		UnicodeString sHydro, sPort, sHz, sSN;
		sHydro = RSetup->ValueByName("Value01");
		sPort  = RSetup->ValueByName("Value02");
		sHz    = RSetup->ValueByName("Value12");
		sSN    = RSetup->ValueByName("Value13");
		FormLeft->SetHydroMeter(3, sHydro, sPort, sHz, sSN);
		DBEnv.sHD3Kind  = RSetup->ValueByName("Value01");
		DBEnv.iAG3_WN   = RSetup->ValueByName("Value07").ToIntDef(40); //상층유속계 셀 개수
		DBEnv.bReverse3 = RSetup->ValueByName("Value10") == "ON"; //상층유속계 리버스 옵션
	} // if
    //수위계
	RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "4");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		DBEnv.sWaterLevelDeviceKind = RSetup->ValueByName("Value01"); //수위계 종류
		//FormLeft->mmSpec->Lines->Add("수위계: " + RSetup->ValueByName("Value01") + "(" + RSetup->ValueByName("Value02") + ")");
		DBEnv.sWaterUseFlag = RSetup->ValueByName("Value13"); //수위계 종류
		if ( DBEnv.sWaterUseFlag.IsEmpty() ) DBEnv.sWaterUseFlag = WATERLEVEL_STR;	// default
		UnicodeString sWaterLevel, sPort, sHz, sSN, sAtmosPort;
		sWaterLevel = RSetup->ValueByName("Value01");
		sPort       = RSetup->ValueByName("Value02");
		//DBEnv.fWaterLevelOffset = RSetup->ValueByName("Value04").ToDouble();  //heaven2
		UnicodeString sWaterLevelOffset;
		sWaterLevelOffset = RSetup->ValueByName("Value04");  //heaven2
		if ( sWaterLevelOffset.IsEmpty() ) {
			DBEnv.fWaterLevelOffset = 0;
		} else {
			try {
				DBEnv.fWaterLevelOffset = sWaterLevelOffset.ToDouble();
			} catch ( Exception &e ) {
#ifdef _DEBUG
				ShowMessage(UnicodeString("WaterLevel Offset Load error =>") + e.Message);
#endif
				DBEnv.fWaterLevelOffset = 0;
			}
		}
		sHz         = RSetup->ValueByName("Value05");
		sSN         = RSetup->ValueByName("Value06");
		sAtmosPort  = RSetup->ValueByName("Value07");
		FormLeft->SetWaterLevel(sWaterLevel, sPort, sHz, sSN, sAtmosPort);
	} // if
    //vth logger
    RSetup->ClearParams();
    RSetup->SetWhereParam("PK1", "20");
    RSetup->SetWhereParam("PK2", "2");
    sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		//FormLeft->mmSpec->Lines->Add("VTH Logger: " + RSetup->ValueByName("Value05"));
		UnicodeString sPort = RSetup->ValueByName("Value05");
		FormLeft->SetVTHInfo(sPort);
	} // if
    //측정주기
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "10");
	RSetup->SetWhereParam("PK2", "1");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		UnicodeString sInterval, sPeriod, sStart;
		sInterval = RSetup->ValueByName("Value01");
		sPeriod   = RSetup->ValueByName("Value02");
		sStart    = RSetup->ValueByName("Value03");
		FormLeft->SetWorkingInfo(sInterval, sPeriod, sStart);
		//FormLeft->mmSpec->Lines->Add("측정주기/동작/시작=" + GetSepItem(RSetup->ValueByName("Value01"), 0, ":") + ":" + GetSepItem(RSetup->ValueByName("Value01"), 1, ":") + "/" +
		//                                                     GetSepItem(RSetup->ValueByName("Value02"), 0, ":") + ":" + GetSepItem(RSetup->ValueByName("Value02"), 1, ":") + "/" +
		//                                                     RSetup->ValueByName("Value03")
		//                            );
	} // if

	//heaven2 2014.09.05 유효범위 ----------------------------------------------

	//수위계 유효범위
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "20");
	RSetup->SetWhereParam("PK2", "11");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		UnicodeString sValue01, sValue02;
		sValue01 = RSetup->ValueByName("Value01");
		sValue02 = RSetup->ValueByName("Value02");
		try {
			DBEnv.fWaterLowerBound = sValue01.ToDouble();	// Min
			DBEnv.fWaterUpperBound = sValue02.ToDouble();	// Max
		} catch ( Exception &e ) {
			DBEnv.fWaterLowerBound = 0;	// Min
			DBEnv.fWaterUpperBound = 0;	// Max
		}

		if ( DBEnv.fWaterLowerBound == 0 ) DBEnv.fWaterLowerBound = -10.0f;	// Init Value(Min Value)
		if ( DBEnv.fWaterUpperBound == 0 ) DBEnv.fWaterUpperBound =  25.0f;	// Init Value(Max Value)
	}
	//유속계 유효범위
	RSetup->ClearParams();
	RSetup->SetWhereParam("PK1", "20");
	RSetup->SetWhereParam("PK2", "12");
	sErrorMessage = RSetup->Open();
	if (sErrorMessage == "")
	{
		UnicodeString sValue09, sValue10;
		sValue09 = RSetup->ValueByName("Value09");		// Min(EW)
		sValue10 = RSetup->ValueByName("Value10");		// Max(EW)
		try {
			DBEnv.fHydroLowerBound = sValue09.ToDouble() * 1000;	// Min
			DBEnv.fHydroUpperBound = sValue10.ToDouble() * 1000;	// Max
		} catch ( Exception &e ) {
			DBEnv.fHydroLowerBound = 0;	// Min
			DBEnv.fHydroUpperBound = 0;	// Max
		}

		if ( DBEnv.fHydroLowerBound == 0) DBEnv.fHydroLowerBound = -15.0f * 1000; // m => mm
		if ( DBEnv.fHydroUpperBound == 0) DBEnv.fHydroUpperBound =  15.0f * 1000; // m => mm
	}

	FormLeft->ShowSetupInfo();	// Show Setup info

	delete RSetup;

	return true;
}
//---------------------------------------------------------------------------
void TFormMain::_WriteDBLog(UnicodeString sDateTime, UnicodeString sText)
{
	if ( !SQLConnection->Connected ) return;

    TfbTable *RViewerLog = new TfbTable(SQLConnection, "RViewerLog");
    RViewerLog->ClearParams();
    RViewerLog->SetFieldParam("LOGDATETIME", sDateTime);
    RViewerLog->SetFieldParam("LOGMESSAGE", sText);
	RViewerLog->Insert();
    delete RViewerLog;
}
//---------------------------------------------------------------------------
void TFormMain::WriteLog(bool bFileWrite, bool bDBWrite, UnicodeString sText)
{
    UnicodeString sDateTime = Now().FormatString("yyyy-mm-dd hh:nn:ss.zzz"); //pk violation을 막기위해 1/1000초 쉰뒤 키를 시스템 시간으로 저장

    UnicodeString sFileDate = sDateTime;
    sFileDate = StringReplace(sFileDate, "-", "", TReplaceFlags() << rfReplaceAll);
    sFileDate = StringReplace(sFileDate, ":", "", TReplaceFlags() << rfReplaceAll);
    sFileDate = StringReplace(sFileDate, " ", "", TReplaceFlags() << rfReplaceAll);
    UnicodeString sPathFile = PathStrAdd(DBEnv.sPubViewerHome, "Log") + "\\" +
                              DBEnv.sPointCode + "_" + sFileDate.SubString(1, 6) + "\\" +
                              DBEnv.sPointCode + "_" + sFileDate.SubString(1, 8) + ".log";
    if (FormLog != NULL) //로그폼에 표시
    {
        UnicodeString sMemoText = "[" + sDateTime.SubString(12, 8) + "]" + sText;
        FormLog->mmLog->Lines->Insert(0, sMemoText);
        if (FormLog->mmLog->Lines->Count > 1000) FormLog->mmLog->Clear();
    } // if
    if (bFileWrite)
    {
        WriteFileLog(sPathFile, sDateTime, sText);
    } // if
    if (bDBWrite) _WriteDBLog(sDateTime, sText);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrConnectToTCPServerTimer(TObject *Sender)
//10초마다 접속 시도(DataSender/DataLogger)
{
	//tmrConnectToTCPServer->Enabled = False;

	if ( !LoggerSocket->Socket->Connected ) ConnectToLoggerServer();
	if ( !SenderSocket->Socket->Connected ) ConnectToSenderServer();

	//ConnectToSenderServer(); //DataSender 접속
	//ConnectToLoggerServer(); //DataLogger 접속

	//tmrConnectToTCPServer->Enabled = True;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderClientConnected(TObject *Sender)
{
	WriteLog(CbFileWrite, !CbDBWrite, "DataSender 서버에 접속하였습니다.");
    tbtnSender->ImageIndex = 13;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SenderClientDisconnected(TObject *Sender)
{
    WriteLog(CbFileWrite, !CbDBWrite, "DataSender 서버와의 연결이 끊어졌습니다.");
    tbtnSender->ImageIndex = 12;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::LoggerClientConnected(TObject *Sender)
{
    WriteLog(CbFileWrite, !CbDBWrite, "DataLogger 서버에 접속하였습니다.");
	tbtnLogger->ImageIndex = 11;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::LoggerClientDisconnected(TObject *Sender)
{
    WriteLog(CbFileWrite, !CbDBWrite, "DataLogger 서버와의 연결이 끊어졌습니다.");
    tbtnLogger->ImageIndex = 10;
}

//---------------------------------------------------------------------------
bool TFormMain::ConnectToSenderServer()
{
	if ( !SenderSocket->Socket->Connected ) {
		SenderSocket->Socket->Close(true);
		Sleep(50);	// Wait for while

		SenderSocket->Host = DBEnv.sDataSenderIP;
		SenderSocket->Port = DBEnv.iDataSenderPort;

		try {
			SenderSocket->Open();
			WriteLog(CbFileWrite, !CbDBWrite, "Try to connect to DataSender...");
		} catch ( Exception &e ) {
			WriteLog(CbFileWrite, !CbDBWrite, "DataSender Connection Error: " + e.Message);
		}
	}

	return true;

/*    if (!SenderClient->Connected())
	{
		SenderClientDisconnected(NULL);
		SenderClient->Host = DBEnv.sDataSenderIP;
		SenderClient->Port = DBEnv.iDataSenderPort;
		try
		{
			SenderClient->Connect();
		} catch(Exception &e)
		{
			WriteLog(CbFileWrite, !CbDBWrite, "DataSender에 연결중 에러: " + e.Message);
		} // catch
	} // if

	return SenderClient->Connected();
*/
}
//---------------------------------------------------------------------------
bool TFormMain::ConnectToLoggerServer()
{
	if ( !LoggerSocket->Socket->Connected ) {
		LoggerSocket->Socket->Close(true);
		Sleep(50);	// Wait for while

		LoggerSocket->Host = DBEnv.sDataLoggerIP;
		LoggerSocket->Port = DBEnv.iDataLoggerPort;

		try {
			LoggerSocket->Open();
			WriteLog(CbFileWrite, !CbDBWrite, "Try to connect to DataLogger...");
		} catch ( Exception &e ) {
			WriteLog(CbFileWrite, !CbDBWrite, "DataLogger Connection Error: " + e.Message);
		}
	}

	return true;

/*
	if (!LoggerClient->Connected())
    {
        LoggerClientDisconnected(NULL);
        try
        {
			LoggerClient->Host = DBEnv.sDataLoggerIP;
			LoggerClient->Port = DBEnv.iDataLoggerPort;
            LoggerClient->Connect();
        } catch(Exception &e)
        {
            WriteLog(CbFileWrite, !CbDBWrite, "DataLogger에 연결중 에러: " + e.Message);
        } // catch
    } // if

	return LoggerClient->Connected();
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrReadSenderPacketTimer(TObject *Sender)
//DataSender의 패킷을 읽음
{
//	tmrReadSenderPacket->Enabled = False;
//
//    SenderClient->CheckForGracefulDisconnect(False);
//    if (SenderClient->Connected())
//    {
//        try
//        {
//            while(!SenderClient->Socket->InputBufferIsEmpty())
//            {
//                char ch = SenderClient->Socket->ReadByte();
//                pbSender->WriteByte(ch);
//            } // while
//            char *pbPacket = NULL;
//            int iSize = 0;
//            while (pbSender->ReadPacket(pbPacket, iSize))
//            {
//                UnicodeString sError = SockPacketValid(pbPacket, iSize);
//                if (sError == "")
//				{
//					WriteLog(CbFileWrite, !CbDBWrite, "DataSender 신호 수신");
//					//패킷값처리
//					TSender2ViewerPacket *svPacket = (TSender2ViewerPacket *)pbPacket;
//					//ftp status
//					UnicodeString sTemp;
//
//					//dc charge
//					fCharge = ntohl(svPacket->iCharge) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
//					sTemp = sTemp.sprintf(L"%.02f", fCharge);
//					//FormLeft->pnlDCCharge->Caption = FormatFloat("#0.00", fCharge);
//					FormLeft->pnlDCCharge->Caption = sTemp;
//					//terperature
//					fTemperature = ntohl(svPacket->iTemperature) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
//					FormLeft->pnlTemperature->Caption = FormatFloat("#0.00", fTemperature);
//					//humidity
//					fHumidity = ntohl(svPacket->iHumidity) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
//					//battery
//					fBattery = ntohl(svPacket->iBattery) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
//					sTemp = sTemp.sprintf(L"%.02f", fBattery);
//					//FormLeft->pnlDCBattery->Caption = FormatFloat("#0.00", fBattery);
//					FormLeft->pnlDCBattery->Caption = sTemp;
//
//					if (fCharge == 0.0f && fBattery == 0.0f )      SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 1);
//					else if (fCharge < 12.0f || fBattery < 12.0f)  SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 0);
//					else                                           SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 1);
//
//					switch(svPacket->cFTPStatus)
//					{
//						case fsIdle:
//							FormLeft->SenderIdle();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Idle");
//							break;
//						case fsServerStart:
//							FormLeft->SenderStart();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Start");
//							//FormLeft->iSenderTick = GetTickCount();
//							//FormLeft->iLogOperation = 4;
//							break;
//						case fsServerWorking:
//							FormLeft->SenderWorking();
//							//FormLeft->iLogOperation = 5;
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Working");
//							break;
//						case fsServerFinish:
//							FormLeft->SenderFinish();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Finish");
//							//FormLeft->iLogOperation = 6;
//							break;
//						case fsRnseaStart:
//							FormLeft->SenderRnseaStart();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Start");
//							//FormLeft->iLogOperation = 7;
//							break;
//						case fsRnseaWorking:
//							FormLeft->SenderRnseaWorking();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Working");
//							//FormLeft->iLogOperation = 8;
//							break;
//						case fsRnseaFinish:
//							FormLeft->SenderRnseaFinish();
//							WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Finish");
//							//FormLeft->iLogOperation = 9;
//							break;
//                    } // switch
//                } else WriteLog(CbFileWrite, !CbDBWrite, "DataSender 신호 수신 에러: " + sError);
//                delete[] pbPacket;
//            } // while
//        } // try
//        catch(Exception &e)
//        {
//            WriteLog(CbFileWrite, !CbDBWrite, "DataSender 신호수신 오류: " + e.Message);
//        } // catch
//    } // if
//
//    tmrReadSenderPacket->Enabled = True;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    pbSender = new TPacketBuffer;
    pbSender->BufferSize = 4096;
	pbLogger = new TPacketBuffer;
	pbLogger->BufferSize = 4096;
    ReadIni();
	if (!ConnectDB())
	{
		//ShowMessage("DB에 연결할 수 없습니다.\n프로그램이 종료됩니다.");
		Application->Terminate();
		return;
	} // if
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	LoggerSocket->Active = false;
	SenderSocket->Active = false;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	WriteLog(CbFileWrite, CbDBWrite, "DataViewer Application 종료");
    delete pbSender;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrReadLoggerPacketTimer(TObject *Sender)
//로거의 패킷 읽기
{
//	tmrReadLoggerPacket->Enabled = False;
//	try
//	{
//        LoggerClient->CheckForGracefulDisconnect(False);
//        if (!LoggerClient->Connected())
//        {
//			tmrReadLoggerPacket->Enabled = True;
//            return;
//        } // if
//        while(!LoggerClient->Socket->InputBufferIsEmpty())
//        {
//        	char ch = LoggerClient->Socket->ReadByte();
//       		pbLogger->WriteByte(ch);
//        } // while
//	} // try
//	catch(Exception &e)
//	{
//        WriteLog(CbFileWrite, !CbDBWrite, "DataLogger 신호수신 오류: " + e.Message);
//	} // catch
//
//	char *pbPacket = NULL;
//	int iSize = 0;
//	while (pbLogger->ReadPacket(pbPacket, iSize), pbPacket != NULL)
//	{
//		UnicodeString sError = SockPacketValid(pbPacket, iSize);
//		if (sError == "")
//		{
//			WriteLog(CbFileWrite, !CbDBWrite, "DataLogger 신호 수신");
//			//logger에서의 패킷 처리
//			switch(pbPacket[3])
//			{
//				case 0x0:	//Idle
//					{
//						bRunning = False;
//						FormLeft->LoggerIdle();
//						break;
//					}
//				case 0x1:	//Start
//					{
//						//FormLeft->iLogOperation = 1;
//						bRunning = True;
//						FormLeft->LoggerStart();
//						break;
//					}
//				case 0x2:	//Logging
//					{
//						//FormLeft->iLogOperation = 2;
//						bRunning = True;
//						FormLeft->LoggerLogging();
//						if (FormLeft->shpLoggerProgress->Width < FormLeft->shpBG1->Width) FormLeft->shpLoggerProgress->Tag = FormLeft->shpLoggerProgress->Tag + 1;
//						break;
//					}
//				case 0x3:	//Finish
//					{
//						//FormLeft->iLogOperation = 3;
//						PostMessage(Handle, WM_USER, 0, 0); //모든 child 폼의 쿼리를 재오픈
//						bRunning = False;
//						FormLeft->LoggerFinish();
//						break;
//					}
//			} // switch
//		} else WriteLog(CbFileWrite, !CbDBWrite, sError);
//        delete[] pbPacket;
//	} // while
//
//	tmrReadLoggerPacket->Enabled = True;
}
//---------------------------------------------------------------------------
void TFormMain::SendMessageToServer(char cMessageKind)
{
//	TIdTCPClient *Client = NULL;
//
//    switch(cMessageKind)
//    {
//        case CcLoggerSetupChange:
//            Client = LoggerClient;
//            break;
//        case CcSenderSetupChange:
//            Client = SenderClient;
//            break;
//    } // switch
//
//    Client->CheckForGracefulDisconnect(False);
//    if (!Client->Connected()) return;
//
//    TMemoryStream *ms = new TMemoryStream;
//    char sData[4] = { 0, };
//    char *Packet = EncodeSockPacket(cMessageKind, sData, 4);
//    int iPacketSize = 4 + CiPacketFixedSize;
//
//    if (Packet != NULL)
//    {
//        ms->Write(Packet, iPacketSize);
//        try
//        {
//            ms->Position = 0;
//            Client->Socket->Write(ms, iPacketSize);
//        } __finally
//        {
//            delete[] Packet;
//        }
//    } // if
//    delete ms;
//
//    switch(cMessageKind)
//    {
//        case CcLoggerSetupChange:
//			WriteLog(CbFileWrite, !CbDBWrite, "DataLogger에 설정변경 신호 전송");
//            break;
//        case CcSenderSetupChange:
//			WriteLog(CbFileWrite, !CbDBWrite, "DataSender에 설정변경 신호 전송");
//            break;
//    } // switch
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actShowConfigExecute(TObject *Sender)
//설정
{
    if (ShowLogin(DBEnv.sLoggedUserID) != "")
    {
        if (!FormMain->DBEnv.bSuperID)
        {
            ShowMessage("권한이 없습니다.");
            return;
        } // if
        ShowConfig(ciPoint | ciLogger | ciSender | ciGuardnet | ciViewer | ciRange | ciStreamMethod | ciEtc, //표시할 설정 페이지
                   ciViewer);
    } // if
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::miAboutRADSDataViewerClick(TObject *Sender)
//about
{
    FormAbout = new TFormAbout(this);
    FormAbout->ShowModal();
    delete FormAbout;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actShowLogExecute(TObject *Sender)
//로그보기
//오른쪽 하단 1/4 영역에 표시
{
    if (actShowLog->Checked)
    {
    	FormLog = new TFormLog(this);
        ArrangeLog();
        FormLog->Show();
    } else FormLog->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::miUserClick(TObject *Sender)
//사용자 관리
{
    if (ShowLogin(DBEnv.sLoggedUserID) != "")
    {
        if (!FormMain->DBEnv.bSuperID)
        {
            ShowMessage("권한이 없습니다.");
            return;
        } // if
        FormUser = new TFormUser(this);
        FormUser->ShowModal();
        delete FormUser;
    } // if
}
//---------------------------------------------------------------------------
void TFormMain::ArrangeChildren()
//mdi child 폼들을 메인의 client area에 배분해서 위치 및 크기 수정
{
    if (MDIChildCount == 0) return;

    struct TnArray
    {
        int cCount;
        int rCount;
    } nArray[] = {
                   1, 1, //1
                   1, 2, //2
                   2, 2, //3
                   2, 2  //4
                 };

    int iChildWidth = (ClientWidth - (Splitter->Left + Splitter->Width + 4)) / nArray[MDIChildCount - 1].cCount;
    int iChildHeight = (ClientHeight - (ToolBar->Height + 4)) / nArray[MDIChildCount - 1].rCount;
    int ix = 0, iy = 1;
    if (MDIChildCount == 3)
    {
        //유량계
        FormFlow->Width = iChildWidth;
        FormFlow->Height = iChildHeight;
        FormFlow->Left = iChildWidth * (1 - 1);
        FormFlow->Top = iChildHeight * (1 - 1);
        //수위계
        FormWaterLevel->Width = iChildWidth;
        FormWaterLevel->Height = iChildHeight;
        FormWaterLevel->Left = iChildWidth * (2 - 1);
        FormWaterLevel->Top = iChildHeight * (1 - 1);
        //유속계
        FormHydroMeter->Width = iChildWidth * 2;
        FormHydroMeter->Height = iChildHeight;
        FormHydroMeter->Left = iChildWidth * (1 - 1);
        FormHydroMeter->Top = iChildHeight * (2 - 1);
    } else
    {
        for(int i = 0; i < MDIChildCount; i++)
        {
            ix++;
            if (ix > nArray[MDIChildCount - 1].cCount)
            {
                ix = 1;
                iy++;
            } // if
            MDIChildren[i]->Left = iChildWidth * (ix - 1);
            MDIChildren[i]->Top = iChildHeight * (iy - 1);
            MDIChildren[i]->Width = iChildWidth;
            MDIChildren[i]->Height = iChildHeight;
        } // for i
    } // else
    ArrangeVth();
    ArrangeLog();
    ArrangeFormSearchData();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actShowReportExecute(TObject *Sender)
//점검일지
{
    FormReport = new TFormReport(this);
    FormReport->ShowModal();
    delete FormReport;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actArrangeWindowExecute(TObject *Sender)
//윈도우 재배열
{
    ArrangeChildren(); //윈도우 재배열
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actQueryReportExecute(TObject *Sender)
//자료조회
{
    if (FormSearchData == NULL) FormSearchData = new TFormSearchData(this);
    ArrangeFormSearchData();
    FormSearchData->Show();
	ArrangeChildren(); //윈도우 재배열
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
{
	bool bResult = false;
	bResult = GetDBEnvInfo();

	if ( !bResult ) {
		Application->Terminate();	// heaven2
		return;
	}

	FormLeft->Show();
	FormLeft->ManualDock(FormMain->PanelLeft);
	FormLeft->Align = alTop;

	this->VertScrollBar->Visible = false;
	this->HorzScrollBar->Visible = false;
	this->WindowState = wsMaximized;

	tmrDisplayTime->Enabled = true;

	WriteLog(CbFileWrite, CbDBWrite, "DataViewer Application 시작");
    DBEnv.sLoggedUserID = "default"; //기본계정

	// Logger/Sender Connect
	tmrConnectToTCPServer->Interval = 30 * 1000;	// Reconnect interval : 10 second
	tmrConnectToTCPServer->Enabled  = true;
	tmrConnectToTCPServerTimer(NULL); //소켓서버 접속

    //디폴트 창 띄우기
    if (DBEnv.bMDIHydroMeter1) miMDIHydroMeter->Click(); //유속계
	if (DBEnv.bMDIWaterLevel)  miMDIWaterLevel->Click(); //수위계
    if (DBEnv.bMDIWaterFlow)   miMDIWaterFlow->Click();  //유량산출
    if (DBEnv.bMDILog)         miMDILog->Click();        //로그창
    if (DBEnv.bMDIVth)         miMDIVth->Click();        //vth logger
    ArrangeChildren(); //윈도우 재배열
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actWaterFlowExecute(TObject *Sender)
//mdi child: 유량
{
    if (actWaterFlow->Checked)
    {
        if (FormFlow == NULL) FormFlow = new TFormFlow(this);
        FormFlow->Show();
        ArrangeChildren(); //윈도우 재배열
    } else FormFlow->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actWaterLevelExecute(TObject *Sender)
//mdi child: 수위계
{
    if (actWaterLevel->Checked)
    {
        if (FormWaterLevel == NULL) FormWaterLevel = new TFormWaterLevel(this);
        FormWaterLevel->Show();
        ArrangeChildren(); //윈도우 재배열
    } else FormWaterLevel->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actHydroMeterExecute(TObject *Sender)
//mdi child: 하층/상층유속계
{
    if (actHydroMeter->Checked)
    {
		if (FormHydroMeter == NULL) FormHydroMeter = new TFormHydroMeter(this);
		FormHydroMeter->Show();
        ArrangeChildren(); //윈도우 재배열
    } else FormHydroMeter->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actVTHLoggerExecute(TObject *Sender)
//vth logger
//1/4크기로 화면 중앙에 위치
{
    if (actVTHLogger->Checked)
    {
        if (FormVTH == NULL) FormVTH = new TFormVTH(this);
        ArrangeVth();
        FormVTH->Show();
    } else FormVTH->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actCCTVExecute(TObject *Sender)
{
    ShellExecute(NULL, TEXT("open"), TEXT("iexplore"), DBEnv.sCctvUrl.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void TFormMain::ArrangeLog()
//오른쪽 하단 1/4 영역에 표시
{
    if (FormLog != NULL)
    {
        int iChildWidth = (ClientWidth - (Splitter->Left + Splitter->Width + 4)) / 2;
        int iChildHeight = (ClientHeight - (ToolBar->Height + 4)) / 2;
        TPoint pnt = TPoint(iChildWidth * 1, iChildHeight * 1);
        pnt = ClientToScreen(pnt);
        FormLog->Left = pnt.x + Splitter->Left + Splitter->Width + 2;
        FormLog->Top = pnt.y + ToolBar->Height + 2;
        FormLog->Width = iChildWidth;
        FormLog->Height = iChildHeight;
    } // if
}
//---------------------------------------------------------------------------
void TFormMain::ArrangeVth()
//1/4크기로 화면 중앙에 위치
{
//    if (FormVTH != NULL)
//    {
//        int iChildWidth = (ClientWidth - (Splitter->Left + Splitter->Width + 4)) / 2;
//        int iChildHeight = (ClientHeight - (ToolBar->Height + 4)) / 2;
//        FormVTH->Left = Left + PanelLeft->Width + Splitter->Width + 10 + iChildWidth / 2;
//        FormVTH->Top = Top + ToolBar->Height + 52 + iChildHeight / 2;
//        FormVTH->Width = iChildWidth;
//        FormVTH->Height = iChildHeight;
//    } // if
}
//---------------------------------------------------------------------------
void TFormMain::ArrangeFormSearchData()
//클라이언트 영역 전체로
{
    if (FormSearchData != NULL)
    {
        int iChildWidth = (ClientWidth - (Splitter->Left + Splitter->Width + 4)) / 2;
        int iChildHeight = (ClientHeight - (ToolBar->Height + 4)) / 2;
        FormSearchData->Left = Left + PanelLeft->Width + Splitter->Width + 10;
        FormSearchData->Top = Top + ToolBar->Height + 52;
        FormSearchData->Width = iChildWidth * 2;
        FormSearchData->Height = iChildHeight * 2;
    } // if
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ApplicationEventsException(TObject *Sender, Exception *E)
{
    ApplicationEvents->CancelDispatch();
    WriteLog(CbFileWrite, !CbDBWrite, E->Message); //file로그에 표시
}

//---------------------------------------------------------------------------
void TFormMain::QueryAllChildren()
{
    if (FormFlow       != NULL) FormFlow->ForceQuery();
    if (FormWaterLevel != NULL) FormWaterLevel->ForceQuery();
    if (FormVTH        != NULL) FormVTH->ForceQuery();
	if (FormHydroMeter != NULL) FormHydroMeter->ForceQuery();
}

//---------------------------------------------------------------------------
// LoggerSocket Connect Event
void __fastcall TFormMain::LoggerSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	m_bLoggerConnect = true;
	tbtnLogger->ImageIndex = enLoggerOnIndex;

	WriteLog(CbFileWrite, !CbDBWrite, "DataLogger connected..");
}

//---------------------------------------------------------------------------
// LoggerSocket Disconnect Event
void __fastcall TFormMain::LoggerSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	m_bLoggerConnect = false;
	tbtnLogger->ImageIndex = enLoggerOffIndex;

	WriteLog(CbFileWrite, !CbDBWrite, "DataLogger disconnected..");
}

//---------------------------------------------------------------------------
// LoggerSocket Error Event
void __fastcall TFormMain::LoggerSocketError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;	// surpress error dialog
	Socket->Close();

	tbtnLogger->ImageIndex = enLoggerOffIndex;
	WriteLog(CbFileWrite, !CbDBWrite, "DataLogger error..");
}

//---------------------------------------------------------------------------
// LoggerSocket Read Event
void __fastcall TFormMain::LoggerSocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
	static bBuffering = false;

	BYTE Data[1024] = "";
	int iRecvSize = Socket->ReceiveBuf(Data, 1024);

	for ( int i = 0 ; i < iRecvSize ; i++ ) {
		if ( Data[i] == STX ) {			// STX ---------------------------------
			if ( bBuffering ) {
				m_LoggerBuffer[m_LoggerIndex++] = Data[i];
			} else {
				m_LoggerIndex = 0;
				m_LoggerBuffer[m_LoggerIndex++] = Data[i];
				bBuffering = true;
			}
		} else if ( Data[i] == ETX ) {	// ETX ---------------------------------
			if ( !bBuffering ) continue;
			m_LoggerBuffer[m_LoggerIndex++] = Data[i];
			int iLen = m_LoggerBuffer[2];	// Length
			if ( m_LoggerIndex == iLen + 5 ) {	// Match
				// -------------------------------------------------------------
				ProcessLogger(m_LoggerBuffer, m_LoggerIndex);
				// -------------------------------------------------------------
				memset(m_LoggerBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_LoggerIndex = 0;
			} else if ( m_LoggerIndex > iLen + 5 ) {	// Frame is too long
				WriteLog(CbFileWrite, !CbDBWrite, "Frame is too long");
				memset(m_LoggerBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_LoggerIndex = 0;
			} else {								// Frame is too Short
				// ETX in payload => continue
			}
		} else {						// Data(Payload) -----------------------
			if ( bBuffering ) {
				m_LoggerBuffer[m_LoggerIndex++] = Data[i];
			} else {
				// unknown contidion
				// nothing to do..
				memset(m_LoggerBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_LoggerIndex = 0;
			}
		}
	} // end of for
}

//---------------------------------------------------------------------------
// SenderSocket Connect Event
void __fastcall TFormMain::SenderSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	m_bSenderConnect = true;
	tbtnSender->ImageIndex = enSenderOnIndex;

	WriteLog(CbFileWrite, !CbDBWrite, "DataSender connected..");
}

//---------------------------------------------------------------------------
// SenderSocket Disconnect Event
void __fastcall TFormMain::SenderSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	m_bSenderConnect = false;
	tbtnSender->ImageIndex = enSenderOffIndex;

	WriteLog(CbFileWrite, !CbDBWrite, "DataSender disconnected..");
}

//---------------------------------------------------------------------------
// SenderSocket Error Event
void __fastcall TFormMain::SenderSocketError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;	// surpress error dialog
	Socket->Close();

	m_bSenderConnect = false;
	tbtnSender->ImageIndex = enSenderOffIndex;

	WriteLog(CbFileWrite, !CbDBWrite, "DataSender error..");
}

//---------------------------------------------------------------------------
// SenderSocket Read Event
void __fastcall TFormMain::SenderSocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
	static bBuffering = false;

	BYTE Data[1024] = "";
	int iRecvSize = Socket->ReceiveBuf(Data, 1024);

	for ( int i = 0 ; i < iRecvSize ; i++ ) {
		if ( Data[i] == STX ) {			// STX ---------------------------------
			if ( bBuffering ) {
				m_SenderBuffer[m_SenderIndex++] = Data[i];
			} else {
				m_LoggerIndex = 0;
				m_SenderBuffer[m_SenderIndex++] = Data[i];
				bBuffering = true;
			}
		} else if ( Data[i] == ETX ) {	// ETX ---------------------------------
			if ( !bBuffering ) continue;
			m_SenderBuffer[m_SenderIndex++] = Data[i];
			int iLen = m_SenderBuffer[2];	// Length
			if ( m_SenderIndex == iLen + 5 ) {	// Match
				// -------------------------------------------------------------
				ProcessSender(m_SenderBuffer, m_SenderIndex);
				// -------------------------------------------------------------
				memset(m_SenderBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_SenderIndex = 0;
			} else if ( m_SenderIndex > iLen + 5 ) {	// Frame is too long
				WriteLog(CbFileWrite, !CbDBWrite, "Frame is too long");
				memset(m_SenderBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_SenderIndex = 0;
			} else {								// Frame is too Short
				// ETX in payload => continue
			}
		} else {						// Data(Payload) -----------------------
			if ( bBuffering ) {
				m_SenderBuffer[m_SenderIndex++] = Data[i];
			} else {
				// unknown contidion
				// nothing to do..
				memset(m_SenderBuffer, 0x00, BUFFER_1K);
				bBuffering = false;
				m_SenderIndex = 0;
			}
		}
	} // end of for
}

//---------------------------------------------------------------------------
void TFormMain::ProcessLogger(BYTE *pBuffer, int iLen)
{
	BYTE cType = *(pBuffer+1);

	if ( cType == MSG_TYPE_LoggerActionEvent ) {

		WriteLog(CbFileWrite, !CbDBWrite, "DataLogger ActionEvent received..");

		BYTE cLoggerStatus = *(pBuffer+3);	// Logging Status
		EN_SENSOR_STATUS enHydro1, enHydro2, enWaterLevel, enFlowStream;

		enHydro1     = *(pBuffer+4);
		enHydro2     = *(pBuffer+5);
		enWaterLevel = *(pBuffer+6);
		enFlowStream = *(pBuffer+7);

		switch(cLoggerStatus)
		{
			case enLoggerIdle:	//Idle
				{
					bRunning = false;
					m_enLoggerStatus = enLoggerIdle;
					FormLeft->LoggerIdle();
					break;
				}
			case enLoggerStart:	//Start
				{
					//FormLeft->iLogOperation = 1;
					bRunning = true;
					m_enLoggerStatus = enLoggerStart;
					FormLeft->LoggerStart();
					//heaven2
					if ( FormHydroMeter != NULL ) FormHydroMeter->LoggingStart();
					if ( FormWaterLevel != NULL ) FormWaterLevel->LoggingStart();
					if ( FormFlow       != NULL ) FormFlow->LoggingStart();
					break;
				}
			case enLoggerLogging:	//Logging
				{
					//FormLeft->iLogOperation = 2;
					bRunning = true;
					m_enLoggerStatus = enLoggerLogging;
					FormLeft->LoggerLogging();

					// 유량산출은 유속/수위센서가 동작이면 무조건 동작하도록 한다.
					if ( (enHydro1 >= enSensorStart && enHydro1 < enSensorFinish) || (enHydro2 >= enSensorStart && enHydro2 < enSensorFinish) ||
						 (enWaterLevel >= enSensorStart && enWaterLevel < enSensorFinish) ) {
						enFlowStream = enSensorStart;
					} else {
						enFlowStream = enSensorFinish;
					}

					//heaven2
					if ( FormHydroMeter != NULL ) FormHydroMeter->LoggingContinue(enHydro1, enHydro2);
					if ( FormWaterLevel != NULL ) FormWaterLevel->LoggingContinue(enWaterLevel);
					if ( FormFlow       != NULL ) FormFlow->LoggingContinue(enFlowStream);

					if (FormLeft->shpLoggerProgress->Width < FormLeft->shpBG1->Width) FormLeft->shpLoggerProgress->Tag = FormLeft->shpLoggerProgress->Tag + 1;
					break;
				}
			case enLoggerFinish:	//Finish
				{
					bRunning = false;
					m_enLoggerStatus = enLoggerFinish;

					//FormLeft->iLogOperation = 3;
					FormLeft->LoggerFinish();

					if ( FormHydroMeter != NULL ) FormHydroMeter->LoggingFinish();
					if ( FormWaterLevel != NULL ) FormWaterLevel->LoggingFinish();
					if ( FormFlow       != NULL ) FormFlow->LoggingFinish();

					PostMessage(Handle, WM_USER, 0, 0); //모든 child 폼의 쿼리를 재오픈
					break;
				}
		} // switch
	}
}

//---------------------------------------------------------------------------
void TFormMain::ProcessSender(BYTE *pBuffer, int iLen)
{
	BYTE cType = *(pBuffer + 1);

	if ( cType == MSG_TYPE_SenderActionEvent ) {

		WriteLog(CbFileWrite, !CbDBWrite, "DataSender ActionEvent received..");

		TSender2ViewerPacket *svPacket = new TSender2ViewerPacket();
		memcpy(svPacket, pBuffer, iLen-1);

		UnicodeString sTemp;

		//dc charge
		fCharge      = ntohl(svPacket->iCharge) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
		sTemp        = sTemp.sprintf(L"%.02f", fCharge);
		//FormLeft->pnlDCCharge->Caption = FormatFloat("#0.00", fCharge);
		FormLeft->pnlDCCharge->Caption = sTemp;
		//terperature
		fTemperature = ((float)((long)ntohl(svPacket->iTemperature))) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
		FormLeft->pnlTemperature->Caption = FormatFloat("#0.00", fTemperature);
		//humidity
		fHumidity    = ntohl(svPacket->iHumidity) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
		//battery
		fBattery     = ntohl(svPacket->iBattery) / 1000.0f; //network to host-long(빅 엔디안 -> 리틀 엔디안)
		sTemp        = sTemp.sprintf(L"%.02f", fBattery);
		//FormLeft->pnlDCBattery->Caption = FormatFloat("#0.00", fBattery);
		FormLeft->pnlDCBattery->Caption = sTemp;

		if (fCharge < 12.0f || fBattery < 12.0f) {
			if ( fCharge == 0.0f && fBattery == 0.0f ) {
				SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 1);	// Battery Init Status(Unknown Status )
			} else {
				SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 0);	// Battery Low
			}
		} else {
			SetBitmapImageListIndex(FormLeft->imgVthStatus, FormLeft->ilVthStatus, 1);	// Battery Normal
		}

		switch(svPacket->cFTPStatus)
		{
			case fsIdle:
				FormLeft->SenderIdle();
				m_enSenderStatus = enSenderIdle;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Idle");
				break;
			case fsServerStart:
				FormLeft->SenderStart();
				m_enSenderStatus = enSenderStart;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Start");
				//FormLeft->iSenderTick = GetTickCount();
				//FormLeft->iLogOperation = 4;
				break;
			case fsServerWorking:
				FormLeft->SenderWorking();
				m_enSenderStatus = enSenderWorking;
				//FormLeft->iLogOperation = 5;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Working");
				break;
			case fsServerFinish:
				FormLeft->SenderFinish();
				m_enSenderStatus = enSenderFinish;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Server Finish");
				//FormLeft->iLogOperation = 6;
				break;
			case fsRnseaStart:
				FormLeft->SenderRnseaStart();
				m_enSenderStatus = enSenderRnseaStart;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Start");
				//FormLeft->iLogOperation = 7;
				break;
			case fsRnseaWorking:
				FormLeft->SenderRnseaWorking();
				m_enSenderStatus = enSenderRnseaWorking;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Working");
				//FormLeft->iLogOperation = 8;
				break;
			case fsRnseaFinish:
				FormLeft->SenderRnseaFinish();
				m_enSenderStatus = enSenderRnseaFinish;
				WriteLog(CbFileWrite, !CbDBWrite, "->DataSender Rnsea Finish");
				//FormLeft->iLogOperation = 9;
				break;
		} // switch
		delete svPacket;	// Release
	}
}


//---------------------------------------------------------------------------
bool TFormMain::SendLogger(BYTE cType, BYTE *pPayload, int iLen)
{
	if ( !LoggerSocket->Socket->Connected) return false;	// no connection

	BYTE Data[1024];
	memset(Data, 0x00, 1024);
	int Pos = 0;

	Data[0] = STX;
	Data[1] = cType;	// 0x01
	Data[2] = iLen;	// 20Byte
	memcpy(Data+3, pPayload, iLen);


	Data[iLen+3] = MakeCRC(Data,iLen+3);
	Data[iLen+4] = ETX;

	LoggerSocket->Socket->SendBuf(Data, iLen+5);

	return true;
}

//---------------------------------------------------------------------------
bool TFormMain::SendSender(BYTE cType, BYTE *pPayload, int iLen)
{
	if ( !SenderSocket->Socket->Connected) return false;	// no connection

	BYTE Data[1024];
	memset(Data, 0x00, 1024);
	int Pos = 0;

	Data[0] = STX;
	Data[1] = cType;	// 0x01
	Data[2] = iLen;	// 20Byte
	memcpy(Data+3, pPayload, iLen);


	Data[iLen+3] = MakeCRC(Data,iLen+3);
	Data[iLen+4] = ETX;

	SenderSocket->Socket->SendBuf(Data, iLen+5);

	return true;
}

//---------------------------------------------------------------------------
// Logger에 UI Open 요청을 한다.
void __fastcall TFormMain::actLoggerUIOpenExecute(TObject *Sender)
{
	if ( !LoggerSocket->Socket->Connected ) {
		ShowMessage("DataLogger와 연결되어 있지 않습니다. 연결 후 다시 시도하십시오.");
		return;
	}
//#define MSG_TYPE_LoggerUIOpen		   0x83
//#define MSG_TYPE_SenderUIOpen		   0x84
	BYTE cType = MSG_TYPE_LoggerUIOpen;
	BYTE cData[100] = "";
	int  iLen = 4;

	bool bResult = false;
	bResult = SendLogger(cType, cData, iLen);
}

//---------------------------------------------------------------------------
// Sender에 UI Open 요청을 한다.
void __fastcall TFormMain::actSenderUIOpenExecute(TObject *Sender)
{
	if ( !SenderSocket->Socket->Connected ) {
		ShowMessage("DataSender와 연결되어 있지 않습니다. 연결 후 다시 시도하십시오.");
		return;
	}
//#define MSG_TYPE_LoggerUIOpen		   0x83
//#define MSG_TYPE_SenderUIOpen		   0x84
	BYTE cType = MSG_TYPE_SenderUIOpen;
	BYTE cData[100] = "";
	int  iLen = 4;

	bool bResult = false;
	bResult = SendSender(cType, cData, iLen);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::actExitExecute(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void TFormMain::OnMessage(TMessage &Message)	// Main Message process
{
	if  ( Message.Msg == WM_USER ) QueryAllChildren();	// 모든 화면의 재 Query
}


