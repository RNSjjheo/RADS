//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitAbout.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAbout *FormAbout = NULL;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::Label3Click(TObject *Sender)
{
    ShellExecute(Handle, TEXT("open"), TEXT("iexplore.exe"), TEXT("http://www.rnsea.com"), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::FormShow(TObject *Sender)
{
    //버전
    lblVersion->Caption = "RADS Viewer V" + GetAppVersion();
    //파일 수정일자
    lblBuild->Caption = "(Build " + GetAppModifiedDateStr() + ")";
}
//---------------------------------------------------------------------------
