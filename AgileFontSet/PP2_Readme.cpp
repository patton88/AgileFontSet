// PP2_Readme.cpp: implementation of the PP2_Readme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP2_Readme.h"

//替换RadminMinfo类的IDC_EDIT_README的窗口过程。当在IDC_EDIT_README的窗口中按回车，便关闭属性表单对话框
WNDPROC winproc_prev_Readme = NULL;
LRESULT CALLBACK WinProc_Readme(
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
		return winproc_prev_Readme(hwnd, uMsg, wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction

PP2_Readme::PP2_Readme() : m_bReInit(FALSE)
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL PP2_Readme::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	m_strReadme.Empty();

	//这里编译报错：error C2065: 'GWL_WNDPROC': undeclared identifier，是因为VS2017默认是x64编译，改为x86编译便不会报错
	//替换RadminMinfo类的IDC_EDIT_README的窗口过程。当在IDC_EDIT_README的窗口中按回车，便关闭属性表单对话框
	if (NULL == winproc_prev_Readme)
		winproc_prev_Readme = (WNDPROC)::SetWindowLong(GetDlgItem(IDC_EDIT_README), GWL_WNDPROC, (LONG)WinProc_Readme);

	// Init dialog resizing
	DlgResize_Init();

	//m_str = L"test";
	GetChinsesHelp();
	//GetEnglishHelp();

	DoDataExchange();
	//::SetFocus(GetDlgItem(IDC_EDIT5));	//无效

	//对话框中各控件都已有Static Text文本提示，无需再有鼠标悬停提示
	//m_editReadme.SetTooltipText(L"ChromePortable.exe程序相关说明");

	////为属性页添加菜单成功，无效
	//CMenu menu;
	//menu.LoadMenuW(IDR_MENU1);
	//SetMenu(menu);
	//menu.Detach();		//用局部变量必须Detach()
	//Invalidate();		//刷新显示

	//return TRUE;
	return FALSE;
}

BOOL PP2_Readme::OnSetActive()
{
	if (m_bReInit)
	{
		m_bReInit = FALSE;
		OnInitDialog(NULL, NULL);
	}

	BOOL fRet = CPropertyPageImpl<PP2_Readme>::OnSetActive();

	PostMessage(WM_SETPAGEFOCUS11, 0, 0L);
	//::PostMessage(GetDlgItem(IDC_EDIT_README), WM_SETFOCUS, 0, 0);		//有效。但该方式让CEdit获得焦点后光标控制有问题
	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_KEYDOWN, VK_HOME, 0);	//有效

	//----------------------------------------
	// Set the Cancel button as the default button
	//SendMessage(DM_SETDEFID, IDCANCEL);	// MFC 中用SendMessage有效，WTL 中无效
	//PostMessage(DM_SETDEFID, IDCANCEL);		// WTL 中必须用PostMessage才有效
	::PostMessageW(GetParent(), DM_SETDEFID, IDCANCEL, NULL);
	//----------------------------------------

	return fRet;
}

//LONG PP2_Readme::OnSetPageFocus(UINT wParam, LONG lParam)
LRESULT PP2_Readme::OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// IDC_BUTTON2 is the button on this property page
	// that you want to give the default focus.

	//CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
	//pEdit->SetFocus();

	/*
	WM_SETFOCUS和SetFocus()的区别

	SetFocus()完成了3件事：
	(1). 向失去键盘焦点的窗口发送WM_KILLFOCUS消息；
	(2). 向接收键盘焦点的窗口发送WM_SETFOCUS消息；
	(3). 同时激活接收焦点的窗口或其父窗口。
	所以仅发送WM_SETFOCUS消息给希望设置焦点的窗口还不行，还必须向原来有焦点的窗口发送WM_KILLFOCUS消息，
	还要向接收焦点的窗口或其父窗口发送WM_ACTIVATE消息来激活。

	设置焦点SetfFocus()函数执行会触发WM_SETFOCuS消息
	The SetFocus function sends a WM_KILLFOCUS message to the window that loses the keyboard focus and a 
	WM_SETFOCUS message to the window that receives the keyboard focus. It also activates either the window 
	that receives the focus or the parent of the window that receives the focus.

	获取焦点GetFocus()

	WM_SETFOCUS和WM_ACTIVE区别是什么？
	激活和获得焦点还是有区别的，比如你打开一个窗口，这就是激活了，但是其子窗口，按钮啊什么的也是要获得焦点的，那么这个时候就是主窗口是激活的，控件是获得焦点的。不能反着说。

	*/

	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_SETFOCUS, 0, 0);		//有效。但该方式让CEdit获得焦点后光标控制有问题
	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_KEYDOWN, VK_HOME, 0);	//有效

	/*
	默认情况下，属性表单的属性页每次显示时，若有内容的CEdit控件是第一Tabstop，
	则CEdit中内容将呈选中状态。

	要想在属性页显示时，有内容的CEdit控件获得焦点，但内容不被选中，保持上一下显示时的光标位置
	可以这样解决：

	1、在属性页中设置第一TabStop为其它控件，而非CEdit控件。
		若要设置Label控件的Tab属性需设置其Tabstop为True

	2、定义自定义消息在BOOL PP2_Readme::OnSetActive()中发送

	3、在自定义消息中将属性页的焦点设置到CEdit控件。
	
	4、若在属性页出现后，手工按Tab键将焦点切换到CEdit控件，CEdit控件的内容会被选中

	*/
	
	m_editReadme.SetFocus();	//有效。该方式CEdit获得焦点后光标控制正确
	//GetDlgItem(IDC_EDIT_README).SetFocus();	//有效。该方式CEdit获得焦点后光标控制正确
	//GetDlgItem(IDC_EDIT_README).PostMessage(EM_SETSEL, 0, 0);	//这样会将光标定位到最前面，无法保持上一次位置

	HWND hwndParent = ::GetParent(m_hWnd);
	::EnableWindow(::GetDlgItem(hwndParent, IDB_LOAD_SET), FALSE);
	::EnableWindow(::GetDlgItem(hwndParent, IDB_SAVE_SET), FALSE);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_APPLY), FALSE);
	::EnableWindow(::GetDlgItem(hwndParent, IDOK), FALSE);

	return 0;
}

//成功解决当CEdit获得焦点时去掉全选，光标定位到起点或终点
//监测文本获得焦点。响应成功
LRESULT PP2_Readme::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//关键在于：调用m_spinXXX之前，应该增加判断m_spinXXX是否是一个窗口。否则异常报错
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//必须是这格式才能正确调用，WTL中不用 -> 调用
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// 这种格式只能调用IsWindow()，调用SetSel()失败
		//只有当GetDlgItem(nID)是窗口时，才能将它绑定到edit，否则报错
		CEditImpl edit;			//运行正常，但应该放在GetFoucs事件响应中
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// 避免闪烁，先禁止重绘
		edit.SetSel(0, 0);		// 0 表示起点，-1 表示终点
		//edit.PostMessage(EM_SETSEL, 0, 0);		//与edit.SetSel(0, 0)类似
		edit.SetRedraw(TRUE);	// 允许重绘
		edit.Invalidate(TRUE);	// 重绘Edit
		edit.Detach();
	}

	return 0;
}

//修改只读CEdit的颜色
LRESULT PP2_Readme::OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
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

//修改普通CEdit的颜色
LRESULT PP2_Readme::OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
	{
		HDC hdc = (HDC)wParam;
		::SetTextColor(hdc, RGB(0, 0, 0));		//设置字体颜色
		::SetBkColor(hdc, RGB(255, 255, 255));	//设置背景色
												//::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));	//选择NULL_BRUSH，帮助编辑框最下面有灰色条带
		::SelectObject(hdc, ::GetStockObject(WHITE_BRUSH));		//选择WHITE_BRUSH，帮助编辑框最下面便没有灰色条带
		bHandled = TRUE;
		return (HRESULT)::GetCurrentObject(hdc, OBJ_BRUSH);
	}

	return 0;
}

void PP2_Readme::GetChinsesHelp()
{
	m_strReadme =
		L"  AgileFontSet v2.0 程序说明\r\n\
  迅捷字体设置程序v2.0 - ybmj@vip.163.com 20180830\r\n\
\r\n\
  (1)、程序用途：快捷设置Windows系统字体和桌面图标间距\r\n\
  WinXP、Win7用户可在系统提供的高级外观设置里修改系统字体和桌面图标间距，但是，Win8、Win10系统却没有提供这一功能。熟悉系统的用户虽然可以通过修改注册表、bat、vbs等方式调整系统字体和桌面图标间距，但这些方法都需要注销后重新登录、或重启计算机才能生效。\r\n\
  该程序 AgileFontSet 便是快捷设置Windows系统字体和桌面图标间距的小程序，中文名为“迅捷字体设置程序”。该程序可以加载和保存Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100等多种用户配置。该程序提供窗口界面、命令行两种使用方式。窗口界面简洁快捷、一目了然，普通用户也可轻松使用，并且，设置后无需注销或重启便可立即生效；命令行方式可用于WinPE等需要自动设置系统字体和桌面图标间距的情况。\r\n\
\r\n\
  (2)、使用环境\r\n\
  该程序采用VS2017 WTL10 x86编译(Windows Template Library，http://wtl.sourceforge.net/ )，程序短小精悍、可独立运行，支持32位和64位的WinXp、Vista、Win7、Win8.x、Win10及相应Windows Server版等操作系统。\r\n\
\r\n\
  (3)、窗口界面快捷设置系统字体和桌面图标间距\r\n\
  在Windows中直接运行 AgileFontSet 便可启动窗口界面，启动后会显示系统字体和桌面图标间距的当前值，设置对话框简洁快捷、一目了然，普通用户也可快速方便地设置系统字体和桌面图标间距，并且可以立即生效。在窗口界面，用户可以直观地加载和保存默认配置、Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100等多种用户配置。\r\n\
\r\n\
  (4)、命令行方式设置系统字体和桌面图标间距\r\n\
  在WinPE等需要自动设置系统字体和桌面图标间距的时候，可以使用命令行方式。用户可按以下格式的参数启动程序，所有参数都不分大小写。更改系统字体和桌面图标间距后程序将立即刷新桌面。加方括号[]的为可选参数，加圆括号()的为只能单独使用的可选独立参数，(无参)是不带任何参数。\r\n\
  [path] ：指定加载ini配置文件的路径，启动后将显示ini配置文件中的默认配置，注意：含空格的路径必须用双引号包围；\r\n\
  [path -xxx] ：指定加载ini配置文件的路径path，-xxx 参数选择ini文件中的xxx配置。xxx可以是：Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100 之一；\r\n\
  [path -hide] ：指定加载ini配置文件的路径path，-hide 参数指定后台加载ini配置文件中的默认配置；\r\n\
  [path -xxx -hide] ：指定加载ini配置文件的路径path，-xxx 参数选择ini文件中的xxx配置，-hide 参数指定进行后台设置。3个参数组合使用，可以后台加载ini配置文件中的xxx配置；\r\n\
  (-?) ：显示设置对话框和此帮助信息(当前选项卡)，可显示系统字体和桌面图标间距的当前值；\r\n\
  (无参) ：显示设置对话框(当前选项卡)和此帮助信息，可显示系统字体和桌面图标间距的当前值。\r\n\
\r\n\
  (5)、程序使用实例\r\n\
  ＃ 指定加载ini配置文件\"D:\\Program Files\\User Data\\myFontSet.ini\"，启动后将显示ini配置文件中的默认配置，注意：含空格的路径必须用双引号包围。\r\n\
  AgileFontSet \"D:\\Program Files\\User Data\\myFontSet.ini\"\r\n\
\r\n\
  ＃ 指定加载ini配置文件D:\\myFontSet.ini，启动后将选择ini文件中的UserPreset1配置。\r\n\
  AgileFontSet D:\\myFontSet.ini -UserPreset1\r\n\
\r\n\
  ＃ 指定后台设置ini配置文件D:\\myFontSet.ini中的默认配置。\r\n\
  AgileFontSet D:\\myFontSet.ini -hide\r\n\
\r\n\
  ＃ 指定后台设置ini配置文件D:\\myFontSet.ini中的Win10Preset配置。\r\n\
  AgileFontSet D:\\myFontSet.ini -Win10Preset -hide\r\n\
\r\n\
  ＃ 显示设置对话框和此帮助信息(当前选项卡)，获取系统字体和桌面图标间距的当前值。\r\n\
  AgileFontSet -?\r\n\
\r\n\
  ＃ 显示设置对话框(当前选项卡)和此帮助信息，获取系统字体和桌面图标间距的当前值。\r\n\
  AgileFontSet\r\n\
\r\n\
  (6)、免责申明\r\n\
  用户可自行斟酌选用该程序，若转载请注明出处。对一切后果，作者不承担任何责任！\r\n\
\r\n\
";

}

void PP2_Readme::GetEnglishHelp()
{
	m_strReadme +=
L"  IconSpacing.exe v1.0 Readme\r\n\
  by ybmj@vip.163.com\r\n\
  \r\n\
  IconSpacing is used to set Icon Horizontal Spacing and Icon Vertical Spacing of windows desktop. \
You can use the following parameters in the following order when launching the program. \
All parameters is case insensitive. IconSpacing will refresh desktop after changing \
Icon Horizontal Spacing and Icon Vertical Spacing, Unless the -N parameter is added.\r\n\
  -Hxxx : Set Icon Horizontal Spacing to xxx. xxx is 3-digit integer between 0 and 150 (Windows 7 metric).\r\n\
  -Vxxx : Set Icon Vertical Spacing to xxx. xxx is 3-digit integer between 0 and 150 (Windows 7 metric).\r\n\
  [-N] : [Optional] Do not refresh desktop after changing Icon Horizontal Spacing and Icon Vertical Spacing.\r\n\
  (-?) or (Default) : (Alone) Display setting dialog and this help message.\r\n\
\r\n\
Examples:\r\n\
  # Set Icon Horizontal Spacing to 60 and Icon Vertical Spacing to 47.\r\n\
  IconSpacing.exe -H60 -V47\r\n\
\r\n\
  # Set Icon Horizontal Spacing to 80 and Icon Vertical Spacing to 50, and refresh desktop after change.\r\n\
  IconSpacing.exe -H76 -V48\r\n\
\r\n\
  # Set Icon Horizontal Spacing to 76 and Icon Vertical Spacing to 48, and do not refresh desktop \
after change.\r\n\
  IconSpacing.exe -H76 -V48 -N\r\n\
\r\n\
  # Display setting dialog and this help message.\r\n\
  IconSpacing.exe -?\r\n\
  \r\n\
";
}
