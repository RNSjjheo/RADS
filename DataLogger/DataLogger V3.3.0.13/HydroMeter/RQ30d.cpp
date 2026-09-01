//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "RQ30D.h"
#include "AniThread.h"
#include "System.RegularExpressions.hpp"
#include "UnitMain.h"
#include "../Database/RHydroMeterCell.h"
#include "RQ30DThread.h"

TCriticalSection *MasterSection = new TCriticalSection();			// for Master Port Thread
TCriticalSection *SlaveSection  = new TCriticalSection();			// for Slave Port Thread

TQueue *MasterQueue = new TQueue();									// for Master Port Thread
TQueue *SlaveQueue  = new TQueue();									// for Slave Port Thread

TEvent *MasterEvent = new TEvent(NULL, false, false, NULL, false);	// for Master Port Thread
TEvent *SlaveEvent  = new TEvent(NULL, false, false, NULL, false);	// fro Slave Port Thread

//int __fastcall CompareRQDVelocity(void *Item1, void *Item2)
//{
//	STRQ30DSet *A, *B;
//	A = (STRQ30DSet *)Item1;
//	B = (STRQ30DSet *)Item2;
//	if ( A->RQ30DData[0].Velocity < B->RQ30DData[0].Velocity )       return -1;	// MeasureValue Velocity True  => Skip
//	else if ( A->RQ30DData[0].Velocity == B->RQ30DData[0].Velocity ) return  0;
//	else                                                             return  1;	// false => Swap
//}

// 2024.09.16 for RQ30D Single Device Sort and select
int __fastcall CompareRQVelocity(void *Item1, void *Item2)
{
	STRQ30DData *A, *B;
	A = (STRQ30DData *)Item1;
	B = (STRQ30DData *)Item2;
	if ( A->Velocity < B->Velocity )       return -1;	// MeasureValue Velocity True  => Skip
	else if ( A->Velocity == B->Velocity ) return  0;
	else                                   return  1;	// false => Swap
}

//---------------------------------------------------------------------------
// TRQStatus
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constructor
TRQStatus::TRQStatus()
{
	memset(Address, 0x00, sizeof(Address));

	for ( int i = 0 ; i < 10 ; i++ ) {
		StartAck[i] = enStartIdle;
		FinishAck[i] = enFinishIdle;
	}

	Count = 0;
}
//---------------------------------------------------------------------------
// Destructor
TRQStatus::~TRQStatus()
{
	// nothing to do..
}
//---------------------------------------------------------------------------
void TRQStatus::SetDeviceAddress(UnicodeString DeviceAddress)	// (ex) 5,4,3,2,1, or  6,7,8,910
{
	Clear();

	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = DeviceAddress;

	if ( pStrList->Count < 0 ) {
		delete pStrList;	// release
		return;			// Address not exist
	}

	Count = pStrList->Count;	// Device Counter

	UnicodeString AddressStr = "";
	for ( int i = 0 ; i < pStrList->Count ; i++ ) {
		AddressStr = pStrList->Strings[i];
		try {
			Address[i] = StrToInt(AddressStr);
		} catch ( Exception &e ) {
			Address[i] = 0;	// Address => 0 => error address
		}
	}

	delete pStrList;
}
//---------------------------------------------------------------------------
int  TRQStatus::GetDeviceCount()
{
	return Count;
}
//---------------------------------------------------------------------------
void TRQStatus::Clear()	// All Status Clear
{
	//memset(Address, 0x00, sizeof(Address));
	for ( int i = 0 ; i < 10 ; i++ ) {
		StartAck[i] = enStartIdle;
		FinishAck[i] = enFinishIdle;
	}
	//Count = 0;
}
//---------------------------------------------------------------------------
int TRQStatus::GetDeviceAddress(int Index)
{
	return (Index < Count) ? Address[Index] : 0;
}
//---------------------------------------------------------------------------
int  TRQStatus::GetDeviceIndex(int Address)
{
	int Index = -1;
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Address ) {
			Index = i;
			break;
		}
	}
	return Index;
}
//---------------------------------------------------------------------------
// Start
//---------------------------------------------------------------------------
void TRQStatus::SetStartByAddress(int Addr)
{
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			StartAck[i] = enStartCmd;	// Send Start Command status
			break;
		}
	}
}

//---------------------------------------------------------------------------
void TRQStatus::SetStartByIndex(int Index)
{
	StartAck[Index] = enStartCmd;	// Send Start Command status
}

//---------------------------------------------------------------------------
void TRQStatus::SetStartAckByAddress(int Addr)
{
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			StartAck[i] = enStartAck;	// Send Start Command acked
			break;
		}
	}
}

//---------------------------------------------------------------------------
void TRQStatus::SetStartAckByIndex(int Index)
{
	StartAck[Index] = enStartAck;	// Send Start Command acked
}

//---------------------------------------------------------------------------
bool TRQStatus::IsStartAckByAddress(int Addr)
{
	bool bStartAck = false;
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			if (StartAck[i] == enStartAck) {
				bStartAck = true;
				break;
			} else {
				bStartAck = false;
				break;
			}
		}
	}
	return bStartAck;
}

//---------------------------------------------------------------------------
bool TRQStatus::IsStartAckByIndex(int Index)
{
	return (StartAck[Index] == enStartAck ) ? true : false;
}

//---------------------------------------------------------------------------
bool TRQStatus::IsAllStartAck()
{
	bool bStartAck = true;
	for ( int i = 0 ; i < Count ; i++ ) {
		if (StartAck[i] != enStartAck) {
			bStartAck = false;
			break;
		}
	}
	return bStartAck;
}

//---------------------------------------------------------------------------
// ¸¶Áö¸· DeviceÀÇ start Ack°¡ ÀÖ¾ú³ª?
bool TRQStatus::IsLastStartAck()
{
	if ( Count <= 0 ) return true;	// device°¡ ¾ø´Ù¸é ¹«Á¶°Ç True

	return (StartAck[Count-1] == enStartAck ) ? true : false;
}

//---------------------------------------------------------------------------
// Finish
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TRQStatus::SetFinishByAddress(int Addr)
{
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			FinishAck[i] = enFinishCmd;	// Send Finish Command status
			break;
		}
	}
}

//---------------------------------------------------------------------------
void TRQStatus::SetFinishByIndex(int Index)
{
	FinishAck[Index] = enFinishCmd;	// Send Finish Command status
}

//---------------------------------------------------------------------------
void TRQStatus::SetFinishAckByAddress(int Addr)
{
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			FinishAck[i] = enFinishAck;	// Send Finish Command Ack status
			break;
		}
	}
}

//---------------------------------------------------------------------------
void TRQStatus::SetFinishAckByIndex(int Index)
{
	FinishAck[Index] = enFinishAck;	// Send Finish Command Ack status
}

//---------------------------------------------------------------------------
bool TRQStatus::IsFinishAckByAddress(int Addr)
{
	bool bFinishAck = false;
	for ( int i = 0 ; i < Count ; i++ ) {
		if ( this->Address[i] == Addr ) {
			if (FinishAck[i] == enFinishAck) {
				bFinishAck = true;
				break;
			} else {
				bFinishAck = false;
				break;
			}
		}
	}
	return bFinishAck;
}

//---------------------------------------------------------------------------
bool TRQStatus::IsFinishAckByIndex(int Index)
{
	return (FinishAck[Index] == enFinishAck ) ? true : false;
}

//---------------------------------------------------------------------------
bool TRQStatus::IsAllFinishAck()
{
	bool bFinishAck = true;
	for ( int i = 0 ; i < Count ; i++ ) {
		if (FinishAck[i] != enFinishAck) {
			bFinishAck = false;
			break;
		}
	}
	return bFinishAck;
}

//---------------------------------------------------------------------------
// ¸¶Áö¸· DeviceÀÇ finish Ack°¡ ÀÖ¾ú³ª?
bool TRQStatus::IsLastFinishAck()
{
	if ( Count <= 0 ) return true; // device°¡ ¾ø´Ù¸é ¹«Á¶°Ç True

	return (FinishAck[Count-1] == enFinishAck) ? true : false;
}


//---------------------------------------------------------------------------
//
// TRQ30D Class
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constructor
TRQ30D::TRQ30D()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep = enSensorIdle;	// Sensor working status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = RQ30D_STR;

	memset(&MasterData, 0x00, sizeof(STRQ30DData));
	memset(&SlaveData, 0x00, sizeof(STRQ30DData));

	SlaveCount = 0;		// Master¿¡ ºÙ¾î ÀÖ´Â Slave Count

	pMasterPortThread = NULL; // Master Port Thread
	pSlavePortThread  = NULL; // Slave Port Thread

	for ( int i = 0 ; i < 10 ; i++ ) {
		m_pMasterList[i] = new TList();
		m_pSlaveList[i]  = new TList();
	}

	m_RecvMsg = "";
	m_MasterMessage = "";
	m_SlaveRecvMsg = "";
	m_SlaveMessage = "";

	m_ShareWaterLevelData = "";
}

//---------------------------------------------------------------------------
// Deconstructor
TRQ30D::~TRQ30D()
{
	// Thread Terminate
	if ( pMasterPortThread != NULL ) {
		try {
			delete pMasterPortThread;
		} catch (...) {
			// nothing to do..
		}
		pMasterPortThread = NULL;
	}
	if ( pSlavePortThread != NULL ) {
		try {
			delete pSlavePortThread;
		} catch (...) {
			// nothing to do..
		}
		pSlavePortThread = NULL;
	}

	// resource release -----------------------------------------------------

	TList *pList = NULL;
	STRQ30DData *pData = NULL;
	for ( int i = 0 ; i < 10 ; i++ ) {
		pList = m_pMasterList[i];
		if ( pList != NULL ) {
			if ( pList->Count > 0 ) {
				for ( int i = 0 ; i < pList->Count ; i++ ) {
					pData = (STRQ30DData *) pList->Items[i];
					delete pData;
				}
				pList->Clear();
			} else {
				// List is Empty
			}
			delete pList;
		} else {
			// List is NULL
		}

		pList = m_pSlaveList[i];
		if ( pList != NULL ) {
			if ( pList->Count > 0 ) {
				for ( int i = 0 ; i < pList->Count ; i++ ) {
					pData = (STRQ30DData *) pList->Items[i];
					delete pData;
				}
				pList->Clear();
			} else {
				// List is Empty
			}
			delete pList;
		} else {
			// List is NULL
		}
	}

	// Resource Release -----------------------------------------------------
	// 2025.02.21 RunTime Áß¿¡ RQ-30d Class°¡ ´Ù½Ã »ý¼ºµÉ ¼ö ÀÖÀ¸¹Ç·Î ÀÏ´Ü ÁÖ¼® Ã³¸®
//	delete MasterSection;		// for Master Port Thread
//	delete SlaveSection;		// for Slave Port Thread
//	delete MasterQueue;			// for Master Port Thread
//	delete SlaveQueue;			// for Slave Port Thread
//	delete MasterEvent;			// for Master Port Thread
//	delete SlaveEvent;			// fro Slave Port Thread
}

//---------------------------------------------------------------------------
// Clear
void TRQ30D::Clear()
{
	memset(m_Buffer, 0x00, BUFFER_8K);
	m_bBuffering = false;			// Buffering or not
	m_RecvMsg    = "";

	//m_pPort      = NULL;			// ComPort
	m_enStep     = enSensorIdle;	// Sensor Status
	m_enWorkingStep = enSensorIdle;	// Sensor working status

	MeasureDate = "";
	MeasureTime = "";

	HydroKind   = RQ30D_STR;

	memset(&MasterData, 0x00, sizeof(STRQ30DData));
	memset(&SlaveData,  0x00, sizeof(STRQ30DData));
	memset(&AirmarData, 0x00, sizeof(STAirmarData));

	// List data release ..
	TList *pList = NULL;
	STRQ30DData *pData = NULL;
	for ( int i = 0 ; i < 10 ; i++ ) {
		// Master data
		pList = m_pMasterList[i];
		if ( pList != NULL ) {
			if ( pList->Count > 0 ) {
				for ( int i = 0 ; i < pList->Count ; i++ ) {
					pData = (STRQ30DData *) pList->Items[i];
					delete pData;
				}
				pList->Clear();
			} else {
				//TList is Empty
			}
			//delete pList;
		} else {
			// TList is Null
		}
		// Slave data..
		pList = m_pSlaveList[i];
		if ( pList != NULL ) {
			if ( pList->Count > 0 ) {
				for ( int i = 0 ; i < pList->Count ; i++ ) {
					pData = (STRQ30DData *) pList->Items[i];
					delete pData;
				}
				pList->Clear();
			} else {
				//TList is Empty
			}
			//delete pList;
		} else {
			// TList is NULL
		}
	}

	MasterStatus.Clear();
	SlaveStatus.Clear();

	m_RecvMsg = "";
	m_MasterMessage = "";
	m_SlaveRecvMsg = "";
	m_SlaveMessage = "";

	m_ShareWaterLevelData = "";
}

//---------------------------------------------------------------------------
// Init
void TRQ30D::Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo)
{
	m_pPort = pPort;
	m_enHydroNo = enHydroNo;
	m_RHydroMeter.enHydroNo = enHydroNo;	// HydroMeter1 or not
	m_RHydroCellContainer.m_enHydroNo = enHydroNo;	// HydroMeter1Cell or not
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30D::SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance)
{
	// nothing to do..
}

//---------------------------------------------------------------------------
// Start
bool TRQ30D::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	bool bResult = false;

	Clear();	// Status Clear

	this->MeasureDate = MeasureDate;
	this->MeasureTime = MeasureTime;
	this->m_enStep    = enSensorStart;

	m_TimeStamp = Now();
	
	StartRQ();

	LogMessage("RQ-30d started..");

	return bResult;
}

//---------------------------------------------------------------------------
// Finish
bool TRQ30D::Finish()
{
	bool bResult = false;
	m_enStep = enSensorFinish;

	bResult = Save(MeasureDate, MeasureTime);

	m_enStep = enSensorIdle;

	LogMessage("RQ-30d finished..");

	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRQ30D::Decode(AnsiString RecvMessage, int iLen)
{
//@ÃøÁ¤½ÃÀÛ ÀÀ´ä
//#A0001ok$mt|4FA9;
//@µ¥ÀÌÅÍÃâ·Â ÀÀ´ä
//#A0001ok$pt|8C35;
//@ÃøÁ¤µ¥ÀÌÅÍ Ãâ·Â
// RQ-30 Style => G00, G01, G02, G03
// #M0001G00se00      15|01    1.35|02 -99.000|03  -18.90|04 -153.85|05    1.94|2827;	// Measurement value
//#M0001G01se06 -99.000|07 -153.85|08     350|09   12.46|2815;                          // Special value
//#M0001G02se10       0|11       1|12       0|13      12|14    3494|15       8|3856;    // Analysis value #1
//#M0001G03se16       0|17       0|1899999998|19    29.4|20    21.5|BEF6;               // Analysis value #2
// RQ-30d Style => G01,G02,...G04
// #M0002G01se0199999998|0299999998|03   0.777|04   76.09|05   35.74|0699999998|F0D5;
//#M0002G02se0799999998|08    0.00|09       7|10   24.09|D746;
//#M0002G03se11    2727|12       3|13   20698|14     133|15      27|16       2|426A;
//#M0002G04se17      10|18       0|1999999998|20    58.2|21    36.4|10D7;
// #M0001G01se0199999998|02    2.49|03   0.444|04   56.97|05    0.68|06   36.42|F4E0;
//#M0001G02se07   0.380|08    0.59|09      11|10   24.17|7946;
//#M0001G03se11     836|12      83|13       7|14      41|15    4809|16       0|EF0D;
//#M0001G04se17       6|18       0|19   99977|20    61.6|21    37.6|EB1B;

// for debug
//LogMessage(RecvMessage);

	RecvMessage = Trim(RecvMessage);	// ¾Õ µÚÀÇ Space¸¦ Áö¿î´Ù. \r\nµµ ³¯¸°´Ù.
										// “S/òòšz½™ÿÙù?Ëß??#A0008ok$pt|2449; => 2024.06.06
										// ¾ÕºÎºÐ¿¡ ³ëÀÌÁî°¡ ²¸¼­ ±úÁö¸é #À» Ã£´Â´Ù.
	int Pos = RecvMessage.AnsiPos("#");
	if ( Pos == 0 ) { // not find
		// noting to do..
	} else if ( Pos == 1 ) {
		// nothing to do..
	} else { // Pos > 1
		RecvMessage = RecvMessage.SubString(Pos, RecvMessage.Length() - Pos + 1 );
	}
	
	AnsiString Start        = RecvMessage.SubString(1,1);           //#
	AnsiString Id           = RecvMessage.SubString(2,1);           //A:ÀÀ´ä, M:µ¥ÀÌÅÍ
	AnsiString SystemKey    = RecvMessage.SubString(3,2);           //00
	AnsiString DeviceNumber = RecvMessage.SubString(5,2);			//01 ~ 08 Device Address
	AnsiString Command      = RecvMessage.SubString(7, RecvMessage.Length()-6);	
	
	///////////////////////////////////////////////////////////////////////////
	// ÃøÁ¤ ½ÃÀÛ/Á¾·á Command¿¡ ´ëÇÑ ÀÀ´ä
	if ( Id == "A" ) { // Ä¿¸àµå¿¡ ´ëÇÑ ÀÀ´ä
//debug
LogMessage(RecvMessage);
		int Pos, Pos2;
		Pos  = RecvMessage.AnsiPos("$");		//#A0001ok$mt|4FA9;
		Pos2 = RecvMessage.AnsiPos("|");
		if ( Pos > 6 && Pos2 > Pos ) {
			AnsiString Result = "";
			AnsiString Cmd    = "";
			try {
				Result = RecvMessage.SubString(7, Pos-7);	     //#A0001ok$mt|4FA9; => "ok"
				Cmd    = RecvMessage.SubString(Pos, Pos2-Pos);	 //#A0001ok$mt|4FA9; => "$mt"
			} catch ( ... ) {
				// nothing to do
			}

			int DeviceAddress = 0;
			try {
				DeviceAddress = StrToInt(DeviceNumber);
			} catch (...) {
				DeviceAddress = 0;
			}
			if ( DeviceAddress == 0 ) {
				LogMessage("RQ-30d Device Address fail..");
				return false;
			}

			if ( Cmd == "$mt") {			// Start command
				if ( Result == "ok" ) {
					MasterStatus.SetStartAckByAddress(DeviceAddress);
					LogMessage("RQ-30d #" + DeviceNumber + " start acked..");
				} else {
					// not ok
					LogMessage("RQ-30d #" + DeviceNumber + " start not acked..");
				}
			} else if ( Cmd == "$pt" ) {	// DataRequest
				if ( Result == "ok" ) {
					MasterStatus.SetFinishAckByAddress(DeviceAddress);
					LogMessage("RQ-30d #" + DeviceNumber + " data acked..");
				} else {
					// not ok
					LogMessage("RQ-30d #" + DeviceNumber + " data not acked..");
				}
			} else {
				// not ok
				LogMessage("RQ-30d #" + DeviceNumber + " data not acked..");
			}
		} else { // ¼ö½Å °á°ú°¡ Çü½Ä¿¡ ¸ÂÁö ¾ÊÀ» ¶§..
			LogMessage("RQ-30d unknown response");
			return false;
		}

	///////////////////////////////////////////////////////////////////////////
	// ÃøÁ¤ °á°ú µ¥ÀÌÅÍ Decoding

	} else if ( Id == "M" ) { // ÃøÁ¤ °á°ú µ¥ÀÌÅÍ

		TStringList *pStrList = new TStringList();

		pStrList->Delimiter = '|';
		pStrList->StrictDelimiter = true;
		pStrList->DelimitedText = RecvMessage;

		if ( pStrList->Count <= 0 ) {
			LogMessage("RQ-30d lack of token..");
			delete pStrList;	// release
			return false;
		}

		AnsiString Prefix = pStrList->Strings[0];	//#M0001G00 => Main Value, #M0001G01 => Special Value
		AnsiString x = "";
		try {
			x = Prefix.SubString(7,3);
		} catch ( Exception &e ) {
			delete pStrList;
			return false;
		}

		AnsiString Token;
		AnsiString No[6];
		AnsiString Value[6];

		// RawData Add
		MasterData.RawData += RecvMessage + "\r\n";
		m_MasterMessage += RecvMessage + "\r\n";

		// Extract Addres
		AnsiString AddressStr = Prefix.SubString(5,2);	// #M0001G00 => 01 (Device Number)
		int Address = 0;
		try {
			Address = StrToInt(AddressStr);
		} catch ( Exception &e ) {
			Address = 0;
			delete pStrList;
			return false;	// Address decode  fail..
		}
		MasterData.Address = Address;	// Device Address

		if ( x == "G01" ) { 	// Main Value => #M0001G01se0199999998|02    2.49|03   0.444|04   56.97|05    0.68|06   36.42|F4E0;
			if ( pStrList->Count < 6 ) {
				LogMessage("RQ-30d Lack of token..main value");
				delete pStrList;
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G01se0199999998     => AUX
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//02    2.49	=> WaterLevel
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//03   0.444	=> Velocity
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//04   56.97	=> Quality
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//05    0.68	=> Discharge
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[5]; 	//06   36.42	=> Total Discharge
			No[5]    = Token.SubString(1,2);
			Value[5] = Token.SubString(3, Token.Length()-3+1);

			int    iNo = 0;
			int    iValue = 0;
			double dValue = 0.0f;
			for ( int i = 0 ; i < 6 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				if ( iNo == 1 ) {
					try {
						iValue = StrToInt(Value[i]);
					} catch ( Exception &e ) {
						iValue = 0.0f;
					}
				} else {
					try {
						dValue = StrToFloat(Value[i]);
					} catch ( Exception &e ) {
						dValue = 0.0f;
					}
				}

				if ( iNo == 1 ) MasterData.SelfCheck      = iValue;
				if ( iNo == 2 ) MasterData.Waterlevel     = dValue;
				if ( iNo == 3 ) MasterData.Velocity       = dValue;
				if ( iNo == 4 ) MasterData.Quality        = dValue;
				if ( iNo == 5 ) MasterData.Discharge      = dValue;
				if ( iNo == 6 ) MasterData.TotalDischarge = dValue;
			} // end for..
			// Save Shared to SlavePort
			if ( Address == 1 ) {	// Master Device Main Value => Shave WaterLevel Data
				m_ShareWaterLevelData = RecvMessage+"\r\n";
			}
			MasterData.Complete |= RQ30D_MAIN;

		} else if ( x == "G02" ) {	// Special Vaue => #M0001G02se07   0.380|08    0.59|09      11|10   24.17|7946;
			if ( pStrList->Count < 4 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				delete pStrList;
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G02se07   0.380 => Learn Velocity
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//08    0.59	=> Learned Discharge
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//09      11	=> Opposite Direction content
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//10   24.17	=> Supply Voltage
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			double dValue = 0;
			for ( int i = 0 ; i < 4 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				try {
					dValue = StrToFloat(Value[i]);
				} catch ( Exception &e ) {
					dValue = 0.0f;
				}

				if ( iNo == 7 ) MasterData.LearnVelocity   = dValue;
				if ( iNo == 8 ) MasterData.LearnDischarge  = dValue;
				if ( iNo == 9 ) MasterData.OppositeContent = dValue;
				if ( iNo == 10) MasterData.SupplyVoltage   = dValue;
			}
			MasterData.Complete |= RQ30D_SPECIAL;

		} else if (x == "G03") { // Analysis Value 01 => #M0001G03se11     836|12      83|13       7|14      41|15    4809|16       0|EF0D;
			if ( pStrList->Count < 6 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				delete pStrList;
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G03se11     836 => Peak
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//12      83	=> CSR
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//13       7	=> AreaPeak
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//14      41	=> RMS
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//15    4809	=> Amp
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[5]; 	//16       0 	=> AmpRelation
			No[5]    = Token.SubString(1,2);
			Value[5] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			int  iValue = 0;
			for ( int i = 0 ; i < 6 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				try {
					iValue = StrToInt(Value[i]);
				} catch ( Exception &e ) {
					iValue = 0;
				}

				if ( iNo == 11 ) MasterData.PeakWidth   = iValue;
				if ( iNo == 12 ) MasterData.CSR         = iValue;
				if ( iNo == 13 ) MasterData.AreaPeak    = iValue;
				if ( iNo == 14 ) MasterData.RmsPic      = iValue;
				if ( iNo == 15 ) MasterData.Amp         = iValue;
				if ( iNo == 16 ) MasterData.AmpRelation = iValue;
			} // end for
			MasterData.Complete |= RQ30D_ANALYSIS1;

		} else if (x == "G04") { // Analysis Value 02 => #M0001G04se17       6|18       0|19   99977|20    61.6|21    37.6|EB1B;
			if ( pStrList->Count < 5 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				delete pStrList;
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G04se17       6
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//18       0
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//19   99977
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//20    61.6
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//21    37.6
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			int  iValue = 0;
			double dValue = 0.0f;
			for ( int i = 0 ; i < 5 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				if ( iNo < 20 ) {
					try {
						iValue = StrToInt(Value[i]);
					} catch ( ... ) {
						iValue = 0;
					}
				} else {	// iNo == 20 => Angle  20,21 => double variable
					try {
						dValue = StrToFloat(Value[i]);
					} catch ( ... ) {
						dValue = 0;
					}
				}

				if ( iNo == 17 ) MasterData.Signal     = iValue;
				if ( iNo == 18 ) MasterData.ErrorCode  = iValue;
				if ( iNo == 20 ) MasterData.Angle      = dValue;
			} // end for
			MasterData.Complete |= RQ30D_ANALYSIS2;
		} else {
			// unknown command
		}

		if (MasterData.Complete == (RQ30D_MAIN | RQ30D_SPECIAL | RQ30D_ANALYSIS1 | RQ30D_ANALYSIS2)) {	// Data Complete => Send Message complete
// debug
LogMessage(m_MasterMessage); m_MasterMessage = "";

			STRQ30DData *pData = new STRQ30DData();
			memcpy(pData, &MasterData, sizeof(STRQ30DData));

			TList *pList = GetMasterListByAddress(Address);
			if ( pList == NULL ) {
				// noting to do..
			} else {
				pList->Add(pData);
			}
			memset(&MasterData, 0x00, sizeof(STRQ30DData));	// 1Set ¹ÞÀº µ¥ÀÌÅÍ¸¦ ClearÇÑ´Ù. => ´ÙÀ½ ¼ö½ÅÀ» À§ÇØ¼­

			if ( theEnv.m_HydroUseSlavePort == ON_STR ) {  // Master Port¸¸ ¿î¿µµÇ´Â »óÈ²
				if ( MasterStatus.IsAllFinishAck() || MasterStatus.IsLastFinishAck() ) {	// ¸ðµç Device or last Deviced°¡ ¼ö½ÅµÇ¾ú´Ù¸é
					if ( SlaveStatus.IsAllFinishAck() || SlaveStatus.IsLastFinishAck() ) {	// SlaveÀÇ ¸ðµç Device or Last Device°¡ ¼ö½ÅµÇ¾ú¾Æ¸é
						FormMain->TimerHydro1->Enabled = false;	// HydroTimer disable
						m_enWorkingStep = enSensorIdle;
						SleepMsg(100);	// ¼öÀ§ µ¥ÀÌÅÍ¸¦ °øÀ¯ÇÏ±â Àü¿¡ Àá±ñ ±â´Ù¸°´Ù.
						if ( !m_ShareWaterLevelData.IsEmpty() ) {
							SendWaterLevelToSlavePort(m_ShareWaterLevelData);	// MasterPort¿¡¼­ ¹ÞÀº ¼öÀ§ µ¥ÀÌÅÍ¸¦ Slave Port·Î °øÀ¯ÇÑ´Ù.
						} else {
							// °øÀ¯ÇÒ ¼öÀ§ µ¥ÀÌÅÍ°¡ ¾øÀ¸¹Ç·Î Skip => Master DeviceÀÇ Main Value buffring ¾ÈµÊ. => error
						}
					} else {
						// Slave Device ¾ÆÁ÷ ¼ö½Å Áß => ¼öÀ§ µ¥ÀÌÅÍ ¾ÆÁ÷ °øÀ¯ÇÒ ¼ö ¾øÀ½ => ´ÙÀ½ Àåºñ ¼ö½Å ¶Ç´Â Time ExpireµÉ ¶§ Ã³¸®
					}
				} else {
					// Master Device Ack ¾ÆÁ÷ ³¡³ªÁö ¾ÊÀ½ => continue buffering
				}
			} else {	// Master Port¸¸ ¿î¿µµÇ´Â »óÈ²
				if ( MasterStatus.IsAllFinishAck() || MasterStatus.IsLastFinishAck() ) {	// ¸ðµç Device or last Deviced°¡ ¼ö½ÅµÇ¾ú´Ù¸é
					// Hydro timer disable
					FormMain->TimerHydro1->Enabled = false;	//
					m_enWorkingStep = enSensorIdle;
				} else {
					// ¾ÆÁ÷ ¸ðµç Device°¡ ¼ö½ÅµÇÁö ¾Ê´Â »óÈ² => continue buffering
				}
			}
		} // ¸ðµç µ¥ÀÌÅÍ ¼ö½Å(Main/Special/Analysis Value ¼ö½Å ¿Ï·á)

		m_TimeStamp = Now();	// TimeStamp refresh
		delete pStrList;

	} else {	// unknown id => ¾ÆÁ÷ ¸ð¸£´Â Command
		// unknown command
		LogMessage("RQ-30d unknown command received..");
	}
	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TRQ30D::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRQ30D::Receive(AnsiString RecvMsg, int iLen)
{
// RQ-30d´Â Idle »óÅÂ¿¡¼­µµ ÃøÁ¤ ÀÛ¾÷À» Áö¼ÓÇÑ´Ù.
	if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_RecvMsg += RecvMsg;

	int Pos = 0;
	AnsiString RecvMessage = "";

	Pos = m_RecvMsg.AnsiPos("\r\n");
	while ( Pos > 0 ) {

		RecvMessage = m_RecvMsg.SubString(1, Pos-1);
		RecvMessage = Trim(RecvMessage);

		Decode(RecvMessage, RecvMessage.Length());

		m_RecvMsg = m_RecvMsg.SubString(Pos+2, m_RecvMsg.Length() - (Pos+1));
		Pos = m_RecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TRQ30D::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Send
bool TRQ30D::Send(AnsiString RecvMsg, int iLen)
{
	if ( m_pPort == NULL ) return false;

	try {
		m_pPort->FlushOutBuffer();
		SleepMsg(10);

		m_pPort->PutString(RecvMsg);
		LogMessage("[RQ30D]=>" + RecvMsg);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Save
bool TRQ30D::Save(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	// Hydro Timer Stop
	FormMain->TimerHydro1->Enabled = false;	// Hydro Timer disabled..

	// Thread Terminate
	//if ( pMasterPortThread != NULL ) delete pMasterPortThread;
	//if ( pSlavePortThread != NULL ) delete pSlavePortThread;

	CaculateRQ30DData();   	// RQ30 data => RQ30DFinalSet °áÁ¤µÊ.
	Value2DB();	// Value Mapping ( include Airmar Data)

	m_RHydroMeter.MeasureDate = MeasureDate;
	m_RHydroMeter.MeasureTime = MeasureTime;
	m_RHydroMeter.HydroKind   = HydroKind;

	bool bResult = false;
	bResult = m_RHydroMeter.Insert();
	
	if ( !bResult ) {
		LogMessage("RQ-30d insert fail");
		return false;
	}
	
	TRHydroMeterCellContainer HydroCellContainer;
	STRQ30DData Data;
	TRHydroMeterCell *pHydroMeterCell = NULL;
	for ( int i = 1 ; i < 10 ; i++ ) {  	// index start 1 => slave #1..
		Data = RQ30DFinalSet.RQ30DData[i];

		if ( Data.Address == 0 ) continue;	// ºñ¾îÀÖ´Â AddressÀÇ µ¥ÀÌÅÍ => Skip
		
		pHydroMeterCell = new TRHydroMeterCell(m_enHydroNo);
		
		pHydroMeterCell->MeasureDate = MeasureDate;
		pHydroMeterCell->MeasureTime = MeasureTime;
		pHydroMeterCell->CellNo      = Data.Address;
		pHydroMeterCell->Value01     = Data.Waterlevel;
		pHydroMeterCell->Value02     = Data.Velocity;
		pHydroMeterCell->Value03     = Data.Quality;
		pHydroMeterCell->Value04     = Data.Discharge;
		pHydroMeterCell->Value05     = Data.TotalDischarge;
		pHydroMeterCell->Value06     = Data.LearnVelocity;
		pHydroMeterCell->Value07     = Data.LearnDischarge;
		pHydroMeterCell->Value08     = Data.OppositeContent;
		pHydroMeterCell->Value09     = Data.SupplyVoltage;
		pHydroMeterCell->Value10     = Data.PeakWidth;
		pHydroMeterCell->Value11     = Data.CSR;
		pHydroMeterCell->Value12     = Data.AreaPeak;
		pHydroMeterCell->Value13     = Data.RmsPic;
		pHydroMeterCell->Value14     = Data.Amp;
		pHydroMeterCell->Value15     = Data.AmpRelation;
		pHydroMeterCell->Value16     = Data.Signal;
		pHydroMeterCell->Value17     = Data.ErrorCode;
		//pHydroMeterCell->Value18     = theEnv.Offset;

		pHydroMeterCell->Value26     = Data.SelfCheck;	// AUX/SelfCheck
		pHydroMeterCell->Value27     = Data.Angle;		// Angle

		//pHydroMeterCell->RawData     = Data.RawData;     // RQ30 RawData

		HydroCellContainer.Add(pHydroMeterCell);
	}
	
	bResult = HydroCellContainer.Save(MeasureDate, MeasureTime);

	return bResult; // Container destoryµÉ ´ë Resource release µÊ..
}

//---------------------------------------------------------------------------
// DB2Value
void TRQ30D::DB2Value()
{
	// RQ30 Data
//	RQ30Data.Waterlevel 	   = m_RHydroMeter.Value01;   // WaterLevel
//	RQ30Data.Velocity		   = m_RHydroMeter.Value02;   // Velocity
//	RQ30Data.Quality		   = m_RHydroMeter.Value03;	  // SNR
//	RQ30Data.Discharge   	   = m_RHydroMeter.Value04;	  // Discharge
//	RQ30Data.Area              = m_RHydroMeter.Value05;	  // Area
//	RQ30Data.LearnVelocity	   = m_RHydroMeter.Value06;	  // LVelocity
//	RQ30Data.LearnDischarge    = m_RHydroMeter.Value07;	  // LDischarge
//	RQ30Data.OppositeContent   = m_RHydroMeter.Value08;	  // OppDirection
//	RQ30Data.SupplyVoltage	   = m_RHydroMeter.Value09;	  // Voltage
//	RQ30Data.PeakWidth         = m_RHydroMeter.Value10;	  // PeakWidth
//	RQ30Data.CSR               = m_RHydroMeter.Value11;	  // CSR
//	RQ30Data.AreaPeak          = m_RHydroMeter.Value12;	  // AreaPeak
//	RQ30Data.RmsPic            = m_RHydroMeter.Value13;	  // RMS
//	RQ30Data.Amp               = m_RHydroMeter.Value14;	  // Amp
//	RQ30Data.AmpRelation       = m_RHydroMeter.Value15;	  // AmpRelation
//	RQ30Data.Signal            = m_RHydroMeter.Value16;	  // AmpRelation
//	RQ30Data.ErrorCode         = m_RHydroMeter.Value17;	  // AmpRelation
//
//	// Airmar Data
//	AirmarData.Direction       = m_RHydroMeter.Value20;	  // Degree
//	AirmarData.Wind		       = m_RHydroMeter.Value21;	  // meter/sec
//	AirmarData.WindGust        = m_RHydroMeter.Value22;	  // meter/sec
//	AirmarData.Pressure        = m_RHydroMeter.Value23;	  // bar
//	AirmarData.Temperature     = m_RHydroMeter.Value24;	  // 'C
//	AirmarData.Humidity        = m_RHydroMeter.Value25;	  // %
//
//	RQ30Data.RawData           = m_RHydroMeter.RawData;   // RQ30 RawData
}

//---------------------------------------------------------------------------
// Value2DB
void TRQ30D::Value2DB()
{
	// RQ-30d Data
	m_RHydroMeter.HydroKind   = HydroKind;
	m_RHydroMeter.AvgVelocity = RQ30DFinalSet.RQ30DData[0].Velocity;		 // Velocity
	m_RHydroMeter.MinVelocity = RQ30DFinalSet.RQ30DData[0].Velocity;		 // Velocity
	m_RHydroMeter.MaxVelocity = RQ30DFinalSet.RQ30DData[0].Velocity;		 // Velocity
	m_RHydroMeter.Value01     = RQ30DFinalSet.RQ30DData[0].Waterlevel;   	 // WaterLevel
	m_RHydroMeter.Value02	  = RQ30DFinalSet.RQ30DData[0].Velocity;         // Velocity
	m_RHydroMeter.Value03	  = RQ30DFinalSet.RQ30DData[0].Quality;		     // SNR
	m_RHydroMeter.Value04	  = RQ30DFinalSet.RQ30DData[0].Discharge;		 // Discharge
	//m_RHydroMeter.Value05	  = RQ30DFinalSet.RQ30DData[0].TotalDischarge;   // Area/TotalDischarge
	m_RHydroMeter.Value05	  = RQ30DFinalSet.TotalQ;						 // TotalQ
	m_RHydroMeter.Value06	  = RQ30DFinalSet.RQ30DData[0].LearnVelocity;	 // LVelocity
	m_RHydroMeter.Value07	  = RQ30DFinalSet.RQ30DData[0].LearnDischarge;   // LDischarge
	m_RHydroMeter.Value08	  = RQ30DFinalSet.RQ30DData[0].OppositeContent;  // OppDirection
	m_RHydroMeter.Value09	  = RQ30DFinalSet.RQ30DData[0].SupplyVoltage;	 // Voltage
	m_RHydroMeter.Value10	  = RQ30DFinalSet.RQ30DData[0].PeakWidth;        // PeakWidth
	m_RHydroMeter.Value11	  = RQ30DFinalSet.RQ30DData[0].CSR;              // CSR
	m_RHydroMeter.Value12	  = RQ30DFinalSet.RQ30DData[0].AreaPeak;         // AreaPeak
	m_RHydroMeter.Value13	  = RQ30DFinalSet.RQ30DData[0].RmsPic;           // RMS
	m_RHydroMeter.Value14	  = RQ30DFinalSet.RQ30DData[0].Amp;              // Amp
	m_RHydroMeter.Value15	  = RQ30DFinalSet.RQ30DData[0].AmpRelation;      // AmpRelation
	m_RHydroMeter.Value16	  = RQ30DFinalSet.RQ30DData[0].Signal;           // Signal
	m_RHydroMeter.Value17	  = RQ30DFinalSet.RQ30DData[0].ErrorCode;        // ErrorCode
	//m_RHydroMeter.Value18	  = theEnv.m_WaterOffset;	   // Offset
	m_RHydroMeter.Value18	  = 0.0f;					   // Offset 2022.05.11 RQ-30Àº OffsetÀ» Àû¿ëÇÏÁö ¾ÊÀ½.
	//m_RHydroMeter.Value19     = RQ30DFinalSet.RQ30DData[0].TotalDischarge;	 // TotalDischarge
	m_RHydroMeter.Value19     = RQ30DFinalSet.TotalLQ;	// 2023.07.18 Master LQ + Slaves LQ

	// Airmar Data
	m_RHydroMeter.Value20	  = AirmarData.Direction;	   // Degree
	m_RHydroMeter.Value21	  = AirmarData.Wind;		   // meter/sec
	m_RHydroMeter.Value22	  = AirmarData.WindGust;       // meter/sec
	m_RHydroMeter.Value23	  = AirmarData.Pressure;       // bar
	m_RHydroMeter.Value24	  = AirmarData.Temperature;    // 'C
	m_RHydroMeter.Value25	  = AirmarData.Humidity;       // %

	m_RHydroMeter.Value26	  = RQ30DFinalSet.RQ30DData[0].SelfCheck;	// AUX/SelfCheck
	m_RHydroMeter.Value27	  = RQ30DFinalSet.RQ30DData[0].Angle;		// Angle

	m_RHydroMeter.RawData     = RQ30DFinalSet.RQ30DData[0].RawData;     // RQ30 RawData
}

//---------------------------------------------------------------------------
// Æò±ÕÀ¯¼ÓÀ» ±¸ÇÑ´Ù.
double TRQ30D::GetAvgVelocity()
{
	return 0.0f;
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30D::SetBreak(bool bBreak)
{
	m_bBreak = bBreak;
}

//---------------------------------------------------------------------------
// Virtual Function
bool TRQ30D::GetBreakStatus()
{
	return m_bBreak;
}

//---------------------------------------------------------------------------
// Virtual Function
void TRQ30D::SendBreak(int iMiliSecond)
{
	if ( m_pPort == NULL ) return;
	if ( m_pPort->Open == false ) return;

	m_pPort->SetBreak(true);
	SleepMsg(iMiliSecond);	// 900ms => 500ms
	m_pPort->SetBreak(false);
	//Sleep(100);			// 100ms
}

//---------------------------------------------------------------------------
void TRQ30D::SetSlavePort(TApdComPort *pPort)	// SlavePort¸¦ ÁöÁ¤ÇÑ´Ù.
{
	m_pSlavePort = pPort;	// Slave Thread°¡ »ç¿ëÇÒ Port¸¦ ÁöÁ¤ÇÑ´Ù.
}

//---------------------------------------------------------------------------
// 1ºÐ ¸¶´Ù ÃøÁ¤À» ½ÃÀÛÇÒ ¶§
void  TRQ30D::StartRQ()	// 1ºÐ ´ÜÀ§ Starrt
{
	// ¼¾¼­ »óÅÂ¿Í  »ó°ü¾øÀÌ ¹«Á¶°Ç ½ÃÀÛÇÑ´Ù.
	//if ( m_enWorkingStep == enSensorWorking ) {
	//	LogMessage("RQ-30d already started..");
	//	return;
	//}

	// RQ-30d Timer Enable
	FormMain->TimerHydro1->Enabled = false;   // Timer´Â µ¥ÀÌÅÍ ¼öÁýÇÒ ¶§¸¸ Enable ½ÃÅ²´Ù.

	ClearRQ();	// 1ºÐ ÃøÁ¤ Àç½ÃÀÛ

	m_enWorkingStep = enSensorStart;	// 1ºÐ´ÜÀ§ ÀÛ¾÷ ½ÃÀÛ

	// At First, Slave Port start ----------------------------------------------

	if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
		STRQ30DMessage *pRQ30DMessage = new STRQ30DMessage();
		pRQ30DMessage->Action = RQ30D_ACTION_START;

		SlaveSection->Enter();
		SlaveQueue->Push(pRQ30DMessage);
		SlaveSection->Leave();

		SlaveEvent->SetEvent();

		SleepMsg(500);	// Wait 500ms => Slave Thread, Master ThreadÀÇ ½Ã°£Â÷¸¦ ÁÖ±â À§ÇÏ¿© ÀÌ·¸°Ô ÇÔ.
	}

	// Master Port Thread ----------------------------------------------------

	STRQ30DMessage *pRQ30DMessage = new STRQ30DMessage();
	pRQ30DMessage->Action = RQ30D_ACTION_START;

	MasterSection->Enter();
	MasterQueue->Push(pRQ30DMessage);
	MasterSection->Leave();

	MasterEvent->SetEvent();

	// AirmarLinker¿¡ µ¥ÀÌÅÍ¸¦ ¿äÃ»ÇÑ´Ù.(AirmarLinker°¡ ÀÖ´Ù¸é)
	if ( FormMain->m_pAirmar != NULL && FormMain->m_pAirmar->m_bLinker ) {
		FormMain->m_pAirmar->SendAirmarLinker();
	} else {
		// AirmarLinker¿¡ ´ëÇÑ ÀÛ¾÷Àº ¾øÀ½.
	}
}

//---------------------------------------------------------------------------
// 1ºÐ ´ÜÀ§ Data Clear 
void  TRQ30D::ClearRQ()	// 1ºÐ ´ÜÀ§ Data Clear
{
	m_enWorkingStep = enSensorIdle;

	MasterStatus.Clear();
	SlaveStatus.Clear();

	m_RecvMsg = "";
	m_MasterMessage = "";
	m_SlaveRecvMsg = "";
	m_SlaveMessage = "";

	m_ShareWaterLevelData = "";	// Slave Port·Î °øÀ¯ÇÒ ¼öÀ§ µ¥ÀÌÅÍ string ( Address #1ÀÇ Main Value )

	memset(&MasterData, 0x00, sizeof(STRQ30DData));
	memset(&SlaveData, 0x00, sizeof(STRQ30DData));
}

//---------------------------------------------------------------------------
void  TRQ30D::FinishRQ()	// 1ºÐ ´ÜÀ§ ÃøÁ¤ Á¾·á => Timer¿¡ ÀÇÇÑ °­Á¦ Á¾·á ½Ã »ç¿ëÇÑ´Ù.
{
	if ( m_enWorkingStep != enSensorStart && m_enWorkingStep != enSensorWorking ) {
		LogMessage("RQ-30d is not started.. ");
		return;
	}

	m_enWorkingStep = enSensorFinish;

	// Hydro Timer Disable
	// RQ-30dÀÇ HydroTimer´Â µ¥ÀÌÅÍ ¼öÁýÇÒ ¶§¸¸ »ç¿ëÇÑ´Ù.
	int MaxDeviceCount = (MasterStatus.GetDeviceCount() >= SlaveStatus.GetDeviceCount()) ? MasterStatus.GetDeviceCount() : SlaveStatus.GetDeviceCount();
	int EstimateSecond = MaxDeviceCount * 3 + 3;	// °¢ Device´ç 3ÃÊ¿Í Ãß°¡½Ã°£ 3ÃÊ¸¦ ÁöÁ¤ÇÑ´Ù. ( Àåºñ 5´ë: 5 * 3 + 3 => 15ÃÊ Timer°¡ µÊ
	FormMain->TimerHydro1->Interval = EstimateSecond * 1000;	// 58 sec Timer => µ¥ÀÌÅÍ ¼ö½ÅÀÌ Á¤»óÀûÀÌ¸é Timer Event ¹ß»ýÇÏ±â Àü¿¡ DisableµÈ´Ù.
	FormMain->TimerHydro1->Enabled = true;

	// At First, Slave Port start ----------------------------------------------

	if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
		STRQ30DMessage *pRQ30DMessage = new STRQ30DMessage();
		pRQ30DMessage->Action = RQ30D_ACTION_FINISH;

		SlaveSection->Enter();
		SlaveQueue->Push(pRQ30DMessage);
		SlaveSection->Leave();

		SlaveEvent->SetEvent();

		SleepMsg(500);	// Wait 500ms => Slave Thread, Master ThreadÀÇ ½Ã°£Â÷¸¦ ÁÖ±â À§ÇÏ¿© ÀÌ·¸°Ô ÇÔ.
	}

	// Master Port Thread ----------------------------------------------------

	STRQ30DMessage *pRQ30DMessage = new STRQ30DMessage();
	pRQ30DMessage->Action = RQ30D_ACTION_FINISH;

	MasterSection->Enter();
	MasterQueue->Push(pRQ30DMessage);
	MasterSection->Leave();

	MasterEvent->SetEvent();
}

//---------------------------------------------------------------------------
// FinishForceRQ() => 1ºÐÀ§ ÃøÁ¤ °­Á¦ Á¾·á(By Timer)
// FinishForrceRQ()´Â HydroTimer¿¡ ÀÇÇØ ¹ß»ýÇÑ´Ù. ¹ß»ýÇÏ´Â »óÈ²: RQ-30DÀÇ µ¥ÀÌÅÍ ¼ö¿ë ½Ã°£ÀÌ Á¶°ú µÇ¾úÀ¸³ª µ¥ÀÌÅÍ SetÀÌ
//   ¸ðµÎ ¼ö½ÅµÇÁö ¾Ê¾ÒÀ» ¶§ TimeExpire·Î ¹ß»ýÇÑ´Ù. DataAck °ªÀ» Ã¼Å©ÇÏ¿© µ¥ÀÌÅÍ ¼ö½ÅÀÌ ÀÖ¾úÀ¸¸é DataSet¿¡ »ó°ü¾øÀÌ
//   STRQ30DSetÀ» ¸¸µé¾î¼­ ÀúÀåÇÑ´Ù.
//---------------------------------------------------------------------------
void TRQ30D::FinishForceRQ()	// 1ºÐ ´ÜÀ§ ÃøÁ¾ Á¾·á(°­Á¦ Á¾·á=>RQ/RG¿¡¼­ µ¥ÀÌÅÍ°¡ ¸ðµÎ ¼ö½ÅµÇÁö ¾ÊÀ» ¶§ °­Á¦ Á¾·á)
{
	FormMain->TimerHydro1->Enabled = false;	// Timer Stop => TimerHydro1Àº µ¥ÀÌÅÍ ¼ö½ÅÀÌ ¸ðµÎ ¾È‰çÀ» ¶§ Timer Expire¿ëÀ¸·Î »ç¿ë

	LogMessage("RQ-30D time expired..");

//	if ( pMasterPortThread != NULL ) {  	// Time ExpireµÆÀ» ¶§ ThreadºÎÅÍ Terminate
//		//delete pMasterPortThread; // Exception ¹ß»ýÀ¸·Î ÀÏ´Ü ÁÖ¼® Ã³¸®
//		pMasterPortThread = NULL;
//	}
//	if ( pSlavePortThread != NULL ) { 		// Time ExpireµÆÀ» ¶§ ThreadºÎÅÍ Terminate
//		//delete pSlavePortThread;	// Exception ¹ß»ýÀ¸·Î ÀÏ´Ü ÁÖ¼® Ã³¸®
//		pSlavePortThread = NULL;
//	}

	// ¼öÀ§ µ¥ÀÌÅÍ °øÀ¯ => To Slave Port
	if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
		if ( !m_ShareWaterLevelData.IsEmpty() ) {
			SendWaterLevelToSlavePort(m_ShareWaterLevelData);	// MasterPort¿¡¼­ ¹ÞÀº ¼öÀ§ µ¥ÀÌÅÍ¸¦ Slave Port·Î °øÀ¯ÇÑ´Ù.
		} else {
			// °øÀ¯ÇÒ ¼öÀ§ µ¥ÀÌÅÍ°¡ ¾øÀ¸¹Ç·Î Skipgksek.
		}
	}

	ClearRQ();
}

//---------------------------------------------------------------------------
// Set Airmar Data
void TRQ30D::SetSlaveCount(int SlaveCount)
{
	this->SlaveCount = SlaveCount;
}

//---------------------------------------------------------------------------
// Set Airmar Data
void  TRQ30D::SetAirmarData(STAirmarData AirmarData)
{
	this->AirmarData = AirmarData;
}

//---------------------------------------------------------------------------
// BufferingÇÑ µ¥ÀÌÅÍ¿¡¼­ °¢ Deviceº°·Î Áß°£°ªÀ» Ã£´Â´Ù. TotalQ, TotalLQµµ Ã£´Â´Ù.
bool TRQ30D::CaculateRQ30DData()	// Find RQ30FinalSet
{
	//memset(&RQ30DFinalSet, 0x00, sizeof(RQ30DFinalSet));	// °á°ú ÁýÇÕ ClearÇÑ´Ù.
	RQ30DFinalSet.Clear();	// Data Clear

	int MasterDeviceCnt = MasterStatus.GetDeviceCount();
	int SlaveDeviceCnt = SlaveStatus.GetDeviceCount();
	TList *pList = NULL;
	STRQ30DData Data;
	int Addr = 0;

	// Master Port -----------------------------------------------------------
	for ( int i = 0 ; i < MasterDeviceCnt ; i++ ) {
		pList = GetMasterListByIndex(i);
		Addr = MasterStatus.GetDeviceAddress(i);	// Device Address ( 1,2,3,...10)

		if ( Addr <= 0 ) continue;
		if ( pList == NULL ) continue;

		Data.Clear();

		if ( pList->Count == 0 ) {	// ÇØ´ç Device¿¡ ¼ö½ÅµÈ µ¥ÀÌÅÍ°¡ ¾ø´Ù
			//memset(&Data, 0x00, sizeof(Data));
			Data.Address = Addr;
			RQ30DFinalSet.RQ30DData[Data.Address-1] = Data;	// ÀüºÎ 0À¸·Î ÃÊ±âµÈ Data
		} else {
			//Data = FindMidData(pList);
			//FindMidData(pList, Data); 	// Áß°£°ª
			GetAvgData(pList, Data);		// Æò±Õ°ª

			if ( Data.Address == 0 ) {	// ÇØ´ç List¿¡ µ¥ÀÌÅÍ ¾øÀ½ => °áÃø »óÈ² => ¸ðµç µ¥ÀÌÅÍ 0À¸·Î ÃÊ±âÈ­ ÇØ¼­ ³Ö´Â´Ù.
				Data.Address = Addr;
			}
			RQ30DFinalSet.RQ30DData[Data.Address-1] = Data;
		}
	}

	if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
		// Slave Port ----------------------------------------------------------
		for ( int i = 0 ; i < SlaveDeviceCnt ; i++ ) {
			pList = GetSlaveListByIndex(i);
			Addr = SlaveStatus.GetDeviceAddress(i);

			if ( Addr <= 0 ) continue;
			if ( pList == NULL ) continue;

			Data.Clear();

			if ( pList->Count == 0 ) {
				//memset(&Data, 0x00, sizeof(Data));
				Data.Address = Addr;
				RQ30DFinalSet.RQ30DData[Data.Address-1] = Data;	// ÀüºÎ 0À¸·Î ÃÊ±âµÈ Data
			} else {
				//Data = FindMidData(pList);
				//FindMidData(pList, Data);
				GetAvgData(pList, Data);		// Æò±Õ°ª

				if ( Data.Address == 0 ) {	// ÇØ´ç List¿¡ µ¥ÀÌÅÍ ¾øÀ½ => °áÃø »óÈ² => ¸ðµç µ¥ÀÌÅÍ 0À¸·Î ÃÊ±âÈ­ ÇØ¼­ ³Ö´Â´Ù.
					Data.Address = Addr;
				}
				RQ30DFinalSet.RQ30DData[Data.Address-1] = Data;
			}
		}
	}

	int DeviceCount = MasterDeviceCnt + SlaveDeviceCnt;
	RQ30DFinalSet.Count = DeviceCount;

	// -----------------------------------------------------------------------
	// Caculate TotalQ, TotalLQ
	for ( int i = 0 ; i < 10 ; i++ ) {
		if ( RQ30DFinalSet.RQ30DData[i].Address != 0 ) {	// °ªÀÌ Á¸Àç
			RQ30DFinalSet.TotalQ  += RQ30DFinalSet.RQ30DData[i].Discharge;
			RQ30DFinalSet.TotalLQ += RQ30DFinalSet.RQ30DData[i].LearnDischarge;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Slave PortÀÇ ¼ö½ÅÀ» Ã³¸®ÇÑ´Ù.
bool TRQ30D::ReceiveSlave(AnsiString RecvMsg, int iLen)
{
// RQ-30d´Â Idle »óÅÂ¿¡¼­µµ ÃøÁ¤ ÀÛ¾÷À» Áö¼ÓÇÑ´Ù.
	if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_SlaveRecvMsg += RecvMsg;

	int Pos = 0;
	AnsiString RecvMessage = "";

	Pos = m_SlaveRecvMsg.AnsiPos("\r\n");
	while ( Pos > 0 ) {

		RecvMessage = m_SlaveRecvMsg.SubString(1, Pos-1);
		RecvMessage = Trim(RecvMessage);

		DecodeSlave(RecvMessage, RecvMessage.Length());

		m_SlaveRecvMsg = m_SlaveRecvMsg.SubString(Pos+2, m_SlaveRecvMsg.Length() - (Pos+1));
		Pos = m_SlaveRecvMsg.AnsiPos("\r\n");
	}

	return true;
}

//---------------------------------------------------------------------------
// Slave Port Decode
bool TRQ30D::DecodeSlave(AnsiString RecvMessage, int iLen)
{
//@ÃøÁ¤½ÃÀÛ ÀÀ´ä
//#A0001ok$mt|4FA9;
//@µ¥ÀÌÅÍÃâ·Â ÀÀ´ä
//#A0001ok$pt|8C35;
//@ÃøÁ¤µ¥ÀÌÅÍ Ãâ·Â
// RQ-30 Style => G00, G01, G02, G03
// #M0001G00se00      15|01    1.35|02 -99.000|03  -18.90|04 -153.85|05    1.94|2827;	// Measurement value
//#M0001G01se06 -99.000|07 -153.85|08     350|09   12.46|2815;                          // Special value
//#M0001G02se10       0|11       1|12       0|13      12|14    3494|15       8|3856;    // Analysis value #1
//#M0001G03se16       0|17       0|1899999998|19    29.4|20    21.5|BEF6;               // Analysis value #2
// RQ-30d Style => G01,G02,...G04
// #M0002G01se0199999998|0299999998|03   0.777|04   76.09|05   35.74|0699999998|F0D5;
//#M0002G02se0799999998|08    0.00|09       7|10   24.09|D746;
//#M0002G03se11    2727|12       3|13   20698|14     133|15      27|16       2|426A;
//#M0002G04se17      10|18       0|1999999998|20    58.2|21    36.4|10D7;
// #M0001G01se0199999998|02    2.49|03   0.444|04   56.97|05    0.68|06   36.42|F4E0;
//#M0001G02se07   0.380|08    0.59|09      11|10   24.17|7946;
//#M0001G03se11     836|12      83|13       7|14      41|15    4809|16       0|EF0D;
//#M0001G04se17       6|18       0|19   99977|20    61.6|21    37.6|EB1B;

// for debug
//LogMessage("(Slave)" + RecvMessage);

	RecvMessage = Trim(RecvMessage);	// ¾Õ µÚÀÇ Space¸¦ Áö¿î´Ù. \r\nµµ ³¯¸°´Ù.
										// “S/òòšz½™ÿÙù?Ëß??#A0008ok$pt|2449; => 2024.06.06
										// ¾ÕºÎºÐ¿¡ ³ëÀÌÁî°¡ ²¸¼­ ±úÁö¸é #À» Ã£´Â´Ù.
	int Pos = RecvMessage.AnsiPos("#");
	if ( Pos == 0 ) { // not find
		// noting to do..
	} else if ( Pos == 1 ) {
		// nothing to do..
	} else { // Pos > 1
		RecvMessage = RecvMessage.SubString(Pos, RecvMessage.Length() - Pos + 1 );
	}

	AnsiString Start        = RecvMessage.SubString(1,1);           //#
	AnsiString Id           = RecvMessage.SubString(2,1);           //A:ÀÀ´ä, M:µ¥ÀÌÅÍ
	AnsiString SystemKey    = RecvMessage.SubString(3,2);           //00
	AnsiString DeviceNumber = RecvMessage.SubString(5,2);			//01 ~ 08 Device Address
	AnsiString Command      = RecvMessage.SubString(7, RecvMessage.Length()-6);

	///////////////////////////////////////////////////////////////////////////
	// ÃøÁ¤ ½ÃÀÛ/Á¾·á Command¿¡ ´ëÇÑ ÀÀ´ä
	if ( Id == "A" ) { // Ä¿¸àµå¿¡ ´ëÇÑ ÀÀ´ä
//debug
LogMessage("(Slave)" + RecvMessage);

		int Pos, Pos2;
		Pos  = RecvMessage.AnsiPos("$");		//#A0001ok$mt|4FA9;
		Pos2 = RecvMessage.AnsiPos("|");
		if ( Pos > 6 && Pos2 > Pos ) {
			AnsiString Result = "";
			AnsiString Cmd    = "";
			try {
				Result = RecvMessage.SubString(7, Pos-7);	     //#A0001ok$mt|4FA9; => "ok"
				Cmd    = RecvMessage.SubString(Pos, Pos2-Pos);	 //#A0001ok$mt|4FA9; => "$mt"
			} catch ( ... ) {
				// nothing to do
			}

			int DeviceAddress = 0;
			try {
				DeviceAddress = StrToInt(DeviceNumber);
			} catch (...) {
				DeviceAddress = 0;
			}
			if ( DeviceAddress == 0 ) {
				LogMessage("RQ-30d(Slave) Device Address fail..");
				return false;
			}

			if ( Cmd == "$mt") {			// Start command
				if ( Result == "ok" ) {
					SlaveStatus.SetStartAckByAddress(DeviceAddress);
					LogMessage("RQ-30d #" + DeviceNumber + " start acked..");
				} else {
					// not ok
					LogMessage("RQ-30d #" + DeviceNumber + " start not acked..");
				}
			} else if ( Cmd == "$pt" ) {	// DataRequest
				if ( Result == "ok" ) {
					SlaveStatus.SetFinishAckByAddress(DeviceAddress);
					LogMessage("RQ-30d #" + DeviceNumber + " data acked..");
				} else {
					// not ok
					LogMessage("RQ-30d #" + DeviceNumber + " data not acked..");
				}
			} else {
				// not ok
				LogMessage("RQ-30d #" + DeviceNumber + " data not acked..");
			}
		} else { // ¼ö½Å °á°ú°¡ Çü½Ä¿¡ ¸ÂÁö ¾ÊÀ» ¶§..
			LogMessage("RQ-30d(SlavePort) unknown response");
			return false;
		}

	///////////////////////////////////////////////////////////////////////////
	// ÃøÁ¤ °á°ú µ¥ÀÌÅÍ Decoding

	} else if ( Id == "M" ) { // ÃøÁ¤ °á°ú µ¥ÀÌÅÍ

		TStringList *pStrList = new TStringList();

		pStrList->Delimiter = '|';
		pStrList->StrictDelimiter = true;
		pStrList->DelimitedText = RecvMessage;

		if ( pStrList->Count <= 0 ) {
			LogMessage("RQ-30d(Slave) lack of token..");
			delete pStrList;	// release
			return false;
		}

		AnsiString Prefix = pStrList->Strings[0];	//#M0001G00 => Main Value, #M0001G01 => Special Value
		AnsiString x = "";
		try {
			x = Prefix.SubString(7,3);
		} catch ( Exception &e ) {
			delete pStrList;
			return false;
		}

		AnsiString Token;
		AnsiString No[6];
		AnsiString Value[6];

		// RawData Add
		SlaveData.RawData += RecvMessage + "\r\n";
		m_SlaveMessage    += RecvMessage + "\r\n";

		// Extract Addres
		AnsiString AddressStr = Prefix.SubString(5,2);	// #M0001G00 => 01 (Device Number)
		int Address = 0;

		try {
			Address = StrToInt(AddressStr);
		} catch ( Exception &e ) {
			Address = 0;
			delete pStrList;
			return false;	// Address decode  fail..
		}
		SlaveData.Address = Address;	// Device Address

		if ( x == "G01" ) { 	// Main Value => #M0001G01se0199999998|02    2.49|03   0.444|04   56.97|05    0.68|06   36.42|F4E0;
			if ( pStrList->Count < 6 ) {
				LogMessage("RQ-30d Lack of token..main value");
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G01se0199999998     => AUX
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//02    2.49	=> WaterLevel
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//03   0.444	=> Velocity
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//04   56.97	=> Quality
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//05    0.68	=> Discharge
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[5]; 	//06   36.42	=> Total Discharge
			No[5]    = Token.SubString(1,2);
			Value[5] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			int iValue = 0;
			double dValue = 0.0f;
			for ( int i = 0 ; i < 6 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}

				if ( iNo == 1 ) {
					try {
						iValue = StrToInt(Value[i]);
					} catch ( Exception &e) {
						iValue = 0;
					}
				} else {	// 2,3,4,5,6
					try {
						dValue = StrToFloat(Value[i]);
					} catch ( Exception &e ) {
						dValue = 0.0f;
					}
				}

				if ( iNo == 1 ) SlaveData.SelfCheck      = iValue;
				if ( iNo == 2 ) SlaveData.Waterlevel     = dValue;
				if ( iNo == 3 ) SlaveData.Velocity       = dValue;
				if ( iNo == 4 ) SlaveData.Quality        = dValue;
				if ( iNo == 5 ) SlaveData.Discharge      = dValue;
				if ( iNo == 6 ) SlaveData.TotalDischarge = dValue;
			} // end for..
			SlaveData.Complete |= RQ30D_MAIN;

		} else if ( x == "G02" ) {	// Special Vaue => #M0001G02se07   0.380|08    0.59|09      11|10   24.17|7946;
			if ( pStrList->Count < 4 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G02se07   0.380 => Learn Velocity
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//08    0.59	=> Learned Discharge
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//09      11	=> Opposite Direction content
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//10   24.17	=> Supply Voltage
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			double dValue = 0;
			for ( int i = 0 ; i < 4 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}

				try {
					dValue = StrToFloat(Value[i]);
				} catch ( Exception &e ) {
					dValue = 0.0f;
				}

				if ( iNo == 7 ) SlaveData.LearnVelocity   = dValue;
				if ( iNo == 8 ) SlaveData.LearnDischarge  = dValue;
				if ( iNo == 9 ) SlaveData.OppositeContent = dValue;
				if ( iNo == 10) SlaveData.SupplyVoltage   = dValue;
			}
			SlaveData.Complete |= RQ30D_SPECIAL;

		} else if (x == "G03") { // Analysis Value 01 => #M0001G03se11     836|12      83|13       7|14      41|15    4809|16       0|EF0D;
			if ( pStrList->Count < 6 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G03se11     836 => Peak
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//12      83	=> CSR
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//13       7	=> AreaPeak
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//14      41	=> RMS
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//15    4809	=> Amp
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[5]; 	//16       0 	=> AmpRelation
			No[5]    = Token.SubString(1,2);
			Value[5] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			int  iValue = 0;
			for ( int i = 0 ; i < 6 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}

				try {
					iValue = StrToInt(Value[i]);
				} catch ( Exception &e ) {
					iValue = 0;
				}

				if ( iNo == 11 ) SlaveData.PeakWidth   = iValue;
				if ( iNo == 12 ) SlaveData.CSR         = iValue;
				if ( iNo == 13 ) SlaveData.AreaPeak    = iValue;
				if ( iNo == 14 ) SlaveData.RmsPic      = iValue;
				if ( iNo == 15 ) SlaveData.Amp         = iValue;
				if ( iNo == 16 ) SlaveData.AmpRelation = iValue;
			} // end for
			SlaveData.Complete |= RQ30D_ANALYSIS1;

		} else if (x == "G04") { // Analysis Value 02 => #M0001G04se17       6|18       0|19   99977|20    61.6|21    37.6|EB1B;
			if ( pStrList->Count < 5 ) {
				LogMessage("RQ-30d Lack of token..special value..");
				return false;
			}

			Token    = pStrList->Strings[0];	//#M0001G04se17       6
			No[0]    = Token.SubString(12,2);
			Value[0] = Token.SubString(14, Token.Length()-14+1);

			Token    = pStrList->Strings[1];   	//18       0
			No[1]    = Token.SubString(1,2);
			Value[1] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[2]; 	//19   99977
			No[2]    = Token.SubString(1,2);
			Value[2] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[3]; 	//20    61.6
			No[3]    = Token.SubString(1,2);
			Value[3] = Token.SubString(3, Token.Length()-3+1);

			Token    = pStrList->Strings[4]; 	//21    37.6
			No[4]    = Token.SubString(1,2);
			Value[4] = Token.SubString(3, Token.Length()-3+1);

			int iNo = 0;
			int iValue = 0;
			double dValue = 0.0f;
			for ( int i = 0 ; i < 5 ; i++ ) {
				try {
					iNo = StrToInt(No[i]);
				} catch ( ... ) {
					iNo = 0;
				}
				if ( iNo < 20 ) {
					try {
						iValue = StrToInt(Value[i]);
					} catch ( ... ) {
						iValue = 0;
					}
				} else { // iNo == 20, 21 = double
					try {
						dValue = StrToFloat(Value[i]);
					} catch ( ... ) {
						dValue = 0;
					}
				}

				if ( iNo == 17 ) SlaveData.Signal      = iValue;	// Signal
				if ( iNo == 18 ) SlaveData.ErrorCode   = iValue;	// ErrorCode
				if ( iNo == 20 ) SlaveData.Angle       = dValue; 	// Angle
			} // end for
			SlaveData.Complete |= RQ30D_ANALYSIS2;
		} else {
			// unknown command
		}

		if (SlaveData.Complete == (RQ30D_MAIN | RQ30D_SPECIAL | RQ30D_ANALYSIS1 | RQ30D_ANALYSIS2)) {	// Data Complete => Send Message complete
// for debug
LogMessage(m_SlaveMessage); m_SlaveMessage = "";

			STRQ30DData *pData = new STRQ30DData();
			memcpy(pData, &SlaveData, sizeof(STRQ30DData));

			TList *pList = GetSlaveListByAddress(Address);
			if ( pList == NULL ) {
				LogMessage("[SlavePort] Can't find List by Address..");
				//return false;
			} else {
				pList->Add(pData);	// Add RQ30Data
			}

			if ( MasterStatus.IsLastFinishAck() && SlaveStatus.IsLastFinishAck() ) {
				FormMain->TimerHydro1->Enabled = false;
				m_enWorkingStep = enSensorIdle;	// »óÅÂ¸¦ Idle »óÅÂ·Î ¸¶²Û´Ù.
				SleepMsg(100);	// ¼öÀ§ µ¥ÀÌÅÍ¸¦ °øÀ¯ÇÏ±â Àü¿¡ Àá±ñ ±â´Ù¸°´Ù.
				if ( !m_ShareWaterLevelData.IsEmpty() ) {
					SendWaterLevelToSlavePort(m_ShareWaterLevelData);	// MasterPort¿¡¼­ ¹ÞÀº ¼öÀ§ µ¥ÀÌÅÍ¸¦ Slave Port·Î °øÀ¯ÇÑ´Ù.
				} else {
					// °øÀ¯ÇÒ ¼öÀ§ µ¥ÀÌÅÍ°¡ ¾øÀ¸¹Ç·Î Skip => Master DeviceÀÇ Main Value buffring ¾ÈµÊ. => error
				}
			}
			memset(&SlaveData, 0x00, sizeof(STRQ30DData));	// Buffer Clear
		}
		delete pStrList;
	} else {	// unknown id => ¾ÆÁ÷ ¸ð¸£´Â Command
		// unknown command
		LogMessage("RQ-30d unknown command received..");
	}
	return true;
}

//---------------------------------------------------------------------------
// MasterPort¿¡¼­ ¼ö½ÅµÈ ¼öÀ§ µ¥ÀÌÅÍ(MainValue)¸¦ SlavePort¿¡ ForwardingÇÑ´Ù.
bool TRQ30D::SendWaterLevelToSlavePort(AnsiString WaterLevelData)
{
	bool bResult = false;

	if ( m_pSlavePort == NULL ) return false;

	AnsiString Space = " ";
	SlaveSend(Space, Space.Length());
	SleepMsg(10);	// wait for while
	SlaveSend(Space, Space.Length());
	SleepMsg(10);	// wait for while
	SlaveSend(Space, Space.Length());
	SleepMsg(10);	// wait for while

	bResult = SlaveSend(WaterLevelData, WaterLevelData.Length());

	return bResult;
}

//---------------------------------------------------------------------------
bool TRQ30D::SlaveSend(AnsiString Command, int iLen)
{
	if ( m_pSlavePort == NULL ) return false;

	try {
		pGSlavePortSection->Enter();
		m_pSlavePort->PutString(Command);
		pGSlavePortSection->Leave();
		LogMessage("[RQ30D](Slave)=>" + Command);
	} catch ( Exception &e ) {
		LogMessage(e.Message);
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
// List¿¡¼­ ÀúÀåµÈ °ªÀ» SortÇÏ¿© Áß°£°ªÀ» ¸®ÅÏÇÑ´Ù.
//STRQ30DData TRQ30D::FindMidData(TList *pList)
void TRQ30D::FindMidData(TList *pList, STRQ30DData &Data)
{
	int Count = 0;
	Count = pList->Count;

	//STRQ30DData Data;
	//memset(&Data, 0x00, sizeof(STRQ30DData));
	double V = 0.0f;

	if ( Count >= 2 ) {   	// ¼öÁýµÈ µ¥ÀÌÅÍ°¡ 2°³ ÀÌ»óÀÌ¸é Á¤·ÄÇÏ¿© Áß°£°ªÀ» ÃëÇÑ´Ù. ´Ü -99ÀÏ ¶§´Â Normal ValueÀ» µû·Î ¸ð¾Æ Áß°£°ªÀ» ÃëÇÑ´Ù.
		pList->Sort(CompareRQVelocity);	// Sort

		int NormalCount = 0;
		STRQ30DData *pData = NULL;
		STRQ30DData *pDataArray = new STRQ30DData[pList->Count];   	// ÀÓ½Ã ÀúÀå °ø°£ È®º¸..

		for ( int i = 0 ; i < pList->Count ; i++ ) { 	// Normal Value°ª¸¸ ¸ðµç´Ù.
			pData = (STRQ30DData *) pList->Items[i];
			V = pData->Velocity;
			if ( pData->Velocity > -90.0f && (abs((int)(V * 100)) != 0 )) {	// Normal Value   (Tide ¿É¼ÇÀÌ¸é -Velocity°¡ ³ª¿Ã ¼ö ÀÖ´Ù.
				pDataArray[NormalCount++] = *pData;
			}
		}
		if ( NormalCount > 0 ) {
			int Index = NormalCount / 2;
			Data = pDataArray[Index];				// Final Set => Normal value Áß¿¡ Áß°£°ªÀ» ¼±ÅÃÇÑ´Ù.
		} else { // Normal Count = 0
			// ÀüºÎ -99 or 0.0f ÀÌ¹Ç·Î ¸®½ºÆ®ÀÇ Ã¹¹øÂ° °ªÀ» »ç¿ëÇÑ´Ù.
			pData = (STRQ30DData *)pList->Items[0];
			Data = *pData;							// Final Set
		}
		delete [] pDataArray; // Array Release
	} else if ( Count == 1 ) {  	// ¼öÁýÇÑ µ¥ÀÌÅÍ°¡ 1°ÇÀÏ ¶§´Â ±×³É ±×°É ¾´´Ù.
		Data = *((STRQ30DData *)pList->Items[0]);
	} else {
		// nothing to do
	}
	//return Data;
}

//---------------------------------------------------------------------------
// List¿¡¼­ ÀúÀåµÈ °ªÀ» Æò±Õ°ªÀ» ReturnÇÑ´Ù.
//STRQ30DData TRQ30D::FindMidData(TList *pList)
void TRQ30D::GetAvgData(TList *pList, STRQ30DData &Data)
{
	STRQ30DData xData;
	STRQ30DData *pData = NULL;
	memset(&xData, 0x00, sizeof(xData));

	int ItemCount = 0;
	for ( int i = 0 ; i < pList->Count ; i++ ) {
		pData = (STRQ30DData *) pList->Items[i];
		if ( pData != NULL ) {
			ItemCount++;
			// SelfCheck ÄÚµå´Â °¡Àå ³ôÀº ¼ö¸¦ ³²±ä´Ù.
			xData.SelfCheck      = ((pData->SelfCheck > xData.SelfCheck ) ? pData->SelfCheck : 0);
			xData.Waterlevel     += pData->Waterlevel;
			xData.Velocity       += pData->Velocity;
			// SNRÀº Æò±ÕÇÏµÇ, ¼Ò¼öÁ¡ ÀÌÇÏ´Â °¡Àå ³ôÀº ¼ö¸¦ ³²±ä´Ù.
			double x1 = xData.Quality  - (int)xData.Quality;	// ¼Ò¼öÁ¡ ÀÌÇÏ¸¦ ³²±ä´Ù
			double x2 = pData->Quality - (int)pData->Quality;	// ¼Ò¼öÁ¡ ÀÌÇÏ¸¦ ³²±ä´Ù.
			xData.Quality        += (int)xData.Quality + (int)pData->Quality;
			xData.Quality        += ((x2 > x1 ) ? x2 : x1);
			//
			xData.Discharge      += pData->Discharge;
			xData.TotalDischarge += pData->TotalDischarge;
			xData.LearnVelocity  += pData->LearnVelocity;
			xData.LearnDischarge += pData->LearnDischarge;
			xData.OppositeContent+= pData->OppositeContent;
			xData.SupplyVoltage  += pData->SupplyVoltage;
			xData.PeakWidth      += pData->PeakWidth;
			xData.CSR            += pData->CSR;
			xData.AreaPeak       += pData->AreaPeak;
			xData.RmsPic         += pData->RmsPic;
			xData.Amp            += pData->Amp;
			xData.AmpRelation    += pData->AmpRelation;
			xData.Signal         += pData->Signal;
			// ¿¡·¯ ÄÚµåµµ °¡Àå ³ôÀº ¿¡·¯ÄÚµå¸¦ ³²±ä´Ù.
			xData.ErrorCode       = ((pData->ErrorCode > xData.ErrorCode) ? pData->ErrorCode : xData.ErrorCode);
			xData.Angle          += pData->Angle;
			xData.Complete        = pData->Complete;
		}
	}

	if ( ItemCount > 0 ) {
		//xData.SelfCheck    = ;
		xData.Waterlevel     = xData.Waterlevel      / ItemCount;
		xData.Velocity       = xData.Velocity        / ItemCount;
		//xData.Quality      = ;
		xData.Discharge      = xData.Discharge       / ItemCount;
		xData.TotalDischarge = xData.TotalDischarge  / ItemCount;
		xData.LearnVelocity  = xData.LearnVelocity   / ItemCount;
		xData.LearnDischarge = xData.LearnDischarge  / ItemCount;
		xData.OppositeContent= xData.OppositeContent / ItemCount;
		xData.SupplyVoltage  = xData.SupplyVoltage   / ItemCount;
		xData.PeakWidth      = xData.PeakWidth       / ItemCount;
		xData.CSR            = xData.CSR             / ItemCount;
		xData.AreaPeak       = xData.AreaPeak        / ItemCount;
		xData.RmsPic         = xData.RmsPic          / ItemCount;
		xData.Amp            = xData.Amp             / ItemCount;
		xData.AmpRelation    = xData.AmpRelation     / ItemCount;
		xData.Signal         = xData.Signal          / ItemCount;
		//xData.ErrorCode    = //xData.ErrorCode
		xData.Angle          = xData.Angle           / ItemCount;
		//xData.Complete     = ;
	} else {
		// nothing to do..
	}
	Data = xData;
}

//---------------------------------------------------------------------------
TList *TRQ30D::GetMasterListByIndex(int Index)
{
	return m_pMasterList[Index];
}

//---------------------------------------------------------------------------
TList *TRQ30D::GetMasterListByAddress(int Address)
{
	int Index = MasterStatus.GetDeviceIndex(Address);
	return m_pMasterList[Index];
}

//---------------------------------------------------------------------------
TList *TRQ30D::GetSlaveListByIndex(int Index)
{
	return m_pSlaveList[Index];
}

//---------------------------------------------------------------------------
TList *TRQ30D::GetSlaveListByAddress(int Address)
{
	int Index = SlaveStatus.GetDeviceIndex(Address);
	return m_pSlaveList[Index];
}

//---------------------------------------------------------------------------
void TRQ30D::CreateThread()
{
	// Master Port Thread ---------------------------------------------------

	pMasterPortThread = new TRQ30DThread(true);	// Thread ÀÏ´Ü Suspend ½ÃÅ°°í.. ÆÄ¶ó¸ÞÅ¸ ³Ö°í active
	pMasterPortThread->InitThread(this, m_pPort, theEnv.m_HydroMasterAddress, true);	// MasterPort Thread
	pMasterPortThread->Start();

	// Slave Port Thread ---------------------------------------------------

	if ( theEnv.m_HydroUseSlavePort == ON_STR ) {
		pSlavePortThread = new TRQ30DThread(true);	// Thread ÀÏ´Ü Suspend ½ÃÅ°°í.. ÆÄ¶ó¸ÞÅ¸ ³Ö°í active
		pSlavePortThread->InitThread(this, m_pSlavePort, theEnv.m_HydroSlaveAddress, false);	// SlavePort Thread
		pSlavePortThread->Start();
	} else {
		pSlavePortThread = NULL;
	}
}

#pragma package(smart_init)


