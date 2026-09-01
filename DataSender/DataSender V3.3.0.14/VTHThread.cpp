//---------------------------------------------------------------------------

#pragma hdrstop

#include "VTHThread.h"
#include "LockeCommon.h"
#include "Env.h"
#include "Thrdqueue.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TVTHThread *thrdVTH;

//---------------------------------------------------------------------------
__fastcall TVTHThread::TVTHThread() : TThread(false)
{
	Priority   = tpLower;
	RPoint     = new TfbTable(frmMain->SQLConnection, "RPOINT");
    RVTHLogger = new TfbTable(frmMain->SQLConnection, "RVTHLOGGER");
}

//---------------------------------------------------------------------------
__fastcall TVTHThread::~TVTHThread()
{
    delete RPoint;
    delete RVTHLogger;
    TThread::~TThread();
}

//---------------------------------------------------------------------------
void TVTHThread::VTHSend()
{
    TQueueNode *aNode = NULL;
    //vth 시리얼 송신자료 처리
    if (frmMain->ApdComPort->Open) //시리얼이 연결된 상태에서만 큐의 자료를 꺼내서 전송
    {
        while(aNode = qVTHOutput->Pop(), aNode != NULL)
        {
            try
            {
                UnicodeString sText = ((TNormalQData*)aNode->Data)->sText;
                frmMain->ApdComPort->PutString(sText);
                if (sText == "!") frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTH 신호전송 [" + sText.Trim() + "]", (frmMain->GiGWResetFrom <= 1) ? true : false);
                else              frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTH 신호전송 [" + sText.Trim() + "]");
            } // try
            catch(Exception &e)
            {
                frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTH 신호전송 에러: " + e.Message);
            } // catch
            GlobalFree(aNode->Data);
            GlobalFree(aNode);
        } // while
    } // if
}

//---------------------------------------------------------------------------
void TVTHThread::VTHReceive()
{
    TQueueNode *aNode = NULL;
    //vth 시리얼 수신자료 처리
    if (frmMain->ApdComPort->Open) //시리얼이 연결된 상태에서만
    {
        while(aNode = qVTHInput->Pop(), aNode != NULL)
        {
            UnicodeString sDateTime = ((TReceiveTimeQData *)aNode->Data)->sDateTime;
            UnicodeString sData = ((TReceiveTimeQData *)aNode->Data)->sText;
            GlobalFree(aNode->Data);
            GlobalFree(aNode);

            //frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTH 수신[" + sDateTime + "]: " + sData.Trim());
            int iCnt = GetSepItemCount(sData, ",");
            if (iCnt > 0)
			{
				// heaven2  2014.06.25
				frmMain->fVTHCharge      = StrToFloat(GetSepItem(sData, 5, ","));
				frmMain->fVTHTemperature = StrToFloat(GetSepItem(sData, 0, ","));
				frmMain->fVTHHumidity    = StrToFloat(GetSepItem(sData, 1, ","));
				frmMain->fVTHBattery     = StrToFloat(GetSepItem(sData, 2, ","));

				::PostMessage(frmMain->Handle, WM_USER + VTHValue, 0, 0);	// Refresh Main UI

				bool bExist;
				RPoint->ClearParams();
				RPoint->SetWhereParam("MEASUREDATE", sDateTime.SubString(1, 8));
				RPoint->SetWhereParam("MEASURETIME", sDateTime.SubString(9, 6));

				try
				{
					bExist = RPoint->Exists();
				} // try
				catch(Exception &e)
				{
					frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "DB 쿼리 오류: " + e.Message);
					frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, RPoint->SqlText);
				} // catch

				if (!bExist)
				{
					RPoint->ClearParams();
					RPoint->SetFieldParam("MEASUREDATE", sDateTime.SubString(1, 8));
					RPoint->SetFieldParam("MEASURETIME", sDateTime.SubString(9, 6));
					RPoint->SetFieldParam("FILEFLAG",   "N");
					RPoint->SetFieldParam("SERVERFLAG", "N");
					RPoint->SetFieldParam("RNSEAFLAG",  "N");
					UnicodeString sErrorMessage = RPoint->Insert();
					if (sErrorMessage != "")
					{
						frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "RPoint DB저장 오류: " + sErrorMessage);
						frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, RPoint->SqlText);
					} // if
				} // if

				//RVTHLOGGER에 insert
				//UnicodeString sVolt = (StrToFloat(GetSepItem(sData, 5, ",")) == 0.0f) ? "0" : "220"; //dc charge=0이면 전압도 0
				UnicodeString sVolt = (StrToFloat(GetSepItem(sData, 2, ",")) < 11.0f) ? "0" : "220"; //dc battery
				RVTHLogger->ClearParams();
				RVTHLogger->SetFieldParam("MEASUREDATE", sDateTime.SubString(1, 8)); 	//일자
				RVTHLogger->SetFieldParam("MEASURETIME", sDateTime.SubString(9, 6)); 	//시간
				RVTHLogger->SetFieldParam("VTHKIND", "VTH Logger"); 					//"VTH Logger"
				RVTHLogger->SetFieldParam("VOLT",        sVolt); 						//전압
				RVTHLogger->SetFieldParam("TEMPERATURE", GetSepItem(sData, 0, ",")); 	//온도
				RVTHLogger->SetFieldParam("HUMIDITY",    GetSepItem(sData, 1, ",")); 	//습도
				RVTHLogger->SetFieldParam("DCCHARGE",    GetSepItem(sData, 5, ",")); 	//dc charge
				RVTHLogger->SetFieldParam("DCBATTERY",   GetSepItem(sData, 2, ",")); 	//dc battery
				RVTHLogger->SetFieldParam("Value01",     GetSepItem(sData, 6, ",")); 	//AIN0
				RVTHLogger->SetFieldParam("Value02",     GetSepItem(sData, 7, ",")); 	//AIN1
				RVTHLogger->SetFieldParam("Value03",     GetSepItem(sData, 8, ",")); 	//AIN2
				RVTHLogger->SetFieldParam("Value04",     GetSepItem(sData, 9, ",")); 	//AIN3
				RVTHLogger->SetFieldParam("RAWDATA",     sData); 						//raw data
				UnicodeString sErrorMessage = RVTHLogger->Insert();
				if (sErrorMessage != "")
				{
					frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTH DB저장 오류: " + sErrorMessage);
					frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, RVTHLogger->SqlText);
				} // if
            } // if
        } // while
    } // if
}

//---------------------------------------------------------------------------
void __fastcall TVTHThread::Execute()
{
	try
	{
        frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTHThread 시작");
		while (!Terminated)
		{
			DWORD dwResult = 0;
//			DWORD dwResult = WaitForMultipleObjects(meVTHEvents->iEventCount,
//													meVTHEvents->GetEventHandles(),
//													false,
//													3000); //INFINITE); // 1000=>3000 VTHLogger가 늦게 답하는 경우를 상정. 2014.08.28
			switch(dwResult)
			{
                case WAIT_OBJECT_0 + 0: //CsVTHSend
                    frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "[VTHThread] CsVTHSend");
					break;
                case WAIT_OBJECT_0 + 1: //CsVTHReceive
                    frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "[VTHThread] CsVTHReceive");
                    break;
            } // switch

			switch(dwResult)
			{
                case WAIT_ABANDONED_0:
				case WAIT_TIMEOUT: //전체 처리
                    VTHSend();           //CsVTHSend
                    VTHReceive();        //CsVTHReceive
                    break;
				case WAIT_OBJECT_0 + 0: //CsVTHSend
                    VTHSend();
                    break;
				case WAIT_OBJECT_0 + 1: //CsVTHReceive
                    VTHReceive();
                    break;
			} //switch
		} // while
        frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTHThread 종료");
	} // try
	catch (Exception &e)
	{
		frmMain->tmrWD->Enabled = false; //watchdog 전송중지
		frmMain->MessageToLogMemo(CbFileWrite, !CbDBWrite, "VTHThread 오류: " + e.Message);
	} // catch
}
//---------------------------------------------------------------------------



