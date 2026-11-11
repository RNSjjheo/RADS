//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UnitLogin.cpp", FormLogin);
USEFORM("UnitMain.cpp", FormMain);
USEFORM("UnitLog.cpp", FormLog);
USEFORM("UnitHydroMeterCell.cpp", FormHydroMeterCell);
USEFORM("UnitLeft.cpp", FormLeft);
USEFORM("UnitVTH.cpp", FormVTH);
USEFORM("UnitWaterLevel.cpp", FormWaterLevel);
USEFORM("UnitUser.cpp", FormUser);
USEFORM("UnitReport.cpp", FormReport);
USEFORM("UnitSearchData.cpp", FormSearchData);
USEFORM("UnitHydroMeter.cpp", FormHydroMeter);
USEFORM("UnitConfig.cpp", FormConfig);
USEFORM("UnitFlow.cpp", FormFlow);
USEFORM("UnitAbout.cpp", FormAbout);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// 2014.06.19 Unhandled Exception 시 O/S 레벨의 디버그 창 표시를 Disable 시킴
	::SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS );

	HANDLE hMutex = NULL;

	try
	{
		// 2014.04.02 중복 실행 방지
		hMutex = CreateMutex(NULL, TRUE, L"Application/DataViewerMutex");
		if ( NULL == hMutex )
			throw Exception("Error:CreateMutex failed.");
		if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
			OutputDebugStringA("Named Mutex already exist. skip Application init..");
			CloseHandle(hMutex);
			return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormLeft), &FormLeft);
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
