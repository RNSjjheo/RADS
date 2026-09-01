//---------------------------------------------------------------------------
#ifndef fbTableH
#define fbTableH

#include <vcl.h>
#include <Vcl.Mask.hpp>
#include <Data.SqlExpr.hpp>
#include <SimpleDS.hpp>

//---------------------------------------------------------------------------
enum TWhereKind { wkEqual, wkLessOrEqual, wkBiggerOrEqual, wkNotEqual, wkIsNull, wkIsNotNull };
enum TOrderKind { okAsc, okDesc };
enum TLiveAccessMode { lamGetControl, lamSetControl };
const bool CbKeyOnly = True;
const bool CbAllFields = False;
const UnicodeString CsCtlTypeText = "TEXT";
const UnicodeString CsCtlTypeIndex = "INDEX";
const UnicodeString CsCtlTypeNoMask = "NOMASK";
struct TLiveFindInfo
{
    TControl *ctl;
    UnicodeString sType;
    UnicodeString sTableName;
    UnicodeString sFieldName;
    UnicodeString sValue;
    bool bIsKeyFIeld;
};
//---------------------------------------------------------------------------
class TfbTable
{
private:
protected:
	UnicodeString TableName;

	TSimpleDataSet *SelectQr;
    TStringList *slWhereParams;
    TStringList *slFieldParams;
    TStringList *slOrderParams;

	TSQLQuery *ExtraQr;
public:
	TfbTable(TSQLConnection *Connection, UnicodeString sTableName, TDataSource *dsDataSource = NULL);
	virtual ~TfbTable();
	UnicodeString ValueByName(UnicodeString sFieldName);
	int   IntByNameDef(UnicodeString sFieldName, int iDefault);
    float FloatByNameDef(UnicodeString sFieldName, float fDefault);
	UnicodeString SqlText; //최근 select나 DML액션의 쿼리문 저장
    TSimpleDataSet* DataSet();
    void ClearParams();
    void SetWhereParam(UnicodeString sParamName, UnicodeString sParamValue, TWhereKind wKind = wkEqual); //where조건 파람 추가
    void SetFieldParam(UnicodeString sParamName, UnicodeString sParamValue); //필드값 추가
    void SetOrderParam(UnicodeString sParamName, TOrderKind oKind = okAsc); //select의 order by절 추가
	UnicodeString Open(); //파라미터로 자료 오픈
	UnicodeString Insert(); //필드값으로 insert
    UnicodeString Update(); //필드값을 where 파라미터로 update
    UnicodeString Delete(); //where 파라미터로 delete
	bool Exists(); //where 파라미터의 record가 있는지 검사
    UnicodeString ExecSQL(UnicodeString sDML);
    TField* FieldByName(UnicodeString sFieldName);
};
//---------------------------------------------------------------------------
#endif
