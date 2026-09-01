//---------------------------------------------------------------------------

#ifndef HydroMeterH
#define HydroMeterH
//---------------------------------------------------------------------------

#include "../Define.h"
#include "../UnitMain.h"
#include "../Database/RHydroMeter.h"
#include "../Database/RHydroMeterCell.h"
#include "../Database/RFlowIVM.h"
#include "../Database/RFlowHA.h"
#include "../Database/RFlowADVM.h"

#include "AdPort.hpp"
#include "OoMisc.hpp"

class THydroMeter
{
public:		// Attribute
	EN_HYDRO_NO    m_enHydroNo;				// 하층유속계(true), 상층유속계(false)
	char           m_Buffer[BUFFER_8K];
	bool           m_bBuffering;			// Buffering or not
	AnsiString     m_RecvMsg;				// Received Message

	TApdComPort   *m_pPort;				// ComPort
	EN_SENSOR_STEP m_enStep;			// Sensor Status ( idle-start-working-finish )
	EN_SENSOR_STEP m_enWorkingStep;		// 2014.09.17 Sensor Working Status (idle-start-finish-start-finish-...)

	bool           m_bBreak;			// Break Status or not
	bool           m_bBreakResponse;	// Break Response receive or not
	bool           m_bFlooding;			// 수신 데이터가 flooding(홍수났을 때) 과도한 로그 막는다.
	int            m_iThrowCount;		// 수신돼었지만 버려지는 횟수

	UnicodeString  MeasureDate;
	UnicodeString  MeasureTime;

	UnicodeString  HydroKind;	// HydorMeter Type : ChannelMaster, AG Series , SL Series

	TDateTime      m_TimeStamp;			// 마지막으로 동작한 시간의 TimeStamp
//	bool           m_bIdleCheck;			// idle pc reboot option =>
//	int            m_iIdleCheckTime;		// default : 180 minutes

	TRHydroMeter   m_RHydroMeter;
	//TRHydroMeterCell m_RHydroMeterCell;
	TRHydroMeterCellContainer m_RHydroCellContainer;

	TRFlowIVM     *m_pRFlowIVM;
	TRFlowHA      *m_pRFlowHA;
	TRFlowADVM    *m_pRFlowADVM;

	TList         *m_pVelocityList;

public:		// Method
	THydroMeter();
	virtual ~THydroMeter();
	virtual void   Clear() = 0;
	virtual void   Init(TApdComPort *pPort, EN_HYDRO_NO enHydroNo) = 0;
	virtual void   SetCellInfo(int CellBegin, int CellEnd, int CellNo, int CellSize, int CellDistance) = 0;	// AG/SL Series는 반드시 초기 데이터로 지정해야 함.
	virtual bool   Start(UnicodeString MeasureDate, UnicodeString MeasureTime) = 0;
	virtual bool   Finish() = 0;
	virtual bool   Decode(AnsiString RecvMsg, int iLen) = 0;
	virtual bool   Decode(BYTE *pBuffer, int iLen) = 0;
	virtual bool   Receive(AnsiString RecvMsg, int iLen) = 0;
	virtual bool   Receive(BYTE *pBuffer, int iLen) = 0;
	virtual bool   Send(AnsiString RecvMsg, int iLen) = 0;
	virtual bool   Save(UnicodeString MeasureDate, UnicodeString MeasureTime) = 0;
	virtual void   DB2Value() = 0;
	virtual void   Value2DB() = 0;
	virtual double GetAvgVelocity() = 0;

	virtual void   SetBreak(bool bBreak) = 0;
	virtual bool   GetBreakStatus() = 0;
	virtual void   SendBreak(int iMiliSecond) = 0;	// Send <BREAK> to ComPort

	void InitFlowStream(TRFlowIVM *pRFlowIVM, TRFlowHA *pRFlowHA, TRFlowADVM *pRFlowADVM);
	EN_SENSOR_STEP GetStatus();
	void LogMessage(UnicodeString sMsg, bool bFile=true);
};


#endif
