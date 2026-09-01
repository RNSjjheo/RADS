// ----------------------------------------------------------------------------
//
//	Define.h	2014.04.02
//		Basic Data Structure define
//
// ----------------------------------------------------------------------------

#ifndef DEFINE_BASIC_DATA
#define DEFINE_BASIC_DATA

// DB Action
#define ACTION_INSERT		   1
#define ACTION_UPDATE		   2
#define ACTION_DELETE		   3

// for Animation
#define FLOW				0x01	// WaterFlow   : 유량산출
#define HYDRO1				0x02	// HydroMeter1 : 유속계1
#define HYDRO2				0x04	// HydroMeter2 : 유속계2
#define WATERLEVEL			0x08	// WaterLevel  : 수위계

#define BUFFER_8K			8192
#define BUFFER_4K			4096
#define BUFFER_2K			2048
#define BUFFER_1K			1024

#define UM_MESSAGE 		   (WM_APP+1234)	// User Message ( Logger Status )
#define EVENT_WAIT_TIME 	1000	// log Event Wait Time ( 1 Sec ) => Every 1 second



enum EN_SOCKET_ACTION { enSocketIdle = 0x00,
						enSocketConnected,
						enSocketDisconnected,
						enSocketError };

// Logger Working Step
enum EN_LOGGER_STEP { enIdle = 0x00,
					  enStart,
					  enWaterLevelStart,
					  enWaterLevelWorking,
					  enWaterLevelFinish,
					  enHydroMeter1Start,
					  enHydroMeter1Working,
					  enHydroMeter1Finish,
					  enHydroMeter2Start,
					  enHydroMeter2Working,
					  enHydroMeter2Finish,
					  enHydroMeter3Start,
					  enHydroMeter3Working,
					  enHydroMeter3Finish,
					  enRQ30Start, 			// heaven2 2022.12.12 1minute event
					  enRQ30Finish,			// heaven2 2022.12.12 1minute event
					  enEWSVStart,
					  enEWSVFinish,
					  enFinish };

// Sensor Status
// Sensor 동작 상태 : 수위계, 유속계
enum EN_SENSOR_STEP { enSensorIdle = 0x00,
					  enSensorStart,
					  enSensorWorking,
					  enSensorFinish };

enum EN_POINT_STATUS { enPointInit,     // 'N' : Init ( 초기값: 측정전값)
					   enPointStart,    // 'S' : 측정 시작
					   enPointFinish,   // 'Y' : 정상 측정
					   enPointLeak,	    // 'L' : Leak ( 결측 : 모든 값이 0 )
					   enPointEmpty };  // 'E' : Empty ( 장비없음 )

// RQ-30d Action
enum EN_RQ30D_ACTION { enRQ30dMeasureStart, enRQ30dMeasureFinish };

// EWSVConverter Action
enum EN_EWSVCONVERTER_ACTION { enEWSVMeasureStart, enEWSVMeasureFinish } ;

enum EN_HYDRO_NO { enHydroFirst, enHydroSecond, enHydroThird };	// 1번 유속계, 2번 유속계, 3번 유속계


//----------------------------------------------------------------------
//
//  Sensor Icon Animation Enumration Type 2014.09.16
//
//----------------------------------------------------------------------

enum EN_HYDROMETER_ICON
	{ enHydroMeterAni0     = 0,
	  enHydroMeterAni1     = 1,
	  enHydroMeterAni2     = 2,
	  enHydroMeterAni3     = 3,
	  enHydroMeterAni4     = 4,
	  enHydroMeterAni5     = 5,
	  enHydroMeterAni6     = 6,
	  enHydroMeterAni7     = 7,
	  enHydroMeterAni8     = 8,
	  enHydroMeterAni9     = 9,
	  enHydroMeterAni10    = 10,
	  enHydroMeterDisable  = 11,
	  enHydroMeterNormal   = 12
	};

enum EN_WATERLEVEL_ICON
	{ enWaterLevelAni0    = 0,
	  enWaterLevelAni1    = 1,
	  enWaterLevelAni2    = 2,
	  enWaterLevelAni3    = 3,
	  enWaterLevelDisable = 4,
	  enWaterLevelNormal  = 5
	};

enum EN_FLOWSTREAM_ICON
	{ enFlowStreamAni0    = 0,
	  enFlowStreamAni1    = 1,
	  enFlowStreamAni2    = 2,
	  enFlowStreamAni3    = 3,
	  enFlowStreamAni4    = 4,
	  enFlowStreamAni5    = 5,
	  enFlowStreamAni6    = 6,
	  enFlowStreamAni7    = 7,
	  enFlowStreamAni8    = 8,
	  enFlowStreamDisable = 9,
	  enFlowStreamNormal  = 10
	};

#define ON_STR			"ON"	// 2024.09.16
#define OFF_STR 		"OFF"	// 2024.09.16

// for Animation
#define FLOW		0x01	// WaterFlow   : 유량산출
#define HYDRO1		0x02	// HydroMeter1 : 유속계1
#define HYDRO2		0x04	// HydroMeter2 : 유속계2
#define WATERLEVEL	0x08	// WaterLevel  : 수위계

#define HYDROMETER_STR      "HydroMeter"	// 수위데이터 사용 구분에 사용
#define WATERLEVEL_STR		"WaterLevel"    // 수위데이터 사용 구분에 사용

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

// 유량산출
#define FLOWSTREAM_OFF			0	// 유량산출 OFF
#define FLOWSTREAM_HSC			1	// 유량조사사업단 방식
#define FLOWSTREAM_RNSEA		2	// RNSEA 자체 방식=> 현재없음.

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

// Sensor Status => Network Value
#define SENSOR_STATUS_TO_NETWORK(x) (( x == enSensorIdle )    ? 0x01 : \
									(( x == enSensorStart )   ? 0x02 : \
									(( x == enSensorWorking ) ? 0x03 : \
									(( x == enSensorFinish )  ? 0x04 : 0x00))))

const bool bGCSFirstHydroMeter = true;
const bool bGCSFileLog = true;
const bool bGCSDBLog = true;

//heaven2 2015.11.19 => for LT400
struct STData {
	double    WaterLevel;
	double    Depth;    		// Dept => Depth 2021.11.23
	double    Temperature;
	double    Pressure;
	double    Offset;
	double    ATemperature;
	double    APressure;
	double    ABaseAtmos;
	double    CPressure;
	//double  CPCompensate;
	double    CDepth;			// CDept => CDepth 2021.11.23
	//double  CDCompensate;

	STData() {
		WaterLevel   = 0;
		Depth        = 0;
		Temperature  = 0;
		Pressure     = 0;
		Offset       = 0;
		ATemperature = 0;
		APressure    = 0;
		ABaseAtmos   = 0;
		CPressure    = 0;
		CDepth       = 0;
	}
};



#pragma pack(1)

struct STRnsWater {
	BYTE  Type;       // 0:NONE, 1:VegaPulse(4~20mA), 2:VegaPuls+485Converter, 3:VegaPulse(485), ..
	float Depth;      // Depth(Unit:m)
	float Temperature;// Tempuerature(Unit:C)
	float Pressure;   // Pressure(Unit:?)
	float Salinity;   // Salinity(Unit:?)
	float Reserved01;
	float Reserved02;
	float Reserved03;
	float Reserved04;
};

//---------------------------------------------------------------------------
// STAirmar Structure => 이 데이터는 Airmar 데이터를 AirmarLinker를 통해 받을 때 사용함
struct STAirmarData {
	float Wind;
	char  WindUnit;
	float WindGust;
	float Direction;
	char  DirectionType;
	float Temperature;
	char  TemperatureUnit;
	float Humidity;
	float Pressure;
	char  PressureUnit;
	BYTE  Complete; 	// Wind-Degree-Temp-Humidity-Pressure ( 1111 1xxxx )

	STAirmarData::STAirmarData() {
		memset(this, 0x00, sizeof(STAirmarData));
	}
};


#pragma pack()


#endif
