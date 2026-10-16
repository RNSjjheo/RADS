//---------------------------------------------------------------------------
#pragma hdrstop

#include "RVTHLogger.h"
#include "UnitMain.h"
#include "Env.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRVTHLogger::TRVTHLogger(TSQLConnection *Connection) : TfbTable(Connection, "rvthlogger")
{
//
}
//---------------------------------------------------------------------------
char TRVTHLogger::GetAdvText(AnsiString &sResult)
//adv text 생성
//return: 자료의 판단값('0'=정상/'1'=오측/'2'=결측)
{
    AnsiString sFormat;

    sResult = "";
    AnsiString sDeci_VTH = ""; //  Deci_VTH, -> vth판단(입력전압, 충전전압, 배터리전압, 시스템온도, 시스템습도)
    int   iAC         = ValueByName("VOLT").ToIntDef(0);     //        AC, ->    입력전압
    float fDC_Charge  = FloatByNameDef("DCCHARGE", 0.0f);    // DC_Charge, ->    충전전압
    float fDC_Battery = FloatByNameDef("DCBATTERY", 0.0f);   //DC_Battery, ->    배터리전압
    float fTemp_Sys   = FloatByNameDef("TEMPERATURE", 0.0f); //  Temp_Sys, ->    시스템온도
    float fHr_Sys     = FloatByNameDef("HUMIDITY", 0.0f);    //    Hr_Sys, ->    시스템습도
    sDeci_VTH += (DataSet()->Eof && DataSet()->Bof) ? "2" : (iAC         < frmMain->DBEnvInfo.iAC_Min         || iAC         > frmMain->DBEnvInfo.iAC_Max        ) ? "1" : "0";
    sDeci_VTH += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fDC_Charge  < frmMain->DBEnvInfo.fDC_Charge_Min  || fDC_Charge  > frmMain->DBEnvInfo.fDC_Charge_Max ) ? "1" : "0";
    sDeci_VTH += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fDC_Battery < frmMain->DBEnvInfo.fDC_Battery_Min || fDC_Battery > frmMain->DBEnvInfo.fDC_Battery_Max) ? "1" : "0";
    sDeci_VTH += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fTemp_Sys   < frmMain->DBEnvInfo.fTemp_Sys_Min   || fTemp_Sys   > frmMain->DBEnvInfo.fTemp_Sys_Max  ) ? "1" : "0";
    sDeci_VTH += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fHr_Sys     < frmMain->DBEnvInfo.fHr_Sys_Min     || fHr_Sys     > frmMain->DBEnvInfo.fHr_Sys_Max    ) ? "1" : "0";
    sResult += sFormat.sprintf("%10s",   sDeci_VTH  ) + ","; //  Deci_VTH, -> vth판단(입력전압, 충전전압, 배터리전압, 시스템온도, 시스템습도)
    sResult += sFormat.sprintf("%10d",   iAC        ) + ","; //        AC, ->    입력전압
    sResult += sFormat.sprintf("%10.1f", fDC_Charge ) + ","; // DC_Charge, ->    충전전압
    sResult += sFormat.sprintf("%10.1f", fDC_Battery) + ","; //DC_Battery, ->    배터리전압
    sResult += sFormat.sprintf("%10.1f", fTemp_Sys  ) + ","; //  Temp_Sys, ->    시스템온도
    sResult += sFormat.sprintf("%10.1f", fHr_Sys    ) + ","; //    Hr_Sys, ->    시스템습도

    return (sDeci_VTH.Pos("2") > 0) ? '2' : (sDeci_VTH.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}
//---------------------------------------------------------------------------
TRVTHLogger::~TRVTHLogger()
{
}
//---------------------------------------------------------------------------



