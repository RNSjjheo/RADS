//---------------------------------------------------------------------------
#pragma hdrstop

#include <vcl.h>
#include <stdio.h>
#include <winver.h>
#include <registry.hpp>
#include <dir.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
//#include "IdHashMessageDigest.hpp"
#include <Winver.h>
#include <Windows.h>
#include <winbase.h>

#include "LockeCommon.h"
#include "Env.h"

#include "GuardnetThread.h"


#include <iphlpapi.h>		// Routing 정보
#pragma comment(lib, "iphlpapi.lib")


//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "ws2_32.lib")

#include <psapi.h>					// Process정보, Memory 사용량 조회
#pragma comment(lib, "psapi.lib")

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
UnicodeString TextToHex(UnicodeString sText, UnicodeString sDelimiter)
{
    UnicodeString sResult = "";

    for(int i = 1; i <= sText.Length(); i++)
    {
        if (i > 1 && sDelimiter != "") sResult += sDelimiter;
        unsigned char uc = sText[i];
        sResult += IntToHex((int)uc, 2);
    } // for i

    return sResult;
}

//---------------------------------------------------------------------------
UnicodeString TextToHex(char *sText, int iSize, UnicodeString sDelimiter)
{
    UnicodeString sResult = "";

    for(int i = 0; i < iSize; i++)
    {
        if (i > 0 && sDelimiter != "") sResult += sDelimiter;
        unsigned char uc = sText[i];
        sResult += IntToHex((int)uc, 2);
    } // for i

    return sResult;
}

//---------------------------------------------------------------------------
UnicodeString SockPacketValid(char *Buffer, int iSize)
//Buffer: STX ~ ETX까지의 패킷 데이터
//return: "" 이면 정상 아니면 오류 메시지
{
	if (Buffer == NULL) return "검사할 패킷자료가 없습니다.";
	UnicodeString sResult = "";

	char cSource = Buffer[0];
	char cCRC = Buffer[iSize - 2];
	for(int i = 1; i < iSize - 2; i++)
	{
		cSource ^= Buffer[i];
	} // for i
	cSource &= 0x7f;

	if (sResult == "" && cCRC != cSource) sResult = "CRC에러, 패킷[" + (UnicodeString)Buffer + "] 패킷CRC=" + UnicodeString(cCRC) + " 자체CRC=" + UnicodeString(cSource);
	return sResult;
}

//---------------------------------------------------------------------------
char *EncodeSockPacket(char cType, char *sData, int iLen)
{
	char *sResult = (char*)GlobalAlloc(GPTR, sizeof(char) * (iLen + 5));

	int idx = 0;
	sResult[idx++] = (char)0xfa; //stx
	sResult[idx++] = (char)cType; //type
	sResult[idx++] = (char)iLen; //len
	for(int i = 0; i < iLen; i++) sResult[idx++] = sData[i];
	sResult[idx++] = (char)GetPacketCrc(sResult, idx); //crc
	sResult[idx++] = (char)0xf5; //etx

	return sResult;
}

//---------------------------------------------------------------------------
char GetPacketCrc(UnicodeString sText)
//패킷crc를 리턴
//sText: STX ~ Data까지여야 함
{
	if (sText == "") return char(0x0);

	char cSource = sText[1];
	for(int i = 2; i <= sText.Length(); i++)
	{
		cSource ^= sText[i];
	} // for i
	cSource &= 0x7f;
	return cSource;
}

//---------------------------------------------------------------------------
char GetPacketCrc(char *sText, int iLen)
//CRC 값을 리턴
//sText: STX ~ Data까지여야 함
{
	if (sText == NULL) return char(0x0);

	char cSource = sText[0];
	for(int i = 1; i < iLen; i++)
	{
		cSource ^= sText[i];
	} // for i
	cSource &= 0x7f;
	return cSource;
}

//---------------------------------------------------------------------------
TSQLQuery* CreateQuery(TSQLConnection *DbConnection)
{
	TSQLQuery *sql = new TSQLQuery(DbConnection->Owner);
	sql->SQLConnection = DbConnection;
	return sql;
}

//---------------------------------------------------------------------------
bool SQLExists(TSQLConnection *DbConnection, UnicodeString sSQL)
{
	bool bResult = true;
	TSQLQuery *sql = CreateQuery(DbConnection);
	sql->SQL->Text = sSQL;
	sql->Open();
	bResult = !(sql->Bof && sql->Eof);
	delete sql;
	return bResult;
}

//---------------------------------------------------------------------------
UnicodeString GetDatabaseDateTime(TSQLConnection *DbConnection, TDateTime &DBDateTime)
{
	UnicodeString Err = "";
	bool bResult = false;
	TSQLQuery *sql = CreateQuery(DbConnection);
	UnicodeString sSQL = "SELECT current_date, current_time FROM rdb$database";
	TDateTime date, time;

	sql->SQL->Text = sSQL;
	try {
		sql->Open();
		while ( !sql->Eof ) {
			try {
				date = sql->FieldByName("current_date")->AsDateTime;
				time = sql->FieldByName("current_time")->AsDateTime;
				sql->Next();
			} catch ( Exception &e ) {
				Err = "GetDatabaseDateTime() " + e.Message;
				bResult = false;
				break;
			}
		}
		DBDateTime = date + time;
		Err = "";
		bResult = true;
	} catch ( Exception &e ) {
		Err = "GetDatabaseDateTime() " + e.Message;
		bResult = false;
	}
	delete sql;

	return Err;
}


//---------------------------------------------------------------------------
UnicodeString GetDecimalStr(UnicodeString sValue)
//수치만 가져온다
{
	UnicodeString sResult = "";

	for(int i = 1; i <= sValue.Length(); i++)
	{
		int j = StrToIntDef(sValue[i], -1);
		if (j != -1) sResult += IntToStr(j);
	} // for i

	return sResult;
}

//---------------------------------------------------------------------------
int GetSepItemCount(UnicodeString sText, UnicodeString sFilter)
//식별문자를 통한 아이템의 개수 리턴
{
	int iResult = 0;
	for(int i = 1; i <= sText.Length(); i++)
	{
		if (sFilter.Pos(sText[i]) > 0) iResult += 1;
	} // for i
	if (sText.Length() > 0) iResult += 1;

	return iResult;
}

//---------------------------------------------------------------------------
UnicodeString GetSepItem(UnicodeString sText, int idx, UnicodeString sFilter)
//식별문자를 기준으로 한 인덱스의 문자열 리턴
{
	UnicodeString sResult = "";
	int index = 0, iFirst = 1, iLast = -1;

	for(int i = 1; i <= sText.Length(); i++)
	{
		if (sFilter.Pos(sText[i]) > 0)
		{
			iFirst = iLast + 2;
			iLast = i - 1;
			if (idx == index)
			{
				sResult = sText.SubString(iFirst, iLast - iFirst + 1);
				return sResult;
			} // if
			index += 1;
		} // if
	} // for i
	if (sText.Length() > 0)
	{
		iFirst = iLast + 2;
		if (idx == index) sResult = sText.SubString(iFirst, 1024);
	} // if

	//return sResult;	//heaven2 2015.02.23
	return Trim(sResult);	// Trim Added 2015.02.23
}

//---------------------------------------------------------------------------
UnicodeString GetInt2String(int iValue)
//integer 4바이트의 UnicodeString 리턴
{
	TszDWord szdw;
	szdw.iValue = iValue;
	UnicodeString sResult = "";
	for (int i = 0; i < 4; i++)
	{
		sResult += (char)szdw.szData[i];
	} // for i
	return sResult;
}

//---------------------------------------------------------------------------
UnicodeString ExtractFtpFilePath(UnicodeString sAddress)
{
    UnicodeString sPath = "";

    for(int i = 0; i < GetSepItemCount(sAddress, "/") - 1; i++)
    {
        if (i > 0) sPath += "/";
        sPath += GetSepItem(sAddress, i, "/");
    } // for i

    return sPath;
}

//---------------------------------------------------------------------------
UnicodeString ExtractFtpFileName(UnicodeString sAddress)
{
    int iCnt = GetSepItemCount(sAddress, "/");
    return GetSepItem(sAddress, iCnt - 1, "/");
}

//---------------------------------------------------------------------------
UnicodeString ToFloatDefStr(UnicodeString sFormat, UnicodeString sValue, float fDefault)
{
    UnicodeString sResult = "";

    try
    {
        sResult.sprintf(sFormat.c_str(), StrToFloat(sValue));
    } catch(...)
    {
        sResult.sprintf(sFormat.c_str(), fDefault);
    } // catch

    return sResult;
}

//---------------------------------------------------------------------------
void SetBitmapImageListIndex(TImage *image, TImageList *il, int idx)
{
    TBitmap *pBitmap = new TBitmap();
    try
    {
        il->GetBitmap(idx, pBitmap);
        image->Picture->Bitmap = pBitmap;
    } catch(...) {}
    delete pBitmap;
}

//---------------------------------------------------------------------------
void WriteFileLog(UnicodeString sPathFile, UnicodeString sDate, UnicodeString sText)
{
    AnsiString sAText = "[" + sDate.SubString(12, 8) + "]" + sText + "\r\n";
    static UnicodeString sLastPathFile = "";

    THandle hFile;
    try
    {
        if (sLastPathFile != sPathFile) //파일명이 바뀌었을 때만 폴더체크해서 생성처리
        {
            ForceDirectories(ExtractFilePath(sPathFile));
            sLastPathFile = sPathFile;
        } // if
        if (FileExists(sPathFile))
        {
            hFile = FileOpen(sPathFile, fmOpenWrite | fmShareDenyNone);
            FileSeek(hFile, 0, 2);
        } else hFile = FileCreate(sPathFile, fmOpenWrite | fmShareDenyNone);
        FileWrite(hFile, sAText.c_str(), sAText.Length());
    } // try
    __finally
    {
		if (hFile > 0) FileClose(hFile);
    } // catch
}

//---------------------------------------------------------------------------
//UnicodeString GetMD5String(UnicodeString sText)
////md5로 인코딩된 16진수 문자열 리턴
//{
//	AnsiString sResult = "";
//	TIdHashMessageDigest5 *idmd5;
//
//	idmd5 = new TIdHashMessageDigest5;
//	try
//	{
//		sResult = idmd5->HashStringAsHex(sText, enUTF8);
//	} catch(...) {}
//	delete idmd5;
//
//	return sResult;
//}

//---------------------------------------------------------------------------
UnicodeString getHDDSerial()
{
	UnicodeString sHDD = "";
	DWORD SerialNum;
	DWORD A, B;
	TCHAR C[256];
	TCHAR Buffer[256];

	if (GetVolumeInformation((wchar_t*)"C:\\", Buffer, 256, &SerialNum, &A, &B, C, 256))
	{
		sHDD = IntToStr((int)SerialNum);
	}
	return sHDD;
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

//---------------------------------------------------------------------------
UnicodeString PathStrAdd(UnicodeString sSourcePath, UnicodeString sSubPath)
//경로를 합침
{
    UnicodeString sResult = sSourcePath + "\\" + sSubPath;
    sResult = StringReplace(sResult, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
    return sResult;
}

//---------------------------------------------------------------------------
UnicodeString ReadIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sDefault)
{
    UnicodeString sResult = "";

	TIniFile *ini = new TIniFile(sIniFileName);
	sResult = ini->ReadString(sSection, sKey, sDefault);
	delete ini;

    return sResult;
}

//---------------------------------------------------------------------------
void WriteIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sValue)
{
    TIniFile *ini = new TIniFile(sIniFileName);

    ini->WriteString(sSection, sKey, sValue);

    delete ini;
}

//---------------------------------------------------------------------------
void RegisterRun(UnicodeString sName, UnicodeString sValue)
{
    //runonce에 권한상승된 datasender를 추가하여 다음 부팅시에도 자동으로 뜨도록 매번 레지스트리에 등록
    TRegistry *reg = new TRegistry;
    reg->RootKey = HKEY_LOCAL_MACHINE;
    reg->Access = KEY_ALL_ACCESS;
    reg->OpenKey("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", false);
    reg->WriteString("DataSender", Application->ExeName);
    reg->CloseKey();
    delete reg;
}

//---------------------------------------------------------------------------
UnicodeString GetAppModifiedDateStr()
{
    UnicodeString sResult = "";

    struct _wffblk st_wffblk;
    if (_wfindfirst(Application->ExeName.c_str(), &st_wffblk, FA_ARCH) != -1)
    {
        int iYear  = ((st_wffblk.ff_fdate >> 9) & 0x007f) + 1980; //Bits 9-15 Years 1111 1110 0000 0000 since 1980 (for example 9 here means 1989)
        int iMonth = ((st_wffblk.ff_fdate >> 5) & 0x000f);        //Bits 5-8 Month  0000 0001 1110 0000
        int iDay   = ((st_wffblk.ff_fdate >> 0) & 0x001f);        //Bits 0-4 Day    0000 0000 0001 1111
        sResult = FormatFloat("0000", iYear) + "." +
                  FormatFloat("00", iMonth) + "." +
                  FormatFloat("00", iDay);
        _wfindclose(&st_wffblk);
    } // if

    return sResult;
}

//---------------------------------------------------------------------------
UnicodeString GetAppVersion()
{
    UnicodeString sResult = "";

    DWORD dwHandle;
    LPVOID ptr;
    UINT uLength;

    AnsiString sExeName = Application->ExeName;
    DWORD dwVersionInfoSize = GetFileVersionInfoSizeA(sExeName.c_str(), &dwHandle);
    LPVOID pFileInfo = (LPVOID)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

    GetFileVersionInfoA(sExeName.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
    VerQueryValueA(pFileInfo, "\\VarFileInfo\\Translation", &ptr, &uLength);

    WORD *id = (WORD *)ptr;
    char szString[255] = { 0, };
    sprintf(szString, "\\StringFileInfo\\%04x%04x\\%s", id[0], id[1], "FileVersion");
    if (VerQueryValueA(pFileInfo, szString, &ptr, &uLength))
    {
        char sOut[255] = { 0, };
        strcpy(sOut, (char*)ptr);
        sResult = sOut;
    } // if

    HeapFree(GetProcessHeap(), 0, pFileInfo);
    return sResult;
}

//---------------------------------------------------------------------------
AnsiString Domain2IP(AnsiString sDomain)
{
    AnsiString sResult = "";

    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;

    DWORD dwError;

    struct hostent *remoteHost;
    char *host_name = sDomain.c_str();
    struct in_addr addr;

    char **pAlias;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) return sResult;

    remoteHost = gethostbyname(host_name);

    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) return sResult;
    } else {
        if (remoteHost->h_addrtype == AF_INET)
        {
            int i = 0;
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                sResult = (AnsiString)inet_ntoa(addr);
            }
        }
    }

    return sResult;
}

//---------------------------------------------------------------------------
// heaven2 2017.10.17
AnsiString IcmpPing(AnsiString sDestineIP, AnsiString sSourceIP)
{
	AnsiString sResult = "";
	if ( sSourceIP.IsEmpty()) {
		sResult = "Source IP is empty..";
		return sResult;
	}

    HANDLE hIcmpFile;
	//unsigned long ipaddr = INADDR_NONE;
	unsigned long DestineAddr = INADDR_NONE;
	unsigned long SourceAddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

	DestineAddr = inet_addr(sDestineIP.c_str());
	if (DestineAddr == INADDR_NONE) {
		sResult = "not proper ip: " + sDestineIP;
		WSACleanup();
		return sResult;
	}
	SourceAddr = inet_addr(sSourceIP.c_str());
	if (SourceAddr == INADDR_NONE) {
		sResult = "not proper ip: " + sSourceIP;
		WSACleanup();
		return sResult;
	}

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        sResult = "Unable to open handle";
        WSACleanup();
        return sResult;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        sResult = "Unable to allocate memory";
        WSACleanup();
        return sResult;
    }

	bool bXP = IsWindowsXP();	// WindowsXP 이하 ( WindowXP... )
	if ( bXP ) {  				// XP 이하 => Source Address 지정이 없다.
		dwRetVal = IcmpSendEcho(hIcmpFile, DestineAddr, SendData, sizeof(SendData),
								NULL, ReplyBuffer, ReplySize, 1000);
//debug
//OutputDebugString(L"WinXP Ping Test\r\n");

	} else {	// WindowsXP 이상 ( Windows Vista, above
		dwRetVal = IcmpSendEcho2Ex(hIcmpFile, NULL, NULL, NULL,SourceAddr, DestineAddr, SendData, sizeof(SendData),
				   NULL, ReplyBuffer, ReplySize, 1000);
//debug
//OutputDebugString(L"Win7 Ping Test\r\n");
	}

    if (dwRetVal != 0) {
		//성공
		sResult = "reply 없음";
		AnsiString sResponse = "";
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		char *cp = (char *)pEchoReply->Data;
		if (strlen(cp) > 0) sResult = "";
//        if (dwRetVal > 1) {
//			sResponse += "Received " + IntToStr((int)dwRetVal) + " icmp message responses\nInformation from the first response:";
//			sResult = ""; //리턴값이 있으면 성공
//        }
//        else {
//			sResponse += "Received " + IntToStr((int)dwRetVal) + " icmp message response\nInformation from this response:";
//			sResult = ""; //리턴값이 있으면 성공
//		}
		sResponse += "Received from " + sDestineIP;
        sResponse += " Status = " + IntToStr((int)pEchoReply->Status);
        sResponse += " Roundtrip time = " + IntToStr((int)pEchoReply->RoundTripTime) + " milliseconds";
    }
	else
	{
		sResult ="IcmpSendEcho returned error: " + IntToStr((int)GetLastError());
	}

	free(ReplyBuffer);	// Release Buffer ( Ping Buffer )

	if (hIcmpFile != INVALID_HANDLE_VALUE) IcmpCloseHandle(hIcmpFile);

	WSACleanup();
	return sResult;
}

//---------------------------------------------------------------------------
//Windows 7                   6.1
//Windows Server 2008 R2      6.1
//Windows Server 2008         6.0
//Windows Vista               6.0
//Windows Server 2003 R2      5.2
//Windows Server 2003         5.2
//Windows XP 64-Bit Edition   5.2
//Windows XP                  5.1
//Windows 2000                5.0
bool IsWindowsXP()
{
	bool bXP = true;
	OSVERSIONINFO OSVersionInfo;
	BOOL bResult = FALSE;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);

	bResult = GetVersionEx(&OSVersionInfo);
	if ( !bResult ) {
		return bXP;	// default => WindowsXP => Default Ping Test
	} else {
		if ( OSVersionInfo.dwMajorVersion <= 5 ) {
			bXP = true;		// WindowsXP, Window98,...
		} else {
			bXP = false;	// Vista, Windows7, Windows8/10/...
		}
	}
	return bXP;
}

//---------------------------------------------------------------------------
UnicodeString GetIPByIF(int IF)
{
	UnicodeString sIP = "";

	PIP_ADAPTER_INFO pAdapterInfo;
	ULONG            OutBufLen = 0;
	DWORD            dwResult = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
	int iSize = sizeof(IP_ADAPTER_INFO);

	dwResult = GetAdaptersInfo(pAdapterInfo, &OutBufLen);
	if ( dwResult == ERROR_BUFFER_OVERFLOW ) {
		//GlobalFree(pAdapterInfo);
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc(OutBufLen);
	}

	if ( GetAdaptersInfo(pAdapterInfo, &OutBufLen) == NO_ERROR ) {

		int iEthCount = 0;
		IP_ADAPTER_INFO *pNext = NULL;
		pNext = pAdapterInfo;

		int iIndex = 0;
		int iType = 0;

		while ( pNext != NULL ) {
			iIndex = pNext->Index;	// Interface Index number

			if ( iIndex == IF ) {
				sIP = pNext->IpAddressList.IpAddress.String;
				break;
			}
			pNext = pNext->Next;
		}
		//GlobalFree(pNext);
		//GlobalFree(pAdapterInfo);
	}
	free(pAdapterInfo);

	return sIP;
}


//---------------------------------------------------------------------------
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES           3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x)   HeapFree(GetProcessHeap(), 0, (x))

UnicodeString GetIfInfo(UnicodeString sError)
/*
    리턴데이터 형식
    <item1>|<item2>|...

    item 형식
    <IF>:<장치명>

    예)
    11:Qualcomm Lan card|14:Wireless lancard
*/
{
	UnicodeString sResult = "";

    /* Declare and initialize variables */

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	//IP_ADAPTER_PREFIX *pPrefix = NULL;

    family = AF_INET; //AF_INET6;

	// Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) {
            sError = "Memory allocation failed for IP_ADAPTER_ADDRESSES struct";
            return sResult;
        }

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        } else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
			//if (pCurrAddresses->IfIndex >= 11) {  // heaven2 2017.10.10 1 => loopback address
			if (pCurrAddresses->IfIndex > 1) {		// zero if IPv4 is not available on the interface
				if (sResult != "") sResult += "|";
				sResult += IntToStr((int)pCurrAddresses->IfIndex) + ":";
                sResult += (UnicodeString)pCurrAddresses->Description;
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    } else {
        sError = "Call to GetAdaptersAddresses failed with error: " + IntToStr((int)dwRetVal);
    }

    if (pAddresses) {
        FREE(pAddresses);
    }

	return sResult;
}


//---------------------------------------------------------------------------
// IF Descritpion으로 IF 번호를 찾는다 .
int  GetIFByDescription(UnicodeString Description)
{
	UnicodeString sResult = "";
	UnicodeString sError = "";
	bool bFound = false;

	/* Declare and initialize variables */

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
	PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	//IP_ADAPTER_PREFIX *pPrefix = NULL;

	family = AF_INET; //AF_INET6;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {
		pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
		if (pAddresses == NULL) {
			sError = "Memory allocation failed for IP_ADAPTER_ADDRESSES struct";
			return -1;	// error
		}

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		} else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	int IF = -1;	// -1: not found, > 0 :  found

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {
			//if (pCurrAddresses->IfIndex >= 11) {  // heaven2 2017.10.10 1 => loopback address
			if (pCurrAddresses->IfIndex > 1) {		// zero if IPv4 is not available on the interface
				UnicodeString xDesc = (UnicodeString)pCurrAddresses->Description;
				if ( xDesc == Description ) {
					bFound = true;
					IF = (int) pCurrAddresses->IfIndex;
					break;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		} // end while
	} else {
		sError = "Call to GetAdaptersAddresses failed with error: " + IntToStr((int)dwRetVal);
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

	return IF;
}

//---------------------------------------------------------------------------
TDateTime GetStrToDateTime(UnicodeString sDate, UnicodeString sTime)
{
    TDateTime dtResult = EncodeDate(sDate.SubString(1, 4).ToIntDef(1901),
                                    sDate.SubString(5, 2).ToIntDef(00),
                                    sDate.SubString(7, 2).ToIntDef(00)
                                   ) +
                         EncodeTime(sTime.SubString(1, 2).ToIntDef(0),
                                    sTime.SubString(3, 2).ToIntDef(0),
                                    sTime.SubString(5, 2).ToIntDef(0),
                                    0
                                   );
    return dtResult;
}

//---------------------------------------------------------------------------
SIZE_T GetProcessWorkingSetSize()
//현재의 메모리 사용량 리턴
{
    SIZE_T ulResult = 0;

    DWORD ProcessID = GetCurrentProcessId();
    HANDLE hProcess;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) ulResult = pmc.WorkingSetSize / 1024;

    CloseHandle(hProcess);
    return ulResult;
}

//---------------------------------------------------------------------------
BYTE MakeCRC(BYTE *pBuff, int iLen)
{
	BYTE crc = 0x00;
	for ( int i = 0 ; i < iLen ; i++ )
		crc ^= *(pBuff + i);

	return crc & 0x7F;
}

//---------------------------------------------------------------------------
int  CheckFrame(BYTE *pBuff, int iLen)
{
	if ( *(pBuff+0) != STX )      return -1;	// STX Error
	if ( *(pBuff+(iLen-1)) != ETX ) return -2;	// ETX Error

	BYTE crc = 0x00;
	for ( int i = 0 ; i < iLen-2 ; i++ )
		crc ^= *(pBuff + i);

	crc = crc & 0x7F;

	if ( *(pBuff+(iLen-2)) != crc ) return -3;	// CRC Error

	return 0;
}

//---------------------------------------------------------------------------
void HWait(int iMiliSecond)	//HeavenWait
{
	int iCnt = iMiliSecond / 100  + (((iMiliSecond % 100) > 0 ) ? 1 : 0);
	for ( int i = 0 ; i < iCnt ; i++ ) {
		Sleep(100);
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
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				PostQuitMessage((int)msg.wParam);
				return FALSE; // abandoned due to WM_QUIT
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}// end of while
	} // end of while
	Application->ProcessMessages();	// Process another event
	Sleep(50);	// Wait for while
	Application->ProcessMessages();	// Process another event
	return TRUE;

//		if (dwStatus == WAIT_OBJECT_0) {
//			MSG msg;
//			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				if (msg.message == WM_QUIT) {
//					PostQuitMessage((int)msg.wParam);
//					return FALSE; // abandoned due to WM_QUIT
//				}
//				//if (!CallMsgFilter(&msg, MSGF_SLEEPMSG)) {
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//				//}
//			}// end of while
//		} // end of if
//		else
//		{
//			MSG msg;
//			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				if (msg.message == WM_QUIT) {
//					PostQuitMessage((int)msg.wParam);
//					return FALSE; // abandoned due to WM_QUIT
//				}
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			} // while
//		} // end of if
//	} // end of while
//	return TRUE; // timed out
}

#define FW(x,y) FindWindowEx(x, NULL, y, L"")

void TrayIconRefresh()	//2015.10.02
{
  HWND  wnd;
  HWND  SysPager;
  TRect rec;
  DWORD w,h;
  DWORD x,y;

  // find a handle of a tray
  wnd = FindWindow(L"Shell_TrayWnd", NULL );

  if( wnd!=NULL )
	wnd = FindWindowEx( wnd, 0, L"TrayNotifyWnd", NULL );

  if( wnd!=NULL ) {
	wnd = FindWindowEx( wnd, 0, L"SysPager", NULL );
	SysPager = wnd;
  }

  if( wnd!=NULL )
	wnd = FindWindowEx( wnd, 0, L"ToolbarWindow32", NULL );

  // get client rectangle (needed for width and height of tray)

  if( wnd!=NULL )
  {

	::GetClientRect( wnd, &rec );

	// get size of small icons
	w = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
	h = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);

	// initial y position of mouse - half of height of icon
	y = w>>1;
	while( y < (DWORD)rec.Bottom )  // while y < height of tray
	{
	  x = h>>1;              // initial x position of mouse - half of width of icon
	  while( x < (DWORD)rec.Right ) // while x < width of tray
	  {
		SendMessage( wnd, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
		x = x + w; // add width of icon to x position
	  }
	  y = y + h; // add height of icon to y position
	}
  }

  // extra wind
  HWND hNotifyIcon, hVisibleIcon, hHiddenIcon;

  hNotifyIcon  = FindWindowEx(SysPager,0, L"ToolbarWindow32", L"Notification Area");
  hVisibleIcon = FindWindowEx(SysPager,0, L"ToolbarWindow32", L"User Promoted Notification Area");
  hHiddenIcon  = FindWindowEx(FW(NULL, L"NotifyIconOverflowWindow"),
								 NULL,
								 L"ToolbarWindow32",
								 L"Overflow Notification Area");

  if( hNotifyIcon!=NULL )
  {
	::GetClientRect( hNotifyIcon, &rec );

	// get size of small icons
	w = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
	h = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);

	// initial y position of mouse - half of height of icon
	y = w>>1;
	while( y < (DWORD)rec.Bottom )  // while y < height of tray
	{
	  x = h>>1;              // initial x position of mouse - half of width of icon
	  while( x < (DWORD)rec.Right ) // while x < width of tray
	  {
		SendMessage( hNotifyIcon, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
		x = x + w; // add width of icon to x position
	  }
	  y = y + h; // add height of icon to y position
	}
  }

  if( hVisibleIcon!=NULL )
  {
	::GetClientRect( hVisibleIcon, &rec );

	// get size of small icons
	w = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
	h = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);

	// initial y position of mouse - half of height of icon
	y = w>>1;
	while( y < (DWORD)rec.Bottom )  // while y < height of tray
	{
	  x = h>>1;              // initial x position of mouse - half of width of icon
	  while( x < (DWORD)rec.Right ) // while x < width of tray
	  {
		SendMessage( hVisibleIcon, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
		x = x + w; // add width of icon to x position
	  }
	  y = y + h; // add height of icon to y position
	}
  }

  if( hHiddenIcon!=NULL )
  {
	::GetClientRect( hHiddenIcon, &rec );

	// get size of small icons
	w = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
	h = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);

	// initial y position of mouse - half of height of icon
	y = w>>1;
	while( y < (DWORD)rec.Bottom )  // while y < height of tray
	{
	  x = h>>1;              // initial x position of mouse - half of width of icon
	  while( x < (DWORD)rec.Right ) // while x < width of tray
	  {
		SendMessage( hHiddenIcon, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
		x = x + w; // add width of icon to x position
	  }
	  y = y + h; // add height of icon to y position
	}
  }
}

//---------------------------------------------------------------------------
//
// GuardnetThread.cpp 에서 피안온 Function
//
//---------------------------------------------------------------------------
MIB_IPFORWARDTABLE *_GetForwardTable()
{
	MIB_IPFORWARDTABLE *prTable = NULL;
	DWORD dwTableSize = 0;
	prTable = (MIB_IPFORWARDTABLE*)malloc(sizeof(MIB_IPFORWARDTABLE));
	if (GetIpForwardTable(prTable, &dwTableSize, false) == ERROR_INSUFFICIENT_BUFFER)
	{
		free(prTable);
		prTable = (MIB_IPFORWARDTABLE*)malloc(dwTableSize);
	} // if
	DWORD dwRetVal = GetIpForwardTable(prTable, &dwTableSize, false);
	if (dwRetVal != NO_ERROR && prTable != NULL)
	{
		free(prTable);
		prTable = NULL;
	} // else
	return prTable;
}

//---------------------------------------------------------------------------
void _ChangeRoutingTable(int iLineIF, int iWirelessIF, bool bWire2Wireless)
{
	int iLineMetric = 0;
	int iWirelessMetric = 0;
	MIB_IPFORWARDROW iprowLine, *piprowLine = NULL;
	MIB_IPFORWARDROW iprowWireless, *piprowWireless = NULL;

	MIB_IPFORWARDTABLE *prTable = _GetForwardTable();

	if (prTable != NULL)
    {
        for (int i = 0; i < prTable->dwNumEntries; i++)
        {
            MIB_IPFORWARDROW aTable = prTable->table[i];
            if (aTable.dwForwardType != MIB_IPROUTE_TYPE_INDIRECT) //MIB_IPROUTE_TYPE_INDIRECT=a remote destination(공유기 자체)
				continue;
            if (aTable.dwForwardIfIndex == iLineIF) //유선
            {
                iLineMetric = aTable.dwForwardMetric1;
                iprowLine = aTable;
                piprowLine = prTable->table + i;
            } // if
            if (aTable.dwForwardIfIndex == iWirelessIF) //무선
            {
				iWirelessMetric = aTable.dwForwardMetric1;
                iprowWireless = aTable;
				piprowWireless = prTable->table + i;
            } // if
		} // for i

		if (  piprowLine == NULL ||  piprowWireless == NULL ) {
			free(prTable);
			return;
		}

		DWORD dwStatus;

		if ( bWire2Wireless ) {	// Wired => Wireless first
			piprowLine->dwForwardMetric1 = iWirelessMetric+1;
			dwStatus = SetIpForwardEntry(piprowLine);

			//piprowWireless->dwForwardMetric1 = 10;
			//dwStatus = SetIpForwardEntry(piprowWireless);
		} else {				// Wireless => Wired first
			piprowLine->dwForwardMetric1 = iWirelessMetric-1;
			dwStatus = SetIpForwardEntry(piprowLine);

			//piprowWireless->dwForwardMetric1 = 40;
			//dwStatus = SetIpForwardEntry(piprowWireless);
		}
	} // if

	if (prTable != NULL) free(prTable);
}

//---------------------------------------------------------------------------
TGuardnetStatus _GetRoutingTableInfo(int iLineIF, int iWirelessIF)
{
	TGuardnetStatus gsResult;
	gsResult.cPrimary = 0x0;
	gsResult.bLineEnabled = false;
	gsResult.bWirelessEnabled = false;
	gsResult.iLineMetric = 10000;
	gsResult.iWirelessMetric = 10000;

	MIB_IPFORWARDTABLE *prTable = _GetForwardTable();

	if (prTable != NULL)
    {
        for (int i = 0; i < prTable->dwNumEntries; i++)
		{
			MIB_IPFORWARDROW aTable = prTable->table[i];
            if (aTable.dwForwardType != MIB_IPROUTE_TYPE_INDIRECT) //MIB_IPROUTE_TYPE_INDIRECT=a remote destination(공유기 자체)
                continue;
            if (aTable.dwForwardIfIndex == iLineIF) //유선
			{
                gsResult.bLineEnabled = true;
                gsResult.iLineMetric = aTable.dwForwardMetric1;
            } // if
            if (aTable.dwForwardIfIndex == iWirelessIF) //무선
            {
                gsResult.bWirelessEnabled = true;
                gsResult.iWirelessMetric = aTable.dwForwardMetric1;
            } // if
		} // for i
//        gsResult.cPrimary = (gsResult.iLineMetric == gsResult.iWirelessMetric) ? 0x0 :
//                            (gsResult.iLineMetric < gsResult.iWirelessMetric) ? 0x1 :
//                            0x2; //metric(connection cost to the internet) 값이 작은게 최우선임
		if ( !gsResult.bLineEnabled && !gsResult.bWirelessEnabled ) {
			free(prTable);
			return gsResult;	// 2015.05.19 양쪽 Metric 정보가 맞지 않는 상태 => 아무 짓도 하지 않는다.
		} else {
			gsResult.cPrimary = (gsResult.iLineMetric <= gsResult.iWirelessMetric) ? 0x1 : 0x2; //metric(connection cost to the internet) 값이 작은게 최우선임
		}
	} // if

//    //유/무선 상태 화면 표시
//	switch(gsResult.cPrimary)
//    {
//        case lkLine:
//			frmMain->fLineStatus = gniLine;
//			break;
//		case lkWireless:
//			frmMain->fLineStatus = gniWireless;
//			break;
//	} // switch
//
//	Synchronize(SetLineIcon);

	if (prTable != NULL) free(prTable);

    return gsResult;
}









//---------------------------------------------------------------------------
// RQ-30d의 각 Serial Port당 할당된 RQ-30/RG-30 Device Address를 합쳐 놓은 상태: (ex) 5,4,3,2,1 (ex) 6,7,8,9,10
// DeviceAddress = "5,4,3,2,1" || DeviceAddress = "6,7,8,9,10"
int HowManyDeviceAddress(UnicodeString DeviceAddress)
{
	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = DeviceAddress;

	if ( pStrList->Count < 0 ) {
		delete pStrList;	// release
		return 0;			// Address not exist
	}

	int Count = pStrList->Count;   // (ex) 5,4,3,2,1 => return 5
	delete pStrList;

	return Count;
}

//---------------------------------------------------------------------------
// DeviceAddress 배열에 좌안부터의 Address를 순서대로 배치한다.
int SetDeviceAddressOrder(int DeviceAddress[], UnicodeString MasterAddress, UnicodeString SlaveAddress)
{
	int Index = 0;

	// Master Port Device Address ------------------------------------------

	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->StrictDelimiter = true;
	pStrList->DelimitedText = MasterAddress;

	if ( pStrList->Count < 0 ) {
		delete pStrList;	// release
		return 0;			// Address not exist
	}

	UnicodeString AddressStr = "";
	for ( int i = 0 ; i < pStrList->Count ; i++ ) {
		AddressStr = pStrList->Strings[i];
		try {
			DeviceAddress[Index++] = StrToInt(AddressStr);
		} catch ( Exception &e ) {
			DeviceAddress[Index++] = 0;	// Address => 0 => error address
		}
	}
	delete pStrList;

	// Slave Port Device Address ------------------------------------------

	if ( !SlaveAddress.IsEmpty() ) {
		TStringList *pStrList = new TStringList();

		pStrList->Delimiter = ',';
		pStrList->StrictDelimiter = true;
		pStrList->DelimitedText = SlaveAddress;

		if ( pStrList->Count < 0 ) {
			delete pStrList;	// release
			return Index;			// Address not exist
		}

		UnicodeString AddressStr = "";
		for ( int i = 0 ; i < pStrList->Count ; i++ ) {
			AddressStr = pStrList->Strings[i];
			try {
				DeviceAddress[Index++] = StrToInt(AddressStr);
			} catch ( Exception &e ) {
				DeviceAddress[Index++] = 0;	// Address => 0 => error address
			}
		}
		delete pStrList;
	}
	return Index;	// Total Address Count
}

//---------------------------------------------------------------------------
// 매 분마다 Garbage Collection 루틴을 수행한다.
void MemoryGarbageCollection()		// Check Garbage Collection
{
    HANDLE LHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (LHandle != NULL)
	{
        // SetProcessWorkingSetSize to -1 means trim as much as possible
        SetProcessWorkingSetSize(LHandle, (SIZE_T)-1, (SIZE_T)-1);
        CloseHandle(LHandle);
	}
}


