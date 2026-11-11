//---------------------------------------------------------------------------

#ifndef UnitUserH
#define UnitUserH
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
#include <Data.DBXFirebird.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TFormUser : public TForm
{
__published:	// IDE-managed Components
    TEdit *edPw;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TButton *btnClear;
    TButton *btnPost;
    TButton *btnDelete;
    TDBGrid *DBGrid;
    TButton *btnClose;
    TDataSource *DataSource;
    TLabel *Label1;
    TSimpleDataSet *SimpleDataSet1;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label2;
    TPanel *pnlPrivilege;
    TEdit *edUserId;
    TEdit *edPassword;
    TComboBox *cbxCompany;
    TEdit *edPos;
    TEdit *edUserName;
    TEdit *edPhone;
    TRadioGroup *rbtnPrivilege;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall btnPostClick(TObject *Sender);
    void __fastcall DBGridCellClick(TColumn *Column);
    void __fastcall btnDeleteClick(TObject *Sender);
    void __fastcall SimpleDataSet1AfterScroll(TDataSet *DataSet);
    void __fastcall SimpleDataSet1BeforePost(TDataSet *DataSet);
private:	// User declarations
    bool bRealPost;
    void ShowToControls(); //db의 값을 관련 컨트롤에 표시
    void ClearControls();
    void EnableKeyControls(bool bEnable);
    void Open();
    void GetComboKind();
public:		// User declarations
    __fastcall TFormUser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUser *FormUser;
//---------------------------------------------------------------------------
#endif
