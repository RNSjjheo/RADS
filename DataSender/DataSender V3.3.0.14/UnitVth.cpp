//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitVth.h"
//#include "LockeCommon.h"
//#include "Env.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmVth *frmVth;

//---------------------------------------------------------------------------
void ShowVth(TfrmMain *MainForm)
{
    frmVth = new TfrmVth(Application);
    frmVth->MainForm = MainForm;
    frmVth->Show();
}

//---------------------------------------------------------------------------
__fastcall TfrmVth::TfrmVth(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::FormShow(TObject *Sender)
{
	tmrRequest->Interval = 5000;	// 5 second
	tmrRequest->Enabled  = true;
	tmrRequestTimer(NULL);

	tmrDisplayInfo->Interval = 1000;	// 1 second
	tmrDisplayInfo->Enabled  = true;
	tmrDisplayInfoTimer(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::btnPowerOffClick(TObject *Sender)
//power off
{
	btnPowerOff->Enabled = false;
	frmMain->GiGWResetFrom = 1;

	MainForm->SendVTH("!"); //신호 전송
	MainForm->GbVthDlgRequest = true;

	lblVthStatus1->Caption = "";
	lblVthStatus2->Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmVth::tmrDisplayInfoTimer(TObject *Sender)
//큐의 정보를 화면에 표시
{
//
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::FormClose(TObject *Sender, TCloseAction &Action)
{
	MainForm->GbVthDlgRequest = false;
	Action = caFree;
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::tmrDelayTimer(TObject *Sender)
{
	tmrDelay->Enabled = false;
	tmrRequest->Enabled = true;	// 다사 @ Request to VTH

	rbtnOn->Checked = true;
	btnPowerOff->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmVth::tmrRequestTimer(TObject *Sender)
{
	MainForm->SendVTH("@"); //5초에 한번씩 신호 전송
	MainForm->GbVthDlgRequest = true;
}

//---------------------------------------------------------------------------
void TfrmVth::Receive(AnsiString sMsg)
{
	if ((sMsg.UpperCase()).Pos("POWER OFF") > 0)
	{
		lblVthStatus1->Caption = sMsg;
		rbtnOff->Checked      = true;
		btnPowerOff->Enabled  = false;

		tmrDelay->Enabled   = true;
		tmrRequest->Enabled = false;	// 1분 후에 Enable

	} else if (GetSepItemCount(sMsg, ",") == 10) {

		rbtnOn->Checked      = true;
		btnPowerOff->Enabled = true;

		float fTemp = 0.0f;
		try
		{
			fTemp = GetSepItem(sMsg, 1, ",").ToDouble();
		} catch(...) {}
		//화면에 데이터 표시
		UnicodeString sNewData  = GetSepItem(sMsg, 5, ",") + "," +      //dc charge
								  GetSepItem(sMsg, 2, ",") + "," +      //dc battery
								  GetSepItem(sMsg, 0, ",");             //temperature
		lblVthStatus1->Caption  = sNewData;
		UnicodeString sNewData2 = GetSepItem(sMsg, 6, ",") + "," +      //AIN0
								  GetSepItem(sMsg, 7, ",") + "," +      //AIN1
								  GetSepItem(sMsg, 8, ",") + "," +      //AIN2
								  GetSepItem(sMsg, 9, ",");             //AIN3
		lblVthStatus2->Caption  = sNewData2;
	} else {
		lblVthStatus1->Caption = sMsg;	// POWER ON,....
	}
}
