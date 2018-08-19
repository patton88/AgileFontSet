// PP3_FontSet.cpp: implementation of the PP3_FontSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP1_FontSet.h"

extern DWORD GetWinOsName(CString& strOsName);
extern CString itos(int i);

//替换RadminMinfo类的IDC_EDIT_README的窗口过程。当在IDC_EDIT_README的窗口中按回车，便关闭属性表单对话框
WNDPROC winproc_prev_PP1 = NULL;
LRESULT CALLBACK WinProc_AllSetText2(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	if (uMsg == WM_CHAR && wParam == 0x0d)
	{
		::PostMessage(::GetParent(::GetParent(hwnd)), WM_CLOSE, 0, 0);
		return 1;
	}
	else
	{
		return winproc_prev_PP1(hwnd, uMsg, wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction

PP1_FontSet::PP1_FontSet() : m_bReInit(FALSE)
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL PP1_FontSet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	m_strChrome.Empty();

	//替换RadminMinfo类的IDC_EDIT_README的窗口过程。当在IDC_EDIT_README的窗口中按回车，便关闭属性表单对话框
	//if (NULL == winproc_prev_My2)
	//	winproc_prev_My2 = (WNDPROC)::SetWindowLong(GetDlgItem(IDC_EDIT_AllSetText), GWL_WNDPROC, (LONG)WinProc_AllSetText2);

	// Init dialog resizing
	DlgResize_Init();

	DoDataExchange();

	//调用WinAPI获得当前桌面图标间距
	ICONMETRICSW im;
	im.cbSize = sizeof(ICONMETRICSW);	//这个非常重要，否则下面函数调用将返回0，即ret=0,说明函数调用失败
	int ret = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);
	m_iOldHS = im.iHorzSpacing - 32;
	m_iOldVS = im.iVertSpacing - 32;

	//Windows内建控件CSpinButtonCtrl的WTL封装类为：CUpDownCtrl
	//GetWindowRect(&rect);
	m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
	//Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
	m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
	m_spinHS.SetRange(0, 150);
	m_spinHS.SetPos(m_iOldHS);
	//MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);

	m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
	//Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
	m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
	m_spinVS.SetRange(0, 150);
	m_spinVS.SetPos(m_iOldVS);

	//显示Windows系统版本号
	CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);

	return FALSE;
}

//发现这种思路存在逻辑错误。不用消息响应。只需增加一个按钮即可
//监测文本改变
LRESULT PP1_FontSet::OnEnChangeEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	static int iHS = -1;
	static int iVS = -1;

	//关键在于：调用m_spinXXX之前，应该增加判断m_spinXXX是否是一个窗口。否则异常报错
	if (m_spinHS.IsWindow() && m_spinVS.IsWindow())
	{
		HWND hwndParent = ::GetParent(m_hWnd);
		if ((m_spinHS.GetPos() == m_iOldHS) && (m_spinVS.GetPos() == m_iOldVS))		//1	m_btnLoadDefaultCurrPage
		{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), FALSE); }
		else
		{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), TRUE); }

		//if ((-1 == m_iNewHS) || (m_spinHS.GetPos() == m_iNewHS) && (m_spinVS.GetPos() == m_iNewVS))	//2	m_btnLoadDefaultCurrPage
		//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), FALSE); }
		//else
		//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), TRUE); }

		//每次输入都获取并且判断 大于提示重置
		wchar_t sz[256] = { 0 };
		::GetWindowTextW(GetDlgItem(nID), sz, 256);
		int len = wcslen(sz);

		//WTL只允许输入0-150之间的数值
		if (len > 3
			|| 1 == len && (sz[0] < 48 || sz[0] > 57)
			|| 2 == len && (sz[1] < 48 || sz[1] > 57)
			|| 3 == len && (sz[2] < 48 || sz[2] > 57)
			|| _wtoi(sz) < 0 || _wtoi(sz) > 150)
		{
			//MessageBoxW(L"请输入1到150之间的数值");
			//MessageBoxW(g_lang.TranslateString(IDS_REC_UPDATES));
			if(nID == IDC_EDIT_HS)
				::SetWindowTextW(GetDlgItem(nID), (-1 == iHS) ? itos(m_iOldHS) : itos(iHS));
			else
				::SetWindowTextW(GetDlgItem(nID), (-1 == iVS) ? itos(m_iOldVS) : itos(iVS));

			//::SetWindowTextW(GetDlgItem(nID), (nID == IDC_EDIT_HS) ? itos(m_iOldHS) : itos(m_iOldVS));
			::SendMessageW(GetDlgItem(nID), WM_KEYDOWN, VK_END, 0);
		}
		else		//保存最新合法数值
		{
			if(nID == IDC_EDIT_HS)
				iHS = _wtoi(sz);
			else
				iVS = _wtoi(sz);
		}
	}

	return 0;
}

//成功解决当CEdit获得焦点时去掉全选，光标定位到起点或终点
//监测文本获得焦点。响应成功
LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//必须是这格式才能正确调用，WTL中不用 -> 调用
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// 这种格式只能调用IsWindow()，调用SetSel()失败
		//只有当GetDlgItem(nID)是窗口时，才能将它绑定到edit，否则报错
		CEditImpl edit;			//运行正常，但应该放在GetFoucs事件响应中
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// 避免闪烁，先禁止重绘
		edit.SetSel(-1, -1);		// 0 表示起点，-1 表示终点
		edit.SetRedraw(TRUE);	// 允许重绘
		edit.Invalidate(TRUE);	// 重绘Edit
		edit.Detach();
	}


	return 0;
}


BOOL PP1_FontSet::OnSetActive()
{
	if (m_bReInit)
	{
		m_bReInit = FALSE;
		OnInitDialog(NULL, NULL);
	}

	BOOL fRet = CPropertyPageImpl<PP1_FontSet>::OnSetActive();

	PostMessage(WM_SETPAGEFOCUS17, 0, 0L);	//必须响应此消息才能调用OnSetPageFocus()禁用启用按钮

	// Set the Cancel button as the default button
	::PostMessageW(GetParent(), DM_SETDEFID, IDCANCEL, NULL);

	return fRet;
}

//LONG PP3_FontSet::OnSetPageFocus(UINT wParam, LONG lParam)
LRESULT PP1_FontSet::OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HWND hwndParent = ::GetParent(m_hWnd);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_DEFAULT_ALL), TRUE);	//0 m_btnLoadDefaultAll;		
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_RELOAD_SET_FROM_FILE), TRUE);	//2	m_btnReloadSetFromFile;	
	if ((m_spinHS.GetPos() == m_iOldHS) && (m_spinVS.GetPos() == m_iOldVS))		//1	m_btnLoadDefaultCurrPage
	{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), FALSE); }
	else
	{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), TRUE); }

	//if ((-1 == m_iNewHS) || (m_spinHS.GetPos() == m_iNewHS) && (m_spinVS.GetPos() == m_iNewVS))	//2	m_btnLoadDefaultCurrPage
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), FALSE); }
	//else
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), TRUE); }

	::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_APPLY), TRUE);				//3	m_btnSaveCurrPage;		
	::EnableWindow(::GetDlgItem(hwndParent, IDOK), TRUE);						//4	m_btnSaveAll;			

	::SendMessage(GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//发送一个End按键，取消选中状态

	return 0;
}
