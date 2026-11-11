//---------------------------------------------------------------------------

#pragma hdrstop

#include "RWaterLevel.h"
#include "UnitMain.h"
#include "Env.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRWaterLevel::TRWaterLevel(TSQLConnection *Connection, float fWLOffset) : TfbTable(Connection, "rwaterlevel")
{
    fOffset = fWLOffset;
}
//---------------------------------------------------------------------------
char TRWaterLevel::GetAdvText(AnsiString &sResult)
//adv text 생성
//return: 자료의 판단값('0'=정상/'1'=오측/'2'=결측)
{
    AnsiString sFormat;

    sResult = "";
    AnsiString sDeci_WL = "";
    float fWaterDepth = FloatByNameDef("Value03", 0.0f); //WaterDepth, ->    수심
    float fWaterLevel = fWaterDepth + fOffset; // WaterLevel, ->    수위(dept + offset)
    float fSalinity   = (ValueByName("WaterKind") == "AT200") ? FloatByNameDef("Value07", 0.0f) : 0.0f; //  Salinity, ->    염도
    sDeci_WL += (fWaterLevel == 0) ? "2" : (fWaterLevel < frmMain->DBEnvInfo.fWaterLevel_Min || fWaterLevel > frmMain->DBEnvInfo.fWaterLevel_Max) ? "1" : "0"; //수위
    sDeci_WL += (fSalinity   == 0) ? "2" : (fSalinity   < frmMain->DBEnvInfo.fSalinity_Min   || fSalinity   > frmMain->DBEnvInfo.fSalinity_Max  ) ? "1" : "0"; //염도
    sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; //   Deci_WL, -> 수위계 판단(수위, 염도)
    sResult += sFormat.sprintf("%10.2f", fWaterLevel) + ","; //WaterLevel, ->    수위(dept + offset)
    sResult += sFormat.sprintf("%10.3f", fWaterDepth) + ","; //WaterDepth, ->    수심
    sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset, ->    오프셋
    sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; //  Salinity, ->    염도

    return (sDeci_WL.Pos("2") > 0) ? '2' : (sDeci_WL.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}


