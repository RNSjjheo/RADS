//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLeft.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormLeft *FormLeft = NULL;

int GetElapseTime(TDateTime Time)	// 경과시간 산출
{
	TDateTime a = Now() - Time;

	USHORT hour, minute, second, msec;
	a.DecodeTime(&hour, &minute, &second, &msec);

	return  (hour * 60 * 60) + (minute * 60) + (second);
}

//---------------------------------------------------------------------------
__fastcall TFormLeft::TFormLeft(TComponent* Owner)
	: TForm(Owner)
{
	m_sPointCode    = "";
	m_sPointName    = "";

	m_sHydro1       = "";
	m_sHydroPort1   = "";
	m_sHydroHz1     = "";
	m_sHydroSN1     = "";

	m_sHydro2       = "";
	m_sHydroPort2   = "";
	m_sHydroHz2     = "";
	m_sHydroSN2     = "";

	m_sHydro3       = "";
	m_sHydroPort3   = "";
	m_sHydroHz3     = "";
	m_sHydroSN3     = "";

	m_sWaterLevel   = "";
	m_sWaterPort    = "";
	m_sWaterHz      = "";
	m_sWaterSN      = "";

	m_sVTHPort      = "";

	m_sInterval     = "";
	m_sPeriod       = "";
	m_sStart        = "";
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::FormResize(TObject *Sender)
{
	DBGrid->Top = this->ClientHeight - DBGrid->Height -2;

	Button1->Top = DBGrid->Top - (Button1->Height + 5);
	Button2->Top = DBGrid->Top - (Button2->Height + 5);

	mmRecentCheckReport->Height = (Button1->Top) - mmRecentCheckReport->Top - 5;
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::FormShow(TObject *Sender)
{
	//heaven2 2014.08.22
	if ( !FormMain->SQLConnection->Connected ) return;

	shpLoggerProgress->Width = 0;
	shpSenderProgress->Width = 0;

	m_LoggerTime = Now() - EncodeTime(0,0,1,0);
	m_SenderTime = Now() - EncodeTime(0,0,1,0);

	m_enLoggerStatus = enLoggerIdle;
	m_enSenderStatus = enSenderIdle;

    RCheckReport = new TfbTable(FormMain->SQLConnection, "RCHECKREPORT", DataSource);
    Open();
}

//---------------------------------------------------------------------------
void TFormLeft::ShowRecentDataToMemo()
{
    UnicodeString sCheckDate;
    CheckDateGetText(RCheckReport->DataSet()->FieldByName("CHECKDATE"), sCheckDate, True);

	UnicodeString sCheckFlag;
    CheckFlagGetText(RCheckReport->DataSet()->FieldByName("CHECKFLAG"), sCheckFlag, True);

    mmRecentCheckReport->Lines->Clear();
    mmRecentCheckReport->Lines->Add(sCheckDate + " " + sCheckFlag);
    mmRecentCheckReport->Lines->Add(RCheckReport->DataSet()->FieldByName("MANAGE")->Text);
    mmRecentCheckReport->Lines->Add("점검자: " + RCheckReport->DataSet()->FieldByName("NAME")->Text);
}

//---------------------------------------------------------------------------
void TFormLeft::Open()
{
    UnicodeString sDate = FormatDateTime("yyyymmdd", Now() - 60); //현재부터 60일 이전

    RCheckReport->ClearParams();
    RCheckReport->SetWhereParam("CHECKDATE", sDate, wkBiggerOrEqual);
    RCheckReport->SetOrderParam("CHECKDATE", okDesc);
    RCheckReport->Open();
    //simpledataset의 필드 조건 세팅
    RCheckReport->FieldByName("CHECKDATE")->DisplayLabel = "점검일자";
    RCheckReport->FieldByName("CHECKDATE")->DisplayWidth = 10;
    RCheckReport->FieldByName("CHECKDATE")->OnGetText = CheckDateGetText;
    RCheckReport->FieldByName("CHECKFLAG")->DisplayLabel = "점검구분";
    RCheckReport->FieldByName("CHECKFLAG")->DisplayWidth = 8;
    RCheckReport->FieldByName("CHECKFLAG")->OnGetText = CheckFlagGetText;
    RCheckReport->FieldByName("NAME")->DisplayLabel = "점검자";
    RCheckReport->FieldByName("NAME")->DisplayWidth = 10;

    RCheckReport->FieldByName("COMPANY")->Visible = False;
    RCheckReport->FieldByName("DATAFORMAT")->Visible = False;
    RCheckReport->FieldByName("PORPOSE")->Visible = False;
    RCheckReport->FieldByName("MANAGE")->Visible = False;
    RCheckReport->FieldByName("COMPLEMENT")->Visible = False;
    RCheckReport->FieldByName("REPLACE")->Visible = False;
    RCheckReport->FieldByName("ETC")->Visible = False;
    RCheckReport->FieldByName("MAILFLAG")->Visible = False;
    RCheckReport->FieldByName("MAILDATETIME")->Visible = False;

    ShowRecentDataToMemo();
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::CheckDateGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText)
//점검일자
{
    Text = Sender->AsString.SubString(1, 4) + '-' +
           Sender->AsString.SubString(5, 2) + '-' +
           Sender->AsString.SubString(7, 2);
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::CheckFlagGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText)
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
void __fastcall TFormLeft::Button2Click(TObject *Sender)
//점검일지 쿼리
{
	Open();
}

//---------------------------------------------------------------------------
TDateTime GetStartTime()
{
    unsigned short iY, iM, iD, iH, iN, iS, iMs;
    DecodeDate(Now(), iY, iM, iD);
    DecodeTime(Now(), iH, iN, iS, iMs);
    if ((iN % 10) >= 5) iN = iN / 10 * 10 + 5;
    else
    {
        if (iN >= 5) iN = iN / 10 * 10 - 5;
        else
        {
            iH -= 1;
            iN = 55;
		} // else
    } // else
    iS = iMs = 0;
    TDateTime dtResult = EncodeDate(iY, iM, iD) + EncodeTime(iH, iN, iS, iMs);
    return dtResult;
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::tmrDisplayLoggerSenderTimeTimer(TObject *Sender)
{
//	static TDateTime dtStart; // = GetStartTime();
//
//    switch(iLogOperation)
//    {
//        case 1: //logger start
//			dtStart = Now() - EncodeTime(0,0,1,0); //1초 뺌
//            shpLoggerProgress->Tag = 0;
//            shpLoggerProgress->Brush->Color = clLime;
//            shpLoggerProgress->Width = 0;
//            iLogOperation = 2;
//        case 2: //logging
//			shpLoggerProgress->Width = (shpBG1->Width / 17) * shpLoggerProgress->Tag;
//            break;
//        case 3: //logger finish
//            shpLoggerProgress->Tag = 0;
//            dtStart = Now();
//            shpLoggerProgress->Width = 0;
//            break;
//        case 4: //server start
//            dtStart = Now();
//            shpLoggerProgress->Width = 0;
//            break;
//        case 5: //server working
//            shpSenderProgress->Brush->Color = clBlue;
//            if (shpSenderProgress->Width < shpBG2->Width) shpSenderProgress->Tag = shpSenderProgress->Tag + 1;
//            shpSenderProgress->Width = (shpBG2->Width / 17) * shpSenderProgress->Tag;
//			break;
//        case 6: //server finish
//            shpSenderProgress->Brush->Color = clBlue;
//            shpSenderProgress->Width = shpBG2->Width;
//            dtStart = 0;
//            shpSenderProgress->Width = 0;
//            break;
//        case 7: //rnsea start
//            dtStart = Now();
//            shpSenderProgress->Width = 0;
//            break;
//        case 8: //rnsea working
//            shpSenderProgress->Brush->Color = clAqua;
//            if (shpSenderProgress->Width < shpBG2->Width) shpSenderProgress->Tag = shpSenderProgress->Tag + 1;
//            shpSenderProgress->Width = (shpBG2->Width / 17) * shpSenderProgress->Tag;
//            break;
//        case 9: //rnsea finish
//            shpSenderProgress->Brush->Color = clAqua;
//            shpSenderProgress->Width = shpBG2->Width;
//            dtStart = 0;
//            iLogOperation++;
//            break;
//        case 10:
//            shpSenderProgress->Width = 0;
//            break;
//    } // switch
//
//    switch(iLogOperation)
//    {
//        case 1:
//        case 2:
//		case 3:
//            if ((int)dtStart != 0) lblLoggerTime->Caption = FormatDateTime("nn:ss", Now() - dtStart);
//            break;
//        case 4:
//        case 5:
//        case 6:
//        case 7:
//        case 8:
//        case 9:
//            if ((int)dtStart != 0) lblSenderTime->Caption = FormatDateTime("nn:ss", Now() - dtStart);
//            break;
//    } // switch
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::FormCreate(TObject *Sender)
{
	shpLoggerProgress->Width = 0;
	shpSenderProgress->Width = 0;

	m_enLoggerStatus = enLoggerIdle;
	m_enSenderStatus = enSenderIdle;

	//iLogOperation = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::N1Click(TObject *Sender)
//vth연결
{
    FormMain->miMDIVth->Click();
//
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::DataSourceDataChange(TObject *Sender, TField *Field)
{
	ShowRecentDataToMemo();
}

//---------------------------------------------------------------------------
void TFormLeft::SetPointInfo(UnicodeString sPointCode, UnicodeString sPointName)
{
	m_sPointCode = sPointCode;
	m_sPointName = sPointName;
}

//---------------------------------------------------------------------------
void TFormLeft::SetHydroMeter(int iNo, UnicodeString sHydro, UnicodeString sPort, UnicodeString sHz, UnicodeString sSN)
{
	if ( iNo == 1 ) {
		m_sHydro1     = sHydro;
		m_sHydroPort1 = sPort;
		m_sHydroHz1   = sHz;
		m_sHydroSN1   = sSN;
	} else if ( iNo == 2 ) {
		m_sHydro2     = sHydro;
		m_sHydroPort2 = sPort;
		m_sHydroHz2   = sHz;
		m_sHydroSN2   = sSN;
	} else {
		m_sHydro3     = sHydro;
		m_sHydroPort3 = sPort;
		m_sHydroHz3   = sHz;
		m_sHydroSN3   = sSN;
	}
}

//---------------------------------------------------------------------------
void TFormLeft::SetWaterLevel(UnicodeString sWaterLevel, UnicodeString sPort, UnicodeString sWaterHz, UnicodeString sWaterSN, UnicodeString sAtmosPort)
{
	m_sWaterLevel = sWaterLevel;
	m_sWaterPort  = sPort;
	m_sWaterHz    = sWaterHz;
	m_sWaterSN    = sWaterSN;
	m_sAtmosPort  = sAtmosPort;
}

//---------------------------------------------------------------------------
void TFormLeft::SetVTHInfo(UnicodeString sPort)
{
	m_sVTHPort = sPort;
}

//---------------------------------------------------------------------------
void TFormLeft::SetWorkingInfo(UnicodeString sInterval, UnicodeString sPeriod, UnicodeString sStart)
{
	m_sInterval = sInterval;
	m_sPeriod   = sPeriod;
	m_sStart    = sStart;
}

//---------------------------------------------------------------------------
void TFormLeft::ShowSetupInfo()
{
	TListItem *pItem = NULL;
	TStrings  *p;

	// 지점 정보
	pItem = ListView->Items->Item[0];
	pItem->Caption = "지점코드";
	p = pItem->SubItems;
	p->Strings[0] = m_sPointCode;
	p->Strings[1] = m_sPointName;
	p->Strings[2] = "";
	p->Strings[3] = "";

	// 수위계
	pItem = ListView->Items->Item[1];
	pItem->Caption = "수위계";
	p = pItem->SubItems;
	p->Strings[0] = m_sWaterLevel;
	if ( m_sWaterLevel == LT400_STR) p->Strings[0] = m_sWaterLevel + "+Atmos";
	p->Strings[1] = m_sWaterPort;
	p->Strings[2] = m_sWaterHz;
	p->Strings[3] = m_sWaterSN;

	//1번 유속계
	pItem = ListView->Items->Item[2];
	pItem->Caption = "1번유속계";
	p = pItem->SubItems;
	p->Strings[0] = m_sHydro1;
	p->Strings[1] = m_sHydroPort1;
	p->Strings[2] = m_sHydroHz1;
	p->Strings[3] = m_sHydroSN1;

	//2번 유속계
	pItem = ListView->Items->Item[3];
	pItem->Caption = "2번유속계";
	p = pItem->SubItems;
	p->Strings[0] = m_sHydro2;
	p->Strings[1] = m_sHydroPort2;
	p->Strings[2] = m_sHydroHz2;
	p->Strings[3] = m_sHydroSN2;

	//3번 유속계
	pItem = ListView->Items->Item[4];
	pItem->Caption = "3번유속계";
	p = pItem->SubItems;
	p->Strings[0] = m_sHydro3;
	p->Strings[1] = m_sHydroPort3;
	p->Strings[2] = m_sHydroHz3;
	p->Strings[3] = m_sHydroSN3;

	//VTH
	pItem = ListView->Items->Item[5];
	pItem->Caption = "VTHLogger";
	p = pItem->SubItems;
	p->Strings[0] = "";
	p->Strings[1] = m_sVTHPort;
	p->Strings[2] = "";
	p->Strings[3] = "";

	//운영정보
	pItem = ListView->Items->Item[6];
	pItem->Caption = "측정/동작/시작";
	p = pItem->SubItems;
	p->Strings[0] = m_sInterval;
	p->Strings[1] = m_sPeriod;
	p->Strings[2] = m_sStart;
	p->Strings[3] = "";
}

//---------------------------------------------------------------------------
//
// Logger Status
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TFormLeft::LoggerIdle()
{
	if ( m_enLoggerStatus == enLoggerIdle ) return;

	m_enLoggerStatus = enLoggerIdle;
	shpLoggerProgress->Width = 0;
	LoggerTimer->Enabled = false;
	m_LoggerTime = Now();	// 1Sec Minus
	lblLoggerTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::LoggerStart()
{
	if ( m_enLoggerStatus == enLoggerStart ) return;	// Already Started

	m_enLoggerStatus = enLoggerStart;
	shpLoggerProgress->Width = 0;
	LoggerTimer->Interval = 1000;	// 1Sec
	LoggerTimer->Enabled = true;
	m_LoggerTime = Now();	// 1Sec Minus
	lblLoggerTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::LoggerLogging()
{
	if ( m_enLoggerStatus == enLoggerStart ) {
		m_enLoggerStatus = enLoggerLogging;
	} else if ( m_enLoggerStatus == enLoggerLogging ) {
		// nothing to do..
	} else {  // m_enLoggerStatus == enLoggerIdle || m_enLoggerStatus == enLoggerFinish
		// 최초실행하였으나 Logger가 이미 Logging 중인 상황
		m_enLoggerStatus = enLoggerLogging;
		shpLoggerProgress->Width = 0;
		LoggerTimer->Interval = 1000;	// 1Sec
		LoggerTimer->Enabled = true;
		m_LoggerTime = Now();	// 1Sec Minus
	}
}

//---------------------------------------------------------------------------
void TFormLeft::LoggerFinish()
{
	if ( m_enLoggerStatus == enLoggerFinish ) return;

	m_enLoggerStatus = enLoggerFinish;
	shpLoggerProgress->Width = shpBG1->Width;
	LoggerTimer->Enabled = false;
	lblLoggerTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
//
// Sender Status
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TFormLeft::SenderIdle()
{
	if ( m_enSenderStatus == enSenderIdle ) return;

	m_enSenderStatus = enSenderIdle;
	shpSenderProgress->Width = 0;
	SenderTimer->Enabled = false;
	lblSenderTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::SenderStart()
{
	if ( m_enSenderStatus == enSenderStart ) return;

	m_enSenderStatus = enSenderStart;
	shpSenderProgress->Width = 0;
	shpSenderProgress->Brush->Color = clLime;
	SenderTimer->Interval = 1000;	// 1Sec
	SenderTimer->Enabled = true;
	m_SenderTime = Now();	// 1Sec Minus
	lblSenderTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::SenderWorking()
{
	if ( m_enSenderStatus == enSenderWorking ) return;
	m_enSenderStatus = enSenderWorking;
}

//---------------------------------------------------------------------------
void TFormLeft::SenderFinish()
{
	if ( m_enSenderStatus == enSenderFinish ) return;

	m_enSenderStatus = enSenderFinish;
	shpSenderProgress->Width = shpBG2->Width;
	SenderTimer->Interval = 1000;	// 1Sec
	SenderTimer->Enabled = false;
	lblSenderTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::SenderRnseaStart()
{
	if ( m_enSenderStatus == enSenderRnseaStart ) return;

	m_enSenderStatus = enSenderRnseaStart;
	shpSenderProgress->Brush->Color = clAqua;
	shpSenderProgress->Width = 0;
	SenderTimer->Interval = 1000;	// 1Sec
	SenderTimer->Enabled = true;
	m_SenderTime = Now();	// 1Sec Minus
	lblSenderTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void TFormLeft::SenderRnseaWorking()
{
	if ( m_enSenderStatus == enSenderRnseaWorking ) return;
	m_enSenderStatus = enSenderRnseaWorking;
}

//---------------------------------------------------------------------------
void TFormLeft::SenderRnseaFinish()
{
	if ( m_enSenderStatus == enSenderRnseaFinish ) return;	// nothing to do..

	m_enSenderStatus = enSenderRnseaFinish;
	shpSenderProgress->Width = shpBG2->Width;
	SenderTimer->Enabled = false;
	lblSenderTime->Caption = "00:00";
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::LoggerTimerTimer(TObject *Sender)
{
	switch ( m_enLoggerStatus ) {
		case enLoggerIdle    : break;
		case enLoggerStart   :
		case enLoggerLogging :
			{
				int iElapseTime = GetElapseTime(m_LoggerTime);
				int iMinute, iSecond;
				iMinute = iElapseTime / 60;
				iSecond = iElapseTime % 60;
				double dPos = ((double)iElapseTime / (double)(60 * 8  + 30)); // 8분 30초 대한 비율
				dPos = ( dPos > 1.0 ) ? 1.0 : dPos;
				shpLoggerProgress->Width = (int)(shpBG1->Width * dPos);
				UnicodeString sSpanTime;
				sSpanTime = sSpanTime.sprintf(L"%02d:%02d",iMinute,iSecond);
				lblLoggerTime->Caption = sSpanTime;
			}
			break;
		case enLoggerFinish  : break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormLeft::SenderTimerTimer(TObject *Sender)
{
	 switch ( m_enSenderStatus ) {
		case enSenderIdle         : break;
		case enSenderStart        :
		case enSenderRnseaStart   :
		case enSenderWorking      :		// Start/Working
		case enSenderRnseaWorking : 	// Start/Working
			{
				int iElapseTime = GetElapseTime(m_SenderTime);
				int iMinute, iSecond;
				iMinute = iElapseTime / 60;
				iSecond = iElapseTime % 60;
				double dPos = ((double)iElapseTime / 10.0); // 10초 대한 비율
				dPos = ( dPos > 1.0 ) ? 1.0 : dPos;
				shpSenderProgress->Width = (int)(shpBG2->Width * dPos);
				UnicodeString sSpanTime;
				sSpanTime = sSpanTime.sprintf(L"%02d:%02d",iMinute,iSecond);
				lblSenderTime->Caption = sSpanTime;
			}
			break;
		case enSenderFinish       : break;
		case enSenderRnseaFinish  : break;
	 }
}
//---------------------------------------------------------------------------

