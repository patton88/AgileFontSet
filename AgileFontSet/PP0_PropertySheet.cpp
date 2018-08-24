// OptionsSheet.cpp: implementation of the PP0_PropertySheet class.
#include "stdafx.h"
#include "resource.h"
#include "PP0_PropertySheet.h"

PP0_PropertySheet::PP0_PropertySheet(_U_STRINGorID title, UINT uStartPage, HWND hWndParent) :
	CPropertySheetImpl<PP0_PropertySheet>(title, uStartPage, hWndParent), m_bCentered(false)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP;

	//AddPage(m_pp1IconSapcing);
	AddPage(m_pp1FontSet);
	AddPage(m_pp2Readme);
}

//BOOL PP0_PropertySheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
BOOL PP0_PropertySheet::OnSheetInitialized()
{
	//处理模态CPropertySheet作为CDialog或另一CPropertySheet子窗口，在焦点切换时死锁的问题

	//WS_EX_CONTROLPARENT = 0x00010000
	//这个窗体本身包含了参与对话框导航的子窗口.如果使用了这个参数, 对话框管理器连入这个窗体的子窗口,
	//当执行导航操作时, 比如按住TAB键, 方向键.

	//P.S.焦点切换后子属性框(modeless)僵死, 请在CPropertySheet::OnInitDialog中添加
	// Override OnInitDialog() for the child CPropertySheet, and add the WS_EX_CONTROLPARENT style.
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	//MessageBoxW(L"Test");


	//----------------------------------------
	// Set the Cancel button as the default button
	//SendMessage(DM_SETDEFID, IDCANCEL);	// MFC 中用SendMessage有效，WTL 中无效
	PostMessage(DM_SETDEFID, IDCANCEL);		// WTL 中必须用PostMessage才有效

	// 除了在PP0_PropertySheet::OnSheetInitialized()函数中添加上面语句外，
	// 还需在属性表单的各属性页的OnSetActive()函数中添加语句，才能在点击各属性页时设置同样的缺省按钮
	//		::PostMessageW(GetParent(), DM_SETDEFID, IDCANCEL, NULL);
	//----------------------------------------

	return FALSE;
}

	//处理模态CPropertySheet作为CDialog或另一CPropertySheet子窗口，在焦点切换时死锁的问题
	/*
	https://support.microsoft.com/en-us/kb/149501

	PRB: Child CPropertySheet Hangs If Focus Is Switched
		This article was previously published under Q149501
		This article has been archived. It is offered "as is" and will no longer be updated.

	SYMPTOMS
		If a modeless CPropertySheet is a child of a CDialog or another CPropertySheet, 
			the program will hang in the following situations :
		A. The focus is switched back to a PropertyPage that previously had the focus.
		B. The focus is placed on a control on a PropertyPage, and then the focus is switched to another
			window(for example, Program Manager).
		C. The focus is on a control on a PropertyPage and the PropertySheet is closed.
		The program hangs because the child CPropertySheet continuously receives a WM_GETDLGCODE message.

	CAUSE
		By default, CPropertyPages have a WS_EX_CONTROLPARENT style. However, CPropertySheets do not
			have this style. This style allows a user to press the TAB key to move from a control
			inside the page to one in the sheet.

		When the focus is switched from the CPropertyPage, code that handles default command buttons
			loops through all the controls in the pages and the sheet. Cycling through controls is
			done using GetNextDlgTabItem(). The loop cycles through controls in the page in the child
			CPropertySheet and finds its way to controls in the parent page or parent dialog. At this point,
			GetNextDlgTabItem() is not able to find controls inside the child CPropertySheet because it
			doesn't have a WS_EX_CONTROLPARENT style. The loop never ends because it never finds the
			original control that had the focus.
	
	RESOLUTION
		Override OnInitDialog() for the child CPropertySheet, and add the WS_EX_CONTROLPARENT style.
	
	STATUS
		This is a problem with the implementation of the Property Sheet common control, and not MFC's
			CPropertySheet wrapper.

	Sample Code
		// CMySheet is derived from CPropertySheet
		BOOL CMySheet::OnInitDialog()
		{
			ModifyStyleEx (0, WS_EX_CONTROLPARENT);
			return CPropertySheet::OnInitDialog();
		}
		// Compile options needed: default

	Properties
		Article ID : 149501 - Last Review : 12 / 04 / 2015 14 : 30 : 11 - Revision : 2.1

		Applies to
		Microsoft Foundation Class Library 4.2
		Microsoft Visual C++ 4.0 Standard Edition
	Keywords :
	kbnosurvey kbarchive kbprb kbpropsheet kbuidesign KB149501
	*/


void PP0_PropertySheet::OnShowWindow(BOOL bShowing, int nReason)
{
	if (bShowing && !m_bCentered)
	{
		m_bCentered = true;
		CenterWindow(m_psh.hwndParent);
	}

	// OK、Cancel 按钮改名
	::SetWindowText(GetDlgItem(IDOK), L"应用");		// 应用设置，退出设置界面
	::SetWindowText(GetDlgItem(IDCANCEL), L"退出");		// 不保存，退出设置界面

	CRect rectNew, rectOK, rectCancel, rectWnd, rectTab;
	int nWidthBTN, nIntervalBTN, nFix;

	//Get button sizes and positions

	//GetDlgItem(IDOK)->GetWindowRect(rectOK);
	//::GetWindowRect(GetDlgItem(IDOK), &rectOK);
	//GetTabControl()->GetWindowRect(rectTab);
	//::GetWindowRect(GetTabControl(), &rectTab);		//获取选项卡RECT
	//ScreenToClient(rectOK); ScreenToClient(rectTab);	//新建的按钮位置不对，是因为对话框自动调整尺寸
	//ScreenToClient(rectOK); ScreenToClient(rectTab);	//新建的按钮位置不对，是因为对话框自动调整尺寸
	//::GetWindowRect(GetTabControl(), &rectTab);		//获取选项卡RECT

	::GetWindowRect(m_hWnd, &rectWnd);			//获取属性表单RECT
	ScreenToClient(rectWnd);
	::GetWindowRect(GetDlgItem(IDOK), &rectOK);
	ScreenToClient(rectOK);
	::GetWindowRect(GetDlgItem(IDCANCEL), &rectCancel);
	ScreenToClient(rectCancel);
	::GetWindowRect(GetTabControl(), &rectTab);
	ScreenToClient(rectTab);

	// 调整按钮大小，以适应各种不同dpi
	//nIntervalBTN = rectCancel.left - rectOK.right;
	nFix = 5;
	nIntervalBTN = 25;
	nWidthBTN = rectCancel.Width() - nFix;

	rectCancel.right = rectTab.right - 2;
	rectCancel.left = rectCancel.right - nWidthBTN;
	GetDlgItem(IDCANCEL).MoveWindow(rectCancel);

	rectOK.left = rectOK.right - nWidthBTN;
	rectOK.MoveToX(rectCancel.left - nIntervalBTN - rectOK.Width());
	GetDlgItem(IDOK).MoveWindow(rectOK);
	
	//不行::ScreenToClient(::GetParent(m_hWnd), &rectOK);	::ScreenToClient(::GetParent(m_hWnd), &rectTab);

	rectNew = rectCancel;
	//rectNew.left = rectWnd.left + (rectWnd.right - rectCancel.right);
	rectNew.left = rectTab.left;
	rectNew.right = rectNew.left + nWidthBTN;

	// 恢复旧有设置，不退出设置界面
	//rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	//rectNew.MoveToX(rectTab.left + 20);
	m_btnLoadOldValue.Create(m_hWnd, rectNew, L"加载设置", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDB_LOAD_SET, (LPVOID)IDB_LOAD_SET);
	m_btnLoadOldValue.SetFont(GetFont());

	// 恢复新的设置，不退出设置界面
	rectNew.MoveToX(rectNew.right + nIntervalBTN);
	//rectNew.MoveToX(rectNew.left + 145);
	m_btnLoadNewValue.Create(m_hWnd, rectNew, L"保存设置", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDB_SAVE_SET, (LPVOID)IDB_SAVE_SET);
	m_btnLoadNewValue.SetFont(GetFont());

	// 应用设置，不退出设置界面
	//rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	////rectNew.MoveToX(rectNew.left + 265);
	//m_btnApply.Create(m_hWnd, rectNew, L"应用", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
	//	WS_TABSTOP, NULL, IDC_BTN_APPLY, (LPVOID)IDC_BTN_APPLY);
	//m_btnApply.SetFont(GetFont());


	//SetWindowPos代码设置控件Tab顺序完全成功
	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDCANCEL), m_hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDOK) , GetDlgItem(IDCANCEL), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);//wndTop是设置为Z顺序的最顶部!)
	::SetWindowPos(GetDlgItem(IDC_BTN_APPLY), GetDlgItem(IDOK), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDB_SAVE_SET), GetDlgItem(IDC_BTN_APPLY), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDB_LOAD_SET), GetDlgItem(IDB_SAVE_SET), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

	//m_btnApplyQuit.Attach(GetDlgItem(IDOK));	//退出报错，估计系统已经绑定该控件
	//m_btnCancle.Attach(GetDlgItem(IDCANCEL));	//退出报错，估计系统已经绑定该控件

	//m_btnApplyQuit.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);//wndTop是设置为Z顺序的最顶部!)
	//m_btnApply.SetWindowPos(m_btnApplyQuit, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	//m_btnLoadNewValue.SetWindowPos(m_btnApply, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	//m_btnLoadOldValue.SetWindowPos(m_btnLoadNewValue, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	//m_btnCancle.SetWindowPos(m_btnLoadOldValue, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

	//m_btnCancle.Detach();
	//m_btnCancle.Detach();

	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	//SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	//SetIcon(hIconSmall, FALSE);

	SetIcon(hIcon);

	//PostMessageW(WM_SETFOCUS, IDCANCEL, NULL);	//有效

	//为属性表单添加菜单成功。属性页无法添加菜单
	//CMenu menu;
	//menu.LoadMenuW(IDR_MENU1);
	//SetMenu(menu);
	//menu.Detach();		//用局部变量必须Detach()
	//Invalidate();		//刷新显示
}

//保存当前页修改，不退出设置界面
LRESULT PP0_PropertySheet::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nActiveIndex = GetActiveIndex();

	m_pp1FontSet.OnSet(wNotifyCode, wID, hWndCtl, bHandled);

	return true;

}

LRESULT PP0_PropertySheet::OnLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iRet = m_pp1FontSet.OnLoad(wNotifyCode, wID, hWndCtl, bHandled);

	return iRet;
}

LRESULT PP0_PropertySheet::OnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iRet = m_pp1FontSet.OnSave(wNotifyCode, wID, hWndCtl, bHandled);

	return iRet;
}
