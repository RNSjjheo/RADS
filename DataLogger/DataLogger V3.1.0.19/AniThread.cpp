//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "AniThread.h"
#include "UnitMain.h"
#include "HydroMeter.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TAnimation::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TAniThread::TAniThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	m_FlowStreamIndex   = enFlowStreamNormal;
	m_WaterLevelIndex   = enWaterLevelNormal;
	m_HydroMeter1Index  = enHydroMeterNormal;
	m_HydroMeter2Index  = enHydroMeterNormal;

	m_bFlowStream  = false;
	m_bHydroMeter1 = false;
	m_bHydroMeter2 = false;
	m_bWaterLevel  = false;
	m_iHydroKind1  = 0;	// 0:None, 1: ChannelMaster, 2:AG/SL
	m_iHydroKind2  = 0;	// 0:None, 1: ChannelMaster, 2:AG/SL
}

//---------------------------------------------------------------------------
void __fastcall TAniThread::Execute()
{
	//bool bFlowStream     = false;
	//bool bHydroMeter1    = false;
	//bool bHydroMeter2    = false;
	//bool bWaterLevel     = false;
	//int  iHydroKind1     = 0;	// 0:None, 1: ChannelMaster, 2:AG/SL
	//int  iHydroKind2     = 0;	// 0:None, 1: ChannelMaster, 2:AG/SL

	if ( FormMain->m_pFlowStream  != NULL ) m_bFlowStream  = false;
	if ( FormMain->m_pHydroMeter1 != NULL ) m_bHydroMeter1 = false;
	if ( FormMain->m_pHydroMeter2 != NULL ) m_bHydroMeter2 = false;
	if ( FormMain->m_pWaterLevel  != NULL ) m_bWaterLevel  = false;

	//if ( m_bHydroMeter1 ) {
	if ( FormMain->m_pHydroMeter1 != NULL ) {
		if ( FormMain->m_pHydroMeter1->HydroKind == CHANNELMASTER_STR )
			m_iHydroKind1 = HYDROMETER_CHANNELMASTER;
		else if ( FormMain->m_pHydroMeter1->HydroKind == AGSERIES_STR || FormMain->m_pHydroMeter1->HydroKind == SLSERIES_STR )
			m_iHydroKind1 = HYDROMETER_SLAG;
		else if ( FormMain->m_pHydroMeter1->HydroKind == ACCUSONIC_A_STR || FormMain->m_pHydroMeter1->HydroKind == ACCUSONIC_B_STR )
			m_iHydroKind1 = HYDROMETER_ACCUSONIC;
		else
			m_iHydroKind1 = HYDROMETER_CHANNELMASTER;
	}

	//if ( m_bHydroMeter2 ) {
	if ( FormMain->m_pHydroMeter2 != NULL ) {
		if ( FormMain->m_pHydroMeter2->HydroKind == CHANNELMASTER_STR )
			m_iHydroKind2 = HYDROMETER_CHANNELMASTER;
		else if ( FormMain->m_pHydroMeter2->HydroKind == AGSERIES_STR || FormMain->m_pHydroMeter2->HydroKind == SLSERIES_STR )
			m_iHydroKind2 = HYDROMETER_SLAG;
		else if ( FormMain->m_pHydroMeter2->HydroKind == ACCUSONIC_A_STR || FormMain->m_pHydroMeter2->HydroKind == ACCUSONIC_B_STR )
			m_iHydroKind2 = HYDROMETER_ACCUSONIC;
		else
			m_iHydroKind2 = HYDROMETER_CHANNELMASTER;
	}

	TWaitResult Result;

	while ( FormMain->m_bAniThread ) {

		Result = GAniEvent->WaitFor(500);	// Wait 1 second

		Synchronize(Animate);

		// FlowStream ------------------------------------------------------
/*
		if ( bFlowStream ) {
			TBitmap *pBitmap = new TBitmap();
			FormMain->ImageListFlow->GetBitmap(m_FlowStreamIndex,pBitmap);
			FormMain->ImageFlow->Picture->Bitmap = pBitmap;
			delete pBitmap;
			m_FlowStreamIndex++;

			if ( m_FlowStreamIndex > 8 ) m_FlowStreamIndex = 0;
		}

		// HydroMeter1 -----------------------------------------------------

		if ( bHydroMeter1) {
			TBitmap *pBitmap = new TBitmap();
			if ( iHydroKind1 == 1 )
				FormMain->ImageListCM->GetBitmap(m_HydroMeter1Index,pBitmap);
			else if ( iHydroKind1 == 2 )
				FormMain->ImageListSONTEK->GetBitmap(m_HydroMeter1Index,pBitmap);
			else
				FormMain->ImageListCM->GetBitmap(m_HydroMeter1Index,pBitmap);

			FormMain->ImageHydro1->Picture->Bitmap = pBitmap;
			delete pBitmap;
			m_HydroMeter1Index++;

			if ( m_HydroMeter1Index > 10 ) m_HydroMeter1Index = 0;
		}

		// HydroMeter2 -----------------------------------------------------

		if ( bHydroMeter2 ) {
			TBitmap *pBitmap = new TBitmap();
			if ( iHydroKind2 == 1 )
				FormMain->ImageListCM->GetBitmap(m_HydroMeter2Index,pBitmap);
			else if ( iHydroKind2 == 2 )
				FormMain->ImageListSONTEK->GetBitmap(m_HydroMeter2Index,pBitmap);
			else
				FormMain->ImageListCM->GetBitmap(m_HydroMeter2Index,pBitmap);

			FormMain->ImageHydro2->Picture->Bitmap = pBitmap;
			delete pBitmap;
			m_HydroMeter2Index++;

			if ( m_HydroMeter2Index > 10 ) m_HydroMeter2Index = 0;
		}

		// WaterLevel ------------------------------------------------------

		if ( bWaterLevel ) {
			TBitmap *pBitmap = new TBitmap();
			FormMain->ImageListWaterLevel->GetBitmap(m_WaterLevelIndex,pBitmap);
			FormMain->ImageWaterLevel->Picture->Bitmap = pBitmap;
			delete pBitmap;
			m_WaterLevelIndex++;

			if ( m_WaterLevelIndex > 3 ) m_WaterLevelIndex = 0;
		}
*/
		//Application->ProcessMessages();

		//Sleep(1000);	// 1 Sec Wait
	}
}

//---------------------------------------------------------------------------
void __fastcall TAniThread::Animate()
{
	if ( m_bFlowStream ) {
		TBitmap *pBitmap = new TBitmap();
		FormMain->ImageListFlow->GetBitmap(m_FlowStreamIndex,pBitmap);
		FormMain->ImageFlow->Picture->Bitmap = pBitmap;
		delete pBitmap;
		m_FlowStreamIndex = m_FlowStreamIndex + 1;

		if ( m_FlowStreamIndex > enFlowStreamAni8 ) m_FlowStreamIndex = enFlowStreamAni0;
	}

	// HydroMeter1 -----------------------------------------------------

	if ( m_bHydroMeter1) {
		TBitmap *pBitmap = new TBitmap();
		if ( m_iHydroKind1 == HYDROMETER_CHANNELMASTER )
			FormMain->ImageListCM->GetBitmap(m_HydroMeter1Index,pBitmap);
		else if ( m_iHydroKind1 == HYDROMETER_SLAG || m_iHydroKind1 == HYDROMETER_ACCUSONIC )
			FormMain->ImageListSONTEK->GetBitmap(m_HydroMeter1Index,pBitmap);
		else
			FormMain->ImageListCM->GetBitmap(m_HydroMeter1Index,pBitmap);

		FormMain->ImageHydro1->Picture->Bitmap = pBitmap;
		delete pBitmap;
		m_HydroMeter1Index = m_HydroMeter1Index + 1;

		if ( m_HydroMeter1Index > enHydroMeterAni10 ) m_HydroMeter1Index = enHydroMeterAni0;
	}

	// HydroMeter2 -----------------------------------------------------

	if ( m_bHydroMeter2 ) {
		TBitmap *pBitmap = new TBitmap();
		if ( m_iHydroKind2 == HYDROMETER_CHANNELMASTER )
			FormMain->ImageListCM->GetBitmap(m_HydroMeter2Index,pBitmap);
		else if ( m_iHydroKind2 == HYDROMETER_SLAG || m_iHydroKind1 == HYDROMETER_ACCUSONIC  )
			FormMain->ImageListSONTEK->GetBitmap(m_HydroMeter2Index,pBitmap);
		else
			FormMain->ImageListCM->GetBitmap(m_HydroMeter2Index,pBitmap);

		FormMain->ImageHydro2->Picture->Bitmap = pBitmap;
		delete pBitmap;
		m_HydroMeter2Index = m_HydroMeter2Index + 1;

		if ( m_HydroMeter2Index > enHydroMeterAni10 ) m_HydroMeter2Index = enHydroMeterAni0;
	}

	// WaterLevel ------------------------------------------------------

	if ( m_bWaterLevel ) {
		TBitmap *pBitmap = new TBitmap();
		FormMain->ImageListWaterLevel->GetBitmap(m_WaterLevelIndex,pBitmap);
		FormMain->ImageWaterLevel->Picture->Bitmap = pBitmap;
		delete pBitmap;
		m_WaterLevelIndex = m_WaterLevelIndex + 1;

		if ( m_WaterLevelIndex > enWaterLevelAni3 ) m_WaterLevelIndex = enWaterLevelAni0;
	}
}

