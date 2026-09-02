//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RSetup.h"
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRSetup::TRSetup()
{
	PK1     = 0;
	PK2     = 0;
	Value01 = "";
	Value02 = "";
	Value03 = "";
	Value04 = "";
	Value05 = "";
	Value06 = "";
	Value07 = "";
	Value08 = "";
	Value09 = "";
	Value10 = "";
	Value11 = "";
	Value12 = "";
	Value13 = "";
	Value14 = "";
	Value15 = "";
	Value16 = "";
	Value17 = "";
	Value18 = "";
	Value19 = "";
	Value20 = "";
	Des     = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TRSetup::~TRSetup()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRSetup::Clear()
{
	PK1     = 0;
	PK2     = 0;
	Value01 = "";
	Value02 = "";
	Value03 = "";
	Value04 = "";
	Value05 = "";
	Value06 = "";
	Value07 = "";
	Value08 = "";
	Value09 = "";
	Value10 = "";
	Value11 = "";
	Value12 = "";
	Value13 = "";
	Value14 = "";
	Value15 = "";
	Value16 = "";
	Value17 = "";
	Value18 = "";
	Value19 = "";
	Value20 = "";
	Des     = "";
}

//---------------------------------------------------------------------------
bool TRSetup::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RSetup "
		" ( PK1, PK2, "
		"   Value01, Value02, Value03, Value04, Value05, "
		"   Value06, Value07, Value08, Value09, Value10, "
		"   Value11, Value12, Value13, Value14, Value15, "
		"   Value16, Value17, Value18, Value19, Value20, Des ) "
		" VALUES ( " + IntToStr(PK1) + " , " +
		"          " + IntToStr(PK2) + " , " +
		"         '" + Value01       + "', " +
		"         '" + Value02       + "', " +
		"         '" + Value03       + "', " +
		"         '" + Value04       + "', " +
		"         '" + Value05       + "', " +
		"         '" + Value06       + "', " +
		"         '" + Value07       + "', " +
		"         '" + Value08       + "', " +
		"         '" + Value09       + "', " +
		"         '" + Value10       + "', " +
		"         '" + Value11       + "', " +
		"         '" + Value12       + "', " +
		"         '" + Value13       + "', " +
		"         '" + Value14       + "', " +
		"         '" + Value15       + "', " +
		"         '" + Value16       + "', " +
		"         '" + Value17       + "', " +
		"         '" + Value18       + "', " +
		"         '" + Value19       + "', " +
		"         '" + Value20       + "') ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRSetup::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RSetup "
		" SET Value01 = '" + Value01 + "', " +
		"     Value02 = '" + Value02 + "', " +
		"     Value03 = '" + Value03 + "', " +
		"     Value04 = '" + Value04 + "', " +
		"     Value05 = '" + Value05 + "', " +
		"     Value06 = '" + Value06 + "', " +
		"     Value07 = '" + Value07 + "', " +
		"     Value08 = '" + Value08 + "', " +
		"     Value09 = '" + Value09 + "', " +
		"     Value10 = '" + Value10 + "', " +
		"     Value11 = '" + Value11 + "', " +
		"     Value12 = '" + Value12 + "', " +
		"     Value13 = '" + Value13 + "', " +
		"     Value14 = '" + Value14 + "', " +
		"     Value15 = '" + Value15 + "', " +
		"     Value16 = '" + Value16 + "', " +
		"     Value17 = '" + Value17 + "', " +
		"     Value18 = '" + Value18 + "', " +
		"     Value19 = '" + Value19 + "', " +
		"     Value20 = '" + Value20 + "'  " +
		" WHERE PK1 = " + IntToStr(PK1) + " AND " +
		"       PK2 = " + IntToStr(PK2);

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRSetup::Update(UnicodeString sColumnName)
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString Value = "";
	if ( sColumnName == "Value01" ) Value = Value01;
	else if ( sColumnName == "Value02" ) Value = Value02;
	else if ( sColumnName == "Value03" ) Value = Value03;
	else if ( sColumnName == "Value04" ) Value = Value04;
	else if ( sColumnName == "Value05" ) Value = Value05;
	else if ( sColumnName == "Value06" ) Value = Value06;
	else if ( sColumnName == "Value07" ) Value = Value07;
	else if ( sColumnName == "Value08" ) Value = Value08;
	else if ( sColumnName == "Value09" ) Value = Value09;
	else if ( sColumnName == "Value10" ) Value = Value10;
	else if ( sColumnName == "Value11" ) Value = Value11;
	else if ( sColumnName == "Value12" ) Value = Value12;
	else if ( sColumnName == "Value13" ) Value = Value13;
	else if ( sColumnName == "Value14" ) Value = Value14;
	else if ( sColumnName == "Value15" ) Value = Value15;
	else if ( sColumnName == "Value16" ) Value = Value16;
	else if ( sColumnName == "Value17" ) Value = Value17;
	else if ( sColumnName == "Value18" ) Value = Value18;
	else if ( sColumnName == "Value19" ) Value = Value19;
	else if ( sColumnName == "Value20" ) Value = Value20;
	else                                 Value = "";

	UnicodeString sSQL =
		" UPDATE RSetup "
		" SET " + sColumnName + " = '" + Value + "' " +
		" WHERE PK1 = " + IntToStr(PK1) + " AND " +
		"       PK2 = " + IntToStr(PK2);

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRSetup::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RSetup "
		" WHERE PK1 = " + IntToStr(PK1) + " AND " +
		"       PK2 = " + IntToStr(PK2);

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRSetup::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RSetup    "
		" WHERE PK1 = " + IntToStr(PK1) + " AND " +
		"       PK2 = " + IntToStr(PK2);

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
bool TRSetup::Check(int iAction)
{
	bool bResult = false;
	if ( iAction == ACTION_INSERT ) {
		if ( PK1 < 1 || PK2 < 1 ) bResult = false;
		else                      bResult = true;
		//if ( Value01.IsEmpty() )  bResult = false;
		//else                      bResult = true;
	} else if ( iAction == ACTION_UPDATE ) {
		if ( PK1 < 1 || PK2 < 1 ) bResult = false;
		else                      bResult = true;
		//if ( Value01.IsEmpty() )  bResult = false;
		//else                      bResult = true;
	} else if ( iAction == ACTION_DELETE ) {
		if ( PK1 < 1 || PK2 < 1 ) bResult = false;
		else                      bResult = true;
	} else {
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
bool TRSetup::GetData()
{
	UnicodeString sSQL;

	sSQL = 	" SELECT * FROM RSetup "
			" WHERE PK1 = " + IntToStr(PK1) + " AND " +
			"       PK2 = " + IntToStr(PK2);

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
		PK1            = pQuery->FieldByName("PK1")->AsInteger;
		PK2            = pQuery->FieldByName("PK2")->AsInteger;
		Value01        = pQuery->FieldByName("Value01")->AsString;
		Value02        = pQuery->FieldByName("Value02")->AsString;
		Value03        = pQuery->FieldByName("Value03")->AsString;
		Value04        = pQuery->FieldByName("Value04")->AsString;
		Value05        = pQuery->FieldByName("Value05")->AsString;
		Value06        = pQuery->FieldByName("Value06")->AsString;
		Value07        = pQuery->FieldByName("Value07")->AsString;
		Value08        = pQuery->FieldByName("Value08")->AsString;
		Value09        = pQuery->FieldByName("Value09")->AsString;
		Value10        = pQuery->FieldByName("Value10")->AsString;
		Value11        = pQuery->FieldByName("Value11")->AsString;
		Value12        = pQuery->FieldByName("Value12")->AsString;
		Value13        = pQuery->FieldByName("Value13")->AsString;
		Value14        = pQuery->FieldByName("Value14")->AsString;
		Value15        = pQuery->FieldByName("Value15")->AsString;
		Value16        = pQuery->FieldByName("Value16")->AsString;
		Value17        = pQuery->FieldByName("Value17")->AsString;
		Value18        = pQuery->FieldByName("Value18")->AsString;
		Value19        = pQuery->FieldByName("Value19")->AsString;
		Value20        = pQuery->FieldByName("Value20")->AsString;
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
