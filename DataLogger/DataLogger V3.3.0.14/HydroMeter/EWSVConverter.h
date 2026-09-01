//---------------------------------------------------------------------------

#ifndef EWSVConverterH
#define EWSVConverterH
//---------------------------------------------------------------------------

#include "HydroMeter.h"

#define UART_BUFFER		8192

#define	EWSV_MeasureStart		0x01
#define	EWSV_MeasureStartAck	0x11
#define	EWSV_MeasureDataReq		0x02
#define	EWSV_MeasureDataRes		0x12

#pragma pack(1)

struct STEWSVData {
	BYTE  DeviceType;	// 0:None, 1:RG-30, 2:PAVOS
	float W;		// WaterLevel
	float V;		// Velocity
	float SNR;		// SNR
	float Q;		// Q: Discharge
	float LV;		// Learned Velocity
	float LQ;		// Learned Q
	float Opposite;	// Opposite Direction Content
	float Volt;		// Volt
	float Angle;	// Inclination
};


struct STEWSVPayload {		// EWSVConverter 의 각 Device당Data Format
	BYTE  Count;        	// Device Type
	STEWSVData Data[20];		// RG-30: 1EA, PAVOs: 1EA
};

#pragma pack()


class TEWSVConverter : public THydroMeter
{
public:		// Attribute

	double AvgVel;		// Average Velocity
	double MinVel;		// Min Velocity
	double MaxVel;		// Max Velocity

	BYTE Buffer[1024];
	int  Pos;
	BYTE Length;
	bool Buffering;

	STEWSVPayload Payload;	//
	STEWSVData    Data;
	float         TQ;
	float         TLQ;

public:		// Method
	TEWSVConverter();
	~TEWSVConverter();
	TList *m_pList;

//Virtual Function
	void   Clear();
	void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo=enHydroFirst);  	// bFirst=> 하층유속계, false: 상층유속계
	void   SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance);
	bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime);
	bool   Finish();
	bool   Decode(AnsiString sCmd, int iLen);
	bool   Decode(BYTE *pBuffer, int iLen);
	bool   Receive(AnsiString RecvMsg, int iLen);
	bool   Receive(BYTE *pBuffer, int iLen);
	bool   Send(AnsiString RecvMsg, int iLen);
	bool   Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void   DB2Value();
	void   Value2DB();
	double GetAvgVelocity();

	void   SetBreak(bool bBreak);
	bool   GetBreakStatus();
	void   SendBreak(int iMiliSecond);	// Send <BREAK> to ComPort

// Method
	bool Send(BYTE *pBuffer, int iLen);
	bool CaculateVelocity();
	bool MeasureStart(int WorkTime, UnicodeString AvgMethod );  	// 4분 간격의 동작
	bool MeasureDataReq();
	bool FrameCheck(BYTE *pBuffer, int iLen);
};

int __fastcall CompareVelocityx(void *Item1, void *Item2);

#endif
