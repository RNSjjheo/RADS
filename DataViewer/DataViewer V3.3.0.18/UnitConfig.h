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

// Hydro Device Idx
#define NONE_IDX			0
#define CHANNELMASTER_IDX	1
#define SL_SERIES_IDX		2
#define AG_SERIES_IDX		3
#define ACCUSONIC_A_IDX		4
#define ACCUSONIC_B_IDX		5
#define RQ_30_IDX			6
#define RQ_30D_IDX			7
#define EWSVCONVERTER_IDX	8

// WaterLevel Device Index
//#define NONE_IDX			0
#define AT200_IDX			1
#define LT400_ATMOS_IDX		2
#define LT500_IDX			3
#define OTTCBS_IDX			4
#define RNSWATER_IDX		5

#define BAUD_NONE_IDX		0
#define BAUD_2400_IDX		1
#define BAUD_4800_IDX       2
#define BAUD_9600_IDX       3
#define BAUD_14400_IDX      4
#define BAUD_19200_IDX      5
#define BAUD_28800_IDX      6
#define BAUD_33600_IDX      7
#define BAUD_38400_IDX      8
#define BAUD_56000_IDX      9

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
    TButton *btnSave7;
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
    TEdit *ed2H1WS;
    TEdit *ed2H1WF;
    TLabel *Label59;
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
    TTabSheet *TabSheet8;
    TButton *btnSave6;
    TPageControl *PageControlFlowStream;
    TTabSheet *tsADVM;
    TLabel *Label89;
    TLabel *Label90;
    TBevel *Bevel5;
    TBevel *Bevel6;
    TLabel *Label91;
    TLabel *Label92;
    TLabel *Label93;
    TLabel *Label94;
    TLabel *Label95;
    TLabel *Label96;
    TLabel *Label97;
    TLabel *Label98;
    TLabel *Label99;
    TLabel *Label100;
    TLabel *Label101;
    TLabel *Label102;
    TLabel *Label103;
    TLabel *Label104;
    TLabel *Label105;
    TLabel *Label106;
    TLabel *Label107;
    TLabel *Label108;
    TLabel *Label109;
    TLabel *Label110;
    TLabel *Label111;
    TLabeledEdit *AXadvm;
    TLabeledEdit *AZadvm;
    TLabeledEdit *APan;
    TLabeledEdit *ATilt;
    TLabeledEdit *APhi;
    TLabeledEdit *ASubmersion;
    TLabeledEdit *AVMin;
    TLabeledEdit *AStartCell;
    TLabeledEdit *BXadvm;
    TLabeledEdit *BZadvm;
    TLabeledEdit *BPan;
    TLabeledEdit *BTilt;
    TLabeledEdit *BPhi;
    TLabeledEdit *BSubmersion;
    TLabeledEdit *BVMin;
    TLabeledEdit *BStartCell;
    TComboBox *AReverseMode;
    TComboBox *AUnitVelocity;
    TComboBox *BUnitVelocity;
    TComboBox *BReverseMode;
    TEdit *AVMax;
    TEdit *AEndCell;
    TEdit *BEndCell;
    TEdit *BVMax;
    TComboBox *BType;
    TTabSheet *tsHA;
    TLabel *Label112;
    TLabel *Label113;
    TLabel *Label114;
    TLabel *Label115;
    TLabel *Label116;
    TLabel *Label117;
    TLabel *Label118;
    TLabel *Label119;
    TLabel *Label120;
    TLabeledEdit *HAMinH;
    TLabeledEdit *HAA6;
    TLabeledEdit *HAA5;
    TLabeledEdit *HAA4;
    TLabeledEdit *HAA3;
    TLabeledEdit *HAA2;
    TLabeledEdit *HAA1;
    TEdit *HAMaxH;
    TLabeledEdit *HAA0;
    TTabSheet *tsIVM;
    TLabel *Label121;
    TLabel *Label122;
    TBevel *Bevel7;
    TBevel *Bevel8;
    TLabel *Label123;
    TLabel *Label124;
    TLabel *Label125;
    TLabel *Label126;
    TLabel *Label127;
    TLabel *Label128;
    TLabel *Label129;
    TLabel *Label130;
    TLabel *Label131;
    TLabel *Label132;
    TLabeledEdit *IVMAMinVi;
    TLabeledEdit *IVMAA;
    TLabeledEdit *IVMAB;
    TLabeledEdit *IVMAC;
    TLabeledEdit *IVMBMinVi;
    TLabeledEdit *IVMBA;
    TLabeledEdit *IVMBB;
    TLabeledEdit *IVMBC;
    TEdit *IVMAMaxVi;
    TEdit *IVMBMaxVi;
	TLabel *Label134;
	TComboBox *cbx3FTPMode;
	TLabel *Label135;
	TComboBox *cbx3RnseaMode;
	TLabel *Label1;
	TComboBox *cbx7StreamMethod;
	TLabeledEdit *led2Hz;
	TLabeledEdit *led2SN;
	TLabeledEdit *led2Hz2;
	TLabeledEdit *led2SN2;
	TLabeledEdit *led2WLSN;
	TPanel *Panel1;
	TPanel *Panel2;
	TLabel *Label56;
	TLabel *Label57;
	TLabel *Label58;
	TLabel *Label60;
	TEdit *ed2H2WF;
	TEdit *ed2H2WN;
	TEdit *ed2H2WS;
	TLabel *Label133;
	TComboBox *cbx3VTHOption;
	TLabel *lbAtmosCaption;
	TComboBox *cbx2AtmosPort;
	TLabeledEdit *edBaseAtmos;
	TOpenDialog *OpenDialog;
	TComboBox *cbx3FTPType;
	TComboBox *cbx3FTPType2;
	TLabeledEdit *led3PrivateKey;
	TButton *btnPrivateKey;
	TLabeledEdit *led3PrivateKey2;
	TButton *btnPrivateKey2;
	TLabel *Label136;
	TComboBox *cbx2WLIdleCheck;
	TComboBox *cbx2WLIdleCheckTime;
	TLabel *Label137;
	TComboBox *cbx2H1IdleCheck;
	TComboBox *cbx2H1IdleCheckTime;
	TLabel *Label138;
	TComboBox *cbx2H2IdleCheck;
	TComboBox *cbx2H2IdleCheckTime;
	TPanel *PanelAirmar;
	TLabel *Label142;
	TLabel *Label139;
	TComboBox *cbx2H1AirmarPort;
	TLabel *Label140;
	TComboBox *cbx2H1AirmarBaud;
	TLabel *Label141;
	TLabel *Label143;
	TEdit *led2EndCell;
	TLabel *Label144;
	TEdit *led2StartCell2;
	TLabel *Label145;
	TEdit *led2EndCell2;
	TEdit *led2StartCell;
	TCheckBox *cbAirmarLinker;
	TPanel *PanelRQ30d;
	TLabel *lbHydro1SlavePort;
	TLabel *lbHydro1SlaveBaudRate;
	TLabel *lbHydro1SlaveAddress;
	TLabel *Label146;
	TLabel *Label147;
	TLabel *Label148;
	TEdit *edHydro1SlaveAddress;
	TComboBox *cbxSlaveCount;
	TEdit *edHydro1MasterAddress;
	TCheckBox *cbxHydro1UseSlavePort;
	TComboBox *cbHydro1SlavePort;
	TComboBox *cbHydro1SlaveBaudRate;
	TGroupBox *GroupBox4;
	TLabel *Label149;
	TLabel *Label150;
	TLabel *Label151;
	TLabel *Label152;
	TLabel *Label153;
	TLabel *Label154;
	TLabel *Label155;
	TLabel *Label156;
	TPanel *Panel3;
	TLabel *Label157;
	TLabel *Label158;
	TLabel *Label159;
	TLabel *Label160;
	TEdit *ed2H3WF;
	TEdit *ed2H3WN;
	TEdit *ed2H3WS;
	TComboBox *cbx2H3Kind;
	TComboBox *cbx2H3Comport;
	TComboBox *cbx2H3CommSpeed;
	TLabeledEdit *led2H3Offset;
	TComboBox *cbx2H3ReverseOption;
	TComboBox *cbx2H3UpSideOption;
	TLabeledEdit *led2Hz3;
	TLabeledEdit *led2SN3;
	TComboBox *cbx2H3IdleCheck;
	TComboBox *cbx2H3IdleCheckTime;
	TEdit *led2StartCell3;
	TEdit *led2EndCell3;
	TCheckBox *cbx2WaterAtmos;
	TCheckBox *cbx2WaterAirmar;
	TLabel *lb2WaterAtmos;
	TLabel *lb2WaterAirmar;
	TLabel *Label161;
	TComboBox *cbx2WaterUseFlag;
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
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall cbx7StreamMethodChange(TObject *Sender);
	void __fastcall cbx2WLKindChange(TObject *Sender);
	void __fastcall cbx3FTPTypeChange(TObject *Sender);
	void __fastcall cbx3FTPType2Change(TObject *Sender);
	void __fastcall btnPrivateKeyClick(TObject *Sender);
	void __fastcall btnPrivateKey2Click(TObject *Sender);
	void __fastcall cbxHydro1UseSlavePortClick(TObject *Sender);
	void __fastcall cbx2WaterAtmosClick(TObject *Sender);
	void __fastcall cbx2WaterAirmarClick(TObject *Sender);
	void __fastcall cbx2H3KindChange(TObject *Sender);
	void __fastcall cbx2H1AirmarPortChange(TObject *Sender);
private:	// User declarations
	bool bSaved;
	bool m_bLoggerNotify;
	bool m_bSenderNotify;

	UnicodeString m_AtmosPort;
public:		// User declarations
    __fastcall TFormConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
void ShowConfig(int iPageSelections, int iActivePageIndex);
//---------------------------------------------------------------------------
extern PACKAGE TFormConfig *FormConfig;
//---------------------------------------------------------------------------
#endif
