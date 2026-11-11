//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RUser.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRUser::TRUser()
{
	UserID    = "";
	UserName  = "";
	Password  = "";
	Privilege = "";
	Pos       = "";
	Company   = "";
	Phone     = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TRUser::~TRUser()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRUser::Clear()
{
	UserID    = "";
	UserName  = "";
	Password  = "";
	Privilege = "";
	Pos       = "";
	Company   = "";
	Phone     = "";
}

//---------------------------------------------------------------------------
bool TRUser::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RUser "
		" ( UserID, UserName, Password, Privilege, Pos, Company, Phone ) "
		" VALUES ( '" + UserID       + "', " +
		"          '" + UserName     + "', " +
		"          '" + Password     + "', " +
		"          '" + Privilege    + "', " +
		"          '" + Pos          + "', " +
		"          '" + Company      + "', " +
		"          '" + Phone        + "') ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRUser::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RUser "
		" SET UserName  = '" + UserName   + "', " +
		"     Passworkd = '" + Password  + "', " +
		"     Privilege = '" + Privilege  + "', " +
		"     Pos       = '" + Pos        + "', " +
		"     Company   = '" + Company    + "', " +
		"     Phone     = '" + Phone      + "'  " +
		" WHERE UserID  = '" + UserID     + "'  ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRUser::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RUser "
		" WHERE UserID  = '" + UserID     + "'  ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRUser::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RUser    "
		" WHERE UserID  = '" + UserID     + "'  ";

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
bool TRUser::Check(int iAction)
{
	bool bResult = false;
	if ( iAction == ACTION_INSERT ) {
		if ( UserID.IsEmpty() ) bResult = false;
		else                    bResult = true;
	} else if ( iAction == ACTION_UPDATE ) {
		if (  UserID.IsEmpty() ) bResult = false;
		else                     bResult = true;
	} else if ( iAction == ACTION_DELETE ) {
		if (  UserID.IsEmpty() ) bResult = false;
		else                     bResult = true;
	} else {
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
bool TRUser::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RUser "
		" WHERE UserID  = '" + UserID     + "'  ";

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
			UserID    = pQuery->FieldByName("UserID")->AsString;
			UserName  = pQuery->FieldByName("UserName")->AsString;
			Password  = pQuery->FieldByName("Password")->AsString;
			Privilege = pQuery->FieldByName("Privilege")->AsString;
			Pos       = pQuery->FieldByName("Pos")->AsString;
			Company   = pQuery->FieldByName("Company")->AsString;
			Phone     = pQuery->FieldByName("Phone")->AsString;
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
