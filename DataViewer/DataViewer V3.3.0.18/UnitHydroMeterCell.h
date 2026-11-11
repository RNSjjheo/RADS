//---------------------------------------------------------------------------

#ifndef UnitHydroMeterCellH
#define UnitHydroMeterCellH
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
//---------------------------------------------------------------------------
class TFormHydroMeterCell : public TForm
{
__published:	// IDE-managed Components
    TSpeedButton *sbtnDay;
    TSpeedButton *sbtnWeek;
    TSpeedButton *sbtnMonth;
    TButton *btnPrior;
    TButton *Button5;
    TSimpleDataSet *query;
    TLabel *lblDisplay;
	TChart *Chart;
	TStaticText *StaticText2;
	TStaticText *StaticText3;
	TAreaSeries *Series1;
	TAreaSeries *Series2;
	TAreaSeries *Series3;
	TSpeedButton *sbtnMinute;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbtnDayClick(TObject *Sender);
    void __fastcall sbtnWeekClick(TObject *Sender);
    void __fastcall sbtnMonthClick(TObject *Sender);
    void __fastcall btnPriorClick(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sbtnMinuteClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	int           iQueryKind;	// 0: 10Minute 1: Day, 7: Week, 30: Month
	UnicodeString sMeasureDate;
	UnicodeString sMeasureTime;	//
	UnicodeString sHydroKind;	// ChannelMaster, SL Series, AG Series
	EN_HYDRO_NO   enHydroNo;	// 1/2/3번 유속계

	TDateTime     DateHome;		// Day Base
	TDateTime     DateMinute;
	TDateTime     DateDay;
	TDateTime     DateWeek;
	TDateTime     DateMonth;
    void ShowPeriod();
    void DoQuery();
	void DoQueryAccusonic();
	__fastcall TFormHydroMeterCell(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHydroMeterCell *FormHydroMeterCell;
//---------------------------------------------------------------------------
#endif
