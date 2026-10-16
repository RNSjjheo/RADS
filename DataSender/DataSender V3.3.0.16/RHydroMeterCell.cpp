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

	SumSlaveLearnDischarge = 0.0f;
}

//---------------------------------------------------------------------------
TRHydroMeterCell::TRHydroMeterCell(TSQLConnection *Connection, int aUnitNo) : TfbTable(Connection, "rhydrometer" + IntToStr(aUnitNo) + "cell")
{
	iUnitNumber = aUnitNo;
	//iWN = aWN;

	SumSlaveLearnDischarge = 0.0f;
}

//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextCM(AnsiString &sResult)
//channel master
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : (SelectQr->RecordCount == 0) ? '2' : '1'; //정상/결측/오측
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
		int iE1      = ValueByName("Value03").ToIntDef(0);
		//        E2, ->        2번 빔 신호강도
		int iE2      = ValueByName("Value04").ToIntDef(0);
		sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
		if (cLoopDecision == '0') //유속 오측 범위 검사
		{
			if (iV_EW * 0.001 < frmMain->DBEnvInfo.fV_EW_Min || iV_NS * 0.001 < frmMain->DBEnvInfo.fV_NS_Min ||
				iV_EW * 0.001 > frmMain->DBEnvInfo.fV_EW_Max || iV_NS * 0.001 > frmMain->DBEnvInfo.fV_NS_Max) cLoopDecision = '1';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextSL(AnsiString &sResult)
//sl series
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : (SelectQr->RecordCount == 0) ? '2' : '1'; //정상/결측/오측
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
		int iE1      = ValueByName("Value05").ToIntDef(0);
		//        E2, ->        2번 빔 신호강도
        int iE2      = ValueByName("Value06").ToIntDef(0);
		sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
		if (cLoopDecision == '0') //유속 오측 범위 검사
		{
			if (iV_EW * 0.001 < frmMain->DBEnvInfo.fV_EW_Min || iV_NS * 0.001 < frmMain->DBEnvInfo.fV_NS_Min ||
				iV_EW * 0.001 > frmMain->DBEnvInfo.fV_EW_Max || iV_NS * 0.001 > frmMain->DBEnvInfo.fV_NS_Max) cLoopDecision = '1';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}
//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextAG(AnsiString &sResult)
//ag series
{
    char cResult = (SelectQr->RecordCount == iWN) ? '0' : (SelectQr->RecordCount == 0) ? '2' : '1'; //정상/결측/오측
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
		int iE1      = ValueByName("Value05").ToIntDef(0);
		//        E2, ->        2번 빔 신호강도
        int iE2      = ValueByName("Value06").ToIntDef(0);
        sResult += sFormat.sprintf("%10d,%10d,%10d,%10d,%10d,", iNo_Cell, iV_EW, iV_NS, iE1, iE2);
        if (cLoopDecision == '0') //오측 범위 검사
        {
			if (iV_EW * 0.001 < frmMain->DBEnvInfo.fV_EW_Min || iV_NS * 0.001 < frmMain->DBEnvInfo.fV_NS_Min ||
				iV_EW * 0.001 > frmMain->DBEnvInfo.fV_EW_Max || iV_NS * 0.001 > frmMain->DBEnvInfo.fV_NS_Max) cLoopDecision = '1';
        } // if
        SelectQr->Next();
    } // while

    return (cResult == '2') ? '2' : (cLoopDecision == '1') ? '1' : '0';
}

//---------------------------------------------------------------------------
char TRHydroMeterCell::_GetAdvTextRQ30D(AnsiString &sResult)
// for RQ-30d
{
	SumSlaveLearnDischarge = 0.0f;	// 2023.03.15 Clear SumSlaveLearnDischarge Value

    char cResult = (SelectQr->RecordCount == iWN) ? '0' : (SelectQr->RecordCount == 0) ? '2' : '1'; //정상/결측/오측
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
//		int iV_EW    = ValueByName("Value01").ToIntDef(0);
//		//      V_NS, ->        셀 유속(ns, mm/s)
//		int iV_NS    = ValueByName("Value02").ToIntDef(0);
//		//        E1, ->        1번 빔 신호강도
//		int iE1      = ValueByName("Value05").ToIntDef(0);
//		//        E2, ->        2번 빔 신호강도
//		int iE2      = ValueByName("Value06").ToIntDef(0);
		float LearnVelocity     = FloatByNameDef("Value06", 0.0f);
		float Velocity          = FloatByNameDef("Value02", 0.0f);
		float LearnDischarge    = FloatByNameDef("Value07", 0.0f);
		float Discharge         = FloatByNameDef("Value04", 0.0f);         // Slave는 2번 Cell 부터 시작한다.
		SumSlaveLearnDischarge += LearnDischarge;	// 2023.03.15 Learn Discharge 누적한다. => TotalLQ 할 때 사용한다.

		sResult += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", iNo_Cell+1, LearnVelocity, Velocity, LearnDischarge, Discharge);
		if (cLoopDecision == '0') //오측 범위 검사
		{
			if (LearnVelocity < frmMain->DBEnvInfo.fV_EW_Min || LearnVelocity < frmMain->DBEnvInfo.fV_NS_Min ||
				LearnVelocity > frmMain->DBEnvInfo.fV_EW_Max || LearnVelocity > frmMain->DBEnvInfo.fV_NS_Max) cLoopDecision = '1';
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
	if (sHydroKind == CsRQ30D        ) fp = &TRHydroMeterCell::_GetAdvTextRQ30D; //RQ-30D
	if (fp != NULL)
	{
		cResult = (this->*fp)(sResult); //함수포인터 실행
	} // if

	return cResult;
}

//---------------------------------------------------------------------------
TRHydroMeterCell::~TRHydroMeterCell()
{
}
//---------------------------------------------------------------------------

