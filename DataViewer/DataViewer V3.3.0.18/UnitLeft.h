//---------------------------------------------------------------------------

#ifndef UnitLeftH
#define UnitLeftH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>

#include "DvEnv.h"
#include "fbTable.h"

//---------------------------------------------------------------------------
class TFormLeft : public TForm
{
__published:	// IDE-managed Components
    TMemo *mmRecentCheckReport;
	TButton *Button1;
	TLabel *Label5;
    TPanel *pnlDCCharge;
    TPanel *pnlDCBattery;
    TPanel *pnlTemperature;
	TButton *Button2;
	TPopupMenu *PopupMenuVTHLogger;
	TMenuItem *N1;
    TImage *imgVthStatus;
    TImageList *ilVthStatus;
    TDataSource *DataSource;
    TDBGrid *DBGrid;
    TShape *shpLoggerProgress;
    TLabel *Label1;
    TLabel *Label2;
    TShape *shpSenderProgress;
    TLabel *lblLoggerTime;
    TLabel *lblSenderTime;
    TTimer *tmrDisplayLoggerSenderTime;
	TListView *ListView;
	TLabel *Label6;
	TShape *shpBG1;
	TShape *shpBG2;
	TTimer *SenderTimer;
	TTimer *LoggerTimer;
	TLabel *Label4;
	TLabel *Label3;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall CheckDateGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText);
    void __fastcall CheckFlagGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText);
    void __fastcall tmrDisplayLoggerSenderTimeTimer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall N1Click(TObject *Sender);
    void __fastcall DataSourceDataChange(TObject *Sender, TField *Field);
	void __fastcall LoggerTimerTimer(TObject *Sender);
	void __fastcall SenderTimerTimer(TObject *Sender);
private:	// User declarations

	TfbTable *RCheckReport;

	void Open();
	void ShowRecentDataToMemo();

	UnicodeString m_sPointCode;
	UnicodeString m_sPointName;

	UnicodeString m_sHydro1;
	UnicodeString m_sHydroPort1;
	UnicodeString m_sHydroHz1;
	UnicodeString m_sHydroSN1;

	UnicodeString m_sHydro2;
	UnicodeString m_sHydroPort2;
	UnicodeString m_sHydroHz2;
	UnicodeString m_sHydroSN2;

	UnicodeString m_sHydro3;
	UnicodeString m_sHydroPort3;
	UnicodeString m_sHydroHz3;
	UnicodeString m_sHydroSN3;

	UnicodeString m_sWaterLevel;
	UnicodeString m_sWaterPort;
	UnicodeString m_sWaterHz;
	UnicodeString m_sWaterSN;
	UnicodeString m_sAtmosPort;

	UnicodeString m_sVTHPort;

	UnicodeString m_sInterval;
	UnicodeString m_sPeriod;
	UnicodeString m_sStart;

	TDateTime m_LoggerTime;
	TDateTime m_SenderTime;

	EN_LOGGER_STATUS m_enLoggerStatus;
	EN_SENDER_STATUS m_enSenderStatus;

public:		// User declarations

	void SetPointInfo(UnicodeString sPointCode, UnicodeString sPointName);
	void SetHydroMeter(int iNo, UnicodeString sHydro, UnicodeString sPort, UnicodeString sHz, UnicodeString sSN);
	void SetWaterLevel(UnicodeString sWaterLevel, UnicodeString sPort, UnicodeString sWaterHz, UnicodeString sWaterSN, UnicodeString sAtmosPort);
	void SetVTHInfo(UnicodeString sPort);
	void SetWorkingInfo(UnicodeString sInterval, UnicodeString sPeriod, UnicodeString sStart);
	void ShowSetupInfo();

	void LoggerIdle();
	void LoggerStart();
	void LoggerLogging();
	void LoggerFinish();

	void SenderIdle();
	void SenderStart();
	void SenderWorking();
	void SenderFinish();
	void SenderRnseaStart();
	void SenderRnseaWorking();
	void SenderRnseaFinish();

	//int iLogOperation;
	//int iSenderTick;

	__fastcall TFormLeft(TComponent* Owner);
};


int GetElapseTime(TDateTime Time);


//---------------------------------------------------------------------------
extern PACKAGE TFormLeft *FormLeft;
//---------------------------------------------------------------------------
#endif
