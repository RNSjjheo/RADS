//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitConfig.h"
#include "UnitMain.h"
#include "DvEnv.h"
#include "LockeCommon.h"
#include "FileCtrl.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormConfig *FormConfig = NULL;

//---------------------------------------------------------------------------
void ShowConfig(int iPageSelections, int iActivePageIndex)
{
	FormConfig = new TFormConfig(Application->Owner);
	//숨겨야할 페이지 설정
	if ((iPageSelections & ciPoint)        != ciPoint       ) FormConfig->pcConfig->Pages[0]->TabVisible = False;
	if ((iPageSelections & ciLogger)       != ciLogger      ) FormConfig->pcConfig->Pages[1]->TabVisible = False;
	if ((iPageSelections & ciSender)       != ciSender      ) FormConfig->pcConfig->Pages[2]->TabVisible = False;
	if ((iPageSelections & ciGuardnet)     != ciGuardnet    ) FormConfig->pcConfig->Pages[3]->TabVisible = False;
	if ((iPageSelections & ciViewer)       != ciViewer      ) FormConfig->pcConfig->Pages[4]->TabVisible = False;
    if ((iPageSelections & ciRange)        != ciRange       ) FormConfig->pcConfig->Pages[5]->TabVisible = False;
    if ((iPageSelections & ciRange)        != ciRange       ) FormConfig->pcConfig->Pages[5]->TabVisible = False;
	if ((iPageSelections & ciStreamMethod) != ciStreamMethod) FormConfig->pcConfig->Pages[6]->TabVisible = False;
    if ((iPageSelections & ciEtc)          != ciEtc         ) FormConfig->pcConfig->Pages[7]->TabVisible = False;
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
		case ciStreamMethod :
			FormConfig->pcConfig->ActivePageIndex = 6;
			break;
        case ciEtc:
			FormConfig->pcConfig->ActivePageIndex = 7;
            break;
    } // switch

    FormConfig->ShowModal();
    delete FormConfig;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::FormActivate(TObject *Sender)
{
	pcConfigChange(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFormConfig::TFormConfig(TComponent* Owner)
    : TForm(Owner)
{
	bSaved          = false;	// 제원의 저장 여부
	m_bLoggerNotify = false;	// Logger에 통지 유무
	m_bSenderNotify = false;	// Sender에 통지 유무

	m_AtmosPort     = "";

    SimpleDataSet->Connection = FormMain->SQLConnection;
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::btnCloseClick(TObject *Sender)
{
	if (bSaved)
	{
		FormMain->GetDBEnvInfo();
		FormMain->WriteLog(!CbFileWrite, CbDBWrite, "환경설정이 변경되었습니다.");

		// Logger Notify
		if ( m_bLoggerNotify ) {
			BYTE cType = MSG_TYPE_LoggerSetupChange;
			BYTE cData[100] = "";
			int  iLen       = 4;
			FormMain->SendLogger(cType, cData, iLen);
		}
		// Sender Notify
		if ( m_bSenderNotify ) {
			BYTE cType = MSG_TYPE_SenderSetupChange;
			BYTE cData[100] = "";
			int  iLen       = 4;
			FormMain->SendSender(cType, cData, iLen);
		}

		ShowMessage("변경 내역이 반영되려면 프로그램을 종료 후 재시작하여 주십시오.");
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

	if ( cbx2H1Kind->ItemIndex == NONE_IDX ) {   	// NONE => 장비 없음 선택 시
		cbx2H1Comport->ItemIndex  = NONE_IDX;
		cbx2H1CommSpeed->ItemIndex = NONE_IDX;
	}

	if ( cbx2H1Kind->ItemIndex == SL_SERIES_IDX || cbx2H1Kind->ItemIndex == AG_SERIES_IDX ) {   	// SL/AG Device
		Panel1->Visible      = true;
		PanelAirmar->Visible = false;
		PanelRQ30d->Visible  = false;	// RQ-30d
	} else if ( cbx2H1Kind->ItemIndex == 6 ) {	// RQ-30
		Panel1->Visible      = false;
		PanelAirmar->Visible = true;
		PanelRQ30d->Visible  = false;	// RQ-30d
	} else if ( cbx2H1Kind->ItemIndex == 7 ) {	// RQ-30d
		Panel1->Visible      = false;
		PanelAirmar->Visible = true;
		PanelRQ30d->Visible  = true;	// RQ-30d
	} else {
		Panel1->Visible      = false;
		PanelAirmar->Visible = false;
		PanelRQ30d->Visible  = false;	// RQ-30d
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::cbx2H2KindChange(TObject *Sender)
{
	if ( cbx2H2Kind->ItemIndex == NONE_IDX ) {   	// NONE => 장비 없음 선택 시
		cbx2H2Comport->ItemIndex  = NONE_IDX;
		cbx2H2CommSpeed->ItemIndex = NONE_IDX;
	}

	if ( cbx2H2Kind->ItemIndex == SL_SERIES_IDX || cbx2H2Kind->ItemIndex == AG_SERIES_IDX ) {
		Panel2->Visible = true;
		//ed2H2WN->Enabled = (cbx2H2Kind->ItemIndex == 2 || cbx2H2Kind->ItemIndex == 3); //ag/sl series 일 경우만 활성화
		//if (!ed2H2WN->Enabled) ed2H2WN->Text = "";
		//ed2H2WS->Enabled = (cbx2H2Kind->ItemIndex == 2 || cbx2H2Kind->ItemIndex == 3); //ag/sl series 일 경우만 활성화
		//if (!ed2H2WS->Enabled) ed2H2WS->Text = "";
		//ed2H2WF->Enabled = (cbx2H2Kind->ItemIndex == 2 || cbx2H2Kind->ItemIndex == 3); //ag/sl series 일 경우만 활성화
		//if (!ed2H2WF->Enabled) ed2H2WF->Text = "";
	} else {
		Panel2->Visible = false;
	}
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
    pnlIF->Visible = True;

    lbxIF->Tag = ((TSpeedButton *)Sender)->Tag;
    lbxIF->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::lbxIFExit(TObject *Sender)
{
    pnlIF->Visible = False;
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
    pnlIF->Visible = False;
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
			{
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '1'";
            SimpleDataSet->Open();
            med2Interval->Text       = SimpleDataSet->FieldByName("VALUE01")->AsString;
            med2Period->Text         = SimpleDataSet->FieldByName("VALUE02")->AsString;
			cbx2StartTime->ItemIndex = cbx2StartTime->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);

			//hydrometer1 ------------------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '2'";
			SimpleDataSet->Open();
			UnicodeString Str = SimpleDataSet->FieldByName("VALUE01")->AsString;
			Str = (Str == CsRQ30 ) ? UnicodeString("RQ-30") : ((Str == CsRQ30D) ? UnicodeString("RQ-30d") : Str);  // RQ30 => RQ-30, RQ30D => RQ-30d
			cbx2H1Kind->ItemIndex          = cbx2H1Kind->Items->IndexOf(Str);
			if ( cbx2H1Kind->ItemIndex == SL_SERIES_IDX || cbx2H1Kind->ItemIndex == AG_SERIES_IDX ) {
				Panel1->Visible      = true; 	// SL/AG Series
				PanelAirmar->Visible = false;
				PanelRQ30d->Visible  = false;
			} else if ( cbx2H1Kind->ItemIndex == RQ_30_IDX ) { // RQ-30
				Panel1->Visible      = false; 	// SL/AG Series
				PanelAirmar->Visible = true;
				PanelRQ30d->Visible  = false;
			} else if ( cbx2H1Kind->ItemIndex == RQ_30D_IDX ) { // RQ-30d
				Panel1->Visible      = false; 	// SL/AG Series
				PanelAirmar->Visible = true;
				PanelRQ30d->Visible  = true;
			} else {
				Panel1->Visible      = false; 	// other case
				PanelAirmar->Visible = false;
				PanelRQ30d->Visible  = false;
			}
			cbx2H1Comport->ItemIndex       = cbx2H1Comport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
			cbx2H1CommSpeed->ItemIndex     = cbx2H1CommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
			led2H1Offset->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
			if ( cbx2H1Kind->ItemIndex == SL_SERIES_IDX || cbx2H1Kind->ItemIndex == AG_SERIES_IDX ) {	// SL/AG Series
				ed2H1WN->Text                  = SimpleDataSet->FieldByName("VALUE07")->AsString;
				ed2H1WS->Text                  = SimpleDataSet->FieldByName("VALUE08")->AsString;
				ed2H1WF->Text                  = SimpleDataSet->FieldByName("VALUE09")->AsString;
			}
			cbx2H1ReverseOption->ItemIndex = cbx2H1ReverseOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE10")->AsString);
			led2Hz->Text                   = SimpleDataSet->FieldByName("VALUE11")->AsString;	// Frequency
			led2SN->Text                   = SimpleDataSet->FieldByName("VALUE12")->AsString;	// Serial Number
			UnicodeString x;
			x = SimpleDataSet->FieldByName("VALUE14")->AsString;
			cbx2H1IdleCheck->ItemIndex     = (x.IsEmpty() || x == "ON") ? 0 : 1;	// default : ON, other => OFF
			x = SimpleDataSet->FieldByName("VALUE15")->AsString;
			cbx2H1IdleCheckTime->ItemIndex = (x.IsEmpty()) ? 2 : (StrToInt(x) -1);  // 3 hours => itemindex => 2
			// Airmar
			cbx2H1AirmarPort->ItemIndex = cbx2H1AirmarPort->Items->IndexOf(SimpleDataSet->FieldByName("VALUE16")->AsString);
			cbx2H1AirmarBaud->ItemIndex = cbx2H1AirmarBaud->Items->IndexOf(SimpleDataSet->FieldByName("VALUE17")->AsString);
			UnicodeString AirmarLinker = SimpleDataSet->FieldByName("Value20")->AsString;
			cbAirmarLinker->Checked = (AirmarLinker == "ON") ? true : false;
			UnicodeString StartCell, EndCell;
			StartCell = SimpleDataSet->FieldByName("VALUE18")->AsString;
			EndCell   = SimpleDataSet->FieldByName("VALUE19")->AsString;
			led2StartCell->Text         = (StartCell.IsEmpty()) ? UnicodeString("1") : StartCell;
			led2EndCell->Text           = (EndCell.IsEmpty()) ? UnicodeString("10") : EndCell;
			// RQ-30d
			// RQ-30d Section. 2024.09.11
			if ( cbx2H1Kind->ItemIndex == RQ_30D_IDX ) { // RQ-30d
				UnicodeString Value05, Value06, Value07, Value08, Value09, Value13;
				Value05 = SimpleDataSet->FieldByName("VALUE05")->AsString;
				Value06 = SimpleDataSet->FieldByName("VALUE06")->AsString;
				Value07 = SimpleDataSet->FieldByName("VALUE07")->AsString;
				Value08 = SimpleDataSet->FieldByName("VALUE08")->AsString;
				Value09 = SimpleDataSet->FieldByName("VALUE09")->AsString;
				Value13 = SimpleDataSet->FieldByName("VALUE13")->AsString;
				cbxSlaveCount->ItemIndex      = (Value13.IsEmpty()) ? 0 : StrToInt(Value13);	// SlaveCount 1 => ItemIndex 1
				edHydro1MasterAddress->Text   = Value05; // Master Port Device Address (ex) 5,4,3,2,1(좌안부터)
				cbxHydro1UseSlavePort->Checked= (Value06 == "ON") ? true : false;
				cbHydro1SlavePort->ItemIndex  = cbHydro1SlavePort->Items->IndexOf(Value07);  	  // COM01/COM02/...
				cbHydro1SlaveBaudRate->ItemIndex= cbHydro1SlaveBaudRate->Items->IndexOf(Value08); // 2400,4800,..
				edHydro1SlaveAddress->Text    = Value09; // Slave Port Device Address (ex) 6,7,8,9,10(좌안부터)
			} else {
				cbxSlaveCount->ItemIndex      = 0;	   // SlaveCount 1 => ItemIndex 1
				edHydro1MasterAddress->Text   = "";    // Master Port Device Address (ex) 5,4,3,2,1(좌안부터)
				cbxHydro1UseSlavePort->Checked= false; // slave port not use
				cbHydro1SlavePort->ItemIndex  = 0;     // COM01/COM02/...
				cbHydro1SlaveBaudRate->ItemIndex= 0;   // 2400,4800,..
				edHydro1SlaveAddress->Text    = "";    // Slave Port Device Address (ex) 6,7,8,9,10(좌안부터)
			}

			//hydrometer2 --------------------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '3'";
			SimpleDataSet->Open();
			cbx2H2Kind->ItemIndex          = cbx2H2Kind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
			if ( cbx2H2Kind->ItemIndex == SL_SERIES_IDX || cbx2H2Kind->ItemIndex == AG_SERIES_IDX ) Panel2->Visible = true; 	// SL/AG Series
			else                                                            Panel2->Visible = false;	// ChannelMaster
			cbx2H2Comport->ItemIndex       = cbx2H2Comport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
			cbx2H2CommSpeed->ItemIndex     = cbx2H2CommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
			led2H2Offset->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
			ed2H2WN->Text                  = SimpleDataSet->FieldByName("VALUE07")->AsString;
			ed2H2WS->Text                  = SimpleDataSet->FieldByName("VALUE08")->AsString;
			ed2H2WF->Text                  = SimpleDataSet->FieldByName("VALUE09")->AsString;
			cbx2H2ReverseOption->ItemIndex = cbx2H2ReverseOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE10")->AsString);
			cbx2H2UpSideOption->ItemIndex  = cbx2H2UpSideOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE11")->AsString);
			led2Hz2->Text                  = SimpleDataSet->FieldByName("VALUE12")->AsString;	// Frequency
			led2SN2->Text                  = SimpleDataSet->FieldByName("VALUE13")->AsString;	// Serial Number
			x = SimpleDataSet->FieldByName("VALUE14")->AsString;
			cbx2H2IdleCheck->ItemIndex     = (x.IsEmpty() || x == "ON") ? 0 : 1;	// default : ON, other => OFF
			x = SimpleDataSet->FieldByName("VALUE15")->AsString;
			cbx2H2IdleCheckTime->ItemIndex = (x.IsEmpty()) ? 2 : (StrToInt(x) -1);  // 3 hours => itemindex => 2
			StartCell = SimpleDataSet->FieldByName("VALUE18")->AsString;
			EndCell   = SimpleDataSet->FieldByName("VALUE19")->AsString;
			led2StartCell2->Text         = (StartCell.IsEmpty()) ? UnicodeString("1") : StartCell;
			led2EndCell2->Text           = (EndCell.IsEmpty()) ? UnicodeString("10") : EndCell;

			//hydrometer3 --------------------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '5'";
			SimpleDataSet->Open();
			cbx2H3Kind->ItemIndex          = cbx2H3Kind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
			if ( cbx2H3Kind->ItemIndex == SL_SERIES_IDX || cbx2H3Kind->ItemIndex == AG_SERIES_IDX ) Panel2->Visible = true; 	// SL/AG Series
			else                                                            Panel2->Visible = false;	// ChannelMaster
			cbx2H3Comport->ItemIndex       = cbx2H2Comport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
			cbx2H3CommSpeed->ItemIndex     = cbx2H2CommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
			led2H3Offset->Text             = SimpleDataSet->FieldByName("VALUE04")->AsString;
			ed2H3WN->Text                  = SimpleDataSet->FieldByName("VALUE07")->AsString;
			ed2H3WS->Text                  = SimpleDataSet->FieldByName("VALUE08")->AsString;
			ed2H3WF->Text                  = SimpleDataSet->FieldByName("VALUE09")->AsString;
			cbx2H3ReverseOption->ItemIndex = cbx2H2ReverseOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE10")->AsString);
			cbx2H3UpSideOption->ItemIndex  = cbx2H2UpSideOption->Items->IndexOf(SimpleDataSet->FieldByName("VALUE11")->AsString);
			led2Hz3->Text                  = SimpleDataSet->FieldByName("VALUE12")->AsString;	// Frequency
			led2SN3->Text                  = SimpleDataSet->FieldByName("VALUE13")->AsString;	// Serial Number
			x = SimpleDataSet->FieldByName("VALUE14")->AsString;
			cbx2H3IdleCheck->ItemIndex     = (x.IsEmpty() || x == "ON") ? 0 : 1;	// default : ON, other => OFF
			x = SimpleDataSet->FieldByName("VALUE15")->AsString;
			cbx2H3IdleCheckTime->ItemIndex = (x.IsEmpty()) ? 2 : (StrToInt(x) -1);  // 3 hours => itemindex => 2
			StartCell = SimpleDataSet->FieldByName("VALUE18")->AsString;
			EndCell   = SimpleDataSet->FieldByName("VALUE19")->AsString;
			led2StartCell3->Text         = (StartCell.IsEmpty()) ? UnicodeString("1") : StartCell;
			led2EndCell3->Text           = (EndCell.IsEmpty()) ? UnicodeString("10") : EndCell;

			//waterlevel ------------------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '4'";
			SimpleDataSet->Open();
			UnicodeString WaterKind = SimpleDataSet->FieldByName("VALUE01")->AsString; // NONE, AT200, LT400+Atmos, LT500, OTTCBS,..
			UnicodeString AtmosPort = SimpleDataSet->FieldByName("VALUE07")->AsString; // NONE, COM1, ...
			if ( WaterKind == LT400_STR ) cbx2WLKind->ItemIndex = LT400_ATMOS_IDX;	// LT400+Atmos
			else                          cbx2WLKind->ItemIndex = cbx2WLKind->Items->IndexOf(SimpleDataSet->FieldByName("VALUE01")->AsString);
			cbx2WLComport->ItemIndex   = cbx2WLComport->Items->IndexOf(SimpleDataSet->FieldByName("VALUE02")->AsString);
			cbx2WLCommSpeed->ItemIndex = cbx2WLCommSpeed->Items->IndexOf(SimpleDataSet->FieldByName("VALUE03")->AsString);
			led2WLOffset->Text         = SimpleDataSet->FieldByName("VALUE04")->AsString;
			led2WLSN->Text             = SimpleDataSet->FieldByName("VALUE06")->AsString;
			if ( AtmosPort.IsEmpty() ) AtmosPort = "NONE";
			m_AtmosPort = AtmosPort;
			cbx2AtmosPort->ItemIndex   = cbx2AtmosPort->Items->IndexOf(AtmosPort);
			edBaseAtmos->Text          = SimpleDataSet->FieldByName("VALUE08")->AsString;
			// heaven2 2026.03.03
			if ( WaterKind == LT400_STR || WaterKind == RNSWATER_STR ) {
				lb2WaterAtmos->Visible   = true;   cbx2WaterAtmos->Visible  = true; 	// 대기압 보정 Enable/Disable
				lbAtmosCaption->Visible  = true;   cbx2AtmosPort->Visible   = true;		// 대기압포트
												   cbx2AtmosPort->ItemIndex = cbx2AtmosPort->Items->IndexOf(m_AtmosPort); // 대기압 포트
												   edBaseAtmos->Visible     = true;     // 기준 대기압
			} else { // 그 외 장비 => 대기압 보정 없음
				lb2WaterAtmos->Visible   = false;   cbx2WaterAtmos->Visible = false; 	// 대기압 보정 Enable/Disable
				lbAtmosCaption->Visible  = false;  cbx2AtmosPort->Visible   = false; 	// 대기압포트
												   cbx2AtmosPort->ItemIndex = 0;	    // NONE
												   edBaseAtmos->Visible     = false;	// 기준 대기압
			}
			if ( WaterKind == RNSWATER_STR ) {	// RNSWater Device
				lb2WaterAirmar->Visible = true;	   cbx2WaterAirmar->Visible = true;
			} else {
				lb2WaterAirmar->Visible = false;   cbx2WaterAirmar->Visible = false;
			}
			x = SimpleDataSet->FieldByName("VALUE09")->AsString;
			cbx2WLIdleCheck->ItemIndex     = (x.IsEmpty() || x == "ON") ? 0 : 1;	// default : ON, other => OFF
			x = SimpleDataSet->FieldByName("VALUE10")->AsString;
			cbx2WLIdleCheckTime->ItemIndex = (x.IsEmpty()) ? 2 : (StrToInt(x) -1);  // 3 hours => itemindex => 2
			// Atmos Enable, Airmar Enable
			x = SimpleDataSet->FieldByName("VALUE11")->AsString;	// ON/OFF
			cbx2WaterAtmos->Checked = (x == "ON") ? true : false;	// 대기압 보정 on/off
			x = SimpleDataSet->FieldByName("VALUE12")->AsString;	// ON/OFF
			cbx2WaterAirmar->Checked = (x == "ON") ? true : false;	// 기상센서 데이터 수용 on/off
			x = SimpleDataSet->FieldByName("VALUE13")->AsString;	// 수위 데이터 사용 기준 : HydroMeter / WaterLevel
			cbx2WaterUseFlag->ItemIndex = ( x == HYDROMETER_STR ) ? 0 : 1; // ( Default: 수위계의 수위 사용 )

			cbx2H1KindChange(NULL);	// 유속계1
			cbx2H2KindChange(NULL);	// 유속계2
			cbx2H3KindChange(NULL);	// 유속계3
			}
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
			led4LineIF->Text               = SimpleDataSet->FieldByName("VALUE16")->AsString;
            led4WirelessIF->Text           = SimpleDataSet->FieldByName("VALUE17")->AsString;
            cbx4GWResetDelay->ItemIndex    = cbx4GWResetDelay->Items->IndexOf(SimpleDataSet->FieldByName("VALUE18")->AsString);
			break;

        case 4: //viewer
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
            SimpleDataSet->Open();
            lel5Ment->Text         = SimpleDataSet->FieldByName("VALUE01")->AsString;
            led5LevelOverSea->Text = SimpleDataSet->FieldByName("VALUE03")->AsString;
            led5CctvUrl->Text      = SimpleDataSet->FieldByName("VALUE04")->AsString;
			cbx5LoginDelayTime->ItemIndex = SimpleDataSet->FieldByName("VALUE05")->AsString.ToIntDef(0) - 1;
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
            //없으면 디폴트 값으로 로딩
            if (ledEmailServer->Text   == "") ledEmailServer->Text   = CsDefaultEmailServerAddress;
            if (ledEmailAccount->Text  == "") ledEmailAccount->Text  = CsDefaultEmailAccount;
            if (ledEmailPassword->Text == "") ledEmailPassword->Text = CsDefaultEmailPassword;
            if (ledEmailDest1->Text    == "") ledEmailDest1->Text    = CsDefaultEmailDest1;
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

		case 6: //자동유량
			PageControlFlowStream->ActivePageIndex = 0;

			//------------------------------------------------------------------
			// 유량산출방식
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
			SimpleDataSet->Open();
			cbx7StreamMethod->ItemIndex = SimpleDataSet->FieldByName("Value02")->AsInteger;
			if ( cbx7StreamMethod->ItemIndex == 0 )
				PageControlFlowStream->Visible = false;
			else
				PageControlFlowStream->Visible = true;

			//------------------------------------------------------------------
			// RFlowADVM
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT First 1 * FROM RFlowADVM WHERE PointCode = '" + FormMain->DBEnv.sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();

			AXadvm->Text            =  SimpleDataSet->FieldByName("AXadvm")->AsFloat;
			AZadvm->Text            =  SimpleDataSet->FieldByName("AZadvm")->AsFloat;
			APan->Text              =  SimpleDataSet->FieldByName("APan")->AsFloat;
			ATilt->Text             =  SimpleDataSet->FieldByName("ATilt")->AsFloat;
			APhi->Text              =  SimpleDataSet->FieldByName("APhi")->AsFloat;
			ASubmersion->Text       =  SimpleDataSet->FieldByName("ASubmersion")->AsFloat;
			AReverseMode->ItemIndex =  SimpleDataSet->FieldByName("AReverseMode")->AsInteger;
			AUnitVelocity->ItemIndex=  SimpleDataSet->FieldByName("AUnitVelocity")->AsInteger;
			AVMin->Text             =  SimpleDataSet->FieldByName("AVMin")->AsFloat;
			AVMax->Text             =  SimpleDataSet->FieldByName("AVMax")->AsFloat;
			AStartCell->Text        =  SimpleDataSet->FieldByName("AStartCell")->AsInteger;
			AEndCell->Text          =  SimpleDataSet->FieldByName("AEndCell")->AsInteger;

			// 하층 유속계
			BXadvm->Text            =  SimpleDataSet->FieldByName("BXadvm")->AsFloat;
			BZadvm->Text            =  SimpleDataSet->FieldByName("BZadvm")->AsFloat;
			BPan->Text              =  SimpleDataSet->FieldByName("BPan")->AsFloat;
			BTilt->Text             =  SimpleDataSet->FieldByName("BTilt")->AsFloat;
			BPhi->Text              =  SimpleDataSet->FieldByName("BPhi")->AsFloat;
			BSubmersion->Text       =  SimpleDataSet->FieldByName("BSubmersion")->AsFloat;
			BReverseMode->ItemIndex =  SimpleDataSet->FieldByName("BReverseMode")->AsInteger;
			BUnitVelocity->ItemIndex=  SimpleDataSet->FieldByName("BUnitVelocity")->AsInteger;
			BVMin->Text             =  SimpleDataSet->FieldByName("BVMin")->AsFloat;
			BVMax->Text             =  SimpleDataSet->FieldByName("BVMax")->AsFloat;
			BStartCell->Text        =  SimpleDataSet->FieldByName("BStartCell")->AsInteger;
			BEndCell->Text          =  SimpleDataSet->FieldByName("BEndCell")->AsInteger;
			BType->ItemIndex        = (SimpleDataSet->FieldByName("BType")->AsString == "UP") ? 0 :
									 ((SimpleDataSet->FieldByName("BType")->AsString == "LEFTRIGHT") ? 1 : 2);

			//------------------------------------------------------------------
			// RFlowHA
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT First 1 * FROM RFlowHA WHERE PointCode = '" + FormMain->DBEnv.sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();

			HAMinH->Text        =  SimpleDataSet->FieldByName("MinH")->AsFloat;
			HAMaxH->Text        =  SimpleDataSet->FieldByName("MaxH")->AsFloat;
			HAA6->Text          =  SimpleDataSet->FieldByName("A6")->AsFloat;
			HAA5->Text          =  SimpleDataSet->FieldByName("A5")->AsFloat;
			HAA4->Text          =  SimpleDataSet->FieldByName("A4")->AsFloat;
			HAA3->Text          =  SimpleDataSet->FieldByName("A3")->AsFloat;
			HAA2->Text          =  SimpleDataSet->FieldByName("A2")->AsFloat;
			HAA1->Text          =  SimpleDataSet->FieldByName("A1")->AsFloat;
			HAA0->Text          =  SimpleDataSet->FieldByName("A0")->AsFloat;

			//------------------------------------------------------------------
			// RFlowIVM
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT First 1 * FROM RFlowIVM WHERE PointCode = '" + FormMain->DBEnv.sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();

			IVMAMinVi->Text      = SimpleDataSet->FieldByName("AMinVi")->AsFloat;
			IVMAMaxVi->Text      = SimpleDataSet->FieldByName("AMaxVi")->AsFloat;
			IVMAA->Text          = SimpleDataSet->FieldByName("AA")->AsFloat;
			IVMAB->Text          = SimpleDataSet->FieldByName("AB")->AsFloat;
			IVMAC->Text          = SimpleDataSet->FieldByName("AC")->AsFloat;
			IVMBMinVi->Text      = SimpleDataSet->FieldByName("BMinVi")->AsFloat;
			IVMBMaxVi->Text      = SimpleDataSet->FieldByName("BMaxVi")->AsFloat;
			IVMBA->Text          = SimpleDataSet->FieldByName("BA")->AsFloat;
			IVMBB->Text          = SimpleDataSet->FieldByName("BB")->AsFloat;
			IVMBC->Text          = SimpleDataSet->FieldByName("BC")->AsFloat;

			break;

        case 7: //기타
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
			{
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '1'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = med2Interval->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = med2Period->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2StartTime->Text;
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

			//hydrometer1 저장 --------------------------------------------------------------------

			if ( cbx2H1Kind->ItemIndex == SL_SERIES_IDX || cbx2H1Kind->ItemIndex == AG_SERIES_IDX ) {	// SL/AG
				if ( ed2H1WN->Text.IsEmpty() || ed2H1WS->Text.IsEmpty() || ed2H1WF->Text.IsEmpty() ) {
					ShowMessage("SL/AG 장비는 셀의 갯수,크기등의 정보를 입력하여야 합니다.");
					return;
				}
			}
			//Accusonic Check => Accusonic use 19200bps
			if ( cbx2H1Kind->ItemIndex == ACCUSONIC_A_IDX || cbx2H1Kind->ItemIndex == ACCUSONIC_B_IDX ) {
				if ( cbx2H1CommSpeed->ItemIndex != BAUD_19200_IDX ) {	// 4=>5, None 추가해서.. 2026.03.11
					ShowMessage("Accusonic 장비는 19200bps로 설정하여야 합니다.");
					return;
				}
			}
			// RQ30 Check
			if ( cbx2H1Kind->ItemIndex == RQ_30_IDX ) {
				if ( cbx2H1AirmarPort->ItemIndex > 0 ) { 	// Airmar를 함께 운영한다면
					// RnsWater의 Airmar 지원 여부 체크
					if (cbx2WLKind->ItemIndex == RNSWATER_IDX && cbx2WaterAirmar->Checked ) {	// 수위계가 RnsWater로 설정되어 있고, 기상 센서를 수용한다면
						ShowMessage("Airmar 설정이 수위계(RnsWater)와 겹칩니다. 한 곳에서만 Airmar를 수용 바랍니다.");
						return;
					}
				}
//				if ( cbx2H1AirmarPort->ItemIndex == -1 ||  cbx2H1AirmarPort->ItemIndex == 0 ) { // not select or NONE
//					if ( Application->MessageBox(L"RQ-30은 Airmar 장비와 함께 운영 됩니다. 계속할까요?", L"Airmar 설정", MB_YESNO) != IDYES ) return;
//				}
//				if ( cbx2H1AirmarBaud->ItemIndex != 1 ) {
//					if ( Application->MessageBox(L"Airmar 장비는 4800bps가 기본입니다. 계속할까요?", L"Airmar 설정", MB_YESNO) != IDYES ) return;
//				}
			}
			// RQ-30d Check
			if ( cbx2H1Kind->ItemIndex == RQ_30D_IDX ) {	// RQ-30d
//				if ( cbx2H1AirmarPort->ItemIndex == -1 ||  cbx2H1AirmarPort->ItemIndex == 0 ) { // not select or NONE
//					if ( Application->MessageBox(L"RQ-30은 Airmar 장비와 함께 운영 됩니다. 계속할까요?", L"Airmar 설정", MB_YESNO) != IDYES ) return;
//				}
				if ( cbx2H1AirmarPort->ItemIndex > 0 ) { 	// Airmar를 함께 운영한다면
					// RnsWater의 Airmar 지원 여부 체크
					if (cbx2WLKind->ItemIndex == RNSWATER_IDX && cbx2WaterAirmar->Checked ) {	// 수위계가 RnsWater로 설정되어 있고, 기상 센서를 수용한다면
						ShowMessage("Airmar 설정이 수위계(RnsWater)와 겹칩니다. 한 곳에서만 Airmar를 수용 바랍니다.");
						return;
					}
					if (cbAirmarLinker->Checked ) {
						if ( cbx2H1AirmarBaud->ItemIndex != BAUD_9600_IDX ) {	// AirmarLiner => 9600bps통신이 기본임.     2=>3
							ShowMessage("AirmarLinker는 9600bps로 통신합니다.");
							return;
						} else {}
					} else {  	// AirmarLinker를 쓰지 않는다면
						if ( cbx2H1AirmarBaud->ItemIndex != BAUD_4800_IDX ) {
							if ( Application->MessageBox(L"Airmar 장비는 4800bps가 기본입니다. 계속할까요?", L"Airmar 설정", MB_YESNO) != IDYES ) return;
						}
					}
					if ( cbxSlaveCount->ItemIndex == -1 || cbxSlaveCount->ItemIndex == 0 ) {
						if ( Application->MessageBox(L"RQ-30d 장비는 Slave(RG-30d)와 함께 운용됩니다. 계속할까요?. ", L"RQ-30d", MB_YESNO) != IDYES ) return;
					}
				}// end if
				// SlaveCount check
				int SlaveCount = cbxSlaveCount->ItemIndex;
				int MasterDeviceCount = HowManyDeviceAddress(edHydro1MasterAddress->Text);
				int SlaveDeviceCount  = HowManyDeviceAddress(edHydro1SlaveAddress->Text);
				int DeviceCount = 0;
				if ( cbxHydro1UseSlavePort->Checked) DeviceCount = MasterDeviceCount + SlaveDeviceCount;
				else                                 DeviceCount = MasterDeviceCount;
				if ( DeviceCount != (SlaveCount+1)) {
					ShowMessage("입력된 장비의 Address 수와 설정된 장비의 수가 일치하지 않습니다.\r\n(MasterPort Device + SlavePort Device == SlaveCount+1)");
					return;
				}
			}
			// data save
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '2'";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			UnicodeString Hydro1Type = cbx2H1Kind->Text;
			if ( cbx2H1Kind->Text == NONE_STR )               Hydro1Type = CsNone;
			else if ( cbx2H1Kind->Text == CHANNELMASTER_STR ) Hydro1Type = CsChannelMaster;
			else if ( cbx2H1Kind->Text == AGSERIES_STR )      Hydro1Type = CsAGSeries;
			else if ( cbx2H1Kind->Text == SLSERIES_STR )      Hydro1Type = CsSLSeries;
			else if ( cbx2H1Kind->Text == ACCUSONIC_A_STR )   Hydro1Type = CsAccusonicA;
			else if ( cbx2H1Kind->Text == ACCUSONIC_B_STR )   Hydro1Type = CsAccusonicB;
			else if ( cbx2H1Kind->Text == "RQ-30" )           Hydro1Type = CsRQ30;
			else if ( cbx2H1Kind->Text == "RQ-30d")           Hydro1Type = CsRQ30D;
			else                                              Hydro1Type = CsNone;
			SimpleDataSet->FieldByName("VALUE01")->AsString = (cbx2H1Kind->Text == "RQ-30") ? CsRQ30 : ((cbx2H1Kind->Text == "RQ-30d") ? CsRQ30D : cbx2H1Kind->Text);
			SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2H1Comport->Text;
			SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2H1CommSpeed->Text;
			SimpleDataSet->FieldByName("VALUE04")->AsString = led2H1Offset->Text;
			if ( Hydro1Type == CsAccusonicA || Hydro1Type == CsAccusonicB ) {
				SimpleDataSet->FieldByName("VALUE07")->AsString = ed2H1WN->Text;  //
				SimpleDataSet->FieldByName("VALUE08")->AsString = ed2H1WS->Text;  //
				SimpleDataSet->FieldByName("VALUE09")->AsString = ed2H1WF->Text;  //
			}
			SimpleDataSet->FieldByName("VALUE10")->AsString = cbx2H1ReverseOption->Text;
			SimpleDataSet->FieldByName("VALUE11")->AsString = led2Hz->Text;	// Frequencey
			SimpleDataSet->FieldByName("VALUE12")->AsString = led2SN->Text;	// SN
			//SimpleDataSet->FieldByName("VALUE13")->AsString = (cbxH1SlaveCount->ItemIndex == -1) ? 0 : cbxH1SlaveCount->ItemIndex;	// RQ-30d Slave Count
			SimpleDataSet->FieldByName("VALUE14")->AsString = (cbx2H1IdleCheck->ItemIndex == -1 || cbx2H1IdleCheck->ItemIndex == 0) ? "ON" : "OFF";
			SimpleDataSet->FieldByName("VALUE15")->AsString = (cbx2H1IdleCheckTime->ItemIndex == -1 ) ? UnicodeString("3") : IntToStr(cbx2H1IdleCheckTime->ItemIndex + 1);	// default: 3 hours, or 1 ~ 6 hours
			//if ( cbx2H1AirmarPort->ItemIndex > 0 ) {	// Airmar 가 연결되어 있다면
			SimpleDataSet->FieldByName("VALUE16")->AsString = cbx2H1AirmarPort->Text;
			SimpleDataSet->FieldByName("VALUE17")->AsString = cbx2H1AirmarBaud->Text;
			SimpleDataSet->FieldByName("VALUE20")->AsString = (cbAirmarLinker->Checked) ? "ON" : "OFF";
			//} else {
			//	SimpleDataSet->FieldByName("VALUE16")->AsString = "";
			//	SimpleDataSet->FieldByName("VALUE17")->AsString = "";
			//	SimpleDataSet->FieldByName("VALUE20")->AsString = "OFF";
			//}
			// heaven2 2022.03.18
			SimpleDataSet->FieldByName("VALUE18")->AsString = (led2StartCell->Text.IsEmpty()) ? UnicodeString("1") : led2StartCell->Text;
			SimpleDataSet->FieldByName("VALUE19")->AsString = (led2EndCell->Text.IsEmpty()) ? UnicodeString("10") : led2EndCell->Text;
			// heaven2 2024.10.12
			if ( Hydro1Type == CsRQ30D ) {
				UnicodeString Value05, Value06, Value07, Value08, Value09, Value13;
				Value05 = edHydro1MasterAddress->Text;	// Master Port Device Address
				Value06 = (cbxHydro1UseSlavePort->Checked) ? "ON" : "OFF";
				Value07 = cbHydro1SlavePort->Text;	// COM01,COM02,..
				Value08 = cbHydro1SlaveBaudRate->Text;	// 2400,4800,9600,..
				Value09 = edHydro1SlaveAddress->Text;	// ""/6,7,8,9,10
				Value13 = cbxSlaveCount->ItemIndex;	// Slave Count
				SimpleDataSet->FieldByName("VALUE05")->AsString = Value05;	// Master Port Address (ex) 5,4,3,2,1
				SimpleDataSet->FieldByName("VALUE06")->AsString = Value06;	// Use Slave Port ON/OFF
				SimpleDataSet->FieldByName("VALUE07")->AsString = Value07;	// Slave Port
				SimpleDataSet->FieldByName("VALUE08")->AsString = Value08;	// Slave Port Baudrate
				SimpleDataSet->FieldByName("VALUE09")->AsString = Value09;	// Slave Port Address
				SimpleDataSet->FieldByName("VALUE13")->AsString = Value13;	// Slave Count
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

			//hydrometer2 저장 ----------------------------------------------------------

			if ( cbx2H2Kind->ItemIndex == SL_SERIES_IDX || cbx2H2Kind->ItemIndex == AG_SERIES_IDX ) {	// SL/AG
				if ( ed2H2WN->Text.IsEmpty() || ed2H2WS->Text.IsEmpty() || ed2H2WF->Text.IsEmpty() ) {
					ShowMessage("SL/AG 장비는 셀의 갯수,크기등의 정보를 입력하여야 합니다.");
					return;
				}
			}
			//Accusonic Check => Accusonic use 19200bps
			if ( cbx2H2Kind->ItemIndex == ACCUSONIC_A_IDX || cbx2H2Kind->ItemIndex == ACCUSONIC_B_IDX ) {
				if ( cbx2H2CommSpeed->ItemIndex != BAUD_19200_IDX ) {	// 4 => 5
					ShowMessage("Accusonic 장비는 19200bps로 설정하여야 합니다.");
					return;
				}
			}
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '3'";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2H2Kind->Text;
			SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2H2Comport->Text;
			SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2H2CommSpeed->Text;
			SimpleDataSet->FieldByName("VALUE04")->AsString = led2H2Offset->Text;
			SimpleDataSet->FieldByName("VALUE07")->AsString = ed2H2WN->Text;
			SimpleDataSet->FieldByName("VALUE08")->AsString = ed2H2WS->Text;
			SimpleDataSet->FieldByName("VALUE09")->AsString = ed2H2WF->Text;
			SimpleDataSet->FieldByName("VALUE10")->AsString = cbx2H2ReverseOption->Text;
			SimpleDataSet->FieldByName("VALUE11")->AsString = cbx2H2UpSideOption->Text;
			SimpleDataSet->FieldByName("VALUE12")->AsString = led2Hz2->Text;	// Frequencey
			SimpleDataSet->FieldByName("VALUE13")->AsString = led2SN2->Text;	// SN
			SimpleDataSet->FieldByName("VALUE14")->AsString = (cbx2H2IdleCheck->ItemIndex == -1 || cbx2H2IdleCheck->ItemIndex == 0) ? "ON" : "OFF";
			SimpleDataSet->FieldByName("VALUE15")->AsString = (cbx2H2IdleCheckTime->ItemIndex == -1 ) ? UnicodeString("3") : IntToStr(cbx2H2IdleCheckTime->ItemIndex + 1);	// default: 3 hours, or 1 ~ 6 hours
			// heaven2 2022.03.18
			SimpleDataSet->FieldByName("VALUE18")->AsString = (led2StartCell2->Text.IsEmpty()) ? UnicodeString("1") : led2StartCell2->Text;
			SimpleDataSet->FieldByName("VALUE19")->AsString = (led2EndCell2->Text.IsEmpty()) ? UnicodeString("10") : led2EndCell2->Text;

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

			//hydrometer3 저장 ----------------------------------------------------------

			if ( cbx2H3Kind->ItemIndex == SL_SERIES_IDX || cbx2H3Kind->ItemIndex == AG_SERIES_IDX ) {	// SL/AG
				if ( ed2H3WN->Text.IsEmpty() || ed2H3WS->Text.IsEmpty() || ed2H3WF->Text.IsEmpty() ) {
					ShowMessage("SL/AG 장비는 셀의 갯수,크기등의 정보를 입력하여야 합니다.");
					return;
				}
			}
			//Accusonic Check => Accusonic use 19200bps
			if ( cbx2H3Kind->ItemIndex == ACCUSONIC_A_IDX || cbx2H3Kind->ItemIndex == ACCUSONIC_B_IDX ) {
				if ( cbx2H3CommSpeed->ItemIndex != BAUD_19200_IDX ) {
					ShowMessage("Accusonic 장비는 19200bps로 설정하여야 합니다.");
					return;
				}
			}
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '5'";	// PK1=10, PK2=5
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2H3Kind->Text;
			SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2H3Comport->Text;
			SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2H3CommSpeed->Text;
			SimpleDataSet->FieldByName("VALUE04")->AsString = led2H3Offset->Text;
			SimpleDataSet->FieldByName("VALUE07")->AsString = ed2H3WN->Text;
			SimpleDataSet->FieldByName("VALUE08")->AsString = ed2H3WS->Text;
			SimpleDataSet->FieldByName("VALUE09")->AsString = ed2H3WF->Text;
			SimpleDataSet->FieldByName("VALUE10")->AsString = cbx2H3ReverseOption->Text;
			SimpleDataSet->FieldByName("VALUE11")->AsString = cbx2H3UpSideOption->Text;
			SimpleDataSet->FieldByName("VALUE12")->AsString = led2Hz3->Text;	// Frequencey
			SimpleDataSet->FieldByName("VALUE13")->AsString = led2SN3->Text;	// SN
			SimpleDataSet->FieldByName("VALUE14")->AsString = (cbx2H3IdleCheck->ItemIndex == -1 || cbx2H3IdleCheck->ItemIndex == 0) ? "ON" : "OFF";
			SimpleDataSet->FieldByName("VALUE15")->AsString = (cbx2H3IdleCheckTime->ItemIndex == -1 ) ? UnicodeString("3") : IntToStr(cbx2H3IdleCheckTime->ItemIndex + 1);	// default: 3 hours, or 1 ~ 6 hours
			// heaven2 2022.03.18
			SimpleDataSet->FieldByName("VALUE18")->AsString = (led2StartCell3->Text.IsEmpty()) ? UnicodeString("1") : led2StartCell3->Text;
			SimpleDataSet->FieldByName("VALUE19")->AsString = (led2EndCell3->Text.IsEmpty()) ? UnicodeString("10") : led2EndCell3->Text;

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

			//waterlevel 저장 ------------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '10' AND PK2 = '4'";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			SimpleDataSet->FieldByName("VALUE01")->AsString = cbx2WLKind->Text;
			if ( cbx2WLKind->Text.Pos(LT400_STR) > 0 )
				SimpleDataSet->FieldByName("VALUE01")->AsString = LT400_STR;
			SimpleDataSet->FieldByName("VALUE02")->AsString = cbx2WLComport->Text;
			SimpleDataSet->FieldByName("VALUE03")->AsString = cbx2WLCommSpeed->Text;
			SimpleDataSet->FieldByName("VALUE04")->AsString = ((led2WLOffset->Text).IsEmpty()) ? UnicodeString("0") : led2WLOffset->Text;
			SimpleDataSet->FieldByName("VALUE06")->AsString = led2WLSN->Text;
			UnicodeString AtmosPort = cbx2AtmosPort->Text;
			if ( AtmosPort.IsEmpty() ) AtmosPort = "NONE";
			m_AtmosPort = AtmosPort;
			SimpleDataSet->FieldByName("VALUE07")->AsString = AtmosPort;
			SimpleDataSet->FieldByName("VALUE08")->AsString = edBaseAtmos->Text;
			SimpleDataSet->FieldByName("VALUE09")->AsString = (cbx2WLIdleCheck->ItemIndex == -1 || cbx2WLIdleCheck->ItemIndex == 0) ? "ON" : "OFF";
			SimpleDataSet->FieldByName("VALUE10")->AsString = (cbx2WLIdleCheckTime->ItemIndex == -1) ? UnicodeString("3") : IntToStr(cbx2WLIdleCheckTime->ItemIndex + 1); // 3 hours, or  others ( 1 ~ 6 hours)
			SimpleDataSet->FieldByName("VALUE11")->AsString = (cbx2WaterAtmos->Checked) ? "ON" : "OFF";	// 대기압 보정 유무
			SimpleDataSet->FieldByName("VALUE12")->AsString = (cbx2WaterAirmar->Checked) ? "ON" : "OFF";// 기상센서 수집 유무
			SimpleDataSet->FieldByName("VALUE13")->AsString = (cbx2WaterUseFlag->ItemIndex == 0) ? HYDROMETER_STR : WATERLEVEL_STR;// 기상센서 수집 유무
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

		case 2: //sender
			{
			if ( cbx3FtpFileFormat->ItemIndex == 1 ) {	// UVM(Accusonic)
//				if ((cbx2H1Kind->ItemIndex != 4 && cbx2H1Kind->ItemIndex != 5 ) ||
//					 cbx2H2Kind->ItemIndex != 4 && cbx2H2Kind->ItemIndex != 5 ) {
					ShowMessage("UVM(Accusonic) 형식은 Accusonic 장비에 한합니다.");
//					return;
//				}
			}
			if ( cbx3FtpFileFormat->ItemIndex == 2 ) {	// UVM(Hydrovision)
				ShowMessage("지원하지 않는 형식입니다.");
				return;
			}
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
            SimpleDataSet->FieldByName("VALUE05")->AsString = cbx3VTHComport->Text;
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
			SimpleDataSet->FieldByName("VALUE16")->AsString = led4LineIF->Text;
            SimpleDataSet->FieldByName("VALUE17")->AsString = led4WirelessIF->Text;
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
            //그래프 옵션
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = lel5Ment->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = led5LevelOverSea->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = led5CctvUrl->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = cbx5LoginDelayTime->Text;
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
            //점검일지 email관련 수신자 정보; 없으면 디폴트 값으로 로딩
            if (ledEmailServer->Text   == "") ledEmailServer->Text   = CsDefaultEmailServerAddress;
            if (ledEmailAccount->Text  == "") ledEmailAccount->Text  = CsDefaultEmailAccount;
            if (ledEmailPassword->Text == "") ledEmailPassword->Text = CsDefaultEmailPassword;
            if (ledEmailDest1->Text    == "") ledEmailDest1->Text    = CsDefaultEmailDest1;
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '5'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = cbxEmailAutoSend->Text;
            SimpleDataSet->FieldByName("VALUE02")->AsString = ledEmailServer->Text;
            SimpleDataSet->FieldByName("VALUE03")->AsString = ledEmailAccount->Text;
            SimpleDataSet->FieldByName("VALUE04")->AsString = ledEmailPassword->Text;
            SimpleDataSet->FieldByName("VALUE05")->AsString = ledEmailDest1->Text;
            SimpleDataSet->FieldByName("VALUE06")->AsString = ledEmailDest2->Text;
            SimpleDataSet->FieldByName("VALUE07")->AsString = ledEmailDest3->Text;
            SimpleDataSet->FieldByName("VALUE08")->AsString = ledEmailDest4->Text;
            SimpleDataSet->FieldByName("VALUE09")->AsString = ledEmailDest5->Text;
            FormMain->DBEnv.bEmailAutoSend      = cbxEmailAutoSend->ItemIndex == 0; //자동전송 유무
            FormMain->DBEnv.sEmailServerAddress = ledEmailServer->Text;             //smtp 메일서버 주소
            FormMain->DBEnv.sEmailAccount       = ledEmailAccount->Text;            //id
            FormMain->DBEnv.sEmailPassword      = ledEmailPassword->Text;           //pw
            FormMain->DBEnv.sEmailDest1         = ledEmailDest1->Text;              //수신자1 -> 자신 rads@rnsea.com
            FormMain->DBEnv.sEmailDest2         = ledEmailDest2->Text;              //수신자2
            FormMain->DBEnv.sEmailDest3         = ledEmailDest3->Text;              //수신자3
            FormMain->DBEnv.sEmailDest4         = ledEmailDest4->Text;              //수신자4
            FormMain->DBEnv.sEmailDest5         = ledEmailDest5->Text;              //수신자5
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
            //그래프관련
            if (!cbxWaterLevel->Checked) cbxWaterLevel->Checked = True; //checked으로 고정
            if (!cbxHydroMeter1->Checked) cbxHydroMeter1->Checked = True; //checked으로 고정
            SimpleDataSet->Close();
            SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '4'";
            SimpleDataSet->Open();
            SimpleDataSet->Edit();
            SimpleDataSet->FieldByName("VALUE01")->AsString = (cbxWaterFlow->Checked  ) ? "ON" : "OFF";
            SimpleDataSet->FieldByName("VALUE02")->AsString = (cbxWaterLevel->Checked ) ? "ON" : "OFF";
            SimpleDataSet->FieldByName("VALUE03")->AsString = (cbxHydroMeter1->Checked) ? "ON" : "OFF";
            SimpleDataSet->FieldByName("VALUE04")->AsString = (cbxLog->Checked        ) ? "ON" : "OFF";
            SimpleDataSet->FieldByName("VALUE05")->AsString = (cbxVthLogger->Checked  ) ? "ON" : "OFF";
            FormMain->DBEnv.bMDIWaterFlow   = cbxWaterFlow->Checked;   //유량산출
            FormMain->DBEnv.bMDIWaterLevel  = cbxWaterLevel->Checked;  //수위계
            FormMain->DBEnv.bMDIHydroMeter1 = cbxHydroMeter1->Checked; //상층유속계
            FormMain->DBEnv.bMDILog         = cbxLog->Checked;         //로그창
            FormMain->DBEnv.bMDIVth         = cbxVthLogger->Checked;   //vth logger
            FormMain->DBEnv.sCctvUrl        = led5CctvUrl->Text; //cctv url
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

		case 6: //자동유량
			{
			UnicodeString sPointCode;
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '1'";
			SimpleDataSet->Open();
			sPointCode = SimpleDataSet->FieldByName("Value01")->AsString;

			//------------------------------------------------------------------
			// 유량산출방식
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT * FROM RSETUP WHERE PK1 = '1' AND PK2 = '3'";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			SimpleDataSet->FieldByName("Value02")->AsInteger = cbx7StreamMethod->ItemIndex;

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

			//------------------------------------------------------------------
			// RFlowADVM
			//------------------------------------------------------------------
			try {
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT FIRST 1  * FROM RFlowADVM WHERE PointCode = '" + sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			//SimpleDataSet->FieldByName("PointCode")->AsString      = cbx2StreamMethod->Text.SubString(1, 1);
			//SimpleDataSet->FieldByName("UseDate")->AsString        = cbx2StreamMethod->Text.SubString(1, 1);
			SimpleDataSet->FieldByName("AXadvm")->AsFloat          = StrToFloat(AXadvm->Text);
			SimpleDataSet->FieldByName("AZadvm")->AsFloat          = StrToFloat(AZadvm->Text);
			SimpleDataSet->FieldByName("APan")->AsFloat            = StrToFloat(APan->Text);
			SimpleDataSet->FieldByName("ATilt")->AsFloat           = StrToFloat(ATilt->Text);
			SimpleDataSet->FieldByName("APhi")->AsFloat            = StrToFloat(APhi->Text);
			SimpleDataSet->FieldByName("ASubmersion")->AsFloat     = StrToFloat(ASubmersion->Text);
			SimpleDataSet->FieldByName("AReverseMode")->AsInteger  = AReverseMode->ItemIndex;	// 0: ON, 1: OFF
			SimpleDataSet->FieldByName("AUnitVelocity")->AsInteger = AUnitVelocity->ItemIndex;	// 0: mm/s, 1:cm/s, 2: m/s
			SimpleDataSet->FieldByName("AVMin")->AsFloat           = StrToFloat(AVMin->Text);
			SimpleDataSet->FieldByName("AVMax")->AsFloat           = StrToFloat(AVMax->Text);
			SimpleDataSet->FieldByName("AStartCell")->AsInteger    = StrToInt(AStartCell->Text);
			SimpleDataSet->FieldByName("AEndCell")->AsInteger      = StrToInt(AEndCell->Text);

			SimpleDataSet->FieldByName("BXadvm")->AsFloat          = StrToFloat(BXadvm->Text);
			SimpleDataSet->FieldByName("BZadvm")->AsFloat          = StrToFloat(BZadvm->Text);
			SimpleDataSet->FieldByName("BPan")->AsFloat            = StrToFloat(BPan->Text);
			SimpleDataSet->FieldByName("BTilt")->AsFloat           = StrToFloat(BTilt->Text);
			SimpleDataSet->FieldByName("BPhi")->AsFloat            = StrToFloat(BPhi->Text);
			SimpleDataSet->FieldByName("BSubmersion")->AsFloat     = StrToFloat(BSubmersion->Text);
			SimpleDataSet->FieldByName("BReverseMode")->AsInteger  = BReverseMode->ItemIndex;	// 0: ON, 1: OFF
			SimpleDataSet->FieldByName("BUnitVelocity")->AsInteger = BUnitVelocity->ItemIndex;	// 0: mm/s, 1:cm/s, 2: m/s
			SimpleDataSet->FieldByName("BVMin")->AsFloat           = StrToFloat(BVMin->Text);
			SimpleDataSet->FieldByName("BVMax")->AsFloat           = StrToFloat(BVMax->Text);
			SimpleDataSet->FieldByName("BStartCell")->AsInteger    = StrToInt(BStartCell->Text);
			SimpleDataSet->FieldByName("BEndCell")->AsInteger      = StrToInt(BEndCell->Text);
			SimpleDataSet->FieldByName("BType")->AsString          =  (BType->ItemIndex == 0) ? "UP" :
																	 ((BType->ItemIndex == 1 ) ? "LEFTRIGHT" : "SOUTHNORTH");
			} catch ( Exception &e ) {
				ShowMessage("입력자료가 올바르지 않습니다. 입력 자료를 확인하여 주십시오.\n"
							"실수자료와 정수형 자료의 혼동이 있을 수 있습니다.");
				return;
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

			//------------------------------------------------------------------
			// RFlowHA
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT FIRST 1  * FROM RFlowHA WHERE PointCode = '" + sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			//SimpleDataSet->FieldByName("PointCode")->AsString      = cbx2StreamMethod->Text.SubString(1, 1);
			//SimpleDataSet->FieldByName("UseDate")->AsString        = cbx2StreamMethod->Text.SubString(1, 1);
			SimpleDataSet->FieldByName("MinH")->AsFloat   = StrToFloat(HAMinH->Text);
			SimpleDataSet->FieldByName("MaxH")->AsFloat   = StrToFloat(HAMaxH->Text);
			SimpleDataSet->FieldByName("A6")->AsFloat     = StrToFloat(HAA6->Text);
			SimpleDataSet->FieldByName("A5")->AsFloat     = StrToFloat(HAA5->Text);
			SimpleDataSet->FieldByName("A4")->AsFloat     = StrToFloat(HAA4->Text);
			SimpleDataSet->FieldByName("A3")->AsFloat     = StrToFloat(HAA3->Text);
			SimpleDataSet->FieldByName("A2")->AsFloat     = StrToFloat(HAA2->Text);
			SimpleDataSet->FieldByName("A1")->AsFloat     = StrToFloat(HAA1->Text);
			SimpleDataSet->FieldByName("A0")->AsFloat     = StrToFloat(HAA0->Text);
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

			//------------------------------------------------------------------
			// RFlowIVM
			//------------------------------------------------------------------
			SimpleDataSet->Close();
			SimpleDataSet->DataSet->CommandText = "SELECT FIRST 1  * FROM RFlowIVM WHERE PointCode = '" + sPointCode + "' ORDER BY UseDate DESC ";
			SimpleDataSet->Open();
			SimpleDataSet->Edit();
			//SimpleDataSet->FieldByName("PointCode")->AsString      = cbx2StreamMethod->Text.SubString(1, 1);
			//SimpleDataSet->FieldByName("UseDate")->AsString        = cbx2StreamMethod->Text.SubString(1, 1);
			SimpleDataSet->FieldByName("AMinVi")->AsFloat    = StrToFloat(IVMAMinVi->Text);
			SimpleDataSet->FieldByName("AMaxVi")->AsFloat    = StrToFloat(IVMAMaxVi->Text);
			SimpleDataSet->FieldByName("AA")->AsFloat        = StrToFloat(IVMAA->Text);
			SimpleDataSet->FieldByName("AB")->AsFloat        = StrToFloat(IVMAB->Text);
			SimpleDataSet->FieldByName("AC")->AsFloat        = StrToFloat(IVMAC->Text);
			SimpleDataSet->FieldByName("BMinVi")->AsFloat    = StrToFloat(IVMBMinVi->Text);
			SimpleDataSet->FieldByName("BMaxVi")->AsFloat    = StrToFloat(IVMBMaxVi->Text);
			SimpleDataSet->FieldByName("BA")->AsFloat        = StrToFloat(IVMBA->Text);
			SimpleDataSet->FieldByName("BB")->AsFloat        = StrToFloat(IVMBB->Text);
			SimpleDataSet->FieldByName("BC")->AsFloat        = StrToFloat(IVMBC->Text);

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
			} // end of first {
			break;

        case 7: //기타
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

	//신호전송
	// heaven2 2014.06.26 일단 막음. => 제원의 변경 사항을 Sender/Logger에 명시적으로 알려주는 것이나,
	// Logger/Sender가 실행된 지 오래되었을 경우 이 메세지를 받고 Hangup되는 증상이 있어 일단 주석 처리함.
	// (실행된 지 얼마되지 않아서(10분~1시간) 메세지를 전달하면 잘 되나, 며칠이 지나고 하면 Hangup 됨
	// Resource를 모두 내리고 다시 올리는 중에 해당 Resource에 접그하는 등에 대한 문제때문으로 추정
	// 일단 위험한 로직이라 주석으로 처리함.
	switch(pcConfig->ActivePageIndex)
	{
		case 0:	// 지점정보  (Logger/Sender)
			m_bLoggerNotify = true;
			m_bSenderNotify = true;
			break;
		case 1:	// Logger    (Logger)
			m_bLoggerNotify = true;
			m_bSenderNotify = true;		// heaven2 2016.04.26 => WaterLevel Offset 정보 변경하면 DataSender에 반영되도록 한다.
			break;
		case 2:	// Sender    (Sender)
			//m_bLoggerNotify = true;
			m_bSenderNotify = true;
			break;
		case 3:	// guardnet  (Sender)
			//m_bLoggerNotify = true;
			m_bSenderNotify = true;
			break;
		case 4: // Viewer    ()
			//m_bLoggerNotify = true;
			//m_bSenderNotify = true;
			break;
		case 5:	// 유효범위  (Sender)
			//m_bLoggerNotify = true;
			m_bSenderNotify = true;
			break;
		case 6: // 유량산출  (Logger)
			m_bLoggerNotify = true;
			//m_bSenderNotify = true;
			break;
		case 7: // 기타(     (Logger/Sender)
			m_bLoggerNotify = true;
			m_bSenderNotify = true;
			break;

//		case 0: //지점정보
//			m_bLoggerNotify = true;
//			m_bSenderNotify = true;
//		case 7: //기타
//			//FormMain->SendMessageToServer(CcLoggerSetupChange); //로거에 신호전송
//			//FormMain->SendMessageToServer(CcSenderSetupChange); //센더에 신호전송
//			break;
//		case 1: //data logger
//			//FormMain->SendMessageToServer(CcLoggerSetupChange); //로거에 신호전송
//			//FormMain->SendMessageToServer(CcSenderSetupChange); //센더에 신호전송
//			break;
//		case 2: //data sender
//		case 3: //guard net
//		case 5: //유효범위
//			//FormMain->SendMessageToServer(CcSenderSetupChange); //센더에 신호전송
//			break;
//		case 6: //자동유량
//			//FormMain->SendMessageToServer(CcLoggerSetupChange); //로거에 신호전송
//			break;
	} //switch

	bSaved = true;
	ShowMessage("저장되었습니다.");
}
//---------------------------------------------------------------------------
void __fastcall TFormConfig::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TFormConfig::cbx7StreamMethodChange(TObject *Sender)
{
	if ( cbx7StreamMethod->ItemIndex == 0 ) {
		PageControlFlowStream->Visible = false;
	} else {
		PageControlFlowStream->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormConfig::cbx2WLKindChange(TObject *Sender)
{
	// 장비 없음을 선택했을 때
	if ( cbx2WLKind->ItemIndex == NONE_IDX ) {
		cbx2WLComport->ItemIndex = NONE_IDX;
		cbx2WLCommSpeed->ItemIndex = NONE_IDX;
	}

	if ( cbx2WLKind->ItemIndex == LT400_ATMOS_IDX || cbx2WLKind->ItemIndex == RNSWATER_IDX ) {	// LT400+Atmos / RnsWater
		lb2WaterAtmos->Visible   = true;   cbx2WaterAtmos->Visible  = true;
		lbAtmosCaption->Visible  = true;   cbx2AtmosPort->Visible   = true;											   // 대기압포트
										   cbx2AtmosPort->ItemIndex = cbx2AtmosPort->Items->IndexOf(m_AtmosPort);      // 대기압 포트
										   edBaseAtmos->Visible     = true;                                            // 기준 대기압
	} else { // 그 외 장비 => 대기압 보정 없음
		lb2WaterAtmos->Visible   = false;   cbx2WaterAtmos->Visible = false;
		lbAtmosCaption->Visible  = false;  cbx2AtmosPort->Visible   = false;
										   cbx2AtmosPort->ItemIndex = 0;	// NONE
										   edBaseAtmos->Visible     = false;
	}

	// RnsWater일 때 Airmar 데이터 수용 여부
	if ( cbx2WLKind->ItemIndex == RNSWATER_IDX ) {	  // Type => 5: RnsWater Type => Airmar Enable/Disable
		lb2WaterAirmar->Visible  = true;  // (기상센서) Enable/Disable Caption
		cbx2WaterAirmar->Visible = true;  // (기상센서) Enable/Disable
	} else {
		lb2WaterAirmar->Visible  = false; // (기상센서) Enable/Disable Caption
		cbx2WaterAirmar->Visible = false; // (기상센서) Enable/Disable
	}

	if ( cbx2WLKind->ItemIndex == RNSWATER_IDX) {	// Sensor Type => 5 (RnsWater) => AirmarLinker 설정 중복 여부
		// AirmarLinker 설정확인
		if ( cbx2H1AirmarPort->ItemIndex > 0 ) { 	// 유속계1의 기상센서 설정 정보가 있다면.
			if ( Application->MessageBox(L"RnsWater는 AirmarLinker의 기능을 포함합니다. AirmarLinker의 설정은 무시됩니다.",
										 L"AirmarLinker경고", MB_YESNO) == IDYES ) {
				// AirmarLinker 옵션 무시 진행 ok

			} else {
				// AirmarLinker 옵션츨 처리
				return;
			}
		} else {
			// noting to do..
		}
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

void __fastcall TFormConfig::cbxHydro1UseSlavePortClick(TObject *Sender)
{
	if ( cbxHydro1UseSlavePort->Checked ) {	// Slave Port를 사용함
		cbHydro1SlavePort->Visible     = true;
		cbHydro1SlaveBaudRate->Visible = true;
		edHydro1SlaveAddress->Visible  = true;
		lbHydro1SlavePort->Visible     = true;
		lbHydro1SlaveBaudRate->Visible = true;
		lbHydro1SlaveAddress->Visible  = true;
	} else {								// Slave Port 사용하지 않음 => Master Port에서 모두 처리
		cbHydro1SlavePort->Visible     = false;
		cbHydro1SlaveBaudRate->Visible = false;
		edHydro1SlaveAddress->Visible  = false;
		lbHydro1SlavePort->Visible     = false;
		lbHydro1SlaveBaudRate->Visible = false;
		lbHydro1SlaveAddress->Visible  = false;
	}
}

//---------------------------------------------------------------------------
// 대기압 Check Box
void __fastcall TFormConfig::cbx2WaterAtmosClick(TObject *Sender)
{
	if ( cbx2WaterAtmos->Enabled ) {
		// do something
	} else {
		// do somethingg
	}
}

//---------------------------------------------------------------------------
// 기상센서 Check Box
void __fastcall TFormConfig::cbx2WaterAirmarClick(TObject *Sender)
{
	if ( cbx2WaterAirmar->Enabled ) {
		// do something
	} else {
		// do something
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormConfig::cbx2H3KindChange(TObject *Sender)
{
	if ( cbx2H3Kind->ItemIndex == NONE_IDX ) {   	// NONE => 장비 없음 선택 시
		cbx2H3Comport->ItemIndex  = NONE_IDX;
		cbx2H3CommSpeed->ItemIndex = NONE_IDX;
	}

	if ( cbx2H3Kind->ItemIndex == SL_SERIES_IDX || cbx2H3Kind->ItemIndex == AG_SERIES_IDX ) {	// SL Series / AG Series   => SL/AG Option Show
		Panel3->Visible = true;
	} else {	// SL/AG Option Hide
		Panel3->Visible = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormConfig::cbx2H1AirmarPortChange(TObject *Sender)
{
	// Airmar를 NONE으로 설정했을 때
	if ( cbx2H1AirmarPort->ItemIndex == NONE_IDX ) {
		cbx2H1AirmarBaud->ItemIndex = NONE_IDX;
		cbAirmarLinker->Checked = false;
	}
}
//---------------------------------------------------------------------------

