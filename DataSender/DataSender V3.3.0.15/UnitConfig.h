//---------------------------------------------------------------------------
#ifndef UnitConfigH
#define UnitConfigH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Data.SqlExpr.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Data.FMTBcd.hpp>
#include <Datasnap.DBClient.hpp>
#include <Datasnap.Provider.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Data.DBXFirebird.hpp>
#include <SimpleDS.hpp>
//---------------------------------------------------------------------------
class TFormConfig : public TForm
{
__published:	// IDE-managed Components
    TPageControl *pcConfig;
    TButton *btnClose;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet5;
    TTabSheet *TabSheet6;
    TButton *btnSave0;
    TButton *btnSave1;
    TButton *btnSave2;
    TButton *btnSave3;
    TButton *btnSave4;
    TButton *btnSave6;
    TLabeledEdit *led1PointCode;
    TLabeledEdit *led1PointName;
    TLabeledEdit *led1PointAddress;
    TLabeledEdit *led1PointZipCode;
    TLabeledEdit *led1PointManager;
    TLabeledEdit *led1PhoneTel;
    TLabeledEdit *led1PointEtc;
    TLabeledEdit *lel5Ment;
    TLabeledEdit *led5LevelOverSea;
    TComboBox *cbx4GuardnetYN;
    TLabel *Label2;
    TComboBox *cbx4GuardnetType;
    TLabel *Label3;
    TLabel *Label4;
    TMaskEdit *med4PCBootingTime;
    TLabel *Label5;
    TLabel *Label7;
    TLabel *Label8;
    TComboBox *cbx4PingStartTime;
    TLabeledEdit *led4PingServer1;
    TLabeledEdit *led4Server1Port;
    TLabeledEdit *led4Server2;
    TLabeledEdit *led4Server2Port;
    TComboBox *cbx4Ping2TestYN;
    TLabel *Label9;
    TComboBox *cbx4PingRetryCount;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TComboBox *cbx4PingRetryCountM;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label16;
    TLabel *Label17;
    TComboBox *cbx4GWResetCount;
    TLabel *Label18;
    TLabel *Label19;
    TComboBox *cbx4MaxPCResetCount;
    TLabel *Label20;
    TLabel *Label23;
    TComboBox *cbx4GWResetDelay;
    TLabeledEdit *led4Interval;
    TLabeledEdit *led4WirelessUseTime;
    TLabeledEdit *led4LineIF;
    TLabeledEdit *led4WirelessIF;
    TLabeledEdit *ledHome;
    TLabeledEdit *ledLogger;
    TLabeledEdit *ledSender;
    TLabeledEdit *ledViewer;
    TComboBox *cbx3ServerSendYN;
    TLabel *Label6;
    TLabeledEdit *led3ServerDomain;
    TLabeledEdit *led3ServerPort;
    TLabeledEdit *led3ServerID;
    TLabeledEdit *led3ServerPW;
    TLabeledEdit *led3ServerReal;
    TLabeledEdit *led3ServerPast;
    TLabeledEdit *led3RnseaDomain;
    TLabeledEdit *led3RnseaPort;
    TLabeledEdit *led3RnseaID;
    TLabeledEdit *led3RnseaPW;
    TLabeledEdit *led3RnseaReal;
    TLabeledEdit *led3RnseaPast;
    TComboBox *cbx3RnseaSendYN;
    TComboBox *cbx3FtpFileFormat;
    TLabel *Label15;
    TLabel *Label21;
    TLabel *Label22;
    TMaskEdit *med2Interval;
    TLabel *Label24;
    TLabel *Label25;
    TMaskEdit *med2Period;
    TLabel *Label26;
    TComboBox *cbx2StartTime;
    TLabel *Label27;
    TLabel *Label28;
    TGroupBox *gbxHydroMeter1;
    TGroupBox *gbxHydroMeter2;
    TGroupBox *gbxWaterLevel;
    TGroupBox *gbxVTHLogger;
    TComboBox *cbx3RetryCount;
    TComboBox *cbx3RebootFlag;
    TComboBox *cbx3RebootRetryCount;
    TComboBox *cbx3VTHComport;
    TComboBox *cbx3VTHCommSpeed;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TComboBox *cbx2H1Kind;
    TComboBox *cbx2H1Comport;
    TLabel *Label35;
    TComboBox *cbx2H1CommSpeed;
    TLabel *Label36;
    TLabeledEdit *led2H1Offset;
    TLabel *Label37;
    TComboBox *cbx2H2Kind;
    TComboBox *cbx2H2Comport;
    TComboBox *cbx2H2CommSpeed;
    TLabeledEdit *led2H2Offset;
    TLabel *Label38;
    TLabel *Label39;
    TComboBox *cbx2WLKind;
    TComboBox *cbx2WLComport;
    TComboBox *cbx2WLCommSpeed;
    TLabeledEdit *led2WLOffset;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TGroupBox *gbxGraph;
    TGroupBox *gbxEmail;
    TLabel *Label48;
    TComboBox *cbxEmailAutoSend;
    TLabel *Label53;
    TLabel *Label54;
    TLabel *Label55;
    TEdit *ed2H1WN;
    TEdit *ed2H2WN;
    TLabel *Label56;
    TLabel *Label57;
    TLabel *Label58;
    TEdit *ed2H1WS;
    TEdit *ed2H1WF;
    TEdit *ed2H2WS;
    TEdit *ed2H2WF;
    TLabel *Label59;
    TLabel *Label60;
    TBevel *Bevel1;
    TBevel *Bevel2;
    TLabeledEdit *ledEmailServer;
    TLabeledEdit *ledEmailAccount;
    TLabeledEdit *ledEmailPassword;
    TLabeledEdit *ledEmailDest1;
    TLabeledEdit *ledEmailDest2;
    TLabeledEdit *ledEmailDest3;
    TLabeledEdit *ledEmailDest4;
    TLabeledEdit *ledEmailDest5;
    TSpeedButton *sbtnSelectIF1;
    TSpeedButton *sbtnSelectIF2;
    TPanel *pnlIF;
    TListBox *lbxIF;
    TLabeledEdit *led5CctvUrl;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TCheckBox *cbxWaterFlow;
    TCheckBox *cbxWaterLevel;
    TCheckBox *cbxHydroMeter1;
    TCheckBox *cbxLog;
    TCheckBox *cbxVthLogger;
    TComboBox *cbx3MaxSendFileCount;
    TLabel *Label43;
    TBevel *Bevel3;
    TBevel *Bevel4;
    TLabel *Label44;
    TLabel *Label45;
    TSimpleDataSet *SimpleDataSet;
    TComboBox *cbx2H1ReverseOption;
    TLabel *Label46;
    TLabel *Label47;
    TComboBox *cbx2H2ReverseOption;
    TComboBox *cbx2H2UpSideOption;
    TLabel *Label49;
    TLabeledEdit *ledDataLoggerIP;
    TLabeledEdit *ledDataSenderIP;
    TLabeledEdit *ledFirebirdIP;
    TLabeledEdit *ledDataLoggerPort;
    TLabeledEdit *ledDataSenderPortFromLogger;
    TLabeledEdit *ledDataSenderPortFromViewer;
    TLabeledEdit *ledWatchDogPort;
    TLabel *Label51;
    TLabel *Label50;
    TLabel *Label52;
    TLabel *Label61;
    TComboBox *cbx5LoginDelayTime;
    TLabel *Label62;
    TLabel *Label1;
    TComboBox *cbx2StreamMethod;
    TTabSheet *TabSheet7;
    TButton *btnSave5;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TLabel *Label63;
    TLabel *Label64;
    TLabel *Label65;
    TLabel *Label66;
    TLabel *Label67;
    TEdit *ed6VoltMin;
    TLabel *Label68;
    TEdit *ed6ChargeMin;
    TLabel *Label69;
    TEdit *ed6ChargeMax;
    TEdit *ed6BatteryMin;
    TLabel *Label70;
    TEdit *ed6BatteryMax;
    TEdit *ed6SysTempMin;
    TLabel *Label71;
    TEdit *ed6SysTempMax;
    TEdit *ed6SysHumidMin;
    TLabel *Label72;
    TEdit *ed6SysHumidMax;
    TEdit *ed6VoltMax;
    TLabel *Label73;
    TLabel *Label74;
    TLabel *Label75;
    TLabel *Label76;
    TLabel *Label77;
    TEdit *ed6TempMin;
    TLabel *Label78;
    TEdit *ed6PressureMin;
    TLabel *Label79;
    TEdit *ed6PressureMax;
    TEdit *ed6PitchMin;
    TLabel *Label80;
    TEdit *ed6PitchMax;
    TEdit *ed6RollMin;
    TLabel *Label81;
    TEdit *ed6RollMax;
	TEdit *ed6VelocityEwMin;
    TLabel *Label82;
	TEdit *ed6VelocityEwMax;
    TEdit *ed6TempMax;
    TLabel *Label83;
    TLabel *Label84;
    TEdit *ed6WaterLevelMin;
    TLabel *Label85;
    TEdit *ed6SalinityMin;
    TLabel *Label86;
    TEdit *ed6SalinityMax;
    TEdit *ed6WaterLevelMax;
	TLabel *Label87;
	TEdit *ed6VelocityNsMin;
	TLabel *Label88;
	TEdit *ed6VelocityNsMax;
	TLabel *Label134;
	TComboBox *cbx3FTPMode;
	TLabel *Label135;
	TComboBox *cbx3RnseaMode;
	TComboBox *cbx3VTHOption;
	TLabel *Label89;
	TMemo *MemoPingLog;
	TButton *ButtonPing;
	TButton *ButtonReboot;
	TTimer *TimerPing;
	TComboBox *cbx3FTPType;
	TComboBox *cbx3FTPType2;
	TOpenDialog *OpenDialog;
	TLabeledEdit *led3PrivateKey;
	TButton *btnPrivateKey;
	TLabeledEdit *led3PrivateKey2;
	TButton *btnPrivateKey2;
	TDateTimePicker *ReSendFromDate;
	TDateTimePicker *ReSendToDate;
	TMaskEdit *ReSendFromTime;
	TMaskEdit *ReSendToTime;
	TLabel *Label90;
	TLabel *Label91;
	TButton *btnReSend;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall pcConfigChange(TObject *Sender);
    void __fastcall FloatEditChange(TObject *Sender);
    void __fastcall cbx2H1KindChange(TObject *Sender);
    void __fastcall cbx2H2KindChange(TObject *Sender);
    void __fastcall OnSelectIF(TObject *Sender);
    void __fastcall lbxIFExit(TObject *Sender);
    void __fastcall lbxIFClick(TObject *Sender);
    void __fastcall OnOffsetExit(TObject *Sender);
    void __fastcall OnFolderHelpClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonPingClick(TObject *Sender);
	void __fastcall ButtonRebootClick(TObject *Sender);
	void __fastcall TimerPingTimer(TObject *Sender);
	void __fastcall cbx3FTPTypeChange(TObject *Sender);
	void __fastcall cbx3FTPType2Change(TObject *Sender);
	void __fastcall btnPrivateKeyClick(TObject *Sender);
	void __fastcall btnPrivateKey2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnReSendClick(TObject *Sender);
private:	// User declarations
	bool bSaved;
	int  m_iPingCnt;	// Ping Packet Counter
public:		// User declarations
	__fastcall TFormConfig(TComponent* Owner);

	void PingLog(UnicodeString sMsg);
	void DataLoggerMessage(BYTE *pData, int Len);
};
//---------------------------------------------------------------------------
void ShowConfig(int iPageSelections, int iActivePageIndex);
//---------------------------------------------------------------------------
extern PACKAGE TFormConfig *FormConfig;
//---------------------------------------------------------------------------
#endif
