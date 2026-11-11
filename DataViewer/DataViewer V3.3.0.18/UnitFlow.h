//---------------------------------------------------------------------------

#ifndef UnitFlowH
#define UnitFlowH
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
#include <Vcl.ImgList.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>

#include "DvEnv.h"

//---------------------------------------------------------------------------
class TFormFlow : public TForm
{
__published:	// IDE-managed Components
	TImage *Image;
    TButton *btnPrior;
	TButton *ButtonNext;
	TTimer *Timer;
	TImageList *ImageList;
    TPanel *pnlAvg;
	TLabel *Label1;
	TSpeedButton *SpeedButtonDay;
	TSpeedButton *SpeedButtonWeek;
	TSpeedButton *SpeedButtonMonth;
    TSimpleDataSet *query;
    TPanel *pnlMin;
    TPanel *pnlMax;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *lblDisplay;
	TChart *Chart;
	TStaticText *StaticText1;
	TAreaSeries *Series1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SpeedButtonDayClick(TObject *Sender);
    void __fastcall SpeedButtonWeekClick(TObject *Sender);
    void __fastcall SpeedButtonMonthClick(TObject *Sender);
    void __fastcall btnPriorClick(TObject *Sender);
    void __fastcall ButtonNextClick(TObject *Sender);
    void __fastcall ChartGetAxisLabel(TChartAxis *Sender, TChartSeries *Series, int ValueIndex,
          UnicodeString &LabelText);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations

	EN_QUERY_TYPE m_enQueryType;

	TDateTime m_FromTime;
	TDateTime m_ToTime;

    TDateTime dtLate;
    TDateTime DateDay;
    TDateTime DateWeek;
	TDateTime DateMonth;

	void ShowPeriod();
	void DoQuery();

public:		// User declarations

	EN_SENSOR_STATUS m_enFlowStream;

	void LoggingStart();
	void LoggingContinue(EN_SENSOR_STATUS enFlowStream);
	void LoggingFinish();

    void ForceQuery();
	__fastcall TFormFlow(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFlow *FormFlow;
//---------------------------------------------------------------------------
#endif
