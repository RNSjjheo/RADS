//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH
//---------------------------------------------------------------------------

#include <vcl.h>
#include "Define.h"

UnicodeString VersionInfo(const UnicodeString &sQuery);
void ForcePCReboot();	// PC Rebooting

float         calcFraction(UnicodeString sFraction);
int           Bin2Dec(UnicodeString sBin);
UnicodeString Hex2Bin(UnicodeString sHex);
float         Bin2IEEE745(UnicodeString sBin);

// 지정한 시간만큼 기다린다.
void HWait(int iMiliSecond);
BOOL SleepMsg(DWORD dwTimeout);

// 2015.11.19
void ComputeDept(STData &Data);

AnsiString TrimHEX(AnsiString Message);


#endif
