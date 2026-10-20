//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitSerialThread.h"
#pragma package(smart_init)

//#include "UnitHydroSetup.h"

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSerialThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSerialThread::TSerialThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	m_pComPort    = NULL;
	m_iPosition   = 0;
	m_pCmdQueue   = new TQueue();

	m_pHydroSetup = NULL;
}

//---------------------------------------------------------------------------
void TSerialThread::Init(TApdComPort *ComPort, THydroSetup *pHydroSetup)
{
	m_pComPort    =  ComPort;
	m_pHydroSetup =  pHydroSetup;
}

//---------------------------------------------------------------------------
void __fastcall TSerialThread::Execute()
{
	if ( m_pCmdQueue->Count() == 0 ) {
		delete m_pCmdQueue;
		return;
	}

	if ( m_pComPort == NULL ) return;

	STCommand *pCmd = NULL;
	int Count = m_pCmdQueue->Count();
	int i = 0;
	while ( m_pCmdQueue->Count() != 0 ) {

		pCmd = (STCommand *) m_pCmdQueue->Pop();

		if ( pCmd->sCmd.IsEmpty() || pCmd->sCmd == "" ) continue;

		try {
			m_pComPort->PutString(pCmd->sCmd + "\r");
		} catch (Exception &e) {
			delete pCmd;
			continue;
		}

		m_iPosition =(int)(((double)(++i)/(double)Count) * 100);

		Synchronize(SetPosition);

		Sleep(pCmd->iDelayTime);	// Sleep for While

		delete pCmd;
	}

	delete m_pCmdQueue;
	m_pHydroSetup->SerialThread = NULL;

	return;
}

//---------------------------------------------------------------------------
void __fastcall TSerialThread::SetPosition()
{
	//
}
