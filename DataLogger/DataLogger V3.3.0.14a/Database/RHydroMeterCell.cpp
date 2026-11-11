//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RHydroMeterCell.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//#include "UnitDM.h"
//#include "Define.h"

//---------------------------------------------------------------------------
// Constructor
TRHydroMeterCell::TRHydroMeterCell(EN_HYDRO_NO enHydroNo)
{
	this->enHydroNo = enHydroNo;
	MeasureDate = "";
	MeasureTime = "";
	CellNo      = 0;
	Value01     = 0;
	Value02     = 0;
	Value03     = 0;
	Value04     = 0;
	Value05     = 0;
	Value06     = 0;
	Value07     = 0;
	Value08     = 0;
	Value09     = 0;
	Value10     = 0;
	Value11     = 0;
	Value12     = 0;
	Value13     = 0;
	Value14     = 0;
	Value15     = 0;
	Value16     = 0;
	Value17     = 0;
	Value18     = 0;
	Value19     = 0;
	Value20     = 0;
	Value21     = 0;
	Value22     = 0;
	Value23     = 0;
	Value24     = 0;
	Value25     = 0;
	Value26     = 0;
	Value27     = 0;
	Value28     = 0;
	Value29     = 0;
	Value30     = 0;
	Value31     = 0;
	Value32     = 0;
	Value33     = 0;
	Value34     = 0;
	Value35     = 0;
	Value36     = 0;
	Value37     = 0;
	Value38     = 0;
	Value39     = 0;
	Value40     = 0;
}

//---------------------------------------------------------------------------
// Deconstructor
TRHydroMeterCell::~TRHydroMeterCell()
{
	// nothing to do
}

//---------------------------------------------------------------------------
void TRHydroMeterCell::Clear()
{
	//First       = true;
	MeasureDate = "";
	MeasureTime = "";
	CellNo      = 0;
	Value01     = 0;
	Value02     = 0;
	Value03     = 0;
	Value04     = 0;
	Value05     = 0;
	Value06     = 0;
	Value07     = 0;
	Value08     = 0;
	Value09     = 0;
	Value10     = 0;
	Value11     = 0;
	Value12     = 0;
	Value13     = 0;
	Value14     = 0;
	Value15     = 0;
	Value16     = 0;
	Value17     = 0;
	Value18     = 0;
	Value19     = 0;
	Value20     = 0;
	Value21     = 0;
	Value22     = 0;
	Value23     = 0;
	Value24     = 0;
	Value25     = 0;
	Value26     = 0;
	Value27     = 0;
	Value28     = 0;
	Value29     = 0;
	Value30     = 0;
	Value31     = 0;
	Value32     = 0;
	Value33     = 0;
	Value34     = 0;
	Value35     = 0;
	Value36     = 0;
	Value37     = 0;
	Value38     = 0;
	Value39     = 0;
	Value40     = 0;
}

//---------------------------------------------------------------------------
bool TRHydroMeterCell::Insert()
{
	bool bResult = false;
	bResult = Check(ACTION_INSERT);
	if ( !bResult) return bResult;

	if ( IsExist() ) {
		bResult = Update();
		return bResult;
	}

	UnicodeString sSQL =
		" INSERT INTO " + GetTable() +
		" ( MeasureDate,     MeasureTime,  CellNo,   "
		"   Value01, Value02, Value03, Value04, Value05, "
		"   Value06, Value07, Value08, Value09, Value10, "
		"   Value11, Value12, Value13, Value14, Value15, "
		"   Value16, Value17, Value18, Value19, Value20, "
		"   Value21, Value22, Value23, Value24, Value25, "
		"   Value26, Value27, Value28, Value29, Value30, "
		"   Value31, Value32, Value33, Value34, Value35, "
		"   Value36, Value37, Value38, Value39, Value40 ) "
		" VALUES ( '" + MeasureDate      + "', " +
		"          '" + MeasureTime      + "', " +
		"           " + IntToStr(CellNo) + " , " +
		"           " + FloatToStr((Extended)Value01) + " , " +
		"           " + FloatToStr((Extended)Value02) + " , " +
		"           " + FloatToStr((Extended)Value03) + " , " +
		"           " + FloatToStr((Extended)Value04) + " , " +
		"           " + FloatToStr((Extended)Value05) + " , " +
		"           " + FloatToStr((Extended)Value06) + " , " +
		"           " + FloatToStr((Extended)Value07) + " , " +
		"           " + FloatToStr((Extended)Value08) + " , " +
		"           " + FloatToStr((Extended)Value09) + " , " +
		"           " + FloatToStr((Extended)Value10) + " , " +
		"           " + FloatToStr((Extended)Value11) + " , " +
		"           " + FloatToStr((Extended)Value12) + " , " +
		"           " + FloatToStr((Extended)Value13) + " , " +
		"           " + FloatToStr((Extended)Value14) + " , " +
		"           " + FloatToStr((Extended)Value15) + " , " +
		"           " + FloatToStr((Extended)Value16) + " , " +
		"           " + FloatToStr((Extended)Value17) + " , " +
		"           " + FloatToStr((Extended)Value18) + " , " +
		"           " + FloatToStr((Extended)Value19) + " , " +
		"           " + FloatToStr((Extended)Value20) + " , " +
		"           " + FloatToStr((Extended)Value21) + " , " +
		"           " + FloatToStr((Extended)Value22) + " , " +
		"           " + FloatToStr((Extended)Value23) + " , " +
		"           " + FloatToStr((Extended)Value24) + " , " +
		"           " + FloatToStr((Extended)Value25) + " , " +
		"           " + FloatToStr((Extended)Value26) + " , " +
		"           " + FloatToStr((Extended)Value27) + " , " +
		"           " + FloatToStr((Extended)Value28) + " , " +
		"           " + FloatToStr((Extended)Value29) + " , " +
		"           " + FloatToStr((Extended)Value30) + " , " +
		"           " + FloatToStr((Extended)Value31) + " , " +
		"           " + FloatToStr((Extended)Value32) + " , " +
		"           " + FloatToStr((Extended)Value33) + " , " +
		"           " + FloatToStr((Extended)Value34) + " , " +
		"           " + FloatToStr((Extended)Value35) + " , " +
		"           " + FloatToStr((Extended)Value36) + " , " +
		"           " + FloatToStr((Extended)Value37) + " , " +
		"           " + FloatToStr((Extended)Value38) + " , " +
		"           " + FloatToStr((Extended)Value39) + " , " +
		"           " + FloatToStr((Extended)Value40) + " ) ";

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRHydroMeterCell::Update()
{
	bool bResult = false;
	bResult = Check(ACTION_UPDATE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" UPDATE " + GetTable() +
		" SET Value01  =  " + FloatToStr((Extended)Value01) + " , " +
		"     Value02  =  " + FloatToStr((Extended)Value02) + " , " +
		"     Value03  =  " + FloatToStr((Extended)Value03) + " , " +
		"     Value04  =  " + FloatToStr((Extended)Value04) + " , " +
		"     Value05  =  " + FloatToStr((Extended)Value05) + " , " +
		"     Value06  =  " + FloatToStr((Extended)Value06) + " , " +
		"     Value07  =  " + FloatToStr((Extended)Value07) + " , " +
		"     Value08  =  " + FloatToStr((Extended)Value08) + " , " +
		"     Value09  =  " + FloatToStr((Extended)Value09) + " , " +
		"     Value10  =  " + FloatToStr((Extended)Value10) + " , " +
		"     Value11  =  " + FloatToStr((Extended)Value01) + " , " +
		"     Value12  =  " + FloatToStr((Extended)Value02) + " , " +
		"     Value13  =  " + FloatToStr((Extended)Value03) + " , " +
		"     Value14  =  " + FloatToStr((Extended)Value04) + " , " +
		"     Value15  =  " + FloatToStr((Extended)Value05) + " , " +
		"     Value16  =  " + FloatToStr((Extended)Value06) + " , " +
		"     Value17  =  " + FloatToStr((Extended)Value07) + " , " +
		"     Value18  =  " + FloatToStr((Extended)Value08) + " , " +
		"     Value19  =  " + FloatToStr((Extended)Value09) + " , " +
		"     Value20  =  " + FloatToStr((Extended)Value10) + " , " +
		"     Value21  =  " + FloatToStr((Extended)Value01) + " , " +
		"     Value22  =  " + FloatToStr((Extended)Value02) + " , " +
		"     Value23  =  " + FloatToStr((Extended)Value03) + " , " +
		"     Value24  =  " + FloatToStr((Extended)Value04) + " , " +
		"     Value25  =  " + FloatToStr((Extended)Value05) + " , " +
		"     Value26  =  " + FloatToStr((Extended)Value06) + " , " +
		"     Value27  =  " + FloatToStr((Extended)Value07) + " , " +
		"     Value28  =  " + FloatToStr((Extended)Value08) + " , " +
		"     Value29  =  " + FloatToStr((Extended)Value09) + " , " +
		"     Value30  =  " + FloatToStr((Extended)Value10) + " , " +
		"     Value31  =  " + FloatToStr((Extended)Value01) + " , " +
		"     Value32  =  " + FloatToStr((Extended)Value02) + " , " +
		"     Value33  =  " + FloatToStr((Extended)Value03) + " , " +
		"     Value34  =  " + FloatToStr((Extended)Value04) + " , " +
		"     Value35  =  " + FloatToStr((Extended)Value05) + " , " +
		"     Value36  =  " + FloatToStr((Extended)Value06) + " , " +
		"     Value37  =  " + FloatToStr((Extended)Value07) + " , " +
		"     Value38  =  " + FloatToStr((Extended)Value08) + " , " +
		"     Value39  =  " + FloatToStr((Extended)Value09) + " , " +
		"     Value40  =  " + FloatToStr((Extended)Value10) + "   " +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "' AND " +
		"       CellNo        =  " + IntToStr(CellNo);

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRHydroMeterCell::Delete()
{
	bool bResult = false;
	bResult = Check(ACTION_DELETE);
	if ( !bResult) return bResult;

	UnicodeString sSQL =
		" DELETE FROM " + GetTable() +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "' AND " +
		"       CellNo        =  " + IntToStr(CellNo);

	int iRow = 0;
	iRow = DM->ExecSQL(sSQL);
	if ( iRow < 1 ) bResult = false;
	else            bResult = true;

	return bResult;
}

//---------------------------------------------------------------------------
bool TRHydroMeterCell::IsExist()
{
	bool bResult = false;

	UnicodeString sSQL =
		" SELECT COUNT(*) AS CNT FROM " + GetTable() +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "' AND " +
		"       CellNo        =  " + IntToStr(CellNo);

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
bool TRHydroMeterCell::Check(int iAction)
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
bool TRHydroMeterCell::GetData()
{
	UnicodeString sSQL;

	sSQL =
		" SELECT * FROM " + GetTable() +
		" WHERE MeasureDate   = '" + MeasureDate     + "' AND " +
		"       MeasureTime   = '" + MeasureTime     + "' AND " +
		"       CellNo        =  " + IntToStr(CellNo);

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
			CellNo           = pQuery->FieldByName("CellNo")->AsInteger;
			Value01          = pQuery->FieldByName("Value01")->AsFloat;
			Value02          = pQuery->FieldByName("Value02")->AsFloat;
			Value03          = pQuery->FieldByName("Value03")->AsFloat;
			Value04          = pQuery->FieldByName("Value04")->AsFloat;
			Value05          = pQuery->FieldByName("Value05")->AsFloat;
			Value06          = pQuery->FieldByName("Value06")->AsFloat;
			Value07          = pQuery->FieldByName("Value07")->AsFloat;
			Value08          = pQuery->FieldByName("Value08")->AsFloat;
			Value09          = pQuery->FieldByName("Value09")->AsFloat;
			Value10          = pQuery->FieldByName("Value10")->AsFloat;
			Value11          = pQuery->FieldByName("Value11")->AsFloat;
			Value12          = pQuery->FieldByName("Value12")->AsFloat;
			Value13          = pQuery->FieldByName("Value13")->AsFloat;
			Value14          = pQuery->FieldByName("Value14")->AsFloat;
			Value15          = pQuery->FieldByName("Value15")->AsFloat;
			Value16          = pQuery->FieldByName("Value16")->AsFloat;
			Value17          = pQuery->FieldByName("Value17")->AsFloat;
			Value18          = pQuery->FieldByName("Value18")->AsFloat;
			Value19          = pQuery->FieldByName("Value19")->AsFloat;
			Value20          = pQuery->FieldByName("Value20")->AsFloat;
			Value21          = pQuery->FieldByName("Value21")->AsFloat;
			Value22          = pQuery->FieldByName("Value22")->AsFloat;
			Value23          = pQuery->FieldByName("Value23")->AsFloat;
			Value24          = pQuery->FieldByName("Value24")->AsFloat;
			Value25          = pQuery->FieldByName("Value25")->AsFloat;
			Value26          = pQuery->FieldByName("Value26")->AsFloat;
			Value27          = pQuery->FieldByName("Value27")->AsFloat;
			Value28          = pQuery->FieldByName("Value28")->AsFloat;
			Value29          = pQuery->FieldByName("Value29")->AsFloat;
			Value30          = pQuery->FieldByName("Value30")->AsFloat;
			Value31          = pQuery->FieldByName("Value31")->AsFloat;
			Value32          = pQuery->FieldByName("Value32")->AsFloat;
			Value33          = pQuery->FieldByName("Value33")->AsFloat;
			Value34          = pQuery->FieldByName("Value34")->AsFloat;
			Value35          = pQuery->FieldByName("Value35")->AsFloat;
			Value36          = pQuery->FieldByName("Value36")->AsFloat;
			Value37          = pQuery->FieldByName("Value37")->AsFloat;
			Value38          = pQuery->FieldByName("Value38")->AsFloat;
			Value39          = pQuery->FieldByName("Value39")->AsFloat;
			Value40          = pQuery->FieldByName("Value40")->AsFloat;

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
UnicodeString TRHydroMeterCell::GetTable()
{
	return ((enHydroNo == enHydroFirst)  ? "RHydroMeter1Cell" :
		   ((enHydroNo == enHydroSecond) ? "RHydroMeter2Cell" : "RHydroMeter3Cell"));
}

//////////////////////////////////////////////////////////////////////////////
//
// TRHydroMeterCellContainer
//
//////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TRHydroMeterCellContainer::TRHydroMeterCellContainer()
{
	m_pList = new TList();
}

//---------------------------------------------------------------------------
TRHydroMeterCellContainer::~TRHydroMeterCellContainer()
{
	TRHydroMeterCell *pCell = NULL;
	for ( int i = 0 ; i < m_pList->Count ; i++ ) {
		pCell = (TRHydroMeterCell *) m_pList->Items[i];
		delete pCell;
	}
	m_pList->Clear();

	delete m_pList;
}

//---------------------------------------------------------------------------
void TRHydroMeterCellContainer::Clear()
{
	TRHydroMeterCell *pCell = NULL;
	for ( int i = 0 ; i < m_pList->Count ; i++ ) {
		pCell = (TRHydroMeterCell *) m_pList->Items[i];
		delete pCell;
	}
	m_pList->Clear();
}

//---------------------------------------------------------------------------
void TRHydroMeterCellContainer::Add(TRHydroMeterCell *pRHydroMeterCell)
{
	if ( m_pList != NULL ) {
		m_enHydroNo = pRHydroMeterCell->enHydroNo;
		m_pList->Add(pRHydroMeterCell);
	}
}

//---------------------------------------------------------------------------
bool TRHydroMeterCellContainer::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	if ( m_pList->Count == 0 ) return true;

	TRHydroMeterCell RHydroMeterCell;
	RHydroMeterCell.MeasureDate = MeasureDate;
	RHydroMeterCell.MeasureTime = MeasureTime;
	RHydroMeterCell.enHydroNo   = m_enHydroNo;

	bool bExist = RHydroMeterCell.IsExist();
	if ( bExist ) return true;	// 이미 존재하면 true로 리턴한다.

	bool bResult = false;
	UnicodeString sSQL;
	UnicodeString sData = "";

	sSQL = " INSERT INTO " + GetTable() +
		   " ( MeasureDate, MeasureTime, CellNo, " +
		   "   Value01, Value02, Value03, Value04, Value05, Value06, Value07, Value08, Value09, Value10,  " +
		   "   Value11, Value12, Value13, Value14, Value15, Value16, Value17, Value18, Value19, Value20,  " +
		   "   Value21, Value22, Value23, Value24, Value25, Value26, Value27, Value28, Value29, Value30,  " +
		   "   Value31, Value32, Value33, Value34, Value35, Value36, Value37, Value38, Value39, Value40 ) ";


	TRHydroMeterCell *pCell = NULL;
	bool bFirst = true;
	for ( int i = 0 ; i < m_pList->Count; i++ ) {
		try {
			pCell = (TRHydroMeterCell *) m_pList->Items[i];
		} catch ( Exception &e ) {
			return false;
		}

		if ( bFirst ) {
			sData = "SELECT   '" + pCell->MeasureDate            + "', " +
							 "'" + pCell->MeasureTime            + "', " +
							 " " + IntToStr(pCell->CellNo)       + " , " +
							 " " + FloatToStr((Extended)pCell->Value01) + " , " +
							 " " + FloatToStr((Extended)pCell->Value02) + " , " +
							 " " + FloatToStr((Extended)pCell->Value03) + " , " +
							 " " + FloatToStr((Extended)pCell->Value04) + " , " +
							 " " + FloatToStr((Extended)pCell->Value05) + " , " +
							 " " + FloatToStr((Extended)pCell->Value06) + " , " +
							 " " + FloatToStr((Extended)pCell->Value07) + " , " +
							 " " + FloatToStr((Extended)pCell->Value08) + " , " +
							 " " + FloatToStr((Extended)pCell->Value09) + " , " +
							 " " + FloatToStr((Extended)pCell->Value10) + " , " +
					 		 " " + FloatToStr((Extended)pCell->Value11) + " , " +
							 " " + FloatToStr((Extended)pCell->Value12) + " , " +
							 " " + FloatToStr((Extended)pCell->Value13) + " , " +
							 " " + FloatToStr((Extended)pCell->Value14) + " , " +
							 " " + FloatToStr((Extended)pCell->Value15) + " , " +
							 " " + FloatToStr((Extended)pCell->Value16) + " , " +
							 " " + FloatToStr((Extended)pCell->Value17) + " , " +
							 " " + FloatToStr((Extended)pCell->Value18) + " , " +
							 " " + FloatToStr((Extended)pCell->Value19) + " , " +
							 " " + FloatToStr((Extended)pCell->Value20) + " , " +
							 " " + FloatToStr((Extended)pCell->Value21) + " , " +
							 " " + FloatToStr((Extended)pCell->Value22) + " , " +
							 " " + FloatToStr((Extended)pCell->Value23) + " , " +
							 " " + FloatToStr((Extended)pCell->Value24) + " , " +
							 " " + FloatToStr((Extended)pCell->Value25) + " , " +
							 " " + FloatToStr((Extended)pCell->Value26) + " , " +
							 " " + FloatToStr((Extended)pCell->Value27) + " , " +
							 " " + FloatToStr((Extended)pCell->Value28) + " , " +
							 " " + FloatToStr((Extended)pCell->Value29) + " , " +
							 " " + FloatToStr((Extended)pCell->Value30) + " , " +
							 " " + FloatToStr((Extended)pCell->Value31) + " , " +
							 " " + FloatToStr((Extended)pCell->Value32) + " , " +
							 " " + FloatToStr((Extended)pCell->Value33) + " , " +
							 " " + FloatToStr((Extended)pCell->Value34) + " , " +
							 " " + FloatToStr((Extended)pCell->Value35) + " , " +
							 " " + FloatToStr((Extended)pCell->Value36) + " , " +
							 " " + FloatToStr((Extended)pCell->Value37) + " , " +
							 " " + FloatToStr((Extended)pCell->Value38) + " , " +
							 " " + FloatToStr((Extended)pCell->Value39) + " , " +
							 " " + FloatToStr((Extended)pCell->Value40) + "   " +
							 " FROM RDB$DATABASE ";
			bFirst = false;
		} else {
			sData += " UNION   "
					 " SELECT '" + pCell->MeasureDate            + "', " +
							 "'" + pCell->MeasureTime            + "', " +
							 " " + IntToStr(pCell->CellNo)       + " , " +
							 " " + FloatToStr((Extended)pCell->Value01) + " , " +
					 		 " " + FloatToStr((Extended)pCell->Value02) + " , " +
					 		 " " + FloatToStr((Extended)pCell->Value03) + " , " +
							 " " + FloatToStr((Extended)pCell->Value04) + " , " +
							 " " + FloatToStr((Extended)pCell->Value05) + " , " +
							 " " + FloatToStr((Extended)pCell->Value06) + " , " +
							 " " + FloatToStr((Extended)pCell->Value07) + " , " +
							 " " + FloatToStr((Extended)pCell->Value08) + " , " +
							 " " + FloatToStr((Extended)pCell->Value09) + " , " +
							 " " + FloatToStr((Extended)pCell->Value10) + " , " +
					 		 " " + FloatToStr((Extended)pCell->Value11) + " , " +
							 " " + FloatToStr((Extended)pCell->Value12) + " , " +
							 " " + FloatToStr((Extended)pCell->Value13) + " , " +
							 " " + FloatToStr((Extended)pCell->Value14) + " , " +
							 " " + FloatToStr((Extended)pCell->Value15) + " , " +
							 " " + FloatToStr((Extended)pCell->Value16) + " , " +
							 " " + FloatToStr((Extended)pCell->Value17) + " , " +
							 " " + FloatToStr((Extended)pCell->Value18) + " , " +
							 " " + FloatToStr((Extended)pCell->Value19) + " , " +
							 " " + FloatToStr((Extended)pCell->Value20) + " , " +
							 " " + FloatToStr((Extended)pCell->Value21) + " , " +
							 " " + FloatToStr((Extended)pCell->Value22) + " , " +
							 " " + FloatToStr((Extended)pCell->Value23) + " , " +
							 " " + FloatToStr((Extended)pCell->Value24) + " , " +
							 " " + FloatToStr((Extended)pCell->Value25) + " , " +
							 " " + FloatToStr((Extended)pCell->Value26) + " , " +
					 		 " " + FloatToStr((Extended)pCell->Value27) + " , " +
							 " " + FloatToStr((Extended)pCell->Value28) + " , " +
							 " " + FloatToStr((Extended)pCell->Value29) + " , " +
							 " " + FloatToStr((Extended)pCell->Value30) + " , " +
							 " " + FloatToStr((Extended)pCell->Value31) + " , " +
							 " " + FloatToStr((Extended)pCell->Value32) + " , " +
							 " " + FloatToStr((Extended)pCell->Value33) + " , " +
							 " " + FloatToStr((Extended)pCell->Value34) + " , " +
							 " " + FloatToStr((Extended)pCell->Value35) + " , " +
							 " " + FloatToStr((Extended)pCell->Value36) + " , " +
							 " " + FloatToStr((Extended)pCell->Value37) + " , " +
							 " " + FloatToStr((Extended)pCell->Value38) + " , " +
							 " " + FloatToStr((Extended)pCell->Value39) + " , " +
							 " " + FloatToStr((Extended)pCell->Value40) + "   " +
							 " FROM RDB$DATABASE ";
		}
		//bResult = pCell->Insert();
		//if ( !bResult ) break;
	}

	sSQL = sSQL + sData;

	int iRow = DM->ExecSQL(sSQL);

	return (iRow >= 0 ) ? true : false;
}

//---------------------------------------------------------------------------
UnicodeString TRHydroMeterCellContainer::GetTable(EN_HYDRO_NO enHydroNo)
{
	return ((enHydroNo == enHydroFirst)  ? "RHydroMeter1Cell" :
		   ((enHydroNo == enHydroSecond) ? "RHydroMeter2Cell" : "RHydroMeter3Cell"));
}

//---------------------------------------------------------------------------
UnicodeString TRHydroMeterCellContainer::GetTable()
{
	return ((m_enHydroNo == enHydroFirst)  ? "RHydroMeter1Cell" :
		   ((m_enHydroNo == enHydroSecond) ? "RHydroMeter2Cell" : "RHydroMeter3Cell"));
}



#pragma package(smart_init)
