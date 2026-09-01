//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitConfig.h"
#include "UnitMain.h"
//#include "Env.h"
//#include "LockeCommon.h"
#include "FileCtrl.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormConfig *FormConfig = NULL;

//---------------------------------------------------------------------------
void ShowConfig(int iPageSelections, int iActivePageIndex)
{
    if (FormConfig != NULL) return;

    FormConfig = new TFormConfig(Application->Owner);
    //숨겨야할 페이지 설정
    if ((iPageSelections & ciPoint)    != ciPoint   ) FormConfig->pcConfig->Pages[0]->TabVisible = false;
    if ((iPageSelections & ciLogger)   != ciLogger  ) FormConfig->pcConfig->Pages[1]->TabVisible = false;
    if ((iPageSelections & ciSender)   != ciSender  ) FormConfig->pcConfig->Pages[2]->TabVisible = false;
    if ((iPageSelections & ciGuardnet) != ciGuardnet) FormConfig->pcConfig->Pages[3]->TabVisible = false;
    if ((iPageSelections & ciViewer)   != ciViewer  ) FormConfig->pcConfig->Pages[4]->TabVisible = false;
    if ((iPageSelections & ciRange)    != ciRange   ) FormConfig->pcConfig->Pages[5]->TabVisible = false;
    if ((iPageSelections & ciEtc)      != ciEtc     ) FormConfig->pcConfig->Pages[6]->TabVisible = false;
    //기본으로 표시할 페이지
    switch(iActivePageIndex)
    {
        case ciPoint:
            FormConfig->pcConfig->ActivePageIndex = 0;
            break;
        case ciLogger:
            FormConfig->pcConfig->ActivePageIndex = 1;
            break;
        case ciSender:
            FormConfig->pcConfig->ActivePageIndex = 2;
            break;
        case ciGuardnet:
            FormConfig->pcConfig->ActivePageIndex = 3;
            break;
        case ciViewer:
            FormConfig->pcConfig->ActivePageIndex = 4;
            break;
        case ciRange:
            FormConfig->pcConfig->ActivePageIndex = 5;
            break;
        case ciEtc:
            FormConfig->pcConfig->ActivePageIndex = 6;
            break;
    } // switch

    FormConfig->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::FormActivate(TObject *Sender)
{
//	m_iPingCnt = 0;
//
//	SimpleDataSet->Connection = frmMain->SQLConnection;
//	pcConfigChange(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFormConfig::TFormConfig(TComponent* Owner)
    : TForm(Owner)
{
    bSaved = False;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::btnCloseClick(TObject *Sender)
{
    if (bSaved)
    {
        ShowMessage("변경 내역이 반영되려면 프로그램을 종료 후 재시작하여 주십시오.");
        frmMain->LogMessage(!CbFileWrite, CbDBWrite, "환경설정이 변경되었습니다.");
        frmMain->DoReset();
    } // if
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::FloatEditChange(TObject *Sender)
//컨트롤의 값이 정수 혹은 실수만 되도록
{
    float fValue;
    if (((TEdit *)Sender)->Text == "") return;
    try
    {
        fValue = ((TEdit *)Sender)->Text.ToDouble();
    } catch(...)
    {
        ((TEdit *)Sender)->Text = "";
        ((TEdit *)Sender)->SelStart = 0;
        ((TEdit *)Sender)->SelLength = 0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::cbx2H1KindChange(TObject *Sender)
{
    ed2H1WN->Enabled = cbx2H1Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H1WN->Enabled) ed2H1WN->Text = "";
    ed2H1WS->Enabled = cbx2H1Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H1WS->Enabled) ed2H1WS->Text = "";
    ed2H1WF->Enabled = cbx2H1Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H1WF->Enabled) ed2H1WF->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::cbx2H2KindChange(TObject *Sender)
{
    ed2H2WN->Enabled = cbx2H2Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H2WN->Enabled) ed2H2WN->Text = "";
    ed2H2WS->Enabled = cbx2H2Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H2WS->Enabled) ed2H2WS->Text = "";
    ed2H2WF->Enabled = cbx2H2Kind->ItemIndex == 2; //ag series 일 경우만 활성화
    if (!ed2H2WF->Enabled) ed2H2WF->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::OnSelectIF(TObject *Sender)
//routing table interface(IF) index를 보여주고 IF 값을 선택하게 한다
/*
    리턴데이터 형식
    <item1>|<item2>|...

    item 형식
    <IF>:<g/w>:<장치명>

    예)
    11:192.168.0.1:Qualcomm Lan card|14:192.168.34.1:Wireless lancard
*/
{
    UnicodeString sError = "";
    UnicodeString sResult = GetIfInfo(sError);
    int iCnt = GetSepItemCount(sResult, "|");
    if (iCnt == 0)
    {
        ShowMessage("라우팅 테이블에 공유기 정보가 없습니다.");
        return;
    } // if
    lbxIF->Items->Clear();
    for(int i = 0; i < iCnt; i++)
    {
        UnicodeString sItem = GetSepItem(sResult, i, "|");
        UnicodeString sIFIndex = GetSepItem(sItem, 0, ":");
        UnicodeString sDesc = GetSepItem(sItem, 1, ":");
		lbxIF->Items->Add(sIFIndex + " " + sDesc);
    } // for i
    pnlIF->Left = 190;
    pnlIF->Top = 280;
    pnlIF->Visible = true;

    lbxIF->Tag = ((TSpeedButton *)Sender)->Tag;
    lbxIF->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::lbxIFExit(TObject *Sender)
{
	pnlIF->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::lbxIFClick(TObject *Sender)
{
	if (lbxIF->ItemIndex >= 0)
	{
		UnicodeString Str  = "";
		UnicodeString Desc = "";
		int Pos = 0;
		switch(lbxIF->Tag)
		{
			case 1:
				{
					Str = lbxIF->Items->Strings[lbxIF->ItemIndex];	// "15 Realtek Gaming 2.5GbE Family Controller"
					Pos = Str.Pos(" ");                             //    <-
					Desc = Str.SubString(Pos+1, Str.Length()-Pos);  // "Realtek Gaming 2.5GbE Family Controller"
					led4LineIF->Text = Desc;
					//led4LineIF->Text = lbxIF->Items->Strings[lbxIF->ItemIndex].SubString(1, 2);
				}
				break;
			case 2:
				{
					Str = lbxIF->Items->Strings[lbxIF->ItemIndex];	// "15 Realtek Gaming 2.5GbE Family Controller"
					Pos = Str.Pos(" ");                             //    <-
					Desc = Str.SubString(Pos+1, Str.Length()-Pos);  // "Realtek Gaming 2.5GbE Family Controller"
					led4WirelessIF->Text = Desc;
					//led4WirelessIF->Text = lbxIF->Items->Strings[lbxIF->ItemIndex].SubString(1, 2);
				}
				break;
        } // switch
    } // if
    pnlIF->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::OnOffsetExit(TObject *Sender)
{
    if (((TLabeledEdit *)Sender)->Text == "") return;
    try
    {
        float fValue = StrToFloat(((TLabeledEdit *)Sender)->Text);
    } catch(...)
    {
        ((TLabeledEdit *)Sender)->Text = "";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::OnFolderHelpClick(TObject *Sender)
{
    TLabeledEdit *led = NULL;
    switch(((TSpeedButton *)Sender)->Tag)
    {
        case 1:
            led = ledHome;
            break;
        case 2:
            led = ledLogger;
            break;
        case 3:
            led = ledSender;
            break;
        case 4:
            led = ledViewer;
            break;
    } // switch

    UnicodeString sDir = led->Text;
	if (SelectDirectory(sDir, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt, 1000)) led->Text = sDir;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::pcConfigChange(TObject *Sender)
{
	//각 페이지에 맞는 rsetup의 자료를 오픈하여 화면에 보여줌
    switch(pcConfig->ActivePageIndex)
    {
        case 0: //지점정보
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '1'";
            SimpleDataSet->Open();
            led1PointCode->Text    = SimpleDataSet->FieldByName("VALUE01")->AsString;
            led1PointName->Text    = SimpleDataSet->FieldByName("VALUE02")->AsString;
            led1PointAddress->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            led1PointZipCode->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;
            led1PointManager->Text = SimpleDataSet->FieldByName("VALUE05")->AsString;
            led1PhoneTel->Text     = SimpleDataSet->FieldByName("VALUE06")->AsString;
            led1PointEtc->Text     = SimpleDataSet->FieldByName("VALUE07")->AsString;
			break;

        case 1: //logger
            //유량산출방식
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
            SimpleDataSet->Open();
            cbx2StreamMethod->ItemIndex  = SimpleDataSet->FieldByName("VALUE02")->AsString.ToIntDef(-1);
            //로거 기본정보
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '1'";
            SimpleDataSet->Open();
            med2Interval->Text       = SimpleDataSet->FieldByName("VALUE01")->AsString;
            med2Period->Text         = SimpleDataSet->FieldByName("VALUE02")->AsString;
            cbx2StartTime->ItemIndex = cbx2StartTime->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
            //hydrometer1
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '2'";
            SimpleDataSet->Open();
            cbx2H1Kind->ItemIndex          = cbx2H1Kind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
            cbx2H1Comport->ItemIndex       = cbx2H1Comport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
            cbx2H1CommSpeed->ItemIndex     = cbx2H1CommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
            led2H1Offset->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
            ed2H1WN->Text                  = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ed2H1WS->Text                  = SimpleDataSet->FieldByName("VALUE08")->AsString;
            ed2H1WF->Text                  = SimpleDataSet->FieldByName("VALUE09")->AsString;
            cbx2H1ReverseOption->ItemIndex = cbx2H1ReverseOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE10")->AsString);
            //hydrometer2
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '3'";
            SimpleDataSet->Open();
            cbx2H2Kind->ItemIndex          = cbx2H2Kind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
            cbx2H2Comport->ItemIndex       = cbx2H2Comport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
            cbx2H2CommSpeed->ItemIndex     = cbx2H2CommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
            led2H2Offset->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
            ed2H2WN->Text                  = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ed2H2WS->Text                  = SimpleDataSet->FieldByName("VALUE08")->AsString;
            ed2H2WF->Text                  = SimpleDataSet->FieldByName("VALUE09")->AsString;
            cbx2H2ReverseOption->ItemIndex = cbx2H2ReverseOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE10")->AsString);
            cbx2H2UpSideOption->ItemIndex  = cbx2H2UpSideOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE11")->AsString);
            //waterlevel
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '4'";
            SimpleDataSet->Open();
            cbx2WLKind->ItemIndex      = cbx2WLKind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
            cbx2WLComport->ItemIndex   = cbx2WLComport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
            cbx2WLCommSpeed->ItemIndex = cbx2WLCommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
            led2WLOffset->Text         = SimpleDataSet->FieldByName("VALUE04")->AsString;
            cbx2H1KindChange(NULL);
            cbx2H2KindChange(NULL);
			break;

		case 2: //sender
			{
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '1'";
			SimpleDataSet->Open();
			cbx3ServerSendYN->ItemIndex  = cbx3ServerSendYN->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
			led3ServerDomain->Text       = SimpleDataSet->FieldByName("VALUE02")->AsString;
			led3ServerPort->Text         = SimpleDataSet->FieldByName("VALUE03")->AsString;
			led3ServerID->Text           = SimpleDataSet->FieldByName("VALUE04")->AsString;
			led3ServerPW->Text           = SimpleDataSet->FieldByName("VALUE05")->AsString;
			led3ServerReal->Text         = SimpleDataSet->FieldByName("VALUE06")->AsString;
			led3ServerPast->Text         = SimpleDataSet->FieldByName("VALUE07")->AsString;
			cbx3RnseaSendYN->ItemIndex   = cbx3RnseaSendYN->Items->IndexOf(SimpleDataSet->FieldByName("VALUE08")->AsString);
			led3RnseaDomain->Text        = SimpleDataSet->FieldByName("VALUE09")->AsString;
			led3RnseaPort->Text          = SimpleDataSet->FieldByName("VALUE10")->AsString;
			led3RnseaID->Text            = SimpleDataSet->FieldByName("VALUE11")->AsString;
			led3RnseaPW->Text            = SimpleDataSet->FieldByName("VALUE12")->AsString;
			led3RnseaReal->Text          = SimpleDataSet->FieldByName("VALUE13")->AsString;
			led3RnseaPast->Text          = SimpleDataSet->FieldByName("VALUE14")->AsString;
			cbx3FtpFileFormat->ItemIndex = cbx3FtpFileFormat->Items->IndexOf(SimpleDataSet->FieldByName("VALUE15")->AsString);
			//heaven2
			cbx3FTPMode->ItemIndex       = cbx3FTPMode->Items->IndexOf(SimpleDataSet->FieldByName("VALUE16")->AsString);
			cbx3RnseaMode->ItemIndex     = cbx3FTPMode->Items->IndexOf(SimpleDataSet->FieldByName("VALUE17")->AsString);
			UnicodeString sFTPType, sPrivateKey;
			sFTPType = SimpleDataSet->FieldByName("VALUE18")->AsString;	// 수집서버1
			if ( sFTPType == "FTP" ) {
				cbx3FTPType->ItemIndex = 0;	// FTP
				led3PrivateKey->Text = "";
				led3PrivateKey->Visible = false;
				btnPrivateKey->Visible = false;
			} else if ( sFTPType.Pos("SFTP") == 1 ) {
				sPrivateKey = sFTPType.SubString(6, sFTPType.Length()-5);
				cbx3FTPType->ItemIndex = 1;	// SFTP
				led3PrivateKey->Text = sPrivateKey;
				led3PrivateKey->Visible = true;
				btnPrivateKey->Visible = true;
			} else {	// default
				cbx3FTPType->ItemIndex = 0;	// FTP
				led3PrivateKey->Text = "";
				led3PrivateKey->Visible = false;
				btnPrivateKey->Visible = false;
			}
			sFTPType = SimpleDataSet->FieldByName("VALUE19")->AsString;	// 수집서버2
			if ( sFTPType == "FTP" ) {
				cbx3FTPType2->ItemIndex = 0;	// FTP
				led3PrivateKey2->Text = "";
				led3PrivateKey2->Visible = false;
				btnPrivateKey2->Visible = false;
			} else if ( sFTPType.Pos("SFTP") == 1 ) {
				sPrivateKey = sFTPType.SubString(6, sFTPType.Length()-5);
				cbx3FTPType2->ItemIndex = 1;	// SFTP
				led3PrivateKey2->Text = sPrivateKey;
				led3PrivateKey2->Visible = true;
				btnPrivateKey2->Visible = true;
			} else {	// default
				cbx3FTPType2->ItemIndex = 0;	// FTP
				led3PrivateKey2->Text = "";
				led3PrivateKey2->Visible = false;
				btnPrivateKey2->Visible = false;
			}

			//vthlogger
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '2'";
			SimpleDataSet->Open();
			cbx3RetryCount->ItemIndex       = cbx3RetryCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
			cbx3RebootFlag->ItemIndex       = cbx3RebootFlag->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
			cbx3MaxSendFileCount->ItemIndex = cbx3MaxSendFileCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
			cbx3RebootRetryCount->ItemIndex = cbx3RebootRetryCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE04")->AsString);
			cbx3VTHComport->ItemIndex       = cbx3VTHComport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE05")->AsString);
			cbx3VTHCommSpeed->ItemIndex     = cbx3VTHCommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE06")->AsString);
			UnicodeString Str;
			Str = SimpleDataSet->FieldByName("VALUE07")->AsString;
			cbx3VTHOption->ItemIndex        = ( Str.IsEmpty() || Str == "BATTERY" ) ? 0 : 1;	// DC Battery or AIN
			}
			break;

        case 3: //guardnet
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '3'";
            SimpleDataSet->Open();
            cbx4GuardnetYN->ItemIndex      = cbx4GuardnetYN->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
            cbx4GuardnetType->ItemIndex    = cbx4GuardnetType->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
            med4PCBootingTime->Text        = SimpleDataSet->FieldByName("VALUE03")->AsString;
            led4Interval->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
            cbx4PingStartTime->ItemIndex   = cbx4PingStartTime->Items->IndexOf(SimpleDataSet->FieldByName("VALUE05")->AsString);
            led4PingServer1->Text          = SimpleDataSet->FieldByName("VALUE06")->AsString;
            led4Server1Port->Text          = SimpleDataSet->FieldByName("VALUE07")->AsString;
            cbx4Ping2TestYN->ItemIndex     = cbx4Ping2TestYN->Items->IndexOf(SimpleDataSet->FieldByName("VALUE08")->AsString);
            led4Server2->Text              = SimpleDataSet->FieldByName("VALUE09")->AsString;
            led4Server2Port->Text          = SimpleDataSet->FieldByName("VALUE10")->AsString;
            cbx4PingRetryCount->ItemIndex  = cbx4PingRetryCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE11")->AsString);
            cbx4PingRetryCountM->ItemIndex = cbx4PingRetryCountM->Items->IndexOf(SimpleDataSet->FieldByName("VALUE12")->AsString);
            led4WirelessUseTime->Text      = SimpleDataSet->FieldByName("VALUE13")->AsString;
            cbx4GWResetCount->ItemIndex    = cbx4GWResetCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE14")->AsString);
			cbx4MaxPCResetCount->ItemIndex = cbx4MaxPCResetCount->Items->IndexOf(SimpleDataSet->FieldByName("VALUE15")->AsString);
			// heaven2 2026.03.02
			led4LineIF->Text               = SimpleDataSet->FieldByName("VALUE16")->AsString;  // 2026.03.02 IF => IF Description
			led4WirelessIF->Text           = SimpleDataSet->FieldByName("VALUE17")->AsString;  // 2026.03.02 IF => IF Description
            cbx4GWResetDelay->ItemIndex    = cbx4GWResetDelay->Items->IndexOf(SimpleDataSet->FieldByName("VALUE18")->AsString);
			break;

        case 4: //viewer
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
            SimpleDataSet->Open();
            lel5Ment->Text         = SimpleDataSet->FieldByName("VALUE01")->AsString;
            led5LevelOverSea->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            led5CctvUrl->Text      = SimpleDataSet->FieldByName("VALUE04")->AsString;
            cbx5LoginDelayTime->ItemIndex = SimpleDataSet->FieldByName("VALUE05")->AsString.ToIntDef(29);
            //email option
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '5'";
            SimpleDataSet->Open();
            cbxEmailAutoSend->ItemIndex = cbxEmailAutoSend->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
            ledEmailServer->Text   = SimpleDataSet->FieldByName("VALUE02")->AsString;
            ledEmailAccount->Text  = SimpleDataSet->FieldByName("VALUE03")->AsString;
            ledEmailPassword->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;
            ledEmailDest1->Text    = SimpleDataSet->FieldByName("VALUE05")->AsString;
            ledEmailDest2->Text    = SimpleDataSet->FieldByName("VALUE06")->AsString;
            ledEmailDest3->Text    = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ledEmailDest4->Text    = SimpleDataSet->FieldByName("VALUE08")->AsString;
            ledEmailDest5->Text    = SimpleDataSet->FieldByName("VALUE09")->AsString;
            //graph option
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '4'";
            SimpleDataSet->Open();
            cbxWaterFlow->Checked   = SimpleDataSet->FieldByName("VALUE01")->AsString == "ON";
            cbxWaterLevel->Checked  = SimpleDataSet->FieldByName("VALUE02")->AsString == "ON";
            cbxHydroMeter1->Checked = SimpleDataSet->FieldByName("VALUE03")->AsString == "ON";
            cbxLog->Checked         = SimpleDataSet->FieldByName("VALUE04")->AsString == "ON";
            cbxVthLogger->Checked   = SimpleDataSet->FieldByName("VALUE05")->AsString == "ON";
			break;

        case 5: //유효범위
            //vth logger
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '10'";
            SimpleDataSet->Open();
            ed6VoltMin->Text = SimpleDataSet->FieldByName("VALUE01")->AsString;
            ed6VoltMax->Text = SimpleDataSet->FieldByName("VALUE02")->AsString;

            ed6ChargeMin->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            ed6ChargeMax->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;

            ed6BatteryMin->Text = SimpleDataSet->FieldByName("VALUE05")->AsString;
            ed6BatteryMax->Text = SimpleDataSet->FieldByName("VALUE06")->AsString;

            ed6SysTempMin->Text = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ed6SysTempMax->Text = SimpleDataSet->FieldByName("VALUE08")->AsString;

            ed6SysHumidMin->Text = SimpleDataSet->FieldByName("VALUE09")->AsString;
            ed6SysHumidMax->Text = SimpleDataSet->FieldByName("VALUE10")->AsString;
            //수위계
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '11'";
            SimpleDataSet->Open();
            ed6WaterLevelMin->Text = SimpleDataSet->FieldByName("VALUE01")->AsString;
            ed6WaterLevelMax->Text = SimpleDataSet->FieldByName("VALUE02")->AsString;

            ed6SalinityMin->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            ed6SalinityMax->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;
            //유속계
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '12'";
            SimpleDataSet->Open();
            ed6TempMin->Text = SimpleDataSet->FieldByName("VALUE01")->AsString;
            ed6TempMax->Text = SimpleDataSet->FieldByName("VALUE02")->AsString;

            ed6PressureMin->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            ed6PressureMax->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;

            ed6PitchMin->Text = SimpleDataSet->FieldByName("VALUE05")->AsString;
            ed6PitchMax->Text = SimpleDataSet->FieldByName("VALUE06")->AsString;

            ed6RollMin->Text = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ed6RollMax->Text = SimpleDataSet->FieldByName("VALUE08")->AsString;

			ed6VelocityEwMin->Text = SimpleDataSet->FieldByName("VALUE09")->AsString;
			ed6VelocityEwMax->Text = SimpleDataSet->FieldByName("VALUE10")->AsString;
			ed6VelocityNsMin->Text = SimpleDataSet->FieldByName("VALUE11")->AsString;
			ed6VelocityNsMax->Text = SimpleDataSet->FieldByName("VALUE12")->AsString;
			break;

		case 6: //기타
			// heaven2 2017.09.26
			MemoPingLog->Lines->Clear();
			m_iPingCnt = 0;
			TimerPing->Enabled = false;

            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '2'";
            SimpleDataSet->Open();
            ledHome->Text   = SimpleDataSet->FieldByName("VALUE01")->AsString;
            ledLogger->Text = SimpleDataSet->FieldByName("VALUE02")->AsString;
            ledSender->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            ledViewer->Text = SimpleDataSet->FieldByName("VALUE04")->AsString;
            ledDataLoggerIP->Text = SimpleDataSet->FieldByName("VALUE05")->AsString;
            ledDataSenderIP->Text = SimpleDataSet->FieldByName("VALUE06")->AsString;
            ledFirebirdIP->Text = SimpleDataSet->FieldByName("VALUE07")->AsString;
            ledDataLoggerPort->Text = SimpleDataSet->FieldByName("VALUE08")->AsString;
            ledDataSenderPortFromLogger->Text = SimpleDataSet->FieldByName("VALUE09")->AsString;
            ledDataSenderPortFromViewer->Text = SimpleDataSet->FieldByName("VALUE10")->AsString;
			ledWatchDogPort->Text = SimpleDataSet->FieldByName("VALUE11")->AsString;

			// 2026.02.13
			ReSendFromDate->DateTime = Now();
			ReSendToDate->DateTime   = Now();
			ReSendFromTime->Text     = "00:00:00";
			ReSendToTime->Text       = "23:50:00";
            break;
    } // switch
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::btnSaveClick(TObject *Sender)
//자료저장
{
	switch(pcConfig->ActivePageIndex)
    {
		case 0: //지점정보
			{
			UnicodeString sOldPointCode, sNewPointCode;
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '1'";
			SimpleDataSet->Open();
			sOldPointCode = SimpleDataSet->FieldByName("Value01")->AsString;
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = led1PointCode->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = led1PointName->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = led1PointAddress->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = led1PointZipCode->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = led1PointManager->Text;
			SimpleDataSet->FieldByName("VALUE06")->AsString = led1PhoneTel->Text;
            SimpleDataSet->FieldByName("VALUE07")->AsString = led1PointEtc->Text;
            try
            {
                SimpleDataSet->Post();
				SimpleDataSet->ApplyUpdates(-1);

				sNewPointCode = led1PointCode->Text;

				SimpleDataSet->Close();
				SimpleDataSet->DataSet->CommandText = "SELECT * FROM RFlowADVM WHERE PointCode = '" + sOldPointCode +"' ";
				SimpleDataSet->Open();
				SimpleDataSet->Edit();
				SimpleDataSet->FieldByName("PointCode")->AsString = led1PointCode->Text;
				SimpleDataSet->Post();
				SimpleDataSet->ApplyUpdates(-1);

				SimpleDataSet->Close();
				SimpleDataSet->DataSet->CommandText = "SELECT * FROM RFlowHA WHERE PointCode = '" + sOldPointCode +"' ";
				SimpleDataSet->Open();
				SimpleDataSet->Edit();
				SimpleDataSet->FieldByName("PointCode")->AsString = led1PointCode->Text;
				SimpleDataSet->Post();
				SimpleDataSet->ApplyUpdates(-1);

				SimpleDataSet->Close();
				SimpleDataSet->DataSet->CommandText = "SELECT * FROM RFlowIVM WHERE PointCode = '" + sOldPointCode +"' ";
				SimpleDataSet->Open();
				SimpleDataSet->Edit();
				SimpleDataSet->FieldByName("PointCode")->AsString = led1PointCode->Text;
				SimpleDataSet->Post();
				SimpleDataSet->ApplyUpdates(-1);

            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
			} // catch
			}
			break;

        case 1: //logger
//            //유량산출방식
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2StreamMethod->Text.SubString(1, 1);
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //로거 기본정보
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '1'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = med2Interval->Text;
//            SimpleDataSet->FieldByName("VALUE02")->AsString = med2Period->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2StartTime->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //hydrometer1 저장
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '2'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2H1Kind->Text;
//            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2H1Comport->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2H1CommSpeed->Text;
//            SimpleDataSet->FieldByName("VALUE04")->AsString = led2H1Offset->Text;
//            SimpleDataSet->FieldByName("VALUE07")->AsString = ed2H1WN->Text;
//            SimpleDataSet->FieldByName("VALUE08")->AsString = ed2H1WS->Text;
//            SimpleDataSet->FieldByName("VALUE09")->AsString = ed2H1WF->Text;
//            SimpleDataSet->FieldByName("VALUE10")->AsString = cbx2H1ReverseOption->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //hydrometer2 저장
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '3'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2H2Kind->Text;
//            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2H2Comport->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2H2CommSpeed->Text;
//            SimpleDataSet->FieldByName("VALUE04")->AsString = led2H2Offset->Text;
//            SimpleDataSet->FieldByName("VALUE07")->AsString = ed2H2WN->Text;
//            SimpleDataSet->FieldByName("VALUE08")->AsString = ed2H2WS->Text;
//            SimpleDataSet->FieldByName("VALUE09")->AsString = ed2H2WF->Text;
//            SimpleDataSet->FieldByName("VALUE10")->AsString = cbx2H2ReverseOption->Text;
//            SimpleDataSet->FieldByName("VALUE11")->AsString = cbx2H2UpSideOption->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //waterlevel 저장
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '4'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2WLKind->Text;
//            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2WLComport->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2WLCommSpeed->Text;
//            SimpleDataSet->FieldByName("VALUE04")->AsString = led2WLOffset->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
			break;

		case 2: //sender
			{
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '1'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx3ServerSendYN->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = led3ServerDomain->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = led3ServerPort->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = led3ServerID->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = led3ServerPW->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = led3ServerReal->Text;
            SimpleDataSet->FieldByName("VALUE07")->AsString = led3ServerPast->Text;
            SimpleDataSet->FieldByName("VALUE08")->AsString = cbx3RnseaSendYN->Text;
            SimpleDataSet->FieldByName("VALUE09")->AsString = led3RnseaDomain->Text;
            SimpleDataSet->FieldByName("VALUE10")->AsString = led3RnseaPort->Text;
            SimpleDataSet->FieldByName("VALUE11")->AsString = led3RnseaID->Text;
            SimpleDataSet->FieldByName("VALUE12")->AsString = led3RnseaPW->Text;
            SimpleDataSet->FieldByName("VALUE13")->AsString = led3RnseaReal->Text;
            SimpleDataSet->FieldByName("VALUE14")->AsString = led3RnseaPast->Text;
			SimpleDataSet->FieldByName("VALUE15")->AsString = cbx3FtpFileFormat->Text;
			SimpleDataSet->FieldByName("VALUE16")->AsString = cbx3FTPMode->Text;     	// FTPMode ( Active/Passive ) heaven2
			SimpleDataSet->FieldByName("VALUE17")->AsString = cbx3RnseaMode->Text;		// FTPMode ( Active/Passive ) heaven2
			UnicodeString sFTPType, sPrivateKey;
			sFTPType = cbx3FTPType->Text;
			sPrivateKey = led3PrivateKey->Text;
			if ( cbx3FTPMode->Text == FORMAT_EWSV_ADVM ) {	// EWSV+ADVM 형식 Check
				if ( Application->MessageBox(L"EWSV+ADVM 형식은 전자파 유속계와 ADVM 유속계가 같이 운용되는 국사에 적용됩니다. 계속할까요?",L"파일형식",MB_YESNO) != IDYES ) return;
			}
			if ( sFTPType == "FTP" ) {
				if ( led3ServerPort->Text != "21" ) {
					if ( Application->MessageBox(L"FTP의 기본 포트는 21번입니다. 계속할까요?",L"Port",MB_YESNO) != IDYES ) return;
				}
				SimpleDataSet->FieldByName("VALUE18")->AsString = sFTPType;     	// FTP/SFTP(유량사업단)
			} else if ( sFTPType == "SFTP" ) {
				if ( led3ServerPort->Text == "21" ) {
					if ( Application->MessageBox(L"SFTP 방식의 기본 포트는 22번입니다. 계속할까요?",L"Port",MB_YESNO) != IDYES ) return;
				}
				SimpleDataSet->FieldByName("VALUE18")->AsString = sFTPType  + ((sPrivateKey == "" ) ? UnicodeString("") : ("|" + sPrivateKey));     	// FTP/SFTP(유량사업단)
			} else {
				SimpleDataSet->FieldByName("VALUE18")->AsString = sFTPType;     	// FTP/SFTP(유량사업단)(default)
			}
			sFTPType = cbx3FTPType2->Text;
			sPrivateKey = led3PrivateKey2->Text;
			if ( sFTPType == "FTP" ) {
				if ( led3RnseaPort->Text != "21" ) {
					if ( Application->MessageBox(L"FTP의 기본 포트는 21번입니다. 계속할까요?",L"Port",MB_YESNO) != IDYES ) return;
				}
				SimpleDataSet->FieldByName("VALUE19")->AsString = sFTPType;     	// 수집서버2
			} else if ( sFTPType == "SFTP" ) {
				if ( led3RnseaPort->Text == "21" ) {
					if ( Application->MessageBox(L"SFTP 방식의 기본 포트는 22번입니다. 계속할까요?",L"Port",MB_YESNO) != IDYES ) return;
				}
				SimpleDataSet->FieldByName("VALUE19")->AsString = sFTPType  + ((sPrivateKey == "" ) ? UnicodeString("") : ("|" + sPrivateKey));     	//  수집서버2
			} else {
				SimpleDataSet->FieldByName("VALUE19")->AsString = sFTPType;     	//  수집서버2
			}
			try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
			{
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
            } // catch
            //vthlogger 저장
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '2'";
			SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx3RetryCount->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx3RebootFlag->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx3MaxSendFileCount->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = cbx3RebootRetryCount->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = cbx3VTHComport->Text;  	// VTHLogger Port=> NONE, COM1, COM2, ... COM32
			SimpleDataSet->FieldByName("VALUE06")->AsString = cbx3VTHCommSpeed->Text;
			SimpleDataSet->FieldByName("VALUE07")->AsString = (cbx3VTHOption->ItemIndex == 0) ? "BATTERY" : "AIN";
            try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
				return;
			} // catch
			}
			break;

        case 3: //guardnet
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '3'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = cbx4GuardnetYN->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = cbx4GuardnetType->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = med4PCBootingTime->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = led4Interval->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = cbx4PingStartTime->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = led4PingServer1->Text;
            SimpleDataSet->FieldByName("VALUE07")->AsString = led4Server1Port->Text;
            SimpleDataSet->FieldByName("VALUE08")->AsString = cbx4Ping2TestYN->Text;
            SimpleDataSet->FieldByName("VALUE09")->AsString = led4Server2->Text;
            SimpleDataSet->FieldByName("VALUE10")->AsString = led4Server2Port->Text;
            SimpleDataSet->FieldByName("VALUE11")->AsString = cbx4PingRetryCount->Text;
            SimpleDataSet->FieldByName("VALUE12")->AsString = cbx4PingRetryCountM->Text;
            SimpleDataSet->FieldByName("VALUE13")->AsString = led4WirelessUseTime->Text;
            SimpleDataSet->FieldByName("VALUE14")->AsString = cbx4GWResetCount->Text;
			SimpleDataSet->FieldByName("VALUE15")->AsString = cbx4MaxPCResetCount->Text;
			// heaven2 2026.03.02
			//SimpleDataSet->FieldByName("VALUE16")->AsString = led4LineIF->Text;
			//SimpleDataSet->FieldByName("VALUE17")->AsString = led4WirelessIF->Text;
			SimpleDataSet->FieldByName("VALUE16")->AsString = led4LineIF->Text;		//2026.03.02 IF => IF Description
			SimpleDataSet->FieldByName("VALUE17")->AsString = led4WirelessIF->Text; //2026.03.02 IF => IF Description
			SimpleDataSet->FieldByName("VALUE18")->AsString = cbx4GWResetDelay->Text;
			try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
            } // catch
			break;

        case 4: //viewer
//			//그래프 옵션
//            SimpleDataSet->Close();
//			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = lel5Ment->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = led5LevelOverSea->Text;
//            SimpleDataSet->FieldByName("VALUE04")->AsString = led5CctvUrl->Text;
//            SimpleDataSet->FieldByName("VALUE05")->AsString = cbx5LoginDelayTime->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //점검일지 email관련 수신자 정보; 없으면 디폴트 값으로 로딩
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '5'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = cbxEmailAutoSend->Text;
//            SimpleDataSet->FieldByName("VALUE02")->AsString = ledEmailServer->Text;
//            SimpleDataSet->FieldByName("VALUE03")->AsString = ledEmailAccount->Text;
//            SimpleDataSet->FieldByName("VALUE04")->AsString = ledEmailPassword->Text;
//            SimpleDataSet->FieldByName("VALUE05")->AsString = ledEmailDest1->Text;
//            SimpleDataSet->FieldByName("VALUE06")->AsString = ledEmailDest2->Text;
//            SimpleDataSet->FieldByName("VALUE07")->AsString = ledEmailDest3->Text;
//            SimpleDataSet->FieldByName("VALUE08")->AsString = ledEmailDest4->Text;
//            SimpleDataSet->FieldByName("VALUE09")->AsString = ledEmailDest5->Text;
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
//            //그래프관련
//            if (!cbxWaterLevel->Checked) cbxWaterLevel->Checked = true; //checked으로 고정
//            if (!cbxHydroMeter1->Checked) cbxHydroMeter1->Checked = true; //checked으로 고정
//            SimpleDataSet->Close();
//            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '4'";
//            SimpleDataSet->Open();
//            SimpleDataSet->Edit();
//            SimpleDataSet->FieldByName("VALUE01")->AsString = (cbxWaterFlow->Checked  ) ? "ON" : "OFF";
//            SimpleDataSet->FieldByName("VALUE02")->AsString = (cbxWaterLevel->Checked ) ? "ON" : "OFF";
//            SimpleDataSet->FieldByName("VALUE03")->AsString = (cbxHydroMeter1->Checked) ? "ON" : "OFF";
//            SimpleDataSet->FieldByName("VALUE04")->AsString = (cbxLog->Checked        ) ? "ON" : "OFF";
//            SimpleDataSet->FieldByName("VALUE05")->AsString = (cbxVthLogger->Checked  ) ? "ON" : "OFF";
//            try
//            {
//                SimpleDataSet->Post();
//                SimpleDataSet->ApplyUpdates(-1);
//            } catch(Exception &e)
//            {
//                ShowMessage(e.Message);
//                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
//                return;
//            } // catch
			break;

        case 5: //유효범위
            //vth logger
			SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '10'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = ed6VoltMin->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = ed6VoltMax->Text;

            SimpleDataSet->FieldByName("VALUE03")->AsString = ed6ChargeMin->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = ed6ChargeMax->Text;

            SimpleDataSet->FieldByName("VALUE05")->AsString = ed6BatteryMin->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = ed6BatteryMax->Text;

            SimpleDataSet->FieldByName("VALUE07")->AsString = ed6SysTempMin->Text;
            SimpleDataSet->FieldByName("VALUE08")->AsString = ed6SysTempMax->Text;

            SimpleDataSet->FieldByName("VALUE09")->AsString = ed6SysHumidMin->Text;
            SimpleDataSet->FieldByName("VALUE10")->AsString = ed6SysHumidMax->Text;
            try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
            } // catch
            //수위계
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '11'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = ed6WaterLevelMin->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = ed6WaterLevelMax->Text;

            SimpleDataSet->FieldByName("VALUE03")->AsString = ed6SalinityMin->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = ed6SalinityMax->Text;
            try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
            } // catch
            //유속계
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '20' AND PK2 = '12'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = ed6TempMin->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = ed6TempMax->Text;

            SimpleDataSet->FieldByName("VALUE03")->AsString = ed6PressureMin->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = ed6PressureMax->Text;

            SimpleDataSet->FieldByName("VALUE05")->AsString = ed6PitchMin->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = ed6PitchMax->Text;

            SimpleDataSet->FieldByName("VALUE07")->AsString = ed6RollMin->Text;
            SimpleDataSet->FieldByName("VALUE08")->AsString = ed6RollMax->Text;

			SimpleDataSet->FieldByName("VALUE09")->AsString = ed6VelocityEwMin->Text;
			SimpleDataSet->FieldByName("VALUE10")->AsString = ed6VelocityEwMax->Text;
			SimpleDataSet->FieldByName("VALUE11")->AsString = ed6VelocityNsMin->Text;
			SimpleDataSet->FieldByName("VALUE12")->AsString = ed6VelocityNsMax->Text;
			try
			{
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
			} // catch
			break;

        case 6: //기타
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '2'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = ledHome->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = ledLogger->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = ledSender->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = ledViewer->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = ledDataLoggerIP->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = ledDataSenderIP->Text;
            SimpleDataSet->FieldByName("VALUE07")->AsString = ledFirebirdIP->Text;
			SimpleDataSet->FieldByName("VALUE08")->AsString = ledDataLoggerPort->Text;
            SimpleDataSet->FieldByName("VALUE09")->AsString = ledDataSenderPortFromLogger->Text;
            SimpleDataSet->FieldByName("VALUE10")->AsString = ledDataSenderPortFromViewer->Text;
            SimpleDataSet->FieldByName("VALUE11")->AsString = ledWatchDogPort->Text;
            try
            {
                SimpleDataSet->Post();
                SimpleDataSet->ApplyUpdates(-1);
            } catch(Exception &e)
            {
                ShowMessage(e.Message);
                if (SimpleDataSet->State == dsEdit) SimpleDataSet->Cancel();
                return;
            } // catch
            break;
    } // switch

	bSaved = True;
	ShowMessage("저장되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormConfig = NULL;
    Action = caFree;
}

//---------------------------------------------------------------------------
// Ping to DataLogger
void __fastcall TFormConfig::ButtonPingClick(TObject *Sender)
{
	bool bResult = false;
	bResult = frmMain->SendPing();
	if ( bResult ) {
		TimerPing->Interval = 2 * 1000;	 // 2 second timer
		TimerPing->Enabled = true;
		PingLog("Ping..");
		m_iPingCnt = 1;
	} else {
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping failed..");
	}
}

//---------------------------------------------------------------------------
// DataLogger PC Reboot
void __fastcall TFormConfig::ButtonRebootClick(TObject *Sender)
{
	if ( TimerPing->Enabled ) {
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping stop..");
	}

	// Warning
	if ( Application->MessageBox(L"Logger PC를 리부팅합니다. \r\n\r\n" \
								 L"Logger PC를 리부팅하면 DataSender와 DataViewer의 DB 접속이 끊어집니다." \
								 L"Logger PC가 리부팅이 완료된 후(2~3분후)에 DataSender와 DataViewer를 재실행해야 합니다.\r\n\r\n" \
								 L"Logger PC를 재부팅할까요?", L"Logger PC 리부팅", MB_YESNO) != IDYES )  return;

	bool bResult = false;
	bResult = frmMain->SendRebootReq();
	if ( bResult ) {
		PingLog("RebootReq to DataLogger");
	} else {
		PingLog("RebootReq failed..");
	}
}

//---------------------------------------------------------------------------
// Ping Timer
void __fastcall TFormConfig::TimerPingTimer(TObject *Sender)
{
	m_iPingCnt++;
	if ( m_iPingCnt >= 5 ) {  	// Ping Timer Expire
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping stop..");
	} else {
		bool bResult = false;
		bResult = frmMain->SendPing();
		if ( bResult ) {
			PingLog("Ping..");
		} else {
			TimerPing->Enabled = false;
			PingLog("Ping failed..");
		}
	}
}

//---------------------------------------------------------------------------
// Ping Test Log
void TFormConfig::PingLog(UnicodeString sMsg)
{
	if ( MemoPingLog->Lines->Count > 100 ) MemoPingLog->Lines->Clear();
	MemoPingLog->Lines->Insert(0, sMsg);
}

//---------------------------------------------------------------------------
// DataLogger message
void TFormConfig::DataLoggerMessage(BYTE *pData, int Len)
{
	BYTE Type = *(pData + 1);

	if ( Type == MSG_TYPE_PingAck ) {
		PingLog("Ping acked..");
		//LogMessage("Ping acked..");
	} else if ( Type == MSG_TYPE_RebootRes ) {
		PingLog("RebootRes received..");
		//LogMessage("RebootRes recevied..");
	} else {
		PingLog("unknown message type");
		//LogMessage("unknown message type");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormConfig::cbx3FTPTypeChange(TObject *Sender)
{
	if ( cbx3FTPType->ItemIndex == 1 ) { 	// SFTP
		led3PrivateKey->Visible = true;
		btnPrivateKey->Visible = true;
		ShowMessage("SFTP Server가 PublicKey를 가지고 있다면 이와 쌍을 이루는 PrivateKey를 지정할 수 있습니다.");
	} else {
		led3PrivateKey->Visible = false;
		btnPrivateKey->Visible = false;
		//ShowMessage("SFTP Server가 PublicKey를 가지고 있다면 이와 쌍을 이루는 PrivateKey를 지정할 수 있습니다.");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormConfig::cbx3FTPType2Change(TObject *Sender)
{
	if ( cbx3FTPType2->ItemIndex == 1 ) { 	// SFTP
		led3PrivateKey2->Visible = true;
		btnPrivateKey2->Visible = true;
		ShowMessage("SFTP Server가 PublicKey를 가지고 있다면 이와 쌍을 이루는 PrivateKey를 지정할 수 있습니다.");
	} else {
		led3PrivateKey2->Visible = false;
		btnPrivateKey2->Visible = false;
		//ShowMessage("SFTP Server가 PublicKey를 가지고 있다면 이와 쌍을 이루는 PrivateKey를 지정할 수 있습니다.");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormConfig::btnPrivateKeyClick(TObject *Sender)
{
	OpenDialog->InitialDir = GetCurrentDir();
	if (OpenDialog->Execute())
		led3PrivateKey->Text = OpenDialog->FileName;
}

//---------------------------------------------------------------------------
void __fastcall TFormConfig::btnPrivateKey2Click(TObject *Sender)
{
	OpenDialog->InitialDir = GetCurrentDir();
	if (OpenDialog->Execute())
		led3PrivateKey2->Text = OpenDialog->FileName;
}

//---------------------------------------------------------------------------
void __fastcall TFormConfig::FormShow(TObject *Sender)
{
	m_iPingCnt = 0;

	SimpleDataSet->Connection = frmMain->SQLConnection;
	pcConfigChange(NULL);
}

//---------------------------------------------------------------------------
// 재전송 처리: RPoint의 FileFlag, ServerFlag를 조정하여 DataSender가 다시 전송할 수 있도록 한다.
void __fastcall TFormConfig::btnReSendClick(TObject *Sender)
{
	UnicodeString FromDate, FromTime;
	UnicodeString ToDate, ToTime;

	// Error Check
	if ( ReSendFromDate->DateTime > ReSendToDate->DateTime ) {
		ShowMessage("재전송 기간 날짜 오류");
		return;
	}

	FromDate = ReSendFromDate->DateTime.FormatString("yyyymmdd");
	FromTime = ReSendFromTime->Text;
	ToDate   = ReSendToDate->DateTime.FormatString("yyyymmdd");
	ToTime   = ReSendToTime->Text;

	if ( FromTime.Length() != 8 || ToTime.Length() != 8 ) {
		ShowMessage("재전송 기간 시간 오류");
		return;
	}

	FromTime = FromTime.SubString(1,2) + FromTime.SubString(4,2) + FromTime.SubString(7,2);	// 00:10:00 => 001000으로 처리(:를 뺀다)
	ToTime   = ToTime.SubString(1,2)   + ToTime.SubString(4,2)   + ToTime.SubString(7,2);	// 00:10:00 => 001000으로 처리(:를 뺀다)

	UnicodeString sSQL =
		" UPDATE RPoint          "
		" SET FileFlag   = 'N' , "
		"     ServerFlag = 'N' , "
		"     RnseaFlag  = 'N'   "
		" WHERE ( MeasureDate = '" + FromDate + "' AND MeasureTime >= '" + FromTime + "' ) OR " +
		"       ( MeasureDate > '" + FromDate + "' AND MeasureDate <  '" + ToDate   + "' ) OR " +
		"       ( MeasureDate = '" + ToDate   + "' AND MeasureTime <= '" + ToTime   + "' )    " ;

	TSQLQuery *pQuery = new TSQLQuery(NULL);
	pQuery->SQLConnection = frmMain->SQLConnection;
	pQuery->Active = false;
	pQuery->SQL->Clear();
	pQuery->SQL->Text = sSQL;

	try {
		int iRow = pQuery->ExecSQL();

		ShowMessage("ok..");
	} catch ( Exception &e ) {
		ShowMessage(e.Message);
	}

	delete pQuery;

}
//---------------------------------------------------------------------------

