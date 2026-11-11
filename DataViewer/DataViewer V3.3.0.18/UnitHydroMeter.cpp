//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitHydroMeter.h"
#include "LockeCommon.h"
#include "UnitMain.h"
#include "UnitHydroMeterCell.h"
#include "DvEnv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormHydroMeter *FormHydroMeter = NULL;

//---------------------------------------------------------------------------
// Main Function
//---------------------------------------------------------------------------
__fastcall TFormHydroMeter::TFormHydroMeter(TComponent* Owner)
	: TForm(Owner)
{
	query->Connection = FormMain->SQLConnection;

	m_enQueryType  = enQueryDay;
	m_enHydro1     = enSensorIdle;	// Sensor1 Working Status
	m_enHydro2     = enSensorIdle;	// Sensor2 Working Status

	m_FromTime = Now();
	m_ToTime   = Now();

	DateDay    = Now(); //현재일
	DateWeek   = Now(); //현재일
	DateMonth  = Now(); //현재일

	sHydroKind  =  FormMain->DBEnv.sHD1Kind;
	sHydroKind2 =  FormMain->DBEnv.sHD2Kind;
	m_bRQ30Flag = false;
	if ( sHydroKind == CsRQ30 || sHydroKind == CsRQ30D || sHydroKind == CsEWSVConverter ) m_bRQ30Flag = true;

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::TimerTimer(TObject *Sender)
{
	if (sHydroKind == "NONE" || sHydroKind == "") {
		// nothing to do..
	} else {
		if (FormMain->bRunning && (m_enHydro1 == enSensorStart || m_enHydro1 == enSensorWorking))
		{
			static int i = 1;
			i++;
			if (i > 12) i = 2;
			if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, i);
			else if ( sHydroKind == CsChannelMaster )                 SetBitmapImageListIndex(Image, ImageListCM, i); //기본 channelmaster
			else {}
		} else
		{
			if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, 1); //normal
			else if ( sHydroKind == CsChannelMaster )                 SetBitmapImageListIndex(Image, ImageListCM, 1); //normal
			else {}
		}
	}

	if (sHydroKind2 == "NONE" || sHydroKind2 == "") {
		// nothing to do..
	} else {
		if (FormMain->bRunning && (m_enHydro2 == enSensorStart || m_enHydro2 == enSensorWorking))
		{
			static int j = 1;
			j++;
			if (j > 12) j = 2;
			if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, j);
			else if ( sHydroKind2 == CsChannelMaster )                  SetBitmapImageListIndex(Image2, ImageListCM, j); //기본 channelmaster
			else {}
		} else
		{
			if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, 1); //normal
			else if ( sHydroKind2 == CsChannelMaster )                  SetBitmapImageListIndex(Image2, ImageListCM, 1); //normal
			else {}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormHydroMeter = NULL;
    FormMain->actHydroMeter->Checked = False;
    Action = caFree;
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::sbtnDayClick(TObject *Sender)
//일
{
	m_enQueryType = enQueryDay;
	DateDay = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::sbtnWeekClick(TObject *Sender)
//주
{
	m_enQueryType = enQueryWeek;
	DateWeek = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::sbtnMonthClick(TObject *Sender)
//월
{
	m_enQueryType = enQueryMonth;
	DateMonth = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void TFormHydroMeter::ShowPeriod()
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
void __fastcall TFormHydroMeter::btnPriorClick(TObject *Sender)
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
void __fastcall TFormHydroMeter::Button5Click(TObject *Sender)
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
void TFormHydroMeter::DoQuery()
//조회
{
	DoQueryHydro1();
	DoQueryHydro2();
	DoQueryHydro3();

//	pnlAvg->Caption  = FormatFloat("0.00", 0.0f);
//	pnlMin->Caption  = FormatFloat("0.00", 0.0f);
//	pnlMax->Caption  = FormatFloat("0.00", 0.0f);
//
//	pnlAvg2->Caption = FormatFloat("0.00", 0.0f);
//	pnlMin2->Caption = FormatFloat("0.00", 0.0f);
//	pnlMax2->Caption = FormatFloat("0.00", 0.0f);
//
//    //min/max를 구함
//	query->Close();
//	query->DataSet->CommandText = "";
//
//	UnicodeString sFromDate,    sFromTime;
//	UnicodeString sToDate,      sToTime;
//	UnicodeString sFromAllDay,  sToAllDay;
//	TDateTime     FromAllDay,   ToAllDay;
//
//	if ( m_enQueryType == enQueryDay ) {
//		m_ToTime   = AdjustDateTime(DateDay, m_enQueryType);   	// 10:23:23 => 10:20:00
//		m_FromTime = m_ToTime - 1 + EncodeTime(0,10,0,0);
//	} else if ( m_enQueryType == enQueryWeek ) {
//		m_ToTime   = AdjustDateTime(DateWeek, m_enQueryType); 	// 10:23:23 => 10:00:00
//		m_FromTime = m_ToTime - 7 + EncodeTime(1,0,0,0);
//		FromAllDay = m_FromTime + 1;
//		ToAllDay   = m_ToTime -1;
//	} else {
//		m_ToTime   = AdjustDateTime(DateMonth, m_enQueryType);	// 2014-10-06 10:23:23 => 2014-10-16 00:00:00
//		m_FromTime = m_ToTime - 30;   // 31일(오늘까지 포함하므로 -30일한다.)
//	}
//
//	sFromDate = m_FromTime.FormatString("yyyymmdd");
//	sFromTime = m_FromTime.FormatString("hhnnss");
//	sToDate   = m_ToTime.FormatString("yyyymmdd");
//	sToTime   = m_ToTime.FormatString("hhnnss");
//
//	sFromAllDay = FromAllDay.FormatString("yyyymmdd");
//	sToAllDay   = ToAllDay.FormatString("yyyymmdd");
//
//	UnicodeString sBefore, sAfter, sAllDay;
//	sBefore = GetBeforeHourStr(m_ToTime); 	// '000000','010000','020000',....,'090000'
//	sAfter  = GetAfterHourStr(m_FromTime);	// '100000','110000','120000','130000',...,'230000'
//	sAllDay = GetFullDayHourStr();			// '000000','010000','020000',....,'230000'
//
//	//-----------------------------------------------------------------------
//	// Min/Max
//	//-----------------------------------------------------------------------
//	UnicodeString sSQL;
//
//	if ( m_bRQ30Flag ) {
//
//		if ( m_enQueryType == enQueryDay ) {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1  "
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
//				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//				" UNION " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
//				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
//		} else if ( m_enQueryType == enQueryWeek ) {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1 "
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//				" UNION " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
//		} else {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1 "
//				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
//				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
//				"         AvgVelocity <> 0  AND " +
//				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//				" UNION  " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
//				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
//				"         AvgVelocity <> 0 AND " +
//				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2);
//		}
//		query->Close();
//		query->DataSet->CommandText = sSQL;
//		query->Open();
//
//	} else {    // CM, Sontek,...
//
//		if ( m_enQueryType == enQueryDay ) {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1  "
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
//				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//				" UNION " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
//				"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
//		} else if ( m_enQueryType == enQueryWeek ) {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1 "
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//				" UNION " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//				" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//				" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//				" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//				"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
//		} else {
//			sSQL =
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
//				" FROM   RHydroMeter1 "
//				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
//				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
//				"         AvgVelocity <> 0  AND " +
//				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//				" UNION  " +
//				" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV " +
//				" FROM   RHydroMeter2 " +
//				" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
//				" 		  MeasureTime IN (" + sAllDay + "))  AND " +
//				"         AvgVelocity <> 0 AND " +
//				"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
//		}
//		query->Close();
//		query->DataSet->CommandText = sSQL;
//		query->Open();
//	}
//
//	float fMin,  fMax;
//	float fMin2, fMax2;
//	fMin =  999999.0f;
//	fMax = -999999.0f;
//	while ( !query->Eof ) {
//		fMin2 = query->FieldByName("MinV")->AsFloat;
//		fMax2 = query->FieldByName("MaxV")->AsFloat;
//		if ( fMin2 < fMin ) fMin = fMin2;
//		if ( fMax2 > fMax ) fMax = fMax2;
//		query->Next();
//	}
//
//	//LeftAxis 그래프값 설정
//	if ( fMax < 0 ) fMax = 0;	// 어떻게든 0은 포함하고 있어야 함.
//	if ( fMin > 0 ) fMin = 0;   // 어떻게든 0은 포함하고 있어야 함.
//	fMax  = ( fMax >= 0 ) ? fMax * 1.1f : fMax * 0.9f;
//	fMin  = ( fMin >= 0 ) ? fMin * 0.9f : fMin * 1.1f;
//	if ( fMax == 0 ) fMax =  1.0f;
//	if ( fMin == 0 ) fMin = -1.0f;
//	//LeftAxis 그래프값 설정
//	Chart->LeftAxis->Minimum   = -50000.0f;
//	Chart->LeftAxis->Maximum   =  50000.0f;
//	Chart->LeftAxis->Minimum   = fMin;
//	Chart->LeftAxis->Maximum   = fMax;
//	Chart->LeftAxis->Automatic = false;
//	Chart->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
//	//BottomAxis 그래프값 설정
//	if ( m_enQueryType == enQueryDay ) {
//		Chart->BottomAxis->Minimum = 0;
//		Chart->BottomAxis->Maximum = 144-1;		// 144건
//	} else if ( m_enQueryType == enQueryWeek ) {
//		Chart->BottomAxis->Minimum = 0;
//		Chart->BottomAxis->Maximum = 168-1;		// 168건
//	} else {
//		Chart->BottomAxis->Minimum = 0;
//		Chart->BottomAxis->Maximum = 31-1;		// 31건
//	}
//	Chart->BottomAxis->Automatic = false;
//	Chart->BottomAxis->Increment = 1;
//
//	// Series Clear ---------------------------------------------------------
//	Series1->Clear(); //하층
//	Series2->Clear(); //상층
//
//	bool Hydro2 = true;
//	if ( FormMain->DBEnv.sHD2Kind == "NONE" ) {	// HydroMeter2 => not exist
//		Image2->Visible = false;
//		pnlMin2->Visible = false;
//		pnlAvg2->Visible = false;
//		pnlMax2->Visible = false;
//		Label4->Visible = false;
//		Label5->Visible = false;
//		Label6->Visible = false;
//
//		Series2->Visible = false;
//		Hydro2 = false;
//	} else {	// HydroMeter2 => exist
//		Image2->Visible = true;
//		pnlMin2->Visible = true;
//		pnlAvg2->Visible = true;
//		pnlMax2->Visible = true;
//		Label4->Visible = true;
//		Label5->Visible = true;
//		Label6->Visible = true;
//
//		Series2->Visible = true;
//		Hydro2 = true;
//	}
//
//	//상층을 먼저 처리하고 하층을 나중에 처리하므로 상층만 있으면 평균/최소/최고유속은 상층기준이 되고, 하층만 있거나 상/하층 둘 다 있을 경우는 하층이 기준이 된다
//	for(int i = 0; i < 2; i++)
//	{
//		if ( i == 1 && Hydro2 == false ) break;	// 상층 유속계, None이면 루틴을 타지 않는다.
//
//		UnicodeString LocalHydroKind, LocalHydroKind2;
//		UnicodeString sTableName;
//		TLineSeries  *Series = NULL;
//		if ( i == 0 ) {
//			sTableName = "RHydroMeter1";
//			Series = Series1;
//		} else {
//			sTableName = "RHydroMeter2";
//			Series = Series2;
//		}
//
//		query->Close();
//		if ( m_bRQ30Flag ) {    // rq30 device : unit: meter
//			if ( m_enQueryType == enQueryDay ) {
//				sSQL =
//					" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
//					"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
//					"        AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//					" ORDER BY 1, 2 ";
//			} else if ( m_enQueryType == enQueryWeek ) {
//				sSQL =
//					" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//					" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//					" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//					" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//					"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//					" ORDER BY 1, 2 ";
//			} else {
//				sSQL =
//					" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
//					" 		   MeasureTime IN (" + sAllDay + ")) AND " +
//					"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound/1000, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound/1000, 2) +
//					" GROUP BY MeasureDate ";
//			}
//		} else {   	// other hydrometer  device ( unit: mm )
//			if ( m_enQueryType == enQueryDay ) {
//				sSQL =
//					" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
//					"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
//					"        AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//					" ORDER BY 1, 2 ";
//			} else if ( m_enQueryType == enQueryWeek ) {
//				sSQL =
//					" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
//					" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
//					" 		   MeasureTime IN (" + sAllDay + ")) OR " +
//					" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
//					"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//					" ORDER BY 1, 2 ";
//			} else {
//				sSQL =
//					" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
//					" FROM   " + sTableName +
//					" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
//					" 		   MeasureTime IN (" + sAllDay + ")) AND " +
//					"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
//					" GROUP BY MeasureDate ";
//
//			}
//		}
//		query->Close();
//		query->DataSet->CommandText = sSQL;
//		query->Open();
//
//		int   iXVal  = 0;
//		float fYVal  = 0.0f;
//
//		TDateTime OneMinute = EncodeTime(0,10,0,0);
//		TDateTime OneHour   = EncodeTime(1,0,0,0);
//		TDateTime OneDay    = 1;
//
//		TDateTime Time   = m_FromTime;	// 이 시간에서 시작함.
//		int iLimitCount  = (m_enQueryType == enQueryDay) ? 144 : (m_enQueryType == enQueryWeek) ? 168 : 31;
//
//		while(!query->Eof)
//		{
//			UnicodeString sDate = query->FieldByName("MeasureDate")->AsString;
//			UnicodeString sTime = query->FieldByName("MeasureTime")->AsString;
//			TDateTime RecTime   = EncodeDate(sDate.SubString(1, 4).ToIntDef(2000), sDate.SubString(5, 2).ToIntDef(1), sDate.SubString(7, 2).ToIntDef(1)) +
//								  EncodeTime(sTime.SubString(1, 2).ToIntDef(0), sTime.SubString(3, 2).ToIntDef(0), sTime.SubString(5, 2).ToIntDef(0), 0);
//
//			//while ( Time < RecTime ) { // 두 개의 double형 값이나 같은 값을 가지고 있으나 작은 것으로 인식하기 때문에 이렇게 인식
//			while ( FormatDateTime("yyyymmddhhnnss", Time) != FormatDateTime("yyyymmddhhnnss", RecTime) ) {
//				Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
//						((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
//				Series->AddXY(iXVal, 0, "", clGray);
//				iXVal++;
//				if (iXVal >= iLimitCount) break;
//			} // while
//
//
//			fYVal = query->FieldByName("AvgVal")->AsFloat;
//			if ( m_bRQ30Flag ) {
//				if ( fYVal <= -99.0f ) {
//					Series->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
//				} else {
//					Series->AddXY(iXVal++, fYVal, "", ((Series == Series1) ? clBlue : clRed));					// Add Series Point
//				}
//			} else {	// CM, SL, AG
//				Series->AddXY(iXVal++, fYVal, "", ((Series == Series1) ? clBlue : clRed));					// Add Series Point
//			}
//
//			if ( query->RecNo == query->RecordCount) {
//				float Avg, Min, Max;
//				Avg = query->FieldByName("AvgVal")->AsFloat;
//				Min = query->FieldByName("MinVal")->AsFloat;
//				Max = query->FieldByName("MaxVal")->AsFloat;
//
//				if ( i == 0 ) {	// i==0: Hydro1
//					pnlAvg->Caption = FormatFloat("0.00", Avg);	// m/sec => mm/sec
//					pnlMin->Caption = FormatFloat("0.00", Min);
//					pnlMax->Caption = FormatFloat("0.00", Max);
//				} else {		// i==1, Hydro2
//					pnlAvg2->Caption  = FormatFloat("0.00", Avg);	// m/sec => mm/sec
//					pnlMin2->Caption  = FormatFloat("0.00", Min);
//					pnlMax2->Caption  = FormatFloat("0.00", Max);
//				}
//			}
//
//			Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
//					((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));
//
//			query->Next();
//		} // while
//
//		int k = 0;
//
//		// ToTime까지 도달하지 못했을 때를 가정하여 .
//		while ( iXVal < iLimitCount ) {
//			Series->AddXY(iXVal, 0, "",clGray);
//			iXVal++;
//		}
//	} // for
//
//	if ( m_bRQ30Flag ) StaticTextUnit->Caption = "m/sec";
//	else               StaticTextUnit->Caption = "mm/sec";
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::Chart1GetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
		  int ValueIndex, UnicodeString &LabelText)
{
	if (Sender == Chart1->BottomAxis || Sender == Chart2->BottomAxis || Sender == Chart3->BottomAxis)
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
void __fastcall TFormHydroMeter::Chart1ClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
          int Y)
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

	EN_HYDRO_NO enHydroNo = enHydroFirst;	// default 1번 유속계
	UnicodeString HydroTableName = "RHydroMeter1";
	if ( Series == Series1 ) {
		enHydroNo = enHydroFirst;			// 1번 유속계
		HydroTableName = "RHydroMeter1";
	} else if ( Series == LineSeries1 ) {
		enHydroNo = enHydroSecond;	// 2번 유속계
		HydroTableName = "RHydroMeter2";
	} else if ( Series == LineSeries3 ) {
		enHydroNo = enHydroThird;	// 3번 유속계
		HydroTableName = "RHydroMeter3";
	} else {	// default
		enHydroNo = 1;			// 1번 유속계
		HydroTableName = "RHydroMeter1";
	}


	UnicodeString sSQL = " SELECT HydroKind FROM " + HydroTableName +
						 " WHERE  MeasureDate = '" + sMeasureDate   + "' AND " +
						 "        MeasureTime = '" + sMeasureTime   + "'     ";

	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	UnicodeString sHydroKind;

	while ( !query->Eof ) {
		sHydroKind = query->FieldByName("HydroKind")->AsString;
		query->Next();
	}

	FormHydroMeterCell->sMeasureDate = sMeasureDate;
	FormHydroMeterCell->sMeasureTime = sMeasureTime;
	FormHydroMeterCell->sHydroKind   = sHydroKind;

	if ( sHydroKind == CsRQ30 || sHydroKind == CsRQ30D ) {
		ShowMessage("RQ-30/RQ-30d 장비는 셀별 유속과 빔 강도가 없습니다.");
		delete FormHydroMeterCell;
		FormHydroMeterCell = NULL;
		return;
	}

	//if (Series == Series1) FormHydroMeterCell->sHydroKind = sHydroKind;
	//else                   FormHydroMeterCell->sHydroKind = sHydroKind2;

	FormHydroMeterCell->enHydroNo = enHydroNo;	// 1/2/3번 유속계
	FormHydroMeterCell->Show();
	FormHydroMeterCell->ShowPeriod();
	FormHydroMeterCell->DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeter::FormCreate(TObject *Sender)
{
	//UnicodeString sHydroKind = (FormMain->DBEnv.sHD1Kind != "NONE" && FormMain->DBEnv.sHD1Kind != "") ? FormMain->DBEnv.sHD1Kind : FormMain->DBEnv.sHD2Kind;
	//UnicodeString sHydroKind  =  FormMain->DBEnv.sHD1Kind;
	//UnicodeString sHydroKind2 =  FormMain->DBEnv.sHD2Kind;

	if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries)
	{
		SetBitmapImageListIndex(Image, ImageListSontek, 1);
	} else if ( sHydroKind == CsChannelMaster )
	{
		SetBitmapImageListIndex(Image, ImageListCM, 1);
	} else
	{
		SetBitmapImageListIndex(Image, ImageListCM, 0);
		pnlMin->Color = clSilver;
		pnlAvg->Color = clSilver;
		pnlMax->Color = clSilver;
	}

	if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries)
	{
		SetBitmapImageListIndex(Image2, ImageListSontek, 1);
	} else if ( sHydroKind2 == CsChannelMaster )
	{
		SetBitmapImageListIndex(Image2, ImageListCM, 1);
	} else
	{
		SetBitmapImageListIndex(Image2, ImageListCM, 0);
		pnlMin2->Color = clSilver;
		pnlAvg2->Color = clSilver;
		pnlMax2->Color = clSilver;
	}

	PageControl1->ActivePageIndex = 0;
}

//---------------------------------------------------------------------------
void TFormHydroMeter::ForceQuery()
{
	// ForceQuery는 4분을 빼지 않음. ForceQuery는 LoggerFinish에 동작하므로 현재 시간에서 확정된 데이터를 Query한다.
//    iQueryKind = 30;
	DateDay   = GetQueryTime(FormMain->m_enLoggerStatus); //현재일
	DateWeek  = DateDay; //현재일
	DateMonth = DateDay; //현재일

	LoggingFinish(); 	// Icon Animation Stop

	DoQuery();
	ShowPeriod();
}

//---------------------------------------------------------------------------
void TFormHydroMeter::LoggingStart()
{
//
}

//---------------------------------------------------------------------------
void TFormHydroMeter::LoggingContinue(EN_SENSOR_STATUS enHydro1, EN_SENSOR_STATUS enHydro2)
{
	if ((sHydroKind  == "NONE" || sHydroKind  == "") &&
		(sHydroKind2 == "NONE" || sHydroKind2 == "")) return;

	if ( sHydroKind != "NONE" && sHydroKind != "" ) {
		if ( m_enHydro1 == enHydro1 ) {	// 상태 변화 없음. => 처리 없음.
			// nothing to do..
		} else {
			m_enHydro1 = enHydro1;
			if ( m_enHydro1 == enSensorStart || m_enHydro1 == enSensorWorking ) {
				Timer->Interval = 500;
				Timer->Enabled  = true;
				if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, 1); //normal
				else                                                      SetBitmapImageListIndex(Image, ImageListCM, 1); //normal
			} else if ( m_enHydro1 == enSensorFinish ) {
				if ( enHydro2 == enSensorFinish) Timer->Enabled = false;
				//Timer->Enabled = false;
				if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, 1); //normal
				else                                                      SetBitmapImageListIndex(Image, ImageListCM, 1); //normal
			} else {
				Timer->Enabled = false;
				if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, 1); //normal
				else                                                      SetBitmapImageListIndex(Image, ImageListCM, 1); //normal
			}
		}
	}

	if ( sHydroKind2 != "NONE" && sHydroKind2 != "" ) {
		if ( m_enHydro2 == enHydro2 ) {	// 상태 변화 없음. => 처리 없음.
			// nothing to do..
		} else {
			m_enHydro2 = enHydro2;
			if ( m_enHydro2 == enSensorStart || m_enHydro2 == enSensorWorking ) {
				Timer->Interval = 500;
				Timer->Enabled  = true;
				if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, 1); //normal
				else                                                        SetBitmapImageListIndex(Image2, ImageListCM, 1); //normal
			} else if ( m_enHydro2 == enSensorFinish ) {
				if ( enHydro1 == enSensorFinish) Timer->Enabled = false;
				//Timer->Enabled = false;
				if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, 1); //normal
				else                                                        SetBitmapImageListIndex(Image2, ImageListCM, 1); //normal
			} else {
				Timer->Enabled = false;
				if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, 1); //normal
				else                                                        SetBitmapImageListIndex(Image2, ImageListCM, 1); //normal
			}
		}
	}

	// 점검용 코드로 센서 모두 중지되고, Timer가 Enable되어 있으면 Disable시킨다.
	if ( m_enHydro1 == enSensorFinish && m_enHydro2 == enSensorFinish ) {
		if ( Timer->Enabled ) Timer->Enabled = false;
	}
}

//---------------------------------------------------------------------------
void TFormHydroMeter::LoggingFinish()
{
	Timer->Enabled = false;	// Timer Stop

	if ( sHydroKind == "NONE" || sHydroKind == "" ) {
		// nothing to do
	} else {
		if (sHydroKind == CsSLSeries || sHydroKind == CsAGSeries) SetBitmapImageListIndex(Image, ImageListSontek, 1); //normal
		else                                                      SetBitmapImageListIndex(Image, ImageListCM, 1); //normal
	}

	if ( sHydroKind2 == "NONE" || sHydroKind2 == "") {
		// nothing to do
	} else {
		if (sHydroKind2 == CsSLSeries || sHydroKind2 == CsAGSeries) SetBitmapImageListIndex(Image2, ImageListSontek, 1); //normal
		else                                                        SetBitmapImageListIndex(Image2, ImageListCM, 1); //normal
	}
}

//---------------------------------------------------------------------------
void TFormHydroMeter::DoQueryHydro1()
{
	pnlAvg->Caption  = FormatFloat("0.00", 0.0f);
	pnlMin->Caption  = FormatFloat("0.00", 0.0f);
	pnlMax->Caption  = FormatFloat("0.00", 0.0f);

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
	double HydroLowerBound = 0.0f;
	double HydroUpperBound = 0.0f;

	if ( m_bRQ30Flag ) { // 전자파 유속계: 단위ㅣ: meter
		HydroLowerBound = FormMain->DBEnv.fHydroLowerBound / 1000;	// 전자파: mm => meter
		HydroUpperBound = FormMain->DBEnv.fHydroUpperBound / 1000;	// 전자파: mm => meter
	} else {
		HydroLowerBound = FormMain->DBEnv.fHydroLowerBound;	// advm : mm
		HydroUpperBound = FormMain->DBEnv.fHydroUpperBound;	// advm : mm
	}

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter1  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND "+
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )";
	} else {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter1 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )";
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin,  fMax;
	float fMin2, fMax2;
	fMin =  999999.0f;
	fMax = -999999.0f;
	while ( !query->Eof ) {
		fMin2 = query->FieldByName("MinV")->AsFloat;
		fMax2 = query->FieldByName("MaxV")->AsFloat;
		if ( fMin2 < fMin ) fMin = fMin2;
		if ( fMax2 > fMax ) fMax = fMax2;
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
	Chart1->LeftAxis->Minimum   = -50000.0f;
	Chart1->LeftAxis->Maximum   =  50000.0f;
	Chart1->LeftAxis->Minimum   = fMin;
	Chart1->LeftAxis->Maximum   = fMax;
	Chart1->LeftAxis->Automatic = false;
	Chart1->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		Chart1->BottomAxis->Minimum = 0;
		Chart1->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		Chart1->BottomAxis->Minimum = 0;
		Chart1->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		Chart1->BottomAxis->Minimum = 0;
		Chart1->BottomAxis->Maximum = 31-1;		// 31건
	}
	Chart1->BottomAxis->Automatic = false;
	Chart1->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	Series1->Clear(); //하층
	//Series2->Clear(); //상층

	UnicodeString LocalHydroKind, LocalHydroKind2;
	UnicodeString sTableName;
	TLineSeries  *Series = NULL;

	Series = Series1;

	query->Close();

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )" +
			" ORDER BY 1, 2 ";
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter1 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND  " +
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )" +
			" ORDER BY 1, 2 ";
	} else {
		sSQL =
			" SELECT MeasureDate, '000000' AS MeasureTime, '' AS HydroKind, Avg(AvgVelocity) AS AvgVal, Min(AvgVelocity) AS MinVal, Max(AvgVelocity) AS MaxVal "
			" FROM   RHydroMeter1 "
			" WHERE  ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) AND  " +
			"        ( AvgVelocity BETWEEN " + HFloatToStr(HydroLowerBound, 2) + " AND  " + HFloatToStr(HydroUpperBound, 2) + " )" +
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
			Series->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( fYVal == 0.0f ) {
			Series->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
		} else {
			Series->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		if ( query->RecNo == query->RecordCount) {
			float Avg, Min, Max;
			Avg = query->FieldByName("AvgVal")->AsFloat;
			Min = query->FieldByName("MinVal")->AsFloat;
			Max = query->FieldByName("MaxVal")->AsFloat;

			pnlAvg->Caption = FormatFloat("0.00", Avg);	// m/sec => mm/sec
			pnlMin->Caption = FormatFloat("0.00", Min);
			pnlMax->Caption = FormatFloat("0.00", Max);
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		Series->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}

//---------------------------------------------------------------------------
void TFormHydroMeter::DoQueryHydro2()
{
	pnlAvg2->Caption = FormatFloat("0.00", 0.0f);
	pnlMin2->Caption = FormatFloat("0.00", 0.0f);
	pnlMax2->Caption = FormatFloat("0.00", 0.0f);

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
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter2 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"         AvgVelocity <> 0  AND " +
			"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin,  fMax;
	float fMin2, fMax2;
	fMin =  999999.0f;
	fMax = -999999.0f;
	while ( !query->Eof ) {
		fMin2 = query->FieldByName("MinV")->AsFloat;
		fMax2 = query->FieldByName("MaxV")->AsFloat;
		if ( fMin2 < fMin ) fMin = fMin2;
		if ( fMax2 > fMax ) fMax = fMax2;
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
	Chart2->LeftAxis->Minimum   = -50000.0f;
	Chart2->LeftAxis->Maximum   =  50000.0f;
	Chart2->LeftAxis->Minimum   = fMin;
	Chart2->LeftAxis->Maximum   = fMax;
	Chart2->LeftAxis->Automatic = false;
	Chart2->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		Chart2->BottomAxis->Minimum = 0;
		Chart2->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		Chart2->BottomAxis->Minimum = 0;
		Chart2->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		Chart2->BottomAxis->Minimum = 0;
		Chart2->BottomAxis->Maximum = 31-1;		// 31건
	}
	Chart2->BottomAxis->Automatic = false;
	Chart2->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	LineSeries1->Clear(); //하층

	//상층을 먼저 처리하고 하층을 나중에 처리하므로 상층만 있으면 평균/최소/최고유속은 상층기준이 되고, 하층만 있거나 상/하층 둘 다 있을 경우는 하층이 기준이 된다

	TLineSeries  *Series = NULL;
	Series = LineSeries1;

	query->Close();
	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter2 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"        AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
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
			Series->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( fYVal == 0.0f ) {
			Series->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
		} else {
			Series->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		if ( query->RecNo == query->RecordCount) {
			float Avg, Min, Max;
			Avg = query->FieldByName("AvgVal")->AsFloat;
			Min = query->FieldByName("MinVal")->AsFloat;
			Max = query->FieldByName("MaxVal")->AsFloat;

			pnlAvg2->Caption = FormatFloat("0.00", Avg);	// m/sec => mm/sec
			pnlMin2->Caption = FormatFloat("0.00", Min);
			pnlMax2->Caption = FormatFloat("0.00", Max);
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	int k = 0;

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		Series->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}

//---------------------------------------------------------------------------
void TFormHydroMeter::DoQueryHydro3()
{
	pnlAvg3->Caption = FormatFloat("0.00", 0.0f);
	pnlMin3->Caption = FormatFloat("0.00", 0.0f);
	pnlMax3->Caption = FormatFloat("0.00", 0.0f);

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
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3  "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' ) OR    " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND  " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else if ( m_enQueryType == enQueryWeek ) {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime  IN ( " + sAfter + " )) OR " +
			" 	     ( MeasureDate BETWEEN '" + sFromAllDay + "' AND '" + sToAllDay   + "' AND " +
			" 		   MeasureTime IN (" + sAllDay + ")) OR " +
			" 	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime IN ( " + sBefore + " ))) AND " +
			"          AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	} else {
		sSQL =
			" SELECT Min(AvgVelocity) AS MinV, Max(AvgVelocity) AS MaxV "
			" FROM   RHydroMeter3 "
			" WHERE ( MeasureDate BETWEEN '" + sFromDate + "' AND '" + sToDate + "' AND " +
			" 		  MeasureTime IN (" + sAllDay + "))  AND " +
			"         AvgVelocity <> 0  AND " +
			"         AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	}
	query->Close();
	query->DataSet->CommandText = sSQL;
	query->Open();

	float fMin,  fMax;
	float fMin2, fMax2;
	fMin =  999999.0f;
	fMax = -999999.0f;
	while ( !query->Eof ) {
		fMin2 = query->FieldByName("MinV")->AsFloat;
		fMax2 = query->FieldByName("MaxV")->AsFloat;
		if ( fMin2 < fMin ) fMin = fMin2;
		if ( fMax2 > fMax ) fMax = fMax2;
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
	Chart3->LeftAxis->Minimum   = -50000.0f;
	Chart3->LeftAxis->Maximum   =  50000.0f;
	Chart3->LeftAxis->Minimum   = fMin;
	Chart3->LeftAxis->Maximum   = fMax;
	Chart3->LeftAxis->Automatic = false;
	Chart3->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
	//BottomAxis 그래프값 설정
	if ( m_enQueryType == enQueryDay ) {
		Chart3->BottomAxis->Minimum = 0;
		Chart3->BottomAxis->Maximum = 144-1;		// 144건
	} else if ( m_enQueryType == enQueryWeek ) {
		Chart3->BottomAxis->Minimum = 0;
		Chart3->BottomAxis->Maximum = 168-1;		// 168건
	} else {
		Chart3->BottomAxis->Minimum = 0;
		Chart3->BottomAxis->Maximum = 31-1;		// 31건
	}
	Chart3->BottomAxis->Automatic = false;
	Chart3->BottomAxis->Increment = 1;

	// Series Clear ---------------------------------------------------------
	LineSeries3->Clear(); //하층

	//상층을 먼저 처리하고 하층을 나중에 처리하므로 상층만 있으면 평균/최소/최고유속은 상층기준이 되고, 하층만 있거나 상/하층 둘 다 있을 경우는 하층이 기준이 된다

	TLineSeries  *Series = NULL;
	Series = LineSeries3;

	query->Close();
	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, HydroKind, AvgVelocity As AvgVal, MinVelocity As MinVal, MaxVelocity AS MaxVal "
			" FROM   RHydroMeter3 "
			" WHERE (( MeasureDate = '" + sFromDate + "' AND MeasureTime >= '" + sFromTime + "' )  OR  " +
			"	     ( MeasureDate = '" + sToDate   + "' AND MeasureTime <= '" + sToTime   + "' )) AND " +
			"        AvgVelocity BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2) +
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
			Series->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while


		fYVal = query->FieldByName("AvgVal")->AsFloat;
		if ( fYVal == 0.0f ) {
			Series->AddXY(iXVal++, 0, "", clBlack);					// Add Series Point
		} else {
			Series->AddXY(iXVal++, fYVal, "", clBlue);					// Add Series Point
		}

		if ( query->RecNo == query->RecordCount) {
			float Avg, Min, Max;
			Avg = query->FieldByName("AvgVal")->AsFloat;
			Min = query->FieldByName("MinVal")->AsFloat;
			Max = query->FieldByName("MaxVal")->AsFloat;

			pnlAvg3->Caption = FormatFloat("0.00", Avg);	// m/sec => mm/sec
			pnlMin3->Caption = FormatFloat("0.00", Min);
			pnlMax3->Caption = FormatFloat("0.00", Max);
		}

		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	int k = 0;

	// ToTime까지 도달하지 못했을 때를 가정하여 .
	while ( iXVal < iLimitCount ) {
		Series->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}


