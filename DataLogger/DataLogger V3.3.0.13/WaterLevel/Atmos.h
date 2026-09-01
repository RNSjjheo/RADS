//---------------------------------------------------------------------------

#ifndef AtmosH
#define AtmosH
//---------------------------------------------------------------------------

#include "Define.h"
#include "common.h"

#include <System.Math.hpp>

#include "AdPort.hpp"
#include "OoMisc.hpp"

struct STAtmos {
	double Temperature;
	double Pressure;
};


class TAtmos {
public:		// Attribute
	double      Temperature;
	double      Pressure;

	AnsiString  MeasureDate;
	AnsiString  MeasureTime;
	AnsiString  m_RecvMsg;

	TApdComPort *m_pComPort;
	TList       *m_pList;

	int         Count;	// ÃøÁ¤ Count(Cycle È½¼ö);
	char        m_Buffer[BUFFER_1K];

	EN_SENSOR_STEP Status;

public:		// Method
	TAtmos();
	~TAtmos();

	void Clear();
	void Init(TApdComPort *pPort);
	bool Start(UnicodeString MeasureDate, UnicodeString MeasureTime, bool bFirst=false);
	bool Finish();
	bool Decode(AnsiString RecvMsg, int iLen);
	bool Receive(AnsiString RecvMsg, int iLen);
	bool Send(AnsiString RecvMsg, int iLen);

	void Caculate();
	void GetData(double &T, double &P);

	bool ClearList();

	void LogMessage(UnicodeString sMsg, bool bFile=true);
};

#endif
