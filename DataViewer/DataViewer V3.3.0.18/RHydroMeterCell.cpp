//---------------------------------------------------------------------------

#pragma hdrstop

#include "RHydroMeterCell.h"
#include "UnitMain.h"
#include "Env.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRHydroMeterCell::TRHydroMeterCell(TSQLConnection *Connection, int aUnitNo, int aWN) : TfbTable(Connection, "rhydrometer" + IntToStr(aUnitNo) + "cell")
{
    iUnitNumber = aUnitNo;
    iWN = aWN;
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextCM(AnsiString &sResult)
//channel master
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : '2'; //정상 아니면 결측
    AnsiString sFormat;

    sResult = "";
    //유속계 셀
    char cLoopDecision = '0';
    SelectQr->First();
    while(!SelectQr->Eof)
    {
        //   No_Cell, ->    측정 셀 번호(1~n까지 반복)
        int iNo_Cell = ValueByName("CellNo").ToIntDef(0);
        //      V_EW, ->        셀 유속(ew, mm/s)
        int iV_EW    = ValueByName("Value01").ToIntDef(0);
        //      V_NS, ->        셀 유속(ns, mm/s)
        int iV_NS    = ValueByName("Value02").ToIntDef(0);
		//        E1, ->        1번 빔 신호강도
		int iE1 = ValueByName("Value03").ToIntDef(0);
		//        E2, ->        2번 빔 신호강도
		int iE2 = ValueByName("Value04").ToIntDef(0);
        sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
        if (cLoopDecision == '0') //유속 오측 범위 검사
        {
            cLoopDecision = (iV_EW < frmMain->DBEnvInfo.iV_EW_Min || iV_NS < frmMain->DBEnvInfo.iV_NS_Min ||
                             iV_EW > frmMain->DBEnvInfo.iV_EW_Max || iV_NS > frmMain->DBEnvInfo.iV_NS_Max) ? '1' : '0';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextSL(AnsiString &sResult)
//sl series
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : '2'; //정상 아니면 결측
    AnsiString sFormat;

    sResult = "";
    //유속계 셀
    char cLoopDecision = '0';
    SelectQr->First();
    while(!SelectQr->Eof)
    {
        //   No_Cell, ->    측정 셀 번호(1~n까지 반복)
        int iNo_Cell = ValueByName("CellNo").ToIntDef(0);
        //      V_EW, ->        셀 유속(ew, mm/s)
        int iV_EW    = ValueByName("Value01").ToIntDef(0);
        //      V_NS, ->        셀 유속(ns, mm/s)
        int iV_NS    = ValueByName("Value02").ToIntDef(0);
        //        E1, ->        1번 빔 신호강도
        int iE1 = ValueByName("Value05").ToIntDef(0);
        //        E2, ->        2번 빔 신호강도
        int iE2 = ValueByName("Value06").ToIntDef(0);
        sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
        if (cLoopDecision == '0') //유속 오측 범위 검사
        {
            cLoopDecision = (iV_EW < frmMain->DBEnvInfo.iV_EW_Min || iV_NS < frmMain->DBEnvInfo.iV_NS_Min ||
                             iV_EW > frmMain->DBEnvInfo.iV_EW_Max || iV_NS > frmMain->DBEnvInfo.iV_NS_Max) ? '1' : '0';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextAG(AnsiString &sResult)
//ag series
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : '2'; //정상 아니면 결측
    AnsiString sFormat;

    sResult = "";
    //유속계 셀
    char cLoopDecision = '0';
    SelectQr->First();
    while(!SelectQr->Eof)
    {
        //   No_Cell, ->    측정 셀 번호(1~n까지 반복)
        int iNo_Cell = ValueByName("CellNo").ToIntDef(0);
        //      V_EW, ->        셀 유속(ew, mm/s)
        int iV_EW    = ValueByName("Value01").ToIntDef(0);
        //      V_NS, ->        셀 유속(ns, mm/s)
        int iV_NS    = ValueByName("Value02").ToIntDef(0);
        //        E1, ->        1번 빔 신호강도
        int iE1 = ValueByName("Value05").ToIntDef(0);
        //        E2, ->        2번 빔 신호강도
        int iE2 = ValueByName("Value06").ToIntDef(0);
        sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
        if (cLoopDecision == '0') //오측 범위 검사
        {
            cLoopDecision = (iV_EW < frmMain->DBEnvInfo.iV_EW_Min || iV_NS < frmMain->DBEnvInfo.iV_NS_Min ||
                             iV_EW > frmMain->DBEnvInfo.iV_EW_Max || iV_NS > frmMain->DBEnvInfo.iV_NS_Max) ? '1' : '0';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::GetAdvText(AnsiString &sResult, UnicodeString sHydroKind)
{
    char cResult = 'Z';
    sResult = "";

    char (TRHydroMeterCell::*fp)(AnsiString &) = NULL;
    if (sHydroKind == CsChannelMaster) fp = &TRHydroMeterCell::_GetAdvTextCM; //채널마스터
    if (sHydroKind == CsSLSeries     ) fp = &TRHydroMeterCell::_GetAdvTextSL; //SL시리즈
    if (sHydroKind == CsAGSeries     ) fp = &TRHydroMeterCell::_GetAdvTextAG; //AG시리즈
    if (fp != NULL)
    {
        cResult = (this->*fp)(sResult); //함수포인터 실행
    } // if

    return cResult;
}

