//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RPoint.h"
//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"


UnicodeString PointStatus2Str(EN_POINT_STATUS enPointStatus)
{
	return ( enPointStatus == enPointInit)   ? "N" :
		   ((enPointStatus == enPointStart)  ? "S" :
		   ((enPointStatus == enPointFinish) ? "Y" :
		   ((enPointStatus == enPointEmpty)  ? "E" :
		   ((enPointStatus == enPointLeak)   ? "L" : "N"))));
}

//---------------------------------------------------------------------------
// Constructor
TRPoint::TRPoint()
{
	MeasureDate     = "";
	MeasureTime     = "";
	HydroMeter1Flag = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	HydroMeter2Flag = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	HydroMeter3Flag = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	WaterLevelFlag  = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	StreamFlag      = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	FileFlag        = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	ServerFlag      = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
	RnseaFlag       = "N";	// default ( not created ) => "Y"·Î º¯°æµÊ
}

//---------------------------------------------------------------------------
// Deconstructor
TRPoint::~TRPoint()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRPoint::Clear()
{
	MeasureDate     = "";
	MeasureTime     = "";
	HydroMeter1Flag = "N";
	HydroMeter2Flag = "N";
	HydroMeter3Flag = "N";
	WaterLevelFlag  = "N";
	StreamFlag      = "N";
	FileFlag        = "N";
	ServerFlag      = "N";
	RnseaFlag       = "N";
}

//---------------------------------------------------------------------------
bool TRPoint::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	if ( IsExist() ) return true;

	UnicodeString sSQL =
		" INSERT INTO RPoint "
		" ( MeasureDate,     MeasureTime,                      "
		"   HydroMeter1Flag, HydroMeter2Flag, HydroMeter3Flag, "
		"   WaterLevelFlag,  StreamFlag,                       "
		"   FileFlag,        ServerFlag,                       "
		"   RnseaFlag )                                        "
		" VALUES ( '" + MeasureDate     + "', " +
		"          '" + MeasureTime     + "', " +
		"          '" + HydroMeter1Flag + "', " +
		"          '" + HydroMeter2Flag + "', " +
		"          '" + HydroMeter3Flag + "', " +
		"          '" + WaterLevelFlag  + "', " +
		"          '" + StreamFlag      + "', " +
		"          '" + FileFlag        + "', " +
		"          '" + ServerFlag      + "', " +
		"          '" + RnseaFlag       + "') ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRPoint::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE RPoint "
		" SET HydroMeter1Flag = '" + HydroMeter1Flag + "', " +
		"     HydroMeter2Flag = '" + HydroMeter2Flag + "', " +
		"     HydroMeter3Flag = '" + HydroMeter3Flag + "', " +
		"     WaterLevelFlag  = '" + WaterLevelFlag  + "', " +
		"     StreamFlag      = '" + StreamFlag      + "', " +
		"     FileFlag        = '" + FileFlag        + "', " +
		"     ServerFlag      = '" + ServerFlag      + "', " +
		"     RnseaFlag       = '" + RnseaFlag       + "'  " +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRPoint::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM RPoint "
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRPoint::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM RPoint    "
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
bool TRPoint::Check(int iAction)
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
bool TRPoint::GetData()
{
	UnicodeString sSQL;

	sSQL = 	" SELECT * FROM RPoint "
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
			MeasureDate      = pQuery->FieldByName("MeasureDate")->AsString;
			MeasureTime      = pQuery->FieldByName("MeasureTime")->AsString;
			HydroMeter1Flag  = pQuery->FieldByName("HydroMeter1Flag")->AsString;
			HydroMeter2Flag  = pQuery->FieldByName("HydroMeter2Flag")->AsString;
			HydroMeter3Flag  = pQuery->FieldByName("HydroMeter3Flag")->AsString;
			WaterLevelFlag   = pQuery->FieldByName("WaterLevelFlag")->AsString;
			StreamFlag       = pQuery->FieldByName("StreamFlag")->AsString;
			FileFlag         = pQuery->FieldByName("FileFlag")->AsString;
			ServerFlag       = pQuery->FieldByName("ServerFlag")->AsString;
			RnseaFlag        = pQuery->FieldByName("RnseaFlag")->AsString;
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
bool TRPoint::UpdateWaterLevel(EN_POINT_STATUS enPointStatus)
{
	WaterLevelFlag = PointStatus2Str(enPointStatus);
	return UpdateStatus("WaterLevelFlag", WaterLevelFlag);
}

//---------------------------------------------------------------------------
bool TRPoint::UpdateHydroMeter1(EN_POINT_STATUS enPointStatus)
{
	HydroMeter1Flag = PointStatus2Str(enPointStatus);
	return UpdateStatus("HydroMeter1Flag", HydroMeter1Flag);
}

//---------------------------------------------------------------------------
bool TRPoint::UpdateHydroMeter2(EN_POINT_STATUS enPointStatus)
{
	HydroMeter2Flag = PointStatus2Str(enPointStatus);
	return UpdateStatus("HydroMeter2Flag", HydroMeter2Flag);
}

//---------------------------------------------------------------------------
bool TRPoint::UpdateHydroMeter3(EN_POINT_STATUS enPointStatus)
{
	HydroMeter3Flag = PointStatus2Str(enPointStatus);
	return UpdateStatus("HydroMeter3Flag", HydroMeter3Flag);
}


//---------------------------------------------------------------------------
bool TRPoint::UpdateFlowStream(EN_POINT_STATUS enPointStatus)
{
	StreamFlag = PointStatus2Str(enPointStatus);
	return UpdateStatus("StreamFlag", StreamFlag);
}

//---------------------------------------------------------------------------
bool TRPoint::UpdateStatus(UnicodeString sColumn, UnicodeString sStatus)
{
	bool bResult = false;
	UnicodeString sSQL = " UPDATE RPoint "
						 " SET " + sColumn + " = '" + sStatus + "' " +
						 " WHERE MeasureDate = '" + MeasureDate + "' AND " +
						 "       MeasureTime = '" + MeasureTime + "'     ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}


#pragma package(smart_init)
