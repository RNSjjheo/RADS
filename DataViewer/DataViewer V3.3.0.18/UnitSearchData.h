//---------------------------------------------------------------------------

#ifndef UnitSearchDataH
#define UnitSearchDataH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>
#include <Data.DBXFirebird.hpp>
#include <Vcl.Samples.Spin.hpp>
#include "cspin.h"

#include "DvEnv.h"
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TFormSearchData : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TPanel *pnl2;
    TSpeedButton *sbtnDay;
    TSpeedButton *sbtnWeek;
    TSpeedButton *sbtnMonth;
    TLabel *lblDisplay;
    TButton *btnPrior;
	TButton *btnNext;
    TSimpleDataSet *query;
    TDataSource *DataSource1;
    TPageControl *PageControl2;
	TTabSheet *tsFlowStream;
	TTabSheet *tsHydroMeter1;
	TTabSheet *tsHydroMeter2;
	TTabSheet *tsWaterLevel;
    TPanel *pnl1;
    TSimpleDataSet *queryMaster;
    TLabel *Label1;
    TLabel *Label2;
    TDateTimePicker *DateTimePicker1;
    TDateTimePicker *DateTimePicker2;
	TButton *cbnQuery;
    TPanel *Panel1;
    TDBGrid *DBGrid;
    TTabSheet *TabSheet9;
	TTabSheet *tsVTHLogger;
    TSpinEdit *se1Minute;
    TSpinEdit *se1Hour;
    TSpinEdit *se2Hour;
    TSpinEdit *se2Minute;
	TChart *ChartWaterLevel;
	TStaticText *StaticText1;
	TAreaSeries *AreaSeriesWaterLevel;
	TChart *ChartHydroMeter2;
	TStaticText *stHydro2Y;
	TAreaSeries *AreaSeriesHydroMeter2;
	TChart *ChartHydroMeter1;
	TStaticText *stHydro1Y;
	TAreaSeries *AreaSeriesHydroMeter1;
	TChart *ChartFlowStream;
	TStaticText *StaticText4;
	TAreaSeries *SeriesFlowStream;
	TChart *ChartVTHLogger;
	TStaticText *StaticText5;
	TStaticText *StaticText6;
	TLineSeries *SeriesDCCharge;
	TLineSeries *SeriesDCBattery;
	TLineSeries *SeriesTemperature;
	TStaticText *StaticText7;
	TButton *cbnExport;
	TSaveDialog *SaveDialog;
	TTabSheet *TabSheet5;
	TTabSheet *tsHydroMeter3;
	TChart *ChartHydroMeter3;
	TStaticText *stHydro3Y;
	TAreaSeries *AreaSeriesHydroMeter3;
	TAreaSeries *AreaSeriesWindSpeed;
	TAreaSeries *AreaSeriesWindGust;
	TRadioButton *rbH1Velocity;
	TRadioButton *rbH1WaterLevel;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall btnPriorClick(TObject *Sender);
	void __fastcall sbtnDayClick(TObject *Sender);
	void __fastcall sbtnWeekClick(TObject *Sender);
	void __fastcall sbtnMonthClick(TObject *Sender);
	void __fastcall btnNextClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall cbnQueryClick(TObject *Sender);
	void __fastcall PageControl2Change(TObject *Sender);
	void __fastcall DBGridCellClick(TColumn *Column);
	void __fastcall queryMasterFieldGetText(TField *Sender, UnicodeString &Text,
		  bool DisplayText);
	void __fastcall ChartHydroMeter1ClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
		  int Y);
	void __fastcall ChartHydroMeter2ClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
		  int Y);
	void __fastcall ChartWaterLevelGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
		  int ValueIndex, UnicodeString &LabelText);
	void __fastcall cbnExportClick(TObject *Sender);
	void __fastcall ChartHydroMeter3ClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
          int Y);
	void __fastcall rbH1VelocityClick(TObject *Sender);
	void __fastcall rbH1WaterLevelClick(TObject *Sender);
private:	// User declarations

	EN_QUERY_TYPE m_enQueryType;

	UnicodeString sAvg, sMax, sMin;
	UnicodeString sHydroKind;

	bool m_bRQ30Flag;   	// device RQ30 or not

	TDateTime m_ToTime;
	TDateTime m_FromTime;

	TDateTime DateHome;
	TDateTime DateDay;
    TDateTime DateWeek;
	TDateTime DateMonth;

	UnicodeString GetDeviceFields(int iDeviceKind);

	void DoQuery();				// Detail Graph를 조회한다.
	void ShowPeriod();

	void DoQueryHydro1Main();	// 유속1 Grid column, width
	void DoQueryHydro2Main(); 	// 유속2 Grid column, width
	void DoQueryHydro3Main(); 	// 유속3 Grid column, width
	void DoQueryWaterMain();   	// 수위계 Grid column, width
	void DoQueryVTHMain(); 		// VTH Grid column, width
	void DoQueryFlowMain(); 	// 유량 Grid column, Width

	void DoQueryHydro1Graph();  // 유속1 Graph
	void DoQueryHydro1WaterGraph();  // 유속1 Water Graph
	void DoQueryHydro2Graph();	// 유속2 Graph
	void DoQueryHydro3Graph();	// 유속2 Graph
	void DoQueryWaterGraph();	// 수위계 Graph
	void DoQueryVTHGraph();     // VTH Graph
	void DoQueryFlowGraph();    // 유량 그래프

public:		// User declarations
	__fastcall TFormSearchData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSearchData *FormSearchData;
//---------------------------------------------------------------------------
#endif
