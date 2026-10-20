//---------------------------------------------------------------------------

#ifndef UnitHydroSetupCMH
#define UnitHydroSetupCMH
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
class THydroSetupCM : public THydroSetup
{
__published:	// IDE-managed Components
	TMemo *Memo;
	TEdit *edCommand;
	TLabel *Label1;
	TButton *cbSend;
	TButton *cbBreak;
	TStatusBar *StatusBar;
	TTimer *Timer;
	TTimer *TimerClose;
	TButton *cbClose;
	TMemo *MemoCmd;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *edWF;
	TEdit *edWN;
	TEdit *edWS;
	TEdit *edWP;
	TEdit *edTP;
	TButton *cbWF;
	TButton *cbWN;
	TButton *cbWS;
	TButton *cbWP;
	TButton *cbTP;
	TButton *edSave;
	TButton *edC;
	TButton *edE;
	TButton *edI;
	TButton *edM;
	TButton *edP;
	TButton *edS;
	TButton *edT;
	TButton *edV;
	TButton *edY;
	TButton *edW;
	TButton *edHelp;
	TBevel *Bevel1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cbSendClick(TObject *Sender);
	void __fastcall cbBreakClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall TimerCloseTimer(TObject *Sender);
	void __fastcall cbCloseClick(TObject *Sender);
	void __fastcall edCommandKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall cbWFClick(TObject *Sender);
	void __fastcall cbWNClick(TObject *Sender);
	void __fastcall cbWSClick(TObject *Sender);
	void __fastcall cbWPClick(TObject *Sender);
	void __fastcall cbTPClick(TObject *Sender);
	void __fastcall edSaveClick(TObject *Sender);
	void __fastcall edCClick(TObject *Sender);
private:	// User declarations

	int  m_iIdleSec;		// Idle Second => 5 Minute => Sensor fall Sleep
	bool m_bSleep;			// Sensor Sleep or Not
	bool m_bSetup;			// Setup Command or not

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

	__fastcall THydroSetupCM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THydroSetupCM *HydroSetupCM;
//---------------------------------------------------------------------------
#endif
