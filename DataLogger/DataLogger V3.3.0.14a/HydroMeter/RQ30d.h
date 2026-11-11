//---------------------------------------------------------------------------

#ifndef RQ30DH
#define RQ30DH
//---------------------------------------------------------------------------

//#include "../Define.h"
//#include "../UnitMain.h"
//
//#include "AdPort.hpp"
//#include "OoMisc.hpp"

#include "HydroMeter.h"
#include "Airmar.h"
#include "UnitEnv.h"

class TRQ30DThread;

#define UART_BUFFER		    8192

#define RQ30D_MAIN          0x80
#define RQ30D_SPECIAL       0x40
#define RQ30D_ANALYSIS1		0x20
#define RQ30D_ANALYSIS2		0x10

#define RQ30D_START			0x01
#define RQ30D_DATAREQ		0x02

#define FRAME_OK               0
#define STX_ERROR             -1
#define ETX_ERROR             -2
#define CRC_ERROR             -3

#define DEVICEPATTERN(x)    ((x == 0x00) ? 0x80 : \
							((x == 0x01) ? 0xC0 : \
							((x == 0x02) ? 0xE0 : \
							((x == 0x03) ? 0xF0 : \
							((x == 0x04) ? 0xF8 : \
							((x == 0x05) ? 0xFC : \
							((x == 0x06) ? 0xFE : \
							((x == 0x07) ? 0xFF : 0x00))))))))

// SlaveCnt = 0 => 1xxx xxxx
// SlaveCnt = 1 => 11xx xxxx
// SlaveCnt = 2 => 111x xxxx
// SlaveCnt = 3 => 1111 xxxx
// SlaveCnt = 4 => 1111 1xxx
// SlaveCnt = 5 => 1111 11xx
// SlaveCnt = 6 => 1111 111x
// SlaveCnt = 7 => 1111 1111

//---------------------------------------------------------------------------
unsigned short int crc16tab[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0 };

unsigned short int CRC16( BYTE c, unsigned short int crc16)
{
	return crc16tab[(unsigned char)(crc16>>8)] ^ (crc16<<8) ^ (unsigned int)(c);
}								
                        	

//---------------------------------------------------------------------------
// RQ30 Data Structure
struct STRQ30DData {
	int        Address;           // Device Address
	int        SelfCheck;         // 1 AUX/Self-check 2024.09.17 SelfCheck
	double     Waterlevel;   	  // 2
	double     Velocity;          // 3
	double     Quality;		      // 4
	double     Discharge;		  // 5
	double     TotalDischarge;    // 6  ( only useful for RQ-30d(Master))
	double     LearnVelocity;	  // 7
	double     LearnDischarge;	  // 8
	double     OppositeContent;	  // 9
	double     SupplyVoltage;	  // 10
	int        PeakWidth;         // 11
	int        CSR;               // 12
	int        AreaPeak;          // 13
	int        RmsPic;            // 14 mV
	int        Amp;               // 15
	int        AmpRelation;       // 16
	int        Signal;            // 17
	int        ErrorCode;         // 18
	double     Angle;             // 21(RQ Protocol)
	byte       Complete;		  // RQ30_MEASUREMENT | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2
	AnsiString RawData;

	STRQ30DData::STRQ30DData() {
		memset(this, 0x00, sizeof(STRQ30DData));
	}

	void STRQ30DData::Clear() {
		Address         = 0;       // Device Address
		SelfCheck       = 0;       // 1 AUX/Self-check 2024.09.17 SelfCheck
		Waterlevel      = 0.0f;    // 2
		Velocity        = 0.0f;    // 3
		Quality         = 0.0f;	   // 4
		Discharge       = 0.0f;	   // 5
		TotalDischarge  = 0.0f;    // 6  ( only useful for RQ-30d(Master))
		LearnVelocity   = 0.0f;	   // 7
		LearnDischarge  = 0.0f;	   // 8
		OppositeContent = 0.0f;	   // 9
		SupplyVoltage   = 0.0f;	   // 10
		PeakWidth       = 0;       // 11
		CSR             = 0;       // 12
		AreaPeak        = 0;       // 13
		RmsPic          = 0;       // 14 mV
		Amp             = 0;       // 15
		AmpRelation     = 0;       // 16
		Signal          = 0;       // 17
		ErrorCode       = 0;       // 18
		Angle           = 0.0f;    // 21(RQ Protocol)
		Complete        = 0x00;	   // RQ30_MEASUREMENT | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2
		RawData         = "";
	}

	void STRQ30DData::operator = (const STRQ30DData & Data)
	{
		this->Address          = Data.Address;             // Device Address
		this->SelfCheck        = Data.SelfCheck;           // 1 AUX/Self-check 2024.09.17 SelfCheck
		this->Waterlevel   	   = Data.Waterlevel;   	   // 2
		this->Velocity         = Data.Velocity;            // 3
		this->Quality		   = Data.Quality;		       // 4
		this->Discharge		   = Data.Discharge;		   // 5
		this->TotalDischarge   = Data.TotalDischarge;      // 6  ( only useful for RQ-30d(Master))
		this->LearnVelocity    = Data.LearnVelocity;	   // 7
		this->LearnDischarge   = Data.LearnDischarge;	   // 8
		this->OppositeContent  = Data.OppositeContent;	   // 9
		this->SupplyVoltage	   = Data.SupplyVoltage;	   // 10
		this->PeakWidth        = Data.PeakWidth;           // 11
		this->CSR              = Data.CSR;                 // 12
		this->AreaPeak         = Data.AreaPeak;            // 13
		this->RmsPic           = Data.RmsPic;              // 14 mV
		this->Amp              = Data.Amp;                 // 15
		this->AmpRelation      = Data.AmpRelation;         // 16
		this->Signal           = Data.Signal;              // 17
		this->ErrorCode        = Data.ErrorCode;           // 18
		this->Angle            = Data.Angle;               // 21(RQ Protocol)
		this->Complete		   = Data.Complete;		  	   // RQ30_MEASUREMENT | RQ30_SPECIAL | RQ30_ANALYSIS1 | RQ30_ANALYSIS2
		this->RawData          = Data.RawData;
	}
};

struct STRQ30DSet {
	STRQ30DData RQ30DData[10];	  // 0: RQ-30d(master), 1: RG-30d(slave#1), 2: Slave#2,... 7:Slave #7
	BYTE        Count;            // Data Recv Count => Master + Slave Count

	double      TotalQ;
	double      TotalLQ;	      // 2023.07.18 Master LQ + Slaves LQ

	STRQ30DSet::STRQ30DSet() {
		memset(this, 0x00, sizeof(STRQ30DSet));
	}

	STRQ30DSet::Clear() {
		for ( int i = 0 ; i < 10 ; i++ ) RQ30DData[i].Clear();
		Count   = 0;

		TotalQ  = 0.0f;
		TotalLQ = 0.0f;
	}
};

// -----------------------------------------------------------------------------
// TRQStatus Class

enum EN_START_ACK   { enStartIdle  = 0x00, enStartCmd  = 0x01, enStartAck = 0x02 };
enum EN_FINISH_ACK  { enFinishIdle = 0x00, enFinishCmd = 0x01, enFinishAck= 0x02 };

class TRQStatus {
private:
	int  Address[10];
	EN_START_ACK  StartAck[10];	// enStartIdle => enStartCmd => enStartAck
	EN_FINISH_ACK FinishAck[10];   // enFinishIdle => enFinishCmd => enFinishAck
	int  Count;		 // Device count
public:
	TRQStatus();
	~TRQStatus();
	void SetDeviceAddress(UnicodeString DeviceAddress);	// (ex) 5,4,3,2,1, or  6,7,8,910
	int  GetDeviceCount();
	void Clear();	// All Status Clear
	int  GetDeviceAddress(int Index);
	int  GetDeviceIndex(int Address);
	// Start
	void SetStartByAddress(int Address);
	void SetStartByIndex(int Index);
	void SetStartAckByAddress(int Address);
	void SetStartAckByIndex(int Index);
	bool IsStartAckByAddress(int Address);
	bool IsStartAckByIndex(int Index);
	bool IsAllStartAck();
	bool IsLastStartAck();	// 마지막 Device의 start Ack가 있었나?
	// Finish
	void SetFinishByAddress(int Address);
	void SetFinishByIndex(int Index);
	void SetFinishAckByAddress(int Address);
	void SetFinishAckByIndex(int Index);
	bool IsFinishAckByAddress(int Address);
	bool IsFinishAckByIndex(int Index);
	bool IsAllFinishAck();
	bool IsLastFinishAck();	// 마지막 Device의 데이터 수신이 있었나?
};

//---------------------------------------------------------------------------
// TRQ30 Class
class TRQ30D : public THydroMeter
{
public:		// Attribute
	STRQ30DData   MasterData;		// for RQ-30D, Master Port
	STRQ30DData   SlaveData;        // for RQ-30D, Slave Port
	STRQ30DSet    RQ30DFinalSet;	// Finial RQ-30D Set Data
	STAirmarData  AirmarData;		// for Airmar Data using Airmar Sensor(Direct connection)

	//TList        *m_pRQ30DList;	// buffer of airmar data
	TList        *m_pMasterList[10];// for Master Port List
	TList        *m_pSlaveList[10];	// for Slave Port List
	TRQStatus     MasterStatus;		// for Master Port Status
	TRQStatus     SlaveStatus;		// for Slave Port Status

	int           SlaveCount;		// RQ-30d에 붙어 있는 Slave Count

	TApdComPort  *m_pSlavePort;		 // RQ30D SlavePort
	TRQ30DThread *pMasterPortThread; // Master Port Thread
	TRQ30DThread *pSlavePortThread;	 // Slave Port Thread

	AnsiString    m_RecvMsg;
	AnsiString    m_MasterMessage;
	AnsiString    m_SlaveRecvMsg;
	AnsiString    m_SlaveMessage;

	AnsiString    m_ShareWaterLevelData;	// Slave Port로 공유할 수위 데이터 string ( Address #1의 Main Value )

public:		// Method
	TRQ30D();
	~TRQ30D();

//Virtual Function
	void   Clear();
	void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo=enHydroFirst);  	// bFirst=> 하층유속계, false: 상층유속계
	void   SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance);	// AG/SL Series는 반드시 초기 데이터로 지정해야 함.
	bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime);
	bool   Finish();
	bool   Decode(AnsiString RecvMessage, int iLen);
	bool   Decode(BYTE *pBuffer, int iLen);
	bool   Receive(AnsiString RecvMessage, int iLen);
	bool   Receive(BYTE *pBuffer, int iLen);
	bool   Send(AnsiString RecvMsg, int iLen);
	bool   Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void   DB2Value();
	void   Value2DB();
	double GetAvgVelocity();

	void   SetBreak(bool bBreak);
	bool   GetBreakStatus();
	void   SendBreak(int iMiliSecond);

// Method
	void   SetSlavePort(TApdComPort *pPort);	// SlavePort를 지정한다.
	void   StartRQ();	// 1분 단위 Starrt
	void   ClearRQ();	// 1분 단위 Data Clear
	void   FinishRQ();	// 1분 단위 측정 종료
	void   FinishForceRQ();	// 1분 단위 측종 종료(강제 종료=>RQ/RG에서 데이터가 모두 수신되지 않을 때 강제 종료)

//	bool   SendCommand(AnsiString Id, AnsiString SystemKey, AnsiString DeviceNumber, AnsiString Cmd, bool Prefix=true);
//	int    CheckFrame(BYTE *pBuffer, int Length);
//	AnsiString MakeCRC(BYTE *pBuffer, int Length);
	void   SetSlaveCount(int SlaveCount);

	void   SetAirmarData(STAirmarData AirmarData);
	bool   CaculateRQ30DData();

	bool   ReceiveSlave(AnsiString RecvMsg, int iLen);	// SlavePort의 수신을 처리한다.
	bool   DecodeSlave(AnsiString RecvMsg, int iLen);
	bool   SendWaterLevelToSlavePort(AnsiString WaterLevelData);
	bool   SlaveSend(AnsiString Command, int iLen);
	//STRQ30DData FindMidData(TList *pList);
	void   FindMidData(TList *pList, STRQ30DData &Data);
	void   GetAvgData(TList *pList, STRQ30DData &Data);

	TList *GetMasterListByIndex(int Index);
	TList *GetMasterListByAddress(int Address);
	TList *GetSlaveListByIndex(int Index);
	TList *GetSlaveListByAddress(int Address);

	void   CreateThread();
};

extern TCriticalSection *MasterSection;		// for Master Port Thread
extern TCriticalSection *SlaveSection;		// for Slave Port Thread
extern TQueue *MasterQueue;					// for Master Port Thread
extern TQueue *SlaveQueue;					// for Slave Port Thread
extern TEvent *MasterEvent;					// for Master Port Thread
extern TEvent *SlaveEvent;					// fro Slave Port Thread

//int __fastcall CompareRQDVelocity(void *Item1, void *Item2);	// for sort by all devices
int __fastcall CompareVelocity(void *Item1, void *Item2); 	// for sort by device only

#endif
