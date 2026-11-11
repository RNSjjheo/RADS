//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitSearchData.h"
#include "UnitMain.h"
#include "UnitHydroMeterCell.h"
#include "UnitMain.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"

#include <ustring.h>

TFormSearchData *FormSearchData = NULL;

//---------------------------------------------------------------------------
__fastcall TFormSearchData::TFormSearchData(TComponent* Owner)
    : TForm(Owner)
{
DWORD Tick = GetTickCount();
WCHAR message[100];
wsprintf(message,L"Form Constructor 1 =%d",Tick);
OutputDebugString(message);

	queryMaster->Connection = FormMain->SQLConnection;
	query->Connection = FormMain->SQLConnection;

	m_enQueryType = enQueryDay;

	m_ToTime   = Now();
	m_FromTime = Now();

	DateHome   = Now();
	DateDay    = Now();
	DateWeek   = Now();
	DateMonth  = Now();

	m_bRQ30Flag = false;	// true: RHydroMeter1에서 수위 참조,  false: RWaterLevel에서 참조

	// 2025.03.11, 2026.02.12
	//if ( FormMain->DBEnv.sWaterLevelDeviceKind == "NONE" ||  FormMain->DBEnv.sWaterLevelDeviceKind == "" ) {
	if ( FormMain->DBEnv.sHD1Kind == CsRQ30 || FormMain->DBEnv.sHD1Kind == CsRQ30D ||
		 FormMain->DBEnv.sHD1Kind == CsEWSVConverter ) m_bRQ30Flag = true;	// 수위계: RHyroMeter1에서 참조
	else m_bRQ30Flag = false;	// 수위계 => RWaterLevel에서 참조
	//} else {
	//	m_bRQ30Flag = false;	// 수위 데이터 => RWaterLevel에서 참조
	//}

DWORD Tick1 = GetTickCount();
WCHAR message1[100];
wsprintf(message1,L"Form Constructor 2=%d",Tick1);
OutputDebugString(message1);
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caFree;
    FormSearchData = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::FormCreate(TObject *Sender)
{
//DWORD Tick = GetTickCount();
//WCHAR message[100];
//wsprintf(message,L"Form Create 1=%d",Tick);
//OutputDebugString(message);

	unsigned short iH, iM, iS, iZ;

    DecodeTime(Now(), iH, iM, iS, iZ);
    iM = (iM / 10) * 10; //분은 n0 분만

    DateTimePicker1->Date = Now() - 1;
    se1Hour->Value = iH;
    se1Minute->Value = iM;

    DateTimePicker2->Date = Now();
    se2Hour->Value = iH;
    se2Minute->Value = iM;

    unsigned short iY4, iM2, iD2;
    DecodeDate(Now(), iY4, iM2, iD2);
    TDateTime dtYmd = EncodeDate(iY4, iM2, iD2);
	TDateTime dtHm = EncodeTime(iH, iM, 0, 0);
	DateHome  = dtYmd + dtHm;
	DateDay   = dtYmd + dtHm; //현재일
	DateWeek  = dtYmd + dtHm; //현재일
	DateMonth = dtYmd + dtHm; //현재일

//DWORD Tick2 = GetTickCount();

	PageControl1->ActivePageIndex = 0;	// 유속계를 1번으로 한다.
	PageControl1Change(NULL);

//DWORD Tick3 = GetTickCount();

	ShowPeriod();

//DWORD Tick4 = GetTickCount();

//WCHAR message2[100];
//wsprintf(message2,L"Form Create 2=%d, %d, %d, ... %d, %d",Tick2, Tick3, Tick4, Tick3-Tick2, Tick4-Tick3);
//OutputDebugString(message2);
}

//---------------------------------------------------------------------------
void TFormSearchData::ShowPeriod()
{
	switch(m_enQueryType)
	{
		case enQueryDay: //일
			lblDisplay->Caption = m_FromTime.FormatString("yyyy-mm-dd hh:nn") + " ~ " +
								  m_ToTime.FormatString("yyyy-mm-dd hh:nn");
			break;
		case enQueryWeek: //주
			lblDisplay->Caption = m_FromTime.FormatString("yyyy-mm-dd hh:nn") + " ~ " +
								  m_ToTime.FormatString("yyyy-mm-dd hh:nn");
			break;
		case enQueryMonth: //월
			lblDisplay->Caption = m_FromTime.FormatString("yyyy-mm-dd hh:nn") + " ~ " +
								  m_ToTime.FormatString("yyyy-mm-dd hh:nn");
            break;
    } //switch
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::PageControl1Change(TObject *Sender)
{
	if ((double)DateTimePicker2->Date - DateTimePicker1->Date > 31.0f)
	{
		ShowMessage("기간은 최고 1개월까지만 가능합니다.");
		return;
	} // if
	switch(PageControl1->ActivePageIndex)
	{
		case 0:	//1번 유속계
			rbH1Velocity->Visible = true; rbH1WaterLevel->Visible = true;
			DoQueryHydro1Main();
			break;
		case 1:	//2번 유속계
			rbH1Velocity->Visible = false; rbH1WaterLevel->Visible = false;
			DoQueryHydro2Main();
			break;
		case 2:	//3번 유속계
			rbH1Velocity->Visible = false; rbH1WaterLevel->Visible = false;
			DoQueryHydro3Main();
			break;
		case 3:	// 수위계
			rbH1Velocity->Visible = false; rbH1WaterLevel->Visible = false;
			DoQueryWaterMain();
			break;
		case 4:	// VTH
			rbH1Velocity->Visible = false; rbH1WaterLevel->Visible = false;
			DoQueryVTHMain();
			break;
		case 5:	// 유량
			rbH1Velocity->Visible = false; rbH1WaterLevel->Visible = false;
			DoQueryFlowMain();
			break;
	} // switch

	PageControl2->ActivePageIndex = PageControl1->ActivePageIndex;
    PageControl2Change(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::btnPriorClick(TObject *Sender)
{
	switch(m_enQueryType)
	{
		case enQueryDay:
			DateDay = DateDay - enQueryDay;
			break;
		case enQueryWeek:
			DateWeek = DateWeek - enQueryWeek;
			break;
		case enQueryMonth:
			DateMonth = DateMonth - enQueryMonth;
			break;
	} // switch

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::sbtnDayClick(TObject *Sender)
//일
{
	m_enQueryType = enQueryDay;
	DateDay = DateHome;

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::sbtnWeekClick(TObject *Sender)
//주
{
	m_enQueryType = enQueryWeek;
	DateWeek = DateHome;

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::sbtnMonthClick(TObject *Sender)
//월
{
	m_enQueryType = enQueryMonth;
	DateMonth = DateHome;

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::btnNextClick(TObject *Sender)
//다음
{
	switch(m_enQueryType)
	{
		case enQueryDay:
			DateDay = DateDay + enQueryDay;
			break;
		case enQueryWeek:
			DateWeek = DateWeek + enQueryWeek;
			break;
		case enQueryMonth:
			DateMonth = DateMonth + enQueryMonth;
			break;
	} // switch

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void TFormSearchData::DoQuery()
{
	PageControl2Change(NULL);
//    PageControl1Change(NULL);
}

//////////////////////////////////////////////////////////////////////////////
//
//
// Master ( Data Grid )
//
//
//////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro1Main()
//하층유속
{
    queryMaster->Close();
	queryMaster->DataSet->CommandText = "";

	if ( m_bRQ30Flag ) { // RQ30 Sensor
		queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
			" SELECT MeasureDate, MeasureTime, AvgVelocity AS Val, MINVELOCITY AS VALMIN, MAXVELOCITY AS VALMAX, HYDROKIND,  " +
			"        Value01 AS WaterLevel,      " +
			"        Value03 AS SNR,             " +
			"        Value04 AS Discharge,       " +
			"        Value05 AS " + ((FormMain->DBEnv.sHD1Kind == CsRQ30) ? "Area" : "TotalQ") + ", " + // RQ30=> "Area", RQ30D => TotalQ
			"        Value19 AS TotalLQ,         " +                                                    // 2023.07.20 Master LQ + Slaves LQ
			"        Value06 AS LearnVelocity,   " +
			"        Value07 AS LearnDischarge,  " +
			"        Value08 AS OppositeDir,     " +
			"        Value09 AS Volt,            " +
			"        Value10 AS PeakWidth,       " +
			"        Value11 AS CSR,             " +
			"        Value12 AS AreaPeak,        " +
			"        Value13 AS RMS,             " +
			"        Value14 AS Amp,             " +
			"        Value15 AS AmpRelation,     " +
			"        Value16 AS Signal,          " +
			"        Value17 AS ErrorCode,       " +
			"        Value26 AS SelfCheck,       " +
			"        Value27 AS Angle,           " +
			"        Value21 AS Wind,            " +
			"        Value22 AS WindGust,        " +
			"        Value20 AS WindAngle,       " +
			"        Value24 AS Temperature,     " +
			"        Value25 AS Humidity         " +
			" FROM   RHYDROMETER1                " +
			" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
			"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
			"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
			"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
			" ORDER BY 1 DESC, 2 DESC";

		queryMaster->Open();
		queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
		queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
		queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
		queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
		queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
		queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
		queryMaster->FieldByName("VAL")->DisplayLabel = "평균유속";
		((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("VALMIN")->DisplayLabel = "최소유속";
		((TNumericField*)queryMaster->FieldByName("VALMIN"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("VALMAX")->DisplayLabel = "최대유속";
		((TNumericField*)queryMaster->FieldByName("VALMAX"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("HYDROKIND")->DisplayLabel = "장비종류";
		queryMaster->FieldByName("HYDROKIND")->DisplayWidth = 15;
		((TNumericField*)queryMaster->FieldByName("WaterLevel"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("WaterLevel")->DisplayLabel = "수위";
		queryMaster->FieldByName("WaterLevel")->DisplayWidth = 15;
		queryMaster->FieldByName("Discharge")->DisplayLabel = "Q";
		((TNumericField*)queryMaster->FieldByName("Discharge"))->DisplayFormat = "#0.00";
		if ( FormMain->DBEnv.sHD1Kind == CsRQ30 ) {  	// RQ-30	( Area => Area )
			queryMaster->FieldByName("Area")->DisplayLabel = "Area";
			((TNumericField*)queryMaster->FieldByName("Area"))->DisplayFormat = "#0.00";
		} else {										// RQ-30d ( Area => TotalQ
			queryMaster->FieldByName("TotalQ")->DisplayLabel = "TotalQ";
			((TNumericField*)queryMaster->FieldByName("TotalQ"))->DisplayFormat = "#0.00";
		}
		queryMaster->FieldByName("TotalLQ")->DisplayLabel = "TotalLQ";
		((TNumericField*)queryMaster->FieldByName("TotalLQ"))->DisplayFormat = "#0.00";

		((TNumericField*)queryMaster->FieldByName("Wind"))->DisplayFormat        = "#0.00";
		((TNumericField*)queryMaster->FieldByName("WindGust"))->DisplayFormat    = "#0.00";
		((TNumericField*)queryMaster->FieldByName("WindAngle"))->DisplayFormat   = "#0.00";
		((TNumericField*)queryMaster->FieldByName("Temperature"))->DisplayFormat = "#0.00";
		((TNumericField*)queryMaster->FieldByName("Humidity"))->DisplayFormat    = "#0.00";
		((TNumericField*)queryMaster->FieldByName("TotalLQ"))->DisplayFormat     = "#0.00";
		((TNumericField*)queryMaster->FieldByName("SNR"))->DisplayFormat         = "#0.00";
		((TNumericField*)queryMaster->FieldByName("Volt"))->DisplayFormat        = "#0.00";
		((TNumericField*)queryMaster->FieldByName("Angle"))->DisplayFormat       = "#0.00";

		SetDBGridColumnWidth(DBGrid, 7, 10); //인덱스7번부터 끝까지 7 * 10으로 width 세팅

	} else { // other Sensor
		queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
			" SELECT MeasureDate, MeasureTime, AvgVelocity AS Val, MINVELOCITY AS VALMIN, MAXVELOCITY AS VALMAX, HYDROKIND, " +
			"        0 AS WaterLevel   " +
			  GetDeviceFields(1)    + " " +
			" FROM   RHydroMeter1 " +
			" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
			"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
			"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
			"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
			" ORDER BY 1 DESC, 2 DESC";

		queryMaster->Open();
		queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
		queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
		queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
		queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
		queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
		queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
		queryMaster->FieldByName("VAL")->DisplayLabel = "평균유속(";
		((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("VALMIN")->DisplayLabel = "최소유속";
		((TNumericField*)queryMaster->FieldByName("VALMIN"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("VALMAX")->DisplayLabel = "최대유속";
		((TNumericField*)queryMaster->FieldByName("VALMAX"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("HYDROKIND")->DisplayLabel = "장비종류";
		queryMaster->FieldByName("HYDROKIND")->DisplayWidth = 15;
		((TNumericField*)queryMaster->FieldByName("WaterLevel"))->DisplayFormat = "#0.000";
		queryMaster->FieldByName("WaterLevel")->DisplayLabel = "수위";
		queryMaster->FieldByName("WaterLevel")->DisplayWidth = 15;

		SetDBGridColumnWidth(DBGrid, 7, 10); //인덱스 6번부터 끝까지 7 * 10으로 width 세팅
	}

//	queryMaster->Open();
//	queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
//	queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
//	queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
//	queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
//	queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
//	queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
//	queryMaster->FieldByName("VAL")->DisplayLabel = "평균유속(";
//	((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.00";
//	queryMaster->FieldByName("VALMIN")->DisplayLabel = "최소유속";
//	((TNumericField*)queryMaster->FieldByName("VALMIN"))->DisplayFormat = "#0.00";
//	queryMaster->FieldByName("VALMAX")->DisplayLabel = "최대유속";
//	((TNumericField*)queryMaster->FieldByName("VALMAX"))->DisplayFormat = "#0.00";
//	queryMaster->FieldByName("HYDROKIND")->DisplayLabel = "장비종류";
//	queryMaster->FieldByName("HYDROKIND")->DisplayWidth = 15;
//	SetDBGridColumnWidth(DBGrid, 6, 10); //인덱스 5번부터 끝까지 7 * 10으로 width 세팅
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro2Main()
//상층유속
{
    queryMaster->Close();
    queryMaster->DataSet->CommandText = "";
    queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
		" SELECT MEASUREDATE, MEASURETIME, AVGVELOCITY AS VAL, MINVELOCITY AS VALMIN, MAXVELOCITY AS VALMAX, HYDROKIND \n" +
		  GetDeviceFields(2) + "\n" +
		" FROM   RHYDROMETER2 \n" +
		" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
		"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
		"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
		"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
		" ORDER BY 1 DESC, 2 DESC";
    queryMaster->Open();
    queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
    queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
    queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
    queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
    queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
    queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
    queryMaster->FieldByName("VAL")->DisplayLabel = "평균유속";
	((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("VALMIN")->DisplayLabel = "최소유속";
	((TNumericField*)queryMaster->FieldByName("VALMIN"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("VALMAX")->DisplayLabel = "최대유속";
	((TNumericField*)queryMaster->FieldByName("VALMAX"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("HYDROKIND")->DisplayLabel = "장비종류";
	queryMaster->FieldByName("HYDROKIND")->DisplayWidth = 15;
	SetDBGridColumnWidth(DBGrid, 6, 10); //인덱스 5번부터 끝까지 7 * 10으로 width 세팅
}

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro3Main()
//상층유속
{
    queryMaster->Close();
    queryMaster->DataSet->CommandText = "";
    queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
		" SELECT MEASUREDATE, MEASURETIME, AVGVELOCITY AS VAL, MINVELOCITY AS VALMIN, MAXVELOCITY AS VALMAX, HYDROKIND \n" +
		  GetDeviceFields(2) + "\n" +
		" FROM   RHYDROMETER3 \n" +
		" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
		"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
		"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
		"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
		" ORDER BY 1 DESC, 2 DESC";
	queryMaster->Open();
	queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
	queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
	queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
	queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
	queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
	queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
	queryMaster->FieldByName("VAL")->DisplayLabel = "평균유속";
	((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("VALMIN")->DisplayLabel = "최소유속";
	((TNumericField*)queryMaster->FieldByName("VALMIN"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("VALMAX")->DisplayLabel = "최대유속";
	((TNumericField*)queryMaster->FieldByName("VALMAX"))->DisplayFormat = "#0.000";
	queryMaster->FieldByName("HYDROKIND")->DisplayLabel = "장비종류";
	queryMaster->FieldByName("HYDROKIND")->DisplayWidth = 15;
    SetDBGridColumnWidth(DBGrid, 6, 10); //인덱스 5번부터 끝까지 7 * 10으로 width 세팅
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryWaterMain()
//수위
{
	UnicodeString sOffset = UnicodeString::FloatToStrF(FormMain->DBEnv.fWaterLevelOffset, ffFixed, 10, 3); // 2.098239883 => 2.098, 2.0985992 => 2.099

// 조회의 수위 데이터: 무조건 RWaterLevel에서 표시한다.
//	if ( m_bRQ30Flag ) {  	// 수위계가 없고, RQ-30에서 수위 데이터를 가져올 때
//		queryMaster->Close();
//		queryMaster->DataSet->CommandText = "";
//		queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
//			" SELECT MeasureDate, MeasureTime, (Value01) AS Val, " +
//			"   '" + ((FormMain->DBEnv.sHD1Kind == CsRQ30) ? "RQ30" : "RQ30D") + "' AS WATERKIND, " +
//			"        '' AS DeviceType,           " +
//			"		 0  AS TEMPERATURE,          " +
//			"		 0  AS PRESSURE,             " +
//			"		 0  AS DEPTH,                " +
//			"		 0  AS Salinity,             " +  // 염도(AT200)/(RnsWater)대기압(mbar)
//			"		 Value18 AS OFFSET,               " +
//			"        Value20 AS WindDegree,           " +
//			"        Value21 AS WindSpeed,            " +
//			"        Value22 AS WindGust,             " +
//			"        Value23 AS AirPress,             " +
//			"        Value24 AS AirTemp,              " +
//			"        Value25 AS AirHumidity           " +
//			" FROM   RHydroMeter1                     " +
//			" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
//			"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
//			"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
//			"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
//			"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
//			"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
//			" ORDER BY 1 DESC, 2 DESC";
//
//	} else {	// other waterlevel sensor : 수위계가 설치된 경우
		queryMaster->Close();
		queryMaster->DataSet->CommandText = "";
		queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
			" SELECT MEASUREDATE, MEASURETIME, Value03 + Value10 AS VAL,   "
			"		 WATERKIND,                       "
			"		 IIF(WATERKIND = 'RNSWATER',      "
			"		(DECODE ( Value12,                "
			"			0, 'NONE',                    "
			"			1, 'VegaPuls',                "
			"			2, 'VegaPuls',                "
			"			3, 'VegaPuls',                "
			"			4, 'AT200',                   "
			"			5, 'LT400',                   "
			"			6, 'LT500',                   "
			"			7, 'OTTCBS',                  "
			"			8, 'MK21',                    "
			"			'-' )), WATERKIND) AS DeviceType,   "
			"		 VALUE01 AS TEMPERATURE,          "
			"		 VALUE02 AS PRESSURE,             "
			"		 VALUE03 AS DEPTH,                "
			"		 IIF(WATERKIND = 'AT200', VALUE07, VALUE07) AS Salinity,  "  // 염도(AT200)/(RnsWater)대기압(mbar)
			"		 VALUE10 AS OFFSET,               "
			"        Value13 AS WindDegree,           "
			"        Value14 AS WindSpeed,            "
			"        Value15 AS WindGust,             "
			"        Value16 AS AirPress,             "
			"        Value17 AS AirTemp,              "
			"        Value18 AS AirHumidity           "
			" FROM   RWATERLEVEL                      "
			" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
			"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
			"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
			"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
			"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
			" ORDER BY 1 DESC, 2 DESC";
//	}
	queryMaster->Open();
	queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
	queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
	queryMaster->FieldByName("MEASUREDATE")->OnGetText    = queryMasterFieldGetText;

	queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
	queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
	queryMaster->FieldByName("MEASURETIME")->OnGetText    = queryMasterFieldGetText;

	queryMaster->FieldByName("VAL")->DisplayLabel         = "수위";
	queryMaster->FieldByName("VAL")->DisplayWidth         = 10;

	queryMaster->FieldByName("WATERKIND")->DisplayLabel   = "장비모델";
	queryMaster->FieldByName("WATERKIND")->DisplayWidth   = 15;

	queryMaster->FieldByName("DeviceType")->DisplayLabel  = "장비종류";
	queryMaster->FieldByName("DeviceType")->DisplayWidth  = 15;

	queryMaster->FieldByName("Salinity")->DisplayLabel    = "염도/대기압(mbar)";
	queryMaster->FieldByName("DeviceType")->DisplayWidth  = 15;

//	if ( m_bRQ30Flag ) { 	// RQ-30d => RHydroMeter1
//		queryMaster->FieldByName("WindDegree")->DisplayLabel   = "풍향(도)";
//		queryMaster->FieldByName("WindSpeed")->DisplayLabel    = "풍속(m)";
//		queryMaster->FieldByName("WindGust")->DisplayLabel     = "최대풍속(m)";
//		queryMaster->FieldByName("AirPress")->DisplayLabel     = "대기압(Bar)";
//		queryMaster->FieldByName("AirTemp")->DisplayLabel      = "대기온도(C)";
//		queryMaster->FieldByName("AirHumidity")->DisplayLabel  = "대기습도(%)";
//
//		//소수점 세째자리까지 표시
//		((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat         = "0.00";
//		((TNumericField*)queryMaster->FieldByName("Temperature"))->DisplayFormat = "0.00";
//		((TNumericField*)queryMaster->FieldByName("Pressure"))->DisplayFormat    = "0.00";
//		((TNumericField*)queryMaster->FieldByName("Depth"))->DisplayFormat       = "0.00";
//		((TNumericField*)queryMaster->FieldByName("Salinity"))->DisplayFormat    = "0.00";
//		((TNumericField*)queryMaster->FieldByName("Offset"))->DisplayFormat      = "0.00";
//		((TNumericField*)queryMaster->FieldByName("WindDegree"))->DisplayFormat  = "0.00";
//		((TNumericField*)queryMaster->FieldByName("WindSpeed"))->DisplayFormat   = "0.00";
//		((TNumericField*)queryMaster->FieldByName("WindGust"))->DisplayFormat    = "0.00";
//		((TNumericField*)queryMaster->FieldByName("AirPress"))->DisplayFormat    = "0.00";
//		((TNumericField*)queryMaster->FieldByName("AirTemp"))->DisplayFormat     = "0.00";
//		((TNumericField*)queryMaster->FieldByName("AirHumidity"))->DisplayFormat = "0.00";
//
//		SetDBGridColumnWidth(DBGrid, 5, 12); //인덱스 5번부터 끝까지 7 * 10으로 width 세팅
//
//	} else {	// WaterLevel => RWaterLevel Table
		queryMaster->FieldByName("WindDegree")->DisplayLabel   = "풍향(도)";
		queryMaster->FieldByName("WindSpeed")->DisplayLabel    = "풍속(m)";
		queryMaster->FieldByName("WindGust")->DisplayLabel     = "최대풍속(m)";
		queryMaster->FieldByName("AirPress")->DisplayLabel     = "대기압(Bar)";
		queryMaster->FieldByName("AirTemp")->DisplayLabel      = "대기온도(C)";
		queryMaster->FieldByName("AirHumidity")->DisplayLabel  = "대기습도(%)";

		//소수점 세째자리까지 표시
		((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat         = "0.00";
		((TNumericField*)queryMaster->FieldByName("Temperature"))->DisplayFormat = "0.00";
		((TNumericField*)queryMaster->FieldByName("Pressure"))->DisplayFormat    = "0.00";
		((TNumericField*)queryMaster->FieldByName("Depth"))->DisplayFormat       = "0.00";
		((TNumericField*)queryMaster->FieldByName("Salinity"))->DisplayFormat    = "0.00";
		((TNumericField*)queryMaster->FieldByName("Offset"))->DisplayFormat      = "0.00";
		((TNumericField*)queryMaster->FieldByName("WindDegree"))->DisplayFormat  = "0.00";
		((TNumericField*)queryMaster->FieldByName("WindSpeed"))->DisplayFormat   = "0.00";
		((TNumericField*)queryMaster->FieldByName("WindGust"))->DisplayFormat    = "0.00";
		((TNumericField*)queryMaster->FieldByName("AirPress"))->DisplayFormat    = "0.00";
		((TNumericField*)queryMaster->FieldByName("AirTemp"))->DisplayFormat     = "0.00";
		((TNumericField*)queryMaster->FieldByName("AirHumidity"))->DisplayFormat = "0.00";

		SetDBGridColumnWidth(DBGrid, 5, 10); //인덱스 5번부터 끝까지 7 * 10으로 width 세팅
//	}
}

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryVTHMain()
//vth
{
	queryMaster->Close();
	queryMaster->DataSet->CommandText = "";
	queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
		" SELECT MEASUREDATE, MEASURETIME, VTHKIND, VOLT, TEMPERATURE, HUMIDITY, DCCHARGE, DCBATTERY \n" +
		" FROM   RVTHLOGGER \n" +
		" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
		"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
		"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
		"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
		" ORDER BY 1 DESC, 2 DESC";
	queryMaster->Open();
    queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
    queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
    queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
    queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
	queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
    queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
    queryMaster->FieldByName("VTHKIND")->DisplayLabel = "장비종류";
    queryMaster->FieldByName("VTHKIND")->DisplayWidth = 15;
    queryMaster->FieldByName("VOLT")->DisplayLabel = "전압";
    ((TFloatField*)queryMaster->FieldByName("VOLT"))->DisplayFormat = "##0.0";
    queryMaster->FieldByName("TEMPERATURE")->DisplayLabel = "온도";
    ((TFloatField*)queryMaster->FieldByName("TEMPERATURE"))->DisplayFormat = "#0.0";
    queryMaster->FieldByName("HUMIDITY")->DisplayLabel = "습도";
    ((TFloatField*)queryMaster->FieldByName("HUMIDITY"))->DisplayFormat = "##0.0";
    queryMaster->FieldByName("DCCHARGE")->DisplayLabel = "DC CHARGE";
    ((TFloatField*)queryMaster->FieldByName("DCCHARGE"))->DisplayFormat = "#0.00";
    queryMaster->FieldByName("DCBATTERY")->DisplayLabel = "DC BATTERY";
    ((TFloatField*)queryMaster->FieldByName("DCBATTERY"))->DisplayFormat = "#0.00";
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryFlowMain()
//유량
{
	queryMaster->Close();
	queryMaster->DataSet->CommandText = "";
	queryMaster->DataSet->CommandText = queryMaster->DataSet->CommandText +
		" SELECT MEASUREDATE, MEASURETIME, AVGSTREAM AS VAL" +
		" FROM   RSTREAM \n" +
		" WHERE (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureTime  > '" + FormatFloat("00", se1Hour->Value) + FormatFloat("00", se1Minute->Value) + "00" + "') OR " +
		"       (MeasureDate  > '" + FormatDateTime("yyyymmdd", DateTimePicker1->Date) + "' AND " +
		"        MeasureDate  < '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "') OR " +
		"       (MeasureDate  = '" + FormatDateTime("yyyymmdd", DateTimePicker2->Date) + "' AND " +
		"        MeasureTime <= '" + FormatFloat("00", se2Hour->Value) + FormatFloat("00", se2Minute->Value) + "00" + "')    " +
		" ORDER BY 1 DESC, 2 DESC";
	queryMaster->Open();
	queryMaster->FieldByName("MEASUREDATE")->DisplayLabel = "일자";
	queryMaster->FieldByName("MEASUREDATE")->DisplayWidth = 10;
	queryMaster->FieldByName("MEASUREDATE")->OnGetText = queryMasterFieldGetText;
	queryMaster->FieldByName("MEASURETIME")->DisplayLabel = "시간";
	queryMaster->FieldByName("MEASURETIME")->DisplayWidth = 8;
	queryMaster->FieldByName("MEASURETIME")->OnGetText = queryMasterFieldGetText;
	queryMaster->FieldByName("VAL")->DisplayLabel = "평균유량";
	((TNumericField*)queryMaster->FieldByName("VAL"))->DisplayFormat = "#0.00";
	//queryMaster->FieldByName("MINVAL")->DisplayLabel = "최소유량";
	//((TNumericField*)queryMaster->FieldByName("MINVAL"))->DisplayFormat = "#0.00";
	//queryMaster->FieldByName("MAXVAL")->DisplayLabel = "최대유량";
	//((TNumericField*)queryMaster->FieldByName("MAXVAL"))->DisplayFormat = "#0.00";
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::ChartWaterLevelGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
          int ValueIndex, UnicodeString &LabelText)
{
	if (Sender == ChartFlowStream->BottomAxis  ||
		Sender == ChartHydroMeter1->BottomAxis ||
		Sender == ChartHydroMeter2->BottomAxis ||
		Sender == ChartHydroMeter3->BottomAxis ||
		Sender == ChartWaterLevel->BottomAxis  ||
		Sender == ChartVTHLogger->BottomAxis)
	{
		TDateTime Time;
		try
		{
			switch(m_enQueryType)
			{
				case enQueryDay: //일
					Time = m_ToTime - 1 + (LabelText.ToIntDef(0) + 1) / 144.0f;
					LabelText = FormatDateTime("hh:nn", Time);
					break;
				case enQueryWeek: //주
					Time = m_ToTime - 7 + (LabelText.ToIntDef(0) + 1) / 24.0f;
					LabelText = FormatDateTime("dd_hh:nn", Time);
					break;
				case enQueryMonth: //월
					Time = m_ToTime - 30 + LabelText.ToIntDef(0);
					LabelText = FormatDateTime("mm-dd", Time);
					break;
			} //switch
		} catch(...)
		{
		} // catch
    } // if
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::FormResize(TObject *Sender)
{
	PageControl2->Height = (ClientHeight - pnl2->Height - pnl1->Height) / 2;
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::cbnQueryClick(TObject *Sender)
//메인조회
{
	PageControl1Change(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::PageControl2Change(TObject *Sender)
{
    switch(PageControl2->ActivePageIndex)
    {
		case 0:		// Hydrometer1
			{
			if ( rbH1Velocity->Checked )
				DoQueryHydro1Graph();
			else if ( rbH1WaterLevel->Checked && m_bRQ30Flag )
				DoQueryHydro1WaterGraph(); 	// 수위 Chart, 유속계가 RQ-30d이면 ok
			else
				DoQueryHydro1Graph();	//  default
			}
			break;
		case 1:		// Hydrometer2
			DoQueryHydro2Graph();
			break;
		case 2:		// Hydrometer3
			DoQueryHydro3Graph();
			break;
		case 3:		// WaterLevel
			DoQueryWaterGraph();
			break;
		case 4:		// VTHLogger
			DoQueryVTHGraph();
			break;
		case 5: 	// FlowStream
			DoQueryFlowGraph();
			break;
	} // switch
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::DBGridCellClick(TColumn *Column)
//해당 선택된 셀의 measuredate, measuretime을 하단 그래프의 시작키가 되게 전달
{
	UnicodeString sDate = queryMaster->FieldByName("MEASUREDATE")->AsString;
	UnicodeString sTime = queryMaster->FieldByName("MEASURETIME")->AsString;
	TDateTime Time = EncodeDate(sDate.SubString(1, 4).ToIntDef(1901),
							  sDate.SubString(5, 2).ToIntDef(1),
                              sDate.SubString(7, 2).ToIntDef(1)
                             ) +
                   EncodeTime(sTime.SubString(1, 2).ToIntDef(0),
                              sTime.SubString(3, 2).ToIntDef(0),
                              sTime.SubString(5, 2).ToIntDef(0),
                              0
							 );

	DateHome  = Time;	// Home DateTime
	DateDay   = Time;
	DateWeek  = Time;
	DateMonth = Time;

	PageControl2Change(NULL);
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormSearchData::queryMasterFieldGetText(TField *Sender, UnicodeString &Text,
          bool DisplayText)
{
    UnicodeString sFieldValue = Sender->AsString;
    if (Sender->FieldName.UpperCase() == "MEASUREDATE")
    {
        Text = sFieldValue.SubString(1, 4) + "-" +
               sFieldValue.SubString(5, 2) + "-" +
               sFieldValue.SubString(7, 2);
    } // if
    if (Sender->FieldName.UpperCase() == "MEASURETIME")
    {
        Text = sFieldValue.SubString(1, 2) + ":" +
               sFieldValue.SubString(3, 2) + ":" +
               sFieldValue.SubString(5, 2);
    } // if
}

//---------------------------------------------------------------------------
UnicodeString TFormSearchData::GetDeviceFields(int iDeviceKind)
{
	UnicodeString sResult = "";

    switch(iDeviceKind)
	{
		case 1: //하층유속계
//            if (FormMain->DBEnv.sHD1Kind == CsChannelMaster) sResult = ", Value01 AS \"NB : NUMBER OF BEAMS\", Value02 AS \"WN : NUMBER OF CELLS\", Value03 AS \"WP : PINGS PER ENSEMBLE\", Value04 AS \"WS : RANGE CELL LENGTH\", Value05 AS \"WF : BLANK AFTER TRANSMIT\", Value06 AS \"WC : LOW CORR THRESH\", Value07 AS \"WE : ERROR VELOCITY MAXIMUM\", Value08 AS \"TP : TPP HUNDREDTHS\", Value09 AS \"EX : COORDINATE TRANSFORM\", Value10 AS \"EZ : SENSOR SOURCE\", Value11 AS \"Bin1Distance : BIN 1 DISTANCE\", Value12 AS \"WA : FALSE TARGET THRESH\", Value13 AS \"WB : SYSTEM BANDWIDTH\", Value14 AS \"EC : SPEED OF SOUND\", Value15 AS \"ED : DEPTH OF TRANSDUCER\", Value16 AS \"PITCH\", Value17 AS \"ROLL\", Value18 AS \"ES : SALINITY\", Value19	AS \"ET : TEMPERATURE\", Value20 AS \"MPT MINUTES\", Value21 AS \"MPT SECONDS\", Value22 AS \"MPT HUNDREDTHS\", Value23	AS \"PITCH STD\", Value24 AS \"ROLL STD\", Value25 AS \"BATTERY VOLTAGE IN VOLTS\", Value26	AS \"PRESSURE\" \n";
			if (FormMain->DBEnv.sHD1Kind == CsChannelMaster) sResult = ", Value01 AS \"NB\", Value02 AS \"WN\", Value03 AS \"WP\", Value04 AS \"WS\", Value05 AS \"WF\", Value06 AS \"WC\", Value07 AS \"WE\", Value08 AS \"TP\", Value09 AS \"EX\", Value10 AS \"EZ\", Value11 AS \"Bin1Distance\", Value12 AS \"WA\", Value13 AS \"WB\", Value14 AS \"EC\", Value15 AS \"ED\", Value16 AS \"PITCH\", Value17 AS \"ROLL\", Value18 AS \"ES\", Value19 AS \"ET\", Value20 AS \"MPT MINUTES\", Value21 AS \"MPT SECONDS\", Value22 AS \"MPT HUNDREDTHS\", Value23 AS \"PITCH STD\", Value24 AS \"ROLL STD\", Value25 AS \"BATTERY VOLTAGE IN VOLTS\", Value26 AS \"PRESSURE\" \n";
//            if (FormMain->DBEnv.sHD1Kind == CsSLSeries)      sResult = ", Value01 AS \"Number of beam(2,3,4)\", Value02 AS \"Orientation(0:down,1:up,2:side)\", Value03 AS \"Temperature mode(0:user,1:measured)\", Value04 AS \"Coordinate system(0:beam, 1:xyz, 2:enu)\", Value05 AS \"Number of cells\", Value06 AS \"Cell size\", Value07 AS \"Blanking distance(cm)\", Value08 AS \"Averaging interval\", Value09 AS \"Number of pings in this profile\", Value10 AS \"Mean heading\", Value11 AS \"Mean pitch\", Value12 AS \"Mean roll\", Value13 AS \"Mean temperature\", Value14 AS \"Mean pressure\", Value15 AS \"Deviation of heading\", Value16 AS \"Deviation of pitch\", Value17 AS \"Deviation of roll\", Value18 AS \"Std temperature\", Value19 AS \"Std pressure\", Value20 AS \"Sound Speed\" \n";
			if (FormMain->DBEnv.sHD1Kind == CsSLSeries)      sResult = ", Value01 AS \"NumberOfBeam(2/3/4)\", Value02 AS \"Orient(0:down/1:up/2:side)\", Value03 AS \"TempMode(0:user/1:measured)\", Value04 AS \"Coordinate(0:beam/1:xyz/2:enu)\", Value05 AS \"NumberOfCells\", Value06 AS \"CellSize\", Value07 AS \"BlankingDist(cm)\", Value08 AS \"AverageInterval\", Value09 AS \"NumberOfPing\", Value10 AS \"MeanHeading\", Value11 AS \"MeanPitch\", Value12 AS \"MeanRoll\", Value13 AS \"MeanTemperature\", Value14 AS \"MeanPressure\", Value15 AS \"DeviationHeading\", Value16 AS \"DeviationPitch\", Value17 AS \"DeviationRoll\", Value18 AS \"StdTemperature\", Value19 AS \"StdPressure\", Value20 AS \"SoundSpeed\" \n";
//            if (FormMain->DBEnv.sHD1Kind == CsAGSeries)      sResult = ", Value01 AS \"Velocity component 1(Beam 1/X/East)\", Value02 AS \"Velocity component 2(Beam 2/Y/North)\", Value03 AS \"Velocity component 3(Beam 3/Z/Up)\", Value04 AS \"Standard error of velocity 1(Beam 1/X/East)\", Value05 AS \"Standard error of velocity 2(Beam 2/Y/North)\", Value06 AS \"Standard error of velocity 3(Beam 3/Z/Up)\", Value07 AS \"Signal strength(Beam 1)\", Value08 AS \"Signal strength(Beam 2)\", Value09 AS \"Signal strength(Beam 3)\", Value10 AS \"Percent good pings\", Value11 AS \"Heading\", Value12 AS \"Pitch(rotation about the Y-axis)\", Value13 AS \"Roll(rotation about the X-axis)\", Value14 AS \"Standard deviation heading\", Value15 AS \"Standard deviation pitch\", Value16 AS \"Standard deviation roll\", Value17 AS \"Mean temperature\", Value18 AS \"Mean pressure\", Value19 AS \"Standard deviation of pressure\", Value20 AS \"Input power level\", Value21 AS \"Starting location of sampling volume\", Value22 AS \"Ending location of sampling volume\", Value23 AS \"Noise Level(Beam 1)\", Value24 AS \"Noise Level(Beam 2)\", Value25 AS \"Noise Level(Beam 3)\" \n";
			if (FormMain->DBEnv.sHD1Kind == CsAGSeries)      sResult = ", Value01 AS \"Velocity1(Beam1/X/East)\", Value02 AS \"Velocity2(Beam2/Y/North)\", Value03 AS \"Velocity3(Beam3/Z/Up)\", Value04 AS \"StdErrorOfVel1(Beam1/X/East)\", Value05 AS \"StdErrorOfVel2(Beam2/Y/North)\", Value06 AS \"StdErrorOfVel3(Beam3/Z/Up)\", Value07 AS \"SignalStrength(Beam1)\", Value08 AS \"SignalStrength(Beam2)\", Value09 AS \"SignalStrength(Beam3)\", Value10 AS \"PercentGoodPings\", Value11 AS \"Heading\", Value12 AS \"Pitch(Y-axis)\", Value13 AS \"Roll(X-axis)\", Value14 AS \"StdDevHeading\", Value15 AS \"StdDevPitch\", Value16 AS \"StdDevRoll\", Value17 AS \"MeanTemperature\", Value18 AS \"MeanPressure\", Value19 AS \"StdDevOfPressure\", Value20 AS \"InputPowerLevel\", Value21 AS \"StartSampleingVolume\", Value22 AS \"EndSamplingVolume\", Value23 AS \"NoiseLevel(Beam1)\", Value24 AS \"NoiseLevel(Beam2)\", Value25 AS \"NoiseLevel(Beam3)\" \n";
			break;
		case 2: //상층유속계
//            if (FormMain->DBEnv.sHD2Kind == CsChannelMaster) sResult = ", Value01 AS \"NB : NUMBER OF BEAMS\", Value02 AS \"WN : NUMBER OF CELLS\", Value03 AS \"WP : PINGS PER ENSEMBLE\", Value04 AS \"WS : RANGE CELL LENGTH\", Value05 AS \"WF : BLANK AFTER TRANSMIT\", Value06 AS \"WC : LOW CORR THRESH\", Value07 AS \"WE : ERROR VELOCITY MAXIMUM\", Value08 AS \"TP : TPP HUNDREDTHS\", Value09 AS \"EX : COORDINATE TRANSFORM\", Value10 AS \"EZ : SENSOR SOURCE\", Value11 AS \"Bin1Distance : BIN 1 DISTANCE\", Value12 AS \"WA : FALSE TARGET THRESH\", Value13 AS \"WB : SYSTEM BANDWIDTH\", Value14 AS \"EC : SPEED OF SOUND\", Value15 AS \"ED : DEPTH OF TRANSDUCER\", Value16 AS \"PITCH\", Value17 AS \"ROLL\", Value18 AS \"ES : SALINITY\", Value19	AS \"ET : TEMPERATURE\", Value20 AS \"MPT MINUTES\", Value21 AS \"MPT SECONDS\", Value22 AS \"MPT HUNDREDTHS\", Value23	AS \"PITCH STD\", Value24 AS \"ROLL STD\", Value25 AS \"BATTERY VOLTAGE IN VOLTS\", Value26	AS \"PRESSURE\" \n";
			if (FormMain->DBEnv.sHD2Kind == CsChannelMaster) sResult = ", Value01 AS \"NB\", Value02 AS \"WN\", Value03 AS \"WP\", Value04 AS \"WS\", Value05 AS \"WF\", Value06 AS \"WC\", Value07 AS \"WE\", Value08 AS \"TP\", Value09 AS \"EX\", Value10 AS \"EZ\", Value11 AS \"Bin1Distance\", Value12 AS \"WA\", Value13 AS \"WB\", Value14 AS \"EC\", Value15 AS \"ED\", Value16 AS \"PITCH\", Value17 AS \"ROLL\", Value18 AS \"ES\", Value19 AS \"ET\", Value20 AS \"MPT MINUTES\", Value21 AS \"MPT SECONDS\", Value22 AS \"MPT HUNDREDTHS\", Value23 AS \"PITCH STD\", Value24 AS \"ROLL STD\", Value25 AS \"BATTERY VOLTAGE IN VOLTS\", Value26 AS \"PRESSURE\" \n";
//            if (FormMain->DBEnv.sHD2Kind == CsSLSeries)      sResult = ", Value01 AS \"Number of beam(2,3,4)\", Value02 AS \"Orientation(0:down,1:up,2:side)\", Value03 AS \"Temperature mode(0:user,1:measured)\", Value04 AS \"Coordinate system(0:beam, 1:xyz, 2:enu)\", Value05 AS \"Number of cells\", Value06 AS \"Cell size\", Value07 AS \"Blanking distance(cm)\", Value08 AS \"Averaging interval\", Value09 AS \"Number of pings in this profile\", Value10 AS \"Mean heading\", Value11 AS \"Mean pitch\", Value12 AS \"Mean roll\", Value13 AS \"Mean temperature\", Value14 AS \"Mean pressure\", Value15 AS \"Deviation of heading\", Value16 AS \"Deviation of pitch\", Value17 AS \"Deviation of roll\", Value18 AS \"Std temperature\", Value19 AS \"Std pressure\", Value20 AS \"Sound Speed\" \n";
			if (FormMain->DBEnv.sHD2Kind == CsSLSeries)      sResult = ", Value01 AS \"NumberOfBeam(2/3/4)\", Value02 AS \"Orient(0:down/1:up/2:side)\", Value03 AS \"TempMode(0:user/1:measured)\", Value04 AS \"Coordinate(0:beam/1:xyz/2:enu)\", Value05 AS \"NumberOfCells\", Value06 AS \"CellSize\", Value07 AS \"BlankingDist(cm)\", Value08 AS \"AverageInterval\", Value09 AS \"NumberOfPing\", Value10 AS \"MeanHeading\", Value11 AS \"MeanPitch\", Value12 AS \"MeanRoll\", Value13 AS \"MeanTemperature\", Value14 AS \"MeanPressure\", Value15 AS \"DeviationHeading\", Value16 AS \"DeviationPitch\", Value17 AS \"DeviationRoll\", Value18 AS \"StdTemperature\", Value19 AS \"StdPressure\", Value20 AS \"SoundSpeed\" \n";
//            if (FormMain->DBEnv.sHD2Kind == CsAGSeries)      sResult = ", Value01 AS \"Velocity component 1(Beam 1/X/East)\", Value02 AS \"Velocity component 2(Beam 2/Y/North)\", Value03 AS \"Velocity component 3(Beam 3/Z/Up)\", Value04 AS \"Standard error of velocity 1(Beam 1/X/East)\", Value05 AS \"Standard error of velocity 2(Beam 2/Y/North)\", Value06 AS \"Standard error of velocity 3(Beam 3/Z/Up)\", Value07 AS \"Signal strength(Beam 1)\", Value08 AS \"Signal strength(Beam 2)\", Value09 AS \"Signal strength(Beam 3)\", Value10 AS \"Percent good pings\", Value11 AS \"Heading\", Value12 AS \"Pitch(rotation about the Y-axis)\", Value13 AS \"Roll(rotation about the X-axis)\", Value14 AS \"Standard deviation heading\", Value15 AS \"Standard deviation pitch\", Value16 AS \"Standard deviation roll\", Value17 AS \"Mean temperature\", Value18 AS \"Mean pressure\", Value19 AS \"Standard deviation of pressure\", Value20 AS \"Input power level\", Value21 AS \"Starting location of sampling volume\", Value22 AS \"Ending location of sampling volume\", Value23 AS \"Noise Level(Beam 1)\", Value24 AS \"Noise Level(Beam 2)\", Value25 AS \"Noise Level(Beam 3)\" \n";
			if (FormMain->DBEnv.sHD2Kind == CsAGSeries)      sResult = ", Value01 AS \"Velocity1(Beam1/X/East)\", Value02 AS \"Velocity2(Beam2/Y/North)\", Value03 AS \"Velocity3(Beam3/Z/Up)\", Value04 AS \"StdErrorOfVel1(Beam1/X/East)\", Value05 AS \"StdErrorOfVel2(Beam2/Y/North)\", Value06 AS \"StdErrorOfVel3(Beam3/Z/Up)\", Value07 AS \"SignalStrength(Beam1)\", Value08 AS \"SignalStrength(Beam2)\", Value09 AS \"SignalStrength(Beam3)\", Value10 AS \"PercentGoodPings\", Value11 AS \"Heading\", Value12 AS \"Pitch(Y-axis)\", Value13 AS \"Roll(X-axis)\", Value14 AS \"StdDevHeading\", Value15 AS \"StdDevPitch\", Value16 AS \"StdDevRoll\", Value17 AS \"MeanTemperature\", Value18 AS \"MeanPressure\", Value19 AS \"StdDevOfPressure\", Value20 AS \"InputPowerLevel\", Value21 AS \"StartSampleingVolume\", Value22 AS \"EndSamplingVolume\", Value23 AS \"NoiseLevel(Beam1)\", Value24 AS \"NoiseLevel(Beam2)\", Value25 AS \"NoiseLevel(Beam3)\" \n";
			break;
    } // switch

    return sResult;
}


//---------------------------------------------------------------------------
void __fastcall TFormSearchData::ChartHydroMeter1ClickSeries(TCustomChart *Sender,
          TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	//셀별 유속 및 신호강도 호출
	if (FormHydroMeterCell == NULL) FormHydroMeterCell = new TFormHydroMeterCell(this);

	UnicodeString sMeasureDate, sMeasureTime;

	switch(m_enQueryType)
	{
		case enQueryDay:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f);
			sMeasureTime = FormatDateTime("hhnndd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f).SubString(1, 3) + "000";
			break;
		case enQueryWeek:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 7 + (ValueIndex + 1) / 24.0f);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 7 + (ValueIndex + 1) / 24.0f).SubString(1, 3) + "000";
			break;
		case enQueryMonth:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 30 + ValueIndex);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 30 + ValueIndex).SubString(1, 3) + "000";
			break;
	} // switch

	EN_HYDRO_NO enHydroNo = enHydroFirst;
	UnicodeString TableName = "RHydroMeter1";
	enHydroNo = enHydroFirst;
	TableName = "RHydroMeter1";

	UnicodeString sSQL = " SELECT HydroKind FROM " + TableName    +
						 " WHERE  MeasureDate = '" + sMeasureDate + "' AND " +
						 "        MeasureTime = '" + sMeasureTime + "'     ";

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	UnicodeString sHydroKind;

	while ( !query->Eof ) {
		sHydroKind = query->FieldByName("HydroKind")->AsString;
		query->Next();
	}

	if ( sHydroKind == CsRQ30 || sHydroKind == CsRQ30D ) {
		ShowMessage("RQ-30/RQ-30d 장비는 셀별 유속과 빔 강도가 없습니다.");
		delete FormHydroMeterCell;
		FormHydroMeterCell = NULL;
		return;
	}

	FormHydroMeterCell->sMeasureDate = sMeasureDate;
	FormHydroMeterCell->sMeasureTime = sMeasureTime;
	FormHydroMeterCell->sHydroKind   = sHydroKind;

	//if (Series == Series1) FormHydroMeterCell->sHydroKind = sHydroKind;
	//else                   FormHydroMeterCell->sHydroKind = sHydroKind2;

	FormHydroMeterCell->enHydroNo = enHydroNo;	// 1,2,3번 유속계
    FormHydroMeterCell->Show();
    FormHydroMeterCell->ShowPeriod();
	FormHydroMeterCell->DoQuery();
}
//---------------------------------------------------------------------------

void __fastcall TFormSearchData::ChartHydroMeter2ClickSeries(TCustomChart *Sender,
          TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	//셀별 유속 및 신호강도 호출
	if (FormHydroMeterCell == NULL) FormHydroMeterCell = new TFormHydroMeterCell(this);

	UnicodeString sMeasureDate, sMeasureTime;

	switch(m_enQueryType)
	{
		case enQueryDay:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f);
			sMeasureTime = FormatDateTime("hhnndd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f).SubString(1, 3) + "000";
			break;
		case enQueryWeek:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 7 + (ValueIndex + 1) / 24.0f);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 7 + (ValueIndex + 1) / 24.0f).SubString(1, 3) + "000";
			break;
		case enQueryMonth:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 30 + ValueIndex);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 30 + ValueIndex).SubString(1, 3) + "000";
			break;
	} // switch

	EN_HYDRO_NO enHydroNo = enHydroSecond;
	UnicodeString TableName = "RHydroMeter2";

	enHydroNo = enHydroSecond;
	TableName = "RHydroMeter2";

	UnicodeString sSQL = " SELECT HydroKind FROM  RHydroMeter2"
						 " WHERE  MeasureDate = '" + sMeasureDate + "' AND " +
						 "        MeasureTime = '" + sMeasureTime + "'     ";

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	UnicodeString sHydroKind;

	while ( !query->Eof ) {
		sHydroKind = query->FieldByName("HydroKind")->AsString;
		query->Next();
	}

	if ( sHydroKind == CsRQ30 || sHydroKind == CsRQ30D ) {
		ShowMessage("RQ-30/RQ-30d 장비는 셀별 유속과 빔 강도가 없습니다.");
		delete FormHydroMeterCell;
		FormHydroMeterCell = NULL;
		return;
	}

	FormHydroMeterCell->sMeasureDate = sMeasureDate;
	FormHydroMeterCell->sMeasureTime = sMeasureTime;
	FormHydroMeterCell->sHydroKind   = sHydroKind;

	//if (Series == Series1) FormHydroMeterCell->sHydroKind = sHydroKind;
	//else                   FormHydroMeterCell->sHydroKind = sHydroKind2;

	FormHydroMeterCell->enHydroNo = enHydroNo;	// 1,2,3 유속계
    FormHydroMeterCell->Show();
    FormHydroMeterCell->ShowPeriod();
	FormHydroMeterCell->DoQuery();
}

//////////////////////////////////////////////////////////////////////////////
//
//
// Detail Graph
//
//
//////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro1Graph()
//하층유속
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	UnicodeString sSQL;

	if ( m_bRQ30Flag ) {  	// RQ-30 HydroMeter => unit : meter
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1  "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
				"          AvgVelocity <> 0 AND " +   // 단위의 치환 =>  mm => meter ( why => RQ30 use meter )
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          AvgVelocity <> 0 AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
		} else {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1 "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
				"         AvgVelocity <> 0 AND " +
				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
		}
	} else {  	// CM, SL, AG, ... => Unit: mm
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1  "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
				"          AvgVelocity <> 0 AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          AvgVelocity <> 0 AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
		} else {
			sSQL =
				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
				" FROM   RHydroMeter1 "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
				"         AvgVelocity <> 0 AND " +
				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
		}
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin, fMax;
	fMin = fMax = 0.0f;
	while ( !query->Eof ) {
		fMin = query->FieldByName("MinV")->AsFloat;
		fMax = query->FieldByName("MaxV")->AsFloat;
		query->Next();
	}

	//LeftAxis 그래프값 설정
	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	ChartHydroMeter1->LeftAxis->Minimum   = -50000.0f;
	ChartHydroMeter1->LeftAxis->Maximum   =  50000.0f;
	ChartHydroMeter1->LeftAxis->Minimum   = fMin;
	ChartHydroMeter1->LeftAxis->Maximum   = fMax;
	ChartHydroMeter1->LeftAxis->Automatic = false;
	ChartHydroMeter1->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartHydroMeter1->BottomAxis->Automatic = false;
	ChartHydroMeter1->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	AreaSeriesHydroMeter1->Clear(); //하층

	if ( m_bRQ30Flag ) {     	// rq30 device : unit:mm

		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
				" ORDER BY 1, 2 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
				" ORDER BY 1, 2 ";
		} else {
			sSQL =
				" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) AND " +
				"          AvgVelocity <> 0 AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
				" GROUP BY MeasureDate ";
		}
		query->Close();
		query->DataSet->CommandText = sSQL;
		query->Open();

	} else {    	// oter hydrometer => Unit: mm

		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
				" ORDER BY 1, 2 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
				" ORDER BY 1, 2 ";
		} else {
			sSQL =
				" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
				" FROM   RHydroMeter1 "
				" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) AND " +
				"          AvgVelocity <> 0 AND " +
				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
				" GROUP BY MeasureDate ";
		}
		query->Close();
		query->DataSet->CommandText = sSQL;
		query->Open();

	}

	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			AreaSeriesHydroMeter1->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while


		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( m_bRQ30Flag) {
			if ( fYVal <= -99.0f )	AreaSeriesHydroMeter1->AddXY(iXVal++,     0, "", clBlack);					// Add Series Point
			else                    AreaSeriesHydroMeter1->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		} else {
			AreaSeriesHydroMeter1->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		AreaSeriesHydroMeter1->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}

	// heaven2 2023.06.21
	if ( m_bRQ30Flag ) stHydro1Y->Caption = "유속(m/sec)";
	else               stHydro1Y->Caption = "유속(mm/sec)";
}

//---------------------------------------------------------------------------
// 유속계1의 수위 Chart View ( RQ-30d)에 한정해서 유효하다.
void TFormSearchData::DoQueryHydro1WaterGraph()
//하층유속
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	UnicodeString sSQL;

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT '' AS MeasureDate, "
			"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
			"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater, "
			"        Min(Value21) AS MinWind,  Max(Value22) AS MaxWind "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >=  '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"          Value01 <> 0 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT '' AS MeasureDate, "
			"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
			"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater, "
			"        Min(Value21) AS MinWind,  Max(Value22) AS MaxWind "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"          Value01 <> 0 ";
	} else {
		sSQL =
			" SELECT MeasureDate, "
			"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
			"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater, "
			"        Min(Value21) AS MinWind,  Max(Value22) AS MaxWind "
			" FROM   RHydroMeter1 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) AND " +
			"         Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"         Value01 <> 0 " +
			" GROUP BY MeasureDate   " +
			" ORDER BY AvgWaterLevel ";
	}

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin, fMax;
	fMin = fMax = 0.0f;
	while ( !query->Eof ) {
		fMin = query->FieldByName("MinWater")->AsFloat;
		fMax = query->FieldByName("MaxWater")->AsFloat;
		query->Next();
	}

	//LeftAxis 그래프값 설정
	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	ChartHydroMeter1->LeftAxis->Minimum   = -50000.0f;
	ChartHydroMeter1->LeftAxis->Maximum   =  50000.0f;
	ChartHydroMeter1->LeftAxis->Minimum   = fMin;
	ChartHydroMeter1->LeftAxis->Maximum   = fMax;
	ChartHydroMeter1->LeftAxis->Automatic = false;
	ChartHydroMeter1->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartHydroMeter1->BottomAxis->Minimum = 0;
		ChartHydroMeter1->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartHydroMeter1->BottomAxis->Automatic = false;
	ChartHydroMeter1->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	AreaSeriesHydroMeter1->Clear(); //하층

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, 'RQ30' AS WaterKind, Value01 AS WaterLevel, Value21 AS WindSpeed, Value22 AS WindGust "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, 'RQ30' AS WaterKind, Value01 AS WaterLevel, Value21 AS WindSpeed, Value22 AS WindGust "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, 'RQ30' AS WaterKind, AVG(Value01) AS WaterLevel, AVG(Value21) AS WindSpeed, AVG(Value22) AS WindGust "
			" FROM   RHydroMeter1 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) AND " +
			"         Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"         Value01 <> 0 " +
			" GROUP BY MeasureDate ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();


	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			AreaSeriesHydroMeter1->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while


		fYVal = query->FieldByName("WaterLevel")->AsFloat;
		AreaSeriesHydroMeter1->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		AreaSeriesHydroMeter1->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}

	// heaven2 2023.06.21
	if ( m_bRQ30Flag ) stHydro1Y->Caption = "수위(m)";
	else               stHydro1Y->Caption = "수위(m)";
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro2Graph()
//상층유속
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	// HydroMeter2 => RQ-30없음..
	UnicodeString sSQL;

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"         AvgVelocity <> 0 AND " +
			"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin, fMax;
	fMin = fMax = 0.0f;
	while ( !query->Eof ) {
		fMin = query->FieldByName("MinV")->AsFloat;
		fMax = query->FieldByName("MaxV")->AsFloat;
		query->Next();
	}

	//LeftAxis 그래프값 설정
	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	ChartHydroMeter2->LeftAxis->Minimum   = -50000.0f;
	ChartHydroMeter2->LeftAxis->Maximum   =  50000.0f;
	ChartHydroMeter2->LeftAxis->Minimum   = fMin;
	ChartHydroMeter2->LeftAxis->Maximum   = fMax;
	ChartHydroMeter2->LeftAxis->Automatic = false;
	ChartHydroMeter2->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartHydroMeter2->BottomAxis->Minimum = 0;
		ChartHydroMeter2->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartHydroMeter2->BottomAxis->Minimum = 0;
		ChartHydroMeter2->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartHydroMeter2->BottomAxis->Minimum = 0;
		ChartHydroMeter2->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartHydroMeter2->BottomAxis->Automatic = false;
	ChartHydroMeter2->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	AreaSeriesHydroMeter2->Clear(); //하층

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter2 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter2 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
			" FROM   RHydroMeter2 "
			" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) AND " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" GROUP BY MeasureDate ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			AreaSeriesHydroMeter2->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while


		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( m_bRQ30Flag ) {
			if ( fYVal <= -99.0f ) {
				AreaSeriesHydroMeter2->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
			} else {
				AreaSeriesHydroMeter2->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
			}
		} else {
			AreaSeriesHydroMeter2->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		AreaSeriesHydroMeter2->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}

	stHydro2Y->Caption = "유속(mm/sec)";
}

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryHydro3Graph()
//상층유속
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	// HydroMeter2 => RQ-30없음..
	UnicodeString sSQL;

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"         AvgVelocity <> 0 AND " +
			"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin, fMax;
	fMin = fMax = 0.0f;
	while ( !query->Eof ) {
		fMin = query->FieldByName("MinV")->AsFloat;
		fMax = query->FieldByName("MaxV")->AsFloat;
		query->Next();
	}

	//LeftAxis 그래프값 설정
	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	ChartHydroMeter3->LeftAxis->Minimum   = -50000.0f;
	ChartHydroMeter3->LeftAxis->Maximum   =  50000.0f;
	ChartHydroMeter3->LeftAxis->Minimum   = fMin;
	ChartHydroMeter3->LeftAxis->Maximum   = fMax;
	ChartHydroMeter3->LeftAxis->Automatic = false;
	ChartHydroMeter3->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartHydroMeter3->BottomAxis->Minimum = 0;
		ChartHydroMeter3->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartHydroMeter3->BottomAxis->Minimum = 0;
		ChartHydroMeter3->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartHydroMeter3->BottomAxis->Minimum = 0;
		ChartHydroMeter3->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartHydroMeter3->BottomAxis->Automatic = false;
	ChartHydroMeter3->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	AreaSeriesHydroMeter3->Clear(); //하층

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter3 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter3 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
			" FROM   RHydroMeter3 "
			" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) AND " +
			"          AvgVelocity <> 0 AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
			" GROUP BY MeasureDate ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			AreaSeriesHydroMeter2->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while


		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( m_bRQ30Flag ) {
			if ( fYVal <= -99.0f ) {
				AreaSeriesHydroMeter3->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
			} else {
				AreaSeriesHydroMeter3->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
			}
		} else {
			AreaSeriesHydroMeter3->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		AreaSeriesHydroMeter3->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}

    stHydro3Y->Caption = "유속(mm/sec)";
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryWaterGraph()
//수위
{
	//min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	UnicodeString sSQL;


	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT '' AS MeasureDate, "
			"        Min(Value03+Value10) AS MinWaterLevel, Max(Value03+Value10) AS MaxWaterLevel, Avg(Value03+Value10) AS AvgWaterLevel, "
			"        Min(Value03+Value10) AS MinWater, Avg(Value03+Value10) AS AvgWater, Max(Value03+Value10) AS MaxWater,                "
			"        Min(Value14) AS MinWind, Max(Value15) AS MaxWind "
			" FROM   RWaterLevel "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2);
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT '' AS MeasureDate, "
			"        Min(Value03+Value10) AS MinWaterLevel, Max(Value03+Value10) AS MaxWaterLevel, Avg(Value03+Value10) AS AvgWaterLevel, "
			"        Min(Value03+Value10) AS MinWater, Avg(Value03+Value10) AS AvgWater, Max(Value03+Value10) AS MaxWater,                "
			"        Min(Value14) AS MinWind, Max(Value15) AS MaxWind "
			" FROM   RWaterLevel "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"          Value03+Value10 <> 0 ";
	} else {
		sSQL =
			" SELECT MeasureDate, "
			"        Min(Value03+Value10) AS MinWaterLevel, Max(Value03+Value10) AS MaxWaterLevel, Avg(Value03+Value10) AS AvgWaterLevel, "
			"        Min(Value03+Value10) AS MinWater, Avg(Value03+Value10) AS AvgWater, Max(Value03+Value10) AS MaxWater,                "
			"        Min(Value14) AS MinWind, Max(Value15) AS MaxWind "
			" FROM   RWaterLevel "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) AND " +
			"         Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"         Value03+Value10 <> 0 " +
			" GROUP BY MeasureDate   " +
			" ORDER BY AvgWaterLevel " ;
	}

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin,  fMax,  fAvg;
	float fMin2, fMax2, fAvg2;
	float MinWind, MaxWind;
	int   iCount = 0;
	fMin  =  99999.9f;	// Min Default
	fMax  = -99999.9f;	// Max Default
	fAvg  =      0.0f;	// Avg Default
	fMin2 = fMax2 = fAvg2 = 0.0f;
	while ( !query->Eof ) {
		if ( m_enQueryType == enQueryDay || m_enQueryType == enQueryWeek ) {
			fMin2   = query->FieldByName("MinWaterLevel")->AsFloat;
			fMax2   = query->FieldByName("MaxWaterLevel")->AsFloat;
			fAvg2   = query->FieldByName("AvgWaterLevel")->AsFloat;
			MinWind = query->FieldByName("MinWind")->AsFloat;
			MaxWind = query->FieldByName("MaxWind")->AsFloat;
		} else { // By Month
			fMin2 = query->FieldByName("AvgWaterLevel")->AsFloat;
			fMax2 = query->FieldByName("AvgWaterLevel")->AsFloat;
			fAvg2 = query->FieldByName("AvgWaterLevel")->AsFloat;
			MinWind = query->FieldByName("MinWind")->AsFloat;
			MaxWind = query->FieldByName("MaxWind")->AsFloat;
		}
		if ( fMin2 < fMin ) fMin = fMin2;
		if ( fMax2 > fMax ) fMax = fMax2;
		fAvg += fAvg2;
		iCount++;
		query->Next();
	}
	if ( m_enQueryType == enQueryMonth ) {
		if ( iCount > 0 ) fAvg = fAvg / (float)iCount;
		else              fAvg = fAvg;
	}

	float DefaultMinWater = -1.0f;
	float DefaultMaxWater = 10.0f;
	float DefaultMinWind  =  0.0f;
	float DefaultMaxWind  = 15.0f;

	if ( fMin < DefaultMinWater ) DefaultMinWater = fMin;	// 수위 최저 갱신
	if ( fMax > DefaultMaxWater ) DefaultMaxWater = fMax;	// 수위 최대 갱신
	DefaultMinWind = 0.0f;	// 수위 최저 갱신
	if ( MaxWind > DefaultMaxWind ) DefaultMaxWater = MaxWind;	// 수위 최대 갱신

	float fOffset = FormMain->DBEnv.fWaterLevelOffset;	// WaterLevel Offset

	//LeftAxis 그래프값 설정
	ChartWaterLevel->LeftAxis->Minimum   = DefaultMinWater;
	ChartWaterLevel->LeftAxis->Maximum   = DefaultMaxWater;
	ChartWaterLevel->RightAxis->Minimum  = DefaultMinWind;
	ChartWaterLevel->RightAxis->Maximum  = DefaultMaxWind;
	ChartWaterLevel->LeftAxis->Automatic = false;
	ChartWaterLevel->LeftAxis->Increment = 1.0f;
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartWaterLevel->BottomAxis->Minimum = 0;
		ChartWaterLevel->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartWaterLevel->BottomAxis->Minimum = 0;
		ChartWaterLevel->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartWaterLevel->BottomAxis->Minimum = 0;
		ChartWaterLevel->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartWaterLevel->BottomAxis->Automatic = false;
	ChartWaterLevel->BottomAxis->Increment = 1;

double leftRange = 0.0f;
double rightRange = 0.0f;
double leftIncrement = 0.0f;
double rightIncrement = 0.0f;

leftRange      = ChartWaterLevel->Axes->Left->Maximum - ChartWaterLevel->Axes->Left->Minimum;
rightRange     = ChartWaterLevel->Axes->Right->Maximum - ChartWaterLevel->Axes->Right->Minimum;
leftIncrement  = ChartWaterLevel->Axes->Left->Increment;
rightIncrement = (leftIncrement / leftRange) * rightRange;
ChartWaterLevel->Axes->Right->Increment = rightIncrement;

	// Series Clear ---------------------------------------------------------
	AreaSeriesWaterLevel->Clear();	// WaterLevel
	AreaSeriesWindSpeed->Clear();	// WindSpeed
	AreaSeriesWindGust->Clear();	// WindGust

	UnicodeString sOffset =  UnicodeString::FloatToStrF(fOffset, ffFixed, 10, 3); // -2.1935 => -2.194

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, WaterKind, Value03 + Value10 AS WaterLevel, Value14 AS WindSpeed, Value15 AS WindGust  "
			" FROM   RWaterLevel "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, WaterKind, Value03 + Value10 AS WaterLevel, Value14 AS WindSpeed, Value15 AS WindGust  "
			" FROM   RWaterLevel "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, '' AS WaterKind, AVG(Value03 + Value10) AS WaterLevel, AVG(Value14) AS WindSpeed, AVG(Value15) AS WindGust "
			" FROM   RWaterLevel "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) AND " +
			"         Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
			"         Value03+Value10 <> 0 " +
			" GROUP BY MeasureDate ";
	}

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	//시리즈 값 대입
	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			AreaSeriesWaterLevel->AddXY(iXVal, 0, "", clGray);
			AreaSeriesWindSpeed->AddXY(iXVal, 0, "", clGray);
			AreaSeriesWindGust->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fYVal = query->FieldByName("WaterLevel")->AsFloat;
		AreaSeriesWaterLevel->AddXY(iXVal, fYVal, "", clBlue);  // Blue

		fYVal = query->FieldByName("WindSpeed")->AsFloat;
		AreaSeriesWindSpeed->AddXY(iXVal, fYVal, "", clRed);	  // Red

		fYVal = query->FieldByName("WindGust")->AsFloat;
		AreaSeriesWindGust->AddXY(iXVal, fYVal, "", clBlack);	  // Black

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		iXVal++;
		query->Next();
	}

	while ( iXVal < iLimitCount ) {
		AreaSeriesWaterLevel->AddXY(iXVal, 0, "",clGray);
		AreaSeriesWindSpeed->AddXY(iXVal, 0, "", clGray);
		AreaSeriesWindGust->AddXY(iXVal, 0, "", clGray);
		iXVal++;
	}
}

//---------------------------------------------------------------------------
void TFormSearchData::DoQueryVTHGraph()
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	UnicodeString sSQL;

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT Min(DCCharge) AS MinDCCharge,   Max(DCCharge) AS MaxDCCharge,   "
			"        Min(DCBattery) AS MinDCBattery, Max(DCBattery) AS MaxDCBattery, "
			"        Min(Temperature) AS MinTemp,    Max(Temperature) AS MaxTemp     "
			" FROM   RVTHLogger "
			" WHERE  ( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )    ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(DCCharge) AS MinDCCharge,   Max(DCCharge) AS MaxDCCharge,   "
			"        Min(DCBattery) AS MinDCBattery, Max(DCBattery) AS MaxDCBattery, "
			"        Min(Temperature) AS MinTemp,    Max(Temperature) AS MaxTemp     "
			" FROM   RVTHLogger "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " )))   ";
	} else {
		sSQL =
			" SELECT Min(DCCharge) AS MinDCCharge,   Max(DCCharge) AS MaxDCCharge,   "
			"        Min(DCBattery) AS MinDCBattery, Max(DCBattery) AS MaxDCBattery, "
			"        Min(Temperature) AS MinTemp,    Max(Temperature) AS MaxTemp     "
			" FROM   RVTHLogger "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMaxDCCharge  = query->FieldByName("MaxDCCharge")->AsFloat;
	float fMinDCCharge  = query->FieldByName("MinDCCharge")->AsFloat;

	float fMaxDCBattery = query->FieldByName("MaxDCBattery")->AsFloat;
	float fMinDCBattery = query->FieldByName("MinDCBattery")->AsFloat;

	float fMaxDCTemp    = query->FieldByName("MaxTemp")->AsFloat;
	float fMinDCTemp    = query->FieldByName("MinTemp")->AsFloat;

	//LeftAxis 그래프값 설정(전압)
	//ChartVTHLogger->LeftAxis->Automatic = false;
	ChartVTHLogger->LeftAxis->Minimum =  0.0f;
	ChartVTHLogger->LeftAxis->Maximum = 20.0f;
	//ChartVTHLogger->LeftAxis->Increment = 1;
	//ChartVTHLogger->LeftAxis->Automatic = false;
	//Chart->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//RightAxis 그래프값 설정(온도)
	//ChartVTHLogger->RightAxis->Automatic = false;
	ChartVTHLogger->RightAxis->Minimum = -10.0f;
	ChartVTHLogger->RightAxis->Maximum =  80.0f;
	//ChartVTHLogger->RightAxis->Increment = 1;
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartVTHLogger->BottomAxis->Minimum = 0;
		ChartVTHLogger->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartVTHLogger->BottomAxis->Minimum = 0;
		ChartVTHLogger->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartVTHLogger->BottomAxis->Minimum = 0;
		ChartVTHLogger->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartVTHLogger->BottomAxis->Automatic = false;
	ChartVTHLogger->BottomAxis->Increment = 1;

// same increment
double leftRange = 0.0f;
double rightRange = 0.0f;
double leftIncrement = 0.0f;
double rightIncrement = 0.0f;

leftRange      = ChartVTHLogger->Axes->Left->Maximum - ChartVTHLogger->Axes->Left->Minimum;
rightRange     = ChartVTHLogger->Axes->Right->Maximum - ChartVTHLogger->Axes->Right->Minimum;
leftIncrement  = ChartVTHLogger->Axes->Left->Increment;
rightIncrement = (leftIncrement / leftRange) * rightRange;
ChartVTHLogger->Axes->Right->Increment = rightIncrement;

	// Series Clear ---------------------------------------------------------
	SeriesDCCharge->Clear();	// DCCharge
	SeriesDCBattery->Clear();   // Temperature
	SeriesTemperature->Clear();   // DCBattery

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, VTHKind, Volt, Temperature, Humidity, DCCharge, DCBattery "
			" FROM   RVTHLogger "
			" WHERE  ( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )    " +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, VTHKind, Volt, Temperature, Humidity, DCCharge, DCBattery "
			" FROM   RVTHLogger "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " )))  " +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, '' AS VTHKind, Avg(Volt) AS Volt, Avg(Temperature) AS Temperature, "
			"        Avg(Humidity) AS Humidity, Avg(DCCharge) AS DCCharge, Avg(DCBattery) AS DCBattery "
			" FROM   RVTHLogger "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) " +
			" GROUP BY MeasureDate ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	//시리즈 값 대입
	int   iXVal  = 0;
	float fYVal  = 0.0f;

	float fDCCharge, fTemperature, fDCBattery;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			SeriesDCCharge->AddXY(iXVal, 0, "", clGray);
			SeriesDCBattery->AddXY(iXVal, 0, "", clGray);
			SeriesTemperature->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fDCCharge    = query->FieldByName("DCCharge")->AsFloat;
		fTemperature = query->FieldByName("Temperature")->AsFloat;
		fDCBattery   = query->FieldByName("DCBattery")->AsFloat;
		SeriesDCCharge->AddXY(iXVal, fDCCharge,    "",clBlue);
		SeriesDCBattery->AddXY(iXVal, fDCBattery,   "",clRed);
		SeriesTemperature->AddXY(iXVal, fTemperature, "",clFuchsia);

		iXVal++;
		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	while ( iXVal < iLimitCount ) {
		SeriesDCCharge->AddXY(iXVal, 0, "",clGray);
		SeriesDCBattery->AddXY(iXVal, 0, "",clGray);
		SeriesTemperature->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}


//---------------------------------------------------------------------------
void TFormSearchData::DoQueryFlowGraph()
//유량
{
    //min/max를 구함
	query->Close();
	query->DataSet->CommandText = "";

	UnicodeString sFromDate,    sFromTime;
	UnicodeString sToDate,      sToTime;
	UnicodeString sFromAllDay,  sToAllDay;
	TDateTime     FromAllDay,   ToAllDay;

	if ( m_enQueryType == enQueryDay ) {
		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
	} else if ( m_enQueryType == enQueryWeek ) {
		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
		FromAllDay = m_FromTime + 1;
		ToAllDay   = m_ToTime -1;
	} else {
		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
	}

	sFromDate = m_FromTime.FormatString("yyyymmdd");
	sFromTime = m_FromTime.FormatString("hhnnss");
	sToDate   = m_ToTime.FormatString("yyyymmdd");
	sToTime   = m_ToTime.FormatString("hhnnss");

	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
	sToAllDay   = ToAllDay.FormatString("yyyymmdd");

	UnicodeString sBefore, sAfter, sAllDay;
	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'

	//-----------------------------------------------------------------------
	// Min/Max
	//-----------------------------------------------------------------------
	UnicodeString sSQL;

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT Min(AvgStream) AS MinStream, Max(AvgStream) AS MaxStream, Avg(AvgStream) AS AvgStream "
			" FROM   RStream  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"          AvgStream <> 0 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgStream) AS MinStream, Max(AvgStream) AS MaxStream, Avg(AvgStream) AS AvgStream "
			" FROM   RStream  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND  " +
			"          AvgStream <> 0 ";
	} else {
		sSQL =
			" SELECT Min(AvgStream) AS MinStream, Max(AvgStream) AS MaxStream, Avg(AvgStream) AS AvgStream "
			" FROM   RStream  "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"         AvgStream <> 0 " +
			" GROUP BY MeasureDate   " +
			" ORDER BY AvgStream     ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin,  fMax,  fAvg;
	float fMin2, fMax2, fAvg2;
	int   iCount = 0;
	fMin  =  99999.9f;	// Min Default
	fMax  = -99999.9f;	// Max Default
	fAvg  =      0.0f;	// Avg Default
	fMin2 = fMax2 = fAvg2 = 0.0f;
	while ( !query->Eof ) {
		if ( m_enQueryType == enQueryDay || m_enQueryType == enQueryWeek ) {
			fMin2 = query->FieldByName("MinStream")->AsFloat;
			fMax2 = query->FieldByName("MaxStream")->AsFloat;
			fAvg2 = query->FieldByName("AvgStream")->AsFloat;
		} else { // By Month
			fMin2 = query->FieldByName("AvgStream")->AsFloat;
			fMax2 = query->FieldByName("AvgStream")->AsFloat;
			fAvg2 = query->FieldByName("AvgStream")->AsFloat;
		}
		if ( fMin2 < fMin ) fMin = fMin2;
		if ( fMax2 > fMax ) fMax = fMax2;
		fAvg += fAvg2;
		iCount++;
		query->Next();
	}
	if ( m_enQueryType == enQueryMonth ) {
		if ( iCount > 0 ) fAvg = fAvg / (float)iCount;
		else              fAvg = fAvg;
	}

	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	ChartFlowStream->LeftAxis->Minimum   = -50000.0f;
	ChartFlowStream->LeftAxis->Maximum   =  50000.0f;
	ChartFlowStream->LeftAxis->Minimum   = fMin;
	ChartFlowStream->LeftAxis->Maximum   = fMax;
	ChartFlowStream->LeftAxis->Automatic = false;
	ChartFlowStream->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		ChartFlowStream->BottomAxis->Minimum = 0;
		ChartFlowStream->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		ChartFlowStream->BottomAxis->Minimum = 0;
		ChartFlowStream->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		ChartFlowStream->BottomAxis->Minimum = 0;
		ChartFlowStream->BottomAxis->Maximum = 31-1;		// 31건
	}
	ChartFlowStream->BottomAxis->Automatic = false;
	ChartFlowStream->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	SeriesFlowStream->Clear();

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, AvgStream "
			" FROM   RStream "
			" WHERE  ( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )    " +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, AvgStream "
			" FROM   RStream "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " )))   " +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, Avg(AvgStream) AS AvgStream "
			" FROM   RStream "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + ")) AND " +
			"         AvgStream <> 0 " +
			" GROUP BY MeasureDate ";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	//시리즈 값 대입
	int   iXVal  = 0;
	float fYVal  = 0.0f;

	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
	int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;

	while(!query->Eof)
	{
		UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
		UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
		TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
							  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);

		//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
		while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
			SeriesFlowStream->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fYVal = query->FieldByName("AvgStream")->AsFloat;
		SeriesFlowStream->AddXY(iXVal++, fYVal, "", clBlue);	// Offset 값 보정하여 화면에 표시

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	while ( iXVal < iLimitCount ) {
		SeriesFlowStream->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}

//---------------------------------------------------------------------------
// 조회된 데이터를 Export한다.
#define TYPE_HYDROMETER      0
#define TYPE_WATERLEVEL      1
#define TYPE_VTHLOGGER       2

void __fastcall TFormSearchData::cbnExportClick(TObject *Sender)
{
	// Data Export Function => 2017.12.05
	// 1. 조회된 내역이 없으면 Skip
	// 2. 파일 이름을 물어본다.
	// 3. 파일을 생성한다.
	// 4. while loop를 돌면서 1 row fetch, 1 row write한다
	// 5. file close한다.

	int Type =  TYPE_HYDROMETER;
	if (PageControl1->ActivePageIndex == 0 || PageControl1->ActivePageIndex == 1 ) Type = TYPE_HYDROMETER;
	else if ( PageControl1->ActivePageIndex == 2 ) Type = TYPE_WATERLEVEL;
	else if ( PageControl1->ActivePageIndex == 3 ) Type = TYPE_VTHLOGGER;
	else return;

	UnicodeString sDate = queryMaster->FieldByName("MEASUREDATE")->AsString;
	UnicodeString sTime = queryMaster->FieldByName("MEASURETIME")->AsString;

	if ( sDate.IsEmpty() && sTime.IsEmpty() ) return;	// Query not set

	SaveDialog->DefaultExt = ".csv";
	SaveDialog->Filter = "csv file|*.csv";
	SaveDialog->FilterIndex = 1;//first filter extension
	SaveDialog->InitialDir = GetCurrentDir();

	if ( !SaveDialog->Execute() ) return;	// File select cancel

	FILE *fp = NULL;
	AnsiString Filename = SaveDialog->FileName;

	fp = fopen(Filename.c_str(), "wt");
	if ( fp == NULL ) {
		ShowMessage("파일 생성 에러입니다.");
		return;
	}

	AnsiString ColumnHeader = "";
	int ColumnCount = DBGrid->Columns->Count;
	for ( int i = 0 ; i < ColumnCount ; i++ ) {
		if ( i == 0 ) {
			ColumnHeader += DBGrid->Columns->Items[i]->DisplayName;
		} else {
			ColumnHeader += "," + DBGrid->Columns->Items[i]->DisplayName;
		}
	}
	ColumnHeader += "\n";	// newline

	size_t size = 0;
	size = fwrite(ColumnHeader.c_str(), 1, ColumnHeader.Length(), fp);
	if ( size <= 0 ) {
		ShowMessage("Write error..");
		return;
	}

	bool bError = false;
	UnicodeString ErrorMessage = "";
	UnicodeString Data = "";
	AnsiString    AnsiData = "";
	double        Value = 0;
	UnicodeString DataStr = "";
	queryMaster->First();	// go first record
	TField *pField = NULL;
	while ( !queryMaster->Eof ) {
		try {
			for ( int i = 0 ; i < ColumnCount ; i++ ) {
				pField = queryMaster->Fields->operator [](i);
				if ( Type == TYPE_HYDROMETER ) {
					if ( i == 2 || i == 3 || i == 4 ) {
						Data = FloatToStrF((Extended)(pField->AsFloat),ffFixed, 10, 3); //
					} else {
						Data = pField->AsString;
					}
				} else if ( Type == TYPE_WATERLEVEL ) {
					if ( i == 2 || i >= 5 ) {
						Data = FloatToStrF((Extended)(pField->AsFloat),ffFixed, 10, 3); //
					} else {
						Data = pField->AsString;
					}
				} else if ( Type == TYPE_VTHLOGGER ) {
					Data = pField->AsString;
				} else {
					// nothing to do
				}
				if ( i == 0 ) DataStr += Data;
				else          DataStr += "," + Data;
			} // for
			DataStr += "\n";
			AnsiData = DataStr;
			fwrite(AnsiData.c_str(), 1, AnsiData.Length(),fp);
			queryMaster->Next();
			DataStr = "";
			bError = false;
		} catch ( Exception &e ) {
			ShowMessage(e.Message);
			ErrorMessage = e.Message;
			bError = true;
			break;
		}
	}

	fclose(fp);

	if ( bError ) {
		//nothing to do..
	} else {
		ShowMessage("Ok");
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSearchData::ChartHydroMeter3ClickSeries(TCustomChart *Sender,
          TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	//셀별 유속 및 신호강도 호출
	if (FormHydroMeterCell == NULL) FormHydroMeterCell = new TFormHydroMeterCell(this);

	UnicodeString sMeasureDate, sMeasureTime;

	switch(m_enQueryType)
	{
		case enQueryDay:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f);
			sMeasureTime = FormatDateTime("hhnndd", m_ToTime - 1 + (ValueIndex + 1) / 144.0f).SubString(1, 3) + "000";
			break;
		case enQueryWeek:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 7 + (ValueIndex + 1) / 24.0f);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 7 + (ValueIndex + 1) / 24.0f).SubString(1, 3) + "000";
			break;
		case enQueryMonth:
			sMeasureDate = FormatDateTime("yyyymmdd", m_ToTime - 30 + ValueIndex);
			sMeasureTime = FormatDateTime("hhnn", m_ToTime - 30 + ValueIndex).SubString(1, 3) + "000";
			break;
	} // switch

	EN_HYDRO_NO enHydroNo = enHydroThird;
	UnicodeString TableName = "RHydroMeter3";

	enHydroNo = enHydroThird;
	TableName = "RHydroMeter3";

	UnicodeString sSQL = " SELECT HydroKind FROM  RHydroMeter3"
						 " WHERE  MeasureDate = '" + sMeasureDate + "' AND " +
						 "        MeasureTime = '" + sMeasureTime + "'     ";

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	UnicodeString sHydroKind;

	while ( !query->Eof ) {
		sHydroKind = query->FieldByName("HydroKind")->AsString;
		query->Next();
	}

	if ( sHydroKind == CsRQ30 || sHydroKind == CsRQ30D ) {
		ShowMessage("RQ-30/RQ-30d 장비는 셀별 유속과 빔 강도가 없습니다.");
		delete FormHydroMeterCell;
		FormHydroMeterCell = NULL;
		return;
	}

	FormHydroMeterCell->sMeasureDate = sMeasureDate;
	FormHydroMeterCell->sMeasureTime = sMeasureTime;
	FormHydroMeterCell->sHydroKind   = sHydroKind;

	//if (Series == Series1) FormHydroMeterCell->sHydroKind = sHydroKind;
	//else                   FormHydroMeterCell->sHydroKind = sHydroKind2;

	FormHydroMeterCell->enHydroNo = enHydroNo;	// 1,2,3 유속계
    FormHydroMeterCell->Show();
    FormHydroMeterCell->ShowPeriod();
	FormHydroMeterCell->DoQuery();
}

//---------------------------------------------------------------------------
// 유속계1번의 유속 Chart
void __fastcall TFormSearchData::rbH1VelocityClick(TObject *Sender)
{
	DoQuery();
//	DoQueryHydro1Graph();
}

//---------------------------------------------------------------------------
// 유속계1번의 수위 Chart
void __fastcall TFormSearchData::rbH1WaterLevelClick(TObject *Sender)
{
	DoQuery();
//	DoQueryHydro1WaterGraph();
}
//---------------------------------------------------------------------------

