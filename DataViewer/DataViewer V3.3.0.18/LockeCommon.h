//---------------------------------------------------------------------------
#include <vcl.h>
#include <Vcl.DBGrids.hpp>

#ifndef LockeCommonH
#define LockeCommonH
//---------------------------------------------------------------------------

#include <Data.SqlExpr.hpp>

#include "DvEnv.h"

union TszDWord
{
	char szData[4];
	int iValue;
};

#pragma pack(1)

struct TSender2ViewerPacket
{
	char  cStx;
	char  cType;
	char  cLen;
	//data
	char  cFTPStatus;
	char  cGuardNetStatus;
	DWORD iTemperature;
	DWORD iHumidity;
	DWORD iBattery;
	DWORD iCharge;
	//reserved
	char  cReserved1;
    char  cReserved2;
};

#pragma pack()

//문자열관련
UnicodeString GetInt2String(int iValue); //integer 4바이트의 UnicodeString 리턴
void ConvertWCtoC(wchar_t* str, char *&pStr, int &strSize); // wchar* -> char* 변환함수
UnicodeString GetDecimalStr(UnicodeString sValue); //수치만 가져온다
int GetSepItemCount(UnicodeString sText, UnicodeString sFilter); //식별문자를 통한 아이템의 개수 리턴
UnicodeString GetSepItem(UnicodeString sText, int idx, UnicodeString sFilter); //식별문자를 기준으로 한 인덱스의 문자열 리턴
UnicodeString ExtractFtpFilePath(UnicodeString sAddress);
UnicodeString ExtractFtpFileName(UnicodeString sAddress);
UnicodeString ToFloatDefStr(UnicodeString sFormat, UnicodeString sValue, float fDefault);
UnicodeString TextToHex(UnicodeString sText, UnicodeString sDelimiter);
UnicodeString TextToHex(char *sText, int iSize, UnicodeString sDelimiter);
UnicodeString GetMD5String(UnicodeString sText); //md5로 인코딩된 16진수 문자열 리턴

//패킷관련
//UnicodeString SockPacketValid(UnicodeString sText); //sText: ETX를 제외한 ~ CRC까지의 값으로 체크
UnicodeString SockPacketValid(char *Buffer, int iSize); //sText: STX ~ ETX까지의 패킷데이터; CRC값으로 체크
UnicodeString EncodeSockPacket(char cType, UnicodeString sData); //소켓 전송을 위한 패킷 생성
char *EncodeSockPacket(char cType, char *sData, int iLen);
char GetPacketCrc(char *sText, int iLen); //패킷crc를 리턴
char GetPacketCrc(UnicodeString sText); //패킷crc를 리턴

//db관련
TSQLQuery* CreateQuery(TSQLConnection *DbConnection);
bool SQLExists(TSQLConnection *DbConnection, UnicodeString sSQL);

//파일관련
void WriteFileLog(UnicodeString sPathFile, UnicodeString sDate, UnicodeString sText);
UnicodeString PathStrAdd(UnicodeString sSourcePath, UnicodeString sSubPath);
UnicodeString ReadIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sDefault);
void WriteIni(UnicodeString sIniFileName, UnicodeString sSection, UnicodeString sKey, UnicodeString sValue);
UnicodeString GetAppVersion(); //파일버전
UnicodeString GetAppModifiedDateStr(); //실행파일 수정일자

//image관련
void SetBitmapImageListIndex(TImage *image, TImageList *il, int idx);

//기타
UnicodeString getHDDSerial();
void ForcePCReboot(); //pc재부팅
void RegisterRun(UnicodeString sName, UnicodeString sValue);
AnsiString Domain2IP(AnsiString sDomain);
AnsiString IcmpPing(AnsiString sIP);
UnicodeString GetIfInfo(UnicodeString sError);
void SetDBGridColumnWidth(TDBGrid *DBGrid, int iStartIndex, int iCount, int iCharLen = 7); //인덱스 n번부터 끝까지 iWidth로 width 세팅

//heaven2
TDateTime AdjustDateTime(TDateTime DateTime);   	// 12:23:42 => 12:20:00
TDateTime AdjustDateTime(TDateTime DateTime, EN_QUERY_TYPE enQueryType);   	// 12:23:42 => 12:20:00
void GetDurationTime(TDateTime QueryTime, EN_QUERY_TYPE enQueryType, TDateTime &FromTime, TDateTime &ToTime); // Query 시간 기준으로 일/주/월의 시작~종료시간을 구한다.

UnicodeString HFloatToStr(float fValue, int iDigit);   	//1234.1234 => ( 1234.1234, 2) => 1234.12로 Return
UnicodeString HDoubleToStr(double dValue, int iDigit);	//1234.1234 => ( 1234.1234, 2) => 1234.12로 Return

TDateTime GetQueryTime(EN_LOGGER_STATUS enLoggerStatus);

UnicodeString GetBeforeHourStr(TDateTime Time);	// 22:45:09 => '000000','010000','020000',.....'220000' ( Use Query Where Statement )
UnicodeString GetAfterHourStr(TDateTime Time);	// 09:45:09 => '100000','110000','120000','130000',...,'230000' ( Use Query Where Statement )
UnicodeString GetFullDayHourStr();				// '000000','010000','020000',......'230000' ( Use Query Where Statement )

// RQ-30d의 각 Serial Port당 할당된 RQ-30/RG-30 Device Address를 합쳐 놓은 상태: (ex) 5,4,3,2,1 (ex) 6,7,8,9,10
int HowManyDeviceAddress(UnicodeString DeviceAddress);

#endif
