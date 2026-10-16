#ifndef EnvH
#define EnvH
//---------------------------------------------------------------------------
// heaven2
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


const bool CbFileWrite = True;
const bool CbDBWrite   = True;

const UnicodeString CsNone          = "NONE";				// 장비 없을 때
const UnicodeString CsChannelMaster = "ChannelMaster";
const UnicodeString CsSLSeries      = "SL Series";
const UnicodeString CsAGSeries      = "AG Series";
const UnicodeString CsAccusonicA    = "AccusonicA";
const UnicodeString CsAccusonicB    = "AccusonicB";
const UnicodeString CsRQ30			= "RQ30";			// 2021.12.09 heaven2
const UnicodeString CsRQ30D         = "RQ30D";			// 2022.12.16 heaven2
const UnicodeString CsEWSVConverter         = "EWSVConverter";			// 2026.08.24 jjheo

const char CcDataSenderSetupChange   = 0x82; //DataSenderSetupChange
const int  CiPBBuffertSize           = 4096;
const int  CiSend2ViewPacketDataSize = 20;
const int  CiVthItemCount            = 10;

enum TThreadMessageType { tmtChangeViewerStatus,
						  tmtWriteToVTH,
						  tmtDoReset,
						  FTPIcon,
						  GuardnetIcon,
						  LineIcon,
						  BatteryIcon,
						  LoggerIcon,
						  WatchDogIcon,
						  ViewerIcon,
						  VTHValue
						};

enum TGuardnetPacketStatus { gnpsIdle    = 0,
							 gnpsStart   = 1,
							 gnpsWorking = 2,
							 gnpsFinish  = 3,
							 gnpsError   = 5 };

enum TLogOperationKind { loIdle,		// Logger Operation Mode ( Idle => Start => Logging => Finish )
						 loStart,
						 loLogging,
						 loFinish };

enum TGuardnetImageIndex { gniDisabled,
						   gniIdle,
						   gniWorking,
						   gniOk,
						   gniError,
						   gniReboot,
						   gniLine,
						   gniWireless };

enum TConfigItems { ciPoint    = 1, //지점정보
					ciLogger   = 2,
					ciSender   = 4,
					ciGuardnet = 8,
					ciViewer   = 16,
					ciRange    = 32,
					ciEtc      = 64 };

enum TFtpStatus { fsIdle          = 0x00,
				  fsServerStart   = 0x01,
				  fsServerWorking = 0x02,
				  fsServerFinish  = 0x03,
				  fsRnseaStart    = 0x10,
				  fsRnseaWorking  = 0x20,
				  fsRnseaFinish   = 0x30 };

enum TAppStatus { asLoggerDisabled =  0,
				  asLoggerEnabled  =  1,
				  asWdDisabled     =  2,
				  asWdEnabled      =  3,
				  asViewDisabled   =  4,
				  asViewEnabled    =  5,
				  asViewUser1D     =  6,
				  asViewUser1E     =  7,
				  asViewUser2D     =  8,
				  asViewUser2E     =  9,
				  asViewUser3D     = 10,
				  asViewUser3E     = 11,
				  asViewUser4D     = 12,
				  asViewUser4E     = 13,
				  asViewUser5D     = 14,
				  asViewUser5E     = 15 };

enum TFtpImageType { fipDisabled,
					 fipEnabled,
					 fipSendingFTP1,
					 fipSendingFTP2,
					 fipCompleteFTP1,
					 fipCompleteFTP2 };

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
#define MSG_TYPE_Ping				   0x20		// 2017.09.21 heaven2
#define MSG_TYPE_PingAck               0x21		// 2017.09.21 heaven2
#define MSG_TYPE_RebootReq             0x22		// 2017.09.21 heaven2
#define MSG_TYPE_RebootRes             0x23		// 2017.09.21 heaven2

#define ACTION_HEARTBEAT			   0x00
#define ACTION_SHUTDOWN_REQUEST		   0xFF

#define UM_MESSAGE 		   (WM_APP+1234)	// User Message ( Logger Status )
#define PAST_FILE_CHECK_JOB				1
#define DB_CONNECTION_CHECK				2	// 2022.04.27
#define GARBAGE_COLLECTION				3	// 2026.03.04

#define FORMAT_ADVM_STR					"ADVM"
#define FORMAT_UVM_ACCUSONIC_STR		"UVM(Accusonic)"
#define FORMAT_UVM_HYDROVISION_STR		"UVM(Hydrovision)"
#define FORMAT_EWSV_STR                 "EWSV"
#define FORMAT_EWSV_ADVM_STR            "EWSV+ADVM"

#define FORMAT_ADVM					1
#define FORMAT_UVM_ACCUSONIC		2
#define FORMAT_UVM_HYDROVISION		3
#define FORMAT_EWSV                 4
#define FORMAT_EWSV_ADVM            5

#define DECISION_OK_STR			    "0"		// 정상
#define DECISION_WRONG_STR			"1"		// 오측
#define DECISION_MISSING_STR   		"2"		// 결측

#define ON_STR						"ON"
#define OFF_STR						"OFF"

// 수위 데이터 사용 구분
#define HYDROMETER_STR      "HydroMeter"	// 수위데이터 사용 구분에 사용
#define WATERLEVEL_STR		"WaterLevel"    // 수위데이터 사용 구분에 사용


// ADVM File Header
const UnicodeString CsAdvHeaderText = "St_code,YYYYMMDDhhmm,  Deci_All,  Deci_VTH,        AC, DC_Charge,DC_Battery,  Temp_Sys,    Hr_Sys,   Deci_WL,WaterLevel,WaterDepth, WL_Offset,  Salinity,   No_ADVM, Deci_ADVM,Temp_Water,Depth_ADVM,     Pitch,      Roll,        WN,        WS,        WP,        WF,      DIS1,      DIS2,   No_Cell,      V_EW,      V_NS,        E1,        E2,\r\n";
const UnicodeString CsUvmHeaderText = "St_code,YYYYMMDDhhmm,  Deci_All,  Deci_VTH,        AC, DC_Charge,DC_Battery,  Temp_Sys,    Hr_Sys,   Deci_WL,WaterLevel,WaterDepth, WL_Offset,  Salinity,  Deci_UVM,     acV11,     acV12,     acV13,     acV14,     acV15,     acV16,     acV17,     acV18,        G1,        G2,        G3,        G4,        G5,        G6,        G7,        G8,        S1,        S2,        S3,        S4,        S5,        S6,        S7,        S8,  Deci_UVM,     acV21,     acV22,     acV23,     acV24,     acV25,     acV26,     acV27,     acV28,        G1,        G2,        G3,        G4,        G5,        G6,        G7,        G8,        S1,        S2,        S3,        S4,        S5,        S6,        S7,        S8\r\n";
const UnicodeString CsEsvHeaderText = "St_code,YYYYMMDDhhmm,  Deci_All,  Deci_VTH,        AC, DC_Charge,DC_Battery,  Temp_Sys,    Hr_Sys,   Deci_WL,WaterLevel,WaterDepth, WL_Offset,  Salinity, Deci_Wind,WindDegree,      Wind,  WindTemp,     WindH, WindAtmos, Deci_EWSV, EWSV_Kind,   EWSV_LV,    EWSV_V,   EWSV_LQ,    EWSV_Q,  EWSV_Cnt, EWSV_Flag,   EWSV_No,   EWSV_LV,    EWSV_V,   EWSV_LQ,    EWSV_Q,  EWSV_Deg,  EWSV_Snr,EWSV_Check,\r\n";

const int CiMaxLogMemoLine            = 100;
const unsigned char CcETX             = 0xf5;

const UnicodeString CsDataPath        = "Data";
const UnicodeString CsLogPath         = "Log";
const UnicodeString CsGuardnetLogPath = "GuardNetLog";
const UnicodeString CsVTHSend         = "VTHSend"; 			//vth 시리얼 송신자료 처리
const UnicodeString CsVTHReceive      = "VTHReceive"; 		//vth 시리얼 수신자료 처리
const UnicodeString CsFTPSend         = "FTPSend"; 			//ftp 전송자료 처리
const UnicodeString CsWriteLogDBnFile = "WriteLogDBnFile"; 	//로그텍스트 파일로 저장
const UnicodeString CsSendToViewers   = ""; 				//뷰어에 패킷 전송
const UnicodeString CsGuardnet        = "Guardnet"; 		//가드넷

//---------------------------------------------------------------------------
//가드넷 관련
const UnicodeString CsGNDayPcRebootCnt = "DAY_PC_REBOOT_CNT";
const UnicodeString CsGNPingTest       = "PingTest";
const UnicodeString CsGNChange2W       = "Change2W";
const UnicodeString CsGNChange2L       = "Change2L";

//---------------------------------------------------------------------------
struct TGuardnetStatus
{
	char cPrimary; 			//우선순위가 높은 인터넷; 0x0=없음, 0x1=유선, 0x2=무선
	bool bLineEnabled; 		//유선이 인식되고 연결상태면 true
	bool bWirelessEnabled; 	//무선이 인식되고 연결상태면 true
	int iLineMetric; 		//유선 metric 값
	int iWirelessMetric; 	//무선 metric 값
};

//---------------------------------------------------------------------------
struct TWriteLogToMemo
{
	bool bFileWrite;
	bool bDBWrite;
	bool bLogMessage;
	UnicodeString sDateTime;
	AnsiString sText;
};

//---------------------------------------------------------------------------
struct TIniInfo
{
	UnicodeString sIP;
	UnicodeString sName;
	UnicodeString sAccount;
};
//} IniInfo;

//---------------------------------------------------------------------------
struct TRHydroMeterHeader
{
	float fTemp_Water; //Temp_Water, ->    수온
	float fDepth_ADVM; //Depth_ADVM, ->    수압
	float fPitch;      //     Pitch, ->    피치
	float fRoll;       //      Roll, ->    롤
	int iWN;           //        WN, ->    측정 셀 총개수
	int iWS;           //        WS, ->    측정 셀 크기(cm)
	int iWP;           //        WP, ->    측정 셀 평균개수
	int iWF;           //        WF, ->    공백거리셀
	int iDIS1;         //      DIS1, ->    첫번째 셀 중심거리(cm)
	int iDIS2;         //      DIS2, ->    마지막 셀 중심거리(cm)
};

//---------------------------------------------------------------------------
struct TRHydroMeterUvmHeader
{
	double V1;			// Velocity 1 (m/s)
	double V2; 			// Velocity 1 (m/s)
	double V3;			// Velocity 1 (m/s)
	double V4;			// Velocity 1 (m/s)
	double V5;			// Velocity 1 (m/s)
	double V6;			// Velocity 1 (m/s)
	double V7;			// Velocity 1 (m/s)
	double V8;			// Velocity 1 (m/s)
	int    Gn1;			// Gain Path 1
	int    Gn2;			// Gain Path 2
	int    Gn3;			// Gain Path 3
	int    Gn4;			// Gain Path 4
	int    Gn5;			// Gain Path 5
	int    Gn6;			// Gain Path 6
	int    Gn7;			// Gain Path 7
	int    Gn8;			// Gain Path 8
	int    Sn1;			// Velocity SND Section 1
	int    Sn2;			// Velocity SND Section 2
	int    Sn3;			// Velocity SND Section 3
	int    Sn4;			// Velocity SND Section 4
	int    Sn5;			// Velocity SND Section 5
	int    Sn6;			// Velocity SND Section 6
	int    Sn7;			// Velocity SND Section 7
	int    Sn8;			// Velocity SND Section 8
};

//---------------------------------------------------------------------------
struct TDBEnvInfo
{
	int           iPCRebootCnt; 	//PC리부팅 카운트

	//지점정보
	UnicodeString sPointCode; 		//지점코드
	UnicodeString sPointName; 		//지점명
	UnicodeString sPointAddress; 	//지점 주소
	UnicodeString sPointZipCode; 	//지점 우편번호
	UnicodeString sPointManager; 	//지점 담당자
	UnicodeString sPointTel; 		//담당자 연락처
	UnicodeString sPointEtc; 		//특이사항

	//공통사항
	UnicodeString sPubRadsHome; 	//Rads Home 기본폴더
	UnicodeString sPubLoggerHome; 	//logger home
	UnicodeString sPubSenderHome; 	//sender home
	UnicodeString sPubViewerHome; 	//viewer home
	UnicodeString sDataSenderIP;
	int           iDataLoggerPort;
	int           iDataViewerPort;
	UnicodeString sWatchDogIP;
	int           iWatchDogPort;

	//Logger 정보: 동작 시간 설정
	UnicodeString sLGInterval; 		//측정주기(nn:ss)
	UnicodeString sLGWorkingPeriod; //WorkingPeriod: 동작주기(08:30초 동안 동작하여 데이터 수집)
	int           iLGStartTime; 	//시작시간(매5,15,25,35,45,55분에 시작하여 10분동안 자료 수집함

	//DataSender 정보: 수집서버
	bool          bServerEnable; 	//수집서버 전송 Flag
	UnicodeString sServerDomain; 	//수집서버 Domain/IP
	int           iServerPort; 		//수집서버 Port
	UnicodeString sServerAccount; 	//서버 id
	UnicodeString sServerPassword; 	//서버 pw
	UnicodeString sServerRealPath; 	//서버 실시간 저장폴더
	UnicodeString sServerPastPath; 	//서버 과거 저장폴더
	bool 		  bRnseaEnable; 	//rnsea ftp 서버 전송 유무
	UnicodeString sRnseaDomain; 	//rnsea ftp 서버 도메인/ip
	int           iRnseaPort; 		//rnsea ftp 서버 Port
	UnicodeString sRnseaAccount; 	//rnsea ftp 서버 id
	UnicodeString sRnseaPassword; 	//rnsea ftp 서버 pw
	UnicodeString sRnseaRealPath; 	//rnsea ftp 서버 실시간 저장폴더
	UnicodeString sRnseaPastPath; 	//rnsea ftp 서버 과거 저장폴더
	UnicodeString sFtpFileFormat; 	//ftp file format(ADVM/UVM/UVM(Hydrovision))
	UnicodeString sFTPMode;			//Active/Passive Mode
	UnicodeString sRnseaFTPMode;	//Active/Passive Mode
	UnicodeString sFTPType;			//FTP/SFTP
	UnicodeString sPrivateKey;		//PrivateKey(privatekey.ras) Location : $RADS_HOME(default)
	UnicodeString sFTPType2;		//FTP/SFTP2
	UnicodeString sPrivateKey2;		//PrivateKey(privatekey.ras) Location : $RADS_HOME(default)2

	//DataSender 정보: 동작 변수, VTHLogger 정보
	int           iRetryCount; 		//FTP 전송 실패 시 재시도 횟수
	bool          bRebootFlag; 		//FTP 실패시 Gateway/PC Reboot할 것인 지에 대한 옵션
	int           iRebootRetryCount; //FTP 전송 연속으로 몇 번 실패할 때 Gateway Reboot/PC Reboot할 것인 지 횟수
	int           iMaxSendFile; 	//한번 연결해서 최대 보낼 수 있는 파일 개수
	int           iComNumber; 		//VTHLogger Comport(COM1,COM2,…COM32)
	int           iBaud; 			//VTHLogger Serial 통신속도(9600,14400,19200,….38400) 9600으로 고정됨
	UnicodeString sACOpt;			//AC Option: BATTERY / AIN

	//DataSender 정보: GuardNet 동작 변수
	bool          bGNEnable; 			//가드넷 사용유무
	char          cGNType; 				//0x0=normal(특정시간에 pc부팅), 0x1=네트웍 상황에 따라 pc부팅
	UnicodeString sGNRebootTime; 		//normal일때 pc부팅시간
	int           iGNPingInterval; 		//핑 주기(초)
	UnicodeString sGNPingStartTime; 	//ping을 시작할 시간(=current면 프로그램 시간한 시간부터/0: 매분(0초 마다))
	UnicodeString sGNPingServer1; 		//ping server 1
	int           iGNPingServer1Port; 	//ping server 1 port
	bool          bGNPingServer2TestOn; //ping2 테스트 유무
	UnicodeString sGNPingServer2; 		//ping server 2
	int           iGNPingServer2Port; 	//ping server 2 port
	int           iGNRetryCount; 		//ping실패시 재시도 횟수
	int           iGNPingRetryCount; 	//1분당 시도 시 재시도 횟수(연속실패횟수)
	int           iGNWirelessUseMinute; //무선 사용 시간; 무선을 전환시 최대운영 시간(Expire되면 유선으로 다시 전환하여 다시 시도)
	int           iGNGatewayRebootMaxCount; //이 숫자를 초과하면 PC Reboot with VTH Reset command
	int           iGNRebootMaxCount; 	//1일 PC 최대 Reboot 할 수 있는 횟수(1,2,3,4,5,6,7,8,9,10)
	int           iGNNetworkWiredIF; 		//NetworkWiredIF : Network Interface No
	UnicodeString sGNNetworkWiredIFDesc; 	//NetworkWiredIF : Network Interface No	Description
	int           iGNNetworkWirelessIF; 	//NetworkWirelessIF : Network Interface No
	UnicodeString sGNNetworkWirelessIFDesc; //NetworkWirelessIF : Network Interface No Description
	int           iGNDelayMinuteAfterGWReset; //g/w reset후 대기시간 분

	//Logger 정보: 수위계 정보
	UnicodeString sWaterKind;		// 수위계 종류
	UnicodeString sAirmarEnable;	// 수위계에서 Airmar 데이터를 받는 지 여부
	float         fWLOffset; //water level offset
	UnicodeString sWaterUseFlag;	// 수위 데이터를 유속계(RQ-30d) 수위 / 수위계의 수위 쓰는 지에 대한 구분 Flag
									// "HydroMeter" / "WaterLevel"

	//Logger 정보: 하층유속계 정보(AG Series)
	UnicodeString sHydroKind1;		// Column: HydroKind
	UnicodeString sAirmarPort;		// 유속계 RQ-30d일 때 Airmar 운영 여부
	int           iAG1_WN; //        WN, ->    측정 셀 총개수
	float         fAG1_WS; //        WS, ->    측정 셀 크기(cm)
	float         fAG1_WF; //        WF, ->    공백거리셀
	int           iSlaveCount;			// RQ-30d => Slave(RG-30d) counter   2022.12.16
	UnicodeString sHydroMasterAddress;	// Master Port Device Address (ex)5,4,3,2,1
	int           iHydroMasterCnt;      // Master Port Device Count
	UnicodeString sHydroUseSlave;       // ON/OFF Slave Port를 사용할지 말 지 Flag
	UnicodeString sHydroSlaveAddress;   // Slave Port Device Address (ex) 6,7,8,9,10
	int           iHydroSlaveCnt;       // Slave Port Device Count
	int           DeviceAddress[10]; 	// 좌안부터 Address를 기초데이터로 가진다.

	//Logger 정보: 상층유속계 정보(AG Series)
	UnicodeString sHydroKind2;		// Column: HydroKind
	int           iAG2_WN; //        WN, ->    측정 셀 총개수
	float         fAG2_WS; //        WS, ->    측정 셀 크기(cm)
	float         fAG2_WF; //        WF, ->    공백거리셀

	UnicodeString sHydroKind3;		// Column: HydroKind
	int           iAG3_WN; //        WN, ->    측정 셀 총개수
	float         fAG3_WS; //        WS, ->    측정 셀 크기(cm)
	float         fAG3_WF; //        WF, ->    공백거리셀

//todo: adv 생성을 위한 각 요소 유효범위(min/max)

	//RVTHLogger
	int   iAC_Min,         iAC_Max;
	float fDC_Charge_Min,  fDC_Charge_Max;
	float fDC_Battery_Min, fDC_Battery_Max;
	float fTemp_Sys_Min,   fTemp_Sys_Max;
	float fHr_Sys_Min,     fHr_Sys_Max;

	//RWaterLevel
	float fWaterLevel_Min, fWaterLevel_Max;
	float fSalinity_Min,   fSalinity_Max;

	//RHydroMeter
	float fTemp_Water_Min, fTemp_Water_Max;
	float fPitch_Min,      fPitch_Max;
	float fRoll_Min,       fRoll_Max;

	//RHydroMeterCell
	int fV_EW_Min,         fV_EW_Max;
	int fV_NS_Min,         fV_NS_Max;
};
//---------------------------------------------------------------------------
#endif
