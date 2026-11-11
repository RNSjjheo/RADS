//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.SysUtils.hpp>
#include <Data.DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBDatabaseInfo.hpp>
#include <IBQuery.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Data.DBXFirebird.hpp>
#include <Data.SqlExpr.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>

#include "DvEnv.h"
#include "PacketBuffer.h"
#include <System.Win.ScktComp.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TToolBar *ToolBar;
	TToolButton *TBQuery;
	TToolButton *TBLog;
	TToolButton *TBSetup;
	TToolButton *TBCCTV;
	TToolButton *TBRelocate;
	TImageList *ImageList;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N9;
    TMenuItem *miMDILog;
	TMenuItem *N11;
	TMenuItem *CCTV1;
	TMenuItem *N12;
    TMenuItem *miUser;
    TMenuItem *miArrangeChildren;
	TMenuItem *N15;
    TMenuItem *miMDIWaterFlow;
    TMenuItem *miMDIWaterLevel;
    TMenuItem *miMDIHydroMeter;
    TMenuItem *miAboutRADSDataViewer;
    TTimer *tmrDisplayTime;
	TPanel *PanelLeft;
	TSplitter *Splitter;
    TMenuItem *miMDIVth;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
    TApplicationEvents *ApplicationEvents;
    TSQLConnection *SQLConnection;
    TIdTCPClient *SenderClient;
    TIdTCPClient *LoggerClient;
    TTimer *tmrConnectToTCPServer;
    TTimer *tmrReadSenderPacket;
    TTimer *tmrReadLoggerPacket;
    TActionList *ActionList;
    TAction *actShowConfig;
    TAction *actShowLog;
    TAction *actShowReport;
    TAction *actArrangeWindow;
    TToolButton *tbtnLogger;
    TToolButton *tbtnSender;
    TAction *actQueryReport;
    TToolButton *ToolButton3;
    TToolButton *ToolButton4;
    TToolButton *ToolButton6;
    TToolButton *ToolButton10;
    TToolButton *ToolButton11;
    TAction *actWaterFlow;
    TAction *actWaterLevel;
    TAction *actHydroMeter;
    TToolButton *ToolButton12;
    TAction *actVTHLogger;
    TAction *actCCTV;
    TStaticText *stTime;
	TClientSocket *SenderSocket;
	TClientSocket *LoggerSocket;
	TAction *actLoggerUIOpen;
	TAction *actSenderUIOpen;
	TAction *actExit;
	void __fastcall tmrDisplayTimeTimer(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SplitterMoved(TObject *Sender);
    void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
    void __fastcall SplitterCanResize(TObject *Sender, int &NewSize, bool &Accept);
	void __fastcall tmrConnectToTCPServerTimer(TObject *Sender);
    void __fastcall SenderClientConnected(TObject *Sender);
    void __fastcall LoggerClientConnected(TObject *Sender);
    void __fastcall SenderClientDisconnected(TObject *Sender);
    void __fastcall LoggerClientDisconnected(TObject *Sender);
    void __fastcall tmrReadSenderPacketTimer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall tmrReadLoggerPacketTimer(TObject *Sender);
    void __fastcall actShowConfigExecute(TObject *Sender);
    void __fastcall miAboutRADSDataViewerClick(TObject *Sender);
    void __fastcall actShowLogExecute(TObject *Sender);
    void __fastcall miUserClick(TObject *Sender);
    void __fastcall actShowReportExecute(TObject *Sender);
    void __fastcall actArrangeWindowExecute(TObject *Sender);
    void __fastcall actQueryReportExecute(TObject *Sender);
    void __fastcall actWaterFlowExecute(TObject *Sender);
    void __fastcall actWaterLevelExecute(TObject *Sender);
    void __fastcall actHydroMeterExecute(TObject *Sender);
    void __fastcall actVTHLoggerExecute(TObject *Sender);
    void __fastcall actCCTVExecute(TObject *Sender);
    void __fastcall ApplicationEventsException(TObject *Sender, Exception *E);
	void __fastcall LoggerSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall LoggerSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall LoggerSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall LoggerSocketRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall SenderSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall SenderSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall SenderSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall SenderSocketRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall actLoggerUIOpenExecute(TObject *Sender);
	void __fastcall actSenderUIOpenExecute(TObject *Sender);
	void __fastcall actExitExecute(TObject *Sender);


private:	// User declarations

	BYTE m_LoggerBuffer[1024];
	BYTE m_SenderBuffer[1024];
	int  m_LoggerIndex;
	int  m_SenderIndex;

	bool m_bLoggerConnect;
	bool m_bSenderConnect;

	float fCharge, fTemperature, fHumidity, fBattery;
	bool  bLoggedIn;
	TPacketBuffer *pbSender;
	TPacketBuffer *pbLogger;

	bool DoLogin();
	void ReadIni();
	bool ConnectDB();
	bool ConnectToSenderServer();
	bool ConnectToLoggerServer();
	void _WriteDBLog(UnicodeString sDateTime, UnicodeString sText);
	void ArrangeChildren();
	void ArrangeLog();
	void ArrangeVth();
	void ArrangeFormSearchData();
	void QueryAllChildren();

	//heaven2 2014.07.17
	void ProcessLogger(BYTE *pBuffer, int iLen);
	void ProcessSender(BYTE *pBuffer, int iLen);

public:		// User declarations

	bool bRunning;
	EN_LOGGER_STATUS m_enLoggerStatus;
	EN_SENDER_STATUS m_enSenderStatus;
	int  iClickedTick;
	//void GetDBEnvInfo();
	bool GetDBEnvInfo();

	TDataViewerDBEnv DBEnv;	// DB Enviremdent

	__fastcall TFormMain(TComponent* Owner);

	void WriteLog(bool bFileWrite, bool bDBWrite, UnicodeString sText);
	void SendMessageToServer(char cMessageKind);

	bool SendLogger(BYTE cType, BYTE *pPayload, int iLen);
	bool SendSender(BYTE cType, BYTE *pPayload, int iLen);

	void OnMessage(TMessage &Message);	// Main Message process

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_USER, TMessage, OnMessage)
	END_MESSAGE_MAP(TForm)
};

BYTE    MakeCRC(BYTE *pBuff, int iLen);
int     CheckFrame(BYTE *pBuff, int iLen);

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
