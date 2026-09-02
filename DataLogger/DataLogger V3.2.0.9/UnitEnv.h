//---------------------------------------------------------------------------

#ifndef UnitEnvH
#define UnitEnvH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>

class TEnv {
public:
	UnicodeString m_DBIP;		// DB IP
	UnicodeString m_DBName;	// DB Name
	UnicodeString m_Account;		// DB Account

	UnicodeString m_PointCode;		// PointCode
	UnicodeString m_PointName;		// PointName

	UnicodeString m_RADSHome;		// RADS Home => D:\RADS
	UnicodeString m_DataLoggerHome;	// DataLogger Home => D:\RADS\DataLogger
	UnicodeString m_DataSenderHome;	// DataSender Home => D:\RADS\DataSender
	UnicodeString m_DataViewerHome;	// DataViewer Home => D:\RADS\DataViewer
	UnicodeString m_DataLoggerIP;	// 192.168.0.50(default)
	UnicodeString m_DataSenderIP;	// 192.168.0.60(default)
	UnicodeString m_DatabaseIP;		// 192.168.0.50(default)
	int           m_DataLoggerPort;	// 5001
	int           m_DataSenderFromLoggerPort;	// 5021
	int           m_DataSenderFromViewerPort;	// 5011
	int           m_WatchDogPort;	// 5031

	int           m_FlowType;		// 유량산출방식: 0: OFF,  '1': 유량사업단, '2': RNSEA 자체 방식
	int           m_SeaLevel;		// 해발표고

	UnicodeString m_Interval;		// 측정 주기( 10:00)
	int           m_IntervalSec;	// 측정 주기( 10:00) => 초(단위)
	UnicodeString m_WorkTime;		// 동작 주기( 08:30)
	int           m_WorkTimeSec;	// 동작 주기( 08:30) => 초(단위)
	int           m_StartTime;		// 시작 시간( 0 ~ 9)
									// 시작시간(0~9) + 측정주기*(0..5) => 다음 측정 시간

	// HydroMeter1
	UnicodeString m_HydroType1;		// 하층유속계 Type(ChannelMaster, SL Series, AG Series )
	UnicodeString m_HydroPort1;		// COM1~COM32
	int           m_HydroBaudRate1;	// 통신속도(9600)
	double        m_HydroOffset1;	// Offset
	UnicodeString m_HydroHz1;		// 동작주파수
	UnicodeString m_HydroSN1;		// 일련번호
	int           m_StartCell1;     // 유효셀 시작 번호(Default: 1) - 2022.03.18
	int           m_EndCell1;       // 유효셀 끝 번호(Default: 10)  - 2022.03.18

	double        m_CellBegin1;		// Cell Begin
	double        m_CellEnd1;		// Cell End
	double        m_CellNumber1;	// Cell Number
	double        m_CellSize1;		// Cell Size;
	double        m_CellBlank1;		// Cell Blank

	bool          m_bIdleCheck1;	// Idle check flag
	int           m_iIdleCheckTime1;// Idle check time => 180minutes(default)

	UnicodeString m_HydroAirmarPort1;	    // Airmar Port
	int           m_HydroAirmarBaudRate1;	// Airmar BaudRate
	bool          m_AirmarLinker;

	// RQ-30d 2024.09.10
	int           m_SlaveCount;		// RQ-30D => how many slave..
	UnicodeString m_HydroMasterAddress;  // RQ-30d Master Port Device Address (ex) "5,4,3,2,1"
	int           m_HydroMasterPortCount;// Master Port에 붙어 있는 Device의 숫자(Master 장비 포함)
	UnicodeString m_HydroUseSlavePort;   // ON/OFF
	UnicodeString m_HydroSlavePort;      // RQ-30d Slave Port
	int           m_HydroSlaveBaudRate;  // 2400,4800,9600,..
	UnicodeString m_HydroSlaveAddress;   // (ex) "6,7,8,9,10"
	int           m_HydroSlavePortCount; // Slave Port에 붙어 있는 Device의 숫자(RG-30d 슬레이브만의 숫자)

	// HydroMeter2
	UnicodeString m_HydroType2;		// 하층유속계 Type(ChannelMaster, SL Series, AG Series )
	UnicodeString m_HydroPort2;		// COM1~COM32
	int           m_HydroBaudRate2;	// 통신속도(9600)
	double        m_HydroOffset2;	// Offset
	UnicodeString m_HydroHz2;		// 동작주파수
	UnicodeString m_HydroSN2;		// 일련번호
	int           m_StartCell2;     // 유효셀 시작 번호(Default: 1) - 2022.03.18
	int           m_EndCell2;       // 유효셀 끝 번호(Default: 10) - 2022.03.18

	double        m_CellBegin2;		// Cell Begin
	double        m_CellEnd2;		// Cell End
	double        m_CellNumber2;	// Cell Number
	double        m_CellSize2;		// Cell Size;
	double        m_CellBlank2;		// Cell Blank

	bool          m_bIdleCheck2;	// Idle check flag
	int           m_iIdleCheckTime2;	// Idle check time => 180minutes(default)

	UnicodeString m_HydroAirmarPort2;	// Airmar Port
	int           m_HydroAirmarBaudRate2;	// Airmar BaudRate

	// WaterLevel
	UnicodeString m_WaterType;		// WaterLevel Type ( LT500, AT200, ... )
	UnicodeString m_WaterPort;		// WaterLevel ComPort
	int           m_WaterBaudRate;	// BaudRate
	double        m_WaterOffset;	// Offset
	UnicodeString m_WaterHz;		// 동작주파수
	UnicodeString m_WaterSN;		// 일련번호
	UnicodeString m_AtmosPort;		// 대기압센서 통신 Port
	double        m_BaseAtmos;		// 기준대기압

	bool          m_bIdleCheckWater;	// Idle check flag
	int           m_iIdleCheckWaterTime;	// Idle check time => 180minutes(default)

	// 유효범위(유속계/수위계 유효범위)
	double        m_dHydroLowerBound;	// 유속 최소값 (단위: mm/sec)
	double        m_dHydroUpperBound;	// 유속 최대값 (단위: mm/sec)
	double        m_dWaterLowerBound;	// 수위 최소값 (단위: m)
	double        m_dWaterUpperBound;	// 수위 최대값 (단위: m)

	int           m_StartMinute[6];	// 5,15,25,35,45,55

public:
	TEnv();
	~TEnv();

	// Ini file Load/Save
	bool LoadINI(UnicodeString sFile);
	bool SaveINI(UnicodeString sFile);

	int  GetIntervalTime();
	int  GetWorkingTime();
	int  GetRemainIdleTime(TDateTime now);	// 현재시간에서 다음 수행 시간까지 남은 시간
	bool IsStartMinute(int iMinute);

	// 환경 설정 정보의 Load/Save
	bool LoadEnv();
	bool SaveEnv();

	// 지점정보
	bool GetPointInfo();
	bool SetPointInto();

	// HOME Dir
	bool GetHomeDirInfo();
	bool SetHomeDirInfo();

	// 기타 정보
	bool GetEtcInfo();
	bool SetEtcInfo();

	// Logger Work info
	bool GetWorkInfo();
	bool SetWorkInfo();

	// Hydro1 Info
	bool GetHydro1Info();
	bool SetHydro1Info();

	// Hydro2 Info
	bool GetHydro2Info();
	bool SetHydro2Info();

	// WaterLevel Info
	bool GetWaterLevelInfo();
	bool SetWaterLevelInfo();
};

int ConvertComPort(UnicodeString sComPort);
int ConvertBaudRate(UnicodeString sBaudRate);

extern TEnv theEnv;		// Global Object

#endif
