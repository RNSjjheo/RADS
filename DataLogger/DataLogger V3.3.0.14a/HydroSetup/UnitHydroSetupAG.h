//---------------------------------------------------------------------------

#ifndef UnitHydroSetupAGH
#define UnitHydroSetupAGH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "UnitHydroSetup.h"

//---------------------------------------------------------------------------
class THydroSetupAG : public THydroSetup
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TMemo *Memo;
	TEdit *edCommand;
	TButton *cbSend;
	TButton *cbBreak;
	TStatusBar *StatusBar;
	TButton *cbClose;
	TTimer *Timer;
	TTimer *TimerClose;
	TMemo *MemoCmd;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *edBlankDistance;
	TEdit *edNCells;
	TEdit *edCellSize;
	TEdit *edCellCount;
	TEdit *edMeasureTime;
	TButton *cbBlankDistance;
	TButton *cbNCells;
	TButton *cbCellSize;
	TButton *cbCellCount;
	TButton *cbMeasureTime;
	TButton *edSave;
	TButton *edShow;
	TButton *edShowConf;
	TButton *edShowSystem;
	TButton *edShowSetup;
	TButton *edShowDeploy;
	TButton *edSetTime;
	TButton *edTime;
	TBevel *Bevel1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cbSendClick(TObject *Sender);
	void __fastcall cbSendBreakClick(TObject *Sender);
	//void __fastcall cbSetTimeClick(TObject *Sender);
	void __fastcall cbSaveSetupClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall TimerCloseTimer(TObject *Sender);
	void __fastcall cbCloseClick(TObject *Sender);
	void __fastcall edCommandKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall edSaveClick(TObject *Sender);
	void __fastcall cbBreakClick(TObject *Sender);
	void __fastcall edShowClick(TObject *Sender);
	void __fastcall cbBlankDistanceClick(TObject *Sender);
	void __fastcall cbNCellsClick(TObject *Sender);
	void __fastcall cbCellSizeClick(TObject *Sender);
	void __fastcall cbCellCountClick(TObject *Sender);
	void __fastcall cbMeasureTimeClick(TObject *Sender);
	void __fastcall edSetTimeClick(TObject *Sender);
private:	// User declarations
	int    m_iIdleSec;		// Idle Second => 5 Minute => Sensor fall Sleep
	bool   m_bSleep;
	bool   m_bSetup;			// Setup Command or not
	
	double m_CellSize;
	double m_BlankDistance;
	double m_Ncells;

	TFormAlert *m_pFormAlert; 	// Alert Message Box

public:		// User declarations
//Virtual Fuction
	void Receive(AnsiString sRecvMsg);

//Local Function
	void LogStatus(UnicodeString sLogMsg);
	void SetIdleZero();
	void Process(AnsiString sMsg);
	void ShowRX(AnsiString sRX);

	void SensorStartNClose(bool bBreak=true);

	__fastcall THydroSetupAG(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THydroSetupAG *HydroSetupAG;
//---------------------------------------------------------------------------
#endif
