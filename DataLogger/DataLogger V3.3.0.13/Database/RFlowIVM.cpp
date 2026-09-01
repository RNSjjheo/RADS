//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RFlowIVM.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRFlowIVM::TRFlowIVM()
{
	PointCode  = "";
	UseDate    = "";
	AMinVi     = 0;
	AMaxVi     = 0;
	AA         = 0;
	AB         = 0;
	AC         = 0;
	BMinVi     = 0;
	BMaxVi     = 0;
	BA         = 0;
	BB         = 0;
	BC         = 0;
}

//---------------------------------------------------------------------------
// Deconstructor
TRFlowIVM::~TRFlowIVM()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRFlowIVM::Clear()
{
	//PointCode  = "";
	//UseDate    = "";
	AMinVi     = 0;
	AMaxVi     = 0;
	AA         = 0;
	AB         = 0;
	AC         = 0;
	BMinVi     = 0;
	BMaxVi     = 0;
	BA         = 0;
	BB         = 0;
	BC         = 0;
}

//---------------------------------------------------------------------------
bool TRFlowIVM::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RFlowIVM "
		" ( PointCode, UseDate, AMinVi, AMaxVi, AA, AB, AC, BMinVi, BMaxVi, BA, BB, BC ) "
		" VALUES ( '" + PointCode  + "', " +
		"          '" + UseDate    + "', " +
		"           " + FloatToStr((Extended)AMinVi)     + " , " +
		"           " + FloatToStr((Extended)AMaxVi)     + " , " +
		"           " + FloatToStr((Extended)AA)         + " , " +
		"           " + FloatToStr((Extended)AB)         + " , " +
		"           " + FloatToStr((Extended)AC)         + " , " +
		"           " + FloatToStr((Extended)AMinVi)     + " , " +
		"           " + FloatToStr((Extended)AMaxVi)     + " , " +
		"           " + FloatToStr((Extended)AA)         + " , " +
		"           " + FloatToStr((Extended)AB)         + " , " +
		"           " + FloatToStr((Extended)AC)         + " ) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowIVM::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowIVM "
		" SET AMinVi  =  " + FloatToStr((Extended)AMinVi)     + " , " +
		"     AMaxVi  =  " + FloatToStr((Extended)AMaxVi)     + " , " +
		"     AA      =  " + FloatToStr((Extended)AA)         + " , " +
		"     AB      =  " + FloatToStr((Extended)AB)         + " , " +
		"     AC      =  " + FloatToStr((Extended)AC)         + " , " +
		"     BMinVi  =  " + FloatToStr((Extended)AMinVi)     + " , " +
		"     BMaxVi  =  " + FloatToStr((Extended)AMaxVi)     + " , " +
		"     BA      =  " + FloatToStr((Extended)AA)         + " , " +
		"     BB      =  " + FloatToStr((Extended)AB)         + " , " +
		"     BC      =  " + FloatToStr((Extended)AC)         + "   " +
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowIVM::UpdatePointCode(UnicodeString sPointCode)
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowIVM "
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
bool TRFlowIVM::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RFlowIVM "
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowIVM::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RFlowIVM    "
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
bool TRFlowIVM::Check(int iAction)
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
bool TRFlowIVM::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowIVM "
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
			AMinVi    = pQuery->FieldByName("AMinVi")->AsFloat;
			AMaxVi    = pQuery->FieldByName("AMaxVi")->AsFloat;
			AA        = pQuery->FieldByName("AA")->AsFloat;
			AB        = pQuery->FieldByName("AB")->AsFloat;
			AC        = pQuery->FieldByName("AC")->AsFloat;
			BMinVi    = pQuery->FieldByName("BMinVi")->AsFloat;
			BMaxVi    = pQuery->FieldByName("BMaxVi")->AsFloat;
			BA        = pQuery->FieldByName("BA")->AsFloat;
			BB        = pQuery->FieldByName("BB")->AsFloat;
			BC        = pQuery->FieldByName("BC")->AsFloat;
			pQuery->Next();
		} catch ( Exception &e ) {
			ShowMessage(e.Message);
			break;
		}
	}
	DM->Release(pQuery);

	return true;
}

//---------------------------------------------------------------------------
bool TRFlowIVM::GetData(UnicodeString sPointCode)
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowIVM "
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
			AMinVi    = pQuery->FieldByName("AMinVi")->AsFloat;
			AMaxVi    = pQuery->FieldByName("AMaxVi")->AsFloat;
			AA        = pQuery->FieldByName("AA")->AsFloat;
			AB        = pQuery->FieldByName("AB")->AsFloat;
			AC        = pQuery->FieldByName("AC")->AsFloat;
			BMinVi    = pQuery->FieldByName("BMinVi")->AsFloat;
			BMaxVi    = pQuery->FieldByName("BMaxVi")->AsFloat;
			BA        = pQuery->FieldByName("BA")->AsFloat;
			BB        = pQuery->FieldByName("BB")->AsFloat;
			BC        = pQuery->FieldByName("BC")->AsFloat;

			break; // 가장 최근 설정 데이터를 사용한다.
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
