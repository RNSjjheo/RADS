//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RFlowADVM.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRFlowADVM::TRFlowADVM()
{
	PointCode     = "";
	UseDate       = "";

	AXadvm        = 0;
	AZadvm        = 0;
	APan          = 0;
	ATilt         = 0;
	APhi          = 0;
	ASubmersion   = 0;
	AReverseMode  = 0;
	AUnitVelocity = 0;
	AVMax         = 0;
	AVMin         = 0;
	AStartCell    = 0;
	AEndCell      = 0;

	BXadvm        = 0;
	BZadvm        = 0;
	BPan          = 0;
	BTilt         = 0;
	BPhi          = 0;
	BSubmersion   = 0;
	BReverseMode  = 0;
	BUnitVelocity = 0;
	BVMax         = 0;
	BVMin         = 0;
	BStartCell    = 0;
	BEndCell      = 0;
	BType         = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TRFlowADVM::~TRFlowADVM()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRFlowADVM::Clear()
{
	//PointCode     = "";
	//UseDate       = "";

	AXadvm        = 0;
	AZadvm        = 0;
	APan          = 0;
	ATilt         = 0;
	APhi          = 0;
	ASubmersion   = 0;
	AReverseMode  = 0;
	AUnitVelocity = 0;
	AVMax         = 0;
	AVMin         = 0;
	AStartCell    = 0;
	AEndCell      = 0;

	BXadvm        = 0;
	BZadvm        = 0;
	BPan          = 0;
	BTilt         = 0;
	BPhi          = 0;
	BSubmersion   = 0;
	BReverseMode  = 0;
	BUnitVelocity = 0;
	BVMax         = 0;
	BVMin         = 0;
	BStartCell    = 0;
	BEndCell      = 0;
	BType         = "";
}

//---------------------------------------------------------------------------
bool TRFlowADVM::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RFlowADVM "
		" ( PointCode, UseDate, "
		"   AXadvm, AZadvm, APan, Atitlt, APhi, ASubmersion, AReverseMode, AUnitVelocity, AVMax, AVMin, AStartCell, AEndCell,  "
		"   BXadvm, BZadvm, BPan, Btitlt, BPhi, BSubmersion, BReverseMode, BUnitVelocity, BVMax, BVMin, BStartCell, BEndCell, BType ) "
		" VALUES ( '" + PointCode  + "', " +
		"          '" + UseDate    + "', " +
		"           " + FloatToStr((Extended)AXadvm)        + " , " +
		"           " + FloatToStr((Extended)AZadvm)        + " , " +
		"           " + FloatToStr((Extended)APan)          + " , " +
		"           " + FloatToStr((Extended)ATilt)         + " , " +
		"           " + FloatToStr((Extended)APhi)          + " , " +
		"           " + FloatToStr((Extended)ASubmersion)   + " , " +
		"           " + IntToStr(AReverseMode)              + " , " +
		"           " + IntToStr(AUnitVelocity)             + " , " +
		"           " + FloatToStr((Extended)AVMax)         + " , " +
		"           " + FloatToStr((Extended)AVMin)         + " , " +
		"           " + IntToStr((int)AStartCell)           + " , " +
		"           " + IntToStr((int)AEndCell)             + " , " +
		"           " + FloatToStr((Extended)BXadvm)        + " , " +
		"           " + FloatToStr((Extended)BZadvm)        + " , " +
		"           " + FloatToStr((Extended)BPan)          + " , " +
		"           " + FloatToStr((Extended)BTilt)         + " , " +
		"           " + FloatToStr((Extended)BPhi)          + " , " +
		"           " + FloatToStr((Extended)BSubmersion)   + " , " +
		"           " + IntToStr(BReverseMode)              + " , " +
		"           " + IntToStr(BUnitVelocity)             + " , " +
		"           " + FloatToStr((Extended)BVMax)         + " , " +
		"           " + FloatToStr((Extended)BVMin)         + " , " +
		"           " + IntToStr((int)BStartCell)           + " , " +
		"           " + IntToStr((int)BEndCell)             + " , " +
		"          '" + BType                               + "') ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowADVM::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowADVM "
		" SET AXadvm        =  " + FloatToStr((Extended)AXadvm)        + " , " +
		"     AZadvm        =  " + FloatToStr((Extended)AZadvm)        + " , " +
		"     APan          =  " + FloatToStr((Extended)APan)          + " , " +
		"     Atilt         =  " + FloatToStr((Extended)ATilt)         + " , " +
		"     APhi          =  " + FloatToStr((Extended)APhi)          + " , " +
		"     ASubmersion   =  " + FloatToStr((Extended)ASubmersion)   + " , " +
		"     AReverseMode  =  " + IntToStr(AReverseMode)              + " , " +
		"     AUnitVelocity =  " + IntToStr(AUnitVelocity)             + " , " +
		"     AVMax         =  " + FloatToStr((Extended)AVMax)         + " , " +
		"     AVMin         =  " + FloatToStr((Extended)AVMin)         + " , " +
		"     AStartCell    =  " + IntToStr((int)AStartCell)           + " , " +
		"     AEndCell      =  " + IntToStr((int)AEndCell)             + " , " +
		"     BXadvm        =  " + FloatToStr((Extended)BXadvm)        + " , " +
		"     BZadvm        =  " + FloatToStr((Extended)BZadvm)        + " , " +
		"     BPan          =  " + FloatToStr((Extended)BPan)          + " , " +
		"     BTilt         =  " + FloatToStr((Extended)BTilt)         + " , " +
		"     BPhi          =  " + FloatToStr((Extended)BPhi)          + " , " +
		"     BSubmersion   =  " + FloatToStr((Extended)BSubmersion)   + " , " +
		"     BReverseMode  =  " + IntToStr(BReverseMode)              + " , " +
		"     BUnitVelocity =  " + IntToStr(BUnitVelocity)             + " , " +
		"     BVMax         =  " + FloatToStr((Extended)BVMax)         + " , " +
		"     BVMin         =  " + FloatToStr((Extended)BVMin)         + " , " +
		"     BStartCell    =  " + IntToStr((int)BStartCell)           + " , " +
		"     BEndCell      =  " + IntToStr((int)BEndCell)             + " , " +
		"     BType         = '" + BType                               + "'  " +
		" WHERE PointCode   = '" + PointCode + "' AND " +
		"       UseDate     = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowADVM::UpdatePointCode(UnicodeString sPointCode)
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RFlowADVM "
		" SET PointCode     = '" + sPointCode + "'     " +
		" WHERE PointCode   = '" + PointCode  + "' AND " +
		"       UseDate     = '" + UseDate    + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowADVM::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RFlowADVM "
		" WHERE PointCode = '" + PointCode + "' AND " +
		"       UseDate   = '" + UseDate   + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRFlowADVM::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RFlowADVM    "
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
bool TRFlowADVM::Check(int iAction)
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
bool TRFlowADVM::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowADVM "
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
			PointCode     = pQuery->FieldByName("PointCode")->AsString;
			UseDate       = pQuery->FieldByName("UseDate")->AsString;
			AXadvm        = pQuery->FieldByName("AXadvm")->AsFloat;
			AZadvm        = pQuery->FieldByName("AZadvm")->AsFloat;
			APan          = pQuery->FieldByName("APan")->AsFloat;
			ATilt         = pQuery->FieldByName("ATilt")->AsFloat;
			APhi          = pQuery->FieldByName("APhi")->AsFloat;
			ASubmersion   = pQuery->FieldByName("ASubmersion")->AsFloat;
			AReverseMode  = pQuery->FieldByName("AReverseMode")->AsInteger;
			AUnitVelocity = pQuery->FieldByName("AUnitVelocity")->AsInteger;
			AVMax         = pQuery->FieldByName("AVMax")->AsFloat;
			AVMin         = pQuery->FieldByName("AVMin")->AsFloat;
			AStartCell    = pQuery->FieldByName("AStartCell")->AsFloat;
			AEndCell      = pQuery->FieldByName("AEndCell")->AsFloat;
			BXadvm        = pQuery->FieldByName("BXadvm")->AsFloat;
			BZadvm        = pQuery->FieldByName("BZadvm")->AsFloat;
			BPan          = pQuery->FieldByName("BPan")->AsFloat;
			BTilt         = pQuery->FieldByName("BTilt")->AsFloat;
			BPhi          = pQuery->FieldByName("BPhi")->AsFloat;
			BSubmersion   = pQuery->FieldByName("BSubmersion")->AsFloat;
			BReverseMode  = pQuery->FieldByName("BReverseMode")->AsInteger;
			BUnitVelocity = pQuery->FieldByName("BUnitVelocity")->AsInteger;
			BVMax         = pQuery->FieldByName("BVMax")->AsFloat;
			BVMin         = pQuery->FieldByName("BVMin")->AsFloat;
			BStartCell    = pQuery->FieldByName("BStartCell")->AsFloat;
			BEndCell      = pQuery->FieldByName("BEndCell")->AsFloat;
			BType         = pQuery->FieldByName("BType")->AsString;

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
bool TRFlowADVM::GetData(UnicodeString sPointCode)
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM RFlowADVM "
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
			PointCode     = pQuery->FieldByName("PointCode")->AsString;
			UseDate       = pQuery->FieldByName("UseDate")->AsString;
			AXadvm        = pQuery->FieldByName("AXadvm")->AsFloat;
			AZadvm        = pQuery->FieldByName("AZadvm")->AsFloat;
			APan          = pQuery->FieldByName("APan")->AsFloat;
			ATilt         = pQuery->FieldByName("ATilt")->AsFloat;
			APhi          = pQuery->FieldByName("APhi")->AsFloat;
			ASubmersion   = pQuery->FieldByName("ASubmersion")->AsFloat;
			AReverseMode  = pQuery->FieldByName("AReverseMode")->AsInteger;
			AUnitVelocity = pQuery->FieldByName("AUnitVelocity")->AsInteger;
			AVMax         = pQuery->FieldByName("AVMax")->AsFloat;
			AVMin         = pQuery->FieldByName("AVMin")->AsFloat;
			AStartCell    = pQuery->FieldByName("AStartCell")->AsFloat;
			AEndCell      = pQuery->FieldByName("AEndCell")->AsFloat;
			BXadvm        = pQuery->FieldByName("BXadvm")->AsFloat;
			BZadvm        = pQuery->FieldByName("BZadvm")->AsFloat;
			BPan          = pQuery->FieldByName("BPan")->AsFloat;
			BTilt         = pQuery->FieldByName("BTilt")->AsFloat;
			BPhi          = pQuery->FieldByName("BPhi")->AsFloat;
			BSubmersion   = pQuery->FieldByName("BSubmersion")->AsFloat;
			BReverseMode  = pQuery->FieldByName("BReverseMode")->AsInteger;
			BUnitVelocity = pQuery->FieldByName("BUnitVelocity")->AsInteger;
			BVMax         = pQuery->FieldByName("BVMax")->AsFloat;
			BVMin         = pQuery->FieldByName("BVMin")->AsFloat;
			BStartCell    = pQuery->FieldByName("BStartCell")->AsFloat;
			BEndCell      = pQuery->FieldByName("BEndCell")->AsFloat;
			BType         = pQuery->FieldByName("BType")->AsString;

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
