//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitSetup.h"
#include "UnitDM.h"
#include "UnitEnv.h"
#include "./Database/RSetup.h"
#include "./Database/RFlowADVM.h"
#include "./Database/RFlowHA.h"
#include "./Database/RFlowIVM.h"
#include "UnitMain.h"

#include <Vcl.FileCtrl.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormSetup *FormSetup;

//---------------------------------------------------------------------------
__fastcall TFormSetup::TFormSetup(TComponent* Owner)
	: TForm(Owner)
{
	m_bChanged = false;
	m_iPingCnt = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::PageSetupChange(TObject *Sender)
{
	if ( PageSetup->ActivePage == TabSheetPointInfo ) {

		ClearPoint();
		LoadPointFromDB();

	} else if ( PageSetup->ActivePage == TabSheetLoggerInfo ) {

		ClearLogger();
		LoadLoggerFromDB();

	} else if ( PageSetup->ActivePage == TabSheetFlowStream ) {

		ClearFlowStream();
		LoadFlowStreamFromDB();

		PageControlFlowStream->ActivePage = tsADVM;	// Set First Tabsheet

	} else if ( PageSetup->ActivePage == TabSheetEtc ) {

		ClearEtc();
		LoadEtcFromDB();

	} else {
		// nothing to do..
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbSavePointClick(TObject *Sender)
{
	bool bResult = false;
	bResult = CheckPoint();

	if ( !bResult ) {
		ShowMessage("입력 자료에 오류가 있습니다.");
		return;
	}

	bResult = SavePointToDB();

	if ( bResult ) {
		ShowMessage("Ok");
		m_bChanged = true;
		LogMessage("Point Setup data is saved..", bGCSFileLog, bGCSDBLog);
	} else {
		ShowMessage("Error");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbSaveLoggerClick(TObject *Sender)
{
	bool bResult = false;
	bResult = CheckLogger();

	if ( !bResult ) {
		ShowMessage("입력 자료에 오류가 있습니다.");
		return;
	}

	bResult = SaveLoggerToDB();

	if ( bResult ) {
		ShowMessage("Ok");
		m_bChanged = true;
		LogMessage("Logger Setup data is saved..", bGCSFileLog, bGCSDBLog);
	} else {
		ShowMessage("Error");
	}
}


void __fastcall TFormSetup::cbSaveFlowStreamClick(TObject *Sender)
{
	bool bResult = false;
	bResult = CheckFlowStream();

	if ( !bResult ) {
		ShowMessage("입력 자료에 오류가 있습니다.");
		return;
	}

	bResult = SaveFlowStreamToDB();

	if ( bResult ) {
		ShowMessage("Ok");
		m_bChanged = true;
		LogMessage("FlowStream Setup data is saved..", bGCSFileLog, bGCSDBLog);
	} else {
		ShowMessage("Error");
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbSaveEtcClick(TObject *Sender)
{
	bool bResult = false;
	bResult = CheckEtc();

	if ( !bResult ) {
		ShowMessage("입력 자료에 오류가 있습니다.");
		return;
	}

	bResult = SaveEtcToDB();

	if ( bResult ) {
		ShowMessage("Ok");
		m_bChanged = true;
		LogMessage("Etc Setup data is saved..", bGCSFileLog, bGCSDBLog);
	} else {
		ShowMessage("Error");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbCloseClick(TObject *Sender)
{
	if ( !m_bChanged ) {	// 변경 내역이 없으면 바로 닫는다.
		ModalResult = mrCancel;
		Close();
	} else {	// 변경 내역이 존재하면 .

		ShowMessage("변경 내역이 반영되려면 프로그램을 종료 후 재시작하여 주십시오.");
		ModalResult = mrOk;
		Close();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::SpeedButton1Click(TObject *Sender)
{
    TLabeledEdit *led = NULL;
    switch(((TSpeedButton *)Sender)->Tag)
    {
        case 1:
			led = edRADSHome;
            break;
        case 2:
			led = edLoggerHome;
            break;
        case 3:
			led = edSenderHome;
            break;
        case 4:
            led = edViewerHome;
            break;
    } // switch

    UnicodeString sDir = led->Text;
	if (SelectDirectory(sDir, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt, 1000)) led->Text = sDir;
}

//---------------------------------------------------------------------------
void TFormSetup::ClearPoint()
{
	edPointCode->Text     = "";
	edPointName->Text     = "";
	edPointAddress->Text  = "";
	edZipCode->Text       = "";
	edManager->Text       = "";
	edManagerTel->Text    = "";
	edEtc->Text           = "";
}

//---------------------------------------------------------------------------
void TFormSetup::ClearLogger()
{
	meInterval->Text        = "00:00";
	mePeriod->Text          = "00:00";
	cbxStartTime->ItemIndex = -1;

	// Hydro1 --------------------------------------------------------------
	cbxHydro1Type->ItemIndex     = -1;
	cbxHydro1ComPort->ItemIndex  = -1;
	cbxHydro1BaudRate->ItemIndex = -1;
	edHydro1Offset->Text         = "0.0";
	edHydro1Hz->Text             = "";
	edHydro1SN->Text             = "";
	edHydro1CellNo->Text         = "";
	edHydro1CellSize->Text       = "";
	edHydro1Blank->Text          = "";
	// Airmar
	cbxAirmar1ComPort->ItemIndex = -1;
	cbxAirmar1BaudRate->ItemIndex= -1;
	cbAirmarLinker->Checked      = false;	// Default: Airmar Sensor , checed => use AirmarLinker
	cbxHydro1Reverse->ItemIndex  = -1;
	cbxHydro1IdleCheck->ItemIndex= -1;
	cbxHydro1IdleCheckTime->ItemIndex = -1;
	edHydro1StartCell->Text      = "";
	edHydro1EndCell->Text        = "";
	// RQ-30d
	cbxSlaveCount->ItemIndex     = 0;	// SlaveCount => 0(default)
	edHydro1MasterAddress->Text  = "";  // (ex) 5,4,3,2,1
	cbxHydro1UseSlavePort->Checked = false;
	cbHydro1SlavePort->ItemIndex = -1;
	cbHydro1SlaveBaudRate->ItemIndex = -1;
	edHydro1SlaveAddress->Text   = "";	// (ex) 6,7,8,9,10

	// Hydor2
	cbxHydro2Type->ItemIndex     = -1;
	cbxHydro2ComPort->ItemIndex  = -1;
	cbxHydro2BaudRate->ItemIndex = -1;
	edHydro2Offset->Text         = "0.0";
	edHydro2Hz->Text             = "";
	edHydro2SN->Text             = "";
	edHydro2CellNo->Text         = "";
	edHydro2CellSize->Text       = "";
	edHydro2Blank->Text          = "";
	cbxHydro2Reverse->ItemIndex  = -1;
	cbxHydro2UpSide->ItemIndex   = 1;
	cbxHydro2IdleCheck->ItemIndex = -1;
	cbxHydro2IdleCheckTime->ItemIndex = -1;
	edHydro2StartCell->Text      = "";
	edHydro2EndCell->Text        = "";

	// WaterLevel
	cbxWaterType->ItemIndex      = -1;
	cbxWaterComPort->ItemIndex   = -1;
	cbxWaterBaudRate->ItemIndex  = -1;
	edWaterOffset->Text          = "0.0";
	lbAtmosCap->Visible          = false;
	cbxAtmosPort->Visible        = false;
	edBaseAtmos->Visible         = false;
	cbxWaterIdleCheck->ItemIndex = -1;
	cbxWaterIdleCheckTime->ItemIndex = -1;
}

//---------------------------------------------------------------------------
void TFormSetup::ClearFlowStream()
{
	cbxFlowStreamType->ItemIndex = -1;

	// FlowADVM Tab ------------------------------------------------------------

	// 하층 유속계
	AXadvm->Text        = "0";
	AZadvm->Text        = "0";
	APan->Text          = "0";
	ATilt->Text         = "0";
	APhi->Text          = "0";
	ASubmersion->Text   = "0";
	AReverseMode->ItemIndex  = 1;	// Default: ReverseOption => OFF ( 1 )
	AUnitVelocity->ItemIndex = 0;	// Default : mm/s
	AVMin->Text         = "0";
	AVMax->Text         = "0";
	AStartCell->Text    = "0";
	AEndCell->Text      = "0";

	// 하층 유속계
	BXadvm->Text        = "0";
	BZadvm->Text        = "0";
	BPan->Text          = "0";
	BTilt->Text         = "0";
	BPhi->Text          = "0";
	BSubmersion->Text   = "0";
	BReverseMode->ItemIndex  = 1;	// Default: ReverseOption => OFF ( 1 )
	BUnitVelocity->ItemIndex = 0;	// Default : mm/s
	BVMin->Text         = "0";
	BVMax->Text         = "0";
	BStartCell->Text    = "0";
	BEndCell->Text      = "0";
	BType->ItemIndex    = 0;		// Default: UP

	// FlowHA Tab --------------------------------------------------------------

	HAMinH->Text        = "0";
	HAMaxH->Text        = "0";
	HAA6->Text          = "0";
	HAA5->Text          = "0";
	HAA4->Text          = "0";
	HAA3->Text          = "0";
	HAA2->Text          = "0";
	HAA1->Text          = "0";
	HAA0->Text          = "0";

	// FlowIVM Tab -------------------------------------------------------------

	IVMAMinVi->Text      = "0";
	IVMAMaxVi->Text      = "0";
	IVMAA->Text          = "0";
	IVMAB->Text          = "0";
	IVMAC->Text          = "0";
	IVMBMinVi->Text      = "0";
	IVMBMaxVi->Text      = "0";
	IVMBA->Text          = "0";
	IVMBB->Text          = "0";
	IVMBC->Text          = "0";

	// RNSEA Tab -------------------------------------------------------------

	// Reserved..

}

//---------------------------------------------------------------------------
void TFormSetup::ClearEtc()
{
	edRADSHome->Text      = "";
	edLoggerHome->Text    = "";
	edSenderHome->Text    = "";
	edViewerHome->Text    = "";

	edLoggerIP->Text      = "";
	edSenderIP->Text      = "";
	edDatabaseIP->Text    = "";

	edLoggerPort->Text    = "";
	edSenderLoggerPort->Text = "";
	edSenderViewerPort->Text = "";
	edWatchDogPort->Text  = "";

	MemoPingLog->Lines->Clear();
	m_iPingCnt = 0;
	TimerPing->Enabled = false;
}

//---------------------------------------------------------------------------
bool TFormSetup::CheckPoint()
{
	if ( edPointCode->Text.IsEmpty() ) return false;
	if ( edPointCode->Text.Length() != 7 ) return false;
	if ( edPointName->Text.IsEmpty() ) return false;
	if ( edPointAddress->Text.IsEmpty() ) return false;

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::CheckLogger()
{
	if ( meInterval->Text.IsEmpty() || meInterval->Text.Length() > 5 ) return false;
	if ( mePeriod->Text.IsEmpty()   || mePeriod->Text.Length() > 5 )   return false;
	if ( cbxStartTime->ItemIndex == -1 ) return false;

	// Hydro1 --------------------------------------------------

	if ( cbxHydro1Type->ItemIndex == -1 )     return false;
	if ( cbxHydro1ComPort->ItemIndex == -1 )  return false;
	if ( cbxHydro1BaudRate->ItemIndex == -1 ) return false;
	//if ( edHydro1Offset->Text.IsEmpty() )     return false;
	if ( cbxHydro1Reverse->ItemIndex == -1 )  return false;
	if ( cbxHydro1IdleCheck->ItemIndex == -1 ) return false;
	if ( cbxHydro1IdleCheckTime->ItemIndex == -1 ) return false;
	//if ( edHydro1StartCell->Text.IsEmpty() )  return false;
	//if ( edHydro1EndCell->Text.IsEmpty() )    return false;

	// Check AG/SL Series
	if ( cbxHydro1Type->ItemIndex == 2 || cbxHydro1Type->ItemIndex == 3 ) {
		if ( edHydro1CellNo->Text.IsEmpty() )   return false;
		if ( edHydro1CellSize->Text.IsEmpty() ) return false;
		if ( edHydro1Blank->Text.IsEmpty() )    return false;
	}

	// Hydro2 --------------------------------------------------

	if ( cbxHydro2Type->ItemIndex == -1 )     return false;
	if ( cbxHydro2ComPort->ItemIndex == -1 )  return false;
	if ( cbxHydro2BaudRate->ItemIndex == -1 ) return false;
	//if ( edHydro2Offset->Text.IsEmpty() )     return false;
	if ( cbxHydro2Reverse->ItemIndex == -1 )  return false;
	if ( cbxHydro2IdleCheck->ItemIndex == -1 ) return false;
	if ( cbxHydro2IdleCheckTime->ItemIndex == -1 ) return false;
	//if ( edHydro2StartCell->Text.IsEmpty() )  return false;
	//if ( edHydro2EndCell->Text.IsEmpty() )    return false;

	// Check AG/SL Series
	if ( cbxHydro2Type->ItemIndex == 2 || cbxHydro2Type->ItemIndex == 3 ) {
		if ( edHydro2CellNo->Text.IsEmpty() )   return false;
		if ( edHydro2CellSize->Text.IsEmpty() ) return false;
		if ( edHydro2Blank->Text.IsEmpty() )    return false;
	}

	// WaterLevel -------------------------------------------------

	if ( cbxWaterType->ItemIndex > 0 ) {
		if ( cbxWaterType->Text.Pos("LT400") > 0 ) {
			if ( cbxAtmosPort->ItemIndex < 0 ) { 	// 2017.12.11 ItemIndex => 1 => 0으로 수정 => COM1로 설정 못하나?
				ShowMessage("LT400 수위계를 사용할 경우 대기압 보정용 포트를 지정하여야 합니다.");
				return false;
			}
			if ( edBaseAtmos->Text.IsEmpty() ) {
				ShowMessage("LT400 수위계를 사용할 경우 대기압 보정용 기준 대기압을 입력하여야 합니다.");
				return false;
			}
		}
		if ( cbxWaterIdleCheck->ItemIndex == -1 ) return false;
		if ( cbxWaterIdleCheckTime->ItemIndex == -1 ) return false;
	} else {
		// WaterLevel => NONE or not select
	}

	// HydroMeter1 check
	if ( cbxHydro1Type->ItemIndex == 4 || cbxHydro1Type->ItemIndex == 5 ) {	//  AccusonicA/AccusonicB이면 통신속도는 19200이어야 한다.
		if ( cbxHydro1BaudRate->ItemIndex != 4 ) {
			ShowMessage("Accusonic 유속계는 19200bps로 설정해야 합니다.");
			return false;
		}
	}
	// HydroMeter2 check
	if ( cbxHydro2Type->ItemIndex == 4 || cbxHydro2Type->ItemIndex == 5 ) {	//  AccusonicA/AccusonicB이면 통신속도는 19200이어야 한다.
		if ( cbxHydro2BaudRate->ItemIndex != 4 ) {
			ShowMessage("Accusonic 유속계는 19200bps로 설정해야 합니다.");
			return false;
		}
	}

	// RQ-30/RQ-30d Check
	if ( cbxHydro1Type->ItemIndex == 6 || cbxHydro1Type->ItemIndex == 7 ) {	// RQ-30/RQ-30d
		if ( cbxAirmar1ComPort->ItemIndex == -1 ) return false;	// 설정안함=> Error
		if ( cbxAirmar1ComPort->ItemIndex == 0 ) {	// NONE
			if ( Application->MessageBox(L"RQ-30 장비는 Airmar 장비와 운영됩니다. Airmar가 없는 상태로 운용할까요?", L"Airmar 설정", MB_YESNO) == IDYES ) {
				// nothing to do
			} else {
				return false;
			}
		} else {
			if ( cbAirmarLinker->Checked ) {	// AirmarLinker를 통해 통신, Airmar 바로 통신 => 4800bps
				if ( cbxAirmar1BaudRate->ItemIndex != 2 ) { // Index => 2 => 9600bps ; AirmarLinker는 9600bps로 통신
					ShowMessage("AirmarLinker를 이용할 경우 통신 속도는 9600bps로 설정 바랍니다.");
					return false;
				} else {}
			} else {	// Airmar Sensor와 바로 통신
				if ( cbxAirmar1BaudRate->ItemIndex != 1 ) { // Index => 1 => 4800bps ; Airmar sensor는 4800bps로 통신
					ShowMessage("Airmar sensor와 바로 연결된다면 통신속도는 4800bps입니다.");
					return false;
				} else {}
			}
		}
	}

	// RQ-30d Check (Slave Count)
	if ( cbxHydro1Type->ItemIndex == 7 ) {	// RQ-30d
		if ( cbxSlaveCount->ItemIndex == -1 ) return false;
		if ( cbxSlaveCount->ItemIndex == 0 ) {	// SlaveCount => 0
			if ( Application->MessageBox(L"RQ-30d는 RG-30d(Slave)와 운용됩니다. Slave 없이 운영할까요?", L"RG-30d(Slave설정)", MB_YESNO) == IDYES ) {
				// nothing to do
				//return true;
			} else {
				return false;
			}
		}
		// check other rule
		int           SlaveCount      = cbxSlaveCount->ItemIndex;	// 0,1,2,3,4,5,6,7,8,9 ( Master를 제외한 Slave의 숫사( RG-30d의 숫자) : Master Port + Slave Port
		UnicodeString MasterAddress   = edHydro1MasterAddress->Text;	// (ex) 5,4,3,2,1(좌안부터 기술)
		int           MasterPortCount = 0; // Master에 연결된 Device 숫자
		int           SlavePort       = cbHydro1SlavePort->ItemIndex;
		int           SlaveBaudRate   = cbHydro1SlaveBaudRate->ItemIndex;
		UnicodeString SlaveAddress    = edHydro1SlaveAddress->Text;	// (ex) 6,7,8,9,10 (ex) 좌안부터 기술..
		int           SlavePortCount  = 0;
		if ( SlaveCount >= 0 ) {
			if ( edHydro1MasterAddress->Text.IsEmpty() ){
				ShowMessage("RQ-30d Master Serial Port에 할당된 Device Address가 입력되지 않았습니다.");
				return false;
			}
			MasterPortCount = HowManyDeviceAddress(edHydro1MasterAddress->Text); 	// (ex)5,4,3,2,1

			if ( !cbxHydro1UseSlavePort->Checked ) {	// RQ-30d Master Port만 쓴다.
				if ( SlaveCount + 1 != MasterPortCount ) {
					ShowMessage("RQ-30d 입력된 Slave Count와 Slave Address Count가 일치하지 않습니다.");
					return false;
				} else {
					// 입력한 Device Address와 SlaveCount와 일치 => nothing to do..
				}
			} else {	// RQ-30d Master Port와 Slave Port : 시리얼을 두개로 분할 처리
				if ( cbHydro1SlavePort->ItemIndex == -1 ) {
					ShowMessage("RQ-30d Slave Port가 지정되지 않았습니다.");
					return false;
				}
				if ( cbHydro1SlaveBaudRate->ItemIndex == -1 ) {
					ShowMessage("RQ-30d Slave Port의 통신속도가 지정되지 않았습니다.");
					return false;
				}
				if ( edHydro1SlaveAddress->Text.IsEmpty() ) {
					ShowMessage("RQ-30d Slave Port에 연결되어 있는 장비의 Address가 지정되지 않았습니다.");
					return false;
				}
				SlavePortCount = HowManyDeviceAddress(edHydro1SlaveAddress->Text);
				if ( SlavePortCount <= 0 ) {
					ShowMessage("RQ-30d Slave Port의 장비 Address가 지정되지 않았습니다.");
					return false;
				}
			}

			// Master/Slave Port에 할당된 Device Address의 합 => SlaveCount +1과 같아야 한다.
			if ( MasterPortCount + SlavePortCount != SlaveCount + 1 ) {
				ShowMessage("RQ-30d의 Master Port/Slave Port의 Device Address와 SlaveCount가 같지 않습니다.");
				return false;
			} else {
				// Master/Slave Port에 할당단 Device Address => SlaveCount+1과 동일한 상태 => 정상 설정
				// nothing to to..
			}
		} else {
			// RQ-30d가 Slave 없이 운용 ..
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::CheckFlowStream()
{
	if ( cbxFlowStreamType->ItemIndex == -1 ) return false;

	if ( cbxFlowStreamType->ItemIndex == 0 ) return true;	// 자동유량 사용하지 않음

	if ( cbxFlowStreamType->ItemIndex == 1 ) {

		// FlowADVM Tab ------------------------------------------------------------

		// 하층 유속계
		if ( AXadvm->Text.IsEmpty() ) return false;
		if ( AZadvm->Text.IsEmpty() )      return false;
		if ( APan->Text.IsEmpty() )        return false;
		if ( ATilt->Text.IsEmpty() )       return false;
		if ( APhi->Text.IsEmpty() )        return false;
		if ( ASubmersion->Text.IsEmpty() ) return false;
		if ( AReverseMode->ItemIndex == -1)  return false;	// Default: ReverseOption => OFF ( 1 )
		if ( AUnitVelocity->ItemIndex == -1) return false;	// Default : mm/s
		if ( AVMin->Text.IsEmpty() )       return false;
		if ( AVMax->Text.IsEmpty() )       return false;
		if ( AStartCell->Text.IsEmpty() )  return false;
		if ( AEndCell->Text.IsEmpty() )    return false;

		// 하층 유속계
		if ( BXadvm->Text.IsEmpty() ) return false;
		if ( BZadvm->Text.IsEmpty() )      return false;
		if ( BPan->Text.IsEmpty() )        return false;
		if ( BTilt->Text.IsEmpty() )       return false;
		if ( BPhi->Text.IsEmpty() )        return false;
		if ( BSubmersion->Text.IsEmpty() ) return false;
		if ( BReverseMode->ItemIndex == -1) return false;	// Default: ReverseOption => OFF ( 1 )
		if ( BUnitVelocity->ItemIndex == -1) return false;	// Default : mm/s
		if ( BVMin->Text.IsEmpty() )      return false;
		if ( BVMax->Text.IsEmpty() )      return false;
		if ( BStartCell->Text.IsEmpty() ) return false;
		if ( BEndCell->Text.IsEmpty() )   return false;
		if ( BType->ItemIndex == -1) return false;		// Default: UP

		// FlowHA Tab --------------------------------------------------------------

		if ( HAMinH->Text.IsEmpty() ) return false;
		if ( HAMaxH->Text.IsEmpty() ) return false;
		if ( HAA6->Text.IsEmpty() )   return false;
		if ( HAA5->Text.IsEmpty() )   return false;
		if ( HAA4->Text.IsEmpty() )   return false;
		if ( HAA3->Text.IsEmpty() )   return false;
		if ( HAA2->Text.IsEmpty() )   return false;
		if ( HAA1->Text.IsEmpty() )   return false;
		if ( HAA0->Text.IsEmpty() )   return false;

		// FlowIVM Tab -------------------------------------------------------------

		if ( IVMAMinVi->Text.IsEmpty() ) return false;
		if ( IVMAMaxVi->Text.IsEmpty() ) return false;
		if ( IVMAA->Text.IsEmpty() )     return false;
		if ( IVMAB->Text.IsEmpty() )     return false;
		if ( IVMAC->Text.IsEmpty() )     return false;
		if ( IVMBMinVi->Text.IsEmpty() ) return false;
		if ( IVMBMaxVi->Text.IsEmpty() ) return false;
		if ( IVMBA->Text.IsEmpty() )     return false;
		if ( IVMBB->Text.IsEmpty() )     return false;
		if ( IVMBC->Text.IsEmpty() )     return false;

		return true;
	}

	if ( cbxFlowStreamType->ItemIndex == 2 ) {	// RNSEA 방식
		return true;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::CheckEtc()
{
	if (edRADSHome->Text.IsEmpty())   return false;
	if (edLoggerHome->Text.IsEmpty()) return false;
	if (edSenderHome->Text.IsEmpty()) return false;
	if (edViewerHome->Text.IsEmpty()) return false;

	if (edLoggerIP->Text.IsEmpty())   return false;
	if (edSenderIP->Text.IsEmpty())   return false;
	if (edDatabaseIP->Text.IsEmpty()) return false;

	if (edLoggerPort->Text.IsEmpty())       return false;
	if (edSenderLoggerPort->Text.IsEmpty()) return false;
	if (edSenderViewerPort->Text.IsEmpty()) return false;
	if (edWatchDogPort->Text.IsEmpty())     return false;

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::LoadPointFromDB()
{
	TSQLQuery *pQuery = NULL;
	TRSetup Setup;
	UnicodeString sSQL = " SELECT * FROM RSetup              " \
						 " WHERE ( PK1 = 1  AND PK2 = 1 )    " \
						 " ORDER BY PK1, PK2                 ";

	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) return false;

	while ( !pQuery->Eof ) {
		try {
			Setup.Clear();

			Setup.PK1     = pQuery->FieldByName("PK1")->AsInteger;
			Setup.PK2     = pQuery->FieldByName("PK2")->AsInteger;
			Setup.Value01 = pQuery->FieldByName("Value01")->AsString;
			Setup.Value02 = pQuery->FieldByName("Value02")->AsString;
			Setup.Value03 = pQuery->FieldByName("Value03")->AsString;
			Setup.Value04 = pQuery->FieldByName("Value04")->AsString;
			Setup.Value05 = pQuery->FieldByName("Value05")->AsString;
			Setup.Value06 = pQuery->FieldByName("Value06")->AsString;
			Setup.Value07 = pQuery->FieldByName("Value07")->AsString;
			Setup.Value08 = pQuery->FieldByName("Value08")->AsString;
			Setup.Value09 = pQuery->FieldByName("Value09")->AsString;
			Setup.Value10 = pQuery->FieldByName("Value10")->AsString;
			Setup.Value11 = pQuery->FieldByName("Value11")->AsString;
			Setup.Value12 = pQuery->FieldByName("Value12")->AsString;
			Setup.Value13 = pQuery->FieldByName("Value13")->AsString;
			Setup.Value14 = pQuery->FieldByName("Value14")->AsString;
			Setup.Value15 = pQuery->FieldByName("Value15")->AsString;
			Setup.Value16 = pQuery->FieldByName("Value16")->AsString;
			Setup.Value17 = pQuery->FieldByName("Value17")->AsString;
			Setup.Value18 = pQuery->FieldByName("Value18")->AsString;
			Setup.Value19 = pQuery->FieldByName("Value19")->AsString;
			Setup.Value20 = pQuery->FieldByName("Value20")->AsString;
			Setup.Des     = pQuery->FieldByName("Des")->AsString;

			pQuery->Next();

		} catch ( Exception &e ) {
			//theLog.LogMessage(e.Message);
		}
	}
	DM->Release(pQuery);

	edPointCode->Text     = Setup.Value01;
	edPointName->Text     = Setup.Value02;
	edPointAddress->Text  = Setup.Value03;
	edZipCode->Text       = Setup.Value04;
	edManager->Text       = Setup.Value05;
	edManagerTel->Text    = Setup.Value06;
	edEtc->Text           = Setup.Value07;

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::LoadLoggerFromDB()
{
	TSQLQuery *pQuery = NULL;
	TRSetup Setup;
	TRSetup FlowStream, LoggerInfo, HydroMeter1, HydroMeter2, WaterLevel;
	UnicodeString sSQL = " SELECT * FROM RSetup               "
						 " WHERE ( PK1 = 1   AND PK2 = 3 ) OR "
						 "       ( PK1 = 10  AND PK2 = 1 ) OR "
						 "       ( PK1 = 10  AND PK2 = 2 ) OR "
						 "       ( PK1 = 10  AND PK2 = 3 ) OR "
						 "       ( PK1 = 10  AND PK2 = 4 )    "
						 " ORDER BY PK1, PK2                  ";

	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) return false;

	while ( !pQuery->Eof ) {
		try {
			Setup.Clear();

			Setup.PK1     = pQuery->FieldByName("PK1")->AsInteger;
			Setup.PK2     = pQuery->FieldByName("PK2")->AsInteger;
			Setup.Value01 = pQuery->FieldByName("Value01")->AsString;
			Setup.Value02 = pQuery->FieldByName("Value02")->AsString;
			Setup.Value03 = pQuery->FieldByName("Value03")->AsString;
			Setup.Value04 = pQuery->FieldByName("Value04")->AsString;
			Setup.Value05 = pQuery->FieldByName("Value05")->AsString;
			Setup.Value06 = pQuery->FieldByName("Value06")->AsString;
			Setup.Value07 = pQuery->FieldByName("Value07")->AsString;
			Setup.Value08 = pQuery->FieldByName("Value08")->AsString;
			Setup.Value09 = pQuery->FieldByName("Value09")->AsString;
			Setup.Value10 = pQuery->FieldByName("Value10")->AsString;
			Setup.Value11 = pQuery->FieldByName("Value11")->AsString;
			Setup.Value12 = pQuery->FieldByName("Value12")->AsString;
			Setup.Value13 = pQuery->FieldByName("Value13")->AsString;
			Setup.Value14 = pQuery->FieldByName("Value14")->AsString;
			Setup.Value15 = pQuery->FieldByName("Value15")->AsString;
			Setup.Value16 = pQuery->FieldByName("Value16")->AsString;
			Setup.Value17 = pQuery->FieldByName("Value17")->AsString;
			Setup.Value18 = pQuery->FieldByName("Value18")->AsString;
			Setup.Value19 = pQuery->FieldByName("Value19")->AsString;
			Setup.Value20 = pQuery->FieldByName("Value20")->AsString;
			Setup.Des     = pQuery->FieldByName("Des")->AsString;

			if ( Setup.PK1 == 1  && Setup.PK2 == 3 ) FlowStream  = Setup;
			if ( Setup.PK1 == 10 && Setup.PK2 == 1 ) LoggerInfo  = Setup;
			if ( Setup.PK1 == 10 && Setup.PK2 == 2 ) HydroMeter1 = Setup;
			if ( Setup.PK1 == 10 && Setup.PK2 == 3 ) HydroMeter2 = Setup;
			if ( Setup.PK1 == 10 && Setup.PK2 == 4 ) WaterLevel  = Setup;

			pQuery->Next();

		} catch ( Exception &e ) {
			//theLog.LogMessage(e.Message);
		}
	}
	DM->Release(pQuery);

	meInterval->Text        = LoggerInfo.Value01;
	mePeriod->Text          = LoggerInfo.Value02;
	cbxStartTime->ItemIndex = StrToInt(LoggerInfo.Value03);

	//cbxFlowStreamType->ItemIndex = StrToInt(FlowStream.Value02);

	// Hydro1
	UnicodeString HydroType1 = HydroMeter1.Value01;
	if ( HydroMeter1.Value01 == RQ30_STR )  HydroType1 = "RQ-30";
	if ( HydroMeter1.Value01 == RQ30D_STR ) HydroType1 = "RQ-30d";
	cbxHydro1Type->ItemIndex     = cbxHydro1Type->Items->IndexOf(HydroType1);
	if ( cbxHydro1Type->ItemIndex == 0 ) { 	// NONE
		Panel1->Visible      = false;	// SL/AG Series
		PanelAirmar->Visible = false;	// Airmar
		PanelRQ30d->Visible  = false;	// RQ-30d
	} else if( cbxHydro1Type->ItemIndex == 2 || cbxHydro1Type->ItemIndex == 3 ) {
		Panel1->Visible      = true;	// SL/AG Series
		PanelAirmar->Visible = false;
		PanelRQ30d->Visible  = false;
	} else if ( cbxHydro1Type->ItemIndex == 6 ) {	// RQ-30
		Panel1->Visible      = false;	// SL/AG Series
		PanelAirmar->Visible = true;	// Airmar
		PanelRQ30d->Visible  = false;
	} else if ( cbxHydro1Type->ItemIndex == 7 ) {	// RQ-30d
		Panel1->Visible      = false;	// SL/AG Series
		PanelAirmar->Visible = true;	// Airmar
		PanelRQ30d->Visible  = true;
	} else {
		Panel1->Visible      = false;	// CM, AccusonicA/B
		PanelAirmar->Visible = false;
		PanelRQ30d->Visible  = false;
	}
	cbxHydro1ComPort->ItemIndex  = cbxHydro1ComPort->Items->IndexOf(HydroMeter1.Value02);
	cbxHydro1BaudRate->ItemIndex = cbxHydro1BaudRate->Items->IndexOf(HydroMeter1.Value03);
	edHydro1Offset->Text         = HydroMeter1.Value04;
	cbxHydro1Reverse->ItemIndex  = (HydroMeter1.Value10 == "OFF") ? 1 : 0;
	if ( HydroMeter1.Value01 == AGSERIES_STR || HydroMeter1.Value01 == SLSERIES_STR ) {
		edHydro1CellNo->Text   = HydroMeter1.Value07;
		edHydro1CellSize->Text = HydroMeter1.Value08;
		edHydro1Blank->Text    = HydroMeter1.Value09;
	} else if ( HydroMeter1.Value01 == RQ30_STR ) {
		cbxAirmar1ComPort->ItemIndex  = cbxAirmar1ComPort->Items->IndexOf(HydroMeter1.Value16);   	// Port
		cbxAirmar1BaudRate->ItemIndex = cbxAirmar1BaudRate->Items->IndexOf(HydroMeter1.Value17); 	// BaudRate
		cbAirmarLinker->Checked       = (HydroMeter1.Value20 == "ON") ? true : false;				// ON=>AirmarLinker 사용
	} else if ( HydroMeter1.Value01 == RQ30D_STR ) {
		cbxAirmar1ComPort->ItemIndex  = cbxAirmar1ComPort->Items->IndexOf(HydroMeter1.Value16);   	// Port
		cbxAirmar1BaudRate->ItemIndex = cbxAirmar1BaudRate->Items->IndexOf(HydroMeter1.Value17); 	// BaudRate
		cbAirmarLinker->Checked       = (HydroMeter1.Value20 == "ON") ? true : false;				// ON=>AirmarLinker 사용
		// RQ-30d Section. 2024.09.11
		cbxSlaveCount->ItemIndex      = (HydroMeter1.Value13.IsEmpty()) ? 0 : StrToInt(HydroMeter1.Value13);	// SlaveCount 1 => ItemIndex 1
		edHydro1MasterAddress->Text   = HydroMeter1.Value05; // Master Port Device Address (ex) 5,4,3,2,1(좌안부터)
		cbxHydro1UseSlavePort->Checked= (HydroMeter1.Value06 == "ON") ? true : false;
		cbHydro1SlavePort->ItemIndex  = cbHydro1SlavePort->Items->IndexOf(HydroMeter1.Value07);  	  // COM01/COM02/...
		cbHydro1SlaveBaudRate->ItemIndex= cbHydro1SlaveBaudRate->Items->IndexOf(HydroMeter1.Value08); // 2400,4800,..
		edHydro1SlaveAddress->Text    = HydroMeter1.Value09; // Slave Port Device Address (ex) 6,7,8,9,10(좌안부터)
	} else {
		// nothing to do..
	}
	edHydro1Hz->Text             = HydroMeter1.Value11;
	edHydro1SN->Text             = HydroMeter1.Value12;
	cbxHydro1IdleCheck->ItemIndex= (HydroMeter1.Value14.IsEmpty() || HydroMeter1.Value14 == "ON" ) ? 0 : 1; // ON/OFF
	cbxHydro1IdleCheckTime->ItemIndex = (HydroMeter1.Value15.IsEmpty()) ? 2 : (StrToInt(HydroMeter1.Value15)-1); // 1 hour ~ 6 hours (default) 3 hours
	//heaven2 2022.03.18
	edHydro1StartCell->Text      = (HydroMeter1.Value18.IsEmpty()) ? UnicodeString("1") : HydroMeter1.Value18;	// Default : 1(
	edHydro1EndCell->Text        = (HydroMeter1.Value19.IsEmpty()) ? UnicodeString("10") : HydroMeter1.Value19;	// Default : 1(

	// Hydro2
	UnicodeString HydroType2 = HydroMeter2.Value01;
	if ( HydroMeter2.Value01 == RQ30_STR )  HydroType2 = "RQ-30";
	if ( HydroMeter2.Value01 == RQ30D_STR ) HydroType2 = "RQ-30d";
	cbxHydro2Type->ItemIndex     = cbxHydro2Type->Items->IndexOf(HydroType2);
	if ( cbxHydro2Type->ItemIndex == 0 ) { // NONE
		Panel2->Visible       = false;	// SL/AG Series
	} else if( cbxHydro2Type->ItemIndex == 2 || cbxHydro2Type->ItemIndex == 3 ) {
		Panel2->Visible       = true;	// SL/AG Series
		//PanelAirmar2->Visible = false;
	} else if ( cbxHydro2Type->ItemIndex == 6 ) {	// RQ-30
		Panel2->Visible       = false;	// SL/AG Series
		//PanelAirmar2->Visible = true;
	} else {
		Panel2->Visible       = false;	// SL/AG Series
		//PanelAirmar2->Visible = false;
	}
	cbxHydro2ComPort->ItemIndex  = cbxHydro2ComPort->Items->IndexOf(HydroMeter2.Value02);
	cbxHydro2BaudRate->ItemIndex = cbxHydro2BaudRate->Items->IndexOf(HydroMeter2.Value03);
	edHydro2Offset->Text         = HydroMeter2.Value04;
	cbxHydro2Reverse->ItemIndex  = (HydroMeter2.Value10 == "OFF")  ? 1 : 0;
	cbxHydro2UpSide->ItemIndex   = (HydroMeter2.Value11 == "SIDE") ? 1 : 0;
	if ( HydroMeter2.Value01 == AGSERIES_STR || HydroMeter2.Value01 == SLSERIES_STR ) {
		edHydro2CellNo->Text   = HydroMeter2.Value07;
		edHydro2CellSize->Text = HydroMeter2.Value08;
		edHydro2Blank->Text    = HydroMeter2.Value09;
	} else if ( HydroMeter2.Value01 == RQ30_STR ) {
//		cbxAirmar2ComPort->ItemIndex  = cbxAirmar2ComPort->Items->IndexOf(HydroMeter2.Value16); 	// Port
//		cbxAirmar2BaudRate->ItemIndex = cbxAirmar2BaudRate->Items->IndexOf(HydroMeter2.Value17);	// BaudRate
	} else {
		// nothing to do..
	}
	edHydro2Hz->Text             = HydroMeter2.Value12;
	edHydro2SN->Text             = HydroMeter2.Value13;
	cbxHydro2IdleCheck->ItemIndex = (HydroMeter2.Value14.IsEmpty() || HydroMeter2.Value14 == "ON" ) ? 0 : 1; // ON/OFF
	cbxHydro2IdleCheckTime->ItemIndex = (HydroMeter2.Value15.IsEmpty()) ? 2 : (StrToInt(HydroMeter2.Value15)-1); // 1 hour ~ 6 hours (default) 3 hours
	//heaven2 2022.03.18
	edHydro2StartCell->Text      = (HydroMeter2.Value18.IsEmpty()) ? UnicodeString("1") : HydroMeter2.Value18;	// Default : 1(
	edHydro2EndCell->Text        = (HydroMeter2.Value19.IsEmpty()) ? UnicodeString("10") : HydroMeter2.Value19;	// Default : 1(

	// WaterLevel
	cbxWaterType->ItemIndex      = cbxWaterType->Items->IndexOf(WaterLevel.Value01);
	cbxWaterComPort->ItemIndex   = cbxWaterComPort->Items->IndexOf(WaterLevel.Value02);
	cbxWaterBaudRate->ItemIndex  = cbxWaterBaudRate->Items->IndexOf(WaterLevel.Value03);
	edWaterOffset->Text          = WaterLevel.Value04;
	edWaterSN->Text              = WaterLevel.Value06;
	// 2015.11.16
	if ( WaterLevel.Value01.Pos(LT400_STR) > 0 ) { // LT400
		cbxWaterType->ItemIndex = 2; // LT400+Atmos
		if ( WaterLevel.Value07.IsEmpty() ) WaterLevel.Value07 = "NONE";	// Init Value
		cbxAtmosPort->ItemIndex = cbxAtmosPort->Items->IndexOf(WaterLevel.Value07);
		edBaseAtmos->Text       = WaterLevel.Value08;
		lbAtmosCap->Visible     = true;
		cbxAtmosPort->Visible   = true;
		edBaseAtmos->Visible    = true;
	} else { // 다른 장비 보여줄 때 감춘다.
		cbxAtmosPort->ItemIndex = 0;
		edBaseAtmos->Text       = 0;
		lbAtmosCap->Visible     = false;
		cbxAtmosPort->Visible   = false;
		edBaseAtmos->Visible    = false;
	}
	cbxWaterIdleCheck->ItemIndex = (WaterLevel.Value09.IsEmpty() || WaterLevel.Value09 == "ON" ) ? 0 : 1; // ON/OFF
	cbxWaterIdleCheckTime->ItemIndex = (WaterLevel.Value10.IsEmpty()) ? 2 : (StrToInt(WaterLevel.Value10)-1); // 1 hour ~ 6 hours (default) 3 hours

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::LoadFlowStreamFromDB()
{
	bool bResult = false;

	TRFlowADVM FlowADVM;
	TRFlowHA   FlowHA;
	TRFlowIVM  FlowIVM;

	FlowADVM.PointCode = theEnv.m_PointCode;
	FlowHA.PointCode   = theEnv.m_PointCode;
	FlowIVM.PointCode  = theEnv.m_PointCode;

	bResult = FlowADVM.GetData();
	if ( !bResult ) return bResult;

	bResult = FlowHA.GetData();
	if ( !bResult ) return bResult;

	bResult = FlowIVM.GetData();
	if ( !bResult ) return bResult;

	TRSetup FlowStream;
	FlowStream.PK1 = 1; FlowStream.PK2 = 3;	// Flow Stream Option
	FlowStream.GetData();

	cbxFlowStreamType->ItemIndex = StrToInt(FlowStream.Value02);
	if ( cbxFlowStreamType->ItemIndex == 0 ) {
		PageControlFlowStream->Visible = false;
	} else {
		PageControlFlowStream->Visible = true;
	}

	// FlowADVM Tab ------------------------------------------------------------

	UnicodeString sTemp = "";

	// 하층 유속계
	AXadvm->Text        = sTemp.sprintf(L"%.2f", FlowADVM.AXadvm);
	AZadvm->Text        = sTemp.sprintf(L"%.2f", FlowADVM.AZadvm);
	APan->Text          = StrToInt(FlowADVM.APan);
	ATilt->Text         = StrToInt(FlowADVM.ATilt);
	APhi->Text          = sTemp.sprintf(L"%.1f", FlowADVM.APhi);
	ASubmersion->Text   = sTemp.sprintf(L"%.2f", FlowADVM.ASubmersion);
	AReverseMode->ItemIndex  = FlowADVM.AReverseMode;	// 0: ON, 1: OFF
	AUnitVelocity->ItemIndex = FlowADVM.AUnitVelocity;	// 0: mm/s, 1:cm/s, 2: m/s
	AVMin->Text         = sTemp.sprintf(L"%.3f", FlowADVM.AVMin);
	AVMax->Text         = sTemp.sprintf(L"%.3f", FlowADVM.AVMax);
	AStartCell->Text    = StrToInt(FlowADVM.AStartCell);
	AEndCell->Text      = StrToInt(FlowADVM.AEndCell);

	// 하층 유속계
	BXadvm->Text        = sTemp.sprintf(L"%.2f", FlowADVM.BXadvm);
	BZadvm->Text        = sTemp.sprintf(L"%.2f", FlowADVM.BZadvm);
	BPan->Text          = StrToInt(FlowADVM.BPan);
	BTilt->Text         = StrToInt(FlowADVM.BTilt);
	BPhi->Text          = sTemp.sprintf(L"%.1f", FlowADVM.BPhi);
	BSubmersion->Text   = sTemp.sprintf(L"%.2f", FlowADVM.BSubmersion);
	BReverseMode->ItemIndex  = FlowADVM.BReverseMode;	// 0: ON, 1: OFF
	BUnitVelocity->ItemIndex = FlowADVM.BUnitVelocity;	// 0: mm/s, 1:cm/s, 2: m/s
	BVMin->Text         = sTemp.sprintf(L"%.3f", FlowADVM.BVMin);
	BVMax->Text         = sTemp.sprintf(L"%.3f", FlowADVM.BVMax);
	BStartCell->Text    = StrToInt(FlowADVM.BStartCell);
	BEndCell->Text      = StrToInt(FlowADVM.BEndCell);
	BType->ItemIndex    = (FlowADVM.BType == "UP") ? 0 : ((FlowADVM.BType == "LEFTRIGHT") ? 1 : 2);

	// FlowHA Tab --------------------------------------------------------------

	HAMinH->Text        = sTemp.sprintf(L"%.2f", FlowHA.MinH);
	HAMaxH->Text        = sTemp.sprintf(L"%.2f", FlowHA.MaxH);
	HAA6->Text          = sTemp.sprintf(L"%.3f", FlowHA.A6);
	HAA5->Text          = sTemp.sprintf(L"%.3f", FlowHA.A5);
	HAA4->Text          = sTemp.sprintf(L"%.3f", FlowHA.A4);
	HAA3->Text          = sTemp.sprintf(L"%.3f", FlowHA.A3);
	HAA2->Text          = sTemp.sprintf(L"%.3f", FlowHA.A2);
	HAA1->Text          = sTemp.sprintf(L"%.3f", FlowHA.A1);
	HAA0->Text          = sTemp.sprintf(L"%.3f", FlowHA.A0);

	// FlowIVM Tab -------------------------------------------------------------

	IVMAMinVi->Text      = sTemp.sprintf(L"%.2f", FlowIVM.AMinVi);
	IVMAMaxVi->Text      = sTemp.sprintf(L"%.2f", FlowIVM.AMaxVi);
	IVMAA->Text          = sTemp.sprintf(L"%.2f", FlowIVM.AA);
	IVMAB->Text          = sTemp.sprintf(L"%.2f", FlowIVM.AB);
	IVMAC->Text          = sTemp.sprintf(L"%.2f", FlowIVM.AC);
	IVMBMinVi->Text      = sTemp.sprintf(L"%.2f", FlowIVM.BMinVi);
	IVMBMaxVi->Text      = sTemp.sprintf(L"%.2f", FlowIVM.BMaxVi);
	IVMBA->Text          = sTemp.sprintf(L"%.2f", FlowIVM.BA);
	IVMBB->Text          = sTemp.sprintf(L"%.2f", FlowIVM.BB);
	IVMBC->Text          = sTemp.sprintf(L"%.2f", FlowIVM.BC);

	// RNSEA Tab -------------------------------------------------------------

	// Reserved..

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::LoadEtcFromDB()
{
	TSQLQuery *pQuery = NULL;
	TRSetup Setup;
	UnicodeString sSQL = " SELECT * FROM RSetup              " \
						 " WHERE ( PK1 = 1  AND PK2 = 2 )    " \
						 " ORDER BY PK1, PK2                 ";

	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) return false;

	while ( !pQuery->Eof ) {
		try {
			Setup.Clear();

			Setup.PK1     = pQuery->FieldByName("PK1")->AsInteger;
			Setup.PK2     = pQuery->FieldByName("PK2")->AsInteger;
			Setup.Value01 = pQuery->FieldByName("Value01")->AsString;
			Setup.Value02 = pQuery->FieldByName("Value02")->AsString;
			Setup.Value03 = pQuery->FieldByName("Value03")->AsString;
			Setup.Value04 = pQuery->FieldByName("Value04")->AsString;
			Setup.Value05 = pQuery->FieldByName("Value05")->AsString;
			Setup.Value06 = pQuery->FieldByName("Value06")->AsString;
			Setup.Value07 = pQuery->FieldByName("Value07")->AsString;
			Setup.Value08 = pQuery->FieldByName("Value08")->AsString;
			Setup.Value09 = pQuery->FieldByName("Value09")->AsString;
			Setup.Value10 = pQuery->FieldByName("Value10")->AsString;
			Setup.Value11 = pQuery->FieldByName("Value11")->AsString;
			Setup.Value12 = pQuery->FieldByName("Value12")->AsString;
			Setup.Value13 = pQuery->FieldByName("Value13")->AsString;
			Setup.Value14 = pQuery->FieldByName("Value14")->AsString;
			Setup.Value15 = pQuery->FieldByName("Value15")->AsString;
			Setup.Value16 = pQuery->FieldByName("Value16")->AsString;
			Setup.Value17 = pQuery->FieldByName("Value17")->AsString;
			Setup.Value18 = pQuery->FieldByName("Value18")->AsString;
			Setup.Value19 = pQuery->FieldByName("Value19")->AsString;
			Setup.Value20 = pQuery->FieldByName("Value20")->AsString;
			Setup.Des     = pQuery->FieldByName("Des")->AsString;

			pQuery->Next();

		} catch ( Exception &e ) {
			//theLog.LogMessage(e.Message);
		}
	}
	DM->Release(pQuery);

	edRADSHome->Text      = Setup.Value01;
	edLoggerHome->Text    = Setup.Value02;
	edSenderHome->Text    = Setup.Value03;
	edViewerHome->Text    = Setup.Value04;

	edLoggerIP->Text      = Setup.Value05;
	edSenderIP->Text      = Setup.Value06;
	edDatabaseIP->Text    = Setup.Value07;

	edLoggerPort->Text    = Setup.Value08;
	edSenderLoggerPort->Text = Setup.Value09;
	edSenderViewerPort->Text = Setup.Value10;
	edWatchDogPort->Text  = Setup.Value11;

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::SavePointToDB()
{
	bool bCheck = false;
	bCheck = CheckPoint();
	if ( !bCheck ) {
		ShowMessage("입력 정보에 오류가 있습니다.");
		return false;
	}

	TRSetup OldSetup;
	OldSetup.PK1 = 1;
	OldSetup.PK2 = 1;

	bool bResult = false;
	bResult = OldSetup.GetData();	// Old PointCode
	if ( !bResult ) return bResult;

	TRSetup Setup;
	Setup.PK1 = 1;
	Setup.PK2 = 1;

	Setup.Value01 = edPointCode->Text;
	Setup.Value02 = edPointName->Text;
	Setup.Value03 = edPointAddress->Text;
	Setup.Value04 = edZipCode->Text;
	Setup.Value05 = edManager->Text;
	Setup.Value06 = edManagerTel->Text;
	Setup.Value07 = edEtc->Text;

	bResult = Setup.Update();

 	if ( OldSetup.Value01 != Setup.Value01 ) { 	// PointCode가 갱신되었다면 ..
		TRFlowADVM FlowADVM;
		TRFlowHA   FlowHA;
		TRFlowIVM  FlowIVM;

		FlowADVM.PointCode = OldSetup.Value01;
		FlowHA.PointCode   = OldSetup.Value01;
		FlowIVM.PointCode  = OldSetup.Value01;

		FlowADVM.GetData();
		FlowHA.GetData();
		FlowIVM.GetData();

		FlowADVM.UpdatePointCode(Setup.Value01);
		FlowHA.UpdatePointCode(Setup.Value01);
		FlowIVM.UpdatePointCode(Setup.Value01);
	}
	return bResult;
}

//---------------------------------------------------------------------------
bool TFormSetup::SaveLoggerToDB()
{
	bool bCheck = false;

//	bCheck = CheckLogger();
//	if ( !bCheck ) {
//		ShowMessage("입력 정보에 오류가 있습니다.");
//		return false;
//	}

	TRSetup FlowStream, LoggerInfo, HydroMeter1, HydroMeter2, WaterLevel;
	bool bResult = false;

	// Logger Info--------------------------------------------------------------

	LoggerInfo.PK1 = 10;
	LoggerInfo.PK2 = 1;

	LoggerInfo.Value01 = meInterval->Text;
	LoggerInfo.Value02 = mePeriod->Text;
	LoggerInfo.Value03 = cbxStartTime->Text;

	bResult = LoggerInfo.Update();
	if ( !bResult ) {
		ShowMessage("Logger 정보 저장 시 오류가 발생하였습니다.");
		return false;
	}

	// FlowStream --------------------------------------------------------------

//	FlowStream.PK1 = 1;
//	FlowStream.PK2 = 3;
//
//	FlowStream.Value02 = cbxFlowStreamType->ItemIndex ; // 0:Off, 1:유량사업단, 2:RNSEA
//	bResult = FlowStream.Update("Value02");
//	if ( !bResult ) {
//		ShowMessage("유량산출방식 저장 에러");
//		return false;
//	}

	// HydroMeter1--------------------------------------------------------------

	HydroMeter1.PK1 = 10;
	HydroMeter1.PK2 = 2;

	HydroMeter1.Value01 = cbxHydro1Type->Text;
	if ( cbxHydro1Type->Text == "RQ-30") HydroMeter1.Value01 = RQ30_STR;	// RQ-30
	if ( cbxHydro1Type->Text == "RQ-30d") HydroMeter1.Value01 = RQ30D_STR;	// RQ-30d
	HydroMeter1.Value02 = cbxHydro1ComPort->Text;
	HydroMeter1.Value03 = cbxHydro1BaudRate->Text;
	HydroMeter1.Value04 = edHydro1Offset->Text;

	if ( HydroMeter1.Value01 == AGSERIES_STR || HydroMeter1.Value01 == SLSERIES_STR ) {
		HydroMeter1.Value05 = "";	// not use
		HydroMeter1.Value06 = "";	// not use
		HydroMeter1.Value07 = edHydro1CellNo->Text;
		HydroMeter1.Value08 = edHydro1CellSize->Text;
		HydroMeter1.Value09 = edHydro1Blank->Text;
	} else if ( HydroMeter1.Value01 == RQ30D_STR ) {
		HydroMeter1.Value05 = edHydro1MasterAddress->Text;	// Master Port Device Address
		HydroMeter1.Value06 = (cbxHydro1UseSlavePort->Checked) ? "ON" : "OFF";
		HydroMeter1.Value07 = cbHydro1SlavePort->Text;	// COM01,COM02,..
		HydroMeter1.Value08 = cbHydro1SlaveBaudRate->Text;	// 2400,4800,9600,..
		HydroMeter1.Value09 = edHydro1SlaveAddress->Text;	// ""/6,7,8,9,10
	} else {
		HydroMeter1.Value05 = "";	// not use
		HydroMeter1.Value06 = "";	// not use
		HydroMeter1.Value07 = "";	// not use
		HydroMeter1.Value08 = "";	// not use
		HydroMeter1.Value09 = "";	// not use
	}

	HydroMeter1.Value10 = cbxHydro1Reverse->Text;
	HydroMeter1.Value11 = edHydro1Hz->Text;
	HydroMeter1.Value12 = edHydro1SN->Text;

	HydroMeter1.Value14 = (cbxHydro1IdleCheck->ItemIndex == -1 || cbxHydro1IdleCheck->ItemIndex == 0) ? "ON" : "OFF";  // default : ON => idle check ON
	HydroMeter1.Value15 = (cbxHydro1IdleCheckTime->ItemIndex == -1) ? 2 : (cbxHydro1IdleCheckTime->ItemIndex + 1); // default : 3 hours

	HydroMeter1.Value16 = cbxAirmar1ComPort->Text;		// Airmar ComPort
	HydroMeter1.Value17 = cbxAirmar1BaudRate->Text;		// Airmar BaudRate
	HydroMeter1.Value20 = (cbAirmarLinker->Checked) ? "ON" : "OFF";	// heaven2 2022.12.01
	HydroMeter1.Value13 = cbxSlaveCount->ItemIndex;		// heaven2 0:SlaveCount None, 1,2,3,4=>SlaveCount

	HydroMeter1.Value18 = (edHydro1StartCell->Text.IsEmpty()) ?  UnicodeString("1") :  edHydro1StartCell->Text;
	HydroMeter1.Value19 = (edHydro1EndCell->Text.IsEmpty())   ? UnicodeString("10") :  edHydro1EndCell->Text;

	bResult = HydroMeter1.Update();
	if ( !bResult ) {
		ShowMessage("하층 유속계  정보 저장 시 오류가 발생하였습니다.");
		return false;
	}

	// HydroMeter2--------------------------------------------------------------

	HydroMeter2.PK1 = 10;
	HydroMeter2.PK2 = 3;

	HydroMeter2.Value01 = cbxHydro2Type->Text;
	if ( cbxHydro2Type->Text == "RQ-30")  HydroMeter2.Value01 = RQ30_STR;
	if ( cbxHydro2Type->Text == "RQ-30d") HydroMeter2.Value01 = RQ30D_STR;
	HydroMeter2.Value02 = cbxHydro2ComPort->Text;
	HydroMeter2.Value03 = cbxHydro2BaudRate->Text;
	HydroMeter2.Value04 = edHydro2Offset->Text;

	HydroMeter2.Value07 = edHydro2CellNo->Text;
	HydroMeter2.Value08 = edHydro2CellSize->Text;
	HydroMeter2.Value09 = edHydro2Blank->Text;

	HydroMeter2.Value10 = cbxHydro2Reverse->Text;
	HydroMeter2.Value11 = cbxHydro2UpSide->Text;

	HydroMeter2.Value12 = edHydro2Hz->Text;
	HydroMeter2.Value13 = edHydro2SN->Text;

	HydroMeter2.Value14 = (cbxHydro2IdleCheck->ItemIndex == -1 || cbxHydro2IdleCheck->ItemIndex == 0) ? "ON" : "OFF";  // default : ON => idle check ON
	HydroMeter2.Value15 = (cbxHydro2IdleCheckTime->ItemIndex == -1) ? 2 : (cbxHydro2IdleCheckTime->ItemIndex + 1); // default : 3 hours

//	HydroMeter2.Value16 = cbxAirmar2ComPort->Text;		// Airmar ComPort
//	HydroMeter2.Value17 = cbxAirmar2BaudRate->Text;		// Airmar BaudRate

	HydroMeter2.Value18 = ((edHydro2StartCell->Text).IsEmpty()) ? UnicodeString("1") :  edHydro2StartCell->Text;
	HydroMeter2.Value19 = ((edHydro2EndCell->Text).IsEmpty())   ? UnicodeString("10") :  edHydro2EndCell->Text;

	bResult = HydroMeter2.Update();
	if ( !bResult ) {
		ShowMessage("상층 유속계  정보 저장 시 오류가 발생하였습니다.");
		return false;
	}

	// WaterLevel --------------------------------------------------------------

	WaterLevel.PK1 = 10;
	WaterLevel.PK2 = 4;

	WaterLevel.Value01 = cbxWaterType->Text;
	WaterLevel.Value02 = cbxWaterComPort->Text;
	WaterLevel.Value03 = cbxWaterBaudRate->Text;
	WaterLevel.Value04 = edWaterOffset->Text;
	//WaterLevel.Value05 = edWaterOffset->Text;	// Frequency => Hz
	WaterLevel.Value06 = edWaterSN->Text;
	WaterLevel.Value07 = cbxAtmosPort->Text;
	if ( WaterLevel.Value07.IsEmpty() ) WaterLevel.Value07 = "NONE";	// INIT Value
	if ( WaterLevel.Value01.Pos(LT400_STR) > 0 ) WaterLevel.Value01 = LT400_STR;	// INIT Value
	WaterLevel.Value08 = edBaseAtmos->Text;	// 기준대기압

	WaterLevel.Value09 = (cbxWaterIdleCheck->ItemIndex == -1 || cbxWaterIdleCheck->ItemIndex == 0 ) ? "ON" : "OFF"; 	// default : ON => idle check
	WaterLevel.Value10 = (cbxWaterIdleCheckTime->ItemIndex == -1) ? 2 : (cbxWaterIdleCheckTime->ItemIndex + 1);	// default : 3 hours

	bResult = WaterLevel.Update();
	if ( !bResult ) {
		ShowMessage("수위계 정보 저장 시 오류가 발생하였습니다.");
		return false;
	}

	return bResult;
}

//---------------------------------------------------------------------------
bool TFormSetup::SaveFlowStreamToDB()
{
	bool bResult = true;

	// FlowStream --------------------------------------------------------------

	TRSetup FlowStream;
	FlowStream.PK1 = 1;
	FlowStream.PK2 = 3;

	FlowStream.Value02 = cbxFlowStreamType->ItemIndex ; // 0:Off, 1:유량사업단, 2:RNSEA
	bResult = FlowStream.Update("Value02");
	if ( !bResult ) {
		ShowMessage("유량산출방식 저장 에러");
		return false;
	}

	if ( cbxFlowStreamType->ItemIndex == 0 ) return true; // 자동유량 사용하지 않음

	if ( cbxFlowStreamType->ItemIndex == 1 ) {	// 유량 사업단 방식

		// FlowADVM, FlowHA, FlowIVM -----------------------------------------------

		TRFlowADVM FlowADVM;
		TRFlowHA   FlowHA;
		TRFlowIVM  FlowIVM;

		FlowADVM.PointCode = theEnv.m_PointCode;
		FlowHA.PointCode   = theEnv.m_PointCode;
		FlowIVM.PointCode  = theEnv.m_PointCode;

		bResult = FlowADVM.GetData();
		if ( !bResult ) return bResult;

		bResult = FlowHA.GetData();
		if ( !bResult ) return bResult;

		bResult = FlowIVM.GetData();
		if ( !bResult ) return bResult;

		// 하층 유속계
		FlowADVM.AXadvm        = StrToFloat(AXadvm->Text);
		FlowADVM.AZadvm        = StrToFloat(AZadvm->Text);
		FlowADVM.APan          = StrToInt(APan->Text);
		FlowADVM.ATilt         = StrToInt(ATilt->Text);
		FlowADVM.APhi          = StrToFloat(APhi->Text);
		FlowADVM.ASubmersion   = StrToFloat(ASubmersion->Text);
		FlowADVM.AReverseMode  = AReverseMode->ItemIndex;	// 0: ON, 1: OFF
		FlowADVM.AUnitVelocity = AUnitVelocity->ItemIndex;	// 0: mm/s, 1:cm/s, 2: m/s
		FlowADVM.AVMin         = StrToFloat(AVMin->Text);
		FlowADVM.AVMax         = StrToFloat(AVMax->Text);
		FlowADVM.AStartCell    = StrToInt(AStartCell->Text);
		FlowADVM.AEndCell      = StrToInt(AEndCell->Text);

		// 상층 유속계
		FlowADVM.BXadvm        = StrToFloat(BXadvm->Text);
		FlowADVM.BZadvm        = StrToFloat(BZadvm->Text);
		FlowADVM.BPan          = StrToInt(BPan->Text);
		FlowADVM.BTilt         = StrToInt(BTilt->Text);
		FlowADVM.BPhi          = StrToFloat(BPhi->Text);
		FlowADVM.BSubmersion   = StrToFloat(BSubmersion->Text);
		FlowADVM.BReverseMode  = BReverseMode->ItemIndex;	// 0: ON, 1: OFF
		FlowADVM.BUnitVelocity = BUnitVelocity->ItemIndex;	// 0: mm/s, 1:cm/s, 2: m/s
		FlowADVM.BVMin         = StrToFloat(BVMin->Text);
		FlowADVM.BVMax         = StrToFloat(BVMax->Text);
		FlowADVM.BStartCell    = StrToInt(BStartCell->Text);
		FlowADVM.BEndCell      = StrToInt(BEndCell->Text);
		FlowADVM.BType         =  (BType->ItemIndex == 0) ? "UP" :
								 ((BType->ItemIndex == 1 ) ? "LEFTRIGHT" : "SOUTHNORTH");

		bResult = FlowADVM.Update();
		if ( !bResult ) return bResult;

		// FlowHA Tab --------------------------------------------------------------

		FlowHA.MinH = StrToFloat(HAMinH->Text);
		FlowHA.MaxH = StrToFloat(HAMaxH->Text);
		FlowHA.A6   = StrToFloat(HAA6->Text);
		FlowHA.A5   = StrToFloat(HAA5->Text);
		FlowHA.A4   = StrToFloat(HAA4->Text);
		FlowHA.A3   = StrToFloat(HAA3->Text);
		FlowHA.A2   = StrToFloat(HAA2->Text);
		FlowHA.A1   = StrToFloat(HAA1->Text);
		FlowHA.A0   = StrToFloat(HAA0->Text);

		bResult = FlowHA.Update();
		if ( !bResult ) return bResult;

		// FlowIVM Tab -------------------------------------------------------------

		FlowIVM.AMinVi = StrToFloat(IVMAMinVi->Text);
		FlowIVM.AMaxVi = StrToFloat(IVMAMaxVi->Text);
		FlowIVM.AA     = StrToFloat(IVMAA->Text);
		FlowIVM.AB     = StrToFloat(IVMAB->Text);
		FlowIVM.AC     = StrToFloat(IVMAC->Text);
		FlowIVM.BMinVi = StrToFloat(IVMBMinVi->Text);
		FlowIVM.BMaxVi = StrToFloat(IVMBMaxVi->Text);
		FlowIVM.BA     = StrToFloat(IVMBA->Text);
		FlowIVM.BB     = StrToFloat(IVMBB->Text);
		FlowIVM.BC     = StrToFloat(IVMBC->Text);

		bResult = FlowIVM.Update();
		if ( !bResult ) return bResult;

		return bResult;
	}

	if ( cbxFlowStreamType->ItemIndex == 2 ) {	// RNSEA 방식
		return true;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TFormSetup::SaveEtcToDB()
{
	bool bCheck = false;
	bCheck = CheckEtc();
	if ( !bCheck ) {
		ShowMessage("입력 정보에 오류가 있습니다.");
		return false;
	}

	TRSetup Setup;
	Setup.PK1 = 1;
	Setup.PK2 = 2;

	Setup.Value01 = edRADSHome->Text;
	Setup.Value02 = edLoggerHome->Text;
	Setup.Value03 = edSenderHome->Text;
	Setup.Value04 = edViewerHome->Text;

	Setup.Value05 = edLoggerIP->Text;
	Setup.Value06 = edSenderIP->Text;
	Setup.Value07 = edDatabaseIP->Text;

	Setup.Value08 = edLoggerPort->Text;
	Setup.Value09 = edSenderLoggerPort->Text;
	Setup.Value10 = edSenderViewerPort->Text;
	Setup.Value11 = edWatchDogPort->Text;

	bool bResult = false;
	bResult = Setup.Update();

	return bResult;
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::FormShow(TObject *Sender)
{
	PageSetup->ActivePage = TabSheetPointInfo;	// First Page

	ClearPoint();
	LoadPointFromDB();
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::BitBtn1Click(TObject *Sender)
{
	if ( !m_bChanged ) {	// 변경 내역이 없으면 바로 닫는다.
		ModalResult = mrCancel;
		//Close();
	} else {	// 변경 내역이 존재하면 .

		ShowMessage("변경 내역이 반영되려면 프로그램을 종료 후 재시작하여 주십시오.");
		ModalResult = mrOk;
		//Close();
	}
}

//---------------------------------------------------------------------------
void TFormSetup::LogMessage(UnicodeString sMsg, bool bFile, bool bDB)
{
	FormMain->LogMessage(sMsg, bFile, bDB);
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbxHydro1TypeChange(TObject *Sender)
{
	if ( cbxHydro1Type->ItemIndex == 0 ) {	// NONE
		//cbxHydro1ComPort->ItemIndex = 0;	// NONE
		//cbxHydro1BaudRate->ItemIndex = 0;	// NONE
		Panel1->Visible          = false;	// SL/AG Option
		PanelAirmar->Visible     = false;	// Airmar Option
		PanelRQ30d->Visible      = false;	// RQ-30d
	} else if ( cbxHydro1Type->ItemIndex == 2 || cbxHydro1Type->ItemIndex == 3 ) { // SL/AG Series
		Panel1->Visible          = true;	// SL/AG Option
		PanelAirmar->Visible     = false;	// Airmar Option
		PanelRQ30d->Visible      = false;
	} else if ( cbxHydro1Type->ItemIndex == 6 ) { // RQ-30
		Panel1->Visible          = false;
		PanelAirmar->Visible     = true;
		PanelRQ30d->Visible      = false;
	} else if ( cbxHydro1Type->ItemIndex == 7 ) { // RQ-30d,
		Panel1->Visible          = false;
		PanelAirmar->Visible     = true;
		PanelRQ30d->Visible      = true;
	} else {  	// CM(1), AccusonicA/B(4/5)
		Panel1->Visible          = false;
		PanelAirmar->Visible     = false;
		PanelRQ30d->Visible      = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSetup::cbxHydro2TypeChange(TObject *Sender)
{
	if ( cbxHydro2Type->ItemIndex == 0 ) { 	// NONE
		//cbxHydro2ComPort->ItemIndex = 0;	// NONE
		//cbxHydro2BaudRate->ItemIndex = 0;	// NONE
		Panel2->Visible          = false;	// SL/AG Option
	} if ( cbxHydro2Type->ItemIndex == 2 || cbxHydro2Type->ItemIndex == 3 ) { // SL/AG Series
		Panel2->Visible       = true;
		//PanelAirmar2->Visible = false;
	} else {
		Panel2->Visible       = false;
		//PanelAirmar2->Visible = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSetup::cbxFlowStreamTypeChange(TObject *Sender)
{
	if ( cbxFlowStreamType->ItemIndex == 0 ) {
		PageControlFlowStream->Visible = false;
	} else {
		PageControlFlowStream->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSetup::cbxWaterTypeChange(TObject *Sender)
{
	if ( cbxWaterType->ItemIndex == 0 ) {	// Device type => NONE
		//cbxWaterComPort->ItemIndex = 0;		// NONE
		//cbxWaterBaudRate->ItemIndex = 0;	// NONE
		lbAtmosCap->Visible     = false;
		cbxAtmosPort->Visible   = false;
		//cbxAtmosPort->ItemIndex = 0;
		edBaseAtmos->Visible    = false;
	} if ( cbxWaterType->Text.Pos(LT400_STR) > 0 ) {
		lbAtmosCap->Visible     = true;
		cbxAtmosPort->Visible   = true;
		edBaseAtmos->Visible    = true;
		cbxAtmosPort->ItemIndex = ConvertComPort(theEnv.m_AtmosPort);	// None
	} else {
		lbAtmosCap->Visible     = false;
		cbxAtmosPort->Visible   = false;
		cbxAtmosPort->ItemIndex = 0;	// None
		edBaseAtmos->Visible    = false;
	}
}

//---------------------------------------------------------------------------
// Ping Test about DataSender
void __fastcall TFormSetup::ButtonPingClick(TObject *Sender)
{
	bool bResult = false;
	bResult = FormMain->SendPing();
	if ( bResult ) {
		TimerPing->Interval = 2 * 1000;	 // 2 second timer
		TimerPing->Enabled = true;
		PingLog("Ping..");
		m_iPingCnt = 1;
	} else {
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping fail..");
	}
}

//---------------------------------------------------------------------------
// Reboot Request to DataSender PC
void __fastcall TFormSetup::ButtonRebootClick(TObject *Sender)
{
	if ( TimerPing->Enabled ) {
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping stop..");
	}
	// Warning
	if ( Application->MessageBox(L"Sender PC를 리부팅합니다.(Ping Test가 성공적일 때 가능합니다.) 계속할까요?", L"Sender PC 리부팅", MB_YESNO) != IDYES ) return;

	bool bResult = false;
	bResult = FormMain->SendRebootReq();
	if ( bResult ) {
		PingLog("RebootReq to DataSender");
	} else {
		PingLog("RebootReq failed..");
	}
}

//---------------------------------------------------------------------------
// Ping Timer Event
void __fastcall TFormSetup::TimerPingTimer(TObject *Sender)
{
	m_iPingCnt++;
	if ( m_iPingCnt >= 5 ) {  	// Ping Timer Expire
		TimerPing->Enabled = false;
		m_iPingCnt = 0;
		PingLog("Ping stop..");
	} else {
		bool bResult = false;
		bResult = FormMain->SendPing();
		if ( bResult ) {
			PingLog("Ping..");
		} else {
			TimerPing->Enabled = false;
			PingLog("Ping failed..");
		}
	}
}

//---------------------------------------------------------------------------
// Ping Timer
void TFormSetup::PingLog(UnicodeString sMsg)
{
	if ( MemoPingLog->Lines->Count > 100 ) MemoPingLog->Lines->Clear();
	MemoPingLog->Lines->Insert(0, sMsg);
}

//---------------------------------------------------------------------------
// Ping Timer

void TFormSetup::DataSenderMessage(BYTE *pData, int Len)
{
	BYTE Type = *(pData + 1);

	if ( Type == MSG_TYPE_PingAck ) {
		PingLog("Ping acked..");
		LogMessage("Ping acked..");
	} else if ( Type == MSG_TYPE_RebootRes ) {
		PingLog("RebootRes received..");
		LogMessage("RebootRes recevied..");
	} else {
		PingLog("unknown message type");
		LogMessage("unknown message type");
	}
}

//---------------------------------------------------------------------------
// Slave Port를 사용/미사용 Check
void __fastcall TFormSetup::cbxHydro1UseSlavePortClick(TObject *Sender)
{
	if ( cbxHydro1UseSlavePort->Checked ) {	// Slave Port를 사용함
		cbHydro1SlavePort->Visible     = true;
		cbHydro1SlaveBaudRate->Visible = true;
		edHydro1SlaveAddress->Visible  = true;
		lbHydro1SlavePort->Visible     = true;
		lbHydro1SlaveBaudRate->Visible = true;
		lbHydro1SlaveAddress->Visible  = true;
	} else {								// Slave Port 사용하지 않음 => Master Port에서 모두 처리
		cbHydro1SlavePort->Visible     = false;
		cbHydro1SlaveBaudRate->Visible = false;
		edHydro1SlaveAddress->Visible  = false;
		lbHydro1SlavePort->Visible     = false;
		lbHydro1SlaveBaudRate->Visible = false;
		lbHydro1SlaveAddress->Visible  = false;
	}
}
//---------------------------------------------------------------------------

