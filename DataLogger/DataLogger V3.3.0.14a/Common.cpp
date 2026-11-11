//---------------------------------------------------------------------------

#pragma hdrstop

#include "Common.h"

#include <stdlib.h>
#include <wchar.h>
#include <Winver.h>
#include <Windows.h>

#include "UnitMain.h"

//const UnicodeString InfoStr[10] = {"CompanyName", "FileDescription", "FileVersion", "InternalName", "LegalCopyright", "LegalTradeMarks", "OriginalFileName", "ProductName", "ProductVersion", "Comments"};
//for (int i = 0; i < 10; i++)
//{
//	Memo->Lines->Add(InfoStr[i] + " : " + VersionInfo(InfoStr[i]));
//}
UnicodeString VersionInfo(const UnicodeString &sQuery)
{
   DWORD dwHandle = 0, dwVersionInfoSize;
   UINT uLength;
   LPVOID pFileInfo, ptr;
   UnicodeString sOut; // 리턴될 버전 정보.

   UnicodeString filename = Application->ExeName;

   dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

   pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

   GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
   VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

   WORD *id = (WORD *) ptr;
   UnicodeString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

   VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
   sOut = UnicodeString((WCHAR*)ptr);
   HeapFree(GetProcessHeap(), 0, pFileInfo );
   return sOut;
}


//---------------------------------------------------------------------------
void ForcePCReboot()
//pc재부팅
{
	LUID             luid;
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, TEXT("SeShutdownPrivilege"), &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tp, 0, NULL, NULL);

    ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);

    Application->Terminate();
}

////////////////////////////////////////////////////////////////////////////////
//
// LT500/AT200 Process
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
float calcFraction(UnicodeString sFraction)
{
	float fData = 0.0;
	unsigned char fra = 0;
	UnicodeString s;
	for(int i = 0 ; i < sFraction.Length() ; i++ )
	{
		s = sFraction.SubString(i+1,1);

		if( s == "0" ) fra = 0;
		else           fra = 1;

		//if(sFraction[i] == '0' )
		//	fra = 0;
		//else
		//	fra = 1;

		fData +=  fra * pow(2.0,-(i+1));
	}
	return fData;
}

//------------------------------------------------------------------------------
int Bin2Dec(UnicodeString sBin)
{
	//return strtol(sBin,NULL,2);
	AnsiString a = sBin;
	return strtol(a.c_str(), NULL, 2);
}

//------------------------------------------------------------------------------
UnicodeString Hex2Bin(UnicodeString sHex)
{
	unsigned long dec = _tcstoul(sHex.c_str(), NULL, 16);

	AnsiString bin;
	AnsiString buf;
	TCHAR mod[2];
	do
	{
		_ultot(dec % 2, mod, 2);
		buf = mod;
		buf += bin;
		bin = buf;
	} while (dec = dec / 2);
	return bin;
}

//------------------------------------------------------------------------------
float Bin2IEEE745(UnicodeString sBin)
{
	float fRetfloat = 0.0;

	if( sBin.Length() < 31 ) // 맨첫자리가 0이면 표시되지않으므로 붙여주자
	{
		int nCnt = 31 - sBin.Length();
		for( int k = 0 ; k < nCnt ; k++)
		{
			sBin = _T("0") + sBin;
		}
	}

	if( sBin.Length() == 31 ) //양수
	{
		sBin = _T("0") + sBin;
	}

	if( sBin.Length() == 32 ) // 32부호비트 0 :양수 1:음수
	{
		int nBuho = 0;
		UnicodeString sSign = sBin.SubString(1,1);
		if ( sSign == "0" ) nBuho = 0;	// plus sign
		else                nBuho = 1;	// minus sign

		//if( sBin[0] == '0' ) //양수
		//	nBuho = 0;
		//else if( sBin[0] == '1' ) //음수
		//	nBuho = 1;

		UnicodeString sExponent =  sBin.SubString(2,8); // sBin.Mid(1,8);
		UnicodeString sFraction =  sBin.SubString(10,sBin.Length()-10 +1);	//sBin.Mid(9);

		int nJisu			= Bin2Dec(sExponent) - 127;
		double nExponent	= pow(2.0,nJisu);
		float fFraction		= calcFraction(sFraction);

		if(nBuho > 0 )
			fRetfloat = (-1) * nExponent  *  ( 1 + fFraction )  ;
		else
			fRetfloat = nExponent  *  ( 1 + fFraction )  ;

	}
	return fRetfloat;
}


//---------------------------------------------------------------------------
void HWait(int iMiliSecond)	//HeavenWait
{
	int iCnt = iMiliSecond / 50  + (((iMiliSecond % 50) > 0 ) ? 1 : 0);
	for ( int i = 0 ; i < iCnt ; i++ ) {
		Sleep(50);
		Application->ProcessMessages();
	}
	return;
}


#define MSGF_SLEEPMSG 0x5300

//---------------------------------------------------------------------------
BOOL SleepMsg(DWORD dwTimeout)
{
	DWORD dwStart = GetTickCount();
	DWORD dwElapsed;
	while ((dwElapsed = GetTickCount() - dwStart) < dwTimeout) {
		DWORD dwStatus = MsgWaitForMultipleObjectsEx(0, NULL,
						dwTimeout - dwElapsed, QS_ALLINPUT,
//						MWFMO_WAITANY | MWMO_INPUTAVAILABLE);
						0 | MWMO_INPUTAVAILABLE);
		if (dwStatus == WAIT_OBJECT_0) {
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					PostQuitMessage((int)msg.wParam);
					return FALSE; // abandoned due to WM_QUIT
				}
				if (!CallMsgFilter(&msg, MSGF_SLEEPMSG)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}// end of while
		} // end of if
	} // end of while
	return TRUE; // timed out
}


//---------------------------------------------------------------------------
void ComputeDept(STData &Data)
{
	double A,B,C,D,X;
	X = 1.019716;	// 고정 상수(dbar=>Dept(m)로 바꾸는 고정 상수)

	A = Data.APressure - Data.ABaseAtmos; // 기준압력에 대한 상대변화 추이
	B = A * 0.01;						 // hp => dbar로 변환
//	C = Data.Pressure - B; 				 // 단위: dbar  => Computed Pressure
	C = Data.Pressure - B; 				 // 2015.12.04 heaven2
	D = C * X;							 // Computed Dept

	Data.CPressure = C;
	Data.CDepth    = D;
}


//---------------------------------------------------------------------------
// Only hexascii text
AnsiString TrimHEX(AnsiString Message)
{
	int Length = Message.Length();
	char p[1024] = "";
	memset(p, 0x00, sizeof(p));

	char c = 0x00;
	int Pos = 0;
	for ( int i = 1 ; i <= Length ; i++ ) {
		c = Message.operator [](i);
		if ( c == ':' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) { // Only ':', 0~9, A~F
			p[Pos++] = c;
		}
	}
	return AnsiString(p);
}

//---------------------------------------------------------------------------
// Only hexascii text
// RQ-30d의 각 Serial Port당 할당된 RQ-30/RG-30 Device Address를 합쳐 놓은 상태: (ex) 5,4,3,2,1 (ex) 6,7,8,9,10
// DeviceAddress = "5,4,3,2,1" || DeviceAddress = "6,7,8,9,10"
int HowManyDeviceAddress(UnicodeString DeviceAddress)
{
	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = DeviceAddress;

	if ( pStrList->Count < 0 ) {
		//LogMessage("lack of address token");
		delete pStrList;	// release
		return -1;			// Address not exist
	}

//	int Count = pStrList->Count;
//	UnicodeString Address;
//	for ( int i = 0 ; i < pStrList->Count ; i++ ) {
//		Address = pStrList->Strings[i];
//		LogMessage(IntToStr(i+1) + "st Address=>" + Address);
//	}

	int Count = pStrList->Count;   // (ex) 5,4,3,2,1 => return 5
	delete pStrList;

	return Count;
}

//---------------------------------------------------------------------------
void LogMessage(UnicodeString Message)
{
	FormMain->LogMessage(Message);
}

#define PI 3.14159265359

/////////////////////////////////////////////////////////////////////////////
// AverageAngle Class
// Reference Project: RMeasureADCP
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// Constructor
AverageAngle::AverageAngle(const enum AngleType type)
{
  _type = type;
  reset();
}

//---------------------------------------------------------------------------
void AverageAngle::add(float alpha, float length)
{
  if (_type == AverageAngle::DEGREES )
  {
	alpha *= (PI / 180.0);
  }
  _sumx += (cos(alpha) * length);
  _sumy += (sin(alpha) * length);
  _count++;
}

//---------------------------------------------------------------------------
void AverageAngle::reset()
{
  _sumx = 0;
  _sumy = 0;
  _count = 0;
}

//---------------------------------------------------------------------------
float AverageAngle::getAverage()
{
  float angle = atan2(_sumy, _sumx);
  if (angle < 0) angle += (PI*2);
  if (_type == AverageAngle::DEGREES )
  {
    angle *= (180.0 / PI);
  }
  return angle;
}

//---------------------------------------------------------------------------
float AverageAngle::getTotalLength()
{
  if (_count == 0) return 0;
  return hypot(_sumy, _sumx);
}

//---------------------------------------------------------------------------
float AverageAngle::getAverageLength()
{
  if (_count == 0) return 0;
  return hypot(_sumy, _sumx) / _count;
}

#pragma package(smart_init)
