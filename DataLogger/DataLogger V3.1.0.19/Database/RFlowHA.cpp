//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RFlowHA.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRFlowHA::TRFlowHA()
{
	PointCode  = "";
	UseDate    = "";
	MinH       = 0;
	MaxH       = 0;
	A6         = 0;
	A5         = 0;
	A4         = 0;
	A3         = 0;
	A2         = 0;
	A1         = 0;
	A0         = 0;
}

//---------------------------------------------------------------------------
// Deconstructor
TRFlowHA::~TRFlowHA()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRFlowHA::Clear()
{
	//PointCode  = "";
	//UseDate    = "";
	MinH       = 0;
	MaxH       = 0;
	A6         = 0;
	A5         = 0;
	A4         = 0;
	A3         = 0;
	A2         = 0;
	A1         = 0;
	A0         = 0;
}

//---------------------------------------------------------------------------
bool TRFlowHA::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RFlowHA "
		" ( PointCode, UseDate, MinH, MaxH, A6, A5, A4, A3, A2, A1, A0 ) "
		" VALUES ( '" + PointCode  + "', " +
		"          '" + UseDate    + "', " +
		"           " + FloatToStr((Extended)MinH)   + " , " +
		"           " + FloatToStr((Extended)MaxH)   + " , " +
		"           " + FloatToStr((Extended)A6)     + " , " +
		"           " + FloatToStr((Extended)A5)     + " , " +
		"           " + FloatToStr((Extended)A4)     + " , " +
		"           " + FloatToStr((Extended)A3)     + " , " +
		"           " + FloatToStr((Extended)A2)     + " , " +
		"           " + FloatToStr((Extended)A1)     + " , " +
		"           " + FloatToStr((Extended)A0)     + " ) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowHA::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowHA "
		" SET MinH = " + FloatToStr((Extended)MinH)   + " , " +
		"     MaxH = " + FloatToStr((Extended)MaxH)   + " , " +
		"     A6   = " + FloatToStr((Extended)A6)     + " , " +
		"     A5   = " + FloatToStr((Extended)A5)     + " , " +
		"     A4   = " + FloatToStr((Extended)A4)     + " , " +
		"     A3   = " + FloatToStr((Extended)A3)     + " , " +
		"     A2   = " + FloatToStr((Extended)A2)     + " , " +
		"     A1   = " + FloatToStr((Extended)A1)     + " , " +
		"     A0   = " + FloatToStr((Extended)A0)     + "   " +
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowHA::UpdatePointCode(UnicodeString sPointCode)
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowHA "
		" SET PointCode   = '" + sPointCode + "'     " +
		" WHERE PointCode = '" + PointCode  + "' AND " +
		"       UseDate   = '" + UseDate    + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowHA::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RFlowHA "
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowHA::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RFlowHA    "
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

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
bool TRFlowHA::Check(int iAction)
{
	bool bResult = false;
	if ( iAction == ACTION_INSERT ) {
		if ( PointCode.IsEmpty() || UseDate.IsEmpty() ) bResult = false;
		else                    bResult = true;
	} else if ( iAction == ACTION_UPDATE ) {
		if (  PointCode.IsEmpty() || UseDate.IsEmpty() ) bResult = false;
		else                     bResult = true;
	} else if ( iAction == ACTION_DELETE ) {
		if (  PointCode.IsEmpty() || UseDate.IsEmpty() ) bResult = false;
		else                     bResult = true;
	} else {
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowHA::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowHA "
		" WHERE PointCode = '" + PointCode + "' " +
		" ORDER BY UseDate DESC ";

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
			PointCode = pQuery->FieldByName("PointCode")->AsString;
			UseDate   = pQuery->FieldByName("UseDate")->AsString;
			MinH      = pQuery->FieldByName("MinH")->AsFloat;
			MaxH      = pQuery->FieldByName("MaxH")->AsFloat;
			A6        = pQuery->FieldByName("A6")->AsFloat;
			A5        = pQuery->FieldByName("A5")->AsFloat;
			A4        = pQuery->FieldByName("A4")->AsFloat;
			A3        = pQuery->FieldByName("A3")->AsFloat;
			A2        = pQuery->FieldByName("A2")->AsFloat;
			A1        = pQuery->FieldByName("A1")->AsFloat;
			A0        = pQuery->FieldByName("A0")->AsFloat;

			break;
			//pQuery->Next();
		} catch ( Exception &e ) {
			ShowMessage(e.Message);
			break;
		}
	}
	DM->Release(pQuery);

	return true;
}

//---------------------------------------------------------------------------
bool TRFlowHA::GetData(UnicodeString sPointCode)
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowHA "
		" WHERE PointCode = '" + sPointCode + "' " +
		" ORDER BY UseDate DESC ";

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
			PointCode = pQuery->FieldByName("PointCode")->AsString;
			UseDate   = pQuery->FieldByName("UseDate")->AsString;
			MinH      = pQuery->FieldByName("MinH")->AsFloat;
			MaxH      = pQuery->FieldByName("MaxH")->AsFloat;
			A6        = pQuery->FieldByName("A6")->AsFloat;
			A5        = pQuery->FieldByName("A5")->AsFloat;
			A4        = pQuery->FieldByName("A4")->AsFloat;
			A3        = pQuery->FieldByName("A3")->AsFloat;
			A2        = pQuery->FieldByName("A2")->AsFloat;
			A1        = pQuery->FieldByName("A1")->AsFloat;
			A0        = pQuery->FieldByName("A0")->AsFloat;

			break;
			//pQuery->Next();
		} catch ( Exception &e ) {
			ShowMessage(e.Message);
			break;
		}
	}
	DM->Release(pQuery);

	return true;
}

#pragma package(smart_init)
