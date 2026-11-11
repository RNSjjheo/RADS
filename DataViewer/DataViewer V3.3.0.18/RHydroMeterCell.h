//---------------------------------------------------------------------------
#include <vcl.h>
#include "fbTable.h"

#ifndef RHydroMeterCellH
#define RHydroMeterCellH
//---------------------------------------------------------------------------
class TRHydroMeterCell : public TfbTable
{
private:
    int iUnitNumber;
    int iWN;
    char _GetAdvTextCM(AnsiString &sResult);
    char _GetAdvTextSL(AnsiString &sResult);
    char _GetAdvTextAG(AnsiString &sResult);
public:
    TRHydroMeterCell(TSQLConnection *Connection, int iUnitNo, int iWN);
    char GetAdvText(AnsiString &sResult, UnicodeString sHydroKind);
};
//---------------------------------------------------------------------------
#endif
