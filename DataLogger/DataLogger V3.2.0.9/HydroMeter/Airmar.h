//---------------------------------------------------------------------------

#ifndef AirmarH
#define AirmarH
//---------------------------------------------------------------------------

#include "../Define.h"
#include "../UnitMain.h"

#include "AdPort.hpp"
#include "OoMisc.hpp"

#define AIRMAR_WIND	        0x80
#define AIRMAR_DIRECTION    0x40
#define AIRMAR_TEMPERATURE  0x20
#define AIRMAR_HUMIDITY     0x10
#define AIRMAR_PRESSURE     0x08

#define AIRMAR_NONE            0
#define AIRMAR_WIMWV           1
#define AIRMAR_HCHDT           2
#define AIRMAR_YXXDR           3
#define AIRMAR_WIMDA           4
#define AIRMAR_GPZDA           5
#define AIRMAR_GPVTG           6
#define AIRMAR_GPGGA           7

#define AIRMAR_DataReq      0x01	// Data Request  to AirmarLinker
#define AIRMAR_DataRes      0x11	// Data Response from  AirmarLinker

#pragma pack(1)

//---------------------------------------------------------------------------
// Airmar Data Structure
//struct STAirmarData {
//	double Wind;			// 풍속(3.5m)
//	char   WindUnit;        // K/M/N/S => Km/hr, m/s, knots, status miles/hr => default: N(knots)
//	double WindGust;		// 최대풍속(Wind 중 최대 풍속) => Complete에서 제외
//	double Direction;		// 풍향(0~360)
//	char   DirectionType;   // 풍향종류(R/T=>Relative/Theoretical)
//	double Temperature;		// 온도
//	char   TemperatureUnit;	// 온도 단위(C)
//	double Humidity;		// 습도
//	double Pressure;		// 기압
//	char   PressureUnit;    // 기압 단위(B:Bar)
//	BYTE   Complete;		//  1111 1000, 풍속-풍향-온도-습도, 기압
//
//	STAirmarData::STAirmarData() {
//		memset(this, 0x00, sizeof(STAirmarData));
//	}
//};

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

//---------------------------------------------------------------------------
// Airmar Class 
class TAirmar 
{
public:		// Attribute
	
	TApdComPort   *m_pPort;				// ComPort
	EN_SENSOR_STEP m_enStep;			// Sensor Status ( idle-start-working-finish )

	UnicodeString  m_MeasureDate;
	UnicodeString  m_MeasureTime;
	
	AnsiString     m_RecvMsg;			// Airmar Receive Message Buffer
	STAirmarData   AirmarData;			// Airmar Data(Receive)
	STAirmarData   Airmar;				// Airmar Data(using AirmarLinker)

	TList         *m_pAirmarList;		// buffer of airmar data

	BYTE           m_LinkerBuffer[BUFFER_1K];
	int            m_LinkerPos;
	bool           m_bLinkerBuffering;
	bool           m_bLinker;		// AirmarLinker 사용 여부

public:		// Method
	TAirmar();
	~TAirmar();

	void   Clear();
	bool   Init(TApdComPort *pPort, int PortNum, int BaudRate);
	bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime);
	bool   Finish();
	bool   Decode(AnsiString RecvMessage, int iLen);
	bool   Decode(BYTE *pBuffer, int iLen);
	bool   Receive(AnsiString RecvMessage, int iLen);
	bool   Receive(BYTE *pBuffer, int iLen);
	bool   ReceiveLinker(BYTE *pBuffer, int iLen);
	bool   DecodeLinker(BYTE *pBuffer, int iLen);
	int    CheckFrame(BYTE *pBuffer, int Len);

	bool   CaculateAirmirLinker();		// STAirmarLinker => STAirmarData to TRQ30 ..
	bool   SendAirmarLinker();	// AirmarLinker에 데이터를 요청한다.

	bool   CaculateAirmarData();		// Caculate Average WindSpeed, Degree, Temp, Humidity
	double CaculateWindGust();		    // Caculate WindGust .. 3 Step 
	
	STAirmarData GetAirmarData();
	
	void   LogMessage(UnicodeString Message);
};

//---------------------------------------------------------------------------
// AverageAngle Class
class AverageAngle
{
public:
    enum AngleType { DEGREES, RADIANS };

    AverageAngle(const enum AngleType type = DEGREES);

    void add(float alpha, float length = 1.0);
    void reset();
    int  count() { return _count; };
    float getAverage();

    float getTotalLength();
    float getAverageLength();

    enum AngleType type() { return _type; };

private:
    enum AngleType _type;
    float _sumx;
    float _sumy;
    uint32_t _count;
};


#endif
