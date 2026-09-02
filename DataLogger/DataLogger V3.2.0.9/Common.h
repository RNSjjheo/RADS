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

// RQ-30d의 각 Serial Port당 할당된 RQ-30/RG-30 Device Address를 합쳐 놓은 상태: (ex) 5,4,3,2,1 (ex) 6,7,8,9,10
int HowManyDeviceAddress(UnicodeString DeviceAddress);

void LogMessage(UnicodeString Message);


#endif
