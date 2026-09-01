//---------------------------------------------------------------------------

#ifndef LT400H
#define LT400H
//---------------------------------------------------------------------------
#include "WaterLevel.h"

#define LT400_START_STR       ":0111EE\r\n"
#define LT400_TEMP_STR        ":0103002D0008C7\r\n"
#define LT400_PRESSURE_STR    ":010300250008CF\r\n"
#define LT400_DEPT_STR        ":010300350008BF\r\n"

enum EN_LT400_STEP { enLT400Idle, enLT400Start, enLT400Temp, enLT400Pressure, enLT400Depth };


struct STLT400Data {
	double Temperature;
	double Pressure;
	double Depth;
};


class TLT400 : public TWaterLevel
{
public:		// Attribute
	double      AvgWaterLevel;
	double      MinWaterLevel;
	double      MaxWaterLevel;
	double      Temperature;
	double      Pressure;
	double      Depth;

	TList      *m_pList;

	bool        bStartAck;	// StartAck
	byte        RecvFlag;	// TROLL_INIT | TROLL_TEMPERATURE | TROLL_PRESSURE | TROLL_DEPTH

	AnsiString  StartAck;
	AnsiString  TempAck;
	AnsiString  PressureAck;
	AnsiString  DeptAck;

	EN_LT400_STEP m_Status;
	int         m_Count;	// ÃøÁ¤ Count(Cycle È½¼ö);

public:		// Method
	TLT400();
	~TLT400();

// Virtual method
	void Clear();
	void Init(TApdComPort *pPort);
	bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false);
	bool SendStart();
	bool SendTemperature();
	bool SendPressure();
	bool SendDepth();
	bool Finish();
	int  MakeStream(char Buffer[], STTROLLHeader &Header);
	int  MakeBinaryStream(byte *pStream, char *pBuffer, int Length);

	bool Decode(AnsiString RecvMsg, int iLen);
	bool Decode(BYTE *pBuffer, int iLen);
	bool Receive(AnsiString RecvMsg, int iLen);
	bool Receive(BYTE *pBuffer, int iLen);
	bool Send(AnsiString RecvMsg, int iLen);
	bool Send(char Buffer[], int iLen);
	bool Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void DB2Value();
	void Value2DB();
	double GetWaterLevel();
	double GetArea();

// Method
	bool CalAvgMinMaxWaterLevel();
	bool DecodeData();

	double Analyze(AnsiString sData);
	bool ClearList();

	void GetData(double &W, double &T, double &D,  double &P);

	bool CheckCRC(byte *pBuffer, int Length);
};


#endif
