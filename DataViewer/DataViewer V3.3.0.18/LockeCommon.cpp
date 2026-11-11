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
#include "LockeCommon.h"
#include "IdHashMessageDigest.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

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
void ConvertWCtoC(wchar_t* str, char *&pStr, int &strSize) // wchar* -> char* 변환함수
{
	//입력받은 wchar_t 변수의 길이를 구함
	strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* 메모리 할당
	pStr = new char[strSize];
	//형 변환
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	strSize -= 1;
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
UnicodeString EncodeSockPacket(char cType, UnicodeString sData)
//type, sData를 파라미터로 소켓 전송 패킷스트링을 생성
{
	UnicodeString sPacket = "";
	sPacket += (char)0xfa; //stx
	sPacket += (char)cType; //type
	int iLen = sData.Length();
	sPacket += (char)iLen; //len
	sPacket += sData; //data
	sPacket += (char)GetPacketCrc(sPacket); //crc
	sPacket += (char)0xf5; //etx
	return sPacket;
}
//---------------------------------------------------------------------------
char *EncodeSockPacket(char cType, char *sData, int iLen)
{
	char *sResult = new char[iLen + 5];

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
    bool bResult = True;
    TSQLQuery *sql = CreateQuery(DbConnection);
    sql->SQL->Text = sSQL;
    sql->Open();
    bResult = !(sql->Bof && sql->Eof);
    delete sql;
	return bResult;
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

	return sResult;
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
        if (hFile != -1) FileClose(hFile);
    } // catch
}
//---------------------------------------------------------------------------
UnicodeString GetMD5String(UnicodeString sText)
//md5로 인코딩된 16진수 문자열 리턴
{
    AnsiString sResult = "";
    TIdHashMessageDigest5 *idmd5;

    idmd5 = new TIdHashMessageDigest5;
    try
    {
        sResult = idmd5->HashStringAsHex(sText, enUTF8);
    } __finally
    {
        delete idmd5;
    } // finally
    return sResult;
}
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

	AdjustTokenPrivileges(hToken, False, &tp, 0, NULL, NULL);

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
    reg->OpenKey("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", False);
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
    int i = 0;

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
AnsiString IcmpPing(AnsiString sIP)
{
    AnsiString sResult = "";

    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    ipaddr = inet_addr(sIP.c_str());
    if (ipaddr == INADDR_NONE) {
        sResult = "not proper ip: " + sIP;
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

    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
        NULL, ReplyBuffer, ReplySize, 1000);
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
		sResponse += "Received from " + sIP;
        sResponse += " Status = " + IntToStr((int)pEchoReply->Status);
        sResponse += " Roundtrip time = " + IntToStr((int)pEchoReply->RoundTripTime) + " milliseconds";
    }
    else {
        sResult ="IcmpSendEcho returned error: " + IntToStr((int)GetLastError());
    }

    if (hIcmpFile != INVALID_HANDLE_VALUE) IcmpCloseHandle(hIcmpFile);

    WSACleanup();
    return sResult;
}
//---------------------------------------------------------------------------
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

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
    IP_ADAPTER_PREFIX *pPrefix = NULL;

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
void SetDBGridColumnWidth(TDBGrid *DBGrid, int iStartIndex, int iCount, int iCharLen)
//인덱스 n번부터 끝까지 iWidth로 width 세팅
{
	for(int i = iStartIndex; i < DBGrid->Columns->Count; i++) {
		DBGrid->Columns->operator [](i)->Width = iCharLen * iCount;
	}
}

//---------------------------------------------------------------------------
// 현재시간을 데이터 수집용 시간으로 Return한다.(ex) 12:23:42 => 12:20:00
TDateTime AdjustDateTime(TDateTime DateTime)
{
	USHORT hour, minute, sec, msec;
	DateTime.DecodeTime(&hour, &minute, &sec, &msec);

	USHORT m = minute % 10;
	USHORT s = sec;
	USHORT ms = msec;

	return DateTime - EncodeTime(0, m, s, ms);
}

//---------------------------------------------------------------------------
// 현재시간을 데이터 수집용 시간으로 Return한다.(ex) 12:23:42 => 12:20:00
TDateTime AdjustDateTime(TDateTime DateTime, EN_QUERY_TYPE enQueryType)
{
	USHORT year, month, day;
	USHORT hour, minute, sec, msec;
	DateTime.DecodeDate(&year, &month, &day);
	DateTime.DecodeTime(&hour, &minute, &sec, &msec);

	TDateTime NewDateTime;

	if ( enQueryType == enQueryDay ) {

		USHORT m = minute % 10;
		USHORT s = sec;
		USHORT ms = msec;
		NewDateTime = DateTime - EncodeTime(0, m, s, ms);	// 09:23:32 => 09:20:00 ( 09:20은 아직 데이터 확정 안됨 )

	} else if ( enQueryType == enQueryWeek ) {

		NewDateTime = DateTime - EncodeTime(0, minute, sec, msec);	// 09:23:32 => 09:00:00

	} else { // QueryType => Month

		NewDateTime = EncodeDate(year, month, day);

	}

	return NewDateTime;
}

//---------------------------------------------------------------------------
// 현재시간을 기준으로 일/주/월로 시작~종료 시간을 구한다.
void GetDurationTime(TDateTime QueryTime, EN_QUERY_TYPE enQueryType, TDateTime &FromTime, TDateTime &ToTime)
{
	TDateTime OneMinute = EncodeTime(0,10,0,0);
	TDateTime OneHour   = EncodeTime(1,0,0,0);
	TDateTime OneDay    = 1;

	USHORT hour, minute, second, msec;
	USHORT year, month, day;

	if ( enQueryType == enQueryDay ) {
		ToTime   = QueryTime;
		FromTime = ToTime - enQueryDay + OneMinute;
	} else if ( enQueryType == enQueryWeek ) {
		QueryTime.DecodeTime(&hour, &minute, &second, &msec);

		ToTime   = QueryTime - EncodeTime(0, minute, second, msec);     // 09:32:23 => 09:00:00
		FromTime = ToTime - enQueryWeek + OneHour;	// 7day

	} else { // enQueryMonth
		QueryTime.DecodeDate(&year, &month, &day);
		QueryTime.DecodeTime(&hour, &minute, &second, &msec);

		ToTime   = EncodeDate(year, month, day); 	// 2014-10-06 09:32:23 => 2014-10-06 00:00:00
		FromTime = ToTime - enQueryMonth + OneDay;
	}
}

//---------------------------------------------------------------------------
UnicodeString HFloatToStr(float fValue, int iDigit)   	//1234.1234 => ( 1234.1234, 2) => 1234.12로 Return
{
	return UnicodeString::FloatToStrF(fValue, ffFixed, 15, iDigit);
}

//---------------------------------------------------------------------------
UnicodeString HDoubleToStr(double dValue, int iDigit)	//1234.1234 => ( 1234.1234, 2) => 1234.12로 Return
{
	return UnicodeString::FloatToStrF(dValue, ffFixed, 15, iDigit);
}

//---------------------------------------------------------------------------
// Query 할 때 현재 시간을 4분 뺄 것인가를 LoggerStatus에 따라 결정
TDateTime GetQueryTime(EN_LOGGER_STATUS enLoggerStatus)
{
	TDateTime Day;
	if ( enLoggerStatus == enLoggerFinish || enLoggerStatus == enLoggerIdle )
		Day = Now();
	else
		Day = Now() - EncodeTime(0,4,0,0);	// 4분을 뺀 상태로 Query한다. 시간은 되었으나 아직 데이터 수집이 되지 않은 상태를 위함.

	return Day;
}

//---------------------------------------------------------------------------
// 기준 시간 전의 시간 데이터 String을 리턴
UnicodeString GetBeforeHourStr(TDateTime Time)	// 22:45:09 => '000000','010000','020000',.....'220000' ( Use Query Where Statement )
{
	USHORT hour, minute, second, msec;
	Time.DecodeTime(&hour, &minute, &second, &msec);

	UnicodeString HourStr = "";
	UnicodeString Str;

	bool bFirst = true;
	for ( int i = 0 ; i <= hour ; i++ ) {
		Str = Str.sprintf(L"%02d0000",i);
		if ( bFirst ) {
			HourStr += "'" + Str + "'";	// '000000','010000',...,'230000' 을 생성
			bFirst = false;
		} else {
			HourStr += ",'" + Str + "'";	// '000000','010000',...,'230000' 을 생성
		}
	}
	return HourStr;
}
//---------------------------------------------------------------------------
// 기준 시간 이전의 시간 데이터 String을 리턴
UnicodeString GetAfterHourStr(TDateTime Time)	// 09:00:00 => '090000', '100000','110000','120000','130000',...,'230000' ( Use Query Where Statement )
{
	USHORT hour, minute, second, msec;
	Time.DecodeTime(&hour, &minute, &second, &msec);

	UnicodeString HourStr = "";
	UnicodeString Str;

	bool bFirst = true;
	for ( int i = hour ; i <= 23 ; i++ ) {
		Str = Str.sprintf(L"%02d0000",i);
		if ( bFirst ) {
			HourStr += "'" + Str + "'";	// '000000','010000',...,'230000' 을 생성
			bFirst = false;
		} else {
			HourStr += ",'" + Str + "'";	// '000000','010000',...,'230000' 을 생성
		}
	}
	return HourStr;
}
//---------------------------------------------------------------------------
// 하루 전체의 기간 데이터 String을 리턴
UnicodeString GetFullDayHourStr()				// '000000','010000','020000',......'230000' ( Use Query Where Statement )
{
	UnicodeString HourStr = "";
	UnicodeString Str;

	bool bFirst = true;
	for ( int i = 0 ; i <= 23 ; i++ ) {
		Str = Str.sprintf(L"%02d0000",i);
		if ( bFirst ) {
			HourStr += "'" + Str + "'";	// '000000','010000',...,'230000' 을 생성
			bFirst = false;
		} else {
			HourStr += ",'" + Str +"'";	// '000000','010000',...,'230000' 을 생성
		}
	}
	return HourStr;

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

