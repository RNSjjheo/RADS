//---------------------------------------------------------------------------

#ifndef AT200H
#define AT200H
//---------------------------------------------------------------------------

#include "WaterLevel.h"

#define AT200_START_STR                   ":0111EE\r\n"
#define AT200_TEMP_STR                    ":0103002D0008C7\r\n"
#define AT200_PRESSURE_STR                ":010300250008CF\r\n"
#define AT200_DEPT_STR                    ":010300350008BF\r\n"
#define AT200_ACTUAL_CONDUCITIVITY_STR	  ":0103003D0008B7\r\n"
#define AT200_SPECIFIC_CONDUCITIVITY_STR  ":010300450008AF\r\n"
#define AT200_RESISTIVITY_STR	          ":0103005D000897\r\n"
#define AT200_SALINITY_STR	              ":0103004D0008A7\r\n"
#define AT200_TOTAL_DISSOLVEDSOLIDS_STR	  ":0103005500089F\r\n"
#define AT200_WATER_DENSITY_STR	          ":0103006500088F\r\n"

enum EN_AT200_STEP { enAT200Idle, enAT200Start, enAT200Temp, enAT200Pressure, enAT200Depth, enAT200Salinity };

struct STAT200Data {
	double Temperature;
	double Pressure;
	double Depth;
	double ActualConductivity;
	double SpecificConductivity;
	double Resistivity;
	double Salinity;
	double TotalDissolvedSolids;
	double WaterDensity;
};


class TAT200 : public TWaterLevel
{
public:		// Attribute
	double       AvgWaterLevel;
	double       MinWaterLevel;
	double       MaxWaterLevel;
	double       Temperature;
	double       Pressure;
	double       Depth;
	double       ActualConductivity;
	double       SpecificConductivity;
	double       Resistivity;
	double       Salinity;
	double       TotalDissolvedSolids;
	double       WaterDensity;

	TList       *m_pList;

	bool        bStartAck;	// StartAck
	byte        RecvFlag;	// TROLL_INIT | TROLL_TEMPERATURE | TROLL_PRESSURE | TROLL_DEPTH

	AnsiString  StartAck;
	AnsiString  TempAck;
	AnsiString  PressureAck;
	AnsiString  DeptAck;
	AnsiString  SalinityAck;

	EN_AT200_STEP m_Status;
	int m_Count;	// ÃøÁ¤ Count(Cycle È½¼ö);

public:		// Method
	TAT200();
	~TAT200();

// Virtual method
	void Clear();
	void Init(TApdComPort *pPort);
	bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false);
	bool SendStart();
	bool SendTemperature();
	bool SendPressure();
	bool SendDepth();
	bool SendSalinity();
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

	bool CheckCRC(byte *pBuffer, int Length);
};




#endif
