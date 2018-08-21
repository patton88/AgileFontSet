// PP1_FontSet.cpp: implementation of the PP3_FontSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP1_FontSet.h"
#include "CommonFun.h"

extern DWORD GetWinOsName(CString& strOsName);
extern CString itos(int i);

//用全局变量 g_ThreadParam[11] 代替 new ThreadParam。避免在子线程异常时造成无法delete lpParameter;
//ThreadParam g_ThreadParam[11];
NONCLIENTMETRICSW* g_pFontMetrics;	//_beginthreadex需要用全局变量

unsigned _stdcall setOnThread(void *p)
{
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICSW),
		g_pFontMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	return 0;
}

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
	m_nOldHS = im.iHorzSpacing - 32;
	m_nOldVS = im.iVertSpacing - 32;

	//当设置CEdit控件随对话框自动缩放后，spin控件与CEdit关联的代码放在这里OnInitDialog()中过早，会引起控件错位
	////Windows内建控件CSpinButtonCtrl的WTL封装类为：CUpDownCtrl
	////GetWindowRect(&rect);
	//m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
	////Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
	//m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
	//m_spinHS.SetRange(0, 150);
	//m_spinHS.SetPos(m_nOldHS);
	////MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);

	//m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
	////Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
	//m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
	//m_spinVS.SetRange(0, 150);
	//m_spinVS.SetPos(m_nOldVS);

	//显示Windows系统版本号
	CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);


	//noMeiryoUI235Dlg::OnInitDialog
	//////////////////////////////////////////////////////////////////////////
	FillMemory(&m_metrics, sizeof(NONCLIENTMETRICSW), 0x00);
	FillMemory(&m_metricsAll, sizeof(NONCLIENTMETRICSW), 0x00);
	FillMemory(&m_iconFont, sizeof(LOGFONTW), 0x00);
	FillMemory(&m_iconFontAll, sizeof(LOGFONTW), 0x00);

	//我们将对每种国家语言进行判断，并根据每种国家语言进行初始化。
	initializeLocale();

	// 若没有指定命令行ini设置文件
	if (m_strSettingFile.IsEmpty()) {
		getActualFont();
	}

	//处理菜单的禁用启用
	if (!use7Compat) {
		// 在Windows 7或更早版本的情况下，无法更改字体大小的处理模式。
		CheckMenuItem(GetMenu(), IDM_COMPAT7, MF_BYCOMMAND | MF_UNCHECKED);
		EnableMenuItem(GetMenu(), IDM_COMPAT7, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	CheckMenuItem(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND | MF_CHECKED);

	//显示Windows系统版本号
	//CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);

	// 更新字体名称显示
	theUpdateDisplay();


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
		if ((m_spinHS.GetPos() == m_nOldHS) && (m_spinVS.GetPos() == m_nOldVS))		//1	m_btnLoadDefaultCurrPage
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
				::SetWindowTextW(GetDlgItem(nID), (-1 == iHS) ? itos(m_nOldHS) : itos(iHS));
			else
				::SetWindowTextW(GetDlgItem(nID), (-1 == iVS) ? itos(m_nOldVS) : itos(iVS));

			//::SetWindowTextW(GetDlgItem(nID), (nID == IDC_EDIT_HS) ? itos(m_nOldHS) : itos(m_nOldVS));
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
//LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
//{
//	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//必须是这格式才能正确调用，WTL中不用 -> 调用
//	{
//		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// 这种格式只能调用IsWindow()，调用SetSel()失败
//		//只有当GetDlgItem(nID)是窗口时，才能将它绑定到edit，否则报错
//		CEditImpl edit;			//运行正常，但应该放在GetFoucs事件响应中
//		edit.Attach(GetDlgItem(nID));
//		edit.SetRedraw(FALSE);	// 避免闪烁，先禁止重绘
//		edit.SetSel(-1, -1);		// 0 表示起点，-1 表示终点
//		edit.SetRedraw(TRUE);	// 允许重绘
//		edit.Invalidate(TRUE);	// 重绘Edit
//		edit.Detach();
//	}
//
//
//	return 0;
//}


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
	//当设置CEdit控件随对话框自动缩放后，spin控件与CEdit关联的代码放在这里才行。放在OnInitDialog()中过早，会引起控件错位
	//Windows内建控件CSpinButtonCtrl的WTL封装类为：CUpDownCtrl
	//GetWindowRect(&rect);
	if (!m_spinHS.IsWindow())
	{
		m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
		//Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
		m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
		m_spinHS.SetRange(0, 150);
		m_spinHS.SetPos(m_nOldHS);
		//MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);
	}
	if (!m_spinVS.IsWindow())
	{
		m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
		//Spin控件绑定Edit控件后，Spin控件将占用Edit控件的宽度，若运行时编辑框太窄，可到资源管理器中调大编辑框宽度
		m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
		m_spinVS.SetRange(0, 150);
		m_spinVS.SetPos(m_nOldVS);
	}

	HWND hwndParent = ::GetParent(m_hWnd);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_DEFAULT_ALL), TRUE);	//0 m_btnLoadDefaultAll;		
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_RELOAD_SET_FROM_FILE), TRUE);	//2	m_btnReloadSetFromFile;	
	//if ((m_spinHS.GetPos() == m_nOldHS) && (m_spinVS.GetPos() == m_nOldVS))		//1	m_btnLoadDefaultCurrPage
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), FALSE); }
	//else
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), TRUE); }

	//if ((-1 == m_iNewHS) || (m_spinHS.GetPos() == m_iNewHS) && (m_spinVS.GetPos() == m_iNewVS))	//2	m_btnLoadDefaultCurrPage
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), FALSE); }
	//else
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), TRUE); }

	::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_APPLY), TRUE);				//3	m_btnSaveCurrPage;		
	::EnableWindow(::GetDlgItem(hwndParent, IDOK), TRUE);						//4	m_btnSaveAll;			

	::SendMessage(GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//发送一个End按键，取消选中状态

	return 0;
}

LRESULT PP1_FontSet::OnCheckAllFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);		//控件to成员变量。缺省为FALSE-变量到控件

	if (m_iCheckAllfont)
	{
		m_chkTitle.EnableWindow(FALSE);
		m_chkIcon.EnableWindow(FALSE);
		m_chkMenu.EnableWindow(FALSE);
		m_chkMessage.EnableWindow(FALSE);
		m_chkPalette.EnableWindow(FALSE);
		m_chkTip.EnableWindow(FALSE);
	}
	else
	{
		m_chkTitle.EnableWindow(TRUE);
		m_chkIcon.EnableWindow(TRUE);
		m_chkMenu.EnableWindow(TRUE);
		m_chkMessage.EnableWindow(TRUE);
		m_chkPalette.EnableWindow(TRUE);
		m_chkTip.EnableWindow(TRUE);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// noMeiryoUI235Dlg.cpp

//成功解决当CEdit获得焦点时去掉全选，光标定位到起点或终点
//监测文本获得焦点。响应成功
LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//关键在于：调用m_spinXXX之前，应该增加判断m_spinXXX是否是一个窗口。否则异常报错
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//必须是这格式才能正确调用，WTL中不用 -> 调用
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// 这种格式只能调用IsWindow()，调用SetSel()失败
		//只有当GetDlgItem(nID)是窗口时，才能将它绑定到edit，否则报错
		CEditImpl edit;			//运行正常，但应该放在GetFoucs事件响应中
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// 避免闪烁，先禁止重绘
		edit.SetSel(-1, -1);		// 0 表示起点，-1 表示终点
									//edit.PostMessage(EM_SETSEL, 0, 0);		//与edit.SetSel(0, 0)类似
		edit.SetRedraw(TRUE);	// 允许重绘
		edit.Invalidate(TRUE);	// 重绘Edit
		edit.Detach();

		//::SendMessageW(GetDlgItem(nID), WM_KEYDOWN, VK_END, 0);		//光标定位到末尾
	}

	return 0;
}

//修改只读CEdit的颜色
LRESULT PP1_FontSet::OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	//if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
	if (IsCtrl((HWND)lParam, L"Edit"))				//判断是否是CEdit控件
	{
		//::SetTextColor(hdc, RGB(255, 0, 0));		//设置文字颜色为红色，运行后可查看是否有效果
		::SetTextColor(hdc, RGB(0, 0, 0));			//设置文字颜色
		::SetBkColor(hdc, RGB(255, 255, 255));		//设置文字背景色

		::SelectObject(hdc, ::GetStockObject(DC_BRUSH));	//选择NULL_BRUSH，帮助编辑框最下面有灰色条带
		SetDCBrushColor(hdc, RGB(255, 255, 255));	//设置编辑框背景色
													//::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));	//选择NULL_BRUSH，帮助编辑框最下面有灰色条带
													//::SelectObject(hdc, ::GetStockObject(WHITE_BRUSH));		//选择WHITE_BRUSH，帮助编辑框最下面便没有灰色条带

		bHandled = TRUE;
		return (HRESULT)::GetCurrentObject(hdc, OBJ_BRUSH);
	}

	//return (HRESULT)::GetCurrentObject(hdc, RGB(155, 155, 255));
	return (HRESULT)::GetCurrentObject(hdc, RGB(255, 255, 255));
}

BOOL PP1_FontSet::IsCtrl(HWND hWnd, CString str)
{
	wchar_t szClassName[MAX_PATH];
	GetClassName(hWnd, szClassName, MAX_PATH);

	//CString::Find		在一个较大的字符串中查找字符或子字符串
	//返回值：	返回此CString对象中与需要的子字符串或字符匹配的第一个字符的从零开始的索引；
	//		如果没有找到子字符串或字符则返回 - 1。
	if (-1 == str.Find(szClassName))
	{
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumeration
}

//获取字体大小。
int PP1_FontSet::getFontSize(int iFontHight)
{
	//获得字体大小。从字体高度H到字体大小S需要进行换算，
	//H = -(S * dpiY / 72)
	//S = -(H * 72 / dpiY)
	//MulDiv(a, b, c) 就是计算 a * b / c，结果四舍五入
	//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

	//获取DPI。
	int dpiY = getDPI();
	int iSzie = -(iFontHight * 72 / dpiY);
	return iSzie;
}

//获取当前设置的字体。
void PP1_FontSet::getActualFont(void)
{
	//
	// 获取单个字体的信息
	//

	// 获取图标以外的字体信息。
	m_metrics.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICSW),
		&m_metrics,
		0);
	m_metricsOld = m_metrics;	//保存一份到m_metricsOld

	// 获取图标的字体信息。
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONTW),
		&m_iconFont,
		0);
	m_iconFontOld = m_iconFont;	//保存一份到m_metricsOld

	//保存当前字体信息。参考OnSet8的读取顺序
	// 保存当前字体名称
	tagFontCur.vecFaces[0].second = m_metrics.lfCaptionFont.lfFaceName;
	tagFontCur.vecFaces[1].second = m_iconFont.lfFaceName;
	tagFontCur.vecFaces[2].second = m_metrics.lfSmCaptionFont.lfFaceName;
	tagFontCur.vecFaces[3].second = m_metrics.lfStatusFont.lfFaceName;
	tagFontCur.vecFaces[4].second = m_metrics.lfMessageFont.lfFaceName;
	tagFontCur.vecFaces[5].second = m_metrics.lfMenuFont.lfFaceName;

	// 保存当前字体大小。从字体高度H到字体大小S需要进行换算
	tagFontCur.vecSizes[0].second = getFontSize(m_metrics.lfCaptionFont.lfHeight);
	tagFontCur.vecSizes[1].second = getFontSize(m_iconFont.lfHeight);
	tagFontCur.vecSizes[2].second = getFontSize(m_metrics.lfSmCaptionFont.lfHeight);
	tagFontCur.vecSizes[3].second = getFontSize(m_metrics.lfStatusFont.lfHeight);
	tagFontCur.vecSizes[4].second = getFontSize(m_metrics.lfMessageFont.lfHeight);
	tagFontCur.vecSizes[5].second = getFontSize(m_metrics.lfMenuFont.lfHeight);

	// 保存当前字体字符集
	tagFontCur.vecCharset[0].second = m_metrics.lfCaptionFont.lfCharSet;
	tagFontCur.vecCharset[1].second = m_iconFont.lfCharSet;
	tagFontCur.vecCharset[2].second = m_metrics.lfSmCaptionFont.lfCharSet;
	tagFontCur.vecCharset[3].second = m_metrics.lfStatusFont.lfCharSet;
	tagFontCur.vecCharset[4].second = m_metrics.lfMessageFont.lfCharSet;
	tagFontCur.vecCharset[5].second = m_metrics.lfMenuFont.lfCharSet;

	//
	// 获取所有字体的信息
	//

	// 获取图标以外的字体信息。
	m_metricsAll.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICSW),
		&m_metricsAll,
		0);

	// 获取图标的字体信息。
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONTW),
		&m_iconFontAll,
		0);

	// 将菜单字体的信息应用于其他字体的信息。
	m_metricsAll.lfStatusFont = m_metricsAll.lfMenuFont;
	m_metricsAll.lfMessageFont = m_metricsAll.lfMenuFont;
	m_metricsAll.lfCaptionFont = m_metricsAll.lfMenuFont;
	m_metricsAll.lfSmCaptionFont = m_metricsAll.lfMenuFont;
	m_iconFontAll = m_metricsAll.lfMenuFont;
}

/**
* 僼僅儞僩昞帵傪峏怴偡傞丅
*/
void PP1_FontSet::theUpdateDisplay(void)
{
	// 僼僅儞僩柤丄億僀儞僩悢昞帵暥帤楍傪嶌惉偡傞丅
	m_strAllFontName = m_metricsAll.lfMenuFont.lfFaceName;
	m_strAllFontName += L"   " + itos(getFontPointInt(&(m_metricsAll.lfMenuFont), m_hWnd)) + L"pt";

	m_strTitleFontName = m_metrics.lfCaptionFont.lfFaceName;
	m_strTitleFontName += L"   " + itos(getFontPointInt(&(m_metrics.lfCaptionFont), m_hWnd)) + L"pt";

	m_strIconFontName = m_iconFont.lfFaceName;
	m_strIconFontName += L"   " + itos(getFontPointInt(&m_iconFont, m_hWnd)) + L"pt";

	m_strPaletteFontName = m_metrics.lfSmCaptionFont.lfFaceName;
	m_strPaletteFontName += L"   " + itos(getFontPointInt(&m_metrics.lfSmCaptionFont, m_hWnd)) + L"pt";

	m_strTipFontName = m_metrics.lfStatusFont.lfFaceName;
	m_strTipFontName += L"   " + itos(getFontPointInt(&m_metrics.lfStatusFont, m_hWnd)) + L"pt";

	m_strMessageFontName = m_metrics.lfMessageFont.lfFaceName;
	m_strMessageFontName += L"   " + itos(getFontPointInt(&m_metrics.lfMessageFont, m_hWnd)) + L"pt";

	// 儊僯儏乕偲慖戰崁栚
	m_strMenuFontName = m_metrics.lfMenuFont.lfFaceName;
	m_strMenuFontName += L"   " + itos(getFontPointInt(&m_metrics.lfMenuFont, m_hWnd)) + L"pt";

	//UpdateData(false);
	DoDataExchange();

	// 慖戰偟偨僼僅儞僩傪僥僉僗僩儃僢僋僗偵愝掕偡傞丅
	if (m_fontAll != NULL) {
		DeleteObject(m_fontAll);
	}
	m_fontAll = createFont(&m_metricsAll.lfMenuFont);
	m_editAllFont.SetFont(m_fontAll);

	if (m_fontTitle != NULL) {
		DeleteObject(m_fontTitle);
	}
	m_fontTitle = createFont(&m_metrics.lfCaptionFont);
	m_editTitleFont.SetFont(m_fontTitle);

	if (m_fontIcon != NULL) {
		DeleteObject(m_fontIcon);
	}
	m_fontIcon = createFont(&m_iconFont);
	m_editIconFont.SetFont(m_fontIcon);

	if (m_fontPalette != NULL) {
		DeleteObject(m_fontPalette);
	}
	m_fontPalette = createFont(&m_metrics.lfSmCaptionFont);
	m_editPaletteFont.SetFont(m_fontPalette);

	if (m_fontTip != NULL) {
		DeleteObject(m_fontTip);
	}
	m_fontTip = createFont(&m_metrics.lfStatusFont);
	m_editTipFont.SetFont(m_fontTip);

	if (m_fontMessage != NULL) {
		DeleteObject(m_fontMessage);
	}
	m_fontMessage = createFont(&m_metrics.lfMessageFont);
	m_editMessageFont.SetFont(m_fontMessage);

	if (m_fontMenu != NULL) {
		DeleteObject(m_fontMenu);
	}
	m_fontMenu = createFont(&m_metrics.lfMenuFont);
	m_editMenuFont.SetFont(m_fontMenu);
}

int PP1_FontSet::getFontPointInt(LOGFONTW *font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	bool WIN8_SIZE = FALSE;
	if (WIN8_SIZE) {
		// Windows 8ディスプレイコントロールパネル互換
		if ((point > 10) || (point < 8)) {
			return (int)point;
		}
		else {
			// 10ptまではWindows 7と同様に計算する。
			// Windows 7以前互換 
			if (point - abs((int)point) > 0.49) {
				return (int)point + 1;
			}
			else {
				return (int)point;
			}
		}
	}
	else {
		// Windows 7以前互換 
		if (point - abs((int)point) > 0.49) {
			return (int)point + 1;
		}
		else {
			return (int)point;
		}
	}
}

/**
*计算与字体像素数对应的点数。
*（Windows 7 / 通用对话框兼容）
*
* @param		font 字体信息
* @param		hWnd Window句柄
* @return	字体大小
*/
double PP1_FontSet::getFontPoint(LOGFONTW *font, HWND hWnd)
{
	//创建一个字体
	HFONT hFont = CreateFontIndirect(font);

	//从自己的窗口句柄创建的设备上下文
	//设置字体。
	//HDC dc = GetDC(hWnd);
	//SelectFont(dc, hFont);
	CClientDC dc(m_hWnd);
	HFONT prevFont = dc.SelectFont(hFont);

	//从设备上下文中获取TEXTMETRIC。
	TEXTMETRIC metric;
	GetTextMetrics(dc, &metric);

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);

	//CClientDC：(1)（客户区设备上下文）用于客户区的输出，与特定窗口关联，
	//	可以让开发者访问目标窗口中客户区，其构造函数中包含了GetDC, 析构函数中包含了ReleaseDC
	//ReleaseDC(hWnd, dc);
	DeleteObject(hFont);

	int height;
	if (font->lfHeight < 0) {
		//如果为负，则lfHeight是字体本身的高度。
		height = 0 - font->lfHeight;
	}
	else if (font->lfHeight > 0) {
		//如果是正数，我们已经包含内部前导并减去该数额。
		height = font->lfHeight - metric.tmInternalLeading;
	}
	else {
		//如果为0，则从您创建的字体中获取默认大小。
		height = metric.tmAscent + metric.tmDescent - metric.tmInternalLeading;
	}

	double point = (double)height * 72 / logPixelY;

	return point;
}

#define APP_FONTSIZE 11		//11偏大
#define MY_APP_FONTSIZE 9
HFONT PP1_FontSet::createFont(LOGFONTW *font)
{
	//HDC hDC = GetDC(m_hWnd);
	CClientDC dc(m_hWnd);

	HFONT newFont = CreateFont(
		-MulDiv(MY_APP_FONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72),
		0,
		0,
		0,
		font->lfWeight,
		font->lfItalic,
		font->lfUnderline,
		font->lfStrikeOut,
		font->lfCharSet,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		font->lfFaceName);

	//ReleaseDC(m_hWnd, hDC);

	return newFont;
}

//LRESULT PP1_FontSet::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
//	dlg.DoModal();
//	return 0;
//}

//设定 按钮响应 IDOK
LRESULT PP1_FontSet::OnSet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//EndDialog(wID);
	//theSetFont(&m_metrics, &m_iconFont);

	DoDataExchange(TRUE);		//控件to成员变量。缺省为FALSE-变量到控件

	if (m_iCheckAllfont)
	{
		theSetFont(&m_metricsAll, &m_iconFontAll);

		memcpy(&m_metrics, &m_metricsAll, sizeof(NONCLIENTMETRICSW));
		memcpy(&m_iconFont, &m_iconFontAll, sizeof(LOGFONTW));

		theUpdateDisplay();
	}
	else if (m_iCheckTitle || m_iCheckIcon || m_iCheckMenu || m_iCheckMessage || m_iCheckPalette || m_iCheckTip)
	{
		LOGFONTW iconFont = m_iconFontOld;
		NONCLIENTMETRICSW metrics = m_metricsOld;
		if (m_iCheckTitle) metrics.lfCaptionFont = m_metrics.lfCaptionFont;
		if (m_iCheckIcon) iconFont = m_iconFont;
		if (m_iCheckMenu) metrics.lfMenuFont = m_metrics.lfMenuFont;
		if (m_iCheckMessage) metrics.lfMessageFont = m_metrics.lfMessageFont;
		if (m_iCheckPalette) metrics.lfSmCaptionFont = m_metrics.lfSmCaptionFont;
		if (m_iCheckTip) metrics.lfStatusFont = m_metrics.lfStatusFont;

		//应用字体设置，刷新桌面
		theSetFont(&metrics, &iconFont);
	}

	//应用图标间距设置，刷新桌面
	if (m_iCheckHS || m_iCheckVS)
	{
		if (m_iCheckHS) {
			unsigned nHS = m_spinHS.GetPos();
			if (nHS >= 0 && nHS <= 150) m_nOldHS = nHS;
		}
		if (m_iCheckVS) {
			unsigned nVS = m_spinVS.GetPos();
			if (nVS >= 0 && nVS <= 150) m_nOldVS = nVS;
		}
		SetIconSpacing(m_nOldHS, m_nOldVS);
	}

	return 0;
}

//设定并退出 按钮响应 IDM_SET_EXIT
LRESULT PP1_FontSet::OnSetExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//theSetFont(&m_metrics, &m_iconFont);
	//EndDialog(wID);

	return 0;
}

//设定全部 按钮响应 ID_SET_ALL
LRESULT PP1_FontSet::OnSetAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#if 0
	// 岆偭偰廲彂偒梡僼僅儞僩傪巜掕偟側偄傛偆栤偄崌傢偣傪峴偆丅
	if (metricsAll.lfMenuFont.lfFaceName[0] == _T('@')) {
		int answer = MessageBox(hWnd,
			L"廲彂偒梡僼僅儞僩(柤慜偑@偱巒傑傞僼僅儞僩)偑\n巜掕偝傟偰偄傑偡偑傛傠偟偄偱偡偐丠"),
			L"妋擣"),
			MB_ICONQUESTION | MB_YESNO);
			if (answer != IDYES) {
				return;
			}
	}
#endif

	// 僼僅儞僩曄峏傪幚巤偡傞丅
	theSetFont(&m_metricsAll, &m_iconFontAll);

	memcpy(&m_metrics, &m_metricsAll, sizeof(NONCLIENTMETRICSW));
	memcpy(&m_iconFont, &m_iconFontAll, sizeof(LOGFONTW));

	// 昞帵傪峏怴偡傞丅
	theUpdateDisplay();

	return 0;
}

//LRESULT PP1_FontSet::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	EndDialog(wID);
//	return 0;
//}

LRESULT PP1_FontSet::OnSelFont(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	DoDataExchange(TRUE);	//控件to成员变量，以备后面恢复。缺省为FALSE-变量到控件。

	LOGFONTW logFont;
	FillMemory(&logFont, sizeof(LOGFONTW), 0x00);

	try {//保留先前的字体
		LPLOGFONTW pTargetFont = mapSelFont[wID].second;		//代替1个20多行的switch (type)语句

		CFontDialog dlg;
		dlg.SetLogFont(pTargetFont);		//加载先前的字体
		if (IDOK == dlg.DoModal())
		{
			logFont = *dlg.m_cf.lpLogFont;
		}
	}
	catch (...) {
		::MessageBox(m_hWnd, L"Internal error in font selection dialog.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	enum fontType type = mapSelFont[wID].first;	//代替1个20多行的switch (wID)语句
	if (logFont.lfFaceName[0] != _T('\0')) {
		switch (type) {
		case allFont:
			//m_metricsAll.lfMenuFont = logFont;
			m_metricsAll.lfStatusFont = logFont;
			m_metricsAll.lfMessageFont = logFont;
			m_metricsAll.lfCaptionFont = logFont;
			m_metricsAll.lfSmCaptionFont = logFont;
			m_iconFontAll = logFont;

			ChangeFont(m_metricsAll.lfMenuFont, logFont, m_strAllFontName, m_fontAll, m_editAllFont);
			break;
		case titleFont:
			ChangeFont(m_metrics.lfCaptionFont, logFont, m_strTitleFontName, m_fontTitle, m_editTitleFont);
			break;
		case iconFont:
			ChangeFont(m_iconFont, logFont, m_strIconFontName, m_fontIcon, m_editIconFont);
			break;
		case paletteFont:	//lfSmCaptionFont 调色板标题Palette Title字体。small caption font
			ChangeFont(m_metrics.lfSmCaptionFont, logFont, m_strPaletteFontName, m_fontPalette, m_editPaletteFont);
			break;
		case tipFont:	//设置状态栏tooltips字体。 the font used in status bars and tooltips
			ChangeFont(m_metrics.lfStatusFont, logFont, m_strTipFontName, m_fontTip, m_editTipFont);
			break;
		case messageFont:
			ChangeFont(m_metrics.lfMessageFont, logFont, m_strMessageFontName, m_fontMessage, m_editMessageFont);
			break;
		case menuFont:
			ChangeFont(m_metrics.lfMenuFont, logFont, m_strMenuFontName, m_fontMenu, m_editMenuFont);
			break;
		}
	}

	theUpdateDisplay();

	return 0;
}

int PP1_FontSet::ChangeFont(LOGFONTW& font, LOGFONTW& fontNew, CString& strFontName, HFONT& hFont, CEditImpl& edit)
{
	font = fontNew;
	strFontName = fontNew.lfFaceName;

	DeleteObject(hFont);
	hFont = createFont(&font);
	edit.SetFont(hFont);

	return 0;
}

void PP1_FontSet::theSetFont(NONCLIENTMETRICSW *fontMetrics, LOGFONTW *iconLogFont) {

	DWORD_PTR ptr;
	LRESULT messageResult;

	// 图标字体设置
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONTW),
		iconLogFont,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETICONTITLELOGFONT,
		0, // (LPARAM)L"Environment"),
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			::MessageBox(m_hWnd, L"SendMessage timeout.", L"Information", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// 除了图标以外的字体设置。启动独立线程设定字体
	//if (GetMenuState(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND)) {
	if (1) {
		// UI和另一个线程SystemParametersInfo(SPI_SETNONCLIENTMETRICS) 运行它
		g_pFontMetrics = fontMetrics;	//初始化独立线程的全局参数

										/*
										typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);

										_Success_(return != 0)
										_ACRTIMP uintptr_t __cdecl _beginthreadex(
										_In_opt_  void*                    _Security,
										_In_      unsigned                 _StackSize,
										_In_      _beginthreadex_proc_type _StartAddress,
										_In_opt_  void*                    _ArgList,
										_In_      unsigned                 _InitFlag,
										_Out_opt_ unsigned*                _ThrdAddr
										);
										*/
										// setOnThread 必须定义为全局函数，不能定义为类成员函数。否则报错如下：
										//(698): error C3867: 'PP1_FontSet::setOnThread': 
										//	non-standard syntax; use '&' to create a pointer to member
		//启动执行字体设置的线程
		uintptr_t startResult = _beginthreadex(NULL, 0, setOnThread, NULL, 0, NULL);
		if (startResult != 0) {
			// 成功启动线程时等待线程终止。
			HANDLE handle;
			handle = (HANDLE)startResult;

			// 等待大约5秒钟
			DWORD waitResult = WaitForSingleObject(handle, 5000);
			if (waitResult == WAIT_TIMEOUT) {
				// 如果线程没有终止，我什么也做不了，所以我退出线程。
				TerminateThread(handle, 0);
			}
			CloseHandle(handle);
		}
	}
	else {
		// 用与UI相同的线程运行它
		SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICSW),
			fontMetrics,
			SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	}

	//向所有活动窗口广播系统单位更新消息
	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETNONCLIENTMETRICS,
		(LPARAM)L"WindowMetrics",
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			::MessageBox(m_hWnd, L"SendMessage timeout.", L"Information", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	//通过重置颜色刷新屏幕。
	//但是，当IObit StartMenu 8运行时，setSysColors
	//我决定不调用，因为我调用时响应会丢失。
#if 0
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1, colorItems, colors);
#endif

}

LRESULT PP1_FontSet::OnLoad(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 设置过滤器。注意：WTL中分割符为\0 ,而非 | 
	wchar_t szFilter[] = L"配置文件 (*.ini)\0*.ini\0所有文件 (*.*)\0*.*\0\0";
	CString strFilePath;
	rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 0);	//这样有效

																									// 构造打开文件对话框
	CFileDialog fileDlg(TRUE, L"ini", strFilePath, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_READONLY, szFilter, m_hWnd);

	fileDlg.m_ofn.lpstrInitialDir = strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，获得选择的文件路径  
		strFilePath = fileDlg.m_ofn.lpstrFile;					//获得打开的全路径(含文件名)
		rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);

		if (FALSE == loadFontInfo(strFilePath)) {
			::MessageBox(m_hWnd, L"无法加载字体设置", L"错误", MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			theUpdateDisplay();
		}
	}

	return 0;
}

BOOL PP1_FontSet::loadFontInfo(CString filename)
{
	BOOL loadResult;
	LOGFONT captionFont;
	LOGFONT newIconFont;
	LOGFONT smCaptionFont;
	LOGFONT statusFont;
	LOGFONT messageFont;
	LOGFONT menuFont;

	loadResult = loadFont(filename, L"TitleFont", &captionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"IconFont", &newIconFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"PaletteFont", &smCaptionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"HintFont", &statusFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"MessageFont", &messageFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"MenuFont", &menuFont);
	if (!loadResult) {
		return FALSE;
	}

	m_metrics.lfCaptionFont = captionFont;
	m_iconFont = newIconFont;
	m_metrics.lfSmCaptionFont = smCaptionFont;
	m_metrics.lfStatusFont = statusFont;
	m_metrics.lfMessageFont = messageFont;
	m_metrics.lfMenuFont = menuFont;

	return TRUE;
}

BOOL PP1_FontSet::loadFont(CString filename, CString section, LOGFONT* font)
{
	wchar_t buf[32];
	DWORD result;

	result = GetPrivateProfileString(section,
		L"FaceName",
		L"",
		font->lfFaceName,
		32,
		filename);
	if (!result) {
		return FALSE;
	}

	result = GetPrivateProfileString(section,
		L"Height",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfHeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Width",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWidth = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Escapement",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfEscapement = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Orientation",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOrientation = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Weight",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Italic",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfItalic = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Underline",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfUnderline = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"StrikeOut",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfStrikeOut = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"CharSet",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfCharSet = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"OutPrecision",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOutPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"ClipPrecision",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfClipPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"Quality",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfQuality = _ttoi(buf);

	result = GetPrivateProfileString(section,
		L"PitchAndFamily",
		L"",
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfPitchAndFamily = _ttoi(buf);

	return TRUE;
}

LRESULT PP1_FontSet::OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 设置过滤器。注意：WTL中分割符为\0 ,而非 | 
	wchar_t szFilter[] = L"配置文件 (*.ini)\0*.ini\0所有文件 (*.*)\0*.*\0\0";
	CString strFilePath, strFileName;

	// 从给定全路径获取文件名或路径。
	//iFlag:		0 返回带扩展名的文件名；1 返回不带扩展名的文件名；
	//			2 返回不带文件名的路径(末尾无斜杠)；3 返回不带文件名的路径(末尾带斜杠)
	//CString getPathPart(CString strPath, int iFlag)

	rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 0);	//这样有效
	if (strFilePath.IsEmpty())
	{
		strFilePath = getCurDir(1);		//1:	//返回当前目录(末尾无斜杠)

										//获得不含文件名的路径(末尾无斜杠)中的不重复唯一文件名称(不含扩展名)
		strFileName = getUniqFileName(strFilePath, L"mySysFontSet");
	}
	else
	{
		strFileName = getPathPart(strFilePath, 1);	//1 返回不带扩展名的文件名
		strFilePath = getPathPart(strFilePath, 2);	//2 返回不带文件名的路径(末尾无斜杠)

													//获得不含文件名的路径(末尾无斜杠)中的不重复唯一文件名称(不含扩展名)
		strFileName = getUniqFileName(strFilePath, strFileName);
	}

	// 构造保存文件对话框 | OFN_NOCHANGEDIR
	CFileDialog fileDlg(FALSE, L"ini", strFileName + L".ini", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, m_hWnd);

	fileDlg.m_ofn.lpstrInitialDir = strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，获得选择的文件路径  
		strFilePath = fileDlg.m_ofn.lpstrFile;					//获得打开的全路径(含文件名)
		rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);

		if (FALSE == startSaveFont(strFilePath)) {
			::MessageBox(m_hWnd, L"无法加载字体设置", L"错误", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	return 0;
}

BOOL PP1_FontSet::startSaveFont(CString filename)
{
	BOOL saveResult;

	//BOOL PP1_FontSet::saveFont(CString filename, CString section, LOGFONT *font)
	saveResult = saveFont(filename, L"TitleFont", &m_metrics.lfCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"IconFont", &m_iconFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"PaletteFont", &m_metrics.lfSmCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"HintFont", &m_metrics.lfStatusFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"MessageFont", &m_metrics.lfMessageFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"MenuFont", &m_metrics.lfMenuFont);
	if (!saveResult) {
		return FALSE;
	}

	return TRUE;
}

//saveResult = saveFont(filename, L"TitleFont", &m_metrics.lfCaptionFont);
BOOL PP1_FontSet::saveFont(CString filename, CString section, LOGFONT *font)
{
	BOOL bRet;

	bRet = WritePrivateProfileString(section,
		L"FaceName",
		font->lfFaceName,
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Height",
		itos(font->lfHeight),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Width",
		itos(font->lfWidth),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Escapement",
		itos(font->lfEscapement),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Orientation",
		itos(font->lfOrientation),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Weight",
		itos(font->lfWeight),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Italic",
		itos(font->lfItalic),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Underline",
		itos(font->lfUnderline),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"StrikeOut",
		itos(font->lfStrikeOut),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"CharSet",
		itos(font->lfCharSet),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"OutPrecision",
		itos(font->lfOutPrecision),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"ClipPrecision",
		itos(font->lfClipPrecision),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"Quality",
		itos(font->lfQuality),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"PitchAndFamily",
		itos(font->lfPitchAndFamily),
		filename);
	if (!bRet) {
		return FALSE;
	}

	return TRUE;
}
LRESULT PP1_FontSet::OnHelpTopic(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::MessageBox(m_hWnd, L"Help Topic Test.", L"Help Topic", MB_OK | MB_ICONINFORMATION);
	return 0;
}

LRESULT PP1_FontSet::OnAbout(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT PP1_FontSet::OnCompat7(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

int PP1_FontSet::getDPI(void)
{
	int dpiY = 96;
	CClientDC dc(m_hWnd);

	if (dc) { dpiY = GetDeviceCaps(dc, LOGPIXELSY); }
	return dpiY;
}

int PP1_FontSet::mySetFontItem(LOGFONTW& font, CString& strFaceName, LONG& lHeight, BYTE& bCharSet)
{
	memset(&font, 0, sizeof(LOGFONTW));
	//wcscpy_s(m_metrics.lfCaptionFont.lfFaceName, fontFaces8[0]);
	wcscpy_s(font.lfFaceName, strFaceName);
	//MulDiv(a, b, c) 就是计算 a * b / c，结果四舍五入
	//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);
	font.lfHeight = -MulDiv(lHeight, getDPI(), 72);
	font.lfWeight = 400;
	//m_metrics.lfCaptionFont.lfCharSet = fontCharset8[0];
	font.lfCharSet = bCharSet;
	font.lfQuality = 5;

	return 0;
}

int PP1_FontSet::mySetFont(NONCLIENTMETRICSW& metrics, LOGFONTW& iconFont, TagFont& tagFont)
{
	//为了保持除字体之外的NONCLIENTMETRICS的当前值，检索NONCLIENTMETRICS的内容。
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);

	//int SetFontItem(LOGFONTW& font, CString& strFaceName, LONG& lHeight, BYTE& bCharSet)
	mySetFontItem(metrics.lfCaptionFont, tagFont.vecFaces[0].second,
		tagFont.vecSizes[0].second, tagFont.vecCharset[0].second);

	mySetFontItem(iconFont, tagFont.vecFaces[1].second,
		tagFont.vecSizes[1].second, tagFont.vecCharset[1].second);

	mySetFontItem(metrics.lfSmCaptionFont, tagFont.vecFaces[2].second,
		tagFont.vecSizes[2].second, tagFont.vecCharset[2].second);

	mySetFontItem(metrics.lfStatusFont, tagFont.vecFaces[3].second,
		tagFont.vecSizes[3].second, tagFont.vecCharset[3].second);

	mySetFontItem(metrics.lfMessageFont, tagFont.vecFaces[4].second,
		tagFont.vecSizes[4].second, tagFont.vecCharset[4].second);

	mySetFontItem(metrics.lfMenuFont, tagFont.vecFaces[5].second,
		tagFont.vecSizes[5].second, tagFont.vecCharset[5].second);

	return 0;
}

/**
*设置Windows 8 / 8.1的预设值。
*/
LRESULT PP1_FontSet::OnSet8(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	mySetFont(m_metrics, m_iconFont, tagFontWin8);

	// 更新显示。
	theUpdateDisplay();

	return 0;
}

/**
*设置Windows 10的预设值。
*/
LRESULT PP1_FontSet::OnSet10(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	mySetFont(m_metrics, m_iconFont, tagFontWin10);

	// 更新显示。
	theUpdateDisplay();

	return 0;
}

/**
*设置Windows Current的预设值。
*/
LRESULT PP1_FontSet::OnSetCurrent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	mySetFont(m_metrics, m_iconFont, tagFontCur);

	// 更新显示。
	theUpdateDisplay();

	return 0;
}

LRESULT PP1_FontSet::OnUniqThread(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//UISetCheck(IDM_UNIQ_THREAD, !b);	//用UISetCheck()比较麻烦
	bool b = GetMenuState(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND);
	if (b)
	{
		CheckMenuItem(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		CheckMenuItem(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND | MF_CHECKED);
	}

	return 0;
}

//字体容器初始化。tag是结构体struct缩写的前缀
void PP1_FontSet::initTagFont(void)
{
	// tagFontWin8字体容器初始化
	// 字体名称容器初始化
	tagFontWin8.vecFaces.push_back(make_pair(L"CAPTION_FACE_8", L""));
	tagFontWin8.vecFaces.push_back(make_pair(L"ICON_FACE_8", L""));
	tagFontWin8.vecFaces.push_back(make_pair(L"SMALLCAPTION_FACE_8", L""));
	tagFontWin8.vecFaces.push_back(make_pair(L"STATUS_FACE_8", L""));
	tagFontWin8.vecFaces.push_back(make_pair(L"MESSAGE_FACE_8", L""));
	tagFontWin8.vecFaces.push_back(make_pair(L"MENU_FACE_8", L""));

	// 字体大小容器初始化
	tagFontWin8.vecSizes.push_back(make_pair(L"CAPTION_SIZE_8", -1));
	tagFontWin8.vecSizes.push_back(make_pair(L"ICON_SIZE_8", -1));
	tagFontWin8.vecSizes.push_back(make_pair(L"SMALLCAPTION_SIZE_8", -1));
	tagFontWin8.vecSizes.push_back(make_pair(L"STATUS_SIZE_8", -1));
	tagFontWin8.vecSizes.push_back(make_pair(L"MESSAGE_SIZE_8", -1));
	tagFontWin8.vecSizes.push_back(make_pair(L"MENU_SIZE_8", -1));

	// 字符集容器初始化
	tagFontWin8.vecCharset.push_back(make_pair("CAPTION_CHARSET_8", -1));
	tagFontWin8.vecCharset.push_back(make_pair("ICON_CHARSET_8", -1));
	tagFontWin8.vecCharset.push_back(make_pair("SMALLCAPTION_CHARSET_8", -1));
	tagFontWin8.vecCharset.push_back(make_pair("STATUS_CHARSET_8", -1));
	tagFontWin8.vecCharset.push_back(make_pair("MESSAGE_CHARSET_8", -1));
	tagFontWin8.vecCharset.push_back(make_pair("MENU_CHARSET_8", -1));

	// tagFontWin10字体容器初始化
	// 字体名称容器初始化
	tagFontWin10.vecFaces.push_back(make_pair(L"CAPTION_FACE_10", L""));
	tagFontWin10.vecFaces.push_back(make_pair(L"ICON_FACE_10", L""));
	tagFontWin10.vecFaces.push_back(make_pair(L"SMALLCAPTION_FACE_10", L""));
	tagFontWin10.vecFaces.push_back(make_pair(L"STATUS_FACE_10", L""));
	tagFontWin10.vecFaces.push_back(make_pair(L"MESSAGE_FACE_10", L""));
	tagFontWin10.vecFaces.push_back(make_pair(L"MENU_FACE_10", L""));

	// 字体大小容器初始化
	tagFontWin10.vecSizes.push_back(make_pair(L"CAPTION_SIZE_10", -1));
	tagFontWin10.vecSizes.push_back(make_pair(L"ICON_SIZE_10", -1));
	tagFontWin10.vecSizes.push_back(make_pair(L"SMALLCAPTION_SIZE_10", -1));
	tagFontWin10.vecSizes.push_back(make_pair(L"STATUS_SIZE_10", -1));
	tagFontWin10.vecSizes.push_back(make_pair(L"MESSAGE_SIZE_10", -1));
	tagFontWin10.vecSizes.push_back(make_pair(L"MENU_SIZE_10", -1));

	// 字符集容器初始化
	tagFontWin10.vecCharset.push_back(make_pair("CAPTION_CHARSET_10", -1));
	tagFontWin10.vecCharset.push_back(make_pair("ICON_CHARSET_10", -1));
	tagFontWin10.vecCharset.push_back(make_pair("SMALLCAPTION_CHARSET_10", -1));
	tagFontWin10.vecCharset.push_back(make_pair("STATUS_CHARSET_10", -1));
	tagFontWin10.vecCharset.push_back(make_pair("MESSAGE_CHARSET_10", -1));
	tagFontWin10.vecCharset.push_back(make_pair("MENU_CHARSET_10", -1));

	// tagFontCur字体容器初始化
	// 字体名称容器初始化
	tagFontCur.vecFaces.push_back(make_pair(L"CAPTION_FACE_CUR", L""));
	tagFontCur.vecFaces.push_back(make_pair(L"ICON_FACE_CUR", L""));
	tagFontCur.vecFaces.push_back(make_pair(L"SMALLCAPTION_FACE_CUR", L""));
	tagFontCur.vecFaces.push_back(make_pair(L"STATUS_FACE_CUR", L""));
	tagFontCur.vecFaces.push_back(make_pair(L"MESSAGE_FACE_CUR", L""));
	tagFontCur.vecFaces.push_back(make_pair(L"MENU_FACE_CUR", L""));

	// 字体大小容器初始化
	tagFontCur.vecSizes.push_back(make_pair(L"CAPTION_SIZE_CUR", -1));
	tagFontCur.vecSizes.push_back(make_pair(L"ICON_SIZE_CUR", -1));
	tagFontCur.vecSizes.push_back(make_pair(L"SMALLCAPTION_SIZE_CUR", -1));
	tagFontCur.vecSizes.push_back(make_pair(L"STATUS_SIZE_CUR", -1));
	tagFontCur.vecSizes.push_back(make_pair(L"MESSAGE_SIZE_CUR", -1));
	tagFontCur.vecSizes.push_back(make_pair(L"MENU_SIZE_CUR", -1));

	// 字符集容器初始化
	tagFontCur.vecCharset.push_back(make_pair("CAPTION_CHARSET_CUR", -1));
	tagFontCur.vecCharset.push_back(make_pair("ICON_CHARSET_CUR", -1));
	tagFontCur.vecCharset.push_back(make_pair("SMALLCAPTION_CHARSET_CUR", -1));
	tagFontCur.vecCharset.push_back(make_pair("STATUS_CHARSET_CUR", -1));
	tagFontCur.vecCharset.push_back(make_pair("MESSAGE_CHARSET_CUR", -1));
	tagFontCur.vecCharset.push_back(make_pair("MENU_CHARSET_CUR", -1));

}

//map<unsigned, pair<enum fontType, LPLOGFONTW>> mapSelFont;
//字体选择容器初始化。
void PP1_FontSet::initSelFont(void)
{
	mapSelFont[IDB_SEL_ALLFONT] = make_pair(allFont, &m_metricsAll.lfMenuFont);
	mapSelFont[IDB_SEL_TITLE] = make_pair(titleFont, &m_metrics.lfCaptionFont);
	mapSelFont[IDB_SEL_ICON] = make_pair(iconFont, &m_iconFont);
	mapSelFont[IDB_SEL_ICON] = make_pair(paletteFont, &m_metrics.lfSmCaptionFont);
	mapSelFont[IDB_SEL_TIP] = make_pair(tipFont, &m_metrics.lfStatusFont);
	mapSelFont[IDB_SEL_MESSAGE] = make_pair(messageFont, &m_metrics.lfMessageFont);
	mapSelFont[IDB_SEL_MENU] = make_pair(menuFont, &m_metrics.lfMenuFont);
}

//我们将对每种国家语言进行判断，并根据每种国家语言进行初始化。
void PP1_FontSet::initializeLocale(void)
{
	//字体容器、字体选择容器初始化。tag是结构体struct缩写的前缀
	initTagFont();
	initSelFont();

	CString langPath;
	int nRet;

	langPath = getCurDir(1);		//g:\MyVC2017\noMeiryoUI235\Debug		//末尾无斜杠
	langPath = langPath.Left(langPath.ReverseFind(L'\\') + 1);	//g:\MyVC2017\noMeiryoUI235\ //末尾含斜杠
	langPath = langPath + L"lang\\" + L"English.lng";	//L"G:\\MyVC2017\\noMeiryoUI235\\lang\\English.lng"

	readResourceFile(langPath);
	nRet = readFontResource8(langPath);
	if (!nRet) {
		has8Preset = false;
	}
	nRet = readFontResource10(langPath);
	if (!nRet) {
		has10Preset = false;
	}

	//设置Win7兼容风格度量菜单是否可用
	//warning C4996: 'GetVersion': was declared deprecated
	//DWORD dwVersion = GetVersion();
	//替代
	//BOOL VerifyVersionInfoW(
	//	LPOSVERSIONINFOEXW lpVersionInformation,
	//	DWORD              dwTypeMask,
	//	DWORDLONG          dwlConditionMask
	//);

	// 获得Windows系统名称和版本(支持Win95到Win10、WinNT4.0到WinSrv2016)
	//DWORD GetWinOsName(CString& strOsName)
	DWORD dwVersion = GetWinOsName(CString());

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if (major < 6) {
		// Windows XP or earlyer
		WIN8_SIZE = false;
		use7Compat = false;
	}
	else if (major == 6) {
		if (minor < 2) {
			// Windows Vista/7
			WIN8_SIZE = false;
			use7Compat = false;
		}
		else {
			// Windows 8/8.1
			WIN8_SIZE = true;
			use7Compat = true;
		}
	}
	else {
		// Windows 10 or later
		WIN8_SIZE = false;
		use7Compat = false;
	}
}

/**
*加载资源项目
*
* @param文件资源文件名
* @param键键名
* @param读取失败时的字符串
*/
void PP1_FontSet::readResourceItem(CString file, CString key, CString fallback)
{
	wchar_t buf[255];
	int len;

	//读取INI文件。 如果该文件在Unicode版本的API中是非Unicode的，它读作每种语言的字符代码文件。
	len = GetPrivateProfileString(_T("RESOURCE"), key, _T(""), buf, 255, file);
	if (len > 0) {
		langResource.push_back(buf);
	}
	else {
		langResource.push_back(fallback);
	}
}

//开始加载资源。@param文件资源文件名
void PP1_FontSet::readResourceFile(CString file)
{
	readResourceItem(file, L"FONT_FACE", L"System");
	readResourceItem(file, L"TITLE", L"No!! Meiryo UI");
	readResourceItem(file, L"MENU_FILE", L"&File");
	readResourceItem(file, L"MENU_FILE_LOAD", L"L&oad font settings...");
	readResourceItem(file, L"MENU_FILE_SAVE", L"S&ave font settings...");
	readResourceItem(file, L"MENU_FILE_SET_QUIT", L"&Set system fonts and quit");
	readResourceItem(file, L"MENU_FILE_QUIT", L"E&xit");
	readResourceItem(file, L"MENU_PRESET", L"&Preset");
	readResourceItem(file, L"MENU_PRESET_8", L"Windows 8/8.1");
	readResourceItem(file, L"MENU_PRESET_10", L"Windows 10");
	readResourceItem(file, L"MENU_TOOLS", L"&Tools");
	readResourceItem(file, L"MENU_TOOLS_THREAD", L"Set system fonts in separated thread");
	readResourceItem(file, L"MENU_TOOLS_SEVEN", L"Calculate font size same as Windows 7");
	readResourceItem(file, L"MENU_HELP", L"&Help");
	readResourceItem(file, L"MENU_HELP_HELP", L"&Help");
	readResourceItem(file, L"MENU_HELP_ABOUT", L"&About No!! Meiryo UI");
	readResourceItem(file, L"DLG_ALL_FONT", L"All fonts");
	readResourceItem(file, L"DLG_TITLE_BAR", L"Title bar");
	readResourceItem(file, L"DLG_ICON", L"Icon");
	readResourceItem(file, L"DLG_PALETTE", L"Palette title");
	readResourceItem(file, L"DLG_HINT", L"Hint");
	readResourceItem(file, L"DLG_MESSAGE", L"Message box");
	readResourceItem(file, L"DLG_MENU", L"Menu");
	readResourceItem(file, L"DLG_SELECT", L"Select...");
	readResourceItem(file, L"DLG_SET_ALL", L"Set all");
	readResourceItem(file, L"DLG_SET", L"Set");
	readResourceItem(file, L"DLG_EXIT", L"Exit");
	readResourceItem(file, L"DLG_FONT_SEL", L"Font Selection");
	readResourceItem(file, L"DLG_FONT_NAME", L"&Font name");
	readResourceItem(file, L"DLG_STYLE", L"St&yle");
	readResourceItem(file, L"DLG_SIZE", L"&Size");
	readResourceItem(file, L"DLG_UNDERLINE", L"&Under line");
	readResourceItem(file, L"DLG_STRIKE", L"S&trikethrough");
	readResourceItem(file, L"DLG_CHARSET", L"&Character set");
	readResourceItem(file, L"DLG_OK", L"OK");
	readResourceItem(file, L"DLG_CANCEL", L"Cancel");

	readResourceItem(file, L"DLG_CHARSET_ANSI", L"Ansi");
	readResourceItem(file, L"DLG_CHARSET_BALTIC", L"Baltic");
	readResourceItem(file, L"DLG_CHARSET_BIG5", L"Traditional Chinese");
	readResourceItem(file, L"DLG_CHARSET_DEFAULT", L"Default");
	readResourceItem(file, L"DLG_CHARSET_EASTEUROPE", L"East Europe");
	readResourceItem(file, L"DLG_CHARSET_GB2312", L"Simplified Chinese");
	readResourceItem(file, L"DLG_CHARSET_GREEK", L"Greek");
	readResourceItem(file, L"DLG_CHARSET_HANGUL", L"Hangul");
	readResourceItem(file, L"DLG_CHARSET_MAC", L"Mac");
	readResourceItem(file, L"DLG_CHARSET_OEM", L"OEM");
	readResourceItem(file, L"DLG_CHARSET_RUSSIAN", L"Russian");
	readResourceItem(file, L"DLG_CHARSET_SHIFTJIS", L"Japanese");
	readResourceItem(file, L"DLG_CHARSET_SYMBOL", L"Symbol");
	readResourceItem(file, L"DLG_CHARSET_TURKISH", L"Turkish");
	readResourceItem(file, L"DLG_CHARSET_VIETNAMESE", L"Vietnamese");
	readResourceItem(file, L"DLG_CHARSET_JOHAB", L"Johab Korean");
	readResourceItem(file, L"DLG_CHARSET_ARABIC", L"Arabic");
	readResourceItem(file, L"DLG_CHARSET_HEBREW", L"Hebrew");
	readResourceItem(file, L"DLG_CHARSET_THAI", L"Thai");

	readResourceItem(file, L"DLG_STYLE_NORMAL", L"Normal");
	readResourceItem(file, L"DLG_STYLE_ITALIC", L"Italic");
	readResourceItem(file, L"DLG_STYLE_BOLD", L"Bold");
	readResourceItem(file, L"DLG_STYLE_BOLD_ITALIC", L"Bold Italic");

	readResourceItem(file, L"MSG_SETTING_FILE", L"Font settings file(*.ini)");
	readResourceItem(file, L"MSG_ALL_FILE", L"All file(*.*)");
	readResourceItem(file, L"MSG_LOAD_FAIL", L"Font settings file load failed.");
	readResourceItem(file, L"MSG_SAVE_FAIL", L"Font settings file save failed.");
	readResourceItem(file, L"MSG_ERROR", L"Error");
	readResourceItem(file, L"MSG_ABOUT", L"About No!! Meiryo UI");
	readResourceItem(file, L"MSG_NO_FONT", L"Please select font.");
	readResourceItem(file, L"MSG_NO_STYLE", L"Please select style.");
	readResourceItem(file, L"MSG_NO_SIZE", L"Please select size.");
	readResourceItem(file, L"MSG_NO_CHARSET", L"Please select charset.");
	readResourceItem(file, L"MSG_TRANSLATE", L"Tatsuhiko Shoji(Tatsu)");
	readResourceItem(file, L"FONT_CHARSET", L"1");
}


/**
*加载Windows 8的字体预设资源
*
* @param文件资源文件名
* @ return 0：设置失败1：设置成功
*/
int PP1_FontSet::readFontResource8(CString file)
{
	// 字体名称容器循环赋值
	for (auto& x : tagFontWin8.vecFaces) {
		if (readFontFace(x.second, file, x.first) == 0) { return 0; }
	}

	// 字体大小容器循环赋值
	for (auto& x : tagFontWin8.vecSizes) {
		if (readFontSize(x.second, file, x.first) == 0) { return 0; }
	}

	// 字符集容器循环赋值
	for (auto& x : tagFontWin8.vecCharset) {
		if (readFontCharset(x.second, file, x.first) == 0) { return 0; }
	}

	return 1;
}

/**
*加载Windows 10的字体预设资源
*
* @param文件资源文件名
* @ return 0：设置失败1：设置成功
*/
int PP1_FontSet::readFontResource10(CString file)
{
	// 字体名称容器循环赋值
	for (auto& x : tagFontWin10.vecFaces) {
		if (readFontFace(x.second, file, x.first) == 0) { return 0; }
	}

	// 字体大小容器循环赋值
	for (auto& x : tagFontWin10.vecSizes) {
		if (readFontSize(x.second, file, x.first) == 0) { return 0; }
	}

	// 字符集容器循环赋值
	for (auto& x : tagFontWin10.vecCharset) {
		if (readFontCharset(x.second, file, x.first) == 0) { return 0; }
	}

	return 1;
}

/**
*加载资源（用于字体名称）。
*
* @param缓冲存储位置
* @param文件资源文件名
* @param键键名
*/
// 字体名称
//result = readFontFace(fontFaces8, file, _T("CAPTION_FACE_8"));		//CAPTION_FACE_8=Segoe UI
int PP1_FontSet::readFontFace(CString& buffer, CString file, CString key)
{
	wchar_t buf[255];
	int len;

	//读取INI文件。 如果该文件在Unicode版本的API中是非Unicode的，它读作每种语言的字符代码文件。
	len = GetPrivateProfileString(L"PRESET", key, L"", buf, 255, file);
	if (len > 0) {
		buffer = buf;
	}

	return len;
}

/**
*加载资源（字体大小）。
*
* @param缓冲存储位置
* @param文件资源文件名
* @param键键名
*/
int PP1_FontSet::readFontSize(LONG& buffer, CString file, CString key)
{
	int size;

	//读取INI文件。 如果该文件在Unicode版本的API中是非Unicode的，它读作每种语言的字符代码文件。
	size = GetPrivateProfileInt(L"PRESET", key, 0, file);
	buffer = size;

	return size;
}

/**
*加载资源（用于字体字符集）。
*
* @param缓冲存储位置
* @param文件资源文件名
* @param键键名
*/
int PP1_FontSet::readFontCharset(BYTE& buffer, CString file, CString key)
{
	int size;

	//读取INI文件。 如果该文件在Unicode版本的API中是非Unicode的，它读作每种语言的字符代码文件。
	size = GetPrivateProfileInt(L"PRESET", key, 1, file);
	buffer = size;

	return size;
}

//应用设置，刷新桌面
LRESULT PP1_FontSet::SetIconSpacing(unsigned iHS, unsigned iVS, BOOL bRefresh)
{
	int ret;

	//调用WinAPI设置桌面图标间距
	//NONCLIENTMETRICS ncm;
	ICONMETRICSW im;

	//typedef struct tagICONMETRICSW
	//{
	//	UINT    cbSize;
	//	int     iHorzSpacing;
	//	int     iVertSpacing;
	//	int     iTitleWrap;
	//	LOGFONTW lfFont;
	//}   ICONMETRICSW, *PICONMETRICSW, *LPICONMETRICSW;

	//这个非常重要，否则下面函数调用将返回0，即ret=0,说明函数调用失败
	//ncm.cbSize = sizeof(NONCLIENTMETRICS);
	im.cbSize = sizeof(ICONMETRICSW);

	//BOOL WINAPI SystemParametersInfo(__in UINT uiAction,__in UINT uiParam, __inout  PVOID pvParam, __in UINT fWinIni)
	//BOOL WINAPI SystemParametersInfo(
	//	__in     UINT uiAction,
	//	__in     UINT uiParam,
	//	__inout  PVOID pvParam,
	//	__in     UINT fWinIni
	//);
	//__in     UINT uiAction，指定要设置的参数。参考uAction常数表。
	//__in     UINT uiParam，参考uAction常数表。
	//__inout  PVOID pvParam，按引用调用的Integer、Long和数据结构。
	//__in     UINT fWinIni这个参数规定了在设置系统参数的时候，是否应更新用户设置参数。


	//读取当期ICONMETRICSW值
	//SystemParametersInfo返回值Long，非零表示成功，零表示失败。会设置GetLastError
	//ret返回1，非零表示成功
	ret = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);

	im.iHorzSpacing = iHS + 32;
	im.iVertSpacing = iVS + 32;

	//以下5种情况，改图标间距刷新都有效
	//二者一样：SPIF_SENDCHANGE       SPIF_SENDWININICHANGE
	//ret返回1，非零表示成功

	//BOOL WINAPI SystemParametersInfo(__in UINT uiAction,__in UINT uiParam, __inout PVOID pvParam, __in UINT fWinIni)
	//uAction常数表——请参考SystemParametersInfo函数
	//SPI_ICONHORIZONTALSPACING		如pvParam为NULL，则uiParam代表桌面图标新的水平间隔距离，以像素为单位
	//SPI_ICONVERTICALSPACING		与SPI_ICONHORIZONTALSPACING相似，只不过指定图标的垂直间距


	//这几种合也行，修改桌面图标间距后才能立即生效
	//修改桌面图标间距
	ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, iHS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, iVS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//刷新桌面，立即生效
	if (bRefresh)
	{
		ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	}
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, NULL);		//可以
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE);	//可以
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_SENDWININICHANGE);	//可以

	//只有这种组合，修改桌面图标间距后才能立即生效
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//这样修改不成功
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//这样修改不成功
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//这样修改不成功
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, NULL);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, NULL);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);


	//终于成功了，修改桌面图标间距，立即生效
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);	//不行
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, 
	//	SPI_ICONVERTICALSPACING | SPI_ICONHORIZONTALSPACING | SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);	//不行


	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS), &im, SPIF_UPDATEINIFILE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS), &im, SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS), &im, NULL);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS), &im, 0);

	//RefreshReg(HKEY_CURRENT_USER, L"Control Panel\\Desktop\\WindowMetrics", L"IconSpacing", REG_SZ);
	//RefreshReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer", L"GlobalAssocChangedCounter", REG_DWORD);
	//im.iHorzSpacing += 1;
	//im.iVertSpacing += 1;
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICS),	s_iconMetrics, SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	//SystemParametersInfo(SPI_SETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), s_fontMetrics, SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	//_endthreadex(0);

	return TRUE;
}

//获取当前图标间距
LRESULT PP1_FontSet::GetIconSpacing(vector<unsigned>& vecIS)
{
	int nRet;

	//调用WinAPI设置桌面图标间距
	ICONMETRICSW im;

	//这个非常重要，否则下面函数调用将返回0，即ret=0,说明函数调用失败
	im.cbSize = sizeof(ICONMETRICSW);

	//读取当期ICONMETRICSW值
	//SystemParametersInfo返回值Long，非零表示成功，零表示失败。会设置GetLastError
	//ret返回1，非零表示成功
	nRet = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);

	vecIS[0] = im.iHorzSpacing - 32;	//Win7风格度量单位，需要-32
	vecIS[1] = im.iVertSpacing - 32;	//Win7风格度量单位，需要-32

	if (vecIS[0] >= 0 && vecIS[0] <= 150 &&	// 两个数字都位于0-150之间
		vecIS[1] >= 0 && vecIS[1] <= 150)
	{	//获取当前图标间距成功
		nRet = 1;
	}
	else
	{
		nRet = 0;
	}

	return nRet;
}
