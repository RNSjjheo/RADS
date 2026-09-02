//---------------------------------------------------------------------------

#ifndef RnsWaterH
#define RnsWaterH
//---------------------------------------------------------------------------
#include "WaterLevel.h"

#define RnsWaterReq		0x01
#define RnsWaterRes		0x11

enum EN_RNSWATER_STEP { enRnsWaterIdle, enRnsWaterStart, enRnsWaterFinish };

#pragma pack(1)

struct STRnsWater {
	BYTE  Type;       // 0:NONE, 1:VegaPulse(4~20mA), 2:VegaPuls+485Converter, 3:VegaPulse(485), .. 
	float Depth;      // Depth(Unit:m)
	float Temperature;// Tempuerature(Unit:C)
	float Pressure;   // Pressure(Unit:?)
	float Salinity;   // Salinity(Unit:?)
	float Reserved01;
	float Reserved02;
	float Reserved03;
	float Reserved04;
};

#pragma pack()

class TRnsWater : public TWaterLevel
{
public:		// Attribute
	BYTE Buffer[BUFFER_1K];
	bool Buffering;
	int  Pos;
	BYTE Length;

	STRnsWater RnsWater;	// RnsWater structure

	double     AvgWaterLevel;
	double     MinWaterLevel;
	double     MaxWaterLevel;

	TList      *m_pList;

	int         m_Count;	// ÃøÁ¤ Count(Cycle È½¼ö);

public:		// Method
	TRnsWater();
// Virtual method
	~TRnsWater();
	void Clear();
	void Init(TApdComPort *pPort);
	bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false);
	bool Finish();
	bool Decode(AnsiString RecvMsg, int iLen);
	bool Decode(BYTE *pBuffer, int iLen);
	bool Receive(AnsiString RecvMsg, int iLen);
	bool Receive(BYTE *pBuffer, int iLen);
	bool Send(AnsiString RecvMsg, int iLen);
	bool Send(BYTE *pBuffer, int iLen);
	bool Send(char Buffer[], int iLen);
	bool Save(UnicodeString MeasureDate, UnicodeString MeasureTime);
	void DB2Value();
	void Value2DB();
	double GetArea();
	double GetWaterLevel();

// Method
	bool SendRnsWaterReq();
	bool CaculateWaterLevel();
	bool ClearList();
	int  CheckFrame(BYTE *pBuffer, int Length);
};


#endif
