//---------------------------------------------------------------------------

#ifndef AccusonicH
#define AccusonicH
//---------------------------------------------------------------------------

#include "HydroMeter.h"

#define ESC_START	0x1B	// Ctrl+{ => ESC
#define ESC_FINISH  0x48	// 'H'

#define ASCII_SPACE 0x20
#define ASCII_BRACE 0x7D

#define MAX_ACCUSONIC_COUNT	120		// Max Item for Accusonic Data

enum EN_VT100_STEP { enVT100Idle = 0x00,    // unknown status
					 enVT100EscStart,		// 0x1B
					 enVT100EscFinish,		// 0x48
					 enVT100Data };

struct STAccusonic {
	double LDF1;	// Log Data Flow Section 1              9.852
	long   LDO1;    // Log Data Volumn Section  1           6620621
	double LDS1;    // Log Data Level #1                    6.000
	int    LDT1;    // Log Data Integration Type Section 1  5
	int    LDE1;    // Log Data Temperature Section 1       241
	double LDV1;    // Log Data Velocity Path 1             0.056
	double LDV2;    // Log Data Velocity Path 2             0.134
	double LDV3;    // Log Data Velocity Path 3             0.100
	double LDV4;    // Log Data Velocity Path 4             0.000
	double LDV5;    // Log Data Velocity Path 5             0.000
	double LDV6;    // Log Data Velocity Path 6             0.000
	double LDV7;    // Log Data Velocity Path 7             0.000
	double LDV8;    // Log Data Velocity Path 8             0.000
	int    LDG1;    // Log Data Gain Path 1                 9
	int    LDG2;    // Log Data Gain Path 2                 4
	int    LDG3;    // Log Data Gain Path 3                 17
	int    LDG4;    // Log Data Gain Path 4                 40
	int    LDG5;    // Log Data Gain Path 5                 40
	int    LDG6;    // Log Data Gain Path 6                 40
	int    LDG7;    // Log Data Gain Path 7                 40
	int    LDG8;    // Log Data Gain Path 8                 40
	int    LDC1;    // Log Data Velocity SND Section 1      1485
	int    LDC2;    // Log Data Velocity SND Section 2      1506
	int    LDC3;    // Log Data Velocity SND Section 3      1489
	int    LDC4;    // Log Data Velocity SND Section 4      0
	int    LDC5;    // Log Data Velocity SND Section 5      0
	int    LDC6;    // Log Data Velocity SND Section 6      0
	int    LDC7;    // Log Data Velocity SND Section 7      0
	int    LDC8;    // Log Data Velocity SND Section 8      0
	int    LDB1;    // Log Data Gain Path 1                 92
	int    LDB2;    // Log Data Gain Path 2                 101
	int    LDB3;    // Log Data Gain Path 3                 99
	int    LDB4;    // Log Data Gain Path 4                 12
	int    LDB5;    // Log Data Gain Path 5                 1
	int    LDB6;    // Log Data Gain Path 6                 6
	int    LDB7;    // Log Data Gain Path 7                 7
	int    LDB8;    // Log Data Gain Path 8                 2
};

class TAccusonic : public THydroMeter
{
public:		// Attribute

	double AvgVel;		// Average Velocity
	double MinVel;		// Min Velocity
	double MaxVel;		// Max Velocity

	STAccusonic stAccusonic;
	STAccusonic AccusonicArray[MAX_ACCUSONIC_COUNT];
	int         Count;		// 데이터 수집횟수

	char  Buffer[BUFFER_4K];
	int   Pos;
	EN_VT100_STEP enVT100Step;	// idle, EscStart, EscFinish, EscData

	//TList *m_pAccuList;
	int   LastSpacePos;		//마지막 Space가 있는 위치(13개의 Space 이후에 추가로 들어온 데이터가 있는 지 판별

public:		// Method
	TAccusonic();
	~TAccusonic();

//Virtual Function
	void   Clear();
	void   Init(TApdComPort *pPort, bool bFirst=true);  	// bFirst=> 하층유속계, false: 상층유속계
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

// Function
	bool CalAvgMinMaxVelocity();
};

//int __fastcall CompareVelocity(void *Item1, void *Item2);

#endif
