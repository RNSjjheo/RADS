//---------------------------------------------------------------------------

#ifndef UnitSerialThreadH
#define UnitSerialThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include "AdPort.hpp"
#include <System.Contnrs.hpp>
#include <System.SyncObjs.hpp>

#include "UnitHydroSetup.h"

struct STCommand {
	AnsiString sCmd;
	int        iDelayTime;
};

class TSerialThread : public TThread
{
private:
public:
	TApdComPort *m_pComPort;
	int          m_iPosition;
	TQueue      *m_pCmdQueue;
	void         Init(TApdComPort *ComPort, THydroSetup *pHydroSetup);

	THydroSetup *m_pHydroSetup;	// Parent Form Pointer

protected:
	void __fastcall SetPosition();
	void __fastcall Execute();

public:
	__fastcall TSerialThread(bool CreateSuspended);
};


//---------------------------------------------------------------------------
#endif
