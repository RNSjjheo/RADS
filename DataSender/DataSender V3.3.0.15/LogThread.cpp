//---------------------------------------------------------------------------

#pragma hdrstop

#include "LogThread.h"
#include "LockeCommon.h"
//#include "Env.h"
//#include "Thrdqueue.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TLogThread *thrdLog;

//---------------------------------------------------------------------------
__fastcall TLogThread::TLogThread() : TThread(false)
{
	Priority   = tpLower;
	RSenderLog = new TfbTable(frmMain->SQLConnection, "RSENDERLOG");
	bWorking   = false; // Thread is idle
	m_pLogMessage = NULL;
	FreeOnTerminate = true;
}

//---------------------------------------------------------------------------
__fastcall TLogThread::~TLogThread()
{
	//delete RSenderLog;	// Resource Release
	if ( RSenderLog != NULL ) delete RSenderLog;	//heaven2 2017.09.26
}

//---------------------------------------------------------------------------
void __fastcall TLogThread::PerformLog()
{
	if ( Application->Terminated ) return;

	// Main UI Log memo
	TMemo *LogMemo = (m_pLogMessage->bLogMessage ) ? frmMain->mmLog : frmMain->mmGuardnetLog;
	UnicodeString sMemoText = "[" + m_pLogMessage->sDate.SubString(12, 8) + "]" + m_pLogMessage->sText;
	LogMemo->Lines->Insert(0, sMemoText);
	if ( LogMemo->Lines->Count > CiMaxLogMemoLine ) LogMemo->Lines->Clear();

	// File Log
	if (m_pLogMessage->bFileWrite)
	{
		UnicodeString sFileDate = m_pLogMessage->sDate;
		sFileDate = StringReplace(sFileDate, "-", "", TReplaceFlags() << rfReplaceAll);
		sFileDate = StringReplace(sFileDate, ":", "", TReplaceFlags() << rfReplaceAll);
		sFileDate = StringReplace(sFileDate, " ", "", TReplaceFlags() << rfReplaceAll);
		UnicodeString sPathFile = "";
		if (m_pLogMessage->bLogMessage) sPathFile = PathStrAdd(frmMain->DBEnvInfo.sPubSenderHome, CsLogPath) + "\\" +
											 frmMain->DBEnvInfo.sPointCode + "_" + sFileDate.SubString(1, 6) + "\\" +
											 frmMain->DBEnvInfo.sPointCode + "_" + sFileDate.SubString(1, 8) + ".log";
		else                     sPathFile = PathStrAdd(frmMain->DBEnvInfo.sPubSenderHome, CsGuardnetLogPath) + "\\" +
											 frmMain->DBEnvInfo.sPointCode + "_" + sFileDate.SubString(1, 6) + "\\" +
											 frmMain->DBEnvInfo.sPointCode + "_" + sFileDate.SubString(1, 8) + ".log";
		try
		{
			WriteFileLog(sPathFile, m_pLogMessage->sDate, m_pLogMessage->sText);
		} catch(Exception &e)
		{
			if ( !Application->Terminated ) {	// 아직 종료되지 않았으면 .
				UnicodeString sFileError = "[LogThrd] 로그파일 출력 에러";
				LogMemo->Lines->Insert(0, sFileError);
			}
		} // catch
	} // if

	// DB Log
	if ( m_pLogMessage->bDBWrite )
		WriteDBLog(m_pLogMessage->sDate, m_pLogMessage->sText);

}
//---------------------------------------------------------------------------
void TLogThread::WriteDBLog(UnicodeString sDateTime, UnicodeString sText)
{
    RSenderLog->ClearParams();
    RSenderLog->SetFieldParam("LOGDATETIME", sDateTime);
    RSenderLog->SetFieldParam("LOGMESSAGE", sText);
    RSenderLog->Insert();
}
//---------------------------------------------------------------------------
void __fastcall TLogThread::Execute()
{
	try
	{
		TWaitResult Result;

		while (!Terminated)
		{
			bWorking = false;	//idle
			Result = pGLogEvent->WaitFor(1000);	// wait 1 sec
			if ( Result == wrSignaled || Result == wrTimeout ) {

				while ( pGLogQueue->Count() ) {
					pGLogSection->Enter();
					m_pLogMessage = (STLogMessage *) pGLogQueue->Pop();
					pGLogSection->Leave();

					// LogWrite Action
					Synchronize(PerformLog);

					delete m_pLogMessage;
				}
			} else if ( Result == wrAbandoned ) {	// Event Object was destored
													// wrError: An error occured while waiting.
													// Check the LastError property for an error
													// code giving more informaiton.
			} else {
				// nothing to do..
			}
		} // while
	} // try
	catch (Exception &e)
	{
		frmMain->tmrWD->Enabled = false; //watchdog 전송중지
	} // catch

	delete RSenderLog;
	RSenderLog = NULL;	//heaven2 2017.09.26
}
//---------------------------------------------------------------------------
void TLogThread::WriteFileLog(UnicodeString sPathFile, UnicodeString sDate, UnicodeString sText)
{
    AnsiString sAText = "[" + sDate.SubString(12, 8) + "]" + sText + "\r\n";
	static UnicodeString sLastPathFile = "";

    Winapi::Windows::THandle hFile;
    try
    {
        if (sLastPathFile != sPathFile) //파일명이 바뀌었을 때만 폴더체크해서 생성처리
        {
            ForceDirectories(ExtractFilePath(sPathFile));
            sLastPathFile = sPathFile;
        } // if
        if (FileExists(sPathFile))
        {
            hFile = FileOpen(sPathFile, fmOpenWrite | fmShareDenyNone);
            FileSeek(hFile, 0, 2);
        } else hFile = FileCreate(sPathFile, fmOpenWrite | fmShareDenyNone);
        FileWrite(hFile, sAText.c_str(), sAText.Length());
    } // try
    __finally
    {
        if (hFile != -1) FileClose(hFile);
    } // catch
}
//---------------------------------------------------------------------------

