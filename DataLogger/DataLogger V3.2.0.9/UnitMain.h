//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Win.ScktComp.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>

#include "AdPort.hpp"
#include "OoMisc.hpp"
//---------------------------------------------------------------------------

#include "UnitLog.h"
#include "UnitDM.h"
#include "UnitEnv.h"
#include "./WaterLevel/Atmos.h"
//#include "./HydroMeter/Airmar.h"
#include "./Database/RPoint.h"
#include "./Database/RFlowIVM.h"
#include "./Database/RFlowHA.h"
#include "./Database/RFlowADVM.h"

//#include "HydroMeter.h"
//#include "WaterLevel.h"
//#include "FlowStream.h"
//#include "Animation.h"

class TFormSetup;
class THydroMeter;
class TWaterLevel;
//class TAtmos;
class TAirmar;
class TFlowStream;
class TAniThread;
class TLogThread;
class THydroSetup;

//class TRFlowIVM;
//class TRFlowHA;
//class TRFlowADVM;

class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TImage *ImageFlow;
	TProgressBar *ProgressBar;
	TLabel *lbPTime;
	TButton *cbOpenClose;
	TImage *ImageHydro1;
	TImage *ImageHydro2;
	TImage *ImageWaterLevel;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *Memo;
	TTimer *TimerGeneral;
	TTrayIcon *TrayIcon;
	TPopupMenu *PopupMenu;
	TMenuItem *Open1;
	TMenuItem *Close1;
	TLabel *lbTime;
	TTimer *TimerTray;
	TImageList *ImageListFlow;
	TImageList *ImageListCM;
	TImageList *ImageListSONTEK;
	TImageList *ImageListWaterLevel;
	TTimer *TimerWater;
	TTimer *TimerHydro1;
	TTimer *TimerWatchDog;
	TTimer *TimerSender;
	TLabel *lbLastLogTime;
	TImage *ImageSender;
	TImage *ImageWatchDog;
	TImage *ImageViewer;
	TButton *cbSetup;
	TImageList *ImageListSocket;
	TApdComPort *HydroPort2;
	TApdComPort *HydroPort1;
	TApdComPort *WaterPort;
	TTimer *TimerHydro2;
	TServerSocket *ServerSocket;
	TClientSocket *SenderSocket;
	TClientSocket *WatchDogSocket;
	TTimer *TimerViewer;
	TListView *ListView;
	TButton *cbSensorSetup;
	TTimer *TimerBREAK;
	TApdComPort *AtmosPort;
	TLabel *lbVersion;
	TApdComPort *AirmarPort;
	TButton *ButtonTest;
	TApdComPort *RQSlavePort;
	void __fastcall cbOpenCloseClick(TObject *Sender);
	void __fastcall TimerGeneralTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall TimerTrayTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TimerWaterTimer(TObject *Sender);
	void __fastcall TimerHydro1Timer(TObject *Sender);
	void __fastcall TimerWatchDogTimer(TObject *Sender);
	void __fastcall TimerSenderTimer(TObject *Sender);
	void __fastcall TrayIconDblClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TimerHydro2Timer(TObject *Sender);

	// Server Socket Event
	void __fastcall ServerSocketAccept(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocketListen(TObject *Sender, TCustomWinSocket *Socket);

	// DataSender Socket Event
	void __fastcall SenderSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall SenderSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall SenderSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
		  int &ErrorCode);
	void __fastcall SenderSocketRead(TObject *Sender, TCustomWinSocket *Socket);

	// WatchDog Socket Event
	void __fastcall WatchDogSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall WatchDogSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall WatchDogSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
		  int &ErrorCode);
	void __fastcall WatchDogSocketRead(TObject *Sender, TCustomWinSocket *Socket);

	// HydroPort1
	void __fastcall HydroPort1PortOpen(TObject *Sender);
	void __fastcall HydroPort1PortClose(TObject *Sender);
	void __fastcall HydroPort1TriggerAvail(TObject *CP, WORD Count);

	// HydroPort2
	void __fastcall HydroPort2PortOpen(TObject *Sender);
	void __fastcall HydroPort2PortClose(TObject *Sender);
	void __fastcall HydroPort2TriggerAvail(TObject *CP, WORD Count);

	// WaterPort
	void __fastcall WaterPortPortOpen(TObject *Sender);
	void __fastcall WaterPortPortClose(TObject *Sender);
	void __fastcall WaterPortTriggerAvail(TObject *CP, WORD Count);
	void __fastcall TimerViewerTimer(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall cbSetupClick(TObject *Sender);
	void __fastcall cbSensorSetupClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TimerBREAKTimer(TObject *Sender);
	void __fastcall AtmosPortPortClose(TObject *Sender);
	void __fastcall AtmosPortPortOpen(TObject *Sender);
	void __fastcall AtmosPortTriggerAvail(TObject *CP, WORD Count);
	void __fastcall WaterPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall AirmarPortPortOpen(TObject *Sender);
	void __fastcall AirmarPortPortClose(TObject *Sender);
	void __fastcall AirmarPortTriggerAvail(TObject *CP, WORD Count);
	void __fastcall AirmarPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall HydroPort1TriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall HydroPort2TriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall AtmosPortTriggerLineError(TObject *CP, WORD Error, bool LineBreak);
	void __fastcall ButtonTestClick(TObject *Sender);
	void __fastcall RQSlavePortPortOpen(TObject *Sender);
	void __fastcall RQSlavePortPortClose(TObject *Sender);
	void __fastcall RQSlavePortTriggerAvail(TObject *CP, WORD Count);
	void __fastcall RQSlavePortTriggerLineError(TObject *CP, WORD Error, bool LineBreak);



private:	// User declarations
public:
	// Attribute ------------------------------------------------------------
	EN_LOGGER_STEP m_enStep;	// Logger Step

	UnicodeString m_CurrentDir;	// DataLogger Execte Director

	TDateTime     m_StartTime;
	UnicodeString m_MeasureDate, m_MeasureTime;
	int           m_iSpendTime;

	char m_PreWaterLevel[BUFFER_8K];
	char m_PreAtmos[BUFFER_8K];
	BYTE m_PreAirmar[BUFFER_8K];
	char m_PreHydroMeter1[BUFFER_8K];
	char m_PreHydroMeter2[BUFFER_8K];
	char m_PreRQBuffer[BUFFER_8K];
	char m_ViewerBuffer[BUFFER_8K];
	int  m_RecvIndex;
	BYTE m_SenderBuffer[BUFFER_2K];
	int  m_SenderIndex;

	bool m_bExit;
	int  m_iDBErrorCnt;		// DB Query Counter

	TFormSetup  *m_FormSetup;   	// SetupForm Pointer

	TRPoint      m_RPoint;
	THydroMeter *m_pHydroMeter1;
	THydroMeter *m_pHydroMeter2;
	TWaterLevel *m_pWaterLevel;
	TAtmos      *m_pAtmos;
	TAirmar     *m_pAirmar;		// Airmar Class 2021.12.06
	TFlowStream *m_pFlowStream;

	bool         m_bHydroBreak;		// 유속계에 Break를 보냈는 지 여부(Logging 시작할 때 false로 만든다.) SL/AG에만 해당
	int          m_iRemainBreak;	// BREAK보내기까지 남은 시간(초)

	TRFlowIVM    m_FlowIVM;
	TRFlowHA     m_FlowHA;
	TRFlowADVM   m_FlowADVM;

	TAniThread  *m_pAniThread;    	// Animation Thread
	bool         m_bAniThread;

	TLogThread  *m_pLogThread;		// log Thread
	bool         m_bLogThread;

	int          m_iViewerCount;

	THydroSetup *m_pHydroSetup1;		// Hydro Setup Form Pointer
	THydroSetup *m_pHydroSetup2;		// Hydro Setup Form Pointer

	// Method ---------------------------------------------------------------

	bool GoToSystemTray();
	bool RestoreFromTray();
	bool LocateRightButton();

public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);

	bool InitLogger();	// Init WaterLevel/HydroMeter1/2,...
	void ClearLogger();	// Clear Logger Resoure and Reload Logger

	bool IsWorking();
	bool IsStartTime(TDateTime now);
	bool IsExpireTime(int iSpanTime);
	void GoNextStep(EN_LOGGER_STEP enStep);
	void GetMeasureDateTime(TDateTime now, UnicodeString &Date, UnicodeString &Time);
	bool IsComplete();						// All Sensor is complete or not
	void ShowSetupInfo();	// Show Setup information

	int  CalProgress(int iSpentTime); 					// 90초 => 23%
	UnicodeString SpentTimeToCaption(int iSpentTime);	// 90초 => "01:30"

	// Logger Status
	bool  IsAllComplete();	// 모든 작업이 끝나서 idle 상태인 지
	WORD GetLoggerStatus();	// 각 장비의 상태를 WORD 상태로 전송 ([WaterLevel][HydroMeter1][HydroMeter2][FlowStream])

	void SendLoggerStatusToSender(EN_LOGGER_STEP enStep);
	void SendLoggerStatusToViewer(EN_LOGGER_STEP enStep);
	void SendHeartBeatToWatchDog(BYTE Action);
	bool SendPing();
	bool SendPingAck();
	bool SendRebootReq();
	bool SendRebootRes();
	bool ProcessSenderMessage(BYTE *pData, int Len);

	void SetSenderSocketIcon(EN_SOCKET_ACTION enSocketAction);
	void SetWatchDogSocketIcon(EN_SOCKET_ACTION enSocketAction);
	void SetViewerSocketIcon(EN_SOCKET_ACTION enSocketAction);
	void SetWorkingIcon(EN_LOGGER_STEP enStep);
	void SetUIExtend(bool bEnlarge=true);

	void SetFlowStreamIcon(EN_FLOWSTREAM_ICON enFlowStreamIcon);
	void SetHydroMeter1Icon(EN_HYDROMETER_ICON enHydroMeterIcon);
	void SetHydroMeter2Icon(EN_HYDROMETER_ICON enHydroMeterIcon);
	void SetWaterLevelIcon(EN_WATERLEVEL_ICON enWaterLevelIcon);

	void LogMessage(UnicodeString sMsg, bool bFile=true, bool bDB=false);

	void OnMessage(TMessage &Message);	// Main Message process

	// Socket Event
	void ViewerMessage(BYTE *pBuffer, int iLen);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(UM_MESSAGE, TMessage, OnMessage)
	END_MESSAGE_MAP(TForm)

};

BYTE    MakeCRC(BYTE *pBuff, int iLen);
int     CheckFrame(BYTE *pBuff, int iLen);
int     GetTimeSpanBySecond(TDateTime SpanTime);	// 시간의 경과를 초로 환산한다.
SIZE_T  GetProcessWorkingSetSize();

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;

extern TCriticalSection *pGLogSection; 	// LogMessage Critical Section
extern TCriticalSection *pGMasterPortSection; 	// MasterPort  Critical Section
extern TCriticalSection *pGSlavePortSection; 	// SlavePort  Critical Section
extern TQueue *GLogQueue;	// global serail queue
extern TEvent *GLogEvent;	// global serial event
extern TEvent *GAniEvent;	// global ani event

//---------------------------------------------------------------------------

#endif
