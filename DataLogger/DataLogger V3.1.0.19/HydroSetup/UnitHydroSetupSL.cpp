//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitHydroSetupSL.h"
#include "../Database/RSetup.h"
#include "UnitSerialThread.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

THydroSetupSL *HydroSetupSL;

//---------------------------------------------------------------------------
__fastcall THydroSetupSL::THydroSetupSL(TComponent* Owner)
	: THydroSetup(Owner)
{
	m_iIdleSec   = 0;
	m_bSleep     = false;
	m_bSetup     = false;

	m_CellSize      = 0;
	m_BlankDistance = 0;
	m_Ncells        = 0;

	m_pFormAlert    = NULL;
}

//---------------------------------------------------------------------------
// Create Event
void __fastcall THydroSetupSL::FormCreate(TObject *Sender)
{
//
}

//---------------------------------------------------------------------------
// Show Event
void __fastcall THydroSetupSL::FormShow(TObject *Sender)
{
//
}

//---------------------------------------------------------------------------
// Close Event
void __fastcall THydroSetupSL::FormClose(TObject *Sender, TCloseAction &Action)
{
	if ( m_bBreak ) { // Break를 누른 후
		if ( m_enExit == enCancel ) { // 종료버튼을 누루지 않은 상태.
			Application->MessageBox(L"<BREAK>된 후 센서가 시작되지 않았습니다. \r\nClose 버튼을 이용하여 화면을 닫으십시오.",
									L"닫기 확인",MB_OK);
			Action = caNone;
		} else {
		}
	} else {
	}
}

//---------------------------------------------------------------------------
// CloseQuery Event
void __fastcall THydroSetupSL::FormCloseQuery(TObject *Sender, bool &CanClose)
{
//
}

//---------------------------------------------------------------------------
// Destory Event
void __fastcall THydroSetupSL::FormDestroy(TObject *Sender)
{
//
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbSendClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogStatus("Port is not ready..");
		return;
	}

	if ( m_bBreak == false ) {	// Break를 하지 않은 상태에서 명령어가  먹지 않도록 한다.
		LogStatus("Set Break command first..");
		return;
	}

	if ( m_bSleep ) {
		LogStatus("Sensor is sleep mode. Send <BREAK> first..");
		return;
	}

	m_bSetup = false;

	UnicodeString sCmd = edCommand->Text;

	if ( sCmd == "Show Setup" ) m_bSetup = true;
	else                        m_bSetup = false;

	Send(sCmd);

	SetIdleZero();	// idle second reset

	LogStatus(sCmd + " sended..");
}

//---------------------------------------------------------------------------
// Send Break Signal to SENSOR ( SL500K )
void __fastcall THydroSetupSL::cbSendBreakClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogMessage("Port is not ready..");
		return;
	}

	// HydroMeter1/2 Clear
	if ( HydroMeter != NULL ) HydroMeter->Clear();	// HydroMeter Clear

	LogStatus("Send Break ....");

	ComPort->SetBreak(true);	// Send Break
	Sleep(500);	// Wait 500ms
	ComPort->SetBreak(false);

	m_bBreak = true;	// Only once use
	m_bSleep = false;	// Sensor Wakeup
	Memo->Color = clBlack;
	Memo->Font->Color = clYellow;

	SetIdleZero();	// idle second reset

	LogStatus("Send Break finished..");
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbSetTimeClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogStatus("Port is not ready..");
		return;
	}

	if ( m_bBreak == false ) {	// Break를 하지 않은 상태에서 명령어가  먹지 않도록 한다.
		LogStatus("Set Break command first..");
		return;
	}

	if ( m_bSleep ) {
		LogStatus("Sensor is sleep mode. Send <BREAK> first..");
		return;
	}

	TDateTime now = Now();
	USHORT year, month, day, hour, minute, sec, msec;
	now.DecodeDate(&year, &month, &day);
	now.DecodeTime(&hour, &minute, &sec, &msec);

	AnsiString date = date.sprintf("%04d/%02d/%02d", year, month, day);
	date = "date " + date;
	AnsiString time = time.sprintf("%02d:%02d:%02d", hour, minute, sec);
	time = "time " + time;

	m_bSetup = false;

	Send(date);
	Sleep(10);	// Wait for while
	Send(time);

	SetIdleZero();	// idle second reset
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbSaveSetupClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogStatus("Port is not ready..");
		return;
	}

	if ( m_bBreak == false ) {	// Break를 하지 않은 상태에서 명령어가  먹지 않도록 한다.
		LogStatus("Set Break command first..");
		return;
	}

	if ( m_bSleep ) {
		LogStatus("Sensor is sleep mode. Send <BREAK> first..");
		return;
	}

	AnsiString sAnsiCmd = "show setup";	// show setup command

	m_bSetup = true;
	Send(sAnsiCmd);

	SetIdleZero();	// idle second reset
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbCloseClick(TObject *Sender)
{
	ModalResult = mrNone;
	m_enExit    = enCancel;

	if ( m_bBreak == false ) {	// 화면을 열고 Break 를 클릭하지 않는 상태 => 바로 닫는다.

		ModalResult = mrOk;
		m_enExit    = enCancel;	// Break 없이 바로 닫으므로 Logging 작업을 그대로 진행한다.
		Close();

	} else {	// Break를 누른 상태이면 Command 상태로 빠져 있으므로 START 명령으로 Sensor를 시작한 후 종료하여야 한다.

		// Check Time
		TDateTime now = Now();
		int iRemainSec = theEnv.GetRemainIdleTime(now);

		if ( iRemainSec <= 15 && iRemainSec >= 10 ) { // 10 <= Reamin Sec <= 15  (왜냐면

			SensorStartNClose(true);	// 무조건 BREAK 한번 하고 START 함.

		} else if ( iRemainSec < 10 ) { // 너무 짧게 남아 있는 시간
			UnicodeString sMsg = "Logger가 동작할 시간과 너무 가깝습니다. " + IntToStr(iRemainSec) + "초 남았습니다.\r\n" +
								 "화면을 닫고 " + IntToStr(iRemainSec) + "초 경과 후 다시 시도하십시오.\r\n";
			ShowMessage(sMsg);

			ModalResult = mrNone;	// 10초 남으면 화면을 못 닫게 한다.

		} else {	// 15초 이상 남은 상황 => 로거의 동작시간보다 오래 남은 상황 => 기다리거나 강제로 진행

			UnicodeString sMsg = "Logger가 동작할 시간에 " + IntToStr(iRemainSec) + "초 남았습니다.\r\n" +
								 "화면을 닫으면 Logger의 동작 시간 10초 전에 <BREAK> 신호와 함께 센서가 측정을 시작합니다.r\n" +
								 "계속 하시겠습니까?";

			if ( Application->MessageBox(sMsg.c_str(),L"로거 동작 시간 체크",MB_YESNO) == IDYES ) {

				iRemainSec = theEnv.GetRemainIdleTime(Now());   	// 현재시간으로 다시 한 번 체크함. (Alert창에서 시간을 소모하므로 다시 체크함)

				if ( iRemainSec > 15 ) {	// 15초 이상 남으면 화면을 닫고 메인 화면에서 <BREAK> + START
					FormMain->m_bHydroBreak = true;
					FormMain->m_iRemainBreak = iRemainSec;
					FormMain->TimerBREAK->Enabled = true;

					ModalResult = mrOk;
					m_enExit    = enOk;

					Close();
				} else if ( iRemainSec <= 15 && iRemainSec >= 10 ) {	// 10 <= x <= 15  이면 설정화면에서 <BREAK> + START
					SensorStartNClose(true);
				} else {
					UnicodeString sMsg = "Logger가 동작할 시간과 너무 가깝습니다. " + IntToStr(iRemainSec) + "초 남았습니다.\r\n" +
										 "화면을 닫고 " + IntToStr(iRemainSec) + "초 경과 후 다시 시도하십시오.\r\n";
					ShowMessage(sMsg);
					ModalResult = mrNone;	// 10초 남으면 화면을 못 닫게 한다.
				}
			} else {	// No를 응답함.
				ModalResult = mrNone;
			} // end of if ( 로거 동작 시간 체크 )
		} // end of if ( remain time )
	} // end of if ( m_bBreak )
}

//---------------------------------------------------------------------------
void THydroSetupSL::SensorStartNClose(bool bBreak)
{
	m_pFormAlert = new TFormAlert(this);
	m_pFormAlert->ShowMessage("센서를 START 하고 화면을 닫습니다.");
	m_pFormAlert->Show();

	//if ( m_bSleep ) {		// Sleep 모드에 빠져있을 때. <BREAK> + "START" Command
	if ( bBreak ) {		// Sleep 모드에 빠져있을 때. <BREAK> + "START" Command
		// At first, Send <BREAK> to sensor, FOR Wake up the Sensor
		ComPort->SetBreak(true);	// Send Break
		SleepMsg(500);	// Wait 500ms
		ComPort->SetBreak(false);
		//Sleep(100);	// Wait for while to Sensor is ready..
	}

	SleepMsg(5000); // Wait 5second for while heaven2

	Send("START\r");
	TimerClose->Enabled = true;	// Close Timer Enable ( 2 Second )

	LogMessage("Sensor is <BREAK> and started..");
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::TimerTimer(TObject *Sender)
{
	m_iIdleSec++;

	if ( m_iIdleSec > (5 * 60) ) {
		LogStatus("Sensor fall sleep mode..");

		m_bSleep = true;
		Memo->Color = clWhite;
		Memo->Font->Color = clBlack;

		Timer->Enabled = false;
		m_iIdleSec = 0;
	}
}

//---------------------------------------------------------------------------
// 종료 Timer
void __fastcall THydroSetupSL::TimerCloseTimer(TObject *Sender)
{
	if ( m_pFormAlert != NULL ) delete m_pFormAlert;	// Destory Alert Box

	TimerClose->Enabled = false;

	ModalResult = mrOk;
	m_enExit    = enOk;

	Close();	// Close
}

//---------------------------------------------------------------------------
void THydroSetupSL::Receive(AnsiString sRecvMsg)
{
	m_sRecvMsg += sRecvMsg;

	int iPos = m_sRecvMsg.Pos("\r\n");

	while ( iPos > 0 ) {
		AnsiString sCmd = m_sRecvMsg.SubString(1, iPos-1);

		Process(sCmd);

		m_sRecvMsg = m_sRecvMsg.SubString(iPos+2, m_sRecvMsg.Length() - (iPos+1));     // "aaa bbb ccc\r\naa bb cc\r\naa"
		iPos = m_sRecvMsg.Pos("\r\n");
	}
}

//---------------------------------------------------------------------------
void THydroSetupSL::Process(AnsiString sMsg)
{
	ShowRX(sMsg);

	if ( m_bSetup ) {

		AnsiString Str;
		AnsiString Str2;

		try {
			if ( sMsg.Pos("CellSize") > 0 && sMsg.Length() > 27) {
				Str        = sMsg.SubString(27, sMsg.Length() - 27 +1);
				Str2       = Trim(Str);
				m_CellSize = StrToFloat(Str2);

				TRSetup HydroMeter;

				HydroMeter.PK1 = 10;
				HydroMeter.PK2 = (m_bFirstHydro) ? 2 : 3;	// 2:하층유속계, 3:상층유속계

				HydroMeter.Value08 = FloatToStr((Extended)m_CellSize); // 2.00

				bool bResult = false;
				bResult = HydroMeter.Update("Value08");

				if ( bResult ) LogStatus("CellSize saved..");
				else           LogStatus("CellSize not saved..");

			} else if ( sMsg.Pos("BlankDistance") > 0 && sMsg.Length() > 27 ) {
				Str        = sMsg.SubString(27, sMsg.Length() - 27 +1);
				Str2       = Trim(Str);
				m_BlankDistance = StrToFloat(Str2);

				TRSetup HydroMeter;

				HydroMeter.PK1 = 10;
				HydroMeter.PK2 = (m_bFirstHydro) ? 2 : 3;	// 2:하층유속계, 3:상층유속계

				HydroMeter.Value09 = FloatToStr((Extended)m_BlankDistance); // 2.00

				bool bResult = false;
				bResult = HydroMeter.Update("Value09");

				if ( bResult ) LogStatus("BlankDistance saved..");
				else           LogStatus("BlankDistance not saved..");

			} else if ( sMsg.Pos("Ncells") > 0 && sMsg.Length() > 27) {
				Str        = sMsg.SubString(27, sMsg.Length() - 27 +1);
				Str2       = Trim(Str);
				m_Ncells   = StrToFloat(Str2);

				TRSetup HydroMeter;

				HydroMeter.PK1 = 10;
				HydroMeter.PK2 = (m_bFirstHydro) ? 2 : 3;	// 2:하층유속계, 3:상층유속계

				HydroMeter.Value07 = FloatToStr((Extended)m_Ncells); // 30

				bool bResult = false;
				bResult = HydroMeter.Update("Value07");

				if ( bResult ) LogStatus("Ncells saved..");
				else           LogStatus("Ncells not saved..");

			} else {
				//nothing to do..
			}
		} catch ( Exception &e ) {
#ifdef _DEBUG
			ShowMessage(e.Message);
#endif
		}
	}
}

//---------------------------------------------------------------------------
void THydroSetupSL::ShowRX(AnsiString sRX)
{
	if ( Memo->Lines->Count > 200 )
		Memo->Lines->Delete(0);

	Memo->Lines->Add(Trim(sRX));
}

//---------------------------------------------------------------------------
void THydroSetupSL::LogStatus(UnicodeString sLogMsg)
{
	StatusBar->SimpleText = sLogMsg;
}

//---------------------------------------------------------------------------
void THydroSetupSL::SetIdleZero()
{
	// Sleep 상태 카운트 다시 시작함.(5분 동안 아무 변화가 없으면 Fall Sleep Mode )
	Timer->Enabled = false;
	m_iIdleSec = 0;
	Timer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::edCommandKeyPress(TObject *Sender, System::WideChar &Key)
{
	if ( Key == '\r' ) {
		cbSendClick(Sender);
	}
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbShowClick(TObject *Sender)
{
	TButton *Button = dynamic_cast<TButton *>(Sender);

	UnicodeString sCmd = Button->Caption;
	edCommand->Text = sCmd;
	cbSendClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::edSaveClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogStatus("Port is not ready..");
		return;
	}

	if ( m_bBreak == false ) {	// Break를 하지 않은 상태에서 명령어가  먹지 않도록 한다.
		LogStatus("Set Break command first..");
		return;
	}

	if ( m_bSleep ) {
		LogStatus("Sensor is sleep mode. Send <BREAK> first..");
		return;
	}

	if ( MemoCmd->Lines->Count == 0 ) return;	// command empty
	if ( SerialThread != NULL ) {
		ShowMessage("Thread is working.. try again a few second later..");
		return;
	}

	// User Confirm Check ( 유속계에 일괄 설정하는 작업이므로 신중해야 함을 통지)
	if ( Application->MessageBox(L"장비에 필요한 제원을 일괄 설정합니다. 계속할까요?", L"장비설정", MB_YESNO) != IDYES ) return;

	////////////////////////////////////////////////
	// Working Process
	////////////////////////////////////////////////

	SerialThread = new TSerialThread(true);
	SerialThread->Init(ComPort, this);

	for ( int i = 0 ; i < MemoCmd->Lines->Count ; i++ ) {

		AnsiString sCmd = MemoCmd->Lines->operator [](i);

		if (sCmd.IsEmpty()) continue;

		STCommand *pCmd = new STCommand();

		pCmd->sCmd = sCmd;
		pCmd->iDelayTime = 500;	// 500ms wait

		SerialThread->m_pCmdQueue->Push(pCmd);
	}

	SerialThread->Start();

	SetIdleZero();	// idle second reset

	LogStatus("");
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbBreakClick(TObject *Sender)
{
	if ( ComPort->Open == false ) {
		LogMessage("Port is not ready..");
		return;
	}

	// HydroMeter1/2 Clear
	if ( HydroMeter != NULL ) HydroMeter->Clear();	// HydroMeter Clear

	LogStatus("Send Break ....");

	ComPort->SetBreak(true);	// Send Break
	Sleep(500);	// Wait 500ms
	ComPort->SetBreak(false);

	m_bBreak = true;
	Memo->Color = clBlack;
	Memo->Font->Color = clYellow;

	SetIdleZero();	// idle second reset

	LogStatus("Send Break finished..");
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbBlankDistanceClick(TObject *Sender)
{
	if (edBlankDistance->Text.IsEmpty()) return;

	UnicodeString sCmd;
	sCmd = "BlankDistance " + edBlankDistance->Text;
	edCommand->Text = sCmd;
	cbSendClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbNCellsClick(TObject *Sender)
{
	if (edNCells->Text.IsEmpty()) return;

	UnicodeString sCmd;
	sCmd = "NCells " + edNCells->Text;
	edCommand->Text = sCmd;
	cbSendClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbCellSizeClick(TObject *Sender)
{
	if (edCellSize->Text.IsEmpty()) return;

	UnicodeString sCmd;
	sCmd = "CellSize " + edCellSize->Text;
	edCommand->Text = sCmd;
	cbSendClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbCellCountClick(TObject *Sender)
{
// 	Undefine
}

//---------------------------------------------------------------------------
void __fastcall THydroSetupSL::cbMeasureTimeClick(TObject *Sender)
{
// 	Undefine
}
//---------------------------------------------------------------------------

