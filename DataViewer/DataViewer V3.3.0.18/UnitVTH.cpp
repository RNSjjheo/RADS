 //---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitVTH.h"
#include "UnitMain.h"
#include "LockeCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVTH *FormVTH = NULL;
//---------------------------------------------------------------------------
__fastcall TFormVTH::TFormVTH(TComponent* Owner)
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
}

//---------------------------------------------------------------------------
void __fastcall TFormVTH::FormCreate(TObject *Sender)
{
    SetBitmapImageListIndex(Image, ImageList, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormVTH = NULL;
    FormMain->actVTHLogger->Checked = False;
    Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::btnPriorClick(TObject *Sender)
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
void __fastcall TFormVTH::SpeedButtonDayClick(TObject *Sender)
//일
{
	m_enQueryType = enQueryDay;
	DateDay = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::SpeedButtonWeekClick(TObject *Sender)
//주
{
	m_enQueryType = enQueryWeek;
	DateWeek = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::SpeedButtonMonthClick(TObject *Sender)
//월
{
	m_enQueryType = enQueryMonth;
	DateMonth = GetQueryTime(FormMain->m_enLoggerStatus); // 현재 시간 또는 4분 전의 시간

	DoQuery();
	ShowPeriod();
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::btnNextClick(TObject *Sender)
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
void TFormVTH::ShowPeriod()
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
void TFormVTH::DoQuery()
//조회
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
			" WHERE  ( MeasureDate = '" + sFromDate + "' AND MeasureTime >=  '" + sFromTime + "' ) OR " +
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

	//RightAxis 그래프값 설정(온도)
	Chart->RightAxis->Automatic = false;
	Chart->RightAxis->Minimum = -10.0f;
	Chart->RightAxis->Maximum =  80.0f;
	Chart->RightAxis->Increment = 1;
	//LeftAxis 그래프값 설정(전압)
	Chart->LeftAxis->Automatic = false;
	Chart->LeftAxis->Minimum =  0.0f;
	Chart->LeftAxis->Maximum = 20.0f;
	Chart->LeftAxis->Automatic = false;
	//Chart->LeftAxis->Increment = ((fMax-fMin) <= 30) ? 1 : 10;	// Increment : 1,2,3,4,.. , 10, 20, 30, ...
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
	Series1->Clear();	// DCCharge
	Series2->Clear();   // Temperature
	Series3->Clear();   // DCBattery

	if ( m_enQueryType == enQueryDay ) {
		sSQL =
			" SELECT MeasureDate, MeasureTime, VTHKind, Volt, Temperature, Humidity, DCCharge, DCBattery "
			" FROM   RVTHLogger "
			" WHERE  ( MeasureDate = '" + sFromDate + "' AND MeasureTime >=  '" + sFromTime + "' ) OR " +
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
			Series1->AddXY(iXVal, 0, "", clGray);
			Series2->AddXY(iXVal, 0, "", clGray);
			Series3->AddXY(iXVal, 0, "", clGray);
			iXVal++;
			if (iXVal >= iLimitCount) break;
		} // while

		fDCCharge    = query->FieldByName("DCCharge")->AsFloat;
		fTemperature = query->FieldByName("Temperature")->AsFloat;
		fDCBattery   = query->FieldByName("DCBattery")->AsFloat;
		Series1->AddXY(iXVal, fDCCharge,    "",clBlue);
		Series2->AddXY(iXVal, fDCBattery,   "",clRed);
		Series3->AddXY(iXVal, fTemperature, "",clFuchsia);

		iXVal++;
		Time =   (m_enQueryType == enQueryDay  ) ? (Time + OneMinute) :
				((m_enQueryType == enQueryWeek ) ? (Time + OneHour)   : (Time + OneDay));

		query->Next();
	} // while

	while ( iXVal < iLimitCount ) {
		Series1->AddXY(iXVal, 0, "",clGray);
		Series2->AddXY(iXVal, 0, "",clGray);
		Series3->AddXY(iXVal, 0, "",clGray);
		iXVal++;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormVTH::ChartGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
          int ValueIndex, UnicodeString &LabelText)
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
void TFormVTH::ForceQuery()
{
	DateDay   = GetQueryTime(FormMain->m_enLoggerStatus); //현재일
	DateWeek  = DateDay; //현재일
	DateMonth = DateDay; //현재일

	DoQuery();
	ShowPeriod();
}
//---------------------------------------------------------------------------

