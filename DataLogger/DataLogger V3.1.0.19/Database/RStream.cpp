//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RStream.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRStream::TRStream()
{
	MeasureDate      = "";
	MeasureTime      = "";
	AvgStream        = 0;
	MinStream        = 0;
	MaxStream        = 0;
	WaterLevel       = 0;	// WaterLevel of measure point
	Area             = 0;	// Area of measure point
	Velocity         = 0;	// Average Velocity
}

//---------------------------------------------------------------------------
// Deconstructor
TRStream::~TRStream()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRStream::Clear()
{
	MeasureDate      = "";
	MeasureTime      = "";
	AvgStream        = 0;
	MinStream        = 0;
	MaxStream        = 0;
	WaterLevel       = 0;	// WaterLevel of measure point
	Area             = 0;	// Area of measure point
	Velocity         = 0;	// Average Velocity
}

//---------------------------------------------------------------------------
bool TRStream::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	bool bExist = IsExist();
	if ( bExist ) {
		bResult = Update();
		return bResult;
	}

	UnicodeString sSQL =
		" INSERT INTO RStream "
		" ( MeasureDate, MeasureTime,     "
		"   AvgStream, MinStream, MaxStream, WaterLevel, Area, Velocity ) "
		" VALUES ( '" + MeasureDate     + "', " +
		"          '" + MeasureTime     + "', " +
		"           " + FloatToStr((Extended)AvgStream)  + " , " +
		"           " + FloatToStr((Extended)MinStream)  + " , " +
		"           " + FloatToStr((Extended)MaxStream)  + " , " +
		"           " + FloatToStr((Extended)WaterLevel) + " , " +
		"           " + FloatToStr((Extended)Area)       + " , " +
		"           " + FloatToStr((Extended)Velocity)   + " ) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRStream::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RStream "
		" SET AvgStream  = " + FloatToStr((Extended)AvgStream)  + " , " +
		"     MinStream  = " + FloatToStr((Extended)MinStream)  + " , " +
		"     MaxStream  = " + FloatToStr((Extended)MaxStream)  + " , " +
		"     WaterLevel = " + FloatToStr((Extended)WaterLevel) + " , " +
		"     Area       = " + FloatToStr((Extended)Area)       + " , " +
		"     Velocity   = " + FloatToStr((Extended)Velocity)   + "   " +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRStream::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RStream "
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRStream::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RStream    "
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
bool TRStream::Check(int iAction)
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
bool TRStream::GetData()
{
	UnicodeString sSQL;

	sSQL = 	" SELECT * FROM RStream "
			" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
			"       MeasureTime   = '" + MeasureTime     + "'     ";

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
			MeasureDate  = pQuery->FieldByName("MeasureDate")->AsString;
			MeasureTime  = pQuery->FieldByName("MeasureTime")->AsString;
			AvgStream    = pQuery->FieldByName("AvgStream")->AsFloat;
			MinStream    = pQuery->FieldByName("MinStream")->AsFloat;
			MaxStream    = pQuery->FieldByName("MaxStream")->AsFloat;
			WaterLevel   = pQuery->FieldByName("WaterLevel")->AsFloat;
			Area         = pQuery->FieldByName("Area")->AsFloat;
			Velocity     = pQuery->FieldByName("Velocity")->AsFloat;
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
