//---------------------------------------------------------------------------
#ifndef RWaterLevelH
#define RWaterLevelH

#include <vcl.h>
#include "fbTable.h"

//---------------------------------------------------------------------------
class TRWaterLevel : public TfbTable
{
private:
	float fOffset;
    UnicodeString sMeasureDate;
    UnicodeString sMeasureTime;
public:
	TRWaterLevel(TSQLConnection *Connection, float fWLOffset);
	void SetMeasureDateTime(UnicodeString MeasureDate, UnicodeString MeasureTime);
	~TRWaterLevel();
	char GetAdvText(AnsiString &sResult);
	char GetWindText(AnsiString &sResult);	// for EWSV Wind Data
};
//---------------------------------------------------------------------------
#endif
