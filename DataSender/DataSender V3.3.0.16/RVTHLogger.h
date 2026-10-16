//---------------------------------------------------------------------------

#ifndef RVTHLoggerH
#define RVTHLoggerH

//#include <vcl.h>
#include "fbTable.h"

//---------------------------------------------------------------------------
class TRVTHLogger : public TfbTable
{
private:
public:
    TRVTHLogger(TSQLConnection *Connection);
    ~TRVTHLogger();
    char GetAdvText(AnsiString &sResult);
};
//---------------------------------------------------------------------------
#endif
