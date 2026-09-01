//---------------------------------------------------------------------------

#ifndef AniThreadH
#define AniThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

#include "Define.h"

#define HYDROMETER_NONE	         0
#define HYDROMETER_CHANNELMASTER 1
#define HYDROMETER_SLAG          2
#define HYDROMETER_ACCUSONIC     3

class TAniThread : public TThread
{
private:
public:
	EN_FLOWSTREAM_ICON m_FlowStreamIndex;
	EN_WATERLEVEL_ICON m_WaterLevelIndex;
	EN_HYDROMETER_ICON m_HydroMeter1Index;
	EN_HYDROMETER_ICON m_HydroMeter2Index;

	bool m_bFlowStream;
	bool m_bHydroMeter1;
	bool m_bHydroMeter2;
	bool m_bWaterLevel;
	int  m_iHydroKind1;	// 0:None, 1: ChannelMaster, 2:AG/SL
	int  m_iHydroKind2;	// 0:None, 1: ChannelMaster, 2:AG/SL

protected:
	void __fastcall Execute();
	void __fastcall Animate();

public:
	__fastcall TAniThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
