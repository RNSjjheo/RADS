//---------------------------------------------------------------------------

#ifndef UnitHydroSetupH
#define UnitHydroSetupH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "../Define.h"
#include "../UnitMain.h"
#include "../UnitDM.h"
#include "../HydroMeter/HydroMeter.h"
#include "../UnitAlert.h"

#include "AdPort.hpp"
#include "OoMisc.hpp"

enum EN_EXIT_KIND { enOk, enCancel };

class TSerialThread;

//---------------------------------------------------------------------------
class THydroSetup : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:

	TApdComPort   *ComPort;
	THydroMeter   *HydroMeter;
	TSerialThread *SerialThread;

	bool         m_bFirstHydro;	// 하층유속계(true) or 상층유속계(false)
	AnsiString   m_sRecvMsg;	// 센서의 수신 Message
	bool         m_bBreak;		// Break or not

	EN_EXIT_KIND m_enExit;		// ok or cancel

public:		// User declarations
	virtual void Init(TApdComPort *pComPort, THydroMeter *pHydroMeter, bool bFirstHydro=true);
	virtual void Receive(AnsiString sRecvMsg);
	virtual bool Send(AnsiString sSendMsg);

	virtual void LogMessage(UnicodeString sLogMsg, bool bFile=true, bool bDB=false);

	__fastcall THydroSetup(TComponent* Owner);
};

UnicodeString ElapsedTime(int iSec);	// 90 => 01:30 형식으로 변환

//---------------------------------------------------------------------------
extern PACKAGE THydroSetup *HydroSetup;
//---------------------------------------------------------------------------
#endif
