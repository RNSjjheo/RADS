//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLogin.h"
#include "UnitMain.h"
#include "fbTable.h"
#include "DvEnv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLogin *FormLogin = NULL;
//---------------------------------------------------------------------------
UnicodeString ShowLogin(UnicodeString sLastLoginID)
{
    UnicodeString sResult = "";

    FormLogin = new TFormLogin(Application->Owner);
//    if (sLastLoginID != "")
//    {
//        FormLogin->edID->Text = sLastLoginID;
//        FormLogin->edPassword->Text = "";
//    } // if
    if (FormLogin->ShowModal() == mrOk) sResult = FormLogin->edID->Text;

    delete FormLogin;
    FormLogin = NULL;

    return sResult;
}
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::btnOkClick(TObject *Sender)
{
    if (edID->Text == "")
    {
        ShowMessage("ID를 입력하세요.");
        edID->SetFocus();
        return;
    } // if
    if (edPassword->Text == "")
    {
        ShowMessage("Password를 입력하세요.");
        edPassword->SetFocus();
        return;
    } // if
    //db에서 validation check
    TfbTable *RUser = new TfbTable(FormMain->SQLConnection, "RUser");
    RUser->ClearParams();
    RUser->SetWhereParam("USERID", edID->Text);
    RUser->SetWhereParam("PASSWORD", edPassword->Text);
	bool bRightExists = RUser->Exists();
    delete RUser;

    if (!bRightExists)
    {
        ShowMessage("ID가 없거나 Password가 잘 못 되었습니다.\n다시 입력해주세요.");
        edPassword->SetFocus();
        return;
    } // if

    //권한얻기; 관리자 권한인지 검사
    RUser = new TfbTable(FormMain->SQLConnection, "RUser");
    RUser->ClearParams();
    RUser->SetWhereParam("USERID", edID->Text);
    RUser->SetWhereParam("PASSWORD", edPassword->Text);
	//RUser->SetWhereParam("PRIVILEGE", "S");
	RUser->SetWhereParam("PRIVILEGE", "'S','A'", wkInStr);
	FormMain->DBEnv.bSuperID = RUser->Exists();
	delete RUser;

    FormMain->DBEnv.sLoggedUserID = edID->Text;
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::btnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
