//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RVTHLogger.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRVTHLogger::TRVTHLogger()
{
	MeasureDate      = "";
	MeasureTime      = "";
	Volt             = 0;
	Temperature      = 0;
	Humidity         = 0;
	DCCharge         = 0;
	DCBattery        = 0;
	Value01          = 0;
	Value02          = 0;
	Value03          = 0;
	Value04          = 0;
	Value05          = 0;
	Value06          = 0;
	Value07          = 0;
	Value08          = 0;
	Value09          = 0;
	Value10          = 0;
	RawData          = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TRVTHLogger::~TRVTHLogger()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRVTHLogger::Clear()
{
	MeasureDate      = "";
	MeasureTime      = "";
	Volt             = 0;
	Temperature      = 0;
	Humidity         = 0;
	DCCharge         = 0;
	DCBattery        = 0;
	Value01          = 0;
	Value02          = 0;
	Value03          = 0;
	Value04          = 0;
	Value05          = 0;
	Value06          = 0;
	Value07          = 0;
	Value08          = 0;
	Value09          = 0;
	Value10          = 0;
	RawData          = "";
}

//---------------------------------------------------------------------------
bool TRVTHLogger::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" INSERT INTO RPoint "
		" ( MeasureDate, MeasureTime,                    "
		"   Volt, Temperature, Humidity, DCCharge,       "
		"   DCBattery, Value01, Value02, Value03,        "
		"   Value04, Value05, Value06, Value07,          "
		"   Value08, Value09, Value10, RawData  )        "
		" VALUES ( '" + MeasureDate                      + "',  " +
		"          '" + MeasureTime                      + "',  " +
		"           " + FloatToStr((Extended)Volt)       + " ,  " +
		"           " + FloatToStr((Extended)Temperature)+ " ,  " +
		"           " + FloatToStr((Extended)Humidity)   + " ,  " +
		"           " + FloatToStr((Extended)DCCharge)   + " ,  " +
		"           " + FloatToStr((Extended)DCBattery)  + " ,  " +
		"           " + FloatToStr((Extended)Value01)    + " ,  " +
		"           " + FloatToStr((Extended)Value02)    + " ,  " +
		"           " + FloatToStr((Extended)Value03)    + " ,  " +
		"           " + FloatToStr((Extended)Value04)    + " ,  " +
		"           " + FloatToStr((Extended)Value05)    + " ,  " +
		"           " + FloatToStr((Extended)Value06)    + " ,  " +
		"           " + FloatToStr((Extended)Value07)    + " ,  " +
		"           " + FloatToStr((Extended)Value08)    + " ,  " +
		"           " + FloatToStr((Extended)Value09)    + " ,  " +
		"           " + FloatToStr((Extended)Value10)    + " ,  " +
		"          '" + RawData                          + "')  ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRVTHLogger::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RPoint "
		" SET Volt        =  " + FloatToStr((Extended)Volt)        + " , " +
		"     Temperature =  " + FloatToStr((Extended)Temperature) + " , " +
		"     Humidity    =  " + FloatToStr((Extended)Humidity)    + " , " +
		"     DCCharge    =  " + FloatToStr((Extended)DCCharge)    + " , " +
		"     DCBattery   =  " + FloatToStr((Extended)DCBattery)   + " , " +
		"     Value01     =  " + FloatToStr((Extended)Value01)     + " , " +
		"     Value02     =  " + FloatToStr((Extended)Value02)     + " , " +
		"     Value03     =  " + FloatToStr((Extended)Value03)     + " , " +
		"     Value04     =  " + FloatToStr((Extended)Value04)     + " , " +
		"     Value05     =  " + FloatToStr((Extended)Value05)     + " , " +
		"     Value06     =  " + FloatToStr((Extended)Value06)     + " , " +
		"     Value07     =  " + FloatToStr((Extended)Value07)     + " , " +
		"     Value08     =  " + FloatToStr((Extended)Value08)     + " , " +
		"     Value09     =  " + FloatToStr((Extended)Value09)     + " , " +
		"     Value10     =  " + FloatToStr((Extended)Value10)     + " , " +
		"     RawData     = '" + RawData                           + "'  " +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRVTHLogger::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RVTHLogger "
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRVTHLogger::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RVTHLogger    "
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

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
bool TRVTHLogger::Check(int iAction)
{
	bool bResult = false;
	if ( iAction == ACTION_INSERT ) {
		if ( MeasureDate.IsEmpty() || MeasureTime.IsEmpty() ) bResult = false;
		else                      bResult = true;
		//if ( Value01.IsEmpty() )  bResult = false;
		//else                      bResult = true;
	} else if ( iAction == ACTION_UPDATE ) {
		if ( MeasureDate.IsEmpty() || MeasureTime.IsEmpty() ) bResult = false;
		else                      bResult = true;
		//if ( Value01.IsEmpty() )  bResult = false;
		//else                      bResult = true;
	} else if ( iAction == ACTION_DELETE ) {
		if ( MeasureDate.IsEmpty() || MeasureTime.IsEmpty() ) bResult = false;
		else                      bResult = true;
	} else {
		bResult = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
bool TRVTHLogger::GetData()
{
	UnicodeString sSQL;

	sSQL = 	" SELECT * FROM RVTHLogger "
			" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
			"       MeasureTime   = '" + MeasureTime     + "'     ";

	TSQLQuery *pQuery = NULL;
	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) {
#ifdef _DEBUG
		ShowMessage("RVTHLogger:GetData Error");
#endif
		return false;
	}

	while ( !pQuery->Eof ) {
		try {
			Volt        = pQuery->FieldByName("Volt")->AsFloat;
			Temperature = pQuery->FieldByName("Temperature")->AsFloat;
			Humidity    = pQuery->FieldByName("Humidity")->AsFloat;
			DCCharge    = pQuery->FieldByName("DCCharge")->AsFloat;
			DCBattery   = pQuery->FieldByName("DCBattery")->AsFloat;
			Value01     = pQuery->FieldByName("Value01")->AsFloat;
			Value02     = pQuery->FieldByName("Value02")->AsFloat;
			Value03     = pQuery->FieldByName("Value03")->AsFloat;
			Value04     = pQuery->FieldByName("Value04")->AsFloat;
			Value05     = pQuery->FieldByName("Value05")->AsFloat;
			Value06     = pQuery->FieldByName("Value06")->AsFloat;
			Value07     = pQuery->FieldByName("Value07")->AsFloat;
			Value08     = pQuery->FieldByName("Value08")->AsFloat;
			Value09     = pQuery->FieldByName("Value09")->AsFloat;
			Value10     = pQuery->FieldByName("Value10")->AsFloat;
			RawData     = pQuery->FieldByName("RawData")->AsFloat;

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
