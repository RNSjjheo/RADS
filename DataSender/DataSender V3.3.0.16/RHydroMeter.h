//---------------------------------------------------------------------------
#ifndef RHydroMeterH
#define RHydroMeterH

//#include <vcl.h>
#include "fbTable.h"
#include "Env.h"

#define DECISION_OK		0	// 판단(정상)
#define DECISION_WRONG	1	// 판단(오측)
#define DECISION_NODATA	2	// 판단(결측)

//---------------------------------------------------------------------------
struct STEsvData {
	int    Address;		// 1,2,...10
	double LV;
	double V;
	double LQ;
	double Q;
	double Degree;
	int    SNR;
	int    SelfCheck;

	STEsvData::STEsvData() {
		memset(this, 0x00, sizeof(STEsvData));
	}
};

//---------------------------------------------------------------------------
class TEsvData
{
private:
	TList *pList;

	AnsiString DecisionEsv;	    // (ex) "00", "11", "22"
	int        DeviceType;	    // 0:None, 1:RQ-30d,  2,3,4,..
	double     AverageLV; 		// Average of Learned Velocity
	double     AverageV;		// Average of Raw Velocity
	double     TotalLQ;			// Sum of Learned Discharge
	double     TotalQ;			// Sum of Raw Discharge
	int        DeviceCount;		// DeviceCount : Master(1) + Slave(4) => 5
	AnsiString DeviceUseFlag;	// (ex)"01110"

	int        VFlag;			// 유속 판단(0:정장, 1:오측, 2: 결측)
	int        QFlag;			// 유량 판단(0:정장, 1:오측, 2: 결측)
	char       DecEsv;			// 유량,유속 최종 판단
	bool       UseFlag[10];		// 단면 데이터가 사용됐다, 안됐다 구분 Flag => DeviceUseFlag에 사용됨

	UnicodeString MeasureDate;
	UnicodeString MeasureTime;
public:
	TEsvData();
	~TEsvData();

	bool GetEsvData(UnicodeString MeasureDate, UnicodeString MeasureTime);	// Master(RHydroMeter1), Slave(RHydroMeter1Cell)
	AnsiString GetEsvHeaderStr();
	AnsiString GetEsvDeviceStr();
	AnsiString GetDecision();	// 유속계 판단(ex) "00", "11", "22"
	char       GetFinalDecision();	// '0','1','2' 최종 판단

	int        GetAddressByIndex(int Index);	// 좌안부터 순서대로의 Address값을 반환한다.
	int        GetIndexByAddress(int Address);	// Address별 좌안부터의 Index값을 찾는다.
	STEsvData *GetEsvDataByIndex(int Index);
	STEsvData *GetEsvDataByAddress(int Address);
};

//---------------------------------------------------------------------------
class TRHydroMeter : public TfbTable
{
private:
    UnicodeString sMeasureDate;
    UnicodeString sMeasureTime;
	int iUnitNumber;
	bool EwsvAdvmFlag;	// true: EWSV+ADVM, false: only ADVM 2026.02.10

	void _GetAdvTextCM(TRHydroMeterHeader &RHydroMeterHeader);
    void _GetAdvTextSL(TRHydroMeterHeader &RHydroMeterHeader);
	void _GetAdvTextAG(TRHydroMeterHeader &RHydroMeterHeader);
	void _GetAdvTextRQ(TRHydroMeterHeader &RHydroMeterHeader);	// RQ-30, heaven2 2021.12.09
	void _GetAdvTextRQ30D(TRHydroMeterHeader &RHydroMeterHeader);	// RQ-30d, heaven2 2022.12.15
	void _GetAdvTextAccsonic(TRHydroMeterUvmHeader &RHydroMeterUvmHeader);
public:
	TRHydroMeter(TSQLConnection *Connection, int iUnitNo);	// EWSV+AVDM 혼용인 지 Flag
	~TRHydroMeter();
	UnicodeString Open(); //파라미터로 자료 오픈
	char GetAdvText(AnsiString &sResult);	// for ADVM Format
	char GetUvmText(AnsiString &sResult);	// for UVM Format
	char GetEsvText(AnsiString &sResult);  	// for EWSV Format
	char GetWindText(AnsiString &sResult);	// for EWSV Wind Data

	char GetWaterAdvText(AnsiString &sResult);	// 유속계에서 수위 데이터를 Return한다.
	void SetEwsvAdvmFlag(bool bEwsvAdvmFlag=false); // true: EWSV+ADVM, false: only ADVM 2026.02.10

};

UnicodeString IsOkWrong(double Value);
bool IsAllZero(TRHydroMeterUvmHeader &RHydroMeterUvmHeader);

// 2026.04.16
// W, V, LV, Q, LQ 의 LowerBound<=>UpperBound Check : ADCP 제외(오직 전자파유속계만 해당)
// W : unit: meter
// V, LV : unit: m/s
// Q, LQ : unit: m3/s
double W_BoundCheck(double W);
double V_BoundCheck(double V);
double Q_BoundCheck(double Q);

//---------------------------------------------------------------------------
#endif
