//---------------------------------------------------------------------------
#pragma hdrstop

#include "RHydroMeter.h"
#include "RHydroMeterCell.h"
#include "UnitMain.h"
//#include "Env.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

UnicodeString IsOkWrong(double Value)
{
	UnicodeString Decision = DECISION_OK_STR;
//	if ( Value >= frmMain->DBEnvInfo.fV_EW_Min && Value <= frmMain->DBEnvInfo.fV_EW_Max )
	if ( Value < frmMain->DBEnvInfo.fV_EW_Min || Value > frmMain->DBEnvInfo.fV_EW_Max )
		Decision = DECISION_WRONG_STR;
	else
		Decision = DECISION_OK_STR;

	return Decision;
}

//---------------------------------------------------------------------------
bool IsAllZero(TRHydroMeterUvmHeader &RHydroMeterUvmHeader)
{
	bool bAllZero = false;
	if ( RHydroMeterUvmHeader.V1 != 0 ||  RHydroMeterUvmHeader.V2 != 0 || RHydroMeterUvmHeader.V3 != 0 || RHydroMeterUvmHeader.V4 != 0 ||
		 RHydroMeterUvmHeader.V5 != 0 ||  RHydroMeterUvmHeader.V6 != 0 || RHydroMeterUvmHeader.V7 != 0 || RHydroMeterUvmHeader.V8 != 0 )
		return false;
	if ( RHydroMeterUvmHeader.Gn1 != 0 ||  RHydroMeterUvmHeader.Gn2 != 0 || RHydroMeterUvmHeader.Gn3 != 0 || RHydroMeterUvmHeader.Gn4 != 0 ||
		 RHydroMeterUvmHeader.Gn5 != 0 ||  RHydroMeterUvmHeader.Gn6 != 0 || RHydroMeterUvmHeader.Gn7 != 0 || RHydroMeterUvmHeader.Gn8 != 0  )
		return false;
	if ( RHydroMeterUvmHeader.Sn1 != 0 ||  RHydroMeterUvmHeader.Sn2 != 0 || RHydroMeterUvmHeader.Sn3 != 0 || RHydroMeterUvmHeader.Sn4 != 0 ||
		 RHydroMeterUvmHeader.Sn5 != 0 ||  RHydroMeterUvmHeader.Sn6 != 0 || RHydroMeterUvmHeader.Sn7 != 0 || RHydroMeterUvmHeader.Sn8 != 0 )
		return false;

	return true;    // 모든 값이 0 => 결측을 의미한다.
}


//---------------------------------------------------------------------------
// 2026.04.16
// W, V, LV, Q, LQ 의 LowerBound<=>UpperBound Check : ADCP 제외(오직 전자파유속계만 해당) : Bound를 초과하면 0으로 초기화
// W : unit: meter
// V, LV : unit: m/s
// Q, LQ : unit: m3/s
//---------------------------------------------------------------------------
// 수위 유효값 체크 ( -100.0 ~ 100.0 ) 단위: m
double W_BoundCheck(double W)
{
	if ( W < frmMain->DBEnvInfo.fWaterLevel_Min || W > frmMain->DBEnvInfo.fWaterLevel_Max ) {
		W = 0.0f;
	}
	return W;
}

//---------------------------------------------------------------------------
// 유속 유효값 체크( -15.0 ~ 15.0 )  단위: m/s
double V_BoundCheck(double V)
{
	if ( V < frmMain->DBEnvInfo.fV_EW_Min || V > frmMain->DBEnvInfo.fV_EW_Max ) {   	// Unit: meter
		V = 0.0f;
	}
	return V;
}

//---------------------------------------------------------------------------
// 유량 유효값 Check ( -15000 ~ 15000 ) 단위: m3/s
double Q_BoundCheck(double Q)
{
	if ( Q < frmMain->DBEnvInfo.fV_EW_Min * 1000 || Q > frmMain->DBEnvInfo.fV_EW_Max * 1000 ) {   	// Unit: meter
		Q = 0.0f;
	}
	return Q;
}



//---------------------------------------------------------------------------
//
// TEsvData Class
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TEsvData::TEsvData()
{
	pList = new TList();		// List

	DecisionEsv = "00";	    // (ex) "00", "11", "22"
	DeviceType  = 0;	    // 0:None, 1:RQ-30d,  2,3,4,..
	AverageLV   = 0.0f;				// Average of Learned Velocity
	AverageV    = 0.0f;				// Average of Raw Velocity
	TotalLQ     = 0.0f;;				// Sum of Learned Discharge
	TotalQ      = 0.0f;;				// Sum of Raw Discharge
	DeviceCount = 0;		// DeviceCount : Master(1) + Slave(4) => 5
	DeviceUseFlag = "";	// (ex)"01110"

	VFlag       = DECISION_OK;			// 유속 판단(0:정장, 1:오측, 2: 결측)
	QFlag       = DECISION_OK;			// 유량 판단(0:정장, 1:오측, 2: 결측)

	MeasureDate = "";
	MeasureTime = "";
}

//---------------------------------------------------------------------------
TEsvData::~TEsvData()
{
	// Resource Clear
	if ( pList->Count > 0 ) {
		STEsvData *pData = NULL;
		for ( int i = 0 ; i < pList->Count ; i++ ) {
			pData = (STEsvData *)pList->Items[i];
			delete pData;
		}
		pList->Clear();
	}
	delete pList;
}

//---------------------------------------------------------------------------
bool TEsvData::GetEsvData(UnicodeString MeasureDate, UnicodeString MeasureTime)	// Master(RHydroMeter1), Slave(RHydroMeter1Cell)
{
	bool bResult = false;

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;

	// 기초 데이터 정리 --------------------------------------------------------

	DecisionEsv  = "00";	    // 일단 정상으로 초기화 (ex) "00", "11", "22"
	DeviceType   =  (frmMain->DBEnvInfo.sHydroKind1 == CsNone ) ? 0 :
				   ((frmMain->DBEnvInfo.sHydroKind1 == CsRQ30D) ? 1 :
				   ((frmMain->DBEnvInfo.sHydroKind1 == CsRQ30 ) ? 2 :
				   ((frmMain->DBEnvInfo.sHydroKind1 == CsEWSVConverter) ? 3 : 0))); // (2025.04.08)0:None, 1:RQ-30d, 2: RQ-30,3,4,..
	AverageLV    = 0.0f; 		// Average of Learned Velocity
	AverageV     = 0.0f;		// Average of Raw Velocity
	TotalLQ      = 0.0f;		// Sum of Learned Discharge
	TotalQ       = 0.0f;		// Sum of Raw Discharge
	if ( DeviceType == 0 ) {	// None Device
		DeviceCount = 0;
	} else if ( DeviceType == 1 ) {	// RQ-30d
		DeviceCount  = frmMain->DBEnvInfo.iHydroMasterCnt + frmMain->DBEnvInfo.iHydroSlaveCnt;		// DeviceCount : Master(1) + Slave(4) => 5
	} else if ( DeviceType == 2 ) {	// RQ-30
		DeviceCount = 1;	// RQ-30 단독 사용
	} else {				// other device
		DeviceCount = 0;	// undefine
	}
	DeviceUseFlag= "";	// 장비사용구분  (ex)"01110"

	VFlag   = DECISION_OK;
	QFlag   = DECISION_OK;

	// Master Device Data
	TRHydroMeter *RHydroMeter1 = new TRHydroMeter(frmMain->SQLConnection, 1);
	RHydroMeter1->ClearParams();
	RHydroMeter1->SetWhereParam("MEASUREDATE", MeasureDate);
	RHydroMeter1->SetWhereParam("MEASURETIME", MeasureTime);
	RHydroMeter1->Open();

	if ( !RHydroMeter1->DataSet()->Eof ) {
		STEsvData *pData = new STEsvData();

		pData->Address   = 1;
		pData->LV        = RHydroMeter1->FloatByNameDef("Value06", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->V         = RHydroMeter1->FloatByNameDef("Value02", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->LQ        = RHydroMeter1->FloatByNameDef("Value07", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->Q         = RHydroMeter1->FloatByNameDef("Value04", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->Degree    = RHydroMeter1->FloatByNameDef("Value27", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->SNR       = RHydroMeter1->FloatByNameDef("Value03", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		pData->SelfCheck = RHydroMeter1->FloatByNameDef("Value26", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field

		if ( DeviceType == 1 ) { 	// RQ-30d
			TotalLQ          = RHydroMeter1->FloatByNameDef("Value19", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
			TotalQ           = RHydroMeter1->FloatByNameDef("Value05", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		} else if ( DeviceType == 2 ) {	// RQ-30
			TotalLQ          = pData->LQ;
			TotalQ           = pData->Q;
		} else {
			TotalLQ          = 0.0f;
			TotalQ           = 0.0f;
		}
		// 정상, 오측, 결측 판단
//		if ( pData->LV == 0.0f && pData->V == 0.0f && pData->LQ == 0.0f && pData->Q == 0.0f ) {
//			VFlag   = DECISION_NODATA;
//			QFlag   = DECISION_NODATA;
//		}
		pList->Add(pData);
	} else { // Master Device Data Not found
		TotalLQ = 0.0f;
		TotalQ  = 0.0f;
		VFlag   = DECISION_NODATA;
		QFlag   = DECISION_NODATA;

		STEsvData *pData = new STEsvData();
		memset(pData, 0x00, sizeof(STEsvData));	// Data Clear

		pData->Address = 1;
		pList->Add(pData);	// Master Data Add
	}
	delete RHydroMeter1;

	// Slave
	if ( DeviceType == 2 ) {	// RQ-30 => no exist in Detail Table
		// nothing to do..
	} else {
		TRHydroMeterCell *RHydroMeterCell = new TRHydroMeterCell(frmMain->SQLConnection, 1);	// RHydroMeterCell
		RHydroMeterCell->ClearParams();
		RHydroMeterCell->SetWhereParam("MEASUREDATE", MeasureDate);
		RHydroMeterCell->SetWhereParam("MEASURETIME", MeasureTime);
		RHydroMeterCell->SetOrderParam("CELLNO");
		RHydroMeterCell->Open();

		if ( !RHydroMeterCell->DataSet()->Eof ) {
			if ( DeviceType == 3 ) DeviceCount = RHydroMeterCell->DataSet()->RecordCount;
			STEsvData *pData = NULL;
			while ( !RHydroMeterCell->DataSet()->Eof ) {
				pData = new STEsvData();

				pData->Address   = RHydroMeterCell->IntByNameDef("CellNo", 0);
				pData->LV        = RHydroMeterCell->FloatByNameDef("Value06", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->V         = RHydroMeterCell->FloatByNameDef("Value02", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->LQ        = RHydroMeterCell->FloatByNameDef("Value07", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->Q         = RHydroMeterCell->FloatByNameDef("Value04", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->Degree    = RHydroMeterCell->FloatByNameDef("Value27", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->SNR       = RHydroMeterCell->FloatByNameDef("Value03", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
				pData->SelfCheck = RHydroMeterCell->FloatByNameDef("Value26", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field

				// 정상, 오측, 결측 판단
	//			if ( pData->LV == 0.0f && pData->V == 0.0f && pData->LQ == 0.0f && pData->Q == 0.0f ) {
	//				VFlag   = DECISION_NODATA;
	//				QFlag   = DECISION_NODATA;
	//			}

				pList->Add(pData);
				RHydroMeterCell->DataSet()->Next();
			} // while
		} else {	// 데이터가 존재하지 않기 때문에 0으로 초기화 된 데이터를 채워 넣는다.
//			VFlag   = DECISION_NODATA;  	// 2024.04.24
//			QFlag   = DECISION_NODATA;  	// 2024.04.24

			int Address = 0;
			STEsvData *pData = NULL;
			for ( int i = 0 ; i < DeviceCount - 1 ; i++ ) {		// DeviceCount: Master + SlaveCount
				Address = GetAddressByIndex(i);

				pData = new STEsvData();
				pData->Address = Address;

				pList->Add(pData);	// 없는 데이터를 채운다.
			}
		}
		delete RHydroMeterCell;
	}

	// Average Velocity 계산 ------------------------------------
	if ( pList->Count > 0 ) {
		double lv = 0.0f;
		double v  = 0.0f;
		int    lvcount = 0;
		int    vcount = 0;
		STEsvData *pData = NULL;
		for ( int i = 0 ; i < pList->Count ; i++ ) {
			pData = (STEsvData *)pList->Items[i];

			if ( pData->LV != 0.0f ) {
				lv += pData->LV;
				lvcount++;
			}
			if ( pData->V != 0.0f ) {
				v += pData->V;
				vcount++;
			}
		}
		AverageLV = (lvcount > 0 ) ? lv / lvcount : 0.0f;
		AverageV  = (vcount  > 0 ) ? v / vcount   : 0.0f;
	} else {
		AverageLV = 0.0f;
		AverageV  = 0.0f;
	}

	// 장비 사용 구분 Check => "01110"의 데이터 생성 -----------------------
	int Addr = 0;
	DeviceUseFlag = "";
	STEsvData *pData = NULL;
	for ( int i = 0 ; i < DeviceCount ; i++ ) {
		if ( DeviceType == 3 ) {
			pData = GetEsvDataByIndex(i + 1);
		} else {
			Addr = GetAddressByIndex(i);	//
			pData = GetEsvDataByAddress(Addr);
		}
		if ( pData == NULL ) {
			UseFlag[i] = false;
			DeviceUseFlag += "0";
		} else {
			if ( pData->LV != 0.0f || pData->V != 0.0f ) {
				UseFlag[i] = true;
				DeviceUseFlag += "1";	//  사용됨
			} else {
				UseFlag[i] = false;
				DeviceUseFlag += "0";	//  사용되지 않음 .
			}
		}
	}

	// 최종 정상, 오측, 결측 구분
	DecisionEsv = "";
	DecisionEsv += (VFlag == DECISION_OK ) ? "0" : ((VFlag == DECISION_WRONG) ? "1" : "2");
	DecisionEsv += (QFlag == DECISION_OK ) ? "0" : ((QFlag == DECISION_WRONG) ? "1" : "2");

	DecEsv =  (DecisionEsv.AnsiPos("2") > 0) ? '2' :  ((DecisionEsv.AnsiPos("1") > 0) ? '1' : '0');

	return true;
}

//---------------------------------------------------------------------------
AnsiString TEsvData::GetEsvHeaderStr()
{
	AnsiString sEsvHeaderStr = "";
	AnsiString sFormat = "";

	sEsvHeaderStr = sFormat.sprintf("%10s,%10d,%10.3f,%10.3f,%10.2f,%10.2f,%10d,%10s,",
							 DecisionEsv,
							 DeviceType,
							 V_BoundCheck(AverageLV),
							 V_BoundCheck(AverageV),
							 Q_BoundCheck(TotalLQ),
							 Q_BoundCheck(TotalQ),
							 DeviceCount,
							 DeviceUseFlag);
	return sEsvHeaderStr;
}

//---------------------------------------------------------------------------
AnsiString TEsvData::GetEsvDeviceStr()
{
	AnsiString sEsvDeviceStr = "";
	AnsiString sFormat = "";

	STEsvData *pData = NULL;
	int Address = 0;
	for ( int i = 0 ; i < DeviceCount ; i++ ) {
		if ( DeviceType == 3 ) {
			pData = GetEsvDataByIndex(i + 1);
		} else {
			Address = GetAddressByIndex(i);	// 좌안부터 Address 순서
			if ( Address == 0 ) continue;
			pData = GetEsvDataByAddress(Address);   // 데이터를 가져온다.
		}
		if ( pData == NULL ) continue;

		sEsvDeviceStr += sFormat.sprintf("%10d,%10.3f,%10.3f,%10.2f,%10.2f,%10.2f,%10d,%10d,",
							//pData->Address,
							(DeviceType == 3) ? pData->Address : i+1,	// 좌안부터 실제 Address가 아닌 1,2,3,... 형태로 표출
							V_BoundCheck(pData->LV),
							V_BoundCheck(pData->V),
							Q_BoundCheck(pData->LQ),
							Q_BoundCheck(pData->Q),
							pData->Degree,
							pData->SNR,
							pData->SelfCheck);
	} // for
	return sEsvDeviceStr;
}

//---------------------------------------------------------------------------
AnsiString TEsvData::GetDecision()	// 유속계 판단(ex) "00", "11", "22"
{
	return DecisionEsv;	// "00", "11", "22"
}

//---------------------------------------------------------------------------
char TEsvData::GetFinalDecision()	// '0','1','2' 최종 판단
{
	return DecEsv;	// 전자파 표면 유속계의 최종 판단ㅇ => EWSV Format의 앞부분에 사용됨.
}

//---------------------------------------------------------------------------
int TEsvData::GetAddressByIndex(int Index)	// 좌안부터 순서대로의 Address값을 반환한다.
{
	int Address = 0;

	if ( Index < frmMain->DBEnvInfo.iSlaveCount + 1 ) {
		Address = frmMain->DBEnvInfo.DeviceAddress[Index];
	} else {
		Address = 0;
	}

	return Address;
}

//---------------------------------------------------------------------------
int TEsvData::GetIndexByAddress(int Address)	// Address별 좌안부터의 Index값을 찾는다.
{
	int Index = -1;
	bool bFound = false;
	for ( int i = 0 ; i < frmMain->DBEnvInfo.iSlaveCount+1 ; i++ ) {
		if ( frmMain->DBEnvInfo.DeviceAddress[i] == Address ) {
			bFound = true;
			Index = i;
			break;
		}
	}

	return (bFound) ? Index : -1;
}

//---------------------------------------------------------------------------
STEsvData *TEsvData::GetEsvDataByIndex(int Index)
{
	STEsvData *pData = NULL;

	if ( pList->Count > 0 ) {
		if ( pList->Count > Index ) {
			pData = (STEsvData *) pList->Items[Index];
		} else {	// Index 값이 List의 Item보다 크다
			pData = NULL;
		}
	} else {	// List에 데이터가 없다
		pData = NULL;
	}

	return pData;
}

//---------------------------------------------------------------------------
STEsvData *TEsvData::GetEsvDataByAddress(int Address)
{
	STEsvData *pData = NULL;
	bool bFound = false;

	if ( pList->Count > 0 ) {
		for ( int i = 0 ; i < pList->Count ; i++ ) {
			pData = (STEsvData *) pList->Items[i];
			if ( pData->Address == Address ) {
				bFound = true;
				break;
			}
		}
	} else {	// List에 데이터가 없다
		pData = NULL;
	}
	return pData;
}

//---------------------------------------------------------------------------
//
// TRHydroMeter Class
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TRHydroMeter::TRHydroMeter(TSQLConnection *Connection, int iUnitNo) : TfbTable(Connection, "rhydrometer" + IntToStr(iUnitNo))
{
	iUnitNumber = iUnitNo;
	sMeasureDate = "";
	sMeasureTime = "";
	EwsvAdvmFlag = false;	// true: EWSV+ADVM, false: only ADVM(default)
}
//---------------------------------------------------------------------------
UnicodeString TRHydroMeter::Open() //파라미터로 자료 오픈
{
	if (slWhereParams->IndexOfName("MEASUREDATE ") >= 0) sMeasureDate = slWhereParams->Values["MEASUREDATE "];
	if (slWhereParams->IndexOfName("MEASURETIME ") >= 0) sMeasureTime = slWhereParams->Values["MEASURETIME "];
	UnicodeString sResult = TfbTable::Open();
	sMeasureDate = StringReplace(sMeasureDate, "\'", "", TReplaceFlags() << rfReplaceAll);
	sMeasureDate = StringReplace(sMeasureDate, " ", "", TReplaceFlags() << rfReplaceAll);
	sMeasureTime = StringReplace(sMeasureTime, "\'", "", TReplaceFlags() << rfReplaceAll);
	sMeasureTime = StringReplace(sMeasureTime, " ", "", TReplaceFlags() << rfReplaceAll);
	return sResult;
}
//---------------------------------------------------------------------------
void TRHydroMeter::_GetAdvTextCM(TRHydroMeterHeader &RHydroMeterHeader)
//channel master
{
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=CM");
#endif
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
#ifdef _DEBUG
    frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=SL");
#endif
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
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=AG");
#endif
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
			RHydroMeterHeader.iWN = frmMain->DBEnvInfo.iAG1_WN;       //        WN, ->    측정 셀 총개수
			RHydroMeterHeader.iWS = frmMain->DBEnvInfo.fAG1_WS * 100; //        WS, ->    측정 셀 크기(cm)
			RHydroMeterHeader.iWF = frmMain->DBEnvInfo.fAG1_WF * 100; //        WF, ->    공백거리셀
			break;
		case 2: //상층유속계 ag series
			RHydroMeterHeader.iWN = frmMain->DBEnvInfo.iAG2_WN;       //        WN, ->    측정 셀 총개수
			RHydroMeterHeader.iWS = frmMain->DBEnvInfo.fAG2_WS * 100; //        WS, ->    측정 셀 크기(cm)
			RHydroMeterHeader.iWF = frmMain->DBEnvInfo.fAG2_WF * 100; //        WF, ->    공백거리셀
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
void TRHydroMeter::_GetAdvTextRQ(TRHydroMeterHeader &RHydroMeterHeader)
//ag series
{
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=RQ");
#endif
	//Temp_Water, ->    수온
	RHydroMeterHeader.fTemp_Water = 0.0f;
	//Depth_ADVM, ->    수압
	RHydroMeterHeader.fDepth_ADVM = 0.0f;
	//     Pitch, ->    피치
	RHydroMeterHeader.fPitch      = 0.0f;
	//      Roll, ->    롤
	RHydroMeterHeader.fRoll       = 0.0f;
	RHydroMeterHeader.iWN         = 2;       // 측정 셀 총개수 RQ-30 데이터를 4개의 Cell에 실어 보낸다.
											 // 2022.12.15 RQ-30은 Cell#1:LV-V-LD-D, Cell#2:LD 전송
	RHydroMeterHeader.iWS         = 0; //        WS, ->    측정 셀 크기(cm)
	RHydroMeterHeader.iWF         = 0; //        WF, ->    공백거리셀
	RHydroMeterHeader.iWP         = 0;
	//      DIS1, ->    첫번째 셀 중심거리(cm)
	RHydroMeterHeader.iDIS1       = 0;
	//      DIS2, ->    마지막 셀 중심거리(cm)
	RHydroMeterHeader.iDIS2       = 0;
}

//---------------------------------------------------------------------------
void TRHydroMeter::_GetAdvTextRQ30D(TRHydroMeterHeader &RHydroMeterHeader)
//ag series
{
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=RQ");
#endif
	//Temp_Water, ->    수온
	RHydroMeterHeader.fTemp_Water = 0.0f;
	//Depth_ADVM, ->    수압
	RHydroMeterHeader.fDepth_ADVM = 0.0f;
	//     Pitch, ->    피치
	RHydroMeterHeader.fPitch      = 0.0f;
	//      Roll, ->    롤
	RHydroMeterHeader.fRoll       = 0.0f;
	RHydroMeterHeader.iWN         = frmMain->DBEnvInfo.iSlaveCount + 2; 	// Master + SlaveCount + 1(Total Q)
											 // 측정 셀 총개수 RQ-30 데이터를 4개의 Cell에 실어 보낸다.
											 // RQ-30d: Slave #N : 총 Cell : Master + Slave #N + 1(Total Q)로 보낸다
											 // Master + Slave #2 => 총 4 Cell 전송( Master(1) + Slave(2) + TotalQ(1)
	RHydroMeterHeader.iWS         = 0; //        WS, ->    측정 셀 크기(cm)
	RHydroMeterHeader.iWF         = 0; //        WF, ->    공백거리셀
	RHydroMeterHeader.iWP         = 0;
	//      DIS1, ->    첫번째 셀 중심거리(cm)
	RHydroMeterHeader.iDIS1       = 0;
	//      DIS2, ->    마지막 셀 중심거리(cm)
	RHydroMeterHeader.iDIS2       = 0;
}


//---------------------------------------------------------------------------
// Accusonic Data
void TRHydroMeter::_GetAdvTextAccsonic(TRHydroMeterUvmHeader &RHydroMeterUvmHeader)
//ag series
{
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp=Accsonic");
#endif
	RHydroMeterUvmHeader.V1 = FloatByNameDef("Value06", 0.0f);	// Velocity 1
	RHydroMeterUvmHeader.V2 = FloatByNameDef("Value07", 0.0f);	// Velocity 2
	RHydroMeterUvmHeader.V3 = FloatByNameDef("Value08", 0.0f);	// Velocity 3
	RHydroMeterUvmHeader.V4 = FloatByNameDef("Value09", 0.0f);	// Velocity 4
	RHydroMeterUvmHeader.V5 = FloatByNameDef("Value10", 0.0f);	// Velocity 5
	RHydroMeterUvmHeader.V6 = FloatByNameDef("Value11", 0.0f);	// Velocity 6
	RHydroMeterUvmHeader.V7 = FloatByNameDef("Value12", 0.0f);	// Velocity 7
	RHydroMeterUvmHeader.V8 = FloatByNameDef("Value13", 0.0f);	// Velocity 8

	RHydroMeterUvmHeader.Gn1 = ValueByName("Value14").ToIntDef(0);	// Gain Path 1 (LDG1)
	RHydroMeterUvmHeader.Gn2 = ValueByName("Value15").ToIntDef(0);	// Gain Path 2 (LDG2)
	RHydroMeterUvmHeader.Gn3 = ValueByName("Value16").ToIntDef(0);	// Gain Path 3 (LDG3)
	RHydroMeterUvmHeader.Gn4 = ValueByName("Value17").ToIntDef(0);	// Gain Path 4 (LDG4)
	RHydroMeterUvmHeader.Gn5 = ValueByName("Value18").ToIntDef(0);	// Gain Path 5 (LDG5)
	RHydroMeterUvmHeader.Gn6 = ValueByName("Value19").ToIntDef(0);	// Gain Path 6 (LDG6)
	RHydroMeterUvmHeader.Gn7 = ValueByName("Value20").ToIntDef(0);	// Gain Path 7 (LDG7)
	RHydroMeterUvmHeader.Gn8 = ValueByName("Value21").ToIntDef(0);	// Gain Path 8 (LDG8)

	RHydroMeterUvmHeader.Sn1 = ValueByName("Value30").ToIntDef(0);	// Gain Path 1 (LDB1)
	RHydroMeterUvmHeader.Sn2 = ValueByName("Value31").ToIntDef(0);	// Gain Path 2 (LDB2)
	RHydroMeterUvmHeader.Sn3 = ValueByName("Value32").ToIntDef(0);	// Gain Path 3 (LDB3)
	RHydroMeterUvmHeader.Sn4 = ValueByName("Value33").ToIntDef(0);	// Gain Path 4 (LDB4)
	RHydroMeterUvmHeader.Sn5 = ValueByName("Value34").ToIntDef(0);	// Gain Path 5 (LDB5)
	RHydroMeterUvmHeader.Sn6 = ValueByName("Value35").ToIntDef(0);	// Gain Path 6 (LDB6)
	RHydroMeterUvmHeader.Sn7 = ValueByName("Value36").ToIntDef(0);	// Gain Path 7 (LDB7)
	RHydroMeterUvmHeader.Sn8 = ValueByName("Value37").ToIntDef(0);	// Gain Path 8 (LDB8)
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
	if (ValueByName("HydroKind") == CsRQ30         ) fp = &TRHydroMeter::_GetAdvTextRQ; //RQ-30
	if (ValueByName("HydroKind") == CsRQ30D        ) fp = &TRHydroMeter::_GetAdvTextRQ30D; //RQ-30D


#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유속계 종류[" + ValueByName("HydroKind") + "]");
	if (fp == NULL) frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp is NULL");
#endif
	if (fp != NULL)
	{
		(this->*fp)(RHydroMeterHeader); //함수포인터 실행
	} // if

	UnicodeString sDeciADVM = "";

	////////////////////////////////////////////////////////////////////////////
	// RQ-30
	////////////////////////////////////////////////////////////////////////////
	if ( ValueByName("HydroKind") == CsRQ30 ) {	// RQ30이면 이 로직을 따른다.

		// Cell #1 ~ #4 Data 생성 ..
		AnsiString sHDCell = "";
		char cHydroCellDecision = '0';	// 셀 유속

		// 결과 String 생성
		double Value02 = 0.0f;
		double Value03 = 0.0f;
		double Value04 = 0.0f;
		double Value05 = 0.0f;
		double Value06 = 0.0f;
		double Value07 = 0.0f;
		double Value08 = 0.0f;
		double Value09 = 0.0f;
		double Value10 = 0.0f;
		double Value11 = 0.0f;
		double Value12 = 0.0f;
		double Value13 = 0.0f;
		double Value14 = 0.0f;
		double Value15 = 0.0f;
		double Value16 = 0.0f;
		double Value17 = 0.0f;

		Value02 =  FloatByNameDef("Value02", 0.0f);	// Velocity(Unit:m/s)
		Value03 =  FloatByNameDef("Value03", 0.0f); // SNR(Quality)
		Value04 =  FloatByNameDef("Value04", 0.0f); // Discharge(m3/sec)
		Value05 =  FloatByNameDef("Value05", 0.0f); // Area (Cross Section area)(m2)
		Value06 =  FloatByNameDef("Value06", 0.0f); // Learned Velocity (Unit:m/s)
		Value07 =  FloatByNameDef("Value07", 0.0f); // Learned Discharge(Unit:m/s)
		Value08 =  FloatByNameDef("Value08", 0.0f); // Opposite Direction
		Value09 =  FloatByNameDef("Value09", 0.0f); // Supply Voltage
		Value10 =  FloatByNameDef("Value10", 0.0f); // Peak Width(mm/s)
		Value11 =  FloatByNameDef("Value11", 0.0f); // CSR(%)
		Value12 =  FloatByNameDef("Value12", 0.0f); // Area of the peak
		Value13 =  FloatByNameDef("Value13", 0.0f); // RMS at the PIC
		Value14 =  FloatByNameDef("Value14", 0.0f); // Amplification
		Value15 =  FloatByNameDef("Value15", 0.0f); // Amplication relation(%)
		Value16 =  FloatByNameDef("Value16", 0.0f); // Signal relation(%)
		Value17 =  FloatByNameDef("Value17", 0.0f); // Error Code

		double Velocity        = Value02;
		double LearnedVelocity = Value06;
		double Discharge       = Value04;
		double LearnedDischarge= Value07;

//		Value02 = Value02 * 1000;	// meter/sec => mm/sec : Velocity
//		Value06 = Value06 * 1000;	// meter/sec => mm/sec : Velocity
		if ( Velocity <= -99.0f )        cHydroCellDecision = '1';	// 오측
		if ( Velocity <= -99.0f )        Velocity           = -32768;   	// Bad Velocity ( ADCP Type )
		if ( LearnedVelocity <= -99.0f ) LearnedVelocity    = -32768;		// Bad Velocity ( ADCP Type )

		// 2022.12.15 이렇게 하기로 변경함.
//		// Cell #1
		sHDCell += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", 1, LearnedVelocity, Velocity, LearnedDischarge, Discharge);
		// Cell #2
		sHDCell += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", 2, LearnedDischarge, 0.0f, 0.0f, 0.0f);	// mean => TotalQ

// 2022.12.15 Cell에 데이터 채우는 방법 달라짐.
//		// Cell #1
//		sHDCell += sFormat.sprintf("%10d,%10.3f,%10d,%10d,%10d,", 1, Velocity,        0, 0, 0); // Velocity
//		// Cell #2
//		sHDCell += sFormat.sprintf("%10d,%10.3f,%10d,%10d,%10d,", 2, LearnedVelocity, 0, 0, 0); // Learned Velocity
//		// Cell #3
//		sHDCell += sFormat.sprintf("%10d,%10.3f,%10d,%10d,%10d,", 3, Discharge,       0, 0, 0);	// Discharge
//		// Cell #4
//		sHDCell += sFormat.sprintf("%10d,%10.3f,%10d,%10d,%10d,", 4, LearnedDischarge,0, 0, 0);	// Learned Discharge

		//리턴데이터
		//   No_ADVM, -> 유속계 번호(n) -> 1(하층유속계) 2(상층유속계)
		sResult += sFormat.sprintf("%10d", iUnitNumber) + ",";
		// Deci_ADVM, ->    유속계 판단(수온, 피치, 롤, 셀유속)
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fTemp_Water < frmMain->DBEnvInfo.fTemp_Water_Min || RHydroMeterHeader.fTemp_Water > frmMain->DBEnvInfo.fTemp_Water_Max) ? "1" : "0"; //수온
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fPitch      < frmMain->DBEnvInfo.fPitch_Min      || RHydroMeterHeader.fPitch      > frmMain->DBEnvInfo.fPitch_Max     ) ? "1" : "0"; //피치
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fRoll       < frmMain->DBEnvInfo.fRoll_Min       || RHydroMeterHeader.fRoll       > frmMain->DBEnvInfo.fRoll_Max      ) ? "1" : "0"; //롤
		sDeciADVM += cHydroCellDecision; //셀유속

#ifdef _DEBUG
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "sDeciADVM=" + sDeciADVM);
#endif
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

	////////////////////////////////////////////////////////////////////////////
	// RQ-30d
	////////////////////////////////////////////////////////////////////////////

	} else if ( ValueByName("HydroKind") == CsRQ30D ) {	// RQ30이면 이 로직을 따른다.

		//유속계 셀 처리
		TRHydroMeterCell *RHydroMeterCell = new TRHydroMeterCell(DataSet()->Connection, iUnitNumber, RHydroMeterHeader.iWN);
		RHydroMeterCell->ClearParams();
		RHydroMeterCell->SetWhereParam("MEASUREDATE", sMeasureDate);
		RHydroMeterCell->SetWhereParam("MEASURETIME", sMeasureTime);
		RHydroMeterCell->SetOrderParam("CELLNO");
		sResult = RHydroMeterCell->Open();

		AnsiString sHDCell = "";
		double SumSlaveLearnDischarge = 0.0f;	// Sum of Slave Learn Discharge
		char cHydroCellDecision = RHydroMeterCell->GetAdvText(sHDCell, ValueByName("HydroKind"));
		SumSlaveLearnDischarge = RHydroMeterCell->SumSlaveLearnDischarge;
		delete RHydroMeterCell;

		// 결과 String 생성
		double Value02 = 0.0f;
		double Value03 = 0.0f;
		double Value04 = 0.0f;
		double Value05 = 0.0f;
		double Value06 = 0.0f;
		double Value07 = 0.0f;
		double Value08 = 0.0f;
		double Value09 = 0.0f;
		double Value10 = 0.0f;
		double Value11 = 0.0f;
		double Value12 = 0.0f;
		double Value13 = 0.0f;
		double Value14 = 0.0f;
		double Value15 = 0.0f;
		double Value16 = 0.0f;
		double Value17 = 0.0f;

		Value02 =  FloatByNameDef("Value02", 0.0f);	// Velocity(Unit:m/s)
		Value03 =  FloatByNameDef("Value03", 0.0f); // SNR(Quality)
		Value04 =  FloatByNameDef("Value04", 0.0f); // Discharge(m3/sec)
		Value05 =  FloatByNameDef("Value05", 0.0f); // Area (Cross Section area)(m2) => TotalDischarge(RQ-30d Mater device)
		Value06 =  FloatByNameDef("Value06", 0.0f); // Learned Velocity (Unit:m/s)
		Value07 =  FloatByNameDef("Value07", 0.0f); // Learned Discharge(Unit:m/s)
		Value08 =  FloatByNameDef("Value08", 0.0f); // Opposite Direction
		Value09 =  FloatByNameDef("Value09", 0.0f); // Supply Voltage
		Value10 =  FloatByNameDef("Value10", 0.0f); // Peak Width(mm/s)
		Value11 =  FloatByNameDef("Value11", 0.0f); // CSR(%)
		Value12 =  FloatByNameDef("Value12", 0.0f); // Area of the peak
		Value13 =  FloatByNameDef("Value13", 0.0f); // RMS at the PIC
		Value14 =  FloatByNameDef("Value14", 0.0f); // Amplification
		Value15 =  FloatByNameDef("Value15", 0.0f); // Amplication relation(%)
		Value16 =  FloatByNameDef("Value16", 0.0f); // Signal relation(%)
		Value17 =  FloatByNameDef("Value17", 0.0f); // Error Code

		double Velocity        = Value02;
		double LearnedVelocity = Value06;
		double Discharge       = Value04;
		double LearnedDischarge= Value07;
		double TotalQ          = Value05;	// TotalQ(TotalDischarge)
		double TotalLQ         = 0.0f;			// 2023.03.15 Sum of Learn Discharge

		TotalLQ += LearnedDischarge;		// 2023.03.15 Master Learn Discharge
		TotalLQ += SumSlaveLearnDischarge;	// 2023.03.15 Sum of Slave Learn Discharge

//		Value02 = Value02 * 1000;	// meter/sec => mm/sec : Velocity
//		Value06 = Value06 * 1000;	// meter/sec => mm/sec : Velocity
		if ( Velocity <= -99.0f )        cHydroCellDecision = '1';	// 오측
		if ( Velocity <= -99.0f )        Velocity           = -32768;   	// Bad Velocity ( ADCP Type )
		if ( LearnedVelocity <= -99.0f ) LearnedVelocity    = -32768;		// Bad Velocity ( ADCP Type )

		AnsiString CellData = "";
//		// Cell #1 => Master
		CellData += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", 1, LearnedVelocity, Velocity, LearnedDischarge, Discharge);

		// Slave(RG-30d) Cell Data here..
		CellData += sHDCell;  	// From RHydroMeterCell Class

		// Cell #N + 1 => TotalQ  => TotalLQ .. 2023.03.15
		//CellData += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", frmMain->DBEnvInfo.iSlaveCount+2, TotalQ, 0.0f, 0.0f, 0.0f);	// mean => TotalQ
		CellData += sFormat.sprintf("%10d,%10.2f,%10.2f,%10.2f,%10.2f,", frmMain->DBEnvInfo.iSlaveCount+2, TotalLQ, 0.0f, 0.0f, 0.0f);	// mean => TotalQ

	//리턴데이터
		//   No_ADVM, -> 유속계 번호(n) -> 1(하층유속계) 2(상층유속계)
		sResult += sFormat.sprintf("%10d", iUnitNumber) + ",";
		// Deci_ADVM, ->    유속계 판단(수온, 피치, 롤, 셀유속)
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fTemp_Water < frmMain->DBEnvInfo.fTemp_Water_Min || RHydroMeterHeader.fTemp_Water > frmMain->DBEnvInfo.fTemp_Water_Max) ? "1" : "0"; //수온
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fPitch      < frmMain->DBEnvInfo.fPitch_Min      || RHydroMeterHeader.fPitch      > frmMain->DBEnvInfo.fPitch_Max     ) ? "1" : "0"; //피치
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fRoll       < frmMain->DBEnvInfo.fRoll_Min       || RHydroMeterHeader.fRoll       > frmMain->DBEnvInfo.fRoll_Max      ) ? "1" : "0"; //롤
		sDeciADVM += cHydroCellDecision; //셀유속

#ifdef _DEBUG
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "sDeciADVM=" + sDeciADVM);
#endif
		sResult += sFormat.sprintf("%10S", sDeciADVM) + ",";
		//Temp_Water, ->    수온
		sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fTemp_Water) + ",";
		//Depth_ADVM, ->    수압
		sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fDepth_ADVM) + ",";
		//     Pitch, ->    피치
		sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fPitch) + ",";
		//      Roll, ->    롤
		sResult += sFormat.sprintf("%10.2f", RHydroMeterHeader.fRoll) + ","; 	// RQ-30d => Master + Slave #n + 1(TotalQ)
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

		//sResult += sHDCell;  	// Slave Data Add
		sResult += CellData;

	////////////////////////////////////////////////////////////////////////////
	// Other Sensor
	////////////////////////////////////////////////////////////////////////////

	} else {	// 기존 로직
		//유속계 셀 처리
		TRHydroMeterCell *RHydroMeterCell = new TRHydroMeterCell(DataSet()->Connection, iUnitNumber, RHydroMeterHeader.iWN);
		RHydroMeterCell->ClearParams();
		RHydroMeterCell->SetWhereParam("MEASUREDATE", sMeasureDate);
		RHydroMeterCell->SetWhereParam("MEASURETIME", sMeasureTime);
		RHydroMeterCell->SetOrderParam("CELLNO");
		sResult = RHydroMeterCell->Open();

		AnsiString sHDCell = "";
		char cHydroCellDecision = RHydroMeterCell->GetAdvText(sHDCell, ValueByName("HydroKind"));
		delete RHydroMeterCell;

	//리턴데이터
		//   No_ADVM, -> 유속계 번호(n) -> 1(하층유속계) 2(상층유속계) : 2026.02.10 EWSV+ADVM: 유속계2번=>1번으로 처리
		sResult += sFormat.sprintf("%10d", ((EwsvAdvmFlag) ? (iUnitNumber-1) : iUnitNumber)) + ",";
		// Deci_ADVM, ->    유속계 판단(수온, 피치, 롤, 셀유속)
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fTemp_Water < frmMain->DBEnvInfo.fTemp_Water_Min || RHydroMeterHeader.fTemp_Water > frmMain->DBEnvInfo.fTemp_Water_Max) ? "1" : "0"; //수온
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fPitch      < frmMain->DBEnvInfo.fPitch_Min      || RHydroMeterHeader.fPitch      > frmMain->DBEnvInfo.fPitch_Max     ) ? "1" : "0"; //피치
		sDeciADVM += (DataSet()->Eof && DataSet()->Bof) ? "2" : (RHydroMeterHeader.fRoll       < frmMain->DBEnvInfo.fRoll_Min       || RHydroMeterHeader.fRoll       > frmMain->DBEnvInfo.fRoll_Max      ) ? "1" : "0"; //롤
		sDeciADVM += cHydroCellDecision; //셀유속

#ifdef _DEBUG
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "sDeciADVM=" + sDeciADVM);
#endif

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
	}

	return (sDeciADVM.Pos("2") > 0) ? '2' : (sDeciADVM.Pos("1") > 0) ? '1' : '0';
}

//---------------------------------------------------------------------------
// UVM(Accusonic) Text
char TRHydroMeter::GetUvmText(AnsiString &sResult)
{
	sResult = "";
//    if (DataSet()->Eof && DataSet()->Bof) return '0'; //마스터 쿼리 자체가 없으므로 정상으로 리턴
	if (DataSet()->Eof && DataSet()->Bof) return '2'; //heaven2 2018.01.26 마스터 쿼리 자체가 없으므로 정상으로 리턴

	TRHydroMeterUvmHeader RHydroMeterUvmHeader;
	memset(&RHydroMeterUvmHeader, 0x00, sizeof(TRHydroMeterUvmHeader));
	AnsiString sFormat;

	void (TRHydroMeter::*fp)(TRHydroMeterUvmHeader &) = NULL;
	if (ValueByName("HydroKind") == CsAccusonicA ||
		ValueByName("HydroKind") == CsAccusonicB   ) fp = &TRHydroMeter::_GetAdvTextAccsonic; // Accusonic series
	else return '0';
#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유속계 종류[" + ValueByName("HydroKind") + "]");
	if (fp == NULL) frmMain->LogMessage(CbFileWrite, !CbDBWrite, "fp is NULL");
#endif
	if (fp != NULL)
    {
		(this->*fp)(RHydroMeterUvmHeader); //함수포인터 실행
	} // if

// Make Decision
	AnsiString UVM[8];	// default => all 결측으로 우선 설정, 0:Ok, 1:오측, 2:결측
	AnsiString sDeciUVM = "";
	UVM[0] = IsOkWrong(RHydroMeterUvmHeader.V1);
	UVM[1] = IsOkWrong(RHydroMeterUvmHeader.V2);
	UVM[2] = IsOkWrong(RHydroMeterUvmHeader.V3);
	UVM[3] = IsOkWrong(RHydroMeterUvmHeader.V4);
	UVM[4] = IsOkWrong(RHydroMeterUvmHeader.V5);
	UVM[5] = IsOkWrong(RHydroMeterUvmHeader.V6);
	UVM[6] = IsOkWrong(RHydroMeterUvmHeader.V7);
	UVM[7] = IsOkWrong(RHydroMeterUvmHeader.V8);

	if (IsAllZero(RHydroMeterUvmHeader))
		sDeciUVM = "22222222";	// 결측을 의미
	else
		sDeciUVM = UVM[0] + UVM[1] + UVM[2] + UVM[3] + UVM[4] + UVM[5] + UVM[6] + UVM[7]; // "00000000" or others

#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "sDeciUVM=" + sDeciUVM);
#endif

// Make UVM Text
	sResult += sFormat.sprintf("%10s,", sDeciUVM);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V1);	// Velocity(m/s)
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V2);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V3);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V4);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V5);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V6);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V7);
	sResult += sFormat.sprintf("%10.3f,",RHydroMeterUvmHeader.V8);

	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn1);	// Gain
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn2);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn3);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn4);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn5);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn6);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn7);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Gn8);

	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn1);	// Velocity SND Section
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn2);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn3);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn4);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn5);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn6);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn7);
	sResult += sFormat.sprintf("%10d,",RHydroMeterUvmHeader.Sn8);

	if ( sDeciUVM == "22222222" )      return '2';		// 결측
	else if ( sDeciUVM == "00000000" ) return '0';	  	// 정상
	else                               return '1';		// 오측
}

//---------------------------------------------------------------------------
// EWSV Format Data Text
char TRHydroMeter::GetEsvText(AnsiString &sResult)
{
	sResult = "";
	//if (DataSet()->Eof && DataSet()->Bof) return '0'; //마스터 쿼리 자체가 없으므로 정상으로 리턴

#ifdef _DEBUG
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "유속계 종류[" + ValueByName("HydroKind") + "]");
#endif

	UnicodeString sDeciEWSV = "";	// 유속계 판단 => "00", "11", "22"..

	TEsvData EsvData;
	bool bResult = true;
	AnsiString sHeaderText = "";
	AnsiString sDeviceText = "";
	char cDeciEWSV = '0';

	bResult = EsvData.GetEsvData(sMeasureDate, sMeasureTime);   	// EWSV 데이터를 가져온다.

	sHeaderText = EsvData.GetEsvHeaderStr();
	sDeviceText = EsvData.GetEsvDeviceStr();
	sDeciEWSV = EsvData.GetDecision();	// "00","11",'22" ( 유속계 판단: 정상(00), 오측(11), 결측(22)
	cDeciEWSV = EsvData.GetFinalDecision(); //'0','1','2' => 장비 최종 판단.

	sResult = sHeaderText +  sDeviceText;	// EWSV Text 중간에 "," 불필요

	return (sDeciEWSV.Pos("2") > 0) ? '2' : (sDeciEWSV.Pos("1") > 0) ? '1' : '0';
}

//---------------------------------------------------------------------------
// Return Wind data for EWSV Format
char TRHydroMeter::GetWindText(AnsiString &sResult)	// for EWSV Wind Data
{
	AnsiString sFormat;
	AnsiString sDeci_Wind = "";

	if ( frmMain->DBEnvInfo.sHydroKind1 == CsRQ30 || frmMain->DBEnvInfo.sHydroKind1 == CsRQ30D ) {
		sResult = "";

		double fDegree   = 0.0f;	// 풍향
		double fWind     = 0.0f;	// 풍속
		double fTemp     = 0.0f;	// 온도
		double fHumidity = 0.0f;	// 습도
		double fAtmos    = 0.0f;	// 대기압

		fDegree   = FloatByNameDef("Value20", 0.0f);	//
		fWind     = FloatByNameDef("Value21", 0.0f);	//
		fTemp     = FloatByNameDef("Value24", 0.0f);	//
		fHumidity = FloatByNameDef("Value25", 0.0f);	//
		fAtmos    = FloatByNameDef("Value23", 0.0f);	//
		fAtmos    = fAtmos * 1000;	// bar => mbar(단위 변환) DB에는 bar로 저장, 전송은 mbar로 전송

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

	} else { 	// RQ30이 추가되기 전의 Logic
		sResult = "";

		double fDegree   = 0.0f;	// 풍향
		double fWind     = 0.0f;	// 풍속
		double fTemp     = 0.0f;	// 온도
		double fHumidity = 0.0f;	// 습도
		double fAtmos    = 0.0f;	// 대기압

		sDeci_Wind = "22222";	// 결측 데이터
		sResult += sFormat.sprintf("%10s",   sDeci_Wind ) + ","; // Deci_Wind,  -> 풍속계 판단
		sResult += sFormat.sprintf("%10.2f", fDegree )    + ","; // Degree,     -> 풍향
		sResult += sFormat.sprintf("%10.3f", fWind )      + ","; // Wind,       -> 풍속
		sResult += sFormat.sprintf("%10.3f", fTemp )      + ","; // Temp,       -> 온도
		sResult += sFormat.sprintf("%10.2f", fHumidity )  + ","; // Humidity,   -> 습도
		sResult += sFormat.sprintf("%10d", (int)fAtmos )  + ","; // Atmos,      -> 대기압
	}

	return (sDeci_Wind.Pos("2") > 0) ? '2' : (sDeci_Wind.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}


//---------------------------------------------------------------------------
// 유속계 1번(RQ-30d)의 수위 데이터를 수위 데이터로 사용한다.
char TRHydroMeter::GetWaterAdvText(AnsiString &sResult)	// 유속계에서 수위 데이터를 Return한다.
{
	AnsiString sFormat;
	AnsiString sDeci_WL = "";

	if ( frmMain->DBEnvInfo.sHydroKind1 == CsRQ30 || frmMain->DBEnvInfo.sHydroKind1 == CsRQ30D ) {
		sResult = "";

		double fWaterLevel = 0.0f;
		double fWaterDepth = 0.0f;
		double fOffset     = 0.0f;
		double fSalinity   = 0.0f;

		fWaterDepth = FloatByNameDef("Value01", 0.0f);	// HydroMeter Table Value01 => RQ30 WaterLevel Field
		fWaterDepth = W_BoundCheck(fWaterDepth);		// 2026.04.16
		fOffset     = FloatByNameDef("Value18", 0.0f);	// WaterLevel Offset
		fOffset     = 0.0f;								// 2022.05.09 RQ-30은 Offset 적용하지 않음.
														// 2022.05.09 RQ-30은 RQ-30의 자체 WaterLevel을 사용 ( WaterLevel의 Offset 정보를 사용하지 않음)
		fSalinity   = 0.0f;                             // RQ30 => 염도 측정 없음
		fWaterLevel = fWaterDepth + fOffset;	        // unit: meter
		sDeci_WL += (DataSet()->Eof && DataSet()->Bof) ? "2" : (fWaterLevel < frmMain->DBEnvInfo.fWaterLevel_Min || fWaterLevel > frmMain->DBEnvInfo.fWaterLevel_Max) ? "1" : "0"; //수위
		sDeci_WL += "2";	// 염도 결측
		sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; // Deci_WL,    -> 수위계 판단(수위, 염도)
		sResult += sFormat.sprintf("%10.2f", fWaterDepth) + ","; // WaterDepth, -> 수심
		sResult += sFormat.sprintf("%10.3f", fWaterLevel) + ","; // WaterLevel, -> 수위(dept + offset)
		sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset,  -> 오프셋
		sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; // Salinity,   -> 염도

	} else { 	// RQ30 시리즈가 아니면
		sResult = "";

		double fWaterDepth = 0.0f; //WaterDepth, ->    수심
		double fWaterLevel = 0.0f; // WaterLevel, ->    수위(dept + offset)
		double fOffset     = 0.0f;
		double fSalinity   = 0.0f; //  Salinity, ->    염도

		sDeci_WL += "2";	// 결측
		sDeci_WL += "2";	// 결측
		sResult += sFormat.sprintf("%10s",   sDeci_WL   ) + ","; // Deci_WL,    -> 수위계 판단(수위, 염도)
		sResult += sFormat.sprintf("%10.2f", fWaterDepth) + ","; // WaterDepth, -> 수심
		sResult += sFormat.sprintf("%10.3f", fWaterLevel) + ","; // WaterLevel, -> 수위(dept + offset)
		sResult += sFormat.sprintf("%10.3f", fOffset    ) + ","; // WL_Offset,  -> 오프셋
		sResult += sFormat.sprintf("%10.2f", fSalinity  ) + ","; // Salinity,   -> 염도
	}
	return (sDeci_WL.Pos("2") > 0) ? '2' : (sDeci_WL.Pos("1") > 0) ? '1' : '0'; //결측이 하나라도 있으면 결측, 오측이 하나라도 있으면 오측 아니면 정상
}


//---------------------------------------------------------------------------
//
void TRHydroMeter::SetEwsvAdvmFlag(bool bEwsvAdvmFlag) // true: EWSV+ADVM, false: only ADVM 2026.02.10
{
	EwsvAdvmFlag = bEwsvAdvmFlag;  	// Default: false
	// true : RHydroMeter2 => 유속계 번호 1,
	// false: RHydroMeter2 => 유속계 번호 2
	// 이유: EWSV+ADVM 일경우 유속계 1번: EWSV
	//                        유속계 2번: ADVM #1, 유속계 3번: ADVM #2
}

//---------------------------------------------------------------------------
TRHydroMeter::~TRHydroMeter()
{
}
//---------------------------------------------------------------------------
