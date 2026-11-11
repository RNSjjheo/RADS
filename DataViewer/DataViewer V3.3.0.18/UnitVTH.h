//---------------------------------------------------------------------------

#ifndef UnitVTHH
#define UnitVTHH
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
#include <VCLTee.TeeSpline.hpp>
#include <Data.DB.hpp>
#include <Data.DBXFirebird.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>
#include <Vcl.Buttons.hpp>

#include "DvEnv.h"

//---------------------------------------------------------------------------
class TFormVTH : public TForm
{
__published:	// IDE-managed Components
	TImage *Image;
	TImageList *ImageList;
	TSpeedButton *SpeedButtonDay;
	TSpeedButton *SpeedButtonWeek;
	TSpeedButton *SpeedButtonMonth;
    TButton *btnPrior;
	TButton *btnNext;
    TSimpleDataSet *query;
    TLabel *lblDisplay;
	TChart *Chart;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TStaticText *StaticText1;
	TStaticText *StaticText2;
	void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnPriorClick(TObject *Sender);
    void __fastcall SpeedButtonDayClick(TObject *Sender);
    void __fastcall SpeedButtonWeekClick(TObject *Sender);
    void __fastcall SpeedButtonMonthClick(TObject *Sender);
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall ChartGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
          int ValueIndex, UnicodeString &LabelText);
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
    void ForceQuery();
	__fastcall TFormVTH(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVTH *FormVTH;
//---------------------------------------------------------------------------
#endif
