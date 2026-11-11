//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitLog.h"
#include <stdio.h>

TLog theLog;

//---------------------------------------------------------------------------
// Constructor
TLog::TLog()
{
	m_sHomeDir = "";
	m_iYear    = 0;
	m_iMonth   = 0;
	m_iDay     = 0;
}

//---------------------------------------------------------------------------
// Constructor
TLog::TLog(UnicodeString sHomeDir)
{
	m_sHomeDir = sHomeDir;
	m_iYear    = 0;
	m_iMonth   = 0;
	m_iDay     = 0;
}

//---------------------------------------------------------------------------
// Destructor
TLog::~TLog()
{
	// nothing to do
}

//---------------------------------------------------------------------------
// Set Home Directory
void TLog::SetHomeDir(UnicodeString sHomeDir)
{
	m_sHomeDir = sHomeDir;

	bool bResult = false;

	if ( !IsExistDir(sHomeDir) ) {
		bResult = CreateDir(sHomeDir);

		if ( bResult ) {
			// nothing to do.. ( Directory exist )
		} else {
			//
		}
	} else {
		// nothing to do..
	}
}

//---------------------------------------------------------------------------
// Wiret Log Message in Log File
bool TLog::LogMessage(UnicodeString sMsg)
{
	sMsg += "\r\n";
	TDateTime t = Now();

	USHORT iYear, iMonth, iDay;
	USHORT iHour, iMinute, iSec, iMSec;

	t.DecodeDate(&iYear, &iMonth, &iDay);
	t.DecodeTime(&iHour, &iMinute, &iSec, &iMSec);

	if ( iYear != m_iYear || iMonth != m_iMonth ) {
		// 기존 Month와 다름
		UnicodeString sDir;
		bool bResult = false;
		sDir = sDir.sprintf(L"%04d%02d",iYear, iMonth);
		sDir = m_sHomeDir + "\\" + sDir;
		bResult = MakeDir(sDir);
		if ( !bResult ) return bResult;

		m_iYear = iYear;
		m_iMonth = iMonth;
	}

	AnsiString sLogMsg;
	//sLogMsg = sLogMsg.sprintf("[%02d:%02d:%02d]",iHour, iMinute, iSec);
	//sLogMsg += sMsg;
	sLogMsg = sMsg;

	AnsiString sFile;
	sFile = sFile.sprintf("%04d%02d\\%04d%02d%02d.log",iYear,iMonth,iYear,iMonth,iDay);
	sFile = m_sHomeDir + "\\" + sFile;

	THandle hFile = NULL;

	try {
		if ( FileExists(sFile) ) {
			hFile = FileOpen(sFile, fmOpenWrite | fmShareDenyNone);
			FileSeek(hFile, 0, 2);	// go To EOF
		} else {
			hFile = FileCreate(sFile, fmOpenWrite | fmShareDenyNone);
		}

		FileWrite(hFile, sLogMsg.c_str(), sLogMsg.Length());

	//} catch ( Exception &e ) {
		//nothing to do. .
	} __finally {
		if ( hFile != -1 ) FileClose(hFile);
	}

/*
	FILE *fp = NULL;
	errno_t error_no;

	error_no = fopen_s(&fp, sFile.c_str(),"a");
	if ( error_no != 0 ) return false;	// file open error
	if ( fp == NULL ) return false;	// log file error

	//fprintf(fp, "%s\n", sLogMsg.c_str());
	fwprintf(fp, L"%s\n", UnicodeString(sLogMsg).c_str());

	fclose(fp);
*/

	return true;
}

//---------------------------------------------------------------------------
// Create Directory
bool TLog::MakeDir(UnicodeString sDir)
{
	bool bResult = false;

	try {
		if ( IsExistDir(sDir) ) {
         	bResult = true;
		} else {
			bResult = CreateDir(sDir);
		}
	} catch ( Exception &e ) {
#ifdef _DEBUG
		ShowMessage(e.Message);
		bResult = false;
#endif
	}
	return bResult;
}

//---------------------------------------------------------------------------
// Create Log File
bool TLog::MakeFile(UnicodeString sFile)
{
	bool bResult = false;

	return bResult;
}

//---------------------------------------------------------------------------
// Is Exist of Direcotry
bool TLog::IsExistDir(UnicodeString sDir)
{
	bool bExist = false;
	bExist = DirectoryExists(sDir);
	return bExist;
}

//---------------------------------------------------------------------------
// Is Exist of File
bool TLog::IsExistFile(UnicodeString sFile)
{
	bool bExist = false;
	bExist = FileExists(sFile);
	return bExist;
}

#pragma package(smart_init)
