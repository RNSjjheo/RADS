//---------------------------------------------------------------------------
#include <vcl.h>                         	// 이 3개는 여기에 반드시 놓아야 함.
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#pragma hdrstop

//#include <vcl.h>
//#include <Vcl.Imaging.jpeg.hpp>
//#include <Vcl.Imaging.pngimage.hpp>
								// vcl compoent를 중복으로 include 하는 것은 Sftp을 include하고
								// unitmain.h를 Include할 때 VCL component와 다시 충돌하기 때문에 이렇게 함
#include "SBSimpleSftp.hpp"   	// 반드시 여기에 쓸 것 (include UnitMain.h 하기 전에 쓸 것)

#include "UnitMain.h"
#include "FtpThread.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SBSimpleSftp"

const bool bSFTPClient  = true;  	// frmMain->SFTPClient
const bool bSFTPClient2 = false;	// frmMain->SFTPClient2

TElSimpleSFTPClient *SFTPClient;
TElSSHMemoryKeyStorage *FKeyStorage;

////////////////////////////////////////////////////////////////////////////////

TFtpThread *thrdFTP;

static int iFailCount     = 0;
static int iFtpFailCount  = 0;
static int iGWRebootCount = 0;
static int iSuccessCount  = 0;

//---------------------------------------------------------------------------
__fastcall TFtpThread::TFtpThread()
{
	Priority = tpLower;
	bWorking = false;
	m_TimeStamp = 0;

	bEsvFlag = false;	// ADVM+EWSV에서 Esv 성공 여부
	bAdvFlag = false;	// ADVM+EWSV에서 Adv 성공 여부

	// SFTP Component
	SFTPClient = new TElSimpleSFTPClient(frmMain);
	FKeyStorage = new TElSSHMemoryKeyStorage(frmMain);
	SFTPClient->KeyStorage = FKeyStorage;

	//heaven2 2017.12.27 SFTP 전송 시 가끔 인증에 과도하게 시간이 걸려 인증 Fail이 나는 현상 때문에 시간을 확장
	SFTPClient->CommandTimeout = 20 * 1000;	// timeout => 15sec => 20sec
	SFTPClient->SocketTimeout  = 20 * 1000;	// timeout => 15sec => 20sec

	SFTPClient->OnAuthenticationFailed  = SFTPClientAuthenticationFailed;
	SFTPClient->OnAuthenticationSuccess = SFTPClientAuthenticationSuccess;
	SFTPClient->OnCloseConnection       = SFTPClientCloseConnection;
	SFTPClient->OnError                 = SFTPClientError;
	SFTPClient->OnKeyValidate           = SFTPClientKeyValidate;
	SFTPClient->OnProgress              = SFTPClientProgress;

	FreeOnTerminate = true;
}

//---------------------------------------------------------------------------
__fastcall TFtpThread::~TFtpThread()
{
	//delete RPoint2;

}

//---------------------------------------------------------------------------
bool TFtpThread::ConnectToFtpServerSendData(TIdFTP *ftp, UnicodeString sFtpDomain, int iPort, UnicodeString sID, UnicodeString sPW, STFTPMessage *pFTPMessage, int ServerNo)
{
    bool bResult = false;

    try
    {
		if (ftp->Connected()) ftp->CheckForGracefulDisconnect(false); //Exception없이 disconnect check
	} catch ( Exception &e ) {
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectToFtpServerSendData]-1-CheckForGracefulDisconnect " + e.Message);
	}
		if (!ftp->Connected())
		{
			ftp->Host = sFtpDomain;
			ftp->Username = sID;
			ftp->Password = sPW;
			try {
				ftp->Connect();
			} catch ( Exception &e ) {
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectToFtpServerSendData]-2-Connect " + e.Message);
				bResult = false;
				frmMain->fFTPStatus      = fsIdle; //ftp 이미지 상태
				frmMain->fFTPImageStatus = fipEnabled; //ftp 이미지 상태
				Synchronize(SetFtpIcon);
				return bResult;
			}
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] FTP Connection opened.");
			//ftp->ChangeDir("data");
		} // if

		//frmMain->FTPImageStatus = fipEnabled; //ftp 상태표시
		if (ftp == frmMain->FTP)  {
			frmMain->fFTPStatus = fsServerStart;
			frmMain->fFTPImageStatus = fipSendingFTP1;
		} else  {
			frmMain->fFTPStatus = fsRnseaStart,
			frmMain->fFTPImageStatus = fipSendingFTP2;
		}
		Synchronize(SetFtpIcon);

		//ftp서버 폴더이동
		//UnicodeString sPath = ExtractFtpFilePath(ftpData->sFTPRootPathFile);
		//UnicodeString sFolder = GetSepItem(sPath, 2, "/"); // ->/data/real 일 경우 real(혹은 past)폴더
		//ftp->ChangeDir(sFolder);

		//ftp서버전송
		//UnicodeString sFTPFile = ExtractFtpFileName(ftpData->sFTPRootPathFile);
//		UnicodeString sFTPFile = pFTPMessage->sFTPRootPathFile;	// (ex) /data/REAL/1234588_201409171350.adv

// 		2024.11.29
//		UnicodeString sFTPFile = pFTPMessage->sFTPRootPathFile;	// (ex) /data/REAL/1234588_201409171350.adv
		UnicodeString sFTPFile = ( ServerNo == SERVER_IRDIMS_FIRST)  ? pFTPMessage->sFTPRootPathFile  :                                   // 서버1의 file path
								 ((ServerNo == SERVER_IRDIMS_SECOND) ? pFTPMessage->sFTPRootPathFile2 : pFTPMessage->sFTPRootPathFile);	  // 서버2의 file path
		if (sFTPFile.IsEmpty()) {
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectSendBySFTP] filename is empty.. ");
			return false;
		}

		//if (ftp == frmMain->FTP) frmMain->FTPImageStatus = fipSendingFTP1; //ftp 상태표시
		//else                     frmMain->FTPImageStatus = fipSendingFTP2;

		try {
			ftp->Put(pFTPMessage->sLocalPathFile, sFTPFile, false, 0);
		} catch ( Exception &e ) {
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectToFtpServerSendData]-3-Put " + e.Message);
			bResult = false;
			frmMain->fFTPStatus      = fsIdle; //ftp 이미지 상태
			frmMain->fFTPImageStatus = fipEnabled; //ftp 이미지 상태
			Synchronize(SetFtpIcon);
			return bResult;
		}
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "FTP전송: " + sFtpDomain + sFTPFile);
		//ftp->ChangeDir(".."); //data폴더로 이동

		if (ftp == frmMain->FTP)  {
			frmMain->fFTPStatus      = fsIdle;
			frmMain->fFTPImageStatus = fipCompleteFTP1;
		} else  {
			frmMain->fFTPStatus      = fsIdle,
			frmMain->fFTPImageStatus = fipCompleteFTP2;
		}
		Synchronize(SetFtpIcon);

		bResult = true;
//	} // try
//	catch(Exception &e)
//	{
//		frmMain->fFTPStatus      = fsIdle; //ftp 이미지 상태
//		frmMain->fFTPImageStatus = fipEnabled; //ftp 이미지 상태
//		Synchronize(SetFtpIcon);
//		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "FTP 에러: " + e.Message);
//		bResult = false;
//    } // catch

    return bResult;
}

//---------------------------------------------------------------------------
// Heaven2 2017.11.10
// SFTP Connect and Send File
bool TFtpThread::ConnectSendBySFTP(bool bSFTPClient, UnicodeString sFtpDomain, int iPort, UnicodeString sID, UnicodeString sPW, UnicodeString PrivateKey, STFTPMessage *pFTPMessage, int ServerNo)
{
	bool bResult = false;

	try {
		if ( SFTPClient->Active ) SFTPClient->Close(false);
	} catch ( Exception &e ) {
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectSendBySFTP] Connection close error..  " + e.Message);
	}

	SFTPClient->Address  = sFtpDomain;
	SFTPClient->Port     = iPort;
	SFTPClient->Username = sID;
	SFTPClient->Password = sPW;

	if (!PrivateKey.IsEmpty()) { 	// SFTP has Public Key <=> Private Key
		SFTPClient->KeyStorage = FKeyStorage;
		FKeyStorage->Clear();
		TElSSHKey *Key = new TElSSHKey;
		if ((!PrivateKey.IsEmpty()) && FileExists(PrivateKey) && (Key->LoadPrivateKey(PrivateKey,"") == 0)) {
			FKeyStorage->Add(Key);
			SFTPClient->AuthenticationTypes = SFTPClient->AuthenticationTypes | SSH_AUTH_TYPE_PUBLICKEY;
		} else {
			SFTPClient->AuthenticationTypes = SFTPClient->AuthenticationTypes & (~SSH_AUTH_TYPE_PUBLICKEY);
		}
		delete Key;
	} else {
		SFTPClient->KeyStorage = FKeyStorage;
		FKeyStorage->Clear();
		SFTPClient->AuthenticationTypes = SFTPClient->AuthenticationTypes & (~SSH_AUTH_TYPE_PUBLICKEY);
	}

	try {
		SFTPClient->Open();
	} catch ( Exception &e ) {
		SFTPClient->Close(false);
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectSendBySFTP] SFTP Connection failed.. " + e.Message);
		bResult = false;
		frmMain->fFTPStatus      = fsIdle; //ftp 이미지 상태
		frmMain->fFTPImageStatus = fipEnabled; //ftp 이미지 상태
		Synchronize(SetFtpIcon);
		//delete FKeyStorage;
		return bResult;
	}

	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] SFTP Connection opened.");

//	if (SFTPClient == frmMain->SFTPClient)  {  	// SFTP1
//		frmMain->fFTPStatus = fsServerStart;
//		frmMain->fFTPImageStatus = fipSendingFTP1;
//	} else  {									// SFTP2
//		frmMain->fFTPStatus = fsRnseaStart,
//		frmMain->fFTPImageStatus = fipSendingFTP2;
//	}
//	Synchronize(SetFtpIcon);

// 2024.11.29
//	UnicodeString sFTPFile = pFTPMessage->sFTPRootPathFile;	// (ex) /data/REAL/1234588_201409171350.adv
	UnicodeString sFTPFile = ( ServerNo == SERVER_IRDIMS_FIRST)  ? pFTPMessage->sFTPRootPathFile  :                                   // 서버1의 file path
							 ((ServerNo == SERVER_IRDIMS_SECOND) ? pFTPMessage->sFTPRootPathFile2 : pFTPMessage->sFTPRootPathFile);	  // 서버2의 file path
	if (sFTPFile.IsEmpty()) {
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectSendBySFTP] filename is empty.. ");
		return false;
	}

	try {
		SFTPClient->UploadFile(pFTPMessage->sLocalPathFile, sFTPFile);
	} catch ( Exception &e ) {
		try {
			SFTPClient->Close();	// Connection close
		} catch ( Exception &e) {
			// nothing to do..
		}
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread][ConnectSendBySFTP] file transfer failed.. " + e.Message);
		bResult = false;
		frmMain->fFTPStatus      = fsIdle; //ftp 이미지 상태
		frmMain->fFTPImageStatus = fipEnabled; //ftp 이미지 상태
		Synchronize(SetFtpIcon);
		//delete FKeyStorage;
		return bResult;
	}

	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] " + sFtpDomain + ":" + sFTPFile + " Ok..");

	try {
		SFTPClient->Close();	// Connection close
	} catch ( Exception &e) {
		// nothing to do..
	}

//	if (SFTPClient == frmMain->SFTPClient)  {
//		frmMain->fFTPStatus      = fsIdle;
//		frmMain->fFTPImageStatus = fipCompleteFTP1;
//	} else  {
//		frmMain->fFTPStatus      = fsIdle,
//		frmMain->fFTPImageStatus = fipCompleteFTP2;
//	}
	Synchronize(SetFtpIcon);
	//delete FKeyStorage;

	bResult = true;
	return bResult;
}

//---------------------------------------------------------------------------
void __fastcall TFtpThread::Execute()
{
	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "FtpThread 시작");

	RPoint2 = new TfbTable(frmMain->SQLConnection, "rpoint");

	TWaitResult Result;
	STFTPMessage *pFTPMessage;

	int Cnt = 0;

	try
	{
		while (!Terminated)
		{
			bWorking = false;
			Result = pGFTPEvent->WaitFor(1000);	// Wait 1 sec
			bWorking = true;
			if ( Result == wrSignaled || Result == wrTimeout ) {
				bool bReal = false;
				bool bQueue = false;
				m_TimeStamp = Now();	// Thread Fire TiemStamp
				while ( pGFTPQueue->Count() ) {
					pGFTPSection->Enter();
					pFTPMessage = (STFTPMessage *)pGFTPQueue->Pop();
					pGFTPSection->Leave();
					if ( frmMain->GbGWPowerOff ) {	// G/W Reset 중이면 Skip한다.
						delete pFTPMessage;
						continue;
					}
					////////////////////////////////////////////////////////////
					// Main Process
					////////////////////////////////////////////////////////////
					FTPProcess(pFTPMessage);

					if (pFTPMessage->bReal) bReal = true;
					bQueue = true;
					delete pFTPMessage;
				}

				if ( bQueue ) {   // Some Item In Queue
					try {
						if (frmMain->FTP->Connected()) {
							frmMain->FTP->Disconnect();
							frmMain->FTP->IOHandler->InputBuffer->Clear();
						}
						if (frmMain->ftpRnsea->Connected()) {
							frmMain->ftpRnsea->Disconnect();
							frmMain->ftpRnsea->IOHandler->InputBuffer->Clear();
						}
						//SleepMsg(100);	// wait 500ms for ftp connection disconnect..
						frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] FTP Connection closed");
					} catch ( Exception &e) {
						frmMain->LogMessage(CbFileWrite, !CbDBWrite, UnicodeString("[FTPThread] ") + e.Message);
					}

					if (bReal) frmMain->fFTPStatus = fsIdle; //ftp상태
					frmMain->fFTPImageStatus = fipEnabled;
					Synchronize(SetFtpIcon);

					if ( bReal ) frmMain->SendSenderStatusToViewer(); //Broadcast

					iFailCount = 0;	// Reset => Queue 루프에서 실패한 파일 수 , iFtpFailCount=>현재까지 FTP 전송 실패한 파일 수
					iSuccessCount = 0;
				} else {
					// nothing to do..
				}

			} else if ( Result == wrAbandoned ) {	// Event Object was destored
													// wrError: An error occured while waiting.
													// Check the LastError property for an error
													// code giving more informaiton.
			} else {
				// nothing to do..
			}
		} // end of while
	} catch ( Exception &e ) {
		frmMain->tmrWD->Enabled = false; //watchdog 전송중지
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, "FTPThread 오류: " + e.Message);
	}

	// SFTP Resource Release
	delete FKeyStorage;	//SFTP Resource Release
	delete SFTPClient;

	// DB Resource Release
	//delete RPoint2;	// heaven2 09/25 왜 안 지웠지?
	delete RPoint2;	// 2017.09.25
}

//////////////////////////////////////////////////////////////////////////////
// Thread Main Process
void TFtpThread::FTPProcess(STFTPMessage *pFTPMessage)
{
	bool bReal = false;
//	static int iFailCount     = 0;
//	static int iFtpFailCount  = 0;
//	static int iGWRebootCount = 0;
//	static int iSuccessCount  = 0;

	bReal = pFTPMessage->bReal;
	if (!FileExists(pFTPMessage->sLocalPathFile))
	{
		frmMain->LogMessage(CbFileWrite, !CbDBWrite, pFTPMessage->sLocalPathFile + " 파일이 없습니다.");
		return;
	} // if

	bool bFtpSuccess = false;
	UnicodeString sServerFlag = "N";

	////////////////////////////////////////////////////////////////////////
	// 수집 서버 전송
	////////////////////////////////////////////////////////////////////////

	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] CsFTPSend Start");

	if (frmMain->DBEnvInfo.bServerEnable)
	{
		if (bReal) {
			frmMain->fFTPStatus = fsServerStart; //ftp상태
		}

		//for(int i = 0; i < frmMain->DBEnvInfo.iRetryCount; i++)
		for(int i = 0; i < 2 ; i++)
		{
			if ( frmMain->DBEnvInfo.sFTPType == "FTP" || frmMain->DBEnvInfo.sFTPType == "" ) {
				bFtpSuccess = ConnectToFtpServerSendData(frmMain->FTP,
														 frmMain->DBEnvInfo.sServerDomain,
														 frmMain->DBEnvInfo.iServerPort,
														 frmMain->DBEnvInfo.sServerAccount,
														 frmMain->DBEnvInfo.sServerPassword,
														 pFTPMessage,
														 SERVER_IRDIMS_FIRST);
			} else if ( frmMain->DBEnvInfo.sFTPType == "SFTP" ) {
				bFtpSuccess  = ConnectSendBySFTP(bSFTPClient,  	// true => frmMain->SFTPClient1
														 frmMain->DBEnvInfo.sServerDomain,
														 frmMain->DBEnvInfo.iServerPort,
														 frmMain->DBEnvInfo.sServerAccount,
														 frmMain->DBEnvInfo.sServerPassword,
														 frmMain->DBEnvInfo.sPrivateKey,
														 pFTPMessage,
														 SERVER_IRDIMS_FIRST);
			} else {
				bFtpSuccess = ConnectToFtpServerSendData(frmMain->FTP,
														 frmMain->DBEnvInfo.sServerDomain,
														 frmMain->DBEnvInfo.iServerPort,
														 frmMain->DBEnvInfo.sServerAccount,
														 frmMain->DBEnvInfo.sServerPassword,
														 pFTPMessage,
														 SERVER_IRDIMS_FIRST);
			}

			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] 통합수신서버 " + IntToStr(i + 1) + "번 시도: " +((bFtpSuccess) ? "성공" : "실패"));
			//db에 업데이트
			if (bFtpSuccess)
			{
				sServerFlag = "Y";
				frmMain->lblFtpTransferDateTime->Caption = "전송시간: " + Now().FormatString("yyyy-mm-dd hh:nn:ss") + " Ok";
				frmMain->lblFtpTransferDateTime->Update();
				iFailCount     = 0;
				iFtpFailCount  = 0; //ftp실패 카운트
				iGWRebootCount = 0; //g/w 리셋 카운트
				iSuccessCount++; //성공카운트
				break;
			} // if
			//SleepMsg(100);	//Wait for While => heaven2 2015.09.15

			// TimeStamp Check => 2분 이상의 시간이 지나면 안되므로
			// 1분 40초 이상 경과되면 Queue를 비우고 탈출한다. (왜냐면 2분 타이머 이므로)
			if ( (double)(Now() - m_TimeStamp) > (double)TDateTime(0,1,40,0)) {
				MakeFTPQueueEmpty(); // Make FTPQueue Empty
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] 1분 40초 이상 경과. FTPQueue를 비움.");
				break;
			}
		} // for

		if (!bFtpSuccess)
		{
			iSuccessCount = 0; //성공카운트
			iFailCount++;
			iFtpFailCount++;

			if ( iFailCount >= 2 ) { // 2개 파일을 연속으로 전송 실패하였을 경우 이미 1분 이상 진행되었으므로 Queue비운다.
				MakeFTPQueueEmpty(); // Make FTPQueue Empty
				iFailCount = 0;
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] 2개 파일 연속 실패함. FTPQueue를 비움.");
			}

			if (frmMain->DBEnvInfo.bRebootFlag)
			{
				if ( iFtpFailCount >= frmMain->DBEnvInfo.iRebootRetryCount ) { 	// G/W Reboot
					MakeFTPQueueEmpty(); // Make FTPQueue Empty

					iFtpFailCount = 0;
					iGWRebootCount++;
					frmMain->GiGWResetFrom = 1;
					frmMain->SendVTH(AnsiString("!"));
				}

				if ( iGWRebootCount >= frmMain->DBEnvInfo.iRebootRetryCount ) { // PC Reboot
					frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] PC 재부팅");
					ForcePCReboot();
					return;
				}
				if (bReal) frmMain->fFTPStatus = fsIdle;
			}
			else
			{
				// bRebootFlag => False 상황
			}
		}
		else	// 전송이 성공한 상황: Fail/ G/W Reset count Reset
		{
			if ( iSuccessCount > 10 ) {
				MakeFTPQueueEmpty();	// Too many item in Queue
				iSuccessCount = 0;
			}
			iFtpFailCount  = 0;
			iGWRebootCount = 0;
		}
		if (bReal) frmMain->fFTPStatus = fsIdle; //ftp상태
	}

	////////////////////////////////////////////////////////////////////////
	// RNSEA Server 전송
	////////////////////////////////////////////////////////////////////////

	UnicodeString sRnseaFlag = "N";
	if (frmMain->DBEnvInfo.bRnseaEnable)
	{
		if (bReal) frmMain->fFTPStatus = fsRnseaStart; //ftp상태
		for ( int i = 0 ; i < 2 ; i++ ) { 	// RNSEA 서버는 두번까지 시도하고 성공여부 상관없이 빠진다.
			if ( frmMain->DBEnvInfo.sFTPType2 == "FTP" || frmMain->DBEnvInfo.sFTPType2 == "" ) {
				bFtpSuccess = ConnectToFtpServerSendData(frmMain->ftpRnsea,
														 frmMain->DBEnvInfo.sRnseaDomain,
														 frmMain->DBEnvInfo.iRnseaPort,
														 frmMain->DBEnvInfo.sRnseaAccount,
														 frmMain->DBEnvInfo.sRnseaPassword,
														 pFTPMessage,
														 SERVER_IRDIMS_SECOND);
			} else if ( frmMain->DBEnvInfo.sFTPType2 == "SFTP" ) {
				bFtpSuccess = ConnectSendBySFTP(bSFTPClient2,  		// false => frmMain->SFTPClient2
														 frmMain->DBEnvInfo.sRnseaDomain,
														 frmMain->DBEnvInfo.iRnseaPort,
														 frmMain->DBEnvInfo.sRnseaAccount,
														 frmMain->DBEnvInfo.sRnseaPassword,
														 frmMain->DBEnvInfo.sPrivateKey2,
														 pFTPMessage,
														 SERVER_IRDIMS_SECOND);
			} else {
				bFtpSuccess = ConnectToFtpServerSendData(frmMain->ftpRnsea,
														 frmMain->DBEnvInfo.sRnseaDomain,
														 frmMain->DBEnvInfo.iRnseaPort,
														 frmMain->DBEnvInfo.sRnseaAccount,
														 frmMain->DBEnvInfo.sRnseaPassword,
														 pFTPMessage,
														 SERVER_IRDIMS_SECOND);
			}
			frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] RNSea 시도: " + UnicodeString((bFtpSuccess) ? "성공" : "실패"));
			if (bFtpSuccess)
			{
				sRnseaFlag = "Y";
				if (bReal)  {
					frmMain->fFTPStatus = fsRnseaFinish; //ftp상태
					//frmMain->SendSenderStatusToViewer(); //Broadcast
					//break;
				}
				break;	// 2014.10.16
			}

			// TimeStamp Check => 2분 이상의 시간이 지나면 안되므로
			// 1분 40초 이상 경과되면 Queue를 비우고 탈출한다. (왜냐면 2분 타이머 이므로)
			if ( (double)(Now() - m_TimeStamp) > (double)TDateTime(0,1,40,0)) {
				MakeFTPQueueEmpty(); // Make FTPQueue Empty
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] 1분 40초 이상 경과. FTPQueue를 비움.");
				break;
			}
			//SleepMsg(100);	//Wait for While => heaven2 2015.09.15
		} // end of for
	} // if

	//db업데이트
	if (sServerFlag == "Y" || sRnseaFlag == "Y")
	{
		// ADVM+EWSV 동시 운영 사이트에 대한 RPoint Table 처리 -----------------
		if ( pFTPMessage->bEsvAdvFlag ) {	// ESV, ADV 동시 운영 사이트 일 때.
			if ( pFTPMessage->enFileType == enEsvFile ) {
				bEsvFlag = (sServerFlag == "Y" ) ? true : false;
				bAdvFlag = false;	// esv 파일을 먼저 보내고, adv 파일을 나중에 보내므로 세트를 맞추려면 이렇게
			} else { // enFileType == enAdvFile
				bAdvFlag = (sServerFlag == "Y" ) ? true : false;
			}

			if ( bEsvFlag && bAdvFlag ) {	// ESV, ADV 전송 모두 성공하였을 때
				RPoint2->ClearParams();
				RPoint2->SetFieldParam("SERVERFLAG", sServerFlag);
				RPoint2->SetFieldParam("RNSEAFLAG", sRnseaFlag);
				RPoint2->SetWhereParam("MEASUREDATE", pFTPMessage->sMeasureDate);
				RPoint2->SetWhereParam("MEASURETIME", pFTPMessage->sMeasureTime);
				UnicodeString sError = RPoint2->Update();
				if (sError != "") {
					frmMain->LogMessage(CbFileWrite, !CbDBWrite, "DB Update 에러: " + sError + " SQL=" + RPoint2->SqlText);
				} else {}
				bEsvFlag = false;	// reset status : 다음 전송 결과를 위해
				bAdvFlag = false;	// reset status
			} else {
				// Esv, Adv 동시 운영 사이트이지만 아직 두개 모두 전송되지 않아서 RPoint Table Update하지 않음
				UnicodeString Message = UnicodeString("ADVM+EWSV Status : EsvFile:")   +
										UnicodeString((bEsvFlag) ? "true " : "false ") +
										UnicodeString("AdvFile:")                      +
										UnicodeString((bAdvFlag) ? "true " : "false ");
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, Message);
			}

		// 1개의 데이터 파일 타입에 대한 로직 (기존 로직) RPoint Table 처리 ----
		} else {	// 1개 데이터 파일 => 기존 Flow
			RPoint2->ClearParams();
			RPoint2->SetFieldParam("SERVERFLAG", sServerFlag);
			RPoint2->SetFieldParam("RNSEAFLAG", sRnseaFlag);
			RPoint2->SetWhereParam("MEASUREDATE", pFTPMessage->sMeasureDate);
			RPoint2->SetWhereParam("MEASURETIME", pFTPMessage->sMeasureTime);
			UnicodeString sError = RPoint2->Update();
			if (sError != "")
			{
				frmMain->LogMessage(CbFileWrite, !CbDBWrite, "DB Update 에러: " + sError + " SQL=" + RPoint2->SqlText);
			} // if
		}
	} // if

//	try {
//		if (frmMain->FTP->Connected()) frmMain->FTP->Disconnect();
//		if (frmMain->ftpRnsea->Connected()) frmMain->ftpRnsea->Disconnect();
//		SleepMsg(100);	// wait 500ms for ftp connection disconnect..
//	} catch ( Exception &e) {
//		frmMain->LogMessage(CbFileWrite, !CbDBWrite, UnicodeString("[FTPThread] ") + e.Message);
//	}
//
//	if (bReal) frmMain->fFTPStatus = fsIdle; //ftp상태
//	frmMain->fFTPImageStatus = fipEnabled;
//	Synchronize(SetFtpIcon);
//
//	if ( bReal ) frmMain->SendSenderStatusToViewer(); //Broadcast

	frmMain->LogMessage(CbFileWrite, !CbDBWrite, "[FTPThread] CsFTPSend end");
}

///////////////////////////////////////////////////////////////////////////
// FTP Icon update
void __fastcall TFtpThread::SetFtpIcon()
{
	 frmMain->ChangeFTPIcon(frmMain->fFTPImageStatus);
}

///////////////////////////////////////////////////////////////////////////
// Make FTPQueue Empty
void TFtpThread::MakeFTPQueueEmpty()
{
	STFTPMessage *pFTPMessage = NULL;

	pGFTPSection->Enter();
	while ( pGFTPQueue->Count() ) {
		pFTPMessage = (STFTPMessage *)pGFTPQueue->Pop();
		delete pFTPMessage;
	}
	pGFTPSection->Leave();
}

//---------------------------------------------------------------------------
void __fastcall TFtpThread::SFTPClientAuthenticationFailed(TObject *Sender, int AuthenticationType)
{
//	LogMessage("Authentication failed.. AuthentifcationType = " + IntToStr(AuthenticationType));
}
//---------------------------------------------------------------------------
void __fastcall TFtpThread::SFTPClientAuthenticationSuccess(TObject *Sender)
{
//	LogMessage("Authentication ok..");
}
//---------------------------------------------------------------------------
void __fastcall TFtpThread::SFTPClientCloseConnection(TObject *Sender)
{
// //	LogMessage("SFTP closed..");
}
//---------------------------------------------------------------------------
void __fastcall TFtpThread::SFTPClientError(TObject *Sender, int ErrorCode)
{
//	LogMessage("SFTP Error => " + IntToStr(ErrorCode));
}
//---------------------------------------------------------------------------
//This event is fired when server's public key is received and must be validated.
void __fastcall TFtpThread::SFTPClientKeyValidate(TObject *Sender, TElSSHKey *ServerKey,
          bool &Validate)
{
//	LogMessage("Server key [" + DigestToStr(ServerKey->FingerprintMD5,false) + "] received");
	Validate = true; 	// continue to processing
}
//---------------------------------------------------------------------------
//This event is fired during data reading/writing to report operation progress
void __fastcall TFtpThread::SFTPClientProgress(TObject *Sender, __int64 Total, __int64 Current,
		  bool &Cancel)
{
// nothing to do
}
