//---------------------------------------------------------------------------
#pragma hdrstop

#include "RWaterLevel.h"
#include "RHydroMeter.h"	// 2021.12.09 heaven2
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
void TRWaterLevel::SetMeasureDateTime(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	sMeasureDate = MeasureDate;
	sMeasureTime = MeasureTime;
}

//---------------------------------------------------------------------------
TRWaterLevel::~TRWaterLevel()
{
}

//---------------------------------------------------------------------------
char TRWaterLevel::GetAdvText(AnsiString &sResult)
//adv text 생성
//return: 자료의 판단값('0'=정상/'1'=오측/'2'=결측)
{
	AnsiString sFormat;
	AnsiString sDeci_WL = "";

// 수위 데이터를 유속계(RQ-30d), 수위계에서 가져오는 냐의 판단은 상위 로직에서 처리
//	if ( frmMain->DBEnvInfo.sHydroKind1 == CsRQ30 || frmMain->DBEnvInfo.sHydroKind1 == CsRQ30D ) {
//		sResult = "";
//		//AnsiString sDeci_WL = "";
//
//		TRHydroMeter *RHydroMeter1 = new TRHydroMeter(frmMain->SQLConnection, 1);
//		RHydroMeter1->ClearParams();
//		RHydroMeter1->SetWhereParam("MEASUREDATE", sMeasureDate);
//		RHydroMeter1->SetWhereParam("MEASURETIME", sMeasureTime);
//		RHydroMeter1->Open();
//
//		double fWaterLevel = 0.0f;
//		double fWaterDepth = 0.0f;
//		double fOffset     = 0.0f;
//		double fSalinity   = 0.0f;
//
//		fWaterDepth = RHydroMeter1->FloatByNameDef("Value01", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
//		fOffset     = RHydroMeter1->FloatByNameDef("Value18", 0.0f);	// WaterLevel Offset
//		fOffset     = 0.0f;								// 2022.05.09 RQ-30은 Offset 적용하지 않음.
//														// 2022.05.09 RQ-30은 RQ-30의 자체 WaterLevel을 사용 ( WaterLevel의 Offset 정보를 사용하지 않음)
//		fSalinity   = 0.0f;                             // RQ30 => 염도 측정 없음
//		fWaterLevel = fWaterDepth + fOffset;	        // unit: meter
//		sDeci_WL += (RHydroMeter1->DataSet()->Eof && RHydroMeter1->DataSet()->Bof) ? "2" : (fWaterLevel < frmMain->DBEnvInfo.fWaterLevel_Min || fWaterLevel > frmMain->DBEnvInfo.fWaterLevel_Max) ? "1" : "0"; //수위
//		sDeci_WL += "2";	// 염도 결측
//		sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; // Deci_WL,    -> 수위계 판단(수위, 염도)
//		sResult += sFormat.sprintf("%10.2f", fWaterDepth) + ","; // WaterDepth, -> 수심
//		sResult += sFormat.sprintf("%10.3f", fWaterLevel) + ","; // WaterLevel, -> 수위(dept + offset)
//		sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset,  -> 오프셋
//		sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; // Salinity,   -> 염도
//
//		delete RHydroMeter1;
//
//	} else { 	// RQ30이 추가되기 전의 Logic

	if ( !DataSet()->Eof ) {	// 수위 데이터가 있으면
		sResult = "";
		//AnsiString sDeci_WL = "";

		float fWaterDepth = FloatByNameDef("Value03", 0.0f); //WaterDepth, ->    수심
		float fWaterLevel = fWaterDepth + fOffset; // WaterLevel, ->    수위(dept + offset)
		float fSalinity   = (ValueByName("WaterKind") == "AT200") ? FloatByNameDef("Value07", 0.0f) : 0.0f; //  Salinity, ->    염도

		sDeci_WL += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fWaterLevel < frmMain->DBEnvInfo.fWaterLevel_Min || fWaterLevel > frmMain->DBEnvInfo.fWaterLevel_Max) ? "1" : "0"; //수위
		sDeci_WL += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fSalinity   < frmMain->DBEnvInfo.fSalinity_Min   || fSalinity   > frmMain->DBEnvInfo.fSalinity_Max  ) ? "1" : "0"; //염도

		sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; // Deci_WL,    -> 수위계 판단(수위, 염도)
		sResult += sFormat.sprintf("%10.2f", fWaterDepth) + ","; // WaterDepth, -> 수심
		sResult += sFormat.sprintf("%10.3f", fWaterLevel) + ","; // WaterLevel, -> 수위(dept + offset)
		sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset,  -> 오프셋
		sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; // Salinity,   -> 염도
	} else {	// 결과셋이 없다=> 수위 데이터 없음
		sResult = "";

		float fWaterDepth = 0.0f;
		float fWaterLevel = 0.0f;
		float fSalinity   = 0.0f;

		sDeci_WL += "2";	// 수위 결측
		sDeci_WL += "2";	// 염도 결측

		sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; // Deci_WL,    -> 수위계 판단(수위, 염도)
		sResult += sFormat.sprintf("%10.2f", fWaterDepth) + ","; // WaterDepth, -> 수심
		sResult += sFormat.sprintf("%10.3f", fWaterLevel) + ","; // WaterLevel, -> 수위(dept + offset)
		sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset,  -> 오프셋
		sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; // Salinity,   -> 염도
	}
	return (sDeci_WL.Pos("2") > 0) ? '2' : (sDeci_WL.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}

//---------------------------------------------------------------------------
char TRWaterLevel::GetWindText(AnsiString &sResult)	// for EWSV Wind Data
{
	AnsiString sFormat;
	AnsiString sDeci_Wind = "";

	sResult = "";

	double fDegree   = 0.0f;	// 풍향
	double fWind     = 0.0f;	// 풍속
	double fTemp     = 0.0f;	// 온도
	double fHumidity = 0.0f;	// 습도
	double fAtmos    = 0.0f;	// 대기압

	fDegree   = FloatByNameDef("Value13", 0.0f);	//
	fWind     = FloatByNameDef("Value14", 0.0f);	//
	fTemp     = FloatByNameDef("Value17", 0.0f);	//
	fHumidity = FloatByNameDef("Value18", 0.0f);	//
	fAtmos    = FloatByNameDef("Value07", 0.0f);	//
	//fAtmos    = fAtmos * 1000;	// bar => mbar(단위 변환) DB에는 bar로 저장, 전송은 mbar로 전송
	//WaterLevel에 있는 Atmos의 단위는 milibar이다 => 무변환
	//HydroMeter(Airmar대기압) => 단위: bar

	if (DataSet()->Eof && DataSet()->Bof) {
		sDeci_Wind = "22222";	// 데이터 없음 => 결측 데이터
	} else {
		sDeci_Wind += (fDegree   >=  0.0f && fDegree   <= 360.0f ) ? "0" : "1";	// 정상 / 오측 => (0 ~ 360도)
		sDeci_Wind += (fWind     >=  0.0f && fWind     <= 200.0f ) ? "0" : "1";	// 정상 / 오측 => (0.0 ~ 200.0m/sec)
		sDeci_Wind += (fTemp     >=-20.0f && fTemp     <= 100.0f ) ? "0" : "1";	// 정상 / 오측 => (-20.0 ~ 100.0'c)
		sDeci_Wind += (fHumidity >=  0.0f && fHumidity <= 100.0f ) ? "0" : "1";	// 정상 / 오측 => ( 0.0 ~ 100.0%)
		sDeci_Wind += (fAtmos    >   0.0f && fAtmos    < 3000.0f ) ? "0" : "1";	// 정상 / 오측 => ( 0.0 < mbar < 3000 )
	}
	sResult += sFormat.sprintf("%10s",   sDeci_Wind ) + ","; // Deci_Wind,  -> 풍속계 판단
	sResult += sFormat.sprintf("%10.2f", fDegree )    + ","; // Degree,     -> 풍향
	sResult += sFormat.sprintf("%10.3f", fWind )      + ","; // Wind,       -> 풍속
	sResult += sFormat.sprintf("%10.3f", fTemp )      + ","; // Temp,       -> 온도
	sResult += sFormat.sprintf("%10.2f", fHumidity )  + ","; // Humidity,   -> 습도
	sResult += sFormat.sprintf("%10d", (int)fAtmos )  + ","; // Atmos,      -> 대기압

	return (sDeci_Wind.Pos("2") > 0) ? '2' : (sDeci_Wind.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}


