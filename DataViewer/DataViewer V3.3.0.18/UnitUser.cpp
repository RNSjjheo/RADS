//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitUser.h"
#include "UnitMain.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUser *FormUser = NULL;
//---------------------------------------------------------------------------
__fastcall TFormUser::TFormUser(TComponent* Owner)
    : TForm(Owner)
{
    bRealPost = False;
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::btnCloseClick(TObject *Sender)
//종료
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::FormShow(TObject *Sender)
{
    GetComboKind(); //기존에 입력된 회사 종류를 db에서 읽어옴
    Open();
    ShowToControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::SimpleDataSet1AfterScroll(TDataSet *DataSet)
{
    if (SimpleDataSet1->State == dsBrowse)
    {
        ShowToControls();
        edPw->Text = "";
    } // if
}
//---------------------------------------------------------------------------
void TFormUser::Open()
//쿼리를 오픈하고 화면에 표시
{
    SimpleDataSet1->DataSet->CommandText = "SELECT * FROM RUSER WHERE Privilege IN ( 'N', 'A' ) ORDER BY 1";
    SimpleDataSet1->Open();
    //simpledataset의 필드 조건 세팅
    SimpleDataSet1->FieldByName("USERID")->DisplayLabel = "사용자ID";
    SimpleDataSet1->FieldByName("USERID")->DisplayWidth = 8;
    SimpleDataSet1->FieldByName("USERNAME")->DisplayLabel = "성명";
    SimpleDataSet1->FieldByName("USERNAME")->DisplayWidth = 10;
    SimpleDataSet1->FieldByName("PASSWORD")->Visible = False;
    SimpleDataSet1->FieldByName("COMPANY")->DisplayLabel = "회사";
    SimpleDataSet1->FieldByName("COMPANY")->DisplayWidth = 14;
    SimpleDataSet1->FieldByName("PHONE")->DisplayLabel = "연락처";
    SimpleDataSet1->FieldByName("PHONE")->DisplayWidth = 13;
    SimpleDataSet1->FieldByName("PRIVILEGE")->DisplayLabel = "권한";
    SimpleDataSet1->FieldByName("PRIVILEGE")->DisplayWidth = 4;
    SimpleDataSet1->FieldByName("POS")->DisplayLabel = "직책";
    SimpleDataSet1->FieldByName("POS")->DisplayWidth = 8;

    EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::btnClearClick(TObject *Sender)
//clear -> insert mode
{
    EnableKeyControls(True);
    ClearControls();
    edPw->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::btnPostClick(TObject *Sender)
//저장
{
    UnicodeString sResult = "";
    //id check
    if (edUserId->Text.Trim() == "")
    {
        ShowMessage("ID를 입력하세요.");
        edUserId->SetFocus();
        return;
    } // if
    //비밀번호 check
    if (edPassword->Text == "")
    {
        ShowMessage("비밀번호를 입력하세요.");
        edPassword->SetFocus();
        return;
    } // if
    if (edPassword->Text.Length() < 6)
    {
        ShowMessage("비밀번호는 6자리 이상 입력하세요.");
        edPassword->SetFocus();
        return;
    } // if
    if (edPw->Text == "")
    {
        ShowMessage("비밀번호 확인란을 입력하세요.");
        edPw->SetFocus();
        return;
    } // if
    if (edPassword->Text != edPw->Text)
    {
        ShowMessage("비밀번호를 틀립니다. 확인하고 정확한 비밀번호를 입력하세요.");
        edPw->SetFocus();
        return;
    } // if

    if (edUserId->Enabled) //추가일 때는 key check
    {
        if (SQLExists(FormMain->SQLConnection,
                      "SELECT * FROM RUSER WHERE  USERID = '" + edUserId->Text + "' "))
        {
            ShowMessage("이미 존재하는 ID입니다.\n새로운 ID를 입력하세요!");
            edUserId->SetFocus();
            return;
        } // if
    } // if
    try
    {
        bRealPost = True;
        if (edUserId->Enabled) SimpleDataSet1->Insert();
        else                   SimpleDataSet1->Edit();

        //컨트롤의 값을 db 필드에 대입
        SimpleDataSet1->FieldByName("USERID")->AsString    = edUserId->Text;
        SimpleDataSet1->FieldByName("USERNAME")->AsString  = edUserName->Text;
        SimpleDataSet1->FieldByName("PASSWORD")->AsString  = edPassword->Text;
		SimpleDataSet1->FieldByName("PRIVILEGE")->AsString = (rbtnPrivilege->ItemIndex == 0) ? "N" : (rbtnPrivilege->ItemIndex ==  1) ? "A" : "";
        SimpleDataSet1->FieldByName("POS")->AsString       = edPos->Text;
        SimpleDataSet1->FieldByName("COMPANY")->AsString   = cbxCompany->Text;
        SimpleDataSet1->FieldByName("PHONE")->AsString     = edPhone->Text;

        SimpleDataSet1->Post();
        SimpleDataSet1->ApplyUpdates(-1);
        GetComboKind(); //기존에 입력된 회사 종류를 db에서 읽어옴
        ShowMessage("저장되었습니다!");
    } catch(Exception &e)
    {
        if (SimpleDataSet1->State == dsInsert ||
            SimpleDataSet1->State == dsEdit) SimpleDataSet1->Cancel();
        ShowMessage("저장중 에러가 발생했습니다.\n에러 내용: " + e.Message);
    } // catch
    bRealPost = False;
}
//---------------------------------------------------------------------------
void TFormUser::EnableKeyControls(bool bEnable)
{
    edUserId->Enabled = bEnable;
    edUserId->Color = (bEnable) ? clWindow : clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::DBGridCellClick(TColumn *Column)
{
	EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
	ShowToControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::btnDeleteClick(TObject *Sender)
//삭제
{
	if (edUserId->Text == "")
	{
		ShowMessage("입력모드입니다. 하단 그리드에서 삭제할 자료를 클릭하고 다시 삭제하세요.");
		return;
	} // if

	if ( edUserId->Text == "default" || edUserId->Text == "rnsea" || edUserId->Text == "hsc" ) {
		ShowMessage("기본 계정은 삭제할 수 없습니다.");
		return;
	}
    if (SQLExists(FormMain->SQLConnection,
                  "SELECT * FROM RUSER WHERE  USERID = '" + edUserId->Text + "' "))
    {
        if (Application->MessageBoxW(TEXT("삭제하시겠습니까?"), TEXT("확인"), MB_OKCANCEL) == IDOK)
        {
            try
            {
                SimpleDataSet1->Delete();
                SimpleDataSet1->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage("삭제중 에러가 발생했습니다.\n에러내용: " + e.Message);
            } // catch
        } // if
    } // if
}
//---------------------------------------------------------------------------
void TFormUser::GetComboKind()
//기존에 입력된 회사 종류를 db에서 읽어옴
{
    cbxCompany->Items->Clear();

    TSQLQuery* sql = CreateQuery(FormMain->SQLConnection);
    sql->CommandText = "SELECT DISTINCT COMPANY "
                       "FROM   RUSER "
                       "WHERE  COMPANY <> '' ";
    sql->Open();
    while(!sql->Eof)
    {
        cbxCompany->Items->Add(sql->FieldByName("COMPANY")->AsString);
        sql->Next();
    } // while

    delete sql;
}
//---------------------------------------------------------------------------
void __fastcall TFormUser::SimpleDataSet1BeforePost(TDataSet *DataSet)
{
    if (!bRealPost)
    {
        SimpleDataSet1->Cancel();
        EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
        Abort();
    } // if
}
//---------------------------------------------------------------------------
void TFormUser::ShowToControls()
//db의 값을 관련 컨트롤에 표시
{
    edUserId->Text           = SimpleDataSet1->FieldByName("USERID")->AsString;
    edPassword->Text         = SimpleDataSet1->FieldByName("PASSWORD")->AsString;
    cbxCompany->ItemIndex    = cbxCompany->Items->IndexOf(SimpleDataSet1->FieldByName("COMPANY")->AsString);
    edPos->Text              = SimpleDataSet1->FieldByName("POS")->AsString;
    edUserName->Text         = SimpleDataSet1->FieldByName("USERNAME")->AsString;
    edPhone->Text            = SimpleDataSet1->FieldByName("PHONE")->AsString;
    UnicodeString sPrivilege = SimpleDataSet1->FieldByName("PRIVILEGE")->AsString;
	rbtnPrivilege->ItemIndex = (sPrivilege == "N") ? 0 : (sPrivilege == "A") ? 1 : -1;
}
//---------------------------------------------------------------------------
void TFormUser::ClearControls()
{
    edUserId->Text        = "";
    edPassword->Text      = "";
    cbxCompany->ItemIndex = -1;
    edPos->Text           = "";
    edUserName->Text      = "";
    edPhone->Text         = "";
    rbtnPrivilege->ItemIndex = 0;
}
//---------------------------------------------------------------------------
