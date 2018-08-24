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
	::SetWindowText(GetDlgItem(IDOK), L"Ӧ��");		// Ӧ�����ã��˳����ý���
	::SetWindowText(GetDlgItem(IDCANCEL), L"�˳�");		// �����棬�˳����ý���

	CRect rectNew, rectOK, rectCancel, rectWnd, rectTab;
	int nWidthBTN, nIntervalBTN, nFix;

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
	
	//����::ScreenToClient(::GetParent(m_hWnd), &rectOK);	::ScreenToClient(::GetParent(m_hWnd), &rectTab);

	rectNew = rectCancel;
	//rectNew.left = rectWnd.left + (rectWnd.right - rectCancel.right);
	rectNew.left = rectTab.left;
	rectNew.right = rectNew.left + nWidthBTN;

	// �ָ��������ã����˳����ý���
	//rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	//rectNew.MoveToX(rectTab.left + 20);
	m_btnLoadOldValue.Create(m_hWnd, rectNew, L"��������", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDB_LOAD_SET, (LPVOID)IDB_LOAD_SET);
	m_btnLoadOldValue.SetFont(GetFont());

	// �ָ��µ����ã����˳����ý���
	rectNew.MoveToX(rectNew.right + nIntervalBTN);
	//rectNew.MoveToX(rectNew.left + 145);
	m_btnLoadNewValue.Create(m_hWnd, rectNew, L"��������", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
		WS_TABSTOP, NULL, IDB_SAVE_SET, (LPVOID)IDB_SAVE_SET);
	m_btnLoadNewValue.SetFont(GetFont());

	// Ӧ�����ã����˳����ý���
	//rectNew.MoveToX(rectOK.left - (rectNew.Width() + nIntervalBTN));
	////rectNew.MoveToX(rectNew.left + 265);
	//m_btnApply.Create(m_hWnd, rectNew, L"Ӧ��", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE |
	//	WS_TABSTOP, NULL, IDC_BTN_APPLY, (LPVOID)IDC_BTN_APPLY);
	//m_btnApply.SetFont(GetFont());


	//SetWindowPos�������ÿؼ�Tab˳����ȫ�ɹ�
	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDCANCEL), m_hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDOK) , GetDlgItem(IDCANCEL), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);//wndTop������ΪZ˳������!)
	::SetWindowPos(GetDlgItem(IDC_BTN_APPLY), GetDlgItem(IDOK), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDB_SAVE_SET), GetDlgItem(IDC_BTN_APPLY), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	::SetWindowPos(GetDlgItem(IDB_LOAD_SET), GetDlgItem(IDB_SAVE_SET), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

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

	//PostMessageW(WM_SETFOCUS, IDCANCEL, NULL);	//��Ч

	//Ϊ���Ա���Ӳ˵��ɹ�������ҳ�޷���Ӳ˵�
	//CMenu menu;
	//menu.LoadMenuW(IDR_MENU1);
	//SetMenu(menu);
	//menu.Detach();		//�þֲ���������Detach()
	//Invalidate();		//ˢ����ʾ
}

//���浱ǰҳ�޸ģ����˳����ý���
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
