//---------------------------------------------------------------------------

#ifndef RQ30DThreadH
#define RQ30DThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

#include "Define.h"
#include "RQ30d.h"
#include "UnitEnv.h"

#include "AdPort.hpp"
#include "OoMisc.hpp"

#define RQ30D_ACTION_QUIT		1
#define RQ30D_ACTION_START		2
#define RQ30D_ACTION_FINISH		3

//---------------------------------------------------------------------------
struct STRQ30DMessage {
	int Action;			// RQ30D_ACTION_QUIT / RQ30D_ACTION_START / RQ30D_ACTION_FINISH
};


//---------------------------------------------------------------------------
class TRQ30DThread : public TThread
{
private:
	bool           m_bMasterPort;		// True: MasterPort, False: SlavePort
	TRQ30D        *m_pRQ30d;
	TApdComPort   *m_pPort;				// ComPort
	
	UnicodeString  m_DeviceAddress;		// (ex) 5,4,3,2,1 or 6,7,8,9,10
	int            m_Address[10];		// 
	int            m_DeviceCount;		// 
public:

protected:
	void __fastcall Execute();

public:
	__fastcall TRQ30DThread(bool CreateSuspended);
	void InitThread(TRQ30D *pRQ30d, TApdComPort *pPort, UnicodeString DevideAddress, bool bMasterPort=true);
	void ParseDeviceAddress();

	void ProcessMessage(STRQ30DMessage *pMessage);
	bool SendCommand(AnsiString Id, AnsiString SystemKey, AnsiString DeviceNumber, AnsiString Cmd, bool Prefix=true);	
	int  CheckFrame(BYTE *pBuffer, int Length);
	AnsiString MakeCRC(BYTE *pBuffer, int Length);	
	
	bool Send(AnsiString Cmd, int iLen);
	int  HowManyDevice();
	int  GetDeviceAddress(int index);
};
//---------------------------------------------------------------------------
#endif
