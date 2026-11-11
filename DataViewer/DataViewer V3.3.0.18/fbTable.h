//---------------------------------------------------------------------------
#include <vcl.h>
#include <Vcl.Mask.hpp>
#include <Data.SqlExpr.hpp>
#include <SimpleDS.hpp>

#ifndef fbTableH
#define fbTableH
//---------------------------------------------------------------------------
/*
    control의 hint 스크립트 구조
    <아이템1>|[<아이템2>]

    <아이템1> = <table명>.<필드명>[<*: 키필드일 경우 포함>]
    예: rsetup.pk1*
        rsetup.value01

    <아이템2> = 특정 컨트롤의 io 타입(콤보박스, maskedit, radio button)
        TEXT  = 값을 TComboBox의 text에 대입
        INDEX = 값을 TComboBox의 ItemIndex에 대입

        TEXT = 값을 TMaskEdit, TDateTimePicker의 EditText에 대입하거나 가져옴
        NOMASK = 값을 TMaskEdit, TDateTimePicker의 Text에 대입하거나 가져옴(mask 제외)

        <VALUE> =해당 radiobutton이 checked 상태이면 db에 <VALUE>로 정한 값이 저장됨

        <CHECKED_VALUE>,<UNCHECKED_VALUE> =컨트롤이 checkbox일 경우 설정

        그외는 생략가능(생략시 text)

    <종합 예>
        rsetup.pk1*
        rsetup.pk2*|text
        rsetup.pk3
        rsetup.pk4|index
        rsetup.pk5|text
        rsetup.pk5|nomask
        user.priv|N <-해당 radiobutton checked이면 db에 N을 저장(default 사용자)
        user.priv|S <-해당 radiobutton checked이면 db에 S를 저장(super 사용자)
        rsetup.active|ON,OFF <-체크박스 필드값 정의
        rsetup.active|1,     <-체크박스 필드값 정의(checked일 때만 1)
        ...
*/
//---------------------------------------------------------------------------
enum TWhereKind { wkEqual, wkLessOrEqual, wkBiggerOrEqual, wkNotEqual, wkIsNull, wkIsNotNull, wkInStr, wkInNumber };
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
	~TfbTable();
	UnicodeString ValueByName(UnicodeString sFieldName);
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
