//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "FlowStream.h"
#include "ChannelMaster.h"
#include "SL500K.h"
#include "AG1500K.h"
#include "Accusonic.h"

#include <typeinfo.h>

//---------------------------------------------------------------------------

int __fastcall CompareFlowVelocity(void *Item1, void *Item2)
{
	STFlowVelocity *A, *B;
	A = (STFlowVelocity *)Item1;
	B = (STFlowVelocity *)Item2;
	if ( A->Velocity < B->Velocity )       return -1;	// True  => Skip
	else if ( A->Velocity == B->Velocity ) return  0;
	else                                   return  1;	// false => Swap
}


//---------------------------------------------------------------------------
TFlowStream::TFlowStream()
{
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep =  enSensorIdle;	// Sensor Working Status
	m_FlowType   = 0;	// 0:not flowstream 1:유량사업단, 2:RNSEA itself
}

//---------------------------------------------------------------------------
TFlowStream::TFlowStream(int iFlowType)
{
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep =  enSensorIdle;	// Sensor Working Status
	m_FlowType = iFlowType;	// 0:not flowstream 1:유량사업단, 2:RNSEA itself
}

//---------------------------------------------------------------------------
TFlowStream::~TFlowStream()
{
}

//---------------------------------------------------------------------------
void TFlowStream::Clear()
{
	m_enStep        = enSensorIdle;			// Sensor Status ( idle-start-working-finish )
	m_enWorkingStep = enSensorIdle;		// 2014.09.17 Sensor Working Status (idle-start-finish-start-finish-...)

	m_FlowType     = 0;	// 0: Not FlowStream 1: 유량사업단, 2:RNSEA itself
	m_MeasureDate  = "";
	m_MeasureTime  = "";

	//m_pRFlowIVM    = NULL;
	//m_pRFlowHA     = NULL;;
	//m_pRFlowADVM   = NULL;;

	//m_pWaterLevel  = NULL;
	//m_pHydroMeter1 = NULL;
	//m_pHydroMeter2 = NULL;

	m_RStream.Clear();
}

//---------------------------------------------------------------------------
void TFlowStream::Init(int iFlowType)
{
	m_FlowType = iFlowType;
}


//---------------------------------------------------------------------------
void TFlowStream::InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM)
{
	m_pRFlowIVM  = pRFlowIVM;
	m_pRFlowHA   = pRFlowHA;
	m_pRFlowADVM = pRFlowADVM;

}

//---------------------------------------------------------------------------
void TFlowStream::InitWaterHydroMeter( TWaterLevel *pWaterLevel, THydroMeter *pHydro1, THydroMeter *pHydro2)
{
	m_pWaterLevel  = pWaterLevel;
	m_pHydroMeter1 = pHydro1;
	m_pHydroMeter2 = pHydro2;
}

//---------------------------------------------------------------------------
void TFlowStream::SetMeasureTime(UnicodeString sMeasureDate, UnicodeString sMeasureTime)
{
	m_MeasureDate = sMeasureDate;
	m_MeasureTime = sMeasureTime;

	m_RStream.MeasureDate = sMeasureDate;
	m_RStream.MeasureTime = sMeasureTime;
}

//---------------------------------------------------------------------------
// 유량을 산출한다.
bool TFlowStream::CaculateFlowStream()
{
	bool   bResult    = false;
	double Q          = 0; 	// 유량
	double WaterLevel = 0;	// 수위
	double Area       = 0; 	// 단면적
	double Velocity   = 0;	// 평균 유속
	double V1         = 0; 	// 하층 유속계 평균 속도
	double V2         = 0; 	// 상층 유속계 평균 속도

	if ( m_pWaterLevel == NULL || m_pHydroMeter1 == NULL ) {
		LogMessage("WaterLevel sensor or HydorMeter sensor is not ready..");
		return false;
	}

	if (typeid(*m_pHydroMeter1) == typeid(TAccusonic) || typeid(*m_pHydroMeter2) == typeid(TAccusonic)) {        // Accusonic 장비는 유량산출 지원하지 않음
		LogMessage("Accusonic sensor is not support flowstream..");
		return false;
	}

	// Error Check
	if ( m_pWaterLevel == NULL || m_pHydroMeter1 == NULL ) {
		bResult = m_RStream.Insert();  	// All Data is 0
		return bResult;
	}

	if ( m_FlowType == 2 ) {
		// -------------------------------------------------------------------------
		// RNSEA 자체 방식의 유량을 산출한다.
		// -------------------------------------------------------------------------
		CaculateFlowStreamByRNSEA();
		return true;

	}

	// -------------------------------------------------------------------------
	// 유량 사업단 방식의 유량을 산출한다.
	// -------------------------------------------------------------------------

	// A(단면적)을 구한다. -----------------------------------------------------

	WaterLevel = m_pWaterLevel->GetWaterLevel();	// 수위
	Area       = m_pWaterLevel->GetArea();

	if ( Area == 0 ) {	// 단면적이 0이면 오류로 모든 데이터를 0인 상태에서 Insert하고 만다.
		bool bResult = false;
		bResult = m_RStream.Insert();
		return bResult;
	}

	// 표면 유속으로 평균 유속을 구한다. ---------------------------------------

	bool bV1, bV2;
	bV1 = bV2 = false;
	UnicodeString Str;

	if ( m_pHydroMeter1 != NULL && m_pHydroMeter2 != NULL && m_pRFlowADVM->BType == "SOUTHNORTH" ) {

		// 하층/상층 유속계가 Sourth/North 형태(한강대교)를 가지고 있을 때 산출 방식
		// 두 유속계의 유효셀을 모아 하나의 리스트로 만들고 이 중에서 평균 유속을 구한다.

		V1 = GetVelocityBySourthNorth();

		if ( V1 != 0 ) bV1 = true;
		else           bV1 = false;

		V2  = 0;
		bV2 = false;

		LogMessage("[TFlowStream::CaculateFlowStream] HydroMeter1/2 is SOUTHNORTH..");

	} else {

		// 일반적인 유량산출 상황일 때는 모두 이 루틴을 이용한다.
		// 하층 유속계의 평균 유속을 구한다. ---------------------------------------

		if ( m_pRFlowADVM->AZadvm + m_pRFlowADVM->ASubmersion > WaterLevel ) { // 하층 유속 => 지표 유속 사용 불가
			V1  = 0;
			bV1 = false;
			Str = Str.sprintf(L"[TFlowStream::CaculateFlowStream] WaterLevel < AZadvm + ASubmersion, V1 = 0, WaterLevel=%f AZadvm=%f ASubmersion=%f", WaterLevel, m_pRFlowADVM->AZadvm, m_pRFlowADVM->ASubmersion);
			LogMessage(Str);
		} else {
			V1  = m_pHydroMeter1->GetAvgVelocity(); // mm => M로 환산하여 return
			bV1 = true;

			if ( V1 < m_pRFlowADVM->AVMin || V1 > m_pRFlowADVM->AVMax ) {	// 설정은 m, 유속은 mm이므로 mm로 환산
				// 측정한 평균 유속이 설정한 최대/최소를 벗어나 신뢰할 수 없음
				Str = Str.sprintf(L"[TFlowStream::CaculateFlowStream] V1 < AVMin Or V1 > AVMax, V1 = 0, V1 = %f, AVMin=%f AVMax=%f", V1, m_pRFlowADVM->AVMin, m_pRFlowADVM->AVMax);
				LogMessage(Str);
				V1  = 0;
				bV1 = false;
			} else {
				bV1 = true;
			}
		}

		// 상층 유속계를 구한다 ----------------------------------------------------

		if ( m_pHydroMeter2 == NULL ) {
			V2  = 0;
			bV2 = false;
		} else {
			if ( m_pRFlowADVM->BZadvm + m_pRFlowADVM->BSubmersion > WaterLevel ) { // 상층 유속 => 지표 유속 사용 불가
				V2 = 0;
				bV2 = false;
				Str = Str.sprintf(L"[TFlowStream::CaculateFlowStream] WaterLevel < BZadvm + BSubmersion, V1 = 0, WaterLevel=%f BZadvm=%f BSubmersion=%f", WaterLevel, m_pRFlowADVM->BZadvm, m_pRFlowADVM->BSubmersion);
				LogMessage(Str);
			} else {
				V2  = m_pHydroMeter2->GetAvgVelocity();	// mm => M로 환산하여 return
				bV2 = true;

				if ( V2 < m_pRFlowADVM->BVMin || V2 > m_pRFlowADVM->BVMax ) {
					// 측정한 평균 유속이 설정한 최대/최소를 벗어나 신뢰할 수 없음
					V2  = 0;
					bV2 = false;
					Str = Str.sprintf(L"[TFlowStream::CaculateFlowStream] V2 < AVMin Or V2 > AVMax, V2 = 0, V2= %f, BVMin=%f BVMax=%f", V1, m_pRFlowADVM->BVMin, m_pRFlowADVM->BVMax);
					LogMessage(Str);
				} else {
					bV2 = true;
				}
			}
		}
	}

	// 만약 HydroMeter2가 한강대교 형태(North/South)형태일 때의 처리
	// 일단 주석

	// 유량을 구한다. ----------------------------------------------------------
	UnicodeString sMsg;
	sMsg = sMsg.sprintf(L"[TFlowStream::CaculateFlowStream] V1=%f, V2=%f, Q=%f, Area=%f, bV1=%s, bV2=%s", V1, V2, Q, Area,
															((bV1) ? L"true" : L"false"), ((bV1) ? L"true" : L"false"));
	LogMessage(sMsg);

	if ( bV1 && bV2 ) {		// 상층/하층 유속계가 모두 유효할 때
		Velocity = (V1+V2) / 2 ;
		Q = Area * Velocity;
	} else if ( bV1 && !bV2 ) {	// 하층 유속만 유효할 때
		Velocity = V1;
		Q = Area * Velocity;
	}  else { // !bV1 && !bV2	=> 두 개의 유속계가 모두 유효하지 않을 때
		Velocity = 0;
		Q = Area * Velocity;
	}

	m_RStream.AvgStream  = Q;
	m_RStream.WaterLevel = WaterLevel;
	m_RStream.Area       = Area;
	m_RStream.Velocity   = Velocity;

	bResult= m_RStream.Insert();

	return bResult;
}

//---------------------------------------------------------------------------
// 남측/북측(한강대교의 경우)에 대한 표준 속도를 구한다.
double TFlowStream::GetVelocityBySourthNorth()
{
	double Velocity = 0;

	if ( m_pHydroMeter1 == NULL || m_pHydroMeter2 == NULL ) return 0;
	if ( m_pRFlowADVM->BType != "SOUTHNORTH" ) return 0;

	TList *pFlowVelocityList = new TList();

	TList *pCellList = NULL;
	STFlowVelocity *pFlowVelocity = NULL;

	// 하층 유속계 -------------------------------------------------------------
	if ( typeid(*m_pHydroMeter1) == typeid(TChannelMaster) ) {
		pCellList = m_pHydroMeter1->m_pVelocityList;
		STVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->AStartCell-1 ; i <= m_pRFlowADVM->AEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 1;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->AReverseMode == 0 ) ? pVelocity->Velocity1 * -1 : pVelocity->Velocity1;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else if ( typeid(*m_pHydroMeter1) == typeid(TAG1500K) ) {
		pCellList = m_pHydroMeter1->m_pVelocityList;
		STAGVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->AStartCell-1 ; i <= m_pRFlowADVM->AEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STAGVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 1;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->AReverseMode == 0 ) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else if ( typeid(*m_pHydroMeter1) == typeid(TSL500K) ) {
		pCellList = m_pHydroMeter1->m_pVelocityList;
		STSLVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->AStartCell-1 ; i <= m_pRFlowADVM->AEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STSLVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 1;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->AReverseMode == 0 ) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else {
	}

	// 상층 유속계 -------------------------------------------------------------
	if ( typeid(*m_pHydroMeter2) == typeid(TChannelMaster) ) {
		pCellList = m_pHydroMeter2->m_pVelocityList;
		STVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->BStartCell-1 ; i <= m_pRFlowADVM->BEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 2;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->BReverseMode == 0 ) ? pVelocity->Velocity1 * -1 : pVelocity->Velocity1;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else if ( typeid(*m_pHydroMeter2) == typeid(TAG1500K) ) {
		pCellList = m_pHydroMeter2->m_pVelocityList;
		STAGVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->BStartCell-1 ; i <= m_pRFlowADVM->BEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STAGVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 2;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->BReverseMode == 0 ) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else if ( typeid(*m_pHydroMeter2) == typeid(TSL500K) ) {
		pCellList = m_pHydroMeter2->m_pVelocityList;
		STSLVelocity *pVelocity = NULL;
		for ( int i = m_pRFlowADVM->BStartCell-1 ; i <= m_pRFlowADVM->BEndCell ; i++ ) {
			pFlowVelocity = new STFlowVelocity();

			pVelocity = (STSLVelocity *)pCellList->Items[i];

			pFlowVelocity->HydroNo = 2;
			pFlowVelocity->CellNo  = i;
			pFlowVelocity->Velocity = (m_pRFlowADVM->BReverseMode == 0 ) ? pVelocity->VelocityX * -1 : pVelocity->VelocityX;
			pFlowVelocityList->Add(pFlowVelocity);
		}
	} else {
	}

	// 평균 유속을 산출한다. ---------------------------------------------------
	if ( pFlowVelocityList->Count > 5 ) {

		pFlowVelocityList->Sort(CompareFlowVelocity);	// Sort

		int iMid = pFlowVelocityList->Count / 2;
		STFlowVelocity *pFlowVelocity = NULL;
		pFlowVelocity = (STFlowVelocity *)pFlowVelocityList->Items[iMid];
		Velocity  = pFlowVelocity->Velocity;

	} else {
		// 평균값 산출
		double sum = 0;
		for ( int i = 0 ; i < pFlowVelocityList->Count ; i++ ) {
			pFlowVelocity = (STFlowVelocity *)pFlowVelocityList->Items[i];
			sum += pFlowVelocity->Velocity;
		}
		Velocity = (pFlowVelocityList->Count == 0) ? 0.0 : sum / pFlowVelocityList->Count;
	}


	// Release Resource --------------------------------------------------------

	for ( int i = 0 ; i < pFlowVelocityList->Count ; i++ ) {
		pFlowVelocity = (STFlowVelocity *)pFlowVelocityList->Items[i];
		delete pFlowVelocity;
	}
	delete pFlowVelocityList;


	return Velocity * 0.001;	//  두 유속계가 Sorth/North 형태로 배치되었을 때의 평균 유속
								// 단위를 m로 환산하여 return한다.
}

//---------------------------------------------------------------------------
// RNSEA 방식의 유량을 산출한다.(향후 확장한다.) - 2014.05.26
bool TFlowStream::CaculateFlowStreamByRNSEA()
{
	return true;
}

//---------------------------------------------------------------------------
void TFlowStream::LogMessage(UnicodeString LogMsg, bool bFile)
{
	FormMain->LogMessage(LogMsg, bFile);
}


#pragma package(smart_init)
