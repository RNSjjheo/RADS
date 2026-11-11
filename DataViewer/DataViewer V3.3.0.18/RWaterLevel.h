//---------------------------------------------------------------------------
#include <vcl.h>
#include "fbTable.h"

#ifndef RWaterLevelH
#define RWaterLevelH
//---------------------------------------------------------------------------
class TRWaterLevel : public TfbTable
{
private:
    float fOffset;
public:
    TRWaterLevel(TSQLConnection *Connection, float fWLOffset);
    char GetAdvText(AnsiString &sResult);
};
//---------------------------------------------------------------------------
#endif
