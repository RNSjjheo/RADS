//---------------------------------------------------------------------------

#ifndef UnitHydroMeterH
#define UnitHydroMeterH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Buttons.hpp>
#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>

#include "DvEnv.h"
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class TFormHydroMeter : public TForm
{
__published:	// IDE-managed Components
	TImage *Image;
    TPanel *pnlAvg;
    TPanel *pnlMin;
    TPanel *pnlMax;
	TTimer *Timer;
    TImageList *ImageListCM;
    TImageList *ImageListSontek;
    TSpeedButton *sbtnDay;
    TSpeedButton *sbtnWeek;
    TSpeedButton *sbtnMonth;
    TButton *btnPrior;
    TButton *Button5;
    TSimpleDataSet *query;
    TLabel *lblDisplay;
	TImage *Image2;
	TPanel *pnlMin2;
	TPanel *pnlAvg2;
	TPanel *pnlMax2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TChart *Chart1;
	TStaticText *StaticTextUnit;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TChart *Chart2;
	TStaticText *StaticText1;
	TLineSeries *LineSeries1;
	TLineSeries *LineSeries2;
	TChart *Chart3;
	TStaticText *StaticText2;
	TLineSeries *LineSeries3;
	TLineSeries *LineSeries4;
	TImage *Image1;
	TPanel *pnlMin3;
	TPanel *pnlAvg3;
	TPanel *pnlMax3;
	void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbtnDayClick(TObject *Sender);
    void __fastcall sbtnWeekClick(TObject *Sender);
    void __fastcall sbtnMonthClick(TObject *Sender);
    void __fastcall btnPriorClick(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Chart1GetAxisLabel(TChartAxis *Sender, TChartSeries *Series, int ValueIndex,
          UnicodeString &LabelText);
    void __fastcall Chart1ClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
          int Y);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations

	EN_QUERY_TYPE m_enQueryType;

	TDateTime DateDay;
	TDateTime DateWeek;
	TDateTime DateMonth;

	TDateTime m_FromTime;
	TDateTime m_ToTime;

	UnicodeString sHydroKind, sHydroKind2;
	bool m_bRQ30Flag;

	void ShowPeriod();
	void DoQuery();
	void DoQueryHydro1();
	void DoQueryHydro2();
	void DoQueryHydro3();

public:		// User declarations

	EN_SENSOR_STATUS m_enHydro1;	// Sensor1 Working Status
	EN_SENSOR_STATUS m_enHydro2;	// Sensor2 Working Status

	void LoggingStart();
	void LoggingContinue(EN_SENSOR_STATUS enHydro1, EN_SENSOR_STATUS enHydro2);
	void LoggingFinish();

	void ForceQuery();
	__fastcall TFormHydroMeter(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TFormHydroMeter *FormHydroMeter;
//---------------------------------------------------------------------------
#endif
