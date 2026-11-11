//---------------------------------------------------------------------------

#ifndef UnitSetupH
#define UnitSetupH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>

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

//---------------------------------------------------------------------------
class TFormSetup : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageSetup;
	TTabSheet *TabSheetPointInfo;
	TButton *cbSavePoint;
	TLabeledEdit *edPointCode;
	TLabeledEdit *edPointName;
	TLabeledEdit *edPointAddress;
	TLabeledEdit *edZipCode;
	TLabeledEdit *edManager;
	TLabeledEdit *edManagerTel;
	TLabeledEdit *edEtc;
	TTabSheet *TabSheetLoggerInfo;
	TLabel *Label22;
	TLabel *Label24;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TButton *cbSaveLogger;
	TMaskEdit *meInterval;
	TMaskEdit *mePeriod;
	TComboBox *cbxStartTime;
	TGroupBox *gbxHydroMeter1;
	TLabel *Label34;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label46;
	TComboBox *cbxHydro1Type;
	TComboBox *cbxHydro1ComPort;
	TComboBox *cbxHydro1BaudRate;
	TLabeledEdit *edHydro1Offset;
	TComboBox *cbxHydro1Reverse;
	TGroupBox *gbxHydroMeter2;
	TLabel *Label37;
	TLabel *Label38;
	TLabel *Label39;
	TLabel *Label47;
	TLabel *Label49;
	TComboBox *cbxHydro2Type;
	TComboBox *cbxHydro2ComPort;
	TComboBox *cbxHydro2BaudRate;
	TLabeledEdit *edHydro2Offset;
	TComboBox *cbxHydro2Reverse;
	TComboBox *cbxHydro2UpSide;
	TGroupBox *gbxWaterLevel;
	TLabel *Label40;
	TLabel *Label41;
	TLabel *Label42;
	TComboBox *cbxWaterType;
	TComboBox *cbxWaterComPort;
	TComboBox *cbxWaterBaudRate;
	TLabeledEdit *edWaterOffset;
	TTabSheet *TabSheetEtc;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TLabel *Label51;
	TLabel *Label50;
	TLabel *Label52;
	TButton *cbSaveEtc;
	TLabeledEdit *edRADSHome;
	TLabeledEdit *edLoggerHome;
	TLabeledEdit *edSenderHome;
	TLabeledEdit *edViewerHome;
	TLabeledEdit *edLoggerIP;
	TLabeledEdit *edSenderIP;
	TLabeledEdit *edDatabaseIP;
	TLabeledEdit *edLoggerPort;
	TLabeledEdit *edSenderLoggerPort;
	TLabeledEdit *edSenderViewerPort;
	TLabeledEdit *edWatchDogPort;
	TBitBtn *BitBtn1;
	TTabSheet *TabSheetFlowStream;
	TLabel *Label2;
	TComboBox *cbxFlowStreamType;
	TButton *cbSaveFlowStream;
	TPageControl *PageControlFlowStream;
	TTabSheet *tsADVM;
	TTabSheet *tsHA;
	TTabSheet *tsIVM;
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
	TLabel *Label1;
	TLabel *Label3;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TComboBox *AReverseMode;
	TComboBox *AUnitVelocity;
	TComboBox *BUnitVelocity;
	TComboBox *BReverseMode;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *AVMax;
	TEdit *AEndCell;
	TEdit *BEndCell;
	TEdit *BVMax;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TComboBox *BType;
	TLabel *Label12;
	TLabeledEdit *HAMinH;
	TLabeledEdit *HAA6;
	TLabeledEdit *HAA5;
	TLabeledEdit *HAA4;
	TLabeledEdit *HAA3;
	TLabeledEdit *HAA2;
	TLabeledEdit *HAA1;
	TEdit *HAMaxH;
	TLabel *Label13;
	TLabeledEdit *HAA0;
	TLabel *Label14;
	TLabel *Label15;
	TBevel *Bevel5;
	TBevel *Bevel6;
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
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label23;
	TLabel *Label29;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label43;
	TLabel *Label44;
	TLabel *Label45;
	TLabel *Label48;
	TLabel *Label61;
	TLabel *Label62;
	TLabel *Label63;
	TLabel *Label64;
	TLabel *Label65;
	TLabel *Label66;
	TLabel *Label67;
	TLabel *Label68;
	TLabel *Label69;
	TLabel *Label70;
	TLabel *Label71;
	TLabel *Label72;
	TLabel *Label73;
	TLabel *Label74;
	TLabel *Label75;
	TLabeledEdit *edHydro1Hz;
	TLabeledEdit *edHydro1SN;
	TLabeledEdit *edHydro2Hz;
	TLabeledEdit *edHydro2SN;
	TLabeledEdit *edWaterSN;
	TPanel *Panel1;
	TLabel *Label53;
	TLabel *Label54;
	TLabel *Label55;
	TLabel *Label59;
	TEdit *edHydro1Blank;
	TEdit *edHydro1CellNo;
	TEdit *edHydro1CellSize;
	TPanel *Panel2;
	TLabel *Label56;
	TLabel *Label57;
	TLabel *Label58;
	TLabel *Label60;
	TEdit *edHydro2Blank;
	TEdit *edHydro2CellNo;
	TEdit *edHydro2CellSize;
	TLabel *lbAtmosCap;
	TComboBox *cbxAtmosPort;
	TLabeledEdit *edBaseAtmos;
	TButton *ButtonPing;
	TMemo *MemoPingLog;
	TButton *ButtonReboot;
	TTimer *TimerPing;
	TComboBox *cbxWaterIdleCheck;
	TComboBox *cbxWaterIdleCheckTime;
	TComboBox *cbxHydro1IdleCheck;
	TComboBox *cbxHydro1IdleCheckTime;
	TLabel *Label18;
	TComboBox *cbxHydro2IdleCheck;
	TComboBox *cbxHydro2IdleCheckTime;
	TLabel *Label76;
	TLabel *Label77;
	TPanel *PanelAirmar;
	TLabel *Label81;
	TLabel *Label78;
	TComboBox *cbxAirmar1ComPort;
	TLabel *Label79;
	TComboBox *cbxAirmar1BaudRate;
	TEdit *edHydro1StartCell;
	TEdit *edHydro1EndCell;
	TLabel *Label80;
	TLabel *Label82;
	TLabel *Label83;
	TEdit *edHydro2StartCell;
	TEdit *edHydro2EndCell;
	TLabel *Label84;
	TCheckBox *cbAirmarLinker;
	TPanel *PanelRQ30d;
	TLabel *lbHydro1SlavePort;
	TLabel *lbHydro1SlaveBaudRate;
	TLabel *lbHydro1SlaveAddress;
	TLabel *Label88;
	TEdit *edHydro1SlaveAddress;
	TComboBox *cbxSlaveCount;
	TLabel *LabelSlaveCount;
	TEdit *edHydro1MasterAddress;
	TCheckBox *cbxHydro1UseSlavePort;
	TComboBox *cbHydro1SlavePort;
	TComboBox *cbHydro1SlaveBaudRate;
	TLabel *Label89;
	TCheckBox *cxWaterAirmar;
	TLabel *lbAirmarCap;
	TLabel *lbAtmosEnable;
	TCheckBox *cxWaterAtmos;
	TPanel *PanelEWSV;
	TLabel *Label85;
	TLabel *Label86;
	TLabel *Label87;
	TComboBox *cbxEWSVAvg;
	TComboBox *cbxEWSVSensorAvg;
	TEdit *edEWSVWorkTime;
	TLabel *Label90;
	TGroupBox *gbxHydroMeter3;
	TLabel *Label91;
	TLabel *Label92;
	TLabel *Label93;
	TLabel *Label94;
	TLabel *Label95;
	TLabel *Label96;
	TLabel *Label97;
	TLabel *Label98;
	TPanel *Panel3;
	TLabel *Label99;
	TLabel *Label100;
	TLabel *Label101;
	TLabel *Label102;
	TEdit *edHydro3Blank;
	TEdit *edHydro3CellNo;
	TEdit *edHydro3CellSize;
	TComboBox *cbxHydro3Type;
	TComboBox *cbxHydro3ComPort;
	TComboBox *cbxHydro3BaudRate;
	TLabeledEdit *edHydro3Offset;
	TComboBox *cbxHydro3Reverse;
	TComboBox *cbxHydro3UpSide;
	TLabeledEdit *edHydro3Hz;
	TLabeledEdit *edHydro3SN;
	TComboBox *cbxHydro3IdleCheck;
	TComboBox *cbxHydro3IdleCheckTime;
	TEdit *edHydro3StartCell;
	TEdit *edHydro3EndCell;
	TLabel *Label103;
	TComboBox *cbxWaterUseFlag;
	void __fastcall PageSetupChange(TObject *Sender);
	void __fastcall cbSavePointClick(TObject *Sender);
	void __fastcall cbSaveLoggerClick(TObject *Sender);
	void __fastcall cbSaveEtcClick(TObject *Sender);
	void __fastcall cbCloseClick(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall cbSaveFlowStreamClick(TObject *Sender);
	void __fastcall cbxHydro1TypeChange(TObject *Sender);
	void __fastcall cbxHydro2TypeChange(TObject *Sender);
	void __fastcall cbxFlowStreamTypeChange(TObject *Sender);
	void __fastcall cbxWaterTypeChange(TObject *Sender);
	void __fastcall ButtonPingClick(TObject *Sender);
	void __fastcall ButtonRebootClick(TObject *Sender);
	void __fastcall TimerPingTimer(TObject *Sender);
	void __fastcall cbxHydro1UseSlavePortClick(TObject *Sender);
	void __fastcall cxWaterAirmarClick(TObject *Sender);
	void __fastcall cxWaterAtmosClick(TObject *Sender);
	void __fastcall cbxHydro3TypeChange(TObject *Sender);
	void __fastcall cbxAirmar1ComPortChange(TObject *Sender);
private:	// User declarations

public:
	bool m_bChanged;	// Data Changed or not
	int  m_iPingCnt;	// Ping Packet Counter

private:
	void ClearPoint();
	void ClearLogger();
	void ClearFlowStream();
	void ClearEtc();

	bool CheckPoint();
	bool CheckLogger();
	bool CheckFlowStream();
	bool CheckEtc();

	bool LoadPointFromDB();
	bool LoadLoggerFromDB();
	bool LoadFlowStreamFromDB();
	bool LoadEtcFromDB();

	bool SavePointToDB();
	bool SaveLoggerToDB();
	bool SaveFlowStreamToDB();
	bool SaveEtcToDB();

	void LogMessage(UnicodeString sMsg, bool bFile=true, bool bDB=false);

	void PingLog(UnicodeString sMsg);

public:
	void DataSenderMessage(BYTE *pData, int Len);


public:		// User declarations
	__fastcall TFormSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSetup *FormSetup;
//---------------------------------------------------------------------------
#endif
