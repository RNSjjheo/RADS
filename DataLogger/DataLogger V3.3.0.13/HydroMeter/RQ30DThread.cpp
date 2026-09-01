//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "Define.h"
#include "RQ30DThread.h"
#include "RQ30D.h"

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

__fastcall TRQ30DThread::TRQ30DThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	m_pRQ30d        = NULL;         // Parent Pointer
	m_pPort         = NULL;
	
	m_DeviceAddress = "";		// (ex) 5,4,3,2,1 or 6,7,8,9,10
	memset(m_Address, 0, sizeof(m_Address));
	m_DeviceCount = 0;
}

//---------------------------------------------------------------------------
void TRQ30DThread::InitThread(TRQ30D *pRQ30d, TApdComPort *pPort, UnicodeString DeviceAddress, bool bMasterPort)
{
	m_pRQ30d        = pRQ30d;         // Parent Pointer
	m_pPort         = pPort;
	m_bMasterPort   = bMasterPort;	// MasterPort Threa/SlavePort Thread Flag

	m_DeviceAddress = DeviceAddress;
	
	ParseDeviceAddress();	// m_DeviceAddress => m_Address, m_DeviceCnount
}

//---------------------------------------------------------------------------
// RQ-30d의 각 Serial Port당 할당된 RQ-30/RG-30 Device Address를 합쳐 놓은 상태: (ex) 5,4,3,2,1 (ex) 6,7,8,9,10
// DeviceAddress = "5,4,3,2,1" || DeviceAddress = "6,7,8,9,10"
void TRQ30DThread::ParseDeviceAddress()
{
	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = m_DeviceAddress;

	if ( pStrList->Count < 0 ) {
		delete pStrList;	// release
		return;			// Address not exist
	}

	m_DeviceCount = pStrList->Count;

	UnicodeString AddressStr = "";
	for ( int i = 0 ; i < pStrList->Count ; i++ ) {
		AddressStr = pStrList->Strings[i];
		try {
			m_Address[i] = StrToInt(AddressStr);
		} catch ( Exception &e ) {
			m_Address[i] = 0;	// Address => 0 => error address
		}
	}
	delete pStrList;
}

//---------------------------------------------------------------------------
void __fastcall TRQ30DThread::Execute()
{
	TWaitResult Result;
	STRQ30DMessage *pRQ30DMessage;

	//---------------------------------------------------------------------
	// Master Port Thread
	if ( m_bMasterPort ) {
		while ( !this->Terminated ) {
			Result = MasterEvent->WaitFor(EVENT_WAIT_TIME);	// wait 1 second
			if ( Result == wrSignaled || Result == wrTimeout ) {
				while ( MasterQueue->Count() ) {

					MasterSection->Enter();
					pRQ30DMessage = (STRQ30DMessage *) MasterQueue->Pop();
					MasterSection->Leave();

					ProcessMessage(pRQ30DMessage);
					delete pRQ30DMessage;
				}
			} else if ( Result == wrAbandoned ) {	// Event Object was destored
													// wrError: An error occured while waiting.
													// Check the LastError property for an error
													// code giving more informaiton.
			} else {
				// nothing to do..
			} // if
			//G_pSoundEvent->ResetEvent();			// Auto Event Reset mode
		} // while
	//---------------------------------------------------------------------
	// Slave Port Thread
	} else {
		while ( !this->Terminated ) {
			Result = SlaveEvent->WaitFor(EVENT_WAIT_TIME);	// wait 1 second
			if ( Result == wrSignaled || Result == wrTimeout ) {
				while ( SlaveQueue->Count() ) {

					SlaveSection->Enter();
					pRQ30DMessage = (STRQ30DMessage *) SlaveQueue->Pop();
					SlaveSection->Leave();

					ProcessMessage(pRQ30DMessage);
					delete pRQ30DMessage;
				}
			} else if ( Result == wrAbandoned ) {	// Event Object was destored
													// wrError: An error occured while waiting.
													// Check the LastError property for an error
													// code giving more informaiton.
			} else {
				// nothing to do..
			}
			//G_pSoundEvent->ResetEvent();			// Auto Event Reset mode
		} // while
	} // if
}

//---------------------------------------------------------------------------
void TRQ30DThread::ProcessMessage(STRQ30DMessage *pRQ30DMessage)
{
	int Address             = 0;
	AnsiString Id           = "W";
	AnsiString SystemKey    = "00";
	AnsiString DeviceNumber = "01";	    // default;
	AnsiString StartCmd     = "$mt";	// measure start
	AnsiString FinishCmd    = "$pt";	// measure finish
	bool       Prefix       = false;
	AnsiString Str = "";

	// 이것은 RQ-30d/RG-30d 장비의 485 통신 Wakeup 을 위해 의미없는 데이터를 하나 던진다.
	AnsiString Space = " ";
	Send(Space, Space.Length());
	SleepMsg(10);	// wait for while
	Send(Space, Space.Length());
	SleepMsg(10);	// wait for while
	Send(Space, Space.Length());
	SleepMsg(10);	// wait for while

	// 측정 시작 Command -------------------------------------------------
	if ( pRQ30DMessage->Action == RQ30D_ACTION_START ) {

		for ( int i = 0 ; i < m_DeviceCount ; i++ ) {
			Address = GetDeviceAddress(i);
			DeviceNumber = Str.sprintf("%02d", Address);	// "01","02",...
			Prefix = ( i == 0 ) ? true : false;
			SendCommand(Id, SystemKey, DeviceNumber, StartCmd, Prefix);

			SleepMsg(1000);	// Wait 1 second ( 2025.02.21 Sleep(1000) => SleepMsg(1000) 로 변경
		}

	// 측정 종료 Command(Data Request) -----------------------------------
	} else if ( pRQ30DMessage->Action == RQ30D_ACTION_FINISH ) { 	// pRQ30DMessage->Action == RQ30D_ACTION_Finish ( Data Request
		int DeviceCount = HowManyDevice(); // 5,4,3,2,1 => 5
		int Address = 0;

		for ( int i = 0 ; i < DeviceCount ; i++ ) {
			Address = GetDeviceAddress(i);
			DeviceNumber = Str.sprintf("%02d", Address);	// "01","02",...
			Prefix = ( i == 0 ) ? true : false;
			SendCommand(Id, SystemKey, DeviceNumber, FinishCmd, Prefix);

			SleepMsg(3000);	// 종료 Command에 대한 응답(Main/Special/Analysis Value 수신할 시간) 3초 기다림.
			                // ( 2025.02.21 Sleep(1000) => SleepMsg(1000) 로 변경
		}
	} else {	// RQ30D_ACTION_QUIT
		// nothing to do..
	}
}

//---------------------------------------------------------------------------
bool TRQ30DThread::SendCommand(AnsiString Id, AnsiString SystemKey, AnsiString DeviceNumber, AnsiString Cmd, bool Prefix)
{                       //          W              00                    01                  $mt / $pt
	bool bResult = false;
	AnsiString StrCmd = "";

	//StrCmd = (Prefix) ? " #" : "#"; // 최초 보낼 때.. Space를 넣어서 걸리도록 한다.
	StrCmd  = "#";
	StrCmd += Id;           // "W/S/R/T/A"
	StrCmd += SystemKey;    // "00"
	StrCmd += DeviceNumber; // "01"
	StrCmd += Cmd;          // $mt / $pt / ... _bq90 / A=1 / B=60 / ...
	StrCmd += "|";          // delimiter

	AnsiString CRC = MakeCRC((BYTE *)StrCmd.c_str(), StrCmd.Length());

	StrCmd += CRC;
	StrCmd += ";";          // end character
	StrCmd += "\r\n";

//LogMessage(StrCmd);

	Send(StrCmd, StrCmd.Length());

    return bResult;
}

//---------------------------------------------------------------------------
int  TRQ30DThread::CheckFrame(BYTE *pBuffer, int Length)   //#A0001ok_bq|6FE5;
{
	if (*(pBuffer+0) != '#') return STX_ERROR;
	if (*(pBuffer+Length-1) != ';') return ETX_ERROR;

	AnsiString StrCRC = "";
	StrCRC = AnsiString((char *)(pBuffer + Length-5),4);

	// make crc
	AnsiString StrCRC2 = MakeCRC(pBuffer, Length-5);

	if ( StrCRC != StrCRC2 ) return CRC_ERROR;

	return FRAME_OK;
}

//---------------------------------------------------------------------------
AnsiString TRQ30DThread::MakeCRC(BYTE *pBuffer, int Length)
{
	AnsiString Str = "";
	unsigned short int crc = 0x0000;

	for ( int i = 0 ; i < Length ; i++ ) {
		crc = CRC16(pBuffer[i], crc);
	}

	Str = IntToHex(crc,4);

	return Str;
}

//---------------------------------------------------------------------------
bool TRQ30DThread::Send(AnsiString Cmd, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		if ( m_bMasterPort) pGMasterPortSection->Enter();	// CriticalSection Enter
		else                pGSlavePortSection->Enter();
			
		m_pPort->PutString(Cmd);
		
		if ( m_bMasterPort) pGMasterPortSection->Leave();	// CriticalSection Leave 
		else                pGSlavePortSection->Leave();
			
		LogMessage(((m_bMasterPort) ? "[RQ30D](Master)=>" : "[RQ30D](Slave)=>") + Cmd);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}
	return true;	
}
		
//---------------------------------------------------------------------------
int  TRQ30DThread::HowManyDevice()
{
	return m_DeviceCount;
}

//---------------------------------------------------------------------------
int  TRQ30DThread::GetDeviceAddress(int index)
{
	return m_Address[index];	
}
