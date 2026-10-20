//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "Airmar.h"

#define PI 3.14159265359

/////////////////////////////////////////////////////////////////////////////
// AverageAngle Class
// Reference Project: RMeasureADCP 
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// Constructor
AverageAngle::AverageAngle(const enum AngleType type)
{
  _type = type;
  reset();
}

//---------------------------------------------------------------------------
void AverageAngle::add(float alpha, float length)
{
  if (_type == AverageAngle::DEGREES )
  {
	alpha *= (PI / 180.0);
  }
  _sumx += (cos(alpha) * length);
  _sumy += (sin(alpha) * length);
  _count++;
}

//---------------------------------------------------------------------------
void AverageAngle::reset()
{
  _sumx = 0;
  _sumy = 0;
  _count = 0;
}

//---------------------------------------------------------------------------
float AverageAngle::getAverage()
{
  float angle = atan2(_sumy, _sumx);
  if (angle < 0) angle += (PI*2);
  if (_type == AverageAngle::DEGREES )
  {
    angle *= (180.0 / PI);
  }
  return angle;
}

//---------------------------------------------------------------------------
float AverageAngle::getTotalLength()
{
  if (_count == 0) return 0;
  return hypot(_sumy, _sumx);
}

//---------------------------------------------------------------------------
float AverageAngle::getAverageLength()
{
  if (_count == 0) return 0;
  return hypot(_sumy, _sumx) / _count;
}

/////////////////////////////////////////////////////////////////////////////
//
// TAirmar Class
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// Constructor
TAirmar::TAirmar()
{
	m_pPort         = NULL;
	m_enStep        = enSensorIdle;			// Sensor Status ( idle-start-working-finish )

	m_MeasureDate = "";
	m_MeasureTime = "";
	
	m_RecvMsg       = "";
	
	m_pAirmarList = new TList();
}

//---------------------------------------------------------------------------
// Deconstructor
TAirmar::~TAirmar()
{
	// Resource release
	STAirmarData *pAirmarData  = NULL;
	for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
		try {
			pAirmarData = (STAirmarData *) m_pAirmarList->Items[i];
			delete pAirmarData;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pAirmarList->Clear();
	
	delete m_pAirmarList;
}

//---------------------------------------------------------------------------
// Clear
void TAirmar::Clear()
{
	m_enStep        = enSensorIdle;			// Sensor Status ( idle-start-working-finish )

	m_MeasureDate = "";
	m_MeasureTime = "";

	m_RecvMsg       = "";
	memset(&AirmarData, 0x00, sizeof(STAirmarData));	// Airmar Data
	
	STAirmarData *pAirmarData  = NULL;
	for ( int i = 0 ; i < m_pAirmarList->Count ; i++ ) {
		try {
			pAirmarData = (STAirmarData *) m_pAirmarList->Items[i];
			delete pAirmarData;
		} catch ( Exception &e ) {
			break;
		}
	}
	m_pAirmarList->Clear();	
}

//---------------------------------------------------------------------------
// Init
bool TAirmar::Init(TApdComPort *pPort, int PortNum, int BaudRate)
{
	bool bResult = false;
	
	m_pPort = pPort;
	
	try {
		m_pPort->ComNumber = PortNum;
		m_pPort->Baud      = BaudRate;	// default: 4800bps
		m_pPort->Open      = true;
		bResult            = true;
	} catch ( Exception &e ) {
		LogMessage("Airmar::Init" + e.Message);
		bResult            = false;
	}
	return bResult;
}

//---------------------------------------------------------------------------
// Start
bool TAirmar::Start(UnicodeString MeasureDate, UnicodeString MeasureTime)
{
	Clear();	// Status Clear

	m_enStep = enSensorStart;

	m_MeasureDate = MeasureDate;
	m_MeasureTime = MeasureTime;

	LogMessage("Airmar started..");

	return true;
}

//---------------------------------------------------------------------------
// Finish
bool TAirmar::Finish()
{
	bool bResult = false;
	
	m_enStep = enSensorFinish;	// STEP : Finish
	
	bResult = CaculateAirmarData();
	
	m_enStep = enSensorIdle;	// STEP : Idle

	bResult = true;	// Finsih

	LogMessage("Airmar finished..");

	return bResult;
}

//---------------------------------------------------------------------------
// Decode
bool TAirmar::Decode(AnsiString RecvMessage, int iLen)
{
	if ( m_enStep < enSensorStart || m_enStep > enSensorFinish ) return false;	// sensor idle status => not buffering
		
	static int Tick    = 0;
	static bool bWIMWV = false;
	static bool bWIMDA = false;

	TStringList *pStrList = new TStringList();

	pStrList->Delimiter = ',';
	pStrList->DelimitedText = RecvMessage;

	if ( pStrList->Count <= 0 ) {
		LogMessage("AirmarProcess..lack of token..");
		delete pStrList;	// release
		return false;
	}

	AnsiString Prefix = pStrList->Strings[0];
	int Type = 0;
	if ( Prefix == "$WIMWV" )      Type = AIRMAR_WIMWV;
	else if ( Prefix == "$HCHDT" ) Type = AIRMAR_HCHDT;
	else if ( Prefix == "$YXXDR" ) Type = AIRMAR_YXXDR;
	else if ( Prefix == "$WIMDA" ) Type = AIRMAR_WIMDA;
	else if ( Prefix == "$GPZDA" ) Type = AIRMAR_GPZDA;
	else if ( Prefix == "$GPVTG" ) Type = AIRMAR_GPVTG;
	else if ( Prefix == "$GPGGA" ) Type = AIRMAR_GPGGA;
	else                           Type = AIRMAR_NONE;

	switch ( Type ) {
		case AIRMAR_WIMWV :    	// $WIMWV,311.3,R,0.4,N,A**27"
			{
				Tick++;
				if ( Tick >= 20 ) {
					bWIMWV = true;
					bWIMDA = true;
				}
				if ( bWIMWV ) {
					bWIMWV = false;
					LogMessage(RecvMessage);
				}
				if ( pStrList->Count < 6 ) {
					LogMessage("Airmar .. Lack of Token .. $WIMWV..");
					break;
				}
				AnsiString Valid = pStrList->Strings[5].SubString(1,1);
				if ( Valid == "A" ) {
					try {
						AnsiString WindStr       = pStrList->Strings[3];
						AnsiString WindUnit      = pStrList->Strings[4];
						AnsiString Direction     = pStrList->Strings[1];
						AnsiString DirectionType = pStrList->Strings[2];

						AirmarData.Wind          = StrToFloat(WindStr);
						AirmarData.WindUnit      = WindUnit.operator [](1);
						AirmarData.Direction     = StrToFloat(Direction);
						AirmarData.DirectionType = DirectionType.operator [](1);

						AirmarData.Complete |= (AIRMAR_WIND | AIRMAR_DIRECTION);

					} catch ( Exception &e ) {
						AirmarData.Wind          = 0;
						AirmarData.WindUnit      = 'N';	// knots
						AirmarData.Direction     = 0;
						AirmarData.DirectionType = 'R';	// relative
						
						AirmarData.Complete |= (AIRMAR_WIND | AIRMAR_DIRECTION);
					}
				} else {
					LogMessage("Invalid AIRMAR_WIMWV message(V)..");
				}
			}
			break;
		case AIRMAR_HCHDT :  	// Heading
		case AIRMAR_YXXDR :		// Transducer Measurements
			break;
		case AIRMAR_WIMDA :  	// Meteorological Composite
			{
				if ( bWIMDA ) {
					bWIMDA = false;
					LogMessage(RecvMessage);
				}
				if ( pStrList->Count < 10 ) {
					LogMessage("Airmar .. Lack of Token .. $WIMDA..");
					break;
				}
				try {
					AnsiString TemperatureStr     = pStrList->Strings[5];
					AnsiString TemperatureUnitStr = pStrList->Strings[6];
					AnsiString HumidityStr        = pStrList->Strings[9];
					AnsiString PressureStr        = pStrList->Strings[3];
					AnsiString PressureUnit       = pStrList->Strings[4];

					AirmarData.Temperature     = StrToFloat(TemperatureStr);
					AirmarData.TemperatureUnit = TemperatureUnitStr.operator [](1);
					AirmarData.Humidity        = StrToFloat(HumidityStr);
					AirmarData.Pressure        = StrToFloat(PressureStr);
					AirmarData.PressureUnit    = PressureUnit.operator [](1);

					AirmarData.Complete |= (AIRMAR_TEMPERATURE | AIRMAR_HUMIDITY | AIRMAR_PRESSURE);

				} catch ( Exception &e ) {
					AirmarData.Temperature     = 0;
					AirmarData.TemperatureUnit = 'C';
					AirmarData.Humidity        = 0;
					AirmarData.Pressure        = 0;
					AirmarData.PressureUnit    = 'B';
					
					AirmarData.Complete |= (AIRMAR_TEMPERATURE | AIRMAR_HUMIDITY | AIRMAR_PRESSURE);
				}
			}
			break;
		case AIRMAR_GPZDA : 	// standard Time and Date
			{
//				if ( GTimeSync ) {
//					if ( pStrList->Count < 6 ) {	// Airmar not catch GPS satilite..
//						LogMessage("Airmar .. Lack of Token .. $GPZDA..");
//						GTimeSync = false;
//						break;
//					}
//					LogMessage("GPS Time Sync ..");
//					LogMessage(RecvMessage);
//					AnsiString hhnnss = pStrList->Strings[1];	// hhnnss
//					AnsiString day    = pStrList->Strings[2];	// 01~31
//					AnsiString month  = pStrList->Strings[3];	// 01~12
//					AnsiString year   = pStrList->Strings[4];	// ex)2006
//
//					USHORT y,m,d,h,n,s,ms;
//					try {
//						y = StrToInt(year);
//						m = StrToInt(month);
//						d = StrToInt(day);
//						h = StrToInt(hhnnss.SubString(1,2));
//						n = StrToInt(hhnnss.SubString(3,2));
//						s = StrToInt(hhnnss.SubString(5,2));
//						ms = 500;
//					} catch ( Exception &e ) {
//						LogMessage("GPS Time token is incorrect..");
//						GTimeSync = false;	// Finished
//						break;
//					}
//
//					if ((y < 2018 || y > 2020)) {
//						GTimeSync = false;	// Finished
//						break;
//					}
//
//					TDateTime x;
//					try {
//						x = TDateTime(y,m,d,h,n,s,500);
//						x = x + TDateTime(theEnv.LocalTime,0,0,0);	// add 9 hours => Local Time;
//					} catch ( Exception &e ) {
//						LogMessage("GPS Time token is incorrect..");
//						GTimeSync = false;
//						break;
//					}
//
//					x.DecodeDate(&y, &m, &d);
//					x.DecodeTime(&h, &n, &s, &ms);
//
//					SYSTEMTIME sysTime;
//					sysTime.wYear   = y;
//					sysTime.wMonth  = m;
//					sysTime.wDay    = d;
//					sysTime.wHour   = h;
//					sysTime.wMinute = n;
//					sysTime.wSecond = s;
//					sysTime.wMilliseconds = ms;
//
//					BOOL bResult = FALSE;
//					bResult = ::SetLocalTime(&sysTime);
//					if ( bResult == TRUE ) {
//						LogMessage("GPS TimeSync ok..");
//					} else {
//						LogMessage("GPS TimeSync fail..");
//					}
//					GTimeSync = false;	// Finished
//				} else {
//					//LogMessage("GPS data.. skip..");
//					// Skip => nothing to do..
//					// not necessary GPS Time Sync
//				}
			}
			break;
		case AIRMAR_GPVTG :
		case AIRMAR_GPGGA :
		default           :
			break;
	} // end of switch

	// 모든 자료가 채워졌으면 AirmarList에 추가한다.
	if ( AirmarData.Complete == (AIRMAR_WIND | AIRMAR_DIRECTION | AIRMAR_TEMPERATURE | AIRMAR_HUMIDITY | AIRMAR_PRESSURE)) {
		STAirmarData *pAirmarData = new STAirmarData();
		memcpy(pAirmarData, &AirmarData, sizeof(STAirmarData));		// Memory copy result set 
		m_pAirmarList->Add(pAirmarData);	// Add Airmar List
		if ( Tick >= 20 ) {
			Tick = 0;
			bWIMWV = false;
			bWIMDA = false;
			LogMessage("Airmar data added.. 20 items");	// it take 20 seconds... every 20 second .. log ..
		}

		memset(&AirmarData, 0x00, sizeof(STAirmarData)); // Airmar value clearr
	}

	delete pStrList;	// release
	return true;
}

//---------------------------------------------------------------------------
// Decode
bool TAirmar::Decode(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAirmar::Receive(AnsiString RecvMsg, int iLen)
{
	if ( m_enStep == enSensorIdle ) return true;	// not receive status

	m_RecvMsg += RecvMsg;

	int Pos = 0;
	AnsiString RecvMessage = "";
	
	Pos = m_RecvMsg.AnsiPos("\r\n");		//(ex)$WIMWV,311.3,.....\r\n
	while ( Pos > 0 ) {
		
		RecvMessage = m_RecvMsg.SubString(1, Pos-1);
		RecvMessage = Trim(RecvMessage);	// Trim other character

WCHAR message[100];
wsprintf(message, L"Airmar data string -------------..%d", RecvMessage.Length());
OutputDebugString(message);

		Decode(RecvMessage, RecvMessage.Length());

		m_RecvMsg = m_RecvMsg.SubString(Pos+2, m_RecvMsg.Length() - (Pos+1));
		Pos = m_RecvMsg.AnsiPos("\r\n");
	}		
	
	return true;
}

//---------------------------------------------------------------------------
// Receive
bool TAirmar::Receive(BYTE *pBuffer, int iLen)
{
	return true;
}

//---------------------------------------------------------------------------
// Caculate Average WindSpeed, Degree, Temp, Humidity
bool TAirmar::CaculateAirmarData()		
{
	bool bResult = false;
	memset(&AirmarData, 0x00, sizeof(STAirmarData));	// Airmar Data Clear 
	
    AverageAngle AA(AverageAngle::DEGREES);
    AA.reset();
    
    STAirmarData Airmar;
    memset(&Airmar, 0x00, sizeof(STAirmarData));
    int Count = m_pAirmarList->Count;

	//------------------------------------------------------------------------    
    if ( Count >= 3 ) {							// 3 record over 
		double *AvgWind = (double *) malloc((Count-2) * sizeof(double));
    	double STemperature = 0.0f;
    	double SHumidity    = 0.0f;
    	double SPressure    = 0.0f;
    	
    	STAirmarData *pAirmar = NULL;
    	int j = 0; 
    	for ( int i = 0 ; i < Count ; i++ ) {
			pAirmar = (STAirmarData *) m_pAirmarList->Items[i];
			AA.add(pAirmar->Direction);							// Direction, Degree
			if ( i == 0 || i == 1 ) {
				STemperature += pAirmar->Temperature;
				SHumidity    += pAirmar->Humidity;
				SPressure    += pAirmar->Pressure;
			} else {
				STemperature += pAirmar->Temperature;
				SHumidity    += pAirmar->Humidity;
				SPressure    += pAirmar->Pressure;
			
				AvgWind[j++] = (((STAirmarData *) m_pAirmarList->Items[i  ])->Wind +
								((STAirmarData *) m_pAirmarList->Items[i-1])->Wind +
								((STAirmarData *) m_pAirmarList->Items[i-2])->Wind) / 3;
			}
    	}
    	
    	// Wind / WindGust
    	double WindGust = *(AvgWind+0);
    	double SWind = 0.0f;
		for ( int i = 0 ; i < j ; i++ ) {
    		SWind += AvgWind[i];
			if ( WindGust < *(AvgWind+i) ) {
				WindGust = *(AvgWind+i);
			}
		}

		AirmarData.Wind         = SWind / Count;	    // 풍속(3.5m)
		AirmarData.Wind         = AirmarData.Wind * 0.514444; 	// Knote => meter/sec
		//AirmarData.WindUnit;                          // K/M/N/S => Km/hr, m/s, knots, status miles/hr => default: N(knots)
		AirmarData.WindGust     = WindGust;		        // 최대풍속(Wind 중 최대 풍속) => Complete에서 제외
		AirmarData.WindGust     = AirmarData.WindGust * 0.514444; 	// Knote => meter/sec
		AirmarData.Direction    = AA.getAverage();		// 풍향(0~360)
		//AirmarData.DirectionType;                     // 풍향종류(R/T=>Relative/Theoretical)
		AirmarData.Temperature  = STemperature / Count;	// 온도
		//AirmarData.TemperatureUnit;	                // 온도 단위(C)
		AirmarData.Humidity     = SHumidity / Count;    // 습도
		AirmarData.Pressure     = SPressure / Count ;   // 기압
		//AirmarData.PressureUnit;                      // 기압 단위(B:Bar)
		//Complete                = 0xFF;	                //  1111 1000, 풍속-풍향-온도-습도, 기압

		free(AvgWind);

	//------------------------------------------------------------------------
	} else if ( Count == 1 || Count == 2 ) {	// 1 or 2 record

		double STemperature = 0.0f;
		double SHumidity    = 0.0f;
		double SPressure    = 0.0f;
		double SWind        = 0.0f;
		double WindGust     = 0.0f;

		STAirmarData *pAirmar = NULL;
		for ( int i = 0 ; i < Count ; i++ ) {
			pAirmar = (STAirmarData *) m_pAirmarList->Items[i];
			AA.add(pAirmar->Direction);							// Direction, Degree

			if ( i == 0 ) WindGust = pAirmar->Wind;				// WindGust Init

			STemperature += pAirmar->Temperature;
			SHumidity    += pAirmar->Humidity;
			SPressure    += pAirmar->Pressure;
			SWind        += pAirmar->Wind;

			if ( WindGust < pAirmar->Wind ) WindGust = pAirmar->Wind;	// WindGust update

		}

		AirmarData.Wind         = SWind / Count;	    // 풍속(3.5m)
		AirmarData.Wind         = AirmarData.Wind * 0.514444; 	// Knote => meter/sec
		//AirmarData.WindUnit;                          // K/M/N/S => Km/hr, m/s, knots, status miles/hr => default: N(knots)
		AirmarData.WindGust     = WindGust;		        // 최대풍속(Wind 중 최대 풍속) => Complete에서 제외
		AirmarData.WindGust     = AirmarData.WindGust * 0.514444; 	// Knote => meter/sec
		AirmarData.Direction    = AA.getAverage();		// 풍향(0~360)
		//AirmarData.DirectionType;                     // 풍향종류(R/T=>Relative/Theoretical)
		AirmarData.Temperature  = STemperature / Count;	// 온도
		//AirmarData.TemperatureUnit;	                // 온도 단위(C)
		AirmarData.Humidity     = SHumidity / Count;    // 습도
		AirmarData.Pressure     = SPressure / Count ;   // 기압
		//AirmarData.PressureUnit;                      // 기압 단위(B:Bar)
		//Complete                = 0xFF;	                //  1111 1000, 풍속-풍향-온도-습도, 기압

	//------------------------------------------------------------------------    
    } else {	// no record 
    	// All is Zero
		memset(&AirmarData, 0x00, sizeof(STAirmarData));	// Airmar Data Clear 
    }	
    
	bResult = true;
	
	return bResult;
}

//---------------------------------------------------------------------------
// Caculate WindGust .. 3 Step 
double TAirmar::CaculateWindGust()			
{
	double WindGust = 0.0f;
	
	// Caculate WindGust
	
	return WindGust;
}

//---------------------------------------------------------------------------
// Get Airmar Result data
STAirmarData TAirmar::GetAirmarData()
{
	return AirmarData;
}
	
//---------------------------------------------------------------------------
// Receive
void TAirmar::LogMessage(UnicodeString Message)
{
	if ( FormMain != NULL ) {
		FormMain->LogMessage(Message);
	}
}

#pragma package(smart_init)
