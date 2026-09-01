//---------------------------------------------------------------------------
#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ImgList.hpp>
#include "AdPort.hpp"
#include "OoMisc.hpp"
#include "SBSimpleSftp.hpp"
#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Data.SqlExpr.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdContext.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdTCPServer.hpp>
#include <Vcl.AppEvnts.hpp>

//#include "SBSimpleSftp.hpp" 	// 2017.11.27 반드시 여기에 놓을 것(Main에 SFTPComonent를 놓을 경우)

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdContext.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>

#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Data.SqlExpr.hpp>
#include <Data.FMTBcd.hpp>
#include <Vcl.AppEvnts.hpp>

#include "AdPort.hpp"
#include "OoMisc.hpp"

#include "LockeCommon.h"
#include "PacketBuffer.h"
#include "fbTable.h"
#include "Env.h"

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbPTime;
	TButton *btnWindowSizeToggle;
	TLabel *Label3;
	TMemo *mmLog;
	TTimer *tmrDisplayTime;
	TTrayIcon *TrayIcon1;
	TPopupMenu *PopupMenu1;
	TMenuItem *Open1;
	TMenuItem *Close1;
	TLabel *lbTime;
	TTimer *tmrGoToTray;
	TLabel *Label1;
	TMemo *mmGuardnetLog;
	TPanel *pnlDCCharge;
	TPanel *pnlDCBattery;
	TPanel *pnlTemperature;
	TImage *imgVthStatus;
	TLabel *lblFtpTransferDateTime;
	TIdTCPServer *LoggerServer;
	TIdTCPServer *ViewerServer;
	TIdTCPClient *WatchDogClient;
	TIdFTP *FTP;
	TApdComPort *ApdComPort;
	TImage *imgFTPStatus;
	TImage *imgGuardnetStatus;
	TImage *imgLineStatus;
	TImageList *ImageListFTP;
	TImageList *ImageListGuardNet;
	TImageList *ImageListVTH;
	TTimer *tmrWD;
	TTimer *tmrCheckDBForFTP;
	TTimer *tmrGWResetDelay;
	TIdFTP *ftpRnsea;
	TImageList *ilStatus;
	TImage *imgWdStatus;
	TSQLConnection *SQLConnection;
	TPopupMenu *PopupMenu2;
	TMenuItem *miVTH;
	TButton *btnL2W;
	TButton *btnW2L;
	TTimer *tmrGuardnet;
	TTimer *tmrSendToViewers;
	TShape *shpBG1;
	TShape *shpProgress;
	TButton *btnConfig;
	TApplicationEvents *ApplicationEvents;
	TListView *ListView;
	TImage *imgLoggerStatus;
	TImage *imgViewerStatus;
	TLabel *lbVersion;
	TButton *Button1;

	void __fastcall btnWindowSizeToggleClick(TObject *Sender);
	void __fastcall tmrDisplayTimeTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall tmrGoToTrayTimer(TObject *Sender);
	void __fastcall TrayIcon1DblClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ViewerServerConnect(TIdContext *AContext);
	void __fastcall LoggerServerExecute(TIdContext *AContext);
	void __fastcall tmrWDTimer(TObject *Sender);
	void __fastcall ViewerServerExecute(TIdContext *AContext);
	void __fastcall ApdComPortTriggerAvail(TObject *CP, WORD Count);
	void __fastcall tmrCheckDBForFTPTimer(TObject *Sender);
	void __fastcall tmrGWResetDelayTimer(TObject *Sender);
	void __fastcall LoggerServerConnect(TIdContext *AContext);
	void __fastcall LoggerServerDisconnect(TIdContext *AContext);
	void __fastcall WatchDogClientConnected(TObject *Sender);
	void __fastcall WatchDogClientDisconnected(TObject *Sender);
	void __fastcall ViewerServerDisconnect(TIdContext *AContext);
	void __fastcall miVTHClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnL2WClick(TObject *Sender);
	void __fastcall btnW2LClick(TObject *Sender);
	void __fastcall tmrGuardnetTimer(TObject *Sender);
	void __fastcall tmrSendToViewersTimer(TObject *Sender);
	void __fastcall btnConfigClick(TObject *Sender);
	void __fastcall ApplicationEventsException(TObject *Sender, Exception *E);
	void __fastcall ApdComPortPortClose(TObject *Sender);
	void __fastcall ApdComPortPortOpen(TObject *Sender);
	void __fastcall ApdComPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall Button1Click(TObject *Sender);
//	void __fastcall SFTPClientKeyValidate(TObject *Sender, TElSSHKey *ServerKey, bool &Validate);
//	void __fastcall SFTPClient2KeyValidate(TObject *Sender, TElSSHKey *ServerKey, bool &Validate);

private:	// User declarations

	TfbTable         *SearchDBRPoint;

public:
	// VTHLogger Status Variable
	char  fFTPStatus; //FTPStatus의 원본
	char  fFTPImageStatus;
	char  fGuardNetStatus;
	char  fLineStatus;
	float fVTHCharge;
	float fVTHTemperature;
	float fVTHHumidity;
	float fVTHBattery;
	float f5V;
	float f3V;
	float fAIN0;
	float fAIN1;
	float fAIN2;
	float fAIN3;
	UnicodeString sVTHMsg;

private:

	bool  m_bExit;	// Close or goto tray..

	TDateTime m_DBCheckTimeStamp;	// 1분단위로 DB Health Check하는 TimeStamp

	TPacketBuffer *pbLogger;
	TPacketBuffer *pbViewer;

	bool  ConnectDB();
	void  ReadIniInfo();
	void  GoToSystemTray();
	void  RestoreFromTray();
	void  LocateRightButton();
	void  OpenVTH();

	bool  DoMakeDB2Adv(UnicodeString sMeasureDate, UnicodeString sMeasureTime);
	void  ResetSocketServer();

public:		// User declarations

	int           GiGWResetFrom;
	TList        *lstViewer; //뷰어 클라이언트들의 연결정보 리스트
	TDBEnvInfo    DBEnvInfo;
	bool          GbGWPowerOff;
	int           GiLogOperation;
	bool          GbVthDlgRequest;

	UnicodeString GsMeasureDate;
	UnicodeString GsMeasureTime;

	TIniInfo      IniInfo;	// heaven2

	TDateTime     m_LoggerTime;
	TDateTime     m_VTHTimeStamp;	// VTHLogger Get TimeStamp
	bool          m_bVTHRequest;

	UnicodeString GetAdvTextFromDB(UnicodeString sMeasureDate, UnicodeString sMeasureTime);
	UnicodeString GetAdvTextFromDB2(UnicodeString sMeasureDate, UnicodeString sMeasureTime); // EWSV+ADVM =>상층 유속계만 데이터 파일을 만든다.
	UnicodeString GetEsvTextFromDB(UnicodeString sMeasureDate, UnicodeString sMeasureTime);	// 2024.10.02 Get EWSV Format Data String
	void          SearchDBMakeFtpFiles(UnicodeString sMeasureDate = "", UnicodeString sMeasureTime = "");
	void          SendFtpFiles(bool bReal = False); 	//true면 real폴더로 false면 past폴더로
	void          GetDBEnvInfo();
	void          DoReset();

	void          LogMessage(bool bFileWrite, bool bDBWrite, UnicodeString sMessage, bool bLogMessage = True);

	// heaven2 2014.06.24
	void          SendSenderStatusToViewer();	// DataSender의 상태를 Viewer에 보낸다.
	void          SendWatchDogMessage(BYTE Action = 0x00);	// 0x00: Normal HeartBeat, 0xFF: Shutdown Request
	AnsiString    m_VTHMsg;
	void 		  VTHProcess(AnsiString sCmd);	// VTHLogger 수신한 내역의 처리

	void          ChangeFTPIcon(int idx);
	void          ChangeGuardNetIcon(int idx);
	void          ChangeLineIcon(int idx);
	void          ChangeBatteryIcon(int idx);
	void          ChangeLoggerIcon(int idx);
	void          ChangeWatchDogIcon(int idx);
	void          ChangeViewerIcon(int idx);
	void          ChangeVTHValue();

	void          SetUIExtend(bool bEnlarge=true);
	void          CheckAllocateMemory(void *p);	// GlobalAlloc() Error로 Restart Check

	bool          SendVTH(AnsiString sMsg);

	void          SaveVTHData();

	bool          IsExistFTPQueue(STFTPMessage *pFTPMessage);
	int           IsFileFormat();

	// heaven2 2017.09.20
	bool SendPing();
	bool SendPingAck();
	bool SendRebootReq();
	bool SendRebootRes();
	bool SendMessageToLogger(BYTE *pData, int Len);
	bool ProcessLoggerMessage(BYTE *pData, int Len);

	void CheckDBConnection();	// Check Database Connection by query ( Query Time ) 2022.04.27

	void OnMessage(TMessage &Message);	// Main Message process

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(UM_MESSAGE, TMessage, OnMessage)
	END_MESSAGE_MAP(TForm)

	__fastcall TfrmMain(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern TEvent *pGLogEvent;
extern TEvent *pGFTPEvent;
extern TEvent *pGGuardnetEvent;

extern TCriticalSection *pGLogSection; 	// LogMessage Critical Section
extern TCriticalSection *pGFTPSection;	// FTP Queue Critical Section
extern TCriticalSection *pGGuardnetSection;	// Guardnet Queue Critical Section

extern TQueue *pGLogQueue;
extern TQueue *pGFTPQueue;
extern TQueue *pGGuardnetQueue;

extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------

#endif


