//---------------------------------------------------------------------------

#ifndef UnitAlertH
#define UnitAlertH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormAlert : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel;
	TTimer *Timer;
private:	// User declarations
public:		// User declarations
	void ShowMessage(UnicodeString sMsg);
	__fastcall TFormAlert(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAlert *FormAlert;
//---------------------------------------------------------------------------
#endif
