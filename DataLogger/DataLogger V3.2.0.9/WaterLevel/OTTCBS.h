//---------------------------------------------------------------------------

#ifndef OTTCBSH
#define OTTCBSH
//---------------------------------------------------------------------------

#include "WaterLevel.h"


struct STOTTCBSData {
	double Temperature;
	double Pressure;
	double Dept;
};


class TCBS : public TWaterLevel
{
public:		// Attribute
	double      AvgWaterLevel;
	double      MinWaterLevel;
	double      MaxWaterLevel;
	double      Temperature;
	double      Pressure;
	double      Dept;

	TList      *m_pList;

	int         m_Count;	// ÃøÁ¤ Count(Cycle È½¼ö);

	int         m_iIndex;

	AnsiString  RecvStr;

public:		// Method
	TCBS();
	~TCBS();

// Virtual method
	void Clear();
	void Init(TApdComPort *pPort);
	bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false);
	bool Finish();
	bool Decode(AnsiString RecvMsg, int iLen);
	bool Decode(BYTE *pBuffer, int iLen);
	bool Receive(AnsiString RecvMsg, int iLen);
	bool Receive(BYTE *pBuffer, int iLen);
	bool Send(AnsiString RecvMsg, int iLen);
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
};


#endif
