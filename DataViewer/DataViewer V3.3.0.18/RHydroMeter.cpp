//---------------------------------------------------------------------------

#pragma hdrstop

#include "RHydroMeter.h"
#include "RHydroMeterCell.h"
#include "UnitMain.h"
#include "DvEnv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRHydroMeter::TRHydroMeter(TSQLConnection *Connection, int iUnitNo) : TfbTable(Connection, "rhydrometer" + IntToStr(iUnitNo))
{
    iUnitNumber = iUnitNo;
    sMeasureDate = "";
    sMeasureTime = "";
}
//---------------------------------------------------------------------------
UnicodeString TRHydroMeter::Open() //파라미터로 자료 오픈
{
    if (slWhereParams->IndexOfName("MEASUREDATE") >= 0) sMeasureDate = slWhereParams->Values["MEASUREDATE"];
    if (slWhereParams->IndexOfName("MEASURETIME") >= 0) sMeasureTime = slWhereParams->Values["MEASURETIME"];
    UnicodeString sResult = TfbTable::Open();
    return sResult;
}
//---------------------------------------------------------------------------
void TRHydroMeter::_GetAdvTextCM(TRHydroMeterHeader &RHydroMeterHeader)
//channel master
{
    //Temp_Water, ->    수온
	RHydroMeterHeader.fTemp_Water = FloatByNameDef("Value19", 0.0f) * 0.01;
	//Depth_ADVM, ->    수압
	RHydroMeterHeader.fDepth_ADVM = FloatByNameDef("Value26", 0.0f) * 0.0001;
    //     Pitch, ->    피치
    RHydroMeterHeader.fPitch      = FloatByNameDef("Value16", 0.0f) * 0.01;
    //      Roll, ->    롤
    RHydroMeterHeader.fRoll       = FloatByNameDef("Value17", 0.0f) * 0.01;
    //        WN, ->    측정 셀 총개수
    RHydroMeterHeader.iWN         = ValueByName("Value02").ToIntDef(0);
    //        WS, ->    측정 셀 크기(cm)
    RHydroMeterHeader.iWS         = ValueByName("Value04").ToIntDef(0);
    //        WP, ->    측정 셀 평균개수
    RHydroMeterHeader.iWP         = ValueByName("Value03").ToIntDef(0);
    //        WF, ->    공백거리셀
    RHydroMeterHeader.iWF         = ValueByName("Value05").ToIntDef(0);
    //      DIS1, ->    첫번째 셀 중심거리(cm)
    RHydroMeterHeader.iDIS1       = ValueByName("Value11").ToIntDef(0);
    //      DIS2, ->    마지막 셀 중심거리(cm)
    RHydroMeterHeader.iDIS2       = RHydroMeterHeader.iWN * RHydroMeterHeader.iWS + RHydroMeterHeader.iDIS1;
}
//---------------------------------------------------------------------------
void TRHydroMeter::_GetAdvTextSL(TRHydroMeterHeader &RHydroMeterHeader)
//sl series
{
    //Temp_Water, ->    수온
    RHydroMeterHeader.fTemp_Water = FloatByNameDef("Value13", 0.0f) * 0.01;
    //Depth_ADVM, ->    수압
    RHydroMeterHeader.fDepth_ADVM = 0.0f;
    //     Pitch, ->    피치
    RHydroMeterHeader.fPitch      = FloatByNameDef("Value11", 0.0f) * 0.1;
    //      Roll, ->    롤
    RHydroMeterHeader.fRoll       = FloatByNameDef("Value12", 0.0f) * 0.1;
    //        WN, ->    측정 셀 총개수
    RHydroMeterHeader.iWN         = ValueByName("Value05").ToIntDef(0);
    //        WS, ->    측정 셀 크기(cm)
	RHydroMeterHeader.iWS         = ValueByName("Value06").ToIntDef(0);
	//        WP, ->    측정 셀 평균개수
	RHydroMeterHeader.iWP         = 0; //고정
    //        WF, ->    공백거리셀
	RHydroMeterHeader.iWF         = ValueByName("Value07").ToIntDef(0);
    //      DIS1, ->    첫번째 셀 중심거리(cm)
    RHydroMeterHeader.iDIS1       = (int)(RHydroMeterHeader.iWF + (0.5 * RHydroMeterHeader.iWS));
    //      DIS2, ->    마지막 셀 중심거리(cm)
    RHydroMeterHeader.iDIS2       = RHydroMeterHeader.iWN * RHydroMeterHeader.iWS + RHydroMeterHeader.iDIS1;
}
//---------------------------------------------------------------------------
void TRHydroMeter::_GetAdvTextAG(TRHydroMeterHeader &RHydroMeterHeader)
//ag series
{
    //Temp_Water, ->    수온
    RHydroMeterHeader.fTemp_Water = FloatByNameDef("Value17", 0.0f) * 0.01;
    //Depth_ADVM, ->    수압
    RHydroMeterHeader.fDepth_ADVM = FloatByNameDef("Value03", 0.0f) * 0.001;
	//     Pitch, ->    피치
    RHydroMeterHeader.fPitch      = FloatByNameDef("Value12", 0.0f) * 0.1;
    //      Roll, ->    롤
    RHydroMeterHeader.fRoll       = FloatByNameDef("Value13", 0.0f) * 0.1;
    switch(iUnitNumber)
    {
        case 1: //하층유속계 ag series
            RHydroMeterHeader.iWN = frmMain->DBEnvInfo.iAG1_WN; //        WN, ->    측정 셀 총개수
            RHydroMeterHeader.iWS = frmMain->DBEnvInfo.fAG1_WS; //        WS, ->    측정 셀 크기(cm)
            RHydroMeterHeader.iWF = frmMain->DBEnvInfo.fAG1_WF; //        WF, ->    공백거리셀
            break;
        case 2: //상층유속계 ag series
            RHydroMeterHeader.iWN = frmMain->DBEnvInfo.iAG2_WN; //        WN, ->    측정 셀 총개수
            RHydroMeterHeader.iWS = frmMain->DBEnvInfo.fAG2_WS; //        WS, ->    측정 셀 크기(cm)
            RHydroMeterHeader.iWF = frmMain->DBEnvInfo.fAG2_WF; //        WF, ->    공백거리셀
            break;
    } // switch
    //        WP, ->    측정 셀 평균개수
    RHydroMeterHeader.iWP         = 0;
    //      DIS1, ->    첫번째 셀 중심거리(cm)
    RHydroMeterHeader.iDIS1       = (int)(RHydroMeterHeader.iWF + (0.5 * RHydroMeterHeader.iWS));
    //      DIS2, ->    마지막 셀 중심거리(cm)
    RHydroMeterHeader.iDIS2       = RHydroMeterHeader.iWN * RHydroMeterHeader.iWS + RHydroMeterHeader.iDIS1;
}
//---------------------------------------------------------------------------
char TRHydroMeter::GetAdvText(AnsiString &sResult)
{
    sResult = "";
    if (DataSet()->Eof && DataSet()->Bof) return '0'; //마스터 쿼리 자체가 없으므로 정상으로 리턴

    TRHydroMeterHeader RHydroMeterHeader;
    AnsiString sFormat;

    void (TRHydroMeter::*fp)(TRHydroMeterHeader &) = NULL;
    if (ValueByName("HydroKind") == CsChannelMaster) fp = &TRHydroMeter::_GetAdvTextCM; //채널마스터
    if (ValueByName("HydroKind") == CsSLSeries     ) fp = &TRHydroMeter::_GetAdvTextSL; //SL시리즈
    if (ValueByName("HydroKind") == CsAGSeries     ) fp = &TRHydroMeter::_GetAdvTextAG; //AG시리즈
    if (fp != NULL)
    {
        (this->*fp)(RHydroMeterHeader); //함수포인터 실행
    } // if

    //유속계 셀 처리
    TRHydroMeterCell *RHydroMeterCell = new TRHydroMeterCell(DataSet()->Connection, iUnitNumber, RHydroMeterHeader.iWN);
    RHydroMeterCell->ClearParams();
//todo:    RHydroMeterCell->SetWhereParam("MEASUREDATE", sMeasureDate);
//todo:    RHydroMeterCell->SetWhereParam("MEASURETIME", sMeasureTime);
    RHydroMeterCell->SetWhereParam("MEASUREDATE", "20140325");
    RHydroMeterCell->SetWhereParam("MEASURETIME", "091010");
    RHydroMeterCell->SetOrderParam("CELLNO");
    RHydroMeterCell->Open();
    AnsiString sHDCell = "";
    char cHydroCellDecision = RHydroMeterCell->GetAdvText(sHDCell, ValueByName("HydroKind"));
    delete RHydroMeterCell;

//리턴데이터
    //   No_ADVM, -> 유속계 번호(n) -> 1(하층유속계) 2(상층유속계)
    sResult += sFormat.sprintf("%10d", iUnitNumber) + ",";
    // Deci_ADVM, ->    유속계 판단(수온, 피치, 롤, 셀유속)
    UnicodeString sDeciADVM = "";
    sDeciADVM += (RHydroMeterHeader.fTemp_Water == 0) ? "2" : (RHydroMeterHeader.fTemp_Water < frmMain->DBEnvInfo.fTemp_Water_Min || RHydroMeterHeader.fTemp_Water > frmMain->DBEnvInfo.fTemp_Water_Max) ? "1" : "0"; //수온
    sDeciADVM += (RHydroMeterHeader.fPitch      == 0) ? "2" : (RHydroMeterHeader.fPitch      < frmMain->DBEnvInfo.fPitch_Min      || RHydroMeterHeader.fPitch      > frmMain->DBEnvInfo.fPitch_Max     ) ? "1" : "0"; //피치
    sDeciADVM += (RHydroMeterHeader.fRoll       == 0) ? "2" : (RHydroMeterHeader.fRoll       < frmMain->DBEnvInfo.fRoll_Min       || RHydroMeterHeader.fRoll       > frmMain->DBEnvInfo.fRoll_Max      ) ? "1" : "0"; //롤
    sDeciADVM += cHydroCellDecision; //셀유속
    sResult += sFormat.sprintf("%10S", sDeciADVM) + ",";
    //Temp_Water, ->    수온
    sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fTemp_Water) + ",";
    //Depth_ADVM, ->    수압
    sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fDepth_ADVM) + ",";
    //     Pitch, ->    피치
    sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fPitch) + ",";
    //      Roll, ->    롤
    sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fRoll) + ",";
    //        WN, ->    측정 셀 총개수
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iWN) + ",";
    //        WS, ->    측정 셀 크기(cm)
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iWS) + ",";
    //        WP, ->    측정 셀 평균개수
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iWP) + ",";
    //        WF, ->    공백거리셀
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iWF) + ",";
    //      DIS1, ->    첫번째 셀 중심거리(cm)
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iDIS1) + ",";
    //      DIS2, ->    마지막 셀 중심거리(cm)
    sResult += sFormat.sprintf("%10d", RHydroMeterHeader.iDIS2) + ",";

    sResult += sHDCell;

    return (sDeciADVM.Pos("2") > 0) ? '2' : (sDeciADVM.Pos("1") > 0) ? '1' : '0';
}


