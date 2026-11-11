//---------------------------------------------------------------------------
#include <vcl.h>
#include "fbTable.h"

#ifndef RVTHLoggerH
#define RVTHLoggerH
//---------------------------------------------------------------------------
class TRVTHLogger : public TfbTable
{
private:
public:
    TRVTHLogger(TSQLConnection *Connection);
    char GetAdvText(AnsiString &sResult);
};
//---------------------------------------------------------------------------
#endif
