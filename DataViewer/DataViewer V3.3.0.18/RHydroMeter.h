//---------------------------------------------------------------------------
#include <vcl.h>
#include "fbTable.h"
#include "DvEnv.h"

#ifndef RHydroMeterH
#define RHydroMeterH
//---------------------------------------------------------------------------
class TRHydroMeter : public TfbTable
{
private:
    UnicodeString sMeasureDate;
    UnicodeString sMeasureTime;
    int iUnitNumber;
	void _GetAdvTextCM(TRHydroMeterHeader &RHydroMeterHeader);
    void _GetAdvTextSL(TRHydroMeterHeader &RHydroMeterHeader);
    void _GetAdvTextAG(TRHydroMeterHeader &RHydroMeterHeader);
public:
    TRHydroMeter(TSQLConnection *Connection, int iUnitNo);
	UnicodeString Open(); //파라미터로 자료 오픈
    char GetAdvText(AnsiString &sResult);
};
//---------------------------------------------------------------------------
#endif
