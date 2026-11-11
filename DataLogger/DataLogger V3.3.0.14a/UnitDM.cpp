//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"


#include "UnitDM.h"
#include <SyncObjs.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TDM *DM;

TCriticalSection *pCriticalSection = new TCriticalSection();

//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
	: TDataModule(Owner)
{
	m_bOpen = false;	// DB is not opened (default)
}
//---------------------------------------------------------------------------

void __fastcall TDM::DataModuleCreate(TObject *Sender)
{
	m_pQueryList = new TQueue();

	m_pQueryList->Push(SQLQuery1);
	m_pQueryList->Push(SQLQuery2);
	m_pQueryList->Push(SQLQuery3);
	m_pQueryList->Push(SQLQuery4);
	m_pQueryList->Push(SQLQuery5);

	m_pProcList = new TQueue();

	m_pProcList->Push(SQLStoredProc1);
	m_pProcList->Push(SQLStoredProc2);
	m_pProcList->Push(SQLStoredProc3);
	m_pProcList->Push(SQLStoredProc4);
	m_pProcList->Push(SQLStoredProc5);
}
//---------------------------------------------------------------------------

void __fastcall TDM::DataModuleDestroy(TObject *Sender)
{
	// Release Query List
	while (m_pQueryList->Count() > 0) {
		m_pQueryList->Pop();
	}
	delete m_pQueryList;

	// Release Proceduere List
	while (m_pProcList->Count() > 0) {
		m_pProcList->Pop();
	}

	SQLConnection->Connected = false;

	delete m_pProcList;

	if ( !pCriticalSection ) {
		delete pCriticalSection;
		pCriticalSection = NULL;
	}
}

//---------------------------------------------------------------------------
// Event: db opened
void __fastcall TDM::SQLConnectionAfterConnect(TObject *Sender)
{
	m_bOpen = true;
}

//---------------------------------------------------------------------------
// Event: db closed
void __fastcall TDM::SQLConnectionAfterDisconnect(TObject *Sender)
{
	m_bOpen = false;
}

//---------------------------------------------------------------------------
// Open database
bool TDM::Open(UnicodeString sDBServer, UnicodeString sDBName, UnicodeString sAccount, UnicodeString sPassword)
{
	try {
		SQLConnection->Params->Add("Database=" + sDBServer);
		SQLConnection->Params->Add("User_Name=" + sAccount);
		SQLConnection->Params->Add("Password=" + sPassword);
		SQLConnection->Connected = true;
		//m_bOpen = true;
	} catch ( Exception &e ) {
#ifdef _DEBUG
		//ShowMessage(e.Message);
#endif
		m_bOpen = false;
		return false;
	}
	return true;
}

//---------------------------------------------------------------------------
// Close database
bool TDM::Close()
{
	bool bResult = false;
	try {
		SQLConnection->Close();
		bResult = true;
	} catch ( Exception &e) {
#ifdef _DEBUG
		ErrorMessage(e.Message);
#endif
		bResult = false;
	}

	// Release Query List
	while (m_pQueryList->Count() > 0) {
		m_pQueryList->Pop();
	}
	delete m_pQueryList;

	// Release Proceduere List
	while (m_pProcList->Count() > 0) {
		m_pProcList->Pop();
	}
	delete m_pProcList;

	return bResult;
}


//---------------------------------------------------------------------------
// IsOpen?
bool TDM::IsOpen()						// DB Open Flag
{
	return m_bOpen;
}


//---------------------------------------------------------------------------
// Get TSQLQuery object
TSQLQuery *TDM::GetQuery()
{
	if ( !m_bOpen ) {
#ifdef _DEBUG
		//ShowMessage("DB is not open.");
#endif
    	return NULL;
	}

	pCriticalSection->Enter();

	TSQLQuery *pQuery = NULL;
	if ( m_pQueryList->Count() > 0 ) {
		pQuery = (TSQLQuery *) m_pQueryList->Pop();
	} else {
#ifdef _DEBUG
		ShowMessage("Query queue is empty");
#endif
	}

	pCriticalSection->Leave();

	return pQuery;
}


//---------------------------------------------------------------------------
// Get Result set of statement
TSQLQuery *TDM::GetResultSet(UnicodeString sSQL)
{
	if ( !m_bOpen ) {
#ifdef _DEBUG
		//ShowMessage("DB is not open.");
#endif
    	return NULL;
    }

	TSQLQuery *pQuery = NULL;
	pQuery = GetQuery();
	if ( pQuery == NULL ) return NULL;

	try {
		pQuery->Active = false;
		pQuery->SQL->Clear();
		pQuery->SQL->Text = sSQL;
		pQuery->Active = true;
	} catch ( Exception &e ) {
#ifdef _DEBUG
		ErrorMessage(e.Message);
#endif
		Release(pQuery);
		pQuery = NULL;
	}
	return pQuery;
}


//---------------------------------------------------------------------------
// Execute SQL Command  -1: error, 0,1,2....: Ok
int TDM::ExecSQL(UnicodeString sSQL)	// Execute SQL command(INSERT, UPDATE, DELETE )
{
	int iRow = -1;
    if ( !m_bOpen ) {
#ifdef _DEBUG
		//ShowMessage("DB is not open.");
#endif
    	return iRow;
    }

	TSQLQuery *pQuery = NULL;;
	pQuery = GetQuery();
	if ( pQuery == NULL ) return iRow;

	try {
		pQuery->Active = false;
		pQuery->SQL->Clear();
		pQuery->SQL->Text = sSQL;
		iRow = pQuery->ExecSQL();
		Release(pQuery);
	} catch ( Exception &e ) {
#ifdef _DEBUG
		ErrorMessage(e.Message);
#endif
		Release(pQuery);
	}
	return iRow;
}


//---------------------------------------------------------------------------
// Release TSQLQuery object
void TDM::Release(TSQLQuery *pQuery)
{
	pCriticalSection->Enter();

	m_pQueryList->Push(pQuery);

	pCriticalSection->Leave();
}


//---------------------------------------------------------------------------
// Get Free TSQLQuery object
int TDM::GetFreeQueryCount()
{
	return m_pQueryList->Count();
}


//---------------------------------------------------------------------------
// Free TSQLQuery exist or not ?  ( all query object is busy ? )
bool TDM::IsFreeQuery()
{
	return (m_pQueryList->Count()) ? true : false;
}


//---------------------------------------------------------------------------
// Get Stored Procedure object
TSQLStoredProc *TDM::GetProc()		// return TSQLStoredProcedure object
{
	if ( !m_bOpen) {
#ifdef _DEBUG
      	//ShowMessage("DB is not open.");
#endif
    	return NULL;
	}

	pCriticalSection->Enter();

	TSQLStoredProc *pProc = NULL;
	if ( m_pProcList->Count() > 0 ) {
		pProc = (TSQLStoredProc *)m_pProcList->Pop();
	} else {
#ifdef _DEBUG
      	ShowMessage("StoredProcedure queue is empty");
#endif
	}

	pCriticalSection->Leave();

	return pProc;
}


//---------------------------------------------------------------------------
// Release TSQLStoredProc Object
void TDM::ReleaseProc(TSQLStoredProc *pProc)
{
	pCriticalSection->Enter();

	m_pProcList->Push(pProc);

	pCriticalSection->Leave();
}


//---------------------------------------------------------------------------
// Get Current DateTime from database ( default: MySQL )
TDateTime TDM::GetCurrentDateTime()			// Get system date time in DB
{
	TDateTime DateTime = 0;
	if ( !m_bOpen ) return DateTime;

//https://firebirdsql.org/refdocs/langrefupd21-now.html
//select date 'Now' from rdb$database
//select time 'now' from rdb$database
//select timestamp 'NOW' from rdb$database

	UnicodeString sSQL = "SELECT timestamp 'NOW' AS CurrentDateTime FROM rdb$database";

	TSQLQuery *pQuery = NULL;
	pQuery = GetResultSet(sSQL);

	if ( pQuery == NULL ) return DateTime;

	try {
		DateTime = pQuery->FieldByName("CurrentDateTime")->AsDateTime;
	} catch ( Exception &e ) {
#ifdef _DEBUG
		ShowMessage("[TDM::GetCurrentDateTime]" + e.Message);
#endif
	}
	Release(pQuery);

	return DateTime;
}


//---------------------------------------------------------------------------
// Show Error Message in Message Box ( only debug mode )
void TDM::ErrorMessage(UnicodeString sMsg)
{
#ifdef _DEBUG
	ShowMessage(sMsg);
#endif
}


//---------------------------------------------------------------------------
// write error message in debug file ( only debug mode )
void TDM::ErrorMessageFile(UnicodeString sMsg)
{
#ifdef _DEBUG
	ShowMessage(sMsg);
#endif
}

