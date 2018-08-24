// PP0_PropertySheet.h: interface for the PP0_PropertySheet class.
#pragma once

//#include "PP1_IconSapcing.h"
#include "PP1_FontSet.h"
#include "PP2_Readme.h"
#include "CButtonImpl.h"
#include <vector>
#include <string>
using std::vector; using std::string;

//#include "ChromeFunctions.h"
//这里包含ChromeFunctions.h，会与ChromePortable.cpp的包含ChromeFunctions.h发生重复定义问题。
//只能通过将这里需要使用的函数申明为外部函数的方式解决
// UNICODE CString字符串转string字符串。Quote。提高效率
extern string& CStringToStringQ(const CString& wstr, string& str);
//CString Trim。用于引用参数和返回值 Quote
extern CString& CTrimQ(CString& str);
// 从路径获得文件名
extern CString GetFileNameFromPath(const CString& strPath);
//best-CString的split函数，strSep可以是多个字符。WIn32Con_UNICDOE_01-CStringSplit测试成功.
extern vector<CString>& CStringSplit(vector<CString>& vecResult, const CString& str, CString strSep);

class PP0_PropertySheet : public CPropertySheetImpl<PP0_PropertySheet>
{
public:
	// Construction
	PP0_PropertySheet(_U_STRINGorID title = (LPCTSTR)NULL,
		UINT uStartPage = 0, HWND hWndParent = NULL);

	// Maps
	BEGIN_MSG_MAP(PP0_PropertySheet)
		//MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		COMMAND_ID_HANDLER(IDOK, OnApply)						// 应用设置，不退出设置界面
		//COMMAND_HANDLER(IDB_LOAD_SET, BN_CLICKED, OnLoadSetFromFile)
		COMMAND_ID_HANDLER(IDB_SAVE_SET, OnSave)
		COMMAND_ID_HANDLER(IDB_LOAD_SET, OnLoad)
		CHAIN_MSG_MAP(CPropertySheetImpl<PP0_PropertySheet>)
	END_MSG_MAP()

	//bool OnSaveAllToFile(CString strFileName);
	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//保存全部修改，不退出设置界面
	//LRESULT OnApplyAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLoad(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	// Message handlers
	void OnShowWindow(BOOL bShowing, int nReason);
	//BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	BOOL OnSheetInitialized();

	// Property pages
	//PP1_IconSapcing m_pp1IconSapcing;
	PP2_Readme m_pp2Readme;
	PP1_FontSet m_pp1FontSet;
	CPropertyPage<IDD_ABOUTBOX> m_ppAbout;
	CButtonImpl m_ButtonCopy;

	CButtonImpl m_btnLoadOldValue;
	CButtonImpl m_btnLoadNewValue;
	CButtonImpl m_btnApply;
	CButtonImpl m_btnApplyQuit;
	CButtonImpl m_btnCancle;

	// Implementation
protected:
	bool m_bCentered;
};
