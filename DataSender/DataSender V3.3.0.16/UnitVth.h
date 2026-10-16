//---------------------------------------------------------------------------

#ifndef UnitVthH
#define UnitVthH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//#include "AdPort.hpp"
//#include "Thrdqueue.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
class TfrmVth : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TButton *btnPowerOff;
    TButton *btnClose;
    TRadioButton *rbtnOn;
    TRadioButton *rbtnOff;
    TShape *Shape1;
	TLabel *lblVthStatus1;
	TLabel *lblVthStatus2;
    TTimer *tmrDisplayInfo;
    TTimer *tmrDelay;
	TLabel *Label2;
	TTimer *tmrRequest;
	TLabel *Label1;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnPowerOffClick(TObject *Sender);
    void __fastcall tmrDisplayInfoTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmrDelayTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tmrRequestTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    TfrmMain *MainForm;
	__fastcall TfrmVth(TComponent* Owner);

	void Receive(AnsiString sMsg);
};
//---------------------------------------------------------------------------
void ShowVth(TfrmMain *MainForm);
//---------------------------------------------------------------------------
extern PACKAGE TfrmVth *frmVth;
//---------------------------------------------------------------------------
#endif
