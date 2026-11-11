//---------------------------------------------------------------------------

#ifndef UnitDMH
#define UnitDMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBClient.hpp>
#include <FMTBcd.hpp>
#include <SimpleDS.hpp>
#include <SqlExpr.hpp>
#include <WideStrings.hpp>
#include <DBXMySql.hpp>
#include <Data.DBXFirebird.hpp>
//---------------------------------------------------------------------------
class TDM : public TDataModule
{
__published:	// IDE-managed Components
	TSQLConnection *SQLConnection;
	TSQLQuery *SQLQuery1;
	TSQLQuery *SQLQuery2;
	TSQLQuery *SQLQuery3;
	TSQLQuery *SQLQuery4;
	TSQLQuery *SQLQuery5;
	TSQLStoredProc *SQLStoredProc1;
	TSQLStoredProc *SQLStoredProc2;
	TSQLStoredProc *SQLStoredProc3;
	TSQLStoredProc *SQLStoredProc4;
	TSQLStoredProc *SQLStoredProc5;
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall DataModuleDestroy(TObject *Sender);
	void __fastcall SQLConnectionAfterConnect(TObject *Sender);
	void __fastcall SQLConnectionAfterDisconnect(TObject *Sender);

private:			// User declarations

	bool	     	m_bOpen;			// DB Open Flag
	TQueue	       *m_pQueryList;		// Query object container
	TQueue	       *m_pProcList;		// Procedure object container

public:			    // User declarations

	bool            Open(UnicodeString sDBServer, UnicodeString sDBName, UnicodeString sAccount, UnicodeString sPassword);
	bool		    Close();
	bool		    IsOpen();							// DB Open Flag
	TSQLQuery  	   *GetQuery();							// return TSQLQuery object
	TSQLQuery  	   *GetResultSet(UnicodeString sSQL);	// Get Result Set
	int 	        ExecSQL(UnicodeString sSQL);		// Execute SQL command(INSERT, UPDATE, DELETE )
	void 		    Release(TSQLQuery *pQuery);
	int			    GetFreeQueryCount();
	bool		    IsFreeQuery();

	TSQLStoredProc *GetProc();							// return TSQLStoredProcedure object
	void 		    ReleaseProc(TSQLStoredProc *pProc);
	TDateTime 	    GetCurrentDateTime();				// Get system date time in DB
	void 		    ErrorMessage(UnicodeString sMsg);
	void            ErrorMessageFile(UnicodeString sMsg);

	__fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDM *DM;
//---------------------------------------------------------------------------
#endif
