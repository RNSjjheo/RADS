//---------------------------------------------------------------------------

#ifndef UnitReportH
#define UnitReportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Data.FMTBcd.hpp>
#include <Data.SqlExpr.hpp>
#include <Datasnap.DBClient.hpp>
#include <SimpleDS.hpp>
#include <Vcl.ComCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <Data.DBXFirebird.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TFormReport : public TForm
{
__published:	// IDE-managed Components
    TLabeledEdit *ledPointName;
    TButton *btnClear;
    TButton *btnPost;
    TButton *btnDelete;
    TDBGrid *DBGrid;
    TButton *btnClose;
    TDataSource *DataSource;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TDateTimePicker *dtpDate1;
    TLabel *Label14;
    TLabel *Label15;
    TDateTimePicker *dtpDate2;
    TButton *btnOpenQuery;
    TButton *btnSendMail;
    TIdSMTP *IdSMTP;
    TSimpleDataSet *SimpleDataSet1;
    TPanel *pnlCheckFlag;
    TLabel *Label16;
    TLabel *Label17;
    TMemo *mmPorpose;
    TMemo *mmManage;
    TMemo *mmComplement;
    TMemo *mmReplace;
    TMemo *mmEtc;
    TRadioGroup *rbtnCheckFlag;
    TMaskEdit *meCheckDate;
    TEdit *edUserID;
    TEdit *edName;
    TComboBox *cbxDataFormat;
    TComboBox *cbxCompany;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall btnPostClick(TObject *Sender);
    void __fastcall DBGridCellClick(TColumn *Column);
    void __fastcall btnDeleteClick(TObject *Sender);
    void __fastcall CheckFlagGetText(TField* Sender, System::UnicodeString &Text, bool DisplayText);
    void __fastcall btnSendMailClick(TObject *Sender);
    void __fastcall btnOpenQueryClick(TObject *Sender);
    void __fastcall SimpleDataSet1BeforePost(TDataSet *DataSet);
    void __fastcall DataSourceDataChange(TObject *Sender, TField *Field);
    void __fastcall SimpleDataSet1AfterScroll(TDataSet *DataSet);
private:	// User declarations
    bool bRealPost;
    void EnableKeyControls(bool bEnable);
    void Open();
    void GetComboKind();
    void ShowToControls(); //db의 값을 관련 컨트롤에 표시
    void ClearControls();
    UnicodeString SendSmtpMail();
public:		// User declarations
    __fastcall TFormReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReport *FormReport;
//---------------------------------------------------------------------------
#endif
