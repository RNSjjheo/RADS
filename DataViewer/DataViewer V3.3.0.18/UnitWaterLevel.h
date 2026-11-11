//---------------------------------------------------------------------------

#ifndef UnitWaterLevelH
#define UnitWaterLevelH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Buttons.hpp>
#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>

#include "DvEnv.h"

//---------------------------------------------------------------------------
class TFormWaterLevel : public TForm
{
__published:	// IDE-managed Components
    TImage *Image;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
    TPanel *pnlAvg;
    TPanel *pnlMin;
    TPanel *pnlMax;
	TTimer *Timer;
	TImageList *ImageList;
    TSimpleDataSet *query;
    TButton *btnPrior;
	TSpeedButton *SpeedButtonDay;
	TSpeedButton *SpeedButtonWeek;
	TSpeedButton *SpeedButtonMonth;
	TButton *btnNext;
    TLabel *lblDisplay;
	TChart *Chart;
	TStaticText *StaticText1;
	TAreaSeries *Series1;
	void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SpeedButtonDayClick(TObject *Sender);
    void __fastcall SpeedButtonMonthClick(TObject *Sender);
    void __fastcall btnPriorClick(TObject *Sender);
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall ChartGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
          int ValueIndex, UnicodeString &LabelText);
    void __fastcall SpeedButtonWeekClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations

	EN_QUERY_TYPE m_enQueryType;

	TDateTime m_FromTime;
	TDateTime m_ToTime;

	TDateTime DateDay;
	TDateTime DateWeek;
	TDateTime DateMonth;

	void ShowPeriod();
	void DoQuery();

public:		// User declarations

	EN_SENSOR_STATUS m_enWaterLevel;

	void LoggingStart();
	void LoggingContinue(EN_SENSOR_STATUS enWaterLevel);
	void LoggingFinish();

	void ForceQuery();
	__fastcall TFormWaterLevel(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormWaterLevel *FormWaterLevel;
//---------------------------------------------------------------------------
#endif
