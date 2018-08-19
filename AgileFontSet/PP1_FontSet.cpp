// PP3_FontSet.cpp: implementation of the PP3_FontSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP1_FontSet.h"

extern DWORD GetWinOsName(CString& strOsName);
extern CString itos(int i);

//�滻RadminMinfo���IDC_EDIT_README�Ĵ��ڹ��̡�����IDC_EDIT_README�Ĵ����а��س�����ر����Ա��Ի���
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

	//�滻RadminMinfo���IDC_EDIT_README�Ĵ��ڹ��̡�����IDC_EDIT_README�Ĵ����а��س�����ر����Ա��Ի���
	//if (NULL == winproc_prev_My2)
	//	winproc_prev_My2 = (WNDPROC)::SetWindowLong(GetDlgItem(IDC_EDIT_AllSetText), GWL_WNDPROC, (LONG)WinProc_AllSetText2);

	// Init dialog resizing
	DlgResize_Init();

	DoDataExchange();

	//����WinAPI��õ�ǰ����ͼ����
	ICONMETRICSW im;
	im.cbSize = sizeof(ICONMETRICSW);	//����ǳ���Ҫ���������溯�����ý�����0����ret=0,˵����������ʧ��
	int ret = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);
	m_iOldHS = im.iHorzSpacing - 32;
	m_iOldVS = im.iVertSpacing - 32;

	//Windows�ڽ��ؼ�CSpinButtonCtrl��WTL��װ��Ϊ��CUpDownCtrl
	//GetWindowRect(&rect);
	m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
	//Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
	m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
	m_spinHS.SetRange(0, 150);
	m_spinHS.SetPos(m_iOldHS);
	//MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);

	m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
	//Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
	m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
	m_spinVS.SetRange(0, 150);
	m_spinVS.SetPos(m_iOldVS);

	//��ʾWindowsϵͳ�汾��
	CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);

	return FALSE;
}

//��������˼·�����߼����󡣲�����Ϣ��Ӧ��ֻ������һ����ť����
//����ı��ı�
LRESULT PP1_FontSet::OnEnChangeEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	static int iHS = -1;
	static int iVS = -1;

	//�ؼ����ڣ�����m_spinXXX֮ǰ��Ӧ�������ж�m_spinXXX�Ƿ���һ�����ڡ������쳣����
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

		//ÿ�����붼��ȡ�����ж� ������ʾ����
		wchar_t sz[256] = { 0 };
		::GetWindowTextW(GetDlgItem(nID), sz, 256);
		int len = wcslen(sz);

		//WTLֻ��������0-150֮�����ֵ
		if (len > 3
			|| 1 == len && (sz[0] < 48 || sz[0] > 57)
			|| 2 == len && (sz[1] < 48 || sz[1] > 57)
			|| 3 == len && (sz[2] < 48 || sz[2] > 57)
			|| _wtoi(sz) < 0 || _wtoi(sz) > 150)
		{
			//MessageBoxW(L"������1��150֮�����ֵ");
			//MessageBoxW(g_lang.TranslateString(IDS_REC_UPDATES));
			if(nID == IDC_EDIT_HS)
				::SetWindowTextW(GetDlgItem(nID), (-1 == iHS) ? itos(m_iOldHS) : itos(iHS));
			else
				::SetWindowTextW(GetDlgItem(nID), (-1 == iVS) ? itos(m_iOldVS) : itos(iVS));

			//::SetWindowTextW(GetDlgItem(nID), (nID == IDC_EDIT_HS) ? itos(m_iOldHS) : itos(m_iOldVS));
			::SendMessageW(GetDlgItem(nID), WM_KEYDOWN, VK_END, 0);
		}
		else		//�������ºϷ���ֵ
		{
			if(nID == IDC_EDIT_HS)
				iHS = _wtoi(sz);
			else
				iVS = _wtoi(sz);
		}
	}

	return 0;
}

//�ɹ������CEdit��ý���ʱȥ��ȫѡ����궨λ�������յ�
//����ı���ý��㡣��Ӧ�ɹ�
LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//���������ʽ������ȷ���ã�WTL�в��� -> ����
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// ���ָ�ʽֻ�ܵ���IsWindow()������SetSel()ʧ��
		//ֻ�е�GetDlgItem(nID)�Ǵ���ʱ�����ܽ����󶨵�edit�����򱨴�
		CEditImpl edit;			//������������Ӧ�÷���GetFoucs�¼���Ӧ��
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// ������˸���Ƚ�ֹ�ػ�
		edit.SetSel(-1, -1);		// 0 ��ʾ��㣬-1 ��ʾ�յ�
		edit.SetRedraw(TRUE);	// �����ػ�
		edit.Invalidate(TRUE);	// �ػ�Edit
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

	PostMessage(WM_SETPAGEFOCUS17, 0, 0L);	//������Ӧ����Ϣ���ܵ���OnSetPageFocus()�������ð�ť

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

	::SendMessage(GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//����һ��End������ȡ��ѡ��״̬

	return 0;
}
