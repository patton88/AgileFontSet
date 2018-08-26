// AgileFontSet.cpp : main source file for AgileFontSet.exe
//

#include "stdafx.h"

#include "resource.h"

//#include "MainDlg.h"

#include "AgileFontSet.h"

CAppModule _Module;
HWND hwndPP1;

CString g_strVerInfo = L"—∏Ω›◊÷ÃÂ…Ë÷√≥Ã–Úv2.0 - ybmj@vip.163.com(20180830)";

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		//CMainDlg dlgMain;
		//nRet = (int)dlgMain.DoModal();
		nRet = VS2013_Win32App_wWinMain(hInstance, hPrevInstance, lpstrCmdLine, nCmdShow);
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
