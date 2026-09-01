//---------------------------------------------------------------------------
#ifndef RHydroMeterCellH
#define RHydroMeterCellH

//#include <vcl.h>
#include "fbTable.h"

//---------------------------------------------------------------------------
class TRHydroMeterCell : public TfbTable
{
private:
    int iUnitNumber;
    int iWN;
    char _GetAdvTextCM(AnsiString &sResult);
    char _GetAdvTextSL(AnsiString &sResult);
    char _GetAdvTextAG(AnsiString &sResult);
	char _GetAdvTextRQ30D(AnsiString &sResult);
public:
	double SumSlaveLearnDischarge;	// 2023.03.15 for Only RQ-30d
    TRHydroMeterCell(TSQLConnection *Connection, int iUnitNo, int iWN);
    TRHydroMeterCell(TSQLConnection *Connection, int iUnitNo);	// heaven2 2024.10.04
	~TRHydroMeterCell();
    char GetAdvText(AnsiString &sResult, UnicodeString sHydroKind);
};
//---------------------------------------------------------------------------
#endif
