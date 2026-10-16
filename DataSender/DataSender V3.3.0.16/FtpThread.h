//---------------------------------------------------------------------------
//#include <IdFTP.hpp>
//#pragma hdrstop

#ifndef FtpThreadH
#define FtpThreadH

#include <vcl.h>
#include <IdFTP.hpp>
#include "LockeCommon.h"
#include "fbTable.h"

// 수신서버 Type ( 2024.11.29 수신서버1: 구 수신서버(RIDIMS 고도화 이전 운여), 수신서버2: 신규 수신서버(IRDIMS 고도화 이후 운영)
//				 ( 최종적으로는 신규서버(수신서버2)가 수신서버1이 되며, 구수신서버는 운영되지 않을 것임
#define SERVER_IRDIMS_FIRST		1	// 수신서버1(Old Irdims Server)
#define SERVER_IRDIMS_SECOND	2	// 수신서버2(New Irdims Server)

class Sbsshkeystorage::TElSSHKey; 	// SFTP Event를 받기 위해 어쩔 수 없는 선택.

//---------------------------------------------------------------------------
class TFtpThread : public TThread
{
__published:	// IDE-managed Components
	void __fastcall SFTPClientAuthenticationFailed(TObject *Sender, int AuthenticationType);
	void __fastcall SFTPClientAuthenticationSuccess(TObject *Sender);
	void __fastcall SFTPClientCloseConnection(TObject *Sender);
	void __fastcall SFTPClientError(TObject *Sender, int ErrorCode);
	void __fastcall SFTPClientKeyValidate(TObject *Sender, TElSSHKey *ServerKey, bool &Validate);
	void __fastcall SFTPClientProgress(TObject *Sender, __int64 Total, __int64 Current,
		  bool &Cancel);
private:
	TfbTable *RPoint2;
	TDateTime m_TimeStamp;

	bool bEsvFlag;	// ADVM+EWSV에서 Esv 성공 여부
	bool bAdvFlag;	// ADVM+EWSV에서 Adv 성공 여부

	bool ConnectToFtpServerSendData(TIdFTP *ftp, UnicodeString sFtpDomain, int iPort, UnicodeString sID, UnicodeString sPW, STFTPMessage *pFTPMessage, int ServerNo);
	bool ConnectSendBySFTP(bool bSFTPClient, UnicodeString sFtpDomain, int iPort, UnicodeString sID, UnicodeString sPW, UnicodeString PrivateKey, STFTPMessage *pFTPMessage, int ServerNo);
	void FTPProcess(STFTPMessage *pFTPMessage);
	void MakeFTPQueueEmpty();
protected:
	void __fastcall Execute();
	void __fastcall SetFtpIcon();
public:
	__fastcall TFtpThread();
	__fastcall ~TFtpThread();
	bool bWorking;
};
//---------------------------------------------------------------------------
extern PACKAGE TFtpThread *thrdFTP;
//---------------------------------------------------------------------------
#endif
