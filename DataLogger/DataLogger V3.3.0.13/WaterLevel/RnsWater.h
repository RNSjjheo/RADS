//---------------------------------------------------------------------------

#ifndef RnsWaterH
#define RnsWaterH
//---------------------------------------------------------------------------
#include "WaterLevel.h"

#define RnsWaterReq			0x01
#define RnsWaterRes			0x11
#define AirmarDataReq		0x02
#define AirmarDataRes		0x12
#define RnsWaterSetup		0x21
#define RnsWaterSetupRes	0x31

enum EN_RNSWATER_STEP { enRnsWaterIdle, enRnsWaterStart, enRnsWaterFinish };

class TRnsWater : public TWaterLevel
{
public:		// Attribute
	BYTE Buffer[BUFFER_1K];
	bool Buffering;
	int  Pos;
	BYTE Length;

	STRnsWater RnsWater;	// RnsWater structure
	STAirmarData AirmarData;	// AirmarData structure

	double     AvgWaterLevel;
	double     MinWaterLevel;
	double     MaxWaterLevel;

	TList     *m_pList;
	TList     *m_pAirmarList;

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
	bool SendAirmarDataReq();
	bool CaculateWaterLevel();
	bool CaculateAirmar();
	bool ClearList();
	int  CheckFrame(BYTE *pBuffer, int Length);

	void GetData(double &W, double &T, double &D,  double &P);
};


#endif
