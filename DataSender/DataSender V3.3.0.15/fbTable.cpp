//---------------------------------------------------------------------------
#pragma hdrstop

#include "fbTable.h"
#include "LockeCommon.h"
//#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TfbTable::TfbTable(TSQLConnection *Connection, UnicodeString sTableName, TDataSource *dsDataSource)
{
    static int iObjectCnt = 0;
    iObjectCnt++;

	TableName = sTableName.UpperCase();

    SelectQr = new TSimpleDataSet(Connection->Owner);
  	SelectQr->Connection = Connection;
    if (dsDataSource != NULL) dsDataSource->DataSet = SelectQr;

    ExtraQr = new TSQLQuery(Connection->Owner);
    ExtraQr->SQLConnection = Connection;

	SqlText = "";
    slWhereParams = new TStringList;
    slFieldParams = new TStringList;
    slOrderParams = new TStringList;
}
//---------------------------------------------------------------------------
TfbTable::~TfbTable()
{
    delete slOrderParams;
    delete slWhereParams;
    delete slFieldParams;
	if (SelectQr->Active) SelectQr->Close();
    delete SelectQr;
    if (ExtraQr->Active) ExtraQr->Close();
    delete ExtraQr;
}
//---------------------------------------------------------------------------
void TfbTable::ClearParams()
{
    slWhereParams->Clear();
    slFieldParams->Clear();
    slOrderParams->Clear();
}
//---------------------------------------------------------------------------
void TfbTable::SetWhereParam(UnicodeString sParamName, UnicodeString sParamValue, TWhereKind wKind)
//where조건 파람 추가
{
    UnicodeString sOperator = "";
    UnicodeString sWhere = "";
    switch(wKind)
    {
        case wkEqual:
            sWhere = sParamName.UpperCase() + " = '" + sParamValue + "'";
            break;
        case wkLessOrEqual:
            sWhere = sParamName.UpperCase() + " <= '" + sParamValue + "'";
            break;
        case wkBiggerOrEqual:
            sWhere = sParamName.UpperCase() + " >= '" + sParamValue + "'";
            break;
        case wkNotEqual:
            sWhere = sParamName.UpperCase() + " <> '" + sParamValue + "'";
            break;
        case wkIsNull:
            sWhere = sParamName.UpperCase() + " IS NULL";
            break;
        case wkIsNotNull:
            sWhere = sParamName.UpperCase() + " IS NOT NULL";
            break;
    } // switch
    slWhereParams->Add(sWhere);
}
//---------------------------------------------------------------------------
void TfbTable::SetFieldParam(UnicodeString sParamName, UnicodeString sParamValue)
//where조건 파람 추가
{
    int idx = slFieldParams->IndexOfName(sParamName);
    if (idx == -1) slFieldParams->Add(sParamName.UpperCase() + "=" + sParamValue);
    else           slFieldParams->Strings[idx] = sParamName.UpperCase() + "=" + sParamValue;
}
//---------------------------------------------------------------------------
void TfbTable::SetOrderParam(UnicodeString sParamName, TOrderKind oKind)
//select의 order by절 추가
{
    UnicodeString sParamValue = (oKind == okDesc) ? "DESC" : "";
    int idx = slOrderParams->IndexOfName(sParamName);
    if (idx == -1) slOrderParams->Add(sParamName.UpperCase() + "=" + sParamValue);
    else           slOrderParams->Strings[idx] = sParamName.UpperCase() + "=" + sParamValue;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::Open()
{
	UnicodeString sError = "";

	if (SelectQr->Active) {
		try {
			SelectQr->Close();
		} catch ( Exception &e ) {
			sError = e.Message;
			return sError;
		}
	}

    UnicodeString sSelectHeader = "SELECT * \n"
                                  "FROM   " + TableName + "\n" +
                                  "WHERE 1 = 1 \n";
    SelectQr->DataSet->CommandText = sSelectHeader;

    //where절 처리
    for(int i = 0; i < slWhereParams->Count; i++)
    {
        UnicodeString sItem = slWhereParams->Strings[i];
        SelectQr->DataSet->CommandText = SelectQr->DataSet->CommandText +
                          "AND " + sItem + " \n";
    } // for i
    //order by절 처리
    if (slOrderParams->Count > 0)
    {
        SelectQr->DataSet->CommandText = SelectQr->DataSet->CommandText +
                          "ORDER BY ";
        for(int i = 0; i < slOrderParams->Count; i++)
        {
            UnicodeString sItem = slOrderParams->Strings[i];
			UnicodeString sName = GetSepItem(sItem, 0, "=");
            UnicodeString sOrderKind = GetSepItem(sItem, 1, "=");
            if (i > 0) SelectQr->DataSet->CommandText = SelectQr->DataSet->CommandText + ", ";
            SelectQr->DataSet->CommandText = SelectQr->DataSet->CommandText + " " + sName + " " + sOrderKind;
        } // for i
    } // if
  	SqlText = SelectQr->DataSet->CommandText;

    try
    {
        SelectQr->Open();
    } // try
    catch(Exception &e)
    {
        sError = e.Message;
    } // catch

    return sError;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::ValueByName(UnicodeString sFieldName)
{
    UnicodeString sFieldValue = "";

    try
    {
        sFieldValue = SelectQr->FieldByName(sFieldName)->AsString;
    } // try
    catch(Exception &e)
    {
    } // catch

    return sFieldValue;
}
//---------------------------------------------------------------------------
// heaven2 2022.12.15
int   TfbTable::IntByNameDef(UnicodeString sFieldName, int iDefault)
{
	int iResult = 0;

	try
	{
		UnicodeString sValue = "";
		sValue = SelectQr->FieldByName(sFieldName)->AsString;
		if ( sValue.IsEmpty() ) {
			iResult = iDefault;
		} else {
			//fResult = StrToFloat(SelectQr->FieldByName(sFieldName)->AsString);
			iResult = StrToInt(sValue);
		}
	} catch(...)
	{
		iResult = iDefault;
	} // catch

	return iResult;

}
//---------------------------------------------------------------------------
float TfbTable::FloatByNameDef(UnicodeString sFieldName, float fDefault)
{
	float fResult = 0.0f;

	try
	{
		UnicodeString sValue = "";
		sValue = SelectQr->FieldByName(sFieldName)->AsString;
		if ( sValue.IsEmpty() ) {
			fResult = fDefault;
		} else {
			//fResult = StrToFloat(SelectQr->FieldByName(sFieldName)->AsString);
			fResult = StrToFloat(sValue);
		}
	} catch(...)
	{
		fResult = fDefault;
	} // catch

	return fResult;
}
//---------------------------------------------------------------------------
bool TfbTable::Exists()
{
	bool bResult = false;

	UnicodeString sQrSelect = "SELECT ";
	UnicodeString sQrWhere  = "WHERE  ";

	for(int i = 0; i < slWhereParams->Count; i++)
	{
		UnicodeString sItem = slWhereParams->Strings[i];
		UnicodeString sFieldName = GetSepItem(sItem, 0, "=");
		if (i > 0)
		{
			sQrSelect += ", ";
			sQrWhere += "AND ";
		} // if
		sQrSelect += sFieldName + " ";
		sQrWhere  += sItem + " \n";
	} // for i

    SqlText = "";
    SqlText += sQrSelect + "\n";
    SqlText += "FROM " + TableName + "\n";
    SqlText += sQrWhere;

    ExtraQr->SQL->Text = SqlText;
	try
	{
        ExtraQr->Open();
    	bResult = !(ExtraQr->Eof && ExtraQr->Bof);
    } // try
    catch(Exception &e)
    {
    } // catch

	return bResult;
}
//---------------------------------------------------------------------------
TSimpleDataSet* TfbTable::DataSet()
{
    return SelectQr;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::Insert()
{
	UnicodeString sErrorMessage = "";

	UnicodeString sFieldNames = "";
	UnicodeString sValues = "";

	for(int i = 0; i < slFieldParams->Count; i++)
	{
		UnicodeString sItem = slFieldParams->Strings[i];
		UnicodeString sFieldName = GetSepItem(sItem, 0, "=");
        int iPos = sItem.Pos("=");
		UnicodeString sValue = sItem.SubString(iPos + 1, sItem.Length() - iPos);
		if (i > 0)
		{
			sFieldNames += ", ";
			sValues += ", ";
		} // if
		sFieldNames += sFieldName;
		sValues += "'" + sValue + "'";
	} // for i

    SqlText = "";
    SqlText += "INSERT INTO " + TableName + "(" + sFieldNames + ") \n";
    SqlText += "VALUES (" + sValues + ") \n";

    ExtraQr->SQL->Text = SqlText;

	try
	{
        ExtraQr->ExecSQL();
	} // try
	catch(Exception &e)
	{
        sErrorMessage = e.Message;
	} // catch

	return sErrorMessage;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::Update()
{
	UnicodeString sErrorMessage = "";

	UnicodeString sSet = "";
	for(int i = 0; i < slFieldParams->Count; i++)
	{
		UnicodeString sItem = slFieldParams->Strings[i];
		UnicodeString sFieldName = GetSepItem(sItem, 0, "=");
        int iPos = sItem.Pos("=");
		UnicodeString sValue = sItem.SubString(iPos + 1, sItem.Length() - iPos);

        sSet += sFieldName + " = '" + sValue + "' ";
        if (i < slFieldParams->Count - 1) sSet += ", \n";
	} // for i

	UnicodeString sWhere = "";
	for(int i = 0; i < slWhereParams->Count; i++)
	{
		UnicodeString sItem = slWhereParams->Strings[i];
		if (i == 0) sWhere += "WHERE ";
        else        sWhere += "AND   ";
		sWhere += sItem + " \n";
	} // for i

	SqlText = "";
	SqlText += "UPDATE " + TableName + "\n";
	SqlText += "SET \n";
	SqlText += sSet + "\n";
	SqlText += sWhere;

    ExtraQr->SQL->Text = SqlText;
	try
	{
		ExtraQr->ExecSQL();
	} // try
	catch(Exception &e)
    {
        sErrorMessage = e.Message;
	} // catch

	return sErrorMessage;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::Delete()
{
	UnicodeString sErrorMessage = "";

	UnicodeString sWhere = "";
	for(int i = 0; i < slWhereParams->Count; i++)
	{
		UnicodeString sItem = slWhereParams->Strings[i];
		if (i == 0) sWhere += "WHERE ";
        else        sWhere += "AND   ";
		sWhere += sItem + " \n";
	} // for i

    SqlText = "";
    SqlText += "DELETE FROM " + TableName + "\n";
    SqlText += sWhere;

    ExtraQr->SQL->Text = SqlText;
	try
	{
        ExtraQr->ExecSQL();
	} // try
	catch(Exception &e)
	{
		sErrorMessage = e.Message;
	} // catch

	return sErrorMessage;
}
//---------------------------------------------------------------------------
UnicodeString TfbTable::ExecSQL(UnicodeString sDML)
{
	UnicodeString sErrorMessage = "";

	SqlText = sDML;
    ExtraQr->SQL->Text = SqlText;
	try
	{
		ExtraQr->ExecSQL();
	} // try
	catch(Exception &e)
	{
		sErrorMessage = e.Message;
	} // catch

	return sErrorMessage;
}
//---------------------------------------------------------------------------
TField* TfbTable::FieldByName(UnicodeString sFieldName)
{
    return SelectQr->FieldByName(sFieldName);
}
//---------------------------------------------------------------------------


