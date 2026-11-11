//---------------------------------------------------------------------------

#ifndef UnitLogH
#define UnitLogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormLog : public TForm
{
__published:	// IDE-managed Components
    TMemo *mmLog;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFormLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLog *FormLog;
//---------------------------------------------------------------------------
#endif
