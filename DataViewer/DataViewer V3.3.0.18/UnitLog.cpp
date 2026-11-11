//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLog.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLog *FormLog = NULL;
//---------------------------------------------------------------------------
__fastcall TFormLog::TFormLog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLog::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormLog = NULL;
    FormMain->actShowLog->Checked = False;
    Action = caFree;
}
//---------------------------------------------------------------------------
