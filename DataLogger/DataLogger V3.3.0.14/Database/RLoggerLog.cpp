//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RLoggerLog.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TLoggerLog::TLoggerLog()
{
	LogDateTime   = 0;
	LogMessage    = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TLoggerLog::~TLoggerLog()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TLoggerLog::Clear()
{
	LogDateTime   = 0;
	LogMessage    = "";
}

//---------------------------------------------------------------------------
bool TLoggerLog::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RLoggerLog "
		" ( LogDateTime, LogMessage ) "
		" VALUES ( CAST('" + LogDateTime.FormatString("yyyy-mm-dd hh:nn:ss.zzz") + "' AS TIMESTAMP) , " +
		"          '" + LogMessage + "' ) " ;

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TLoggerLog::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RLoggerLog "
		" SET LogMessage = '" + LogMessage + "' " +
		" WHERE LogDateTime = CAST('" + LogDateTime.FormatString("yyyy-mm-dd hh:nn:ss.zzz") + "' AS TIMESTAMP) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TLoggerLog::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RLoggerLog "
		" WHERE LogDateTime = CAST('" + LogDateTime.FormatString("yyyy-mm-dd hh:nn:ss.zzz") + "' AS TIMESTAMP) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TLoggerLog::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RLoggerLog    "
		" WHERE LogDateTime = CAST('" + LogDateTime.FormatString("yyyy-mm-dd hh:nn:ss.zzz") + "' AS TIMESTAMP) ";

	TSQLQuery *pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) return false;

	int iCnt = 0;
	while ( !pQuery->Eof ) {
		try {
			iCnt = pQuery->FieldByName("CNT")->AsInteger;
			pQuery->Next();
		} catch ( Exception &e ) {
#ifdef _DEBUG
			ShowMessage(e.Message);
#endif
			break;
		}
	}
	DM->Release(pQuery);

	if ( iCnt > 0 ) bResult = true;		// exist
	else            bResult = false;	// not exist

	return bResult;
}

//---------------------------------------------------------------------------
bool TLoggerLog::Check(int iAction)
{
	bool bResult = false;

	if ( iAction == ACTION_INSERT ) {
		if ( (int)LogDateTime == 0 ) bResult = false;
		else                    bResult = true;
	} else if ( iAction == ACTION_UPDATE ) {
		if ( (int)LogDateTime == 0 ) bResult = false;
		else                    bResult = true;
	} else if ( iAction == ACTION_DELETE ) {
		if ( (int)LogDateTime == 0 ) bResult = false;
		else                    bResult = true;
	} else {
		bResult = false;
	}

	return bResult;
}

//---------------------------------------------------------------------------
bool TLoggerLog::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM TLoggerLog "
		" WHERE LogDateTime = CAST('" + LogDateTime.FormatString("yyyy-mm-dd hh:nn:ss.zzz") + "' AS TIMESTAMP) ";

	TSQLQuery *pQuery = NULL;
	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) {
#ifdef _DEBUG
		ShowMessage("RSetup:GetData Error");
#endif
		return false;
	}

	while ( !pQuery->Eof ) {
		try {
			LogMessage      = pQuery->FieldByName("LogMessage")->AsString;
			pQuery->Next();
		} catch ( Exception &e ) {
			ShowMessage(e.Message);
			break;
		}
	}
	DM->Release(pQuery);

	return true;
}




#pragma package(smart_init)
