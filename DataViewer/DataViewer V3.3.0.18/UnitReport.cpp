//---------------------------------------------------------------------------

#include <vcl.h>
#include <IdAttachmentFile.hpp>
#pragma hdrstop

#include "UnitReport.h"
#include "UnitMain.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReport *FormReport = NULL;
//---------------------------------------------------------------------------
__fastcall TFormReport::TFormReport(TComponent* Owner)
    : TForm(Owner)
{
    SimpleDataSet1->Connection = FormMain->SQLConnection;
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnCloseClick(TObject *Sender)
//종료
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::FormShow(TObject *Sender)
{
    edUserID->Text = FormMain->DBEnv.sLoggedUserID;
    dtpDate1->Date = Now() - 60;
    dtpDate2->Date = Now();
    ledPointName->Text = FormMain->DBEnv.sPointName; //지점명
    GetComboKind(); //기존에 입력된 회사 종류를 db에서 읽어옴
    Open();
    ShowToControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnClearClick(TObject *Sender)
//clear
{
    EnableKeyControls(True);
    ClearControls();
    rbtnCheckFlag->ItemIndex = 0;
    meCheckDate->Text = FormatDateTime("yyyymmdd", Now());
    edUserID->Text = FormMain->DBEnv.sLoggedUserID;
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnPostClick(TObject *Sender)
//저장
{
    UnicodeString sResult = "";
    //점검일자
    if (meCheckDate->EditText == "    -  -  ")
    {
        ShowMessage("점검일자를 입력하세요.");
        meCheckDate->SetFocus();
        return;
    } // if
    //점검자 이름
    if (edName->Text == "")
    {
        ShowMessage("점검자 이름을 입력하세요.");
        edName->SetFocus();
        return;
    } // if
    //소속회사
    if (cbxCompany->Text == "")
    {
        ShowMessage("소속회사를 선택하거나 입력하세요.");
        cbxCompany->SetFocus();
        return;
    } // if
    //설치방식
    if (cbxDataFormat->Text == "")
    {
        ShowMessage("설치방식을 입력하세요.");
        cbxDataFormat->SetFocus();
        return;
    } // if
    if (rbtnCheckFlag->Enabled) //추가일 때는 key check
    {
        UnicodeString sCheckFlag = (rbtnCheckFlag->ItemIndex == 0) ? "R" :
                                   (rbtnCheckFlag->ItemIndex == 1) ? "O" :
                                   (rbtnCheckFlag->ItemIndex == 2) ? "W" :
                                   (rbtnCheckFlag->ItemIndex == 3) ? "E" :
                                   "";
        if (SQLExists(FormMain->SQLConnection,
                      "SELECT * FROM RCHECKREPORT WHERE CHECKDATE = '" + meCheckDate->Text + "' AND CHECKFLAG = '" + sCheckFlag + "' AND NAME = '" + edName->Text + "' "))
        {
            ShowMessage("이미 존재하는 자료입니다. 점검구분, 점검일자, 점검자등을 바꾸세요!");
            rbtnCheckFlag->SetFocus();
            return;
        } // if
    } // if
    try
    {
        bRealPost = True;
        if (rbtnCheckFlag->Enabled) SimpleDataSet1->Insert();
        else                        SimpleDataSet1->Edit();

        //컨트롤의 값을 db 필드에 대입
        SimpleDataSet1->FieldByName("CHECKFLAG")->AsString = (rbtnCheckFlag->ItemIndex == 0) ? "R" :
                                                             (rbtnCheckFlag->ItemIndex == 1) ? "O" :
                                                             (rbtnCheckFlag->ItemIndex == 2) ? "W" :
                                                             (rbtnCheckFlag->ItemIndex == 3) ? "E" :
                                                             "";
        SimpleDataSet1->FieldByName("DATAFORMAT")->AsString =  cbxDataFormat->Text;
        SimpleDataSet1->FieldByName("CHECKDATE")->AsString  = meCheckDate->Text;
        SimpleDataSet1->FieldByName("NAME")->AsString       = edName->Text;
        SimpleDataSet1->FieldByName("COMPANY")->AsString    = cbxCompany->Text;
        SimpleDataSet1->FieldByName("PORPOSE")->AsString    = mmPorpose->Text;
        SimpleDataSet1->FieldByName("MANAGE")->AsString     = mmManage->Text;
        SimpleDataSet1->FieldByName("COMPLEMENT")->AsString = mmComplement->Text;
        SimpleDataSet1->FieldByName("REPLACE")->AsString    = mmReplace->Text;
        SimpleDataSet1->FieldByName("ETC")->AsString        = mmEtc->Text;

        SimpleDataSet1->Post();
        SimpleDataSet1->ApplyUpdates(-1);
        GetComboKind(); //기존에 입력된 회사 종류를 db에서 읽어옴
        if (FormMain->DBEnv.bEmailAutoSend) SendSmtpMail();
        ShowMessage("저장되었습니다!");
    } catch(Exception &e)
    {
        ShowMessage("저장중 에러가 발생했습니다.\n에러 내용: " + e.Message);
    } // catch
    bRealPost = False;
}
//---------------------------------------------------------------------------
void TFormReport::EnableKeyControls(bool bEnable)
{
    rbtnCheckFlag->Enabled = bEnable;
    meCheckDate->Enabled = bEnable;
    meCheckDate->Color = (bEnable) ? clWindow : clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::DBGridCellClick(TColumn *Column)
{
    EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
    ShowToControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnDeleteClick(TObject *Sender)
//삭제
{
    if (edName->Text == "")
    {
        ShowMessage("입력모드입니다. 하단 그리드에서 삭제할 자료를 클릭하고 다시 삭제하세요.");
        return;
    } // if
    if (SQLExists(FormMain->SQLConnection,
                  "SELECT * FROM RCHECKREPORT WHERE CHECKDATE = '" + SimpleDataSet1->FieldByName("CHECKDATE")->AsString + "' AND CHECKFLAG = '" + SimpleDataSet1->FieldByName("CHECKFLAG")->AsString + "' AND NAME = '" + SimpleDataSet1->FieldByName("NAME")->AsString + "' "))
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
void TFormReport::GetComboKind()
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

    sql->Close();
    sql->CommandText = "SELECT DISTINCT COMPANY "
                       "FROM   RCHECKREPORT "
                       "WHERE  COMPANY <> '' ";
    sql->Open();
    while(!sql->Eof)
    {
        int idx = cbxCompany->Items->IndexOf(sql->FieldByName("COMPANY")->AsString);
        if (idx == -1) cbxCompany->Items->Add(sql->FieldByName("COMPANY")->AsString);
        sql->Next();
    } // while

    delete sql;
}
//---------------------------------------------------------------------------
void TFormReport::Open()
//쿼리를 오픈하고 화면에 표시
{
    UnicodeString sDate1 = FormatDateTime("yyyymmdd", dtpDate1->Date);
    UnicodeString sDate2 = FormatDateTime("yyyymmdd", dtpDate2->Date);

    SimpleDataSet1->DataSet->CommandText = "";
    SimpleDataSet1->DataSet->CommandText = SimpleDataSet1->DataSet->CommandText +
                                           "SELECT * " +
                                           "FROM RCHECKREPORT " +
                                           "WHERE CHECKDATE >= '" + sDate1 + "' " +
                                           "AND   CHECKDATE <= '" + sDate2 + "' " +
                                           "ORDER BY CHECKDATE DESC ";
    if (SimpleDataSet1->Active) SimpleDataSet1->Close();
    SimpleDataSet1->Open();
    //simpledataset의 필드 조건 세팅
    SimpleDataSet1->FieldByName("CHECKDATE")->DisplayLabel = "날짜";
    SimpleDataSet1->FieldByName("CHECKDATE")->DisplayWidth = 10;
    SimpleDataSet1->FieldByName("CHECKDATE")->EditMask = "####-##-##;0";
    SimpleDataSet1->FieldByName("CHECKFLAG")->DisplayLabel = "점검구분";
    SimpleDataSet1->FieldByName("CHECKFLAG")->DisplayWidth = 8;
    SimpleDataSet1->FieldByName("CHECKFLAG")->OnGetText = CheckFlagGetText;
    SimpleDataSet1->FieldByName("NAME")->DisplayLabel = "점검자";
    SimpleDataSet1->FieldByName("NAME")->DisplayWidth = 10;

    SimpleDataSet1->FieldByName("COMPANY")->Visible = False;
    SimpleDataSet1->FieldByName("DATAFORMAT")->Visible = False;
    SimpleDataSet1->FieldByName("PORPOSE")->Visible = False;
    SimpleDataSet1->FieldByName("MANAGE")->Visible = False;
    SimpleDataSet1->FieldByName("COMPLEMENT")->Visible = False;
    SimpleDataSet1->FieldByName("REPLACE")->Visible = False;
    SimpleDataSet1->FieldByName("ETC")->Visible = False;

    SimpleDataSet1->FieldByName("MAILFLAG")->DisplayLabel = "메일전송";
    SimpleDataSet1->FieldByName("MAILFLAG")->DisplayWidth = 8;
    SimpleDataSet1->FieldByName("MAILDATETIME")->DisplayLabel = "전송일시";
    SimpleDataSet1->FieldByName("MAILDATETIME")->DisplayWidth = 24;

    EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnOpenQueryClick(TObject *Sender)
//조회
{
    Open();
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::CheckFlagGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText)
//점검구분
{
    if (Sender->AsString == "") return;
    switch(Sender->AsString[1])
    {
        case 'R':
            Text = "정기점검";
            break;
        case 'O':
            Text = "수시점검";
            break;
        case 'W':
            Text = "수중점검";
            break;
        case 'E':
            Text = "기타";
            break;
    } // switch
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::btnSendMailClick(TObject *Sender)
//메일전송
{
    UnicodeString sResult = SendSmtpMail();
    if (sResult == "") ShowMessage("메일이 성공적으로 전송되었습니다!");
    else               ShowMessage("메일 전송시 에러가 발생했습니다.\n" + sResult);
}
//---------------------------------------------------------------------------
UnicodeString TFormReport::SendSmtpMail()
{
    UnicodeString sResult = "";

    //smtp 서버에 접속
    IdSMTP->Host = FormMain->DBEnv.sEmailServerAddress;
    IdSMTP->Port = 587;
    IdSMTP->Username = FormMain->DBEnv.sEmailAccount;
    IdSMTP->Password = FormMain->DBEnv.sEmailPassword;
    try
    {
        IdSMTP->CheckForGracefulDisconnect(False);
        if (!IdSMTP->Connected()) IdSMTP->Connect();
    } catch(Exception &e)
    {
        sResult = "메일서버에 연결할 수 없습니다.\n" + e.Message;
        return sResult;
    } // catch

    //제목설정
    UnicodeString sSubject = "점검일지(";
    UnicodeString sCheckKind;
    UnicodeString sCheckFlag;
    switch(rbtnCheckFlag->ItemIndex)
    {
        case 0:
            sCheckKind = "정기점검";
            sCheckFlag = "R";
            break;
        case 1:
            sCheckKind = "수시점검";
            sCheckFlag = "O";
            break;
        case 2:
            sCheckKind = "수중점검";
            sCheckFlag = "W";
            break;
        case 3:
            sCheckKind = "기타";
            sCheckFlag = "E";
            break;
    } //switch
    sSubject += sCheckKind;
    sSubject += ")-" + meCheckDate->Text.SubString(1, 4) + "." + meCheckDate->Text.SubString(5, 2) + "." + meCheckDate->Text.SubString(7, 2);
    //본문작성
    UnicodeString sBodyText = "";
    sBodyText += "점검일자: " + meCheckDate->Text + "\n";
    sBodyText += "점검구분: " + sCheckKind + "\n";
    sBodyText += "점 검 자: " + edName->Text + "\n";
    sBodyText += "소    속: " + cbxCompany->Text + "\n";
    sBodyText += "설치방식: " + cbxDataFormat->Text + "\n";
    sBodyText += "점검목적: " + mmPorpose->Text + "\n";
    sBodyText += "문제점 및 조치사항: " + mmManage->Text + "\n";
    sBodyText += "미처리사항 및 보완계획: " + mmComplement->Text + "\n";
    sBodyText += "교체물품: " + mmReplace->Text + "\n";
    sBodyText += "기타사항: " + mmEtc->Text + "\n";
    //메일 생성
    TIdMessage *IdMessage = new TIdMessage;
    IdMessage->From->Address = FormMain->DBEnv.sEmailDest1.Trim();
    IdMessage->ReplyTo->Add()->Text = FormMain->DBEnv.sEmailDest1.Trim();
    if (FormMain->DBEnv.sEmailDest1.Trim() != "") IdMessage->Recipients->Add()->Address = FormMain->DBEnv.sEmailDest1.Trim();
    if (FormMain->DBEnv.sEmailDest2.Trim() != "") IdMessage->Recipients->Add()->Address = FormMain->DBEnv.sEmailDest2.Trim();
    if (FormMain->DBEnv.sEmailDest3.Trim() != "") IdMessage->Recipients->Add()->Address = FormMain->DBEnv.sEmailDest3.Trim();
    if (FormMain->DBEnv.sEmailDest4.Trim() != "") IdMessage->Recipients->Add()->Address = FormMain->DBEnv.sEmailDest4.Trim();
    if (FormMain->DBEnv.sEmailDest5.Trim() != "") IdMessage->Recipients->Add()->Address = FormMain->DBEnv.sEmailDest5.Trim();
    IdMessage->ContentTransferEncoding = "base64";
    IdMessage->Subject = sSubject;
    IdMessage->CharSet = "utf-8";
    IdMessage->Body->Text = sBodyText;
    //첨부파일 생성
    UnicodeString sSaveFile = PathStrAdd(FormMain->DBEnv.sPubViewerHome, "data") + "\\CheckReport_" + meCheckDate->Text + "_" + sCheckFlag + ".xml";
    ForceDirectories(ExtractFilePath(sSaveFile));
    TStringList *sl = new TStringList;
    AnsiString sAttachBody = (AnsiString)"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
                                         "<CheckReport>\n" +
                                         "<CheckDate>" + meCheckDate->Text.Trim() + "</CheckDate>\n" +
                                         "<CheckFlag>" + sCheckFlag + "</CheckFlag>\n" +
                                         "<Name>" + edName->Text.Trim() + "</Name>\n" +
                                         "<Company>" + cbxCompany->Text.Trim() + "</Company>\n" +
                                         "<DataFormat>" + cbxDataFormat->Text.Trim() + "</DataFormat>\n" +
                                         "<Porpose>" + mmPorpose->Text.Trim() + "</Porpose>\n" +
                                         "<Manage>" + mmManage->Text.Trim() + "</Manage>\n" +
                                         "<Complement>" + mmComplement->Text.Trim() + "</Complement>\n" +
                                         "<Replace>" + mmReplace->Text.Trim() + "</Replace>\n" +
                                         "<Etc>" + mmEtc->Text.Trim() + "</Etc>\n" +
                                         "</CheckReport>";
    sl->Text = sAttachBody;
    if (FileExists(sSaveFile)) DeleteFile(sSaveFile);
    sl->SaveToFile(sSaveFile, TEncoding::UTF8);
    delete sl;
    //첨부파일 추가
    TIdAttachment *pAttachment = new TIdAttachmentFile(IdMessage->MessageParts, sSaveFile);

    //전송
    try
    {
        Screen->Cursor = crHourGlass;
        IdSMTP->Send(IdMessage);
        IdSMTP->Disconnect();
    } catch(Exception &e)
    {
        Screen->Cursor = crDefault;
        sResult = "메일 전송중 에러가 발생했습니다.\n" + e.Message;
    } //catch

    //db update
    if (sResult == "")
    {
        SimpleDataSet1->Edit();
        SimpleDataSet1->FieldByName("MAILFLAG")->AsString = "Y";
        SimpleDataSet1->FieldByName("MAILDATETIME")->AsString = FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());
        bRealPost = True;
        SimpleDataSet1->Post();
        SimpleDataSet1->ApplyUpdates(-1);
        bRealPost = False;
    } // if

    Screen->Cursor = crDefault;
    delete pAttachment;
    delete IdMessage;

    return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::SimpleDataSet1BeforePost(TDataSet *DataSet)
{
    if (!bRealPost)
    {
        SimpleDataSet1->Cancel();
        EnableKeyControls(DBGrid->DataSource->DataSet->Eof && DBGrid->DataSource->DataSet->Bof);
        Abort();
    } // if
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::DataSourceDataChange(TObject *Sender, TField *Field)
{
    if (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "R") rbtnCheckFlag->ItemIndex = 0;
    if (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "O") rbtnCheckFlag->ItemIndex = 1;
    if (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "W") rbtnCheckFlag->ItemIndex = 2;
    if (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "E") rbtnCheckFlag->ItemIndex = 3;
}
//---------------------------------------------------------------------------
void TFormReport::ShowToControls()
//db의 값을 관련 컨트롤에 표시
{
    rbtnCheckFlag->ItemIndex = (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "R") ? 0 :
                               (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "O") ? 1 :
                               (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "W") ? 2 :
                               (SimpleDataSet1->FieldByName("CHECKFLAG")->AsString == "E") ? 3 :
                               -1;
    cbxDataFormat->ItemIndex = cbxDataFormat->Items->IndexOf(SimpleDataSet1->FieldByName("DATAFORMAT")->AsString);
    meCheckDate->Text        = SimpleDataSet1->FieldByName("CHECKDATE")->AsString;
    edName->Text             = SimpleDataSet1->FieldByName("NAME")->AsString;
    cbxCompany->Text         = SimpleDataSet1->FieldByName("COMPANY")->AsString;
    mmPorpose->Text          = SimpleDataSet1->FieldByName("PORPOSE")->AsString;
    mmManage->Text           = SimpleDataSet1->FieldByName("MANAGE")->AsString;
    mmComplement->Text       = SimpleDataSet1->FieldByName("COMPLEMENT")->AsString;
    mmReplace->Text          = SimpleDataSet1->FieldByName("REPLACE")->AsString;
    mmEtc->Text              = SimpleDataSet1->FieldByName("ETC")->AsString;
}
//---------------------------------------------------------------------------
void TFormReport::ClearControls()
{
    rbtnCheckFlag->ItemIndex = 0;
    cbxDataFormat->ItemIndex = -1;
    meCheckDate->Text        = "";
    edName->Text             = "";
    cbxCompany->Text         = "";
    rbtnCheckFlag->ItemIndex = 0;
    mmPorpose->Text          = "";
    mmManage->Text           = "";
    mmComplement->Text       = "";
    mmReplace->Text          = "";
    mmEtc->Text              = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormReport::SimpleDataSet1AfterScroll(TDataSet *DataSet)
{
    if (SimpleDataSet1->State == dsBrowse) ShowToControls();
}
//---------------------------------------------------------------------------

