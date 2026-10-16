//---------------------------------------------------------------------------
#ifndef LockeCommonH
#define LockeCommonH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <Data.SqlExpr.hpp>
#include <ipmib.h>		// 이 library를 사용할 것
//#include <Iprtrmib.h>	// MSDN은 이것이지만 ipmib.h를 사용할 것 .. 이 해더를 쓰면 AsyncPro와 Conflict

#include "Env.h"

union TszDWord
{
	char szData[4];
	int iValue;
};

struct TSender2ViewerPacket
{
    char cStx;
    char cType;
    char cLen;
    //data
    char cFTPStatus;
    char cGuardNetStatus;
    int iCharge;
    int iBattery;
    int iTemperature;
    int iHumidity;
    //reserved
    char cReserved1;
    char cReserved2;
};

UnicodeString VersionInfo(const UnicodeString &sQuery);

//문자열관련
UnicodeString GetInt2String(int iValue); 										//integer 4바이트의 UnicodeString 리턴
UnicodeString GetDecimalStr(UnicodeString sValue); 								//수치만 가져온다
int           GetSepItemCount(UnicodeString sText, UnicodeString sFilter); 		//식별문자를 통한 아이템의 개수 리턴
UnicodeString GetSepItem(UnicodeString sText, int idx, UnicodeString sFilter); 	//식별문자를 기준으로 한 인덱스의 문자열 리턴
UnicodeString ExtractFtpFilePath(UnicodeString sAddress);
UnicodeString ExtractFtpFileName(UnicodeString sAddress);
UnicodeString ToFloatDefStr(UnicodeString sFormat, UnicodeString sValue, float fDefault);
UnicodeString TextToHex(UnicodeString sText, UnicodeString sDelimiter);
UnicodeString TextToHex(char *sText, int iSize, UnicodeString sDelimiter);
//UnicodeString GetMD5String(UnicodeString sText); 								//md5로 인코딩된 16진수 문자열 리턴

//패킷관련
//UnicodeString SockPacketValid(UnicodeString sText); 							//sText: ETX를 제외한 ~ CRC까지의 값으로 체크
UnicodeString SockPacketValid(char *Buffer, int iSize); 						//sText: STX ~ ETX까지의 패킷데이터; CRC값으로 체크
//UnicodeString EncodeSockPacket(char cType, UnicodeString sData); 				//소켓 전송을 위한 패킷 생성
char         *EncodeSockPacket(char cType, char *sData, int iLen);
char          GetPacketCrc(char *sText, int iLen); 								//패킷crc를 리턴
char          GetPacketCrc(UnicodeString sText); 								//패킷crc를 리턴

//db관련
TSQLQuery    *CreateQuery(TSQLConnection *DbConnection);
bool          SQLExists(TSQLConnection *DbConnection, UnicodeString sSQL);
UnicodeString GetDatabaseDateTime(TSQLConnection *DbConnection, TDateTime &DBDateTime);

//파일관련
void          WriteFileLog(UnicodeString sPathFile, UnicodeString sDate, UnicodeString sText);
UnicodeString PathStrAdd(UnicodeString sSourcePath, UnicodeString sSubPath);
UnicodeString ReadIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sDefault);
void          WriteIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sValue);
UnicodeString GetAppVersion(); 				//파일버전
UnicodeString GetAppModifiedDateStr(); 		//실행파일 수정일자

//image관련
void          SetBitmapImageListIndex(TImage *image, TImageList *il, int idx);

//기타
UnicodeString getHDDSerial();
void          ForcePCReboot(); 										//pc재부팅
void          RegisterRun(UnicodeString sName, UnicodeString sValue);
AnsiString    Domain2IP(AnsiString sDomain);
AnsiString    IcmpPing(AnsiString sDestineIP, AnsiString sSourceIP);
bool          IsWindowsXP();
UnicodeString GetIPByIF(int IF);
int           GetIFByDescription(UnicodeString Description);
UnicodeString GetIfInfo(UnicodeString sError);
TDateTime     GetStrToDateTime(UnicodeString sDate, UnicodeString sTime);
SIZE_T        GetProcessWorkingSetSize();

void          TrayIconRefresh();	//2015.10.02

//---------------------------------------------------------------------------

// Heaven2
BYTE          MakeCRC(BYTE *pBuff, int iLen);
int           CheckFrame(BYTE *pBuff, int iLen);

// 지정한 시간만큼 기다린다. ( 그동안 message도 처리한다. )
void          HWait(int iMiliSecond);
BOOL          SleepMsg(DWORD dwTimeout);

MIB_IPFORWARDTABLE *_GetForwardTable();
void            _ChangeRoutingTable(int iLineIF, int iWirelessIF, bool bWire2Wireless=true);
TGuardnetStatus _GetRoutingTableInfo(int iLineIF, int iWirelessIF);


///////////////////////////////////////////////////////////////////////////
// Thread간 통신하는 객체 정의 2015.02.05
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
struct STLogMessage
{
	bool          bFileWrite;
	bool          bDBWrite;
	bool          bLogMessage; 		// true: Log, false: Guardnet Log
	UnicodeString sDate;			// format : yyyy-mm-dd hh:nn:ss.zzz
	UnicodeString sText;
};

// FTP로 전송할 때 Queue에 전송하는 파일의 타입이 어떤 것인 지 체크
// ADVM+EWSV 운영 사이트는 esv 파일을 먼저 보내고, adv 파일을 나중에 보냄
// 따라서 esv 파일 전송 결과를 setup 할 때 adv 파일 결과는 reset해야 함.
enum EN_FILE_TYPE { enAdvFile = 0x01,
					enUvmFile = 0x02,
					enEsvFile = 0x03 };

//////////////////////////////////////////////////////////////////////////
struct STFTPMessage
{
	bool          bReal; 			//real인지 유무
	bool          bEsvAdvFlag;		// EWSV+ADVM 같이 운용하는 사이트  인 지 Flag 2026.04.28
	EN_FILE_TYPE  enFileType;		// enEsvFile, enAdvFile
	UnicodeString sMeasureDate;		// yyyymmdd
	UnicodeString sMeasureTime;		// hhnnss
	UnicodeString sLocalPathFile; 	//local 폴더/파일명
	UnicodeString sFTPRootPathFile; //ftp연결시 대상 폴더/파일명 ( 수신서버1: IRDIMS Server ) => 구 수집 서버
	UnicodeString sFTPRootPathFile2;	// 2024.11.29 TargetServer FileName ( 수신서버2: Rnsea Server ) => 신규 수집 서버
};


//////////////////////////////////////////////////////////////////////////
struct STGuardnetMessage
{
	UnicodeString sText;
};

//////////////////////////////////////////////////////////////////////////
int HowManyDeviceAddress(UnicodeString DeviceAddress);
int SetDeviceAddressOrder(int DeviceAddress[], UnicodeString MasterAddress, UnicodeString SlaveAddress);
void MemoryGarbageCollection();		// Check Garbage Collection

#endif
