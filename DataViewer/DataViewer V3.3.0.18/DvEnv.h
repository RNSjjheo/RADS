#ifndef __DV_DBENV.H__
#define __DV_DBENV.H__

// Protocol Message Type

#define STX					 0xFA
#define ETX					 0xF5
#define FRAME_OK	   			0
#define FRAME_STX_ERROR	   	   -1
#define FRAME_ETX_ERROR	   	   -2
#define FRAME_CRC_ERROR	   	   -3

#define MSG_TYPE_LoggerActionEvent	   0x01
#define MSG_TYPE_LoggerSetupChange     0x81
#define MSG_TYPE_SenderActionEvent     0x02
#define MSG_TYPE_SenderSetupChange     0x82
#define MSG_TYPE_LoggerUIOpen		   0x83
#define MSG_TYPE_SenderUIOpen		   0x84
#define MSG_TYPE_HeartbeatSignal       0x90

#define ACTION_HEARTBEAT			   0x00
#define ACTION_SHUTDOWN_REQUEST		   0xFF

#define BUFFER_8K			8192
#define BUFFER_4K			4096
#define BUFFER_2K			2048
#define BUFFER_1K			1024

//heaven2 2015.11.18
#define NONE_STR			"NONE"
#define LT500_STR			"LT500"
#define LT400_STR			"LT400"
#define AT200_STR			"AT200"
#define OTTCBS_STR          "OTTCBS"
#define RNSWATER_STR        "RNSWATER"   	// 2024.07.25
#define CHANNELMASTER_STR	"ChannelMaster"
#define AGSERIES_STR		"AG Series"
#define SLSERIES_STR		"SL Series"
#define ACCUSONIC_A_STR		"AccusonicA"
#define ACCUSONIC_B_STR		"AccusonicB"
#define RQ30_STR            "RQ30"
#define RQ30D_STR			"RQ30D"			// heaven2 2022.12.06
#define EWSVCONVERTER_STR	"EWSVConverter"	// heaven2 2026.01.22

#define HYDROMETER_STR      "HydroMeter"	// 수위데이터 사용 구분에 사용
#define WATERLEVEL_STR		"WaterLevel"    // 수위데이터 사용 구분에 사용

const UnicodeString CsNone          = "NONE";
const UnicodeString CsLT500         = "LT500";
const UnicodeString CsLT400         = "LT400";
const UnicodeString CsAT200         = "AT200";
const UnicodeString CsOTTCBS        = "OTTCBS";
const UnicodeString CsRnsWater      = "RNSWATER";
const UnicodeString CsChannelMaster = "ChannelMaster";
const UnicodeString CsSLSeries      = "SL Series";
const UnicodeString CsAGSeries      = "AG Series";
const UnicodeString CsAccusonicA    = "AccusonicA";
const UnicodeString CsAccusonicB    = "AccusonicB";
const UnicodeString CsRQ30          = "RQ30";
const UnicodeString CsRQ30D         = "RQ30D";
const UnicodeString CsEWSVConverter = "EWSVConverter";

const UnicodeString CsDefaultEmailServerAddress = "mail.rnsea.com";
const UnicodeString CsDefaultEmailAccount       = "rads@rnsea.com";
const UnicodeString CsDefaultEmailPassword      = "rnsea2014!!";
const UnicodeString CsDefaultEmailDest1         = "rads@rnsea.com";

const bool CbFileWrite = true;
const bool CbDBWrite   = true;

const int CcLoggerSetupChange = 0x81;
const int CcSenderSetupChange = 0x82;
const int CiPacketFixedSize   = 5;

// heaven2 Image Index
enum EN_IMAGEINDEX { enLoggerOffIndex = 10,
					 enLoggerOnIndex  = 11,
					 enSenderOffIndex = 12,
					 enSenderOnIndex  = 13
				   };

enum TFtpStatus { fsIdle          = 0x00,
				  fsServerStart   = 0x01,
				  fsServerWorking = 0x02,
				  fsServerFinish  = 0x03,
				  fsRnseaStart    = 0x10,
				  fsRnseaWorking  = 0x20,
				  fsRnseaFinish   = 0x30
				};

enum TConfigItems { ciPoint        =   1, //지점정보
					ciLogger       =   2,
					ciSender       =   4,
					ciGuardnet     =   8,
					ciViewer       =  16,
					ciRange        =  32,
					ciStreamMethod =  64,
					ciEtc          = 128
				  };

enum EN_LOGGER_STATUS { enLoggerIdle    = 0x00,
						enLoggerStart   = 0x01,
						enLoggerLogging = 0x02,
						enLoggerFinish  = 0x03
					  };

enum EN_SENSOR_STATUS { enSensorNone    = 0x00,
						enSensorIdle    = 0x01,
						enSensorStart   = 0x02,
						enSensorWorking = 0x03,
						enSensorFinish  = 0x04
					  };

enum EN_SENDER_STATUS { enSenderIdle         = 0,
						enSenderStart        = 1,
						enSenderWorking      = 2,
						enSenderFinish       = 3,
						enSenderRnseaStart   = 4,
						enSenderRnseaWorking = 5,
						enSenderRnseaFinish  = 6
					  };

enum EN_QUERY_TYPE { enQueryMinute =  0,
					 enQueryDay    =  1,
					 enQueryWeek   =  7,
					 enQueryMonth  = 31 };

enum EN_HYDRO_NO { enHydroFirst, enHydroSecond, enHydroThird };


// 일/주/월 데이터 조호 시 조회 건수
#define DAY_QUERY_CNT	144
#define WEEK_QUERY_CNT	168
#define MONTH_QUERY_CNT  31


struct TDataViewerDBEnv
{
	bool bSuperID; //관리자계정으로 로그인 한 것인지 유무

	//db접속정보
	UnicodeString sDatabaseIP;
	UnicodeString sDatabaseName;
	UnicodeString sDatabaseAccount;
	UnicodeString sLoggedUserID;
	UnicodeString sDataSenderIP;
	int iDataSenderPort;
	UnicodeString sDataLoggerIP;
	int iDataLoggerPort;

	//지점정보
	UnicodeString sPointCode;    //지점코드
	UnicodeString sPointName;    //지점명
    UnicodeString sPointAddress; //지점 주소
    UnicodeString sPointZipCode; //지점 우편번호
    UnicodeString sPointManager; //지점 담당자
	UnicodeString sPointTel;     //담당자 연락처
	UnicodeString sPointEtc;     //특이사항

    //공통사항
	UnicodeString sPubRadsHome;   //Rads Home 기본폴더
    UnicodeString sPubLoggerHome; //logger home
    UnicodeString sPubSenderHome; //sender home
	UnicodeString sPubViewerHome; //viewer home

    //운영정보
    UnicodeString sRStreamMethod; //유량산출방식
	int           iMSL;           //해발표고
    UnicodeString sCctvUrl;
	int           iLoginDelayMinute;

	//1/2/3 유속계 정보
	UnicodeString sHD1Kind;
	int           iAG1_WN;   //유속계1; 하층유속계
	bool          bReverse1; //하층유속계 리버스 옵션
	UnicodeString sHD2Kind;
	int           iAG2_WN;   //유속계2; 상층유속계
	bool          bReverse2; //상층유속계 리버스 옵션
	UnicodeString sHD3Kind;
	int           iAG3_WN;   //유속계2; 상층유속계
	bool          bReverse3; //상층유속계 리버스 옵션

    //수위계 정보
	UnicodeString sWaterLevelDeviceKind; //수위계 종류
	float         fWaterLevelOffset;	 // 수위계 Offset heaven2
	UnicodeString sWaterUseFlag;	// HydroMeter / WaterLevel => 수위 데이터를 어디서 쓸거냐?

    //화면 graph 옵션
	bool          bMDIWaterFlow;   //유량산출
	bool          bMDIWaterLevel;  //수위계
	bool          bMDIHydroMeter1; //하층유속계
	bool          bMDILog;         //로그창
	bool          bMDIVth;         //vth logger

    //email 정보
	bool          bEmailAutoSend;      //자동전송 유무
    UnicodeString sEmailServerAddress; //smtp 메일서버 주소
    UnicodeString sEmailAccount;       //id
    UnicodeString sEmailPassword;      //pw
    UnicodeString sEmailDest1;         //수신자1 -> 자신 rads@rnsea.com
	UnicodeString sEmailDest2;         //수신자2
    UnicodeString sEmailDest3;         //수신자3
    UnicodeString sEmailDest4;         //수신자4
	UnicodeString sEmailDest5;         //수신자5

	// 유효범위(heaven2)
	float         fHydroLowerBound;
	float         fHydroUpperBound;
	float         fWaterLowerBound;
	float         fWaterUpperBound;
};
#endif
