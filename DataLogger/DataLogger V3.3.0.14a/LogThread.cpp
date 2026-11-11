//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "LogThread.h"
#include "UnitMain.h"
//#include "UnitLog.h"	// Log Class

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TLogThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TLogThread::TLogThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}

void TLogThread::InitThread(UnicodeString sHomeDir)
{
	m_Log.SetHomeDir(sHomeDir);
}

//---------------------------------------------------------------------------
void __fastcall TLogThread::Execute()
{
	TWaitResult Result;
	STLogMessage *pLogMessage;
	while ( FormMain->m_bLogThread ) {
		Result = GLogEvent->WaitFor(EVENT_WAIT_TIME);	// wait 1 second
        if ( Result == wrSignaled || Result == wrTimeout ) {
			while ( GLogQueue->Count() ) {

				pGLogSection->Enter();
				pLogMessage = (STLogMessage *) GLogQueue->Pop();
				pGLogSection->Leave();

				LogWrite(pLogMessage);
				delete pLogMessage;
            }
		} else if ( Result == wrAbandoned ) {	// Event Object was destored
        										// wrError: An error occured while waiting.
												// Check the LastError property for an error
                                                // code giving more informaiton.
    	} else {
        	// nothing to do..
        }
        //G_pSoundEvent->ResetEvent();			// Auto Event Reset mode
	}
}
//---------------------------------------------------------------------------

void TLogThread::LogWrite(STLogMessage *pLogMessage)
{
	TDateTime t = pLogMessage->LogTime;
	USHORT iHour, iMinute, iSec, iMSec;

	t.DecodeTime(&iHour, &iMinute, &iSec, &iMSec);

	UnicodeString sLogMsg;
	sLogMsg = sLogMsg.sprintf(L"[%02d:%02d:%02d] ",iHour, iMinute, iSec);
	sLogMsg += pLogMessage->LogMessage;

	if ( pLogMessage->bFile ) {
		m_Log.LogMessage(sLogMsg);
	}

	// VCL controls must only be accessed by the main UI thread.
	m_sUILogMessage = sLogMsg;
	Synchronize(UpdateLogMemo);
}
//---------------------------------------------------------------------------

void __fastcall TLogThread::UpdateLogMemo()
{
	if ( Application->Terminated || FormMain == NULL ) return;

	if ( FormMain->Memo->Lines->Count > 100 ) FormMain->Memo->Lines->Clear();

	FormMain->Memo->Lines->Insert(0, m_sUILogMessage);
}
//---------------------------------------------------------------------------
