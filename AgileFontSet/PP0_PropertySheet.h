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
		COMMAND_ID_HANDLER(IDOK, OnApplyAll)					// 应用设置，退出设置界面
		//COMMAND_ID_HANDLER(IDM_ABOUT, OnAppAbout)				// 响应关于菜单项
		//COMMAND_HANDLER(IDOK, BN_CLICKED, OnSaveAllToFile)
		COMMAND_HANDLER(IDC_BTN_APPLY, BN_CLICKED, OnApply)		// 应用设置，不退出设置界面
		COMMAND_HANDLER(IDC_BTN_LOAD_SET_FROM_FILE, BN_CLICKED, OnLoadSetFromFile)
		//COMMAND_HANDLER(IDC_BTN_LOAD_NEW_VALUE, BN_CLICKED, OnLoadDefaultAll)
		//COMMAND_HANDLER(IDC_BTN_LOAD_NEW_VALUE, BN_CLICKED, OnLoadNewValue)// 加载新的设置，不退出设置界面
		//COMMAND_HANDLER(IDC_BTN_LOAD_OLD_VALUE, BN_CLICKED, OnLoadOldValue)// 恢复旧有设置，不退出设置界面
		//COMMAND_ID_HANDLER_EX(IDC_BTN_RELOAD_SET_FROM_FILE, OnReloadSetFromFile)
		CHAIN_MSG_MAP(CPropertySheetImpl<PP0_PropertySheet>)
	END_MSG_MAP()

	//bool OnSaveAllToFile(CString strFileName);
	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//保存全部修改，不退出设置界面
	LRESULT OnApplyAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	//LRESULT OnLoadNewValue(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//{
	//	// TODO: Add validation code
	//	int nActiveIndex = GetActiveIndex();

	//	//加载新的值
	//	m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iNewHS);
	//	m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iNewVS);

	//	//设置焦点
	//	::SetFocus(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS));	//有效。该方式CEdit获得焦点后光标控制正确
	//	::SendMessage(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//发送一个End按键，取消选中状态

	//	return 0;
	//}

	//LRESULT OnLoadOldValue(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//{
	//	int nActiveIndex = GetActiveIndex();

	//	//若用户当前输入值与旧有值不同，则保存为新值，并启用“新的值”按钮
	//	if ((m_pp1IconSapcing.m_spinHS.GetPos() != m_pp1IconSapcing.m_iOldHS) ||
	//		(m_pp1IconSapcing.m_spinVS.GetPos() != m_pp1IconSapcing.m_iOldVS))
	//	{
	//		//使用CUpDownCtrl控件，编程简洁方便
	//		m_pp1IconSapcing.m_iNewHS = m_pp1IconSapcing.m_spinHS.GetPos();
	//		m_pp1IconSapcing.m_iNewVS = m_pp1IconSapcing.m_spinVS.GetPos();
	//		m_pp1IconSapcing.UpdatelistNewInfo();		//更新m_listNewInfo信息
	//		::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_LOAD_NEW_VALUE), TRUE);	//2	m_btnLoadDefaultCurrPage
	//	}

	//	//加载旧有值
	//	m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iOldHS);
	//	m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iOldVS);

	//	//设置焦点
	//	::SetFocus(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS));	//有效。该方式CEdit获得焦点后光标控制正确
	//	::SendMessage(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//发送一个End按键，取消选中状态

	//	//设置加载旧有值、加载新的值标志
	//	//m_iFlag = 1;		//加载旧有值、加载新的值标志。1 (起始)加载旧有值，0 加载新的值

	//	//发现这种思路存在逻辑错误。不用消息响应。只需增加一个按钮即可
	//	//关键在于：调用m_spinXXX之前，应该增加判断m_spinXXX是否是一个窗口。否则异常报错
	//	//if (m_pp1IconSapcing.m_spinHS.IsWindow() && m_pp1IconSapcing.m_spinVS.IsWindow())
	//	//{
	//	//	if (0 == m_pp1IconSapcing.m_iFlag)		//0 (起始)加载旧有值
	//	//	{
	//	//		m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iOldHS);
	//	//		m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iOldVS);
	//	//	}
	//	//	else						//1 加载新的值
	//	//	{
	//	//		m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iNewHS);
	//	//		m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iNewVS);
	//	//	}
	//	//	m_pp1IconSapcing.OnEnChangeEdit(0, 0, CWindow());
	//	//}

	//	return 0;
	//}

	LRESULT OnLoadDefaultAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		// 所有属性页重新加载已更新的数据
		m_pp1FontSet.m_bReInit = TRUE;
		m_pp2Readme.m_bReInit = TRUE;		//已解决该项重新加载报错问题。这项只读，可不重新加载。

		if (m_pp1FontSet.m_hWnd)			//检查属性页是否存在
		{
			m_pp1FontSet.OnInitDialog(NULL, NULL);
		}

		SetActivePage(GetActiveIndex());		//刷新当前属性页

		return 0;
	}

	//void OnReloadSetFromFile(UINT uCode, int nID, HWND hwncCtrl)
	LRESULT OnLoadSetFromFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		m_pp2Readme.m_bReInit = TRUE;		//已解决该项重新加载报错问题。这项只读，可不重新加载。
		m_pp1FontSet.m_bReInit = TRUE;

		SetActivePage(GetActiveIndex());		//刷新当前属性页
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
