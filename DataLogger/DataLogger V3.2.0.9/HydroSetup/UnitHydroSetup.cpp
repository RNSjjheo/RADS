//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitHydroSetup.h"
#include "UnitSerialThread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

THydroSetup *HydroSetup;

UnicodeString ElapsedTime(int iSec)	// 90 => 01:30 형식으로 변환
{
	UnicodeString Str;

	Str = Str.sprintf(L"%02d:%02d", iSec/60, iSec%60);

	return Str;
}

//---------------------------------------------------------------------------
__fastcall THydroSetup::THydroSetup(TComponent* Owner)
	: TForm(Owner)
{
	ComPort      = NULL;
	HydroMeter   = NULL;
	SerialThread = NULL;

	m_sRecvMsg = "";
	m_bBreak   = false;	// Set Break를 하였는 지 아닌 지에 대한 Flag
						// Set Break를 한 후에 Setup 화면에서 빠져나갈 때 true가 되어 있으면 Start Command를 보낸다.
	m_enExit   = enCancel;
}
//---------------------------------------------------------------------------
void THydroSetup::Init(TApdComPort *pComPort, THydroMeter *pHydroMeter,bool bFirstHydro)
{
	ComPort       = pComPort;
	HydroMeter    = pHydroMeter;
	m_bFirstHydro = bFirstHydro;
}
//---------------------------------------------------------------------------
void THydroSetup::Receive(AnsiString sRecvMsg)
{
//
}
//---------------------------------------------------------------------------
bool THydroSetup::Send(AnsiString sSendMsg)
{
	if ( ComPort == NULL ) return false;
	if ( ComPort->Open == false ) return false;

	try {
		ComPort->PutString(sSendMsg + "\r");
	} catch ( Exception &e ) {
#ifdef _DEBUG
		ShowMessage(e.Message);
#endif
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void THydroSetup::LogMessage(UnicodeString sLogMsg, bool bFile, bool bDB)
{
	FormMain->LogMessage(sLogMsg, bFile, bDB);
}


