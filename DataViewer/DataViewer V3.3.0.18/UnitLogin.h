//---------------------------------------------------------------------------

#ifndef UnitLoginH
#define UnitLoginH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLogin : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edID;
    TEdit *edPassword;
    TButton *btnOk;
    TButton *btnCancel;
    TBevel *Bevel1;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
UnicodeString ShowLogin(UnicodeString sLastLoginID = "");
//---------------------------------------------------------------------------
extern PACKAGE TFormLogin *FormLogin;
//---------------------------------------------------------------------------
#endif
