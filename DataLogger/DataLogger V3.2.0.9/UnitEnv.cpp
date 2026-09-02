//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitEnv.h"
#include "UnitDM.h"
//#include <System.IOUtils.hpp>
#include "RSetup.h"

TEnv theEnv;

//---------------------------------------------------------------------------
// NONE,COM1,COM2,... => 0, 1, 2, ....., 32
int ConvertComPort(UnicodeString sComPort)
{
	int iComPort = 0;
	if ( UpperCase(sComPort) == "NONE" ) iComPort = 0;
	else {
		int iPos = sComPort.Pos("COM");
		if ( iPos > 0 ) {
			int iLen = sComPort.Length();
			UnicodeString sPort = sComPort.SubString(iPos+3, iLen-(iPos+3)+1);
			try {
				iComPort = StrToInt(sPort);
			} catch ( Exception &e) {
				iComPort = 0;
			}
		} else {
			iComPort = 0;
		}
	}
	return iComPort;
}

//---------------------------------------------------------------------------
// NONE, 9600, 19200, 38400, 57600, 115200(String) => 0,9600, 19200, 38400, 57600, 115200(Integer)
int ConvertBaudRate(UnicodeString sBaudRate)
{
	int iBaudRate = 0;
	if ( sBaudRate == "NONE") iBaudRate = 0;
	else {
		try {
			iBaudRate = StrToInt(sBaudRate);
		} catch ( Exception &e ) {
			iBaudRate = 0;
		}
	}
	return iBaudRate;
}

//---------------------------------------------------------------------------
//
// TEnv Member
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constructor
TEnv::TEnv()
{
	m_DBIP           = "127.0.0.1";						// DB IP
	m_DBName         = "D:\\RADS\\Database\\RADS.FDB";	// DB Name
	m_Account        = "rads";							// DB Account

	m_PointCode      = "000000";						// PointCode
	m_PointName      = "RNSEA";							// PointName

	m_RADSHome       = "C:\\Program Files\\RADS";		// RADS Home => D:\RADS
	m_DataLoggerHome = "D:\\RADS\\DataLogger";			// DataLogger Home => D:\RADS\DataLogger
	m_DataSenderHome = "D:\\RADS\\DataSender";			// DataSender Home => D:\RADS\DataSender
	m_DataViewerHome = "D:\\RADS\\DataViewer";			// DataViewer Home => D:\RADS\DataViewer

	m_FlowType       = 0;								// 유량산출방식: '0': 유량사업단, '1': RNSEA 자체 방식
	m_SeaLevel       = 0;								// 해발표고

	m_Interval       = "10:00";		// 측정 주기( 10:00)
	m_WorkTime       = "08:30";		// 동작 주기( 08:30)
	m_StartTime      = 5;			// 시작 시간( 0 ~ 9)
									// 시작시간(0~9) + 측정주기*(0..5) => 다음 측정 시간

	// HydroMeter1
	m_HydroType1     = "NONE";		// 하층유속계 Type(ChannelMaster, SL Series, AG Series )
	m_HydroPort1     = "NONE";		// COM1~COM32
	m_HydroBaudRate1 = 0;			// 통신속도(9600)
	m_HydroOffset1   = 0;			// Offset
	m_HydroHz1       = "";			// 동작주파수
	m_HydroSN1       = "";			// 일련번호
	m_StartCell1     = 1;           // 유효셀 시작 번호(Default: 1) - 2022.03.18
	m_EndCell1       = 10;          // 유효셀 끝 번호(Default: 10)  - 2022.03.18

	m_CellBegin1     = 0;			// Cell Begin
	m_CellEnd1       = 0;			// Cell End
	m_CellNumber1    = 0;			// Cell Number
	m_CellSize1      = 0;			// Cell Size;
	m_CellBlank1     = 0;			// Cell Blank

	m_bIdleCheck1    = true;		// idle Check flag
	m_iIdleCheckTime1 = 3;		    // idle check time => 180 mintues => 3 hours

	m_HydroAirmarPort1 = "NONE";    // Airmar Port
	m_HydroAirmarBaudRate1 = 0;		// Airmar BaudRate
	m_AirmarLinker     = false;   	// AirmarLinker Y/N
	m_SlaveCount       = 0;		    // RQ-30D => how many slave..

	// HydroMeter2
	m_HydroType2     = "NONE";		// 하층유속계 Type(ChannelMaster, SL Series, AG Series )
	m_HydroPort2     = "NONE";		// COM1~COM32
	m_HydroBaudRate2 = 0;			// 통신속도(9600)
	m_HydroOffset2   = 0;			// Offset
	m_HydroHz2       = "";			// 동작주파수
	m_HydroSN2       = "";			// 일련번호
	m_StartCell2     = 1;           // 유효셀 시작 번호(Default: 1) - 2022.03.18
	m_EndCell2       = 10;          // 유효셀 끝 번호(Default: 10)  - 2022.03.18

	m_CellBegin2     = 0;			// Cell Begin
	m_CellEnd2       = 0;			// Cell End
	m_CellNumber2    = 0;			// Cell Number
	m_CellSize2      = 0;			// Cell Size;
	m_CellBlank2     = 0;			// Cell Blank

	m_bIdleCheck2    = true;		// idle Check flag
	m_iIdleCheckTime2 = 3;		// idle check time => 180 mintues => 3 hours


	// WaterLevel
	m_WaterType      = "NONE";		// WaterLevel Type ( LT500, AT200, ... )
	m_WaterPort      = "NONE";		// WaterLevel ComPort
	m_WaterBaudRate  = 0;			// BaudRate
	m_WaterOffset    = 0;			// Offset
	m_WaterHz        = "";			// 동작주파수
	m_WaterSN        = "";			// 일련번호

	m_bIdleCheckWater    = true;		// idle Check flag
	m_iIdleCheckWaterTime = 3;		// idle check time => 180 mintues => 3 hours

	// 유효범위(유속계/수위계 유효범위)
	m_dHydroLowerBound = -15.0 * 1000;	// 유속 최소값 (단위: mm/sec) (-15m/sec ~ 15m/sec ) => mm로 환산하여 이렇게 됨
	m_dHydroUpperBound =  15.0 * 1000;	// 유속 최대값 (단위: mm/sec) (-15m/sec ~ 15m/sec ) => mm로 환산하여 이렇게 됨
	m_dWaterLowerBound = -100.0;			// 수위 최소값 (단위: m)
	m_dWaterUpperBound =  100.0;			// 수위 최대값 (단위: m)
}

//---------------------------------------------------------------------------
// Constructor
TEnv::~TEnv()
{
	// nothing to do..
}

//---------------------------------------------------------------------------
// load Ini file
bool TEnv::LoadINI(UnicodeString sFile)
{
	bool bResult = false;

	if ( sFile.IsEmpty() ) return false;

	if ( !TFile::Exists(sFile)) {
		SaveINI(sFile);
	}

	TIniFile *IniFile = new TIniFile(sFile);


	try {

		m_DBIP    = IniFile->ReadString("DATABASE","DatabaseIP",      "127.0.0.1");
		m_DBName  = IniFile->ReadString("DATABASE","DatabaseName",    "D:\\RADS\\Database\\RADS.FDB");
		m_Account = IniFile->ReadString("DATABASE","DatabaseAccount", "rads");

		bResult = true;

	} catch ( Exception &e ) {

		bResult = false;
	}

	delete IniFile;

	return bResult;
}

//---------------------------------------------------------------------------
// Save Ini file
bool TEnv::SaveINI(UnicodeString sFile)
{
	bool bResult = false;

	if ( sFile.IsEmpty() ) return false;

	TIniFile *IniFile = new TIniFile(sFile);

	try {

		IniFile->WriteString("DATABASE","DatabaseIP",      m_DBIP);
		IniFile->WriteString("DATABASE","DatabaseName",    m_DBName);
		IniFile->WriteString("DATABASE","DatabaseAccount", m_Account);

		bResult = true;

	} catch ( Exception &e ) {

		bResult = false;
	}

	delete IniFile;

	return bResult;
}

//---------------------------------------------------------------------------
// Load Env from DB
bool TEnv::LoadEnv()
{
	TSQLQuery *pQuery = NULL;
	TRSetup Setup;
	UnicodeString sSQL = " SELECT * FROM RSetup              " \
						 " WHERE ( PK1 = 1  AND PK2 = 1 ) OR " \
						 "       ( PK1 = 1  AND PK2 = 2 ) OR " \
						 "       ( PK1 = 1  AND PK2 = 3 ) OR " \
						 "       ( PK1 = 10 AND PK2 = 1 ) OR " \
						 "       ( PK1 = 10 AND PK2 = 2 ) OR " \
						 "       ( PK1 = 10 AND PK2 = 3 ) OR " \
						 "       ( PK1 = 10 AND PK2 = 4 )    " \
						 " ORDER BY PK1, PK2                 ";

	pQuery = DM->GetResultSet(sSQL);
	if ( pQuery == NULL ) return false;

	while ( !pQuery->Eof ) {
		try {
			Setup.Clear();

			Setup.PK1     = pQuery->FieldByName("PK1")->AsInteger;
			Setup.PK2     = pQuery->FieldByName("PK2")->AsInteger;
			Setup.Value01 = pQuery->FieldByName("Value01")->AsString;
			Setup.Value02 = pQuery->FieldByName("Value02")->AsString;
			Setup.Value03 = pQuery->FieldByName("Value03")->AsString;
			Setup.Value04 = pQuery->FieldByName("Value04")->AsString;
			Setup.Value05 = pQuery->FieldByName("Value05")->AsString;
			Setup.Value06 = pQuery->FieldByName("Value06")->AsString;
			Setup.Value07 = pQuery->FieldByName("Value07")->AsString;
			Setup.Value08 = pQuery->FieldByName("Value08")->AsString;
			Setup.Value09 = pQuery->FieldByName("Value09")->AsString;
			Setup.Value10 = pQuery->FieldByName("Value10")->AsString;
			Setup.Value11 = pQuery->FieldByName("Value11")->AsString;
			Setup.Value12 = pQuery->FieldByName("Value12")->AsString;
			Setup.Value13 = pQuery->FieldByName("Value13")->AsString;
			Setup.Value14 = pQuery->FieldByName("Value14")->AsString;
			Setup.Value15 = pQuery->FieldByName("Value15")->AsString;
			Setup.Value16 = pQuery->FieldByName("Value16")->AsString;
			Setup.Value17 = pQuery->FieldByName("Value17")->AsString;
			Setup.Value18 = pQuery->FieldByName("Value18")->AsString;
			Setup.Value19 = pQuery->FieldByName("Value19")->AsString;
			Setup.Value20 = pQuery->FieldByName("Value20")->AsString;
			Setup.Des     = pQuery->FieldByName("Des")->AsString;

			if ( Setup.PK1 == 1 && Setup.PK2 == 1 ) {          	// 지점정보
				m_PointCode       = Setup.Value01;	// Pointcode
				m_PointName       = Setup.Value02;	// PointName
			} else if ( Setup.PK1 == 1 && Setup.PK2 == 2 ) {	// 공통사항(Homedir)
				m_RADSHome        = Setup.Value01;
				m_DataLoggerHome  = Setup.Value02;
				m_DataSenderHome  = Setup.Value03;
				m_DataViewerHome  = Setup.Value04;
				m_DataLoggerIP    =(Setup.Value05.IsEmpty()) ? UnicodeString("127.0.0.1") : Setup.Value05;	// 192.168.0.50(default)
				m_DataSenderIP    =(Setup.Value06.IsEmpty()) ? UnicodeString("127.0.0.1") : Setup.Value06;	// 192.168.0.60(default)
				m_DatabaseIP      =(Setup.Value07.IsEmpty()) ? UnicodeString("127.0.0.1") : Setup.Value07;		// 192.168.0.50(default)
				m_DataLoggerPort           =(Setup.Value08.IsEmpty()) ? 5001 : StrToInt(Setup.Value08);	// 5001
				m_DataSenderFromLoggerPort =(Setup.Value09.IsEmpty()) ? 5021 : StrToInt(Setup.Value09);	// 5021
				m_DataSenderFromViewerPort =(Setup.Value10.IsEmpty()) ? 5011 : StrToInt(Setup.Value10);	// 5011
				m_WatchDogPort             =(Setup.Value11.IsEmpty()) ? 5031 : StrToInt(Setup.Value11);	// 5031
			} else if ( Setup.PK1 == 1 && Setup.PK2 == 3 ) { 	// DataViewer 운영 정보
				m_FlowType        = (Setup.Value02.IsEmpty()) ? 0 : StrToInt(Setup.Value02);    // 유량산출방식: '0': 유량사업단, '1': RNSEA 자체 방식
				m_SeaLevel        = (Setup.Value03.IsEmpty()) ? 0 : StrToInt(Setup.Value03);    // 해발표고
			} else if ( Setup.PK1 == 10 && Setup.PK2 == 1 ) {	// Logger 동작 시간
				m_Interval        = Setup.Value01;              // 측정 주기( 10:00)
				m_WorkTime        = Setup.Value02;              // 동작 주기( 08:30)
				m_StartTime       = (Setup.Value03.IsEmpty()) ? 0 : StrToInt(Setup.Value03);    // 시작 시간( 0 ~ 9)
			} else if ( Setup.PK1 == 10 && Setup.PK2 == 2 ) {	// 하층 유속계 정보
				m_HydroType1      = Setup.Value01;				// 하층유속계 Type(ChannelMaster, SL Series, AG Series )
				m_HydroPort1      = Setup.Value02;				// COM1~COM32
				m_HydroBaudRate1  = (Setup.Value03.IsEmpty()) ? 0 : StrToInt(Setup.Value03);				// 통신속도(9600)
				m_HydroOffset1    = (Setup.Value04.IsEmpty()) ? 0 : StrToFloat(Setup.Value04);	// Offset
				if ( m_HydroType1 == AGSERIES_STR || m_HydroType1 == SLSERIES_STR ) {
					m_CellBegin1      = (Setup.Value05.IsEmpty()) ? 0 : StrToFloat(Setup.Value05);	// Cell Begin
					m_CellEnd1        = (Setup.Value06.IsEmpty()) ? 0 : StrToFloat(Setup.Value06);	// Cell End
					m_CellNumber1     = (Setup.Value07.IsEmpty()) ? 0 : StrToFloat(Setup.Value07);	// Cell Number
					m_CellSize1       = (Setup.Value08.IsEmpty()) ? 0 : StrToFloat(Setup.Value08);	// Cell Size;
					m_CellBlank1      = (Setup.Value09.IsEmpty()) ? 0 : StrToFloat(Setup.Value09);	// Cell Blank
				} else if ( m_HydroType1 == RQ30D_STR ) {
					m_HydroMasterAddress = Setup.Value05;	// Master Port Address	(ex) 5,4,3,2,1
					m_HydroUseSlavePort  = Setup.Value06;	// Use Slave Port ( ON/OFF )
					m_HydroSlavePort     = Setup.Value07;   // Slave Port (COM01,COM2,...
					m_HydroSlaveBaudRate = (Setup.Value08.IsEmpty()) ? 0 : StrToInt(Setup.Value08);   // Slave Port Baud Rate (2400,4800,...
					m_HydroSlaveAddress  = Setup.Value09;   // Slave Port Address ( (ex) 6,7,8,9,10
					m_SlaveCount         = (Setup.Value13.IsEmpty()) ?  0 : StrToInt(Setup.Value13);	// heaven2 2022.12.12 RQ-30d Slave Count
				} else {
					// nothing to to..
				}
				m_HydroHz1        = Setup.Value11;  // Frequency
				m_HydroSN1        = Setup.Value12;  // Serial Number
				m_bIdleCheck1     = (Setup.Value14.IsEmpty()) ? true : ((Setup.Value14 == "ON") ? true : false);
				m_iIdleCheckTime1 = (Setup.Value15.IsEmpty()) ? 3  : StrToInt(Setup.Value15);	// 180minutes or thers
				// heaven2 2021.12.06   Airmar Port/Baudrate
				m_HydroAirmarPort1 = Setup.Value16;	// NULL, NONE, COM01, COM02,.. COM32
				m_HydroAirmarBaudRate1 = (Setup.Value17.IsEmpty()) ? 4800 : ((Setup.Value17 == "NONE") ? 0 : StrToInt(Setup.Value17)); // 4800(default), 9600,....
				m_AirmarLinker     = (Setup.Value20.IsEmpty()) ? false : ((Setup.Value20 == "ON" ) ? true : false); //heaven2 2022.12.01
				m_StartCell1       = (Setup.Value18.IsEmpty()) ?  1 : StrToInt(Setup.Value18);	// 유효셀 시작 번호(Default:1)
				m_EndCell1         = (Setup.Value19.IsEmpty()) ? 10 : StrToInt(Setup.Value19);	// 유효셀 끝번호(Default:10)
			} else if ( Setup.PK1 == 10 && Setup.PK2 == 3 ) {	// 상층 유속계 정보
				m_HydroType2      = Setup.Value01;				// 상층유속계 Type(ChannelMaster, SL Series, AG Series )
				m_HydroPort2      = Setup.Value02;				// COM1~COM32
				m_HydroBaudRate2  = (Setup.Value03.IsEmpty()) ? 0 : StrToInt(Setup.Value03);				// 통신속도(9600)
				m_HydroOffset2    = (Setup.Value04.IsEmpty()) ? 0 : StrToFloat(Setup.Value04);	// Offset
				m_CellBegin2      = (Setup.Value05.IsEmpty()) ? 0 : StrToFloat(Setup.Value05);	// Cell Begin
				m_CellEnd2        = (Setup.Value06.IsEmpty()) ? 0 : StrToFloat(Setup.Value06);	// Cell End
				m_CellNumber2     = (Setup.Value07.IsEmpty()) ? 0 : StrToFloat(Setup.Value07);	// Cell Number
				m_CellSize2       = (Setup.Value08.IsEmpty()) ? 0 : StrToFloat(Setup.Value08);	// Cell Size;
				m_CellBlank2      = (Setup.Value09.IsEmpty()) ? 0 : StrToFloat(Setup.Value09);	// Cell Blank
				m_HydroHz2        = Setup.Value12;  // Frequency
				m_HydroSN2        = Setup.Value13;  // Serial Number
				m_bIdleCheck2     = (Setup.Value14.IsEmpty()) ? true : ((Setup.Value14 == "ON") ? true : false);
				m_iIdleCheckTime2 = (Setup.Value15.IsEmpty()) ? 3  : StrToInt(Setup.Value15);	// 180minutes or thers
				// heaven2 2021.12.06   Airmar Port/Baudrate
				m_HydroAirmarPort2 = Setup.Value16;	// NULL, NONE, COM01, COM02,.. COM32
				m_HydroAirmarBaudRate2 = (Setup.Value17.IsEmpty()) ? 4800 : StrToInt(Setup.Value17); // 4800(default), 9600,....
				m_StartCell2       = (Setup.Value18.IsEmpty()) ?  1 : StrToInt(Setup.Value18);	// 유효셀 시작 번호(Default:1)
				m_EndCell2         = (Setup.Value19.IsEmpty()) ? 10 : StrToInt(Setup.Value19);	// 유효셀 끝번호(Default:10)
			} else if ( Setup.PK1 == 10 && Setup.PK2 == 4 ) {	// 수위계 정보
				m_WaterType       = Setup.Value01;				// WaterLevel Type ( LT500, AT200, ... )
				m_WaterPort       = Setup.Value02;				// WaterLevel ComPort
				m_WaterBaudRate   = (Setup.Value03.IsEmpty()) ? 0 : StrToInt(Setup.Value03);	// BaudRate
				m_WaterOffset     = (Setup.Value04.IsEmpty()) ? 0 : StrToFloat(Setup.Value04);	// Offset
				m_WaterHz         = Setup.Value05;	// Frequency
				m_WaterSN         = Setup.Value06;	// Serial Number
				m_AtmosPort       = Setup.Value07;	// Atmospheric Port if WaterLevel is LT400 2015.11.16
				m_BaseAtmos       = (Setup.Value08.IsEmpty()) ? 0 : StrToFloat(Setup.Value08); // 기준대기압
				m_bIdleCheckWater = (Setup.Value09.IsEmpty()) ? true : ((Setup.Value09 == "ON") ? true : false);
				m_iIdleCheckWaterTime = (Setup.Value10.IsEmpty()) ? 3  : StrToInt(Setup.Value10);	// 180minutes or thers
			} else {
				//nothing to do..
			}

			pQuery->Next();

		} catch ( Exception &e ) {
			//theLog.LogMessage(e.Message);
		}
	}
	DM->Release(pQuery);

	// m_Interval, m_WorkTime => 나중에 속도를 빠르게 하기 위해 미리 계산해 놓는다.
	int iMinute, iSecond;
	if ( m_Interval.Length() == 5 ) {	// 10:00
		try {
			iMinute = StrToInt(m_Interval.SubString(1,2));	// 08:30
			iSecond = StrToInt(m_Interval.SubString(4,2));	// 08:30
		} catch ( Exception &e ) {
			iMinute = 10;	// default
			iSecond =  0;	//default
		}
		m_IntervalSec = iMinute * 60 + iSecond;
	} else {
		m_IntervalSec = 0;
	}

	if ( m_WorkTime.Length() == 5 ) {	// 08:30
		try {
			iMinute = StrToInt(m_WorkTime.SubString(1,2));	// 08:30
			iSecond = StrToInt(m_WorkTime.SubString(4,2));	// 08:30
		} catch ( Exception &e ) {
			iMinute = 8;	// default
			iSecond = 30;	//default
		}
		m_WorkTimeSec = iMinute * 60 + iSecond;
	} else {
		m_WorkTimeSec = 0;
	}

	iMinute = StrToInt(m_Interval.SubString(1,2));

	m_StartMinute[0] = m_StartTime;					// 05
	m_StartMinute[1] = m_StartMinute[0] + iMinute;	// 15
	m_StartMinute[2] = m_StartMinute[1] + iMinute;	// 25
	m_StartMinute[3] = m_StartMinute[2] + iMinute;	// 35
	m_StartMinute[4] = m_StartMinute[3] + iMinute;	// 45
	m_StartMinute[5] = m_StartMinute[4] + iMinute;	// 55

	return true;
}

//---------------------------------------------------------------------------
// Save Env to DB
bool TEnv::SaveEnv()
{
	return true;
}

//---------------------------------------------------------------------------
// return Workint Time
int TEnv::GetIntervalTime()
{
	return (m_IntervalSec == 0 ) ? 0 : m_IntervalSec;
}

//---------------------------------------------------------------------------
// return Workint Time
int TEnv::GetWorkingTime()
{
	return (m_WorkTimeSec == 0) ? 0 : m_WorkTimeSec;
}

//---------------------------------------------------------------------------
int TEnv::GetRemainIdleTime(TDateTime now)	// 현재시간에서 다음 수행 시간까지 남은 시간
{
	USHORT hour, min, sec, msec;
	now.DecodeTime(&hour, &min, &sec, &msec);
	int iRemainSec = 0;
	int iIndex = 0;

	if ( min < m_StartMinute[0] ) {				// 5
		iIndex = 0;
	} else if ( min < m_StartMinute[1] ) {		// 15
		iIndex = 1;
	} else if ( min < m_StartMinute[2] ) {		// 25
		iIndex = 2;
	} else if ( min < m_StartMinute[3] ) {		// 35
		iIndex = 3;
	} else if ( min < m_StartMinute[4] ) {		// 45
		iIndex = 4;
	} else if ( min < m_StartMinute[5] ) {		// 55
		iIndex = 5;
	} else {	// 55 ~ 59 minute
		iRemainSec = (60 * 60) - ( min * 60 + sec ) + 5 * 60;	// 60:00 - 58:20 + 05:00 (자정까지 남은 시간 + 5분 시간)
		return iRemainSec;
	}

	iRemainSec = (m_StartMinute[iIndex] * 60) - (min * 60 + sec);

	return iRemainSec;
}

//---------------------------------------------------------------------------
// 현재 분이 시작해야할 분이면 True, 아니면 False
bool TEnv::IsStartMinute(int iMinute)
{
	bool bFound = false;
	for ( int i = 0 ; i < 6 ; i++ ) {
		if ( iMinute == m_StartMinute[i] ) {
			bFound = true;
			break;
		}
	}
	return bFound;
}

//---------------------------------------------------------------------------
// 지점정보
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get Point Info
bool TEnv::GetPointInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Save Point Info
bool TEnv::SetPointInto()
{
	return true;
}

//---------------------------------------------------------------------------
// Get Home Dir Info
bool TEnv::GetHomeDirInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Set Home Dir Info
bool TEnv::SetHomeDirInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// 기타 정보
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get ETC Info
bool TEnv::GetEtcInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Set Etc Info
bool TEnv::SetEtcInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Logger Work info
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get Logger Work Info
bool TEnv::GetWorkInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Set Logger Work Info
bool TEnv::SetWorkInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Hydro1 Info
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get Hydro1 Info
bool TEnv::GetHydro1Info()
{
	return true;
}

//---------------------------------------------------------------------------
// Set Hydro1 Info
bool TEnv::SetHydro1Info()
{
	return true;
}

//---------------------------------------------------------------------------
// Hydro2 Info
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get Hydro2 Info
bool TEnv::GetHydro2Info()
{
	return true;
}

//---------------------------------------------------------------------------
// Set Hydro2 Info
bool TEnv::SetHydro2Info()
{
	return true;
}

//---------------------------------------------------------------------------
// WaterLevel Info
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get WaterLevel Info
bool TEnv::GetWaterLevelInfo()
{
	return true;
}

//---------------------------------------------------------------------------
// Set WaterLevel Info
bool TEnv::SetWaterLevelInfo()
{
	return true;
}


#pragma package(smart_init)
