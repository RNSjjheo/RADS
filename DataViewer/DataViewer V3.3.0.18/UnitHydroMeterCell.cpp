//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitHydroMeterCell.h"
#include "LockeCommon.h"
#include "UnitMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormHydroMeterCell *FormHydroMeterCell = NULL;

//---------------------------------------------------------------------------
__fastcall TFormHydroMeterCell::TFormHydroMeterCell(TComponent* Owner)
	: TForm(Owner)
{
    query->Connection = FormMain->SQLConnection;
	iQueryKind = 0;	// Default : 10 Minute
	DateMinute = Now();
	DateDay = Now(); //현재일
	DateWeek = Now(); //현재일
	DateMonth = Now(); //현재일
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::FormShow(TObject *Sender)
{
	if ( sMeasureDate.IsEmpty() && sMeasureTime.IsEmpty() ) {
		DateHome = Now();
	} else {
		if ( sMeasureDate.Length() == 8 && sMeasureTime.Length() == 6 ) {
			try {
				UINT year, month, day, hour, minute, sec, msec;
				year   = StrToInt(sMeasureDate.SubString(1,4)); //2014 09 01
				month  = StrToInt(sMeasureDate.SubString(5,2));
				day    = StrToInt(sMeasureDate.SubString(7,2));
				hour   = StrToInt(sMeasureTime.SubString(1,2)); // 093000
				minute = StrToInt(sMeasureTime.SubString(3,2));
				sec    = StrToInt(sMeasureTime.SubString(5,2));
				msec   = 0;
				DateHome = TDateTime(year, month, day, hour, minute, sec, msec);
			} catch ( Exception &e ) {
				DateHome = Now();
			}
		} else {
			DateHome = Now();
		}
	}

	DateMinute = DateHome;
	DateDay    = DateHome; //현재일
	DateWeek   = DateHome; //현재일
	DateMonth  = DateHome; //현재일
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormHydroMeterCell = NULL;
    Action = caFree;
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::sbtnMinuteClick(TObject *Sender)
{
	iQueryKind = 0;
	DateDay = DateHome;
	ShowPeriod();
	DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::sbtnDayClick(TObject *Sender)
//일
{
	iQueryKind = 1;
	DateDay = DateHome;
	ShowPeriod();
	DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::sbtnWeekClick(TObject *Sender)
//주
{
	iQueryKind = 7;
	DateWeek = DateHome;
    ShowPeriod();
    DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::sbtnMonthClick(TObject *Sender)
//월
{
	iQueryKind = 30;
	DateMonth = DateHome;
    ShowPeriod();
    DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::btnPriorClick(TObject *Sender)
//이전
{
	switch(iQueryKind)
	{
		case 0 :
			DateMinute = DateMinute - EncodeTime(0, 10, 0, 0);
			break;
        case 1:
            DateDay = DateDay - iQueryKind;
            break;
        case 7:
            DateWeek = DateWeek - iQueryKind;
            break;
        case 30:
            DateMonth = DateMonth - iQueryKind;
            break;
    } // switch
	ShowPeriod();
    DoQuery();
}

//---------------------------------------------------------------------------
void __fastcall TFormHydroMeterCell::Button5Click(TObject *Sender)
//다음
{
	switch(iQueryKind)
	{
		case 0 :
			DateMinute = DateMinute + EncodeTime(0, 10, 0, 0);
			break;
        case 1:
            DateDay = DateDay + iQueryKind;
            break;
        case 7:
            DateWeek = DateWeek + iQueryKind;
			break;
        case 30:
            DateMonth = DateMonth + iQueryKind;
            break;
    } // switch
    ShowPeriod();
    DoQuery();
}

//---------------------------------------------------------------------------
void TFormHydroMeterCell::ShowPeriod()
{
	TDateTime date;
	switch ( iQueryKind ) {
		case 0  : date = DateMinute; break;
		case 1  : date = DateDay;    break;
		case 7  : date = DateWeek;   break;
		case 30 : date = DateMonth;  break;
		default : date = DateHome;   break;
	}

	sMeasureDate = date.FormatString("yyyymmdd");
	sMeasureTime = date.FormatString("hhnnss");

	lblDisplay->Caption = sMeasureDate.SubString(1, 4) + "-" +
						  sMeasureDate.SubString(5, 2) + "-" +
						  sMeasureDate.SubString(7, 2) + " " +
						  sMeasureTime.SubString(1, 2) + ":" +
						  sMeasureTime.SubString(3, 2) + ":" +
						  "00";
}

//---------------------------------------------------------------------------
void TFormHydroMeterCell::DoQuery()
//조회
{
	if ( sHydroKind == CsAccusonicA || sHydroKind == CsAccusonicB ) {	// Accusonic Data Chart
		DoQueryAccusonic();
		return;
	}

	UnicodeString sTableName = ((enHydroNo == enHydroFirst)  ? "RHydroMeter1Cell" :
							   ((enHydroNo == enHydroSecond) ? "RHydroMeter2Cell" : "RHydroMeter3Cell"));
	//Heaven2 2014.06.16
	Chart->Title->Text->Clear();
	Chart->Title->Text->Add(((enHydroNo == enHydroFirst) ? "1번유속계 셀별 유속/신호세기" :
							((enHydroNo == enHydroSecond) ? "2번유속계 셀별 유속/신호세기" : "3번유속계 셀별 유속/신호세기")));

    //시리즈 초기화
	Series1->Clear();
    Series2->Clear();
	Series3->Clear();

    //RightAxis 그래프값 설정(신호강도)
	query->Close();
	query->DataSet->CommandText = " SELECT ";
	if (sHydroKind == CsChannelMaster)
	{
		query->DataSet->CommandText = query->DataSet->CommandText +
								  " Min(Value01) AS VelMin, Max(Value01) AS VelMax, Min(Value03) AS Strength1Min, Min(Value04) AS Strength2Min, Max(Value03) AS Strength1Max, Max(Value04) AS Strength2Max, Count(*) AS CellCnt ";
								  //" MIN(VALUE01) AS VELMIN, MAX(VALUE01) AS VELMAX, MIN(VALUE03) AS STRENGTH1MIN, MIN(VALUE04) AS STRENGTH2MIN, MAX(VALUE03) AS STRENGTH1MAX, MAX(VALUE04) AS STRENGTH2MAX, COUNT(*) AS CELLCNT ";
	} // if
	else if (sHydroKind == CsSLSeries)
	{
		query->DataSet->CommandText = query->DataSet->CommandText +
								  " Min(Value01) AS VelMin, Max(Value01) AS VelMax, Min(Value05) AS Strength1Min, Min(Value06) AS Strength2Min, Max(Value05) AS Strength1Max, Max(Value06) AS Strength2Max, Count(*) AS CellCnt ";
								  //" MIN(VALUE01) AS VELMIN, MAX(VALUE01) AS VELMAX, MIN(VALUE05) AS STRENGTH1MIN, MIN(VALUE06) AS STRENGTH2MIN, MAX(VALUE05) AS STRENGTH1MAX, MAX(VALUE06) AS STRENGTH2MAX, COUNT(*) AS CELLCNT ";
	} // if
	else if (sHydroKind == CsAGSeries)
	{
		query->DataSet->CommandText = query->DataSet->CommandText +
								  " Min(Value01) AS VelMin, Max(Value01) AS VelMax, Min(Value05) AS Strength1Min, Min(Value06) AS Strength2Min, Max(Value05) AS Strength1Max, Max(Value06) AS Strength2Max, Count(*) AS CellCnt ";
								  //" MIN(VALUE01) AS VELMIN, MAX(VALUE01) AS VELMAX, MIN(VALUE05) AS STRENGTH1MIN, MIN(VALUE06) AS STRENGTH2MIN, MAX(VALUE05) AS STRENGTH1MAX, MAX(VALUE06) AS STRENGTH2MAX, COUNT(*) AS CELLCNT ";
	} else {	// default
		query->DataSet->CommandText = query->DataSet->CommandText +
								  " Min(Value01) AS VelMin, Max(Value01) AS VelMax, Min(Value05) AS Strength1Min, Min(Value06) AS Strength2Min, Max(Value05) AS Strength1Max, Max(Value06) AS Strength2Max, Count(*) AS CellCnt ";
								  //" MIN(VALUE01) AS VELMIN, MAX(VALUE01) AS VELMAX, MIN(VALUE05) AS STRENGTH1MIN, MIN(VALUE06) AS STRENGTH2MIN, MAX(VALUE05) AS STRENGTH1MAX, MAX(VALUE06) AS STRENGTH2MAX, COUNT(*) AS CELLCNT ";
	}
	query->DataSet->CommandText = query->DataSet->CommandText +
								  " FROM " + sTableName + //하층/상층
								  " WHERE MeasureDate = '" + sMeasureDate + "' AND  " +
								  "       MeasureTime = '" + sMeasureTime + "'      ";
								  //"       Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
	query->Open();

	float fVelMin, fVelMax;
	float fMin, fMax;
	int   iCellCnt = 0;
	fMin = fMax = 0;

	fMin = (query->FieldByName("Strength1Min")->AsFloat < query->FieldByName("Strength2Min")->AsFloat) ? query->FieldByName("Strength1Min")->AsFloat : query->FieldByName("Strength2Min")->AsFloat;
	fMax = (query->FieldByName("Strength1Max")->AsFloat < query->FieldByName("Strength2Max")->AsFloat) ? query->FieldByName("Strength2Max")->AsFloat : query->FieldByName("Strength1Max")->AsFloat;
	iCellCnt = query->FieldByName("CellCnt")->AsInteger;

	//RightAxis 그래프값 설정
	Chart->RightAxis->Minimum = -50000.0f;
	Chart->RightAxis->Maximum =  50000.0f;
	if ( fMin > 0.0f ) fMin = 0.0f;
	if ( fMax < 0.0f ) fMax = 0.0f;
	fMin = (fMin >= 0 ) ? fMin * 0.9 : fMin * 1.1;
	fMax = (fMax >= 0 ) ? fMax * 1.1 : fMax * 0.9;
	if ( fMin == 0.0 ) fMin = fMin - 10.0;
	if ( fMax == 0.0 ) fMax = fMax + 10.0;
	Chart->RightAxis->Minimum   = 0;
	Chart->RightAxis->Maximum   = 255;
	//Chart->RightAxis->Automatic = false;
	//Chart->RightAxis->Increment = 1;

	//LeftAxis 그래프값 설정
	Chart->LeftAxis->Minimum   = -50000.0f;
	Chart->LeftAxis->Maximum   =  50000.0f;

	fMin = query->FieldByName("VelMin")->AsFloat;
	fMax = query->FieldByName("VelMax")->AsFloat;

	if ( fMin < FormMain->DBEnv.fHydroLowerBound || fMin > FormMain->DBEnv.fHydroUpperBound ||
		 fMax < FormMain->DBEnv.fHydroLowerBound || fMax > FormMain->DBEnv.fHydroUpperBound ) {
		// Cell 유속이 유효값을 넘어설 때 . 유효범위 안에서의 최대/최소값을 다시 구한다.
		query->Close();
		query->DataSet->CommandText = " SELECT Min(Value01) AS MinVal, Max(Value01) AS MaxVal "
									  " FROM " + sTableName + //하층/상층
									  " WHERE MeasureDate = '" + sMeasureDate + "' AND  " +
									  "       MeasureTime = '" + sMeasureTime + "' AND  " +
									  "       Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2);
		query->Open();
		fMin = query->FieldByName("MinVal")->AsFloat;
		fMax = query->FieldByName("MaxVal")->AsFloat;
	}

	//if ( fMin < FormMain->DBEnv.fHydroLowerBound || fMin > FormMain->DBEnv.fHydroUpperBound ) fMin = FormMain->DBEnv.fHydroLowerBound;
	//if ( fMax < FormMain->DBEnv.fHydroLowerBound || fMax > FormMain->DBEnv.fHydroUpperBound ) fMax = FormMain->DBEnv.fHydroUpperBound;
	fMin = (fMin >= 0 ) ? fMin * 0.9 : fMin * 1.1;
	fMax = (fMax >= 0 ) ? fMax * 1.1 : fMax * 0.9;
	if ( fMin == 0.0 ) fMin    = fMin - 10.0;
	if ( fMax == 0.0 ) fMax    = fMax + 10.0;
	Chart->LeftAxis->Minimum   = fMin;
	Chart->LeftAxis->Maximum   = fMax;
	Chart->LeftAxis->Automatic = false;
	Chart->LeftAxis->Increment = ((fMax - fMin) < 30) ? 1 : 10;

	//BottomAxis 그래프값 설정
	Chart->BottomAxis->Automatic = False;
	//int iCellCnt = query->FieldByName("CELLCNT")->AsInteger; //(bHM1) ? FormMain->DBEnv.iAG1_WN : FormMain->DBEnv.iAG2_WN; //상층
	Chart->BottomAxis->Minimum = 0;
	Chart->BottomAxis->Maximum = (iCellCnt > 1 ) ? iCellCnt - 1 : 0;
	Chart->BottomAxis->Increment = 1;

// same increment
double leftRange = 0.0f;
double rightRange = 0.0f;
double leftIncrement = 0.0f;
double rightIncrement = 0.0f;

leftRange      = Chart->Axes->Left->Maximum - Chart->Axes->Left->Minimum;
rightRange     = Chart->Axes->Right->Maximum - Chart->Axes->Right->Minimum;
leftIncrement  = Chart->Axes->Left->Increment;
rightIncrement = (leftIncrement / leftRange) * rightRange;
Chart->Axes->Right->Increment = rightIncrement;

	//셀별 쿼리 오픈
	UnicodeString sFields = "";
	if (sHydroKind == CsChannelMaster)
	{
		sFields = "Value01 AS EW, Value02 AS NS, Value03 AS Strength1, Value04 AS Strength2 ";
	} // if
	else if (sHydroKind == CsSLSeries)
	{
		sFields = "Value01 AS EW, Value02 AS NS, Value05 AS Strength1, Value06 AS Strength2 ";
	} // if
	else if (sHydroKind == CsAGSeries)
	{
		sFields = "Value01 AS EW, Value02 AS NS, Value05 AS Strength1, Value06 AS Strength2 ";
	} else {
		sFields = "Value01 AS EW, Value02 AS NS, Value05 AS Strength1, Value06 AS Strength2 ";
	} // if

	query->Close();
	query->DataSet->CommandText = "SELECT " + sFields    + " \n" +
								  "FROM   " + sTableName + " \n" +
								  "WHERE MeasureDate = '" + sMeasureDate + "' AND \n" +
								  "      MeasureTime = '" + sMeasureTime + "'     \n" +
								  //"      Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2)+
								  "ORDER BY CellNo ";
	query->Open();
	int iXVal = 0;
	float fVelocity = 0;
	float fStrength1, fStrength2;

	while(!query->Eof)
	{
		fVelocity  = query->FieldByName("EW")->AsFloat;
		fStrength1 = query->FieldByName("Strength1")->AsFloat;
		fStrength2 = query->FieldByName("Strength2")->AsFloat;

		//if ( FormMain->DBEnv.bReverse1 ) Velocity = Velocity * -1;
		if ( fVelocity < FormMain->DBEnv.fHydroLowerBound || fVelocity > FormMain->DBEnv.fHydroUpperBound )
			Series1->AddXY(iXVal, 0, "", clGray);			// Non-Normal Data
		else
			Series1->AddXY(iXVal, fVelocity, "", clRed);	// Normal Data

		Series2->AddXY(iXVal, fStrength1, "", clBlue);
		Series3->AddXY(iXVal, fStrength2, "", clFuchsia);

		iXVal++;
		query->Next();
	} // while
}

//---------------------------------------------------------------------------
void TFormHydroMeterCell::DoQueryAccusonic()
//조회
{
	UnicodeString sTableName = ((enHydroNo == enHydroFirst)  ? "RHydroMeter1" :
							   ((enHydroNo == enHydroSecond) ? "RHydroMeter2" : "RHydroMeter3"));
	//Heaven2 2014.06.16
	Chart->Title->Text->Clear();
	Chart->Title->Text->Add(((enHydroNo == enHydroFirst)  ? "1번유속계 셀별 유속/신호세기" :
							((enHydroNo == enHydroSecond) ? "2번유속계 셀별 유속/신호세기" : "3번유속계 셀별 유속/신호세기")));

	//시리즈 초기화
	Series1->Clear();  	// Velocity
	Series2->Clear();	// Gain 1
	Series3->Clear();	// Gain 2

	//RightAxis 그래프값 설정(신호강도)
	query->Close();
	query->DataSet->CommandText = " SELECT " \
								  " Min(Value06) As V1Min,  Max(Value06) AS V1Max,  " \
								  " Min(Value07) As V2Min,  Max(Value07) AS V2Max,  " \
								  " Min(Value08) As V3Min,  Max(Value08) AS V3Max,  " \
								  " Min(Value09) As V4Min,  Max(Value09) AS V4Max,  " \
								  " Min(Value10) As V5Min,  Max(Value10) AS V5Max,  " \
								  " Min(Value11) As V6Min,  Max(Value11) AS V6Max,  " \
								  " Min(Value12) As V7Min,  Max(Value12) AS V7Max,  " \
								  " Min(Value13) As V8Min,  Max(Value13) AS V8Max,  " \
								  " Min(Value14) As Gn1Min, Max(Value14) AS Gn1Max, " \
								  " Min(Value15) As Gn2Min, Max(Value15) AS Gn2Max, " \
								  " Min(Value16) As Gn3Min, Max(Value16) AS Gn3Max, " \
								  " Min(Value17) As Gn4Min, Max(Value17) AS Gn4Max, " \
								  " Min(Value18) As Gn5Min, Max(Value18) AS Gn5Max, " \
								  " Min(Value19) As Gn6Min, Max(Value19) AS Gn6Max, " \
								  " Min(Value20) As Gn7Min, Max(Value20) AS Gn7Max, " \
								  " Min(Value21) As Gn8Min, Max(Value21) AS Gn8Max, " \
								  " Min(Value30) AS Sn1Min, Max(Value30) AS Sn1Max, " \
								  " Min(Value31) AS Sn2Min, Max(Value31) AS Sn2Max, " \
								  " Min(Value32) AS Sn3Min, Max(Value32) AS Sn3Max, " \
								  " Min(Value33) AS Sn4Min, Max(Value33) AS Sn4Max, " \
								  " Min(Value34) AS Sn5Min, Max(Value34) AS Sn5Max, " \
								  " Min(Value35) AS Sn6Min, Max(Value35) AS Sn6Max, " \
								  " Min(Value36) AS Sn7Min, Max(Value36) AS Sn7Max, " \
								  " Min(Value37) AS Sn8Min, Max(Value37) AS Sn8Max, " \
								  " 8 AS CellCnt "                                    \
								  " FROM " + sTableName + //하층/상층
								  " WHERE MeasureDate = '" + sMeasureDate + "' AND  " +
								  "       MeasureTime = '" + sMeasureTime + "'      ";
	query->Open();

	float fVelMin, fVelMax;
	float fMin, fMax;
	int   iCellCnt = 0;
	fMin = fMax = 0;
	float VMin[8], VMax[8], GnMin[8], GnMax[8], SnMin[8], SnMax[8];
	VMin[0] = query->FieldByName("V1Min")->AsFloat; VMax[0] = query->FieldByName("V1Max")->AsFloat;
	VMin[1] = query->FieldByName("V2Min")->AsFloat; VMax[1] = query->FieldByName("V2Max")->AsFloat;
	VMin[2] = query->FieldByName("V3Min")->AsFloat; VMax[2] = query->FieldByName("V3Max")->AsFloat;
	VMin[3] = query->FieldByName("V4Min")->AsFloat; VMax[3] = query->FieldByName("V4Max")->AsFloat;
	VMin[4] = query->FieldByName("V5Min")->AsFloat; VMax[4] = query->FieldByName("V5Max")->AsFloat;
	VMin[5] = query->FieldByName("V6Min")->AsFloat; VMax[5] = query->FieldByName("V6Max")->AsFloat;
	VMin[6] = query->FieldByName("V7Min")->AsFloat; VMax[6] = query->FieldByName("V7Max")->AsFloat;
	VMin[7] = query->FieldByName("V8Min")->AsFloat; VMax[7] = query->FieldByName("V8Max")->AsFloat;

	GnMin[0] = query->FieldByName("Gn1Min")->AsFloat; GnMax[0] = query->FieldByName("Gn1Max")->AsFloat;
	GnMin[1] = query->FieldByName("Gn2Min")->AsFloat; GnMax[1] = query->FieldByName("Gn2Max")->AsFloat;
	GnMin[2] = query->FieldByName("Gn3Min")->AsFloat; GnMax[2] = query->FieldByName("Gn3Max")->AsFloat;
	GnMin[3] = query->FieldByName("Gn4Min")->AsFloat; GnMax[3] = query->FieldByName("Gn4Max")->AsFloat;
	GnMin[4] = query->FieldByName("Gn5Min")->AsFloat; GnMax[4] = query->FieldByName("Gn5Max")->AsFloat;
	GnMin[5] = query->FieldByName("Gn6Min")->AsFloat; GnMax[5] = query->FieldByName("Gn6Max")->AsFloat;
	GnMin[6] = query->FieldByName("Gn7Min")->AsFloat; GnMax[6] = query->FieldByName("Gn7Max")->AsFloat;
	GnMin[7] = query->FieldByName("Gn8Min")->AsFloat; GnMax[7] = query->FieldByName("Gn8Max")->AsFloat;

	SnMin[0] = query->FieldByName("Sn1Min")->AsFloat; SnMax[0] = query->FieldByName("Sn1Max")->AsFloat;
	SnMin[1] = query->FieldByName("Sn2Min")->AsFloat; SnMax[1] = query->FieldByName("Sn2Max")->AsFloat;
	SnMin[2] = query->FieldByName("Sn3Min")->AsFloat; SnMax[2] = query->FieldByName("Sn3Max")->AsFloat;
	SnMin[3] = query->FieldByName("Sn4Min")->AsFloat; SnMax[3] = query->FieldByName("Sn4Max")->AsFloat;
	SnMin[4] = query->FieldByName("Sn5Min")->AsFloat; SnMax[4] = query->FieldByName("Sn5Max")->AsFloat;
	SnMin[5] = query->FieldByName("Sn6Min")->AsFloat; SnMax[5] = query->FieldByName("Sn6Max")->AsFloat;
	SnMin[6] = query->FieldByName("Sn7Min")->AsFloat; SnMax[6] = query->FieldByName("Sn7Max")->AsFloat;
	SnMin[7] = query->FieldByName("Sn8Min")->AsFloat; SnMax[7] = query->FieldByName("Sn8Max")->AsFloat;

	fVelMin = VMin[0];
	fVelMax = VMax[0];
	fMin = GnMin[0];
	fMax = GnMax[0];
	for ( int i = 0 ; i < 8 ; i++ ) {	// Velocity, Gain 의 최대 최소값을 찾는다.
		if ( fVelMin > VMin[i] ) fVelMin = VMin[i];
		if ( fVelMax < VMax[i] ) fVelMax = VMax[i];
		if ( fMin > GnMin[i] ) fMin = GnMin[i];    // Gn,Sn에서 최대/최소값을 찾는다.
		if ( fMax < GnMax[i] ) fMax = GnMax[i];
		if ( fMin > SnMin[i] ) fMin = SnMin[i];
		if ( fMax < SnMax[i] ) fMax = SnMax[i];
	}
	iCellCnt = 8;

	//RightAxis 그래프값 설정
	Chart->RightAxis->Minimum = -50000.0f;
	Chart->RightAxis->Maximum =  50000.0f;
	if ( fMin > 0.0f ) fMin = 0.0f;
	if ( fMax < 0.0f ) fMax = 0.0f;
	fMin = (fMin >= 0 ) ? fMin * 0.9 : fMin * 1.1;
	fMax = (fMax >= 0 ) ? fMax * 1.1 : fMax * 0.9;
	if ( fMin == 0.0 ) fMin = fMin - 10.0;
	if ( fMax == 0.0 ) fMax = fMax + 10.0;
	Chart->RightAxis->Minimum   = fMin;
	Chart->RightAxis->Maximum   = fMax;
	Chart->RightAxis->Automatic = false;
	Chart->RightAxis->Increment = 1;

	//LeftAxis 그래프값 설정
	Chart->LeftAxis->Minimum   = -50000.0f;
	Chart->LeftAxis->Maximum   =  50000.0f;

	fMin = fVelMin * 1000;	// m/s => mm/s로 변환
	fMax = fVelMax * 1000; 	// m/s => mm/s로 변환

	//if ( fMin < FormMain->DBEnv.fHydroLowerBound || fMin > FormMain->DBEnv.fHydroUpperBound ) fMin = FormMain->DBEnv.fHydroLowerBound;
	//if ( fMax < FormMain->DBEnv.fHydroLowerBound || fMax > FormMain->DBEnv.fHydroUpperBound ) fMax = FormMain->DBEnv.fHydroUpperBound;
	fMin = (fMin >= 0 ) ? fMin * 0.9 : fMin * 1.1;
	fMax = (fMax >= 0 ) ? fMax * 1.1 : fMax * 0.9;
	if ( fMin == 0.0 ) fMin    = fMin - 10.0;
	if ( fMax == 0.0 ) fMax    = fMax + 10.0;
	Chart->LeftAxis->Minimum   = fMin;
	Chart->LeftAxis->Maximum   = fMax;
	Chart->LeftAxis->Automatic = false;
	Chart->LeftAxis->Increment = ((fMax - fMin) < 30) ? 1 : 10;

	//BottomAxis 그래프값 설정
	Chart->BottomAxis->Automatic = False;
	//int iCellCnt = query->FieldByName("CELLCNT")->AsInteger; //(bHM1) ? FormMain->DBEnv.iAG1_WN : FormMain->DBEnv.iAG2_WN; //상층
	Chart->BottomAxis->Minimum = 0;
	Chart->BottomAxis->Maximum = (iCellCnt > 1 ) ? iCellCnt - 1 : 0;
	Chart->BottomAxis->Increment = 1;

	//셀별 쿼리 오픈
	// Velocity, Gain, Gain Select
	UnicodeString sFields = "";
	sFields = " Value06 As V1, Value07 As V2, Value08 As V3, Value09 As V4, " \
			  " Value10 As V5, Value11 As V6, Value13 As V7, Value13 As V8, " \
			  " Value14 As Gn1, Value15 As Gn2, Value16 As Gn3, Value17 As Gn4, " \
			  " Value18 As Gn5, Value19 As Gn6, Value20 As Gn7, Value21 As Gn8, " \
			  " Value30 As Sn1, Value31 As Sn2, Value32 As Sn3, Value33 As Sn4, " \
			  " Value34 As Sn5, Value35 As Sn6, Value36 As Sn7, Value37 As Sn8  " ;

	query->Close();
	query->DataSet->CommandText = "SELECT " + sFields    + " \n" +
								  "FROM   " + sTableName + " \n" +
								  "WHERE MeasureDate = '" + sMeasureDate + "' AND \n" +
								  "      MeasureTime = '" + sMeasureTime + "'       ";
								  //"      Value01 BETWEEN " + HFloatToStr(FormMain->DBEnv.fHydroLowerBound, 2) + " AND  " + HFloatToStr(FormMain->DBEnv.fHydroUpperBound, 2)+
	query->Open();
	float fVelocity[8];
	float fGain1[8];
	float fGain2[8];

	fVelocity[0] = query->FieldByName("V1")->AsFloat;   fVelocity[1] = query->FieldByName("V2")->AsFloat;
	fVelocity[2] = query->FieldByName("V3")->AsFloat;   fVelocity[3] = query->FieldByName("V4")->AsFloat;
	fVelocity[4] = query->FieldByName("V5")->AsFloat;   fVelocity[5] = query->FieldByName("V6")->AsFloat;
	fVelocity[6] = query->FieldByName("V7")->AsFloat;   fVelocity[7] = query->FieldByName("V8")->AsFloat;

	fGain1[0] = query->FieldByName("Gn1")->AsFloat; fGain1[1] = query->FieldByName("Gn2")->AsFloat;
	fGain1[2] = query->FieldByName("Gn3")->AsFloat; fGain1[3] = query->FieldByName("Gn4")->AsFloat;
	fGain1[4] = query->FieldByName("Gn5")->AsFloat; fGain1[5] = query->FieldByName("Gn6")->AsFloat;
	fGain1[6] = query->FieldByName("Gn7")->AsFloat; fGain1[7] = query->FieldByName("Gn8")->AsFloat;

	fGain2[0] = query->FieldByName("Sn1")->AsFloat; fGain2[1] = query->FieldByName("Sn2")->AsFloat;
	fGain2[2] = query->FieldByName("Sn3")->AsFloat; fGain2[3] = query->FieldByName("Sn4")->AsFloat;
	fGain2[4] = query->FieldByName("Sn5")->AsFloat; fGain2[5] = query->FieldByName("Sn6")->AsFloat;
	fGain2[6] = query->FieldByName("Sn7")->AsFloat; fGain2[7] = query->FieldByName("Sn8")->AsFloat;

	for ( int i = 0 ; i < 8 ; i++ ) {
		if ( fVelocity[i] < FormMain->DBEnv.fHydroLowerBound || fVelocity[i] > FormMain->DBEnv.fHydroUpperBound )
			Series1->AddXY(i, 0, "", clGray);			// Non-Normal Data
		else
			Series1->AddXY(i, fVelocity[i]*1000, "", clRed);	// m/s => mm/s

		Series2->AddXY(i, fGain1[i], "", clBlue);
		Series3->AddXY(i, fGain2[i], "", clFuchsia);
	}
}

