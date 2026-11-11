//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <ustring.h>

#include "UnitWaterLevel.h"
#include "UnitMain.h"
#include "LockeCommon.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormWaterLevel *FormWaterLevel = NULL;

//---------------------------------------------------------------------------
__fastcall TFormWaterLevel::TFormWaterLevel(TComponent* Owner)
	: TForm(Owner)
{
	query->Connection = FormMain->SQLConnection;

	m_enQueryType = enQueryDay;

	m_FromTime = Now();
	m_ToTime   = Now();

	DateDay    = Now(); //현재일
	DateWeek   = Now(); //현재일
	DateMonth  = Now(); //현재일

	DoQuery();
	ShowPeriod();

	m_enWaterLevel = enSensorIdle;
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::TimerTimer(TObject *Sender)
{
	if (FormMain->DBEnv.sWaterLevelDeviceKind == "NONE" || FormMain->DBEnv.sWaterLevelDeviceKind == "") return;
    if (FormMain->bRunning )
    {
    	static int i = 1;
    	i++;
    	if (i > 5) i = 2;
        SetBitmapImageListIndex(Image, ImageList, i);
    } else SetBitmapImageListIndex(Image, ImageList, 1); //normal
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormWaterLevel = NULL;
    FormMain->actWaterLevel->Checked = False;
    Action = caFree;
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::SpeedButtonDayClick(TObject *Sender)
//일
{
	m_enQueryType = enQueryDay;
	DateDay = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::SpeedButtonWeekClick(TObject *Sender)
//주
{
	m_enQueryType = enQueryWeek;
	DateWeek = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::SpeedButtonMonthClick(TObject *Sender)
//월
{
	m_enQueryType = enQueryMonth;
	DateMonth = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void TFormWaterLevel::ShowPeriod()
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
	} //switch
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::btnPriorClick(TObject *Sender)
//이전
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
void __fastcall TFormWaterLevel::btnNextClick(TObject *Sender)
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
void TFormWaterLevel::DoQuery()
//조회
{
//	bool bRQ30Flag = false;
//
//	if ( FormMain->DBEnv.sWaterLevelDeviceKind == "NONE" ||  FormMain->DBEnv.sWaterLevelDeviceKind == "" ) {
//		if ( FormMain->DBEnv.sHD1Kind == CsRQ30 || FormMain->DBEnv.sHD1Kind == CsRQ30D ) bRQ30Flag = true;	// 수위계: RHyroMeter1에서 참조
//		else bRQ30Flag = false;	// 수위계 => RWaterLevel에서 참조
//	} else {
//		bRQ30Flag = false;	// 수위 데이터 => RWaterLevel에서 참조
//	}

	//if ( FormMain->DBEnv.sHD1Kind == CsRQ30 || FormMain->DBEnv.sHD1Kind == CsRQ30D ) bRQ30Flag = true;	// RQ-30

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

	//if ( bRQ30Flag ) {	// RQ-30 / RQ-30d
	if ( FormMain->DBEnv.sWaterUseFlag == HYDROMETER_STR ) {
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT '' AS MeasureDate, "
				"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
				"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >=  '" + sFromTime + "' )  OR  " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
				"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"          Value01 <> 0 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT '' AS MeasureDate, "
				"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
				"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"          Value01 <> 0 ";
		} else {	// Month
			sSQL =
				" SELECT MeasureDate, "
				"        Min(Value01) AS MinWaterLevel, Max(Value01) AS MaxWaterLevel, Avg(Value01) AS AvgWaterLevel, "
				"        Min(Value01) AS MinWater, Avg(Value01) AS AvgWater, Max(Value01) AS MaxWater "
				" FROM   RHydroMeter1 "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + ")) AND " +
				"         Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"         Value01 <> 0 " +
				" GROUP BY MeasureDate   " +
				" ORDER BY AvgWaterLevel ";
		}
	} else {	// FormMain->DBEnv.sWaterUseFlag == WATERLEVEL_STR
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT '' AS MeasureDate, "
				"        Min(Value03 + Value10) AS MinWaterLevel, Max(Value03 + Value10) AS MaxWaterLevel, Avg(Value03 + Value10) AS AvgWaterLevel, "
				"        Min(Value03) AS MinWater, Avg(Value03) AS AvgWater, Max(Value03) AS MaxWater "
				" FROM   RWaterLevel "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >=  '" + sFromTime + "' )  OR  " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
				"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"          Value03+Value10 <> 0 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT '' AS MeasureDate, "
				"        Min(Value03 + Value10) AS MinWaterLevel, Max(Value03 + Value10) AS MaxWaterLevel, Avg(Value03 + Value10) AS AvgWaterLevel, "
				"        Min(Value03) AS MinWater, Avg(Value03) AS AvgWater, Max(Value03) AS MaxWater "
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
				"        Min(Value03 + Value10) AS MinWaterLevel, Max(Value03 + Value10) AS MaxWaterLevel, Avg(Value03 + Value10) AS AvgWaterLevel, "
				"        Min(Value03) AS MinWater, Avg(Value03) AS AvgWater, Max(Value03) AS MaxWater "
				" FROM   RWaterLevel "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + ")) AND " +
				"         Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"         Value03+Value10 <> 0 " +
				" GROUP BY MeasureDate   " +
				" ORDER BY AvgWaterLevel ";
		}
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
			fMin2 = query->FieldByName("MinWaterLevel")->AsFloat;
			fMax2 = query->FieldByName("MaxWaterLevel")->AsFloat;
			fAvg2 = query->FieldByName("AvgWaterLevel")->AsFloat;
		} else { // By Month
			fMin2 = query->FieldByName("AvgWaterLevel")->AsFloat;
			fMax2 = query->FieldByName("AvgWaterLevel")->AsFloat;
			fAvg2 = query->FieldByName("AvgWaterLevel")->AsFloat;
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

	float fOffset = FormMain->DBEnv.fWaterLevelOffset;	// WaterLevel Offset

	pnlAvg->Caption = FormatFloat("0.00", fAvg);
	pnlMin->Caption = FormatFloat("0.00", fMin);
	pnlMax->Caption = FormatFloat("0.00", fMax);

	//fMax += fOffset;	// Offset 값 보정
	//fMin += fOffset;	// Offset 값 보정
	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
	fMax  = ( fMax >= 0 ) ? fMax * 1.2f : fMax * 0.8f;
	fMin  = ( fMin >= 0 ) ? fMin * 0.8f : fMin * 1.2f;
	if ( fMax == 0 ) fMax =  1.0f;
	if ( fMin == 0 ) fMin = -1.0f;
	//LeftAxis 그래프값 설정
	Chart->LeftAxis->Minimum   = -50000.0f;
	Chart->LeftAxis->Maximum   =  50000.0f;
	Chart->LeftAxis->Minimum   = fMin;
	Chart->LeftAxis->Maximum   = fMax;
	Chart->LeftAxis->Automatic = false;
	Chart->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		Chart->BottomAxis->Minimum = 0;
		Chart->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		Chart->BottomAxis->Minimum = 0;
		Chart->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		Chart->BottomAxis->Minimum = 0;
		Chart->BottomAxis->Maximum = 31-1;		// 31건
	}
	Chart->BottomAxis->Automatic = false;
	Chart->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	Series1->Clear();	// WaterLevel
	//Series2->Clear();   // 해발고도

	UnicodeString sOffset =  UnicodeString::FloatToStrF(fOffset, ffFixed, 10, 3); // -2.1935 => -2.194

	//if ( bRQ30Flag ) {	// RQ-30 / RQ-30d
	if ( FormMain->DBEnv.sWaterUseFlag == HYDROMETER_STR ) {
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, 'RQ30' AS WaterKind, Value01 AS WaterLevel "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
				"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
				" ORDER BY 1, 2 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, 'RQ30' AS WaterKind, Value01 AS WaterLevel "
				" FROM   RHydroMeter1 "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
				" ORDER BY 1, 2 ";
		} else {
			sSQL =
				" SELECT MeasureDate, '000000' AS MeasureTime, 'RQ30' AS WaterKind, AVG(Value01) AS WaterLevel "
				" FROM   RHydroMeter1 "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + ")) AND " +
				"         Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"         Value01 <> 0 " +
				" GROUP BY MeasureDate ";
		}

	} else {	// FormMain->DBEnv.sWaterUseFlag == WATERLEVEL_STR
		if ( m_enQueryType == enQueryDay ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, WaterKind, Value03 + Value10 AS WaterLevel "
				" FROM   RWaterLevel "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
				"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
				" ORDER BY 1, 2 ";
		} else if ( m_enQueryType == enQueryWeek ) {
			sSQL =
				" SELECT MeasureDate, MeasureTime, WaterKind, Value03 + Value10 AS WaterLevel "
				" FROM   RWaterLevel "
				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
				"          Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) +
				" ORDER BY 1, 2 ";
		} else {
			sSQL =
				" SELECT MeasureDate, '000000' AS MeasureTime, '' AS WaterKind, AVG(Value03 + Value10) AS WaterLevel "
				" FROM   RWaterLevel "
				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
				" 		  MeasureTime IN (" + sAllDay + ")) AND " +
				"         Value03+Value10 BETWEEN " + HFloatToStr(FormMain->DBEnv.fWaterLowerBound,2) + " AND " + HFloatToStr(FormMain->DBEnv.fWaterUpperBound,2) + " AND " +
				"         Value03+Value10 <> 0 " +
				" GROUP BY MeasureDate ";
		}
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
			Series1->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fYVal = query->FieldByName("WaterLevel")->AsFloat;
		Series1->AddXY(iXVal++, fYVal, "", clBlue);	// Offset 값 보정하여 화면에 표시

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	}

	while ( iXVal < iLimitCount ) {
		Series1->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormWaterLevel::ChartGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
          int VALIndex, UnicodeString &LabelText)
{
    if (Sender == Chart->BottomAxis)
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
void __fastcall TFormWaterLevel::FormCreate(TObject *Sender)
{
    SetBitmapImageListIndex(Image, ImageList, 1);
    if (FormMain->DBEnv.sWaterLevelDeviceKind == "NONE" || FormMain->DBEnv.sWaterLevelDeviceKind == "") SetBitmapImageListIndex(Image, ImageList, 0); //수위계 off이면 disable 아이콘으로
}

//---------------------------------------------------------------------------
void TFormWaterLevel::ForceQuery()
{
	DateDay   = GetQueryTime(FormMain->m_enLoggerStatus); //현재일
	DateWeek  = DateDay; //현재일
	DateMonth = DateDay; //현재일

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void TFormWaterLevel::LoggingStart()
{
	// nothing to do..
}

//---------------------------------------------------------------------------
void TFormWaterLevel::LoggingContinue(EN_SENSOR_STATUS enWaterLevel)
{
	if (FormMain->DBEnv.sWaterLevelDeviceKind == "NONE" || FormMain->DBEnv.sWaterLevelDeviceKind == "" ) return;

	if ( m_enWaterLevel == enWaterLevel ) {	// Same Status
		// nothing to do..
	} else {
		m_enWaterLevel = enWaterLevel;
		if ( m_enWaterLevel == enSensorStart || m_enWaterLevel == enSensorWorking ) {
			Timer->Interval = 500;
			Timer->Enabled = true;
			SetBitmapImageListIndex(Image, ImageList, 1);
		} else if ( m_enWaterLevel == enSensorFinish ) {
			Timer->Enabled = false;
			SetBitmapImageListIndex(Image, ImageList, 1);
		} else {	// enSensorIdle, enSensorError..
			Timer->Enabled = false;
			SetBitmapImageListIndex(Image, ImageList, 1);
		}
	}
}

//---------------------------------------------------------------------------
void TFormWaterLevel::LoggingFinish()
{
	Timer->Enabled = false;	// Timer Stop
	SetBitmapImageListIndex(Image, ImageList, 1);
}

