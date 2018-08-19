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
	//����ģ̬CPropertySheet��ΪCDialog����һCPropertySheet�Ӵ��ڣ��ڽ����л�ʱ����������

	//WS_EX_CONTROLPARENT = 0x00010000
	//������屾������˲���Ի��򵼺����Ӵ���.���ʹ�����������, �Ի���������������������Ӵ���,
	//��ִ�е�������ʱ, ���簴סTAB��, �����.

	//P.S.�����л��������Կ�(modeless)����, ����CPropertySheet::OnInitDialog�����
	// Override OnInitDialog() for the child CPropertySheet, and add the WS_EX_CONTROLPARENT style.
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	//MessageBoxW(L"Test");


	//----------------------------------------
	// Set the Cancel button as the default button
	//SendMessage(DM_SETDEFID, IDCANCEL);	// MFC ����SendMessage��Ч��WTL ����Ч
	PostMessage(DM_SETDEFID, IDCANCEL);		// WTL �б�����PostMessage����Ч

	// ������PP0_PropertySheet::OnSheetInitialized()�����������������⣬
	// ���������Ա��ĸ�����ҳ��OnSetActive()�����������䣬�����ڵ��������ҳʱ����ͬ����ȱʡ��ť
	//		::PostMessageW(GetParent(), DM_SETDEFID, IDCANCEL, NULL);
	//----------------------------------------

	return FALSE;
}

	//����ģ̬CPropertySheet��ΪCDialog����һCPropertySheet�Ӵ��ڣ��ڽ����л�ʱ����������
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

	// OK��Cancel ��ť����
	::SetWindowText(GetDlgItem(IDOK), L"ȫ��Ӧ��");		// Ӧ�����ã��˳����ý���
	::SetWindowText(GetDlgItem(IDCANCEL), L"�˳�");		// �����棬�˳����ý���

	CRect rectNew, rectOK, rectCancel, rectWnd, rectTab;
	int nWidthBTN, nIntervalBTN;

	//Get button sizes and positions

	//GetDlgItem(IDOK)->GetWindowRect(rectOK);
	//::GetWindowRect(GetDlgItem(IDOK), &rectOK);
	//GetTabControl()->GetWindowRect(rectTab);
	//::GetWindowRect(GetTabControl(), &rectTab);		//��ȡѡ�RECT
	//ScreenToClient(rectOK); ScreenToClient(rectTab);	//�½��İ�ťλ�ò��ԣ�����Ϊ�Ի����Զ������ߴ�
	//ScreenToClient(rectOK); ScreenToClient(rectTab);	//�½��İ�ťλ�ò��ԣ�����Ϊ�Ի����Զ������ߴ�
	//::GetWindowRect(GetTabControl(), &rectTab);		//��ȡѡ�RECT

	::GetWindowRect(m_hWnd, &rectWnd);			//��ȡ���Ա�RECT
	ScreenToClient(rectWnd);
	::GetWindowRect(GetDlgItem(IDOK), &rectOK);
	ScreenToClient(rectOK);
	::GetWindowRect(GetDlgItem(IDCANCEL), &rectCancel);
	ScreenToClient(rectCancel);
	::GetWindowRect(GetTabControl(), &rectTab);
	ScreenToClient(rectTab);

	// ������ť��С������Ӧ���ֲ�ͬdpi
	rectCancel.left += 10;
	rectCancel.right = rectTab.right - 1;
	//rectCancel.left -= 5;
	//rectCancel.right -= 20;
	GetDlgItem(IDCANCEL).MoveWindow(rectCancel);

	//nIntervalBTN = rectCancel.left - rectOK.right;
	nIntervalBTN = 20;
	nWidthBTN = rectCancel.Width();

	rectOK.left += 20;	//rectOK��ť��С20
	//rectOK.right -= 20;
	//rectOK.left -= 0;
	//rectOK.right -= 20;
	//::MoveWindow(GetDlgItem(IDOK), rectOK);
	//rectOK.MoveToX(rectOK.right - nIntervalBTN);	//��rectOK.Left = ����
	rectOK.MoveToX(rectCancel.left - nIntervalBTN - rectOK.Width());
	GetDlgItem(IDOK).MoveWindow(rectOK);
	
	//����::ScreenToClient(::GetParent(m_hWnd), &rectOK);	::ScreenToClient(::GetParent(m_hWnd), &rectTab);


	rectNew = rectCancel;
	//rectNew.left = rectWnd.left + (rectWnd.right - rectCancel.right);
	rectNew.left = rectTab.left;
	rectNew.right = rectNew.left + nWidthBTN;

	// �ָ��������ã����˳����ý���
	//rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	//rectNew.MoveToX(rectTab.left + 20);
	m_btnLoadOldValue.Create(m_hWnd, rectNew, L"��������", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDC_BTN_LOAD_SET_FROM_FILE, (LPVOID)IDC_BTN_LOAD_SET_FROM_FILE);
	m_btnLoadOldValue.SetFont(GetFont());

	// �ָ��µ����ã����˳����ý���
	rectNew.MoveToX(rectNew.right + nIntervalBTN);
	//rectNew.MoveToX(rectNew.left + 145);
	m_btnLoadNewValue.Create(m_hWnd, rectNew, L"��������", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDC_BTN_SAVE_SET_TO_FILE, (LPVOID)IDC_BTN_SAVE_SET_TO_FILE);
	m_btnLoadNewValue.SetFont(GetFont());

	// Ӧ�����ã����˳����ý���
	rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	//rectNew.MoveToX(rectNew.left + 265);
	m_btnApply.Create(m_hWnd, rectNew, L"Ӧ��", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDC_BTN_APPLY, (LPVOID)IDC_BTN_APPLY);
	m_btnApply.SetFont(GetFont());


	//SetWindowPos�������ÿؼ�Tab˳����ȫ�ɹ�
	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDCANCEL), m_hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDOK) , GetDlgItem(IDCANCEL), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);//wndTop������ΪZ˳������!)
	::SetWindowPos(GetDlgItem(IDC_BTN_APPLY), GetDlgItem(IDOK), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDC_BTN_SAVE_SET_TO_FILE), GetDlgItem(IDC_BTN_APPLY), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDC_BTN_LOAD_SET_FROM_FILE), GetDlgItem(IDC_BTN_SAVE_SET_TO_FILE), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

	//m_btnApplyQuit.Attach(GetDlgItem(IDOK));	//�˳���������ϵͳ�Ѿ��󶨸ÿؼ�
	//m_btnCancle.Attach(GetDlgItem(IDCANCEL));	//�˳���������ϵͳ�Ѿ��󶨸ÿؼ�

	//m_btnApplyQuit.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);//wndTop������ΪZ˳������!)
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

	//Ϊ���Ա���Ӳ˵��ɹ�������ҳ�޷���Ӳ˵�
	//CMenu menu;
	//menu.LoadMenuW(IDR_MENU1);
	//SetMenu(menu);
	//menu.Detach();		//�þֲ���������Detach()
	//Invalidate();		//ˢ����ʾ
}

//���浱ǰҳ�޸ģ����˳����ý���
LRESULT PP0_PropertySheet::OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
{
	int nActiveIndex = GetActiveIndex();

	m_pp1FontSet.DoDataExchange(TRUE);		//ȱʡΪTRUE���ؼ�to��Ա����

	//Ӧ�����ã�ˢ������
	SetIconSpacing(m_pp1FontSet.m_spinHS.GetPos(), m_pp1FontSet.m_spinVS.GetPos(), TRUE);

	return true;

}

// Ӧ�����ã��˳����ý���
LRESULT PP0_PropertySheet::OnApplyQuit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//OnApply(wNotifyCode, wID, hWndCtl, bHandled);	// Ӧ������
	this->PostMessageW(WM_CLOSE);					// �˳����ý��档��PostMessageW��������к���ɨβ����

	return true;
}

//Ӧ�����ã�ˢ������
LRESULT PP0_PropertySheet::SetIconSpacing(int iHS, int iVS, BOOL bRefresh)
{
	int ret;

	//����WinAPI��������ͼ����
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

	//����ǳ���Ҫ���������溯�����ý�����0����ret=0,˵����������ʧ��
	//ncm.cbSize = sizeof(NONCLIENTMETRICS);
	im.cbSize = sizeof(ICONMETRICSW);

	//BOOL WINAPI SystemParametersInfo(__in UINT uiAction,__in UINT uiParam, __inout  PVOID pvParam, __in UINT fWinIni)
	//BOOL WINAPI SystemParametersInfo(
	//	__in     UINT uiAction,
	//	__in     UINT uiParam,
	//	__inout  PVOID pvParam,
	//	__in     UINT fWinIni
	//);
	//__in     UINT uiAction��ָ��Ҫ���õĲ������ο�uAction������
	//__in     UINT uiParam���ο�uAction������
	//__inout  PVOID pvParam�������õ��õ�Integer��Long�����ݽṹ��
	//__in     UINT fWinIni��������涨��������ϵͳ������ʱ���Ƿ�Ӧ�����û����ò�����


	//��ȡ����ICONMETRICSWֵ
	//SystemParametersInfo����ֵLong�������ʾ�ɹ������ʾʧ�ܡ�������GetLastError
	//ret����1�������ʾ�ɹ�
	ret = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);

	im.iHorzSpacing = iHS + 32;
	im.iVertSpacing = iVS + 32;

	//����5���������ͼ����ˢ�¶���Ч
	//����һ����SPIF_SENDCHANGE       SPIF_SENDWININICHANGE
	//ret����1�������ʾ�ɹ�

	//BOOL WINAPI SystemParametersInfo(__in UINT uiAction,__in UINT uiParam, __inout PVOID pvParam, __in UINT fWinIni)
	//uAction����������ο�SystemParametersInfo����
	//SPI_ICONHORIZONTALSPACING		��pvParamΪNULL����uiParam��������ͼ���µ�ˮƽ������룬������Ϊ��λ
	//SPI_ICONVERTICALSPACING		��SPI_ICONHORIZONTALSPACING���ƣ�ֻ����ָ��ͼ��Ĵ�ֱ���


	//�⼸�ֺ�Ҳ�У��޸�����ͼ��������������Ч
	//�޸�����ͼ����
	ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, iHS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, iVS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//ˢ�����棬������Ч
	if (bRefresh)
	{
		ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	}
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, NULL);		//����
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE);	//����
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_SENDWININICHANGE);	//����

	//ֻ��������ϣ��޸�����ͼ��������������Ч
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//�����޸Ĳ��ɹ�
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//�����޸Ĳ��ɹ�
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//�����޸Ĳ��ɹ�
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, NULL);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, NULL);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);


	//���ڳɹ��ˣ��޸�����ͼ���࣬������Ч
	//ret = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, vecIconSpacing[0] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, vecIconSpacing[1] + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//ret = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);	//����
	//ret = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, 
	//	SPI_ICONVERTICALSPACING | SPI_ICONHORIZONTALSPACING | SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);	//����


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

//��ȡ��ǰͼ����
LRESULT PP0_PropertySheet::GetIconSpacing(vector<int>& vecIconSpacing)
{
	int nRet;

	//����WinAPI��������ͼ����
	ICONMETRICSW im;

	//����ǳ���Ҫ���������溯�����ý�����0����ret=0,˵����������ʧ��
	im.cbSize = sizeof(ICONMETRICSW);

	//��ȡ����ICONMETRICSWֵ
	//SystemParametersInfo����ֵLong�������ʾ�ɹ������ʾʧ�ܡ�������GetLastError
	//ret����1�������ʾ�ɹ�
	nRet = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);

	vecIconSpacing[0] = im.iHorzSpacing - 32;	//Win7��������λ����Ҫ-32
	vecIconSpacing[1] = im.iVertSpacing - 32;	//Win7��������λ����Ҫ-32

	if (vecIconSpacing[0] >= 0 && vecIconSpacing[0] <= 150 &&	// �������ֶ�λ��0-150֮��
		vecIconSpacing[1] >= 0 && vecIconSpacing[1] <= 150)
	{	//��ȡ��ǰͼ����ɹ�
		nRet = 1;
	}
	else
	{
		nRet = 0;
	}

	return nRet;
}

//���浽�ļ�
BOOL PP0_PropertySheet::SaveAllSetToFile()
{
	return true;
}
