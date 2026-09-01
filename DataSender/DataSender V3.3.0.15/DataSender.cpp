//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//#pragma resource "WindowsVista.res" //권한상승 =>heaven2 2016.08.25
#include <tchar.h>
//#include "LockeCommon.h"
//---------------------------------------------------------------------------
USEFORM("UnitMain.cpp", frmMain);
USEFORM("UnitVth.cpp", frmVth);
USEFORM("UnitConfig.cpp", FormConfig);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //Unhandled Exception 시 O/S 레벨의 디버그 창 표시를 Disable 시킴
	SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);

//#ifdef _DEBUG
//	ReportMemoryLeaksOnShutdown = true;
//#endif

	HANDLE hMutex = NULL;
	try
	{
		//중복 실행 방지
        hMutex = CreateMutex(NULL, TRUE, L"Application/DataSenderMutex");
        if (NULL == hMutex) throw Exception("Error:CreateMutex failed.");
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
             CloseHandle(hMutex);
             return 0;
		} // if

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
