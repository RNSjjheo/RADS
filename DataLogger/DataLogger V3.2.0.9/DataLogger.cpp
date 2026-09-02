//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//#include "Precompile.h"

//#pragma hdrstop

// UAC Disable로 처리함.
//#pragma resource "WindowsVista.res" //권한상승

#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("HydroSetup\UnitHydroSetupSL.cpp", HydroSetupSL);
USEFORM("HydroSetup\UnitHydroSetupCM.cpp", HydroSetupCM);
USEFORM("UnitAlert.cpp", FormAlert);
USEFORM("HydroSetup\UnitHydroSetupAG.cpp", HydroSetupAG);
USEFORM("HydroSetup\UnitHydroSetup.cpp", HydroSetup);
USEFORM("UnitDM.cpp", DM); /* TDataModule: File Type */
USEFORM("UnitSetup.cpp", FormSetup);
USEFORM("UnitMain.cpp", FormMain);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //Unhandled Exception 시 O/S 레벨의 디버그 창 표시를 Disable 시킴
	SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);

	// 2014.04.02 중복 실행 방지
	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, TRUE, L"Application/DataLoggerMutex");
	if ( NULL == hMutex )
		throw Exception("Error:CreateMutex failed.");
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
		OutputDebugStringA("Named Mutex already exist. skip Application init..");
		CloseHandle(hMutex);
		return 0;
	}

	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TDM), &DM);
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

	OutputDebugStringA("return..");
	if (hMutex) CloseHandle(hMutex);

	return 0;
}
//---------------------------------------------------------------------------
