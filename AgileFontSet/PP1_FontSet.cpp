// PP1_FontSet.cpp: implementation of the PP3_FontSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP1_FontSet.h"
#include "CommonFun.h"

extern DWORD GetWinOsName(CString& strOsName);
extern CString itos(int i);
extern HWND hwndPP1;

//��ȫ�ֱ��� g_ThreadParam[11] ���� new ThreadParam�����������߳��쳣ʱ����޷�delete lpParameter;
//ThreadParam g_ThreadParam[11];
NONCLIENTMETRICSW* g_pFontMetrics;	//_beginthreadex��Ҫ��ȫ�ֱ���

unsigned _stdcall setOnThread(void *p)
{
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICSW),
		g_pFontMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	return 0;
}

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
	hwndPP1 = m_hWnd;

	//�滻RadminMinfo���IDC_EDIT_README�Ĵ��ڹ��̡�����IDC_EDIT_README�Ĵ����а��س�����ر����Ա��Ի���
	//if (NULL == winproc_prev_My2)
	//	winproc_prev_My2 = (WNDPROC)::SetWindowLong(GetDlgItem(IDC_EDIT_AllSetText), GWL_WNDPROC, (LONG)WinProc_AllSetText2);

	// Init dialog resizing
	DlgResize_Init();

	DoDataExchange();

	////��ȡWin8.x��Win10Ԥ������
	//CString langPath;
	//int nRet;

	//langPath = getCurDir(1);		//g:\MyVC2017\noMeiryoUI235\Debug		//ĩβ��б��
	//langPath = langPath.Left(langPath.ReverseFind(L'\\') + 1);	//g:\MyVC2017\noMeiryoUI235\ //ĩβ��б��
	//langPath = langPath + L"lang\\" + L"English.lng";	//L"G:\\MyVC2017\\noMeiryoUI235\\lang\\English.lng"

	//readResourceFile(langPath);
	//nRet = readFontResource8(langPath);
	//if (!nRet) {
	//	has8Preset = false;
	//}
	//nRet = readFontResource10(langPath);
	//if (!nRet) {
	//	has10Preset = false;
	//}

	//����WinAPI��õ�ǰ����ͼ����
	//ICONMETRICSW im;
	//im.cbSize = sizeof(ICONMETRICSW);	//����ǳ���Ҫ���������溯�����ý�����0����ret=0,˵����������ʧ��
	//int ret = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);
	//m_tagIScur.nHS = im.iHorzSpacing - 32;
	//m_tagIScur.nVS = im.iVertSpacing - 32;

	//������CEdit�ؼ���Ի����Զ����ź�spin�ؼ���CEdit�����Ĵ����������OnInitDialog()�й��磬������ؼ���λ
	////Windows�ڽ��ؼ�CSpinButtonCtrl��WTL��װ��Ϊ��CUpDownCtrl
	////GetWindowRect(&rect);
	//m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
	////Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
	//m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
	//m_spinHS.SetRange(0, 150);
	//m_spinHS.SetPos(m_tagIScur.nHS);
	////MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);

	//m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
	////Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
	//m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
	//m_spinVS.SetRange(0, 150);
	//m_spinVS.SetPos(m_tagIScur.nVS);

	//��ʾWindowsϵͳ�汾��
	CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);


	//------------------------------------------------------
	//���� m_comboPreSet
	//m_comboPreSet.Clear();			//�������CComboBox���ݡ�ֻ��ɾ��CComboBox�ı༭�ؼ��ĵ�ǰѡ������еĻ�����
	m_comboPreSet.ResetContent();	//CComboBox::ResetContent��������������ݡ�
	//ע��������Ͽ����ԣ�TypeΪDrop List��SortΪFalse
	//m_comboPreSet.AddString(L"��ǰ��ʾ����");	//�����������壬����ȥ��
	m_comboPreSet.AddString(L"����ʱ����");		//0
	m_comboPreSet.AddString(L"��һ������");		//1
	m_comboPreSet.AddString(L"Win8.x����");		//2
	m_comboPreSet.AddString(L"Win10����");		//3
	m_comboPreSet.SetCurSel(0);
	//------------------------------------------------------

	//noMeiryoUI235Dlg::OnInitDialog
	//////////////////////////////////////////////////////////////////////////
	FillMemory(&m_metrics, sizeof(NONCLIENTMETRICSW), 0x00);
	FillMemory(&m_metricsAll, sizeof(NONCLIENTMETRICSW), 0x00);
	FillMemory(&m_iconFont, sizeof(LOGFONTW), 0x00);
	FillMemory(&m_iconFontAll, sizeof(LOGFONTW), 0x00);
	m_tagIScur.nHS = m_tagIScur.nVS = -1;

	//���ǽ���ÿ�ֹ������Խ����жϣ�������ÿ�ֹ������Խ��г�ʼ����
	initializeLocale();

	// ��û��ָ��������ini�����ļ�
	if (m_strSettingFile.IsEmpty()) {
		getActualFont();
	}

	//����˵��Ľ�������
	if (!use7Compat) {
		// ��Windows 7�����汾������£��޷����������С�Ĵ���ģʽ��
		CheckMenuItem(GetMenu(), IDM_COMPAT7, MF_BYCOMMAND | MF_UNCHECKED);
		EnableMenuItem(GetMenu(), IDM_COMPAT7, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	CheckMenuItem(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND | MF_CHECKED);

	//��ʾWindowsϵͳ�汾��
	//CString str;
	GetWinOsName(str);
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VERNO), str);

	// ��������������ʾ
	theUpdateDisplay();


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
		//HWND hwndParent = ::GetParent(m_hWnd);
		//if ((m_spinHS.GetPos() == m_tagIScur.nHS) && (m_spinVS.GetPos() == m_tagIScur.nVS))		//1	m_btnLoadDefaultCurrPage
		//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), FALSE); }
		//else
		//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), TRUE); }

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
			|| 2 == len && (sz[0] < 48 || sz[0] > 57 || sz[1] < 48 || sz[1] > 57)
			|| 3 == len && (sz[0] < 48 || sz[0] > 57 || sz[1] < 48 || sz[1] > 57 || sz[2] < 48 || sz[2] > 57)
			|| _wtoi(sz) < 0 || _wtoi(sz) > 150)
		{
			//MessageBoxW(L"������1��150֮�����ֵ");
			//MessageBoxW(g_lang.TranslateString(IDS_REC_UPDATES));
			if(nID == IDC_EDIT_HS)
				::SetWindowTextW(GetDlgItem(nID), (-1 == iHS) ? itos(m_tagIScur.nHS) : itos(iHS));
			else
				::SetWindowTextW(GetDlgItem(nID), (-1 == iVS) ? itos(m_tagIScur.nVS) : itos(iVS));

			//::SetWindowTextW(GetDlgItem(nID), (nID == IDC_EDIT_HS) ? itos(m_tagIScur.nHS) : itos(m_tagIScur.nVS));
			::SendMessageW(GetDlgItem(nID), WM_KEYDOWN, VK_END, 0);
		}
		else		//�������ºϷ���ֵ
		{
			if(nID == IDC_EDIT_HS)
				iHS = _wtoi(sz);
			else
				iVS = _wtoi(sz);
		}

		if (len > 1 && len < 4)
		{
			//ȥ��ǰ��0
			int f = 0;
			while (len > 1 && 48 == sz[0])
			{
				f++;
				for (int i = 0; i < len; i++) { sz[i] = sz[i + 1]; }
				len--;
				sz[len] = 0;
			}
			
			if (f > 0){ ::SetWindowTextW(GetDlgItem(nID), CString(sz));	}	//д�ر༭��
		}
	}

	return 0;
}

//�ɹ������CEdit��ý���ʱȥ��ȫѡ����궨λ�������յ�
//����ı���ý��㡣��Ӧ�ɹ�
//LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
//{
//	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//���������ʽ������ȷ���ã�WTL�в��� -> ����
//	{
//		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// ���ָ�ʽֻ�ܵ���IsWindow()������SetSel()ʧ��
//		//ֻ�е�GetDlgItem(nID)�Ǵ���ʱ�����ܽ����󶨵�edit�����򱨴�
//		CEditImpl edit;			//������������Ӧ�÷���GetFoucs�¼���Ӧ��
//		edit.Attach(GetDlgItem(nID));
//		edit.SetRedraw(FALSE);	// ������˸���Ƚ�ֹ�ػ�
//		edit.SetSel(-1, -1);		// 0 ��ʾ��㣬-1 ��ʾ�յ�
//		edit.SetRedraw(TRUE);	// �����ػ�
//		edit.Invalidate(TRUE);	// �ػ�Edit
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

	PostMessage(WM_SETPAGEFOCUS17, 0, 0L);	//������Ӧ����Ϣ���ܵ���OnSetPageFocus()�������ð�ť

	// Set the Cancel button as the default button
	::PostMessageW(GetParent(), DM_SETDEFID, IDCANCEL, NULL);
	// Let the Cancel button get focus
	//::PostMessageW(GetParent(), WM_SETFOCUS, IDCANCEL, NULL);//��Ч
	//MessageBoxW(L"Test");	//��Ч

	//PostMessageW(WM_SETFOCUS, IDB_SEL_ALLFONT, NULL);//��Ч

	return fRet;
}

//LONG PP3_FontSet::OnSetPageFocus(UINT wParam, LONG lParam)
LRESULT PP1_FontSet::OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//������CEdit�ؼ���Ի����Զ����ź�spin�ؼ���CEdit�����Ĵ������������С�����OnInitDialog()�й��磬������ؼ���λ
	//Windows�ڽ��ؼ�CSpinButtonCtrl��WTL��װ��Ϊ��CUpDownCtrl
	//GetWindowRect(&rect);
	if (!m_spinHS.IsWindow())
	{
		m_spinHS.Attach(GetDlgItem(IDC_SPIN_HS));
		//Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
		m_spinHS.SetBuddy(GetDlgItem(IDC_EDIT_HS));
		m_spinHS.SetRange(0, 150);
		m_spinHS.SetPos(m_tagIScur.nHS);
		//MoveWindow(rect.top, rect.bottom, rect.Width(), rect.Height(), true);
	}
	if (!m_spinVS.IsWindow())
	{
		m_spinVS.Attach(GetDlgItem(IDC_SPIN_VS));
		//Spin�ؼ���Edit�ؼ���Spin�ؼ���ռ��Edit�ؼ��Ŀ�ȣ�������ʱ�༭��̫խ���ɵ���Դ�������е���༭����
		m_spinVS.SetBuddy(GetDlgItem(IDC_EDIT_VS));
		m_spinVS.SetRange(0, 150);
		m_spinVS.SetPos(m_tagIScur.nVS);
	}

	HWND hwndParent = ::GetParent(m_hWnd);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_DEFAULT_ALL), TRUE);	//0 m_btnLoadDefaultAll;		
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_RELOAD_SET_FROM_FILE), TRUE);	//2	m_btnReloadSetFromFile;	
	//if ((m_spinHS.GetPos() == m_tagIScur.nHS) && (m_spinVS.GetPos() == m_tagIScur.nVS))		//1	m_btnLoadDefaultCurrPage
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), FALSE); }
	//else
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_SET_FROM_FILE), TRUE); }

	//if ((-1 == m_iNewHS) || (m_spinHS.GetPos() == m_iNewHS) && (m_spinVS.GetPos() == m_iNewVS))	//2	m_btnLoadDefaultCurrPage
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), FALSE); }
	//else
	//{ ::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_LOAD_NEW_VALUE), TRUE); }

	::EnableWindow(::GetDlgItem(hwndParent, IDB_LOAD_SET), TRUE);
	::EnableWindow(::GetDlgItem(hwndParent, IDB_SAVE_SET), TRUE);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_APPLY), TRUE);		//3	m_btnSaveCurrPage;		
	::EnableWindow(::GetDlgItem(hwndParent, IDOK), TRUE);				//4	m_btnSaveAll;			

	//::SendMessage(GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//����һ��End������ȡ��ѡ��״̬

	//m_editChrome.SetFocus();		//��Ч���÷�ʽCEdit��ý�����������ȷ

	//::PostMessageW(GetParent(), WM_SETFOCUS, IDCANCEL, NULL);//��Ч

	m_editAllFont.SetFocus();		//��Ч

	//PostMessageW(WM_SETFOCUS, IDB_SEL_ALLFONT, NULL);//��Ч
	//SendMessageW(WM_SETFOCUS, IDB_SEL_ALLFONT, NULL);//��Ч

	//CButtonImpl btn;
	//btn.Attach(GetDlgItem(IDB_SEL_ALLFONT));
	//btn.SetFocus();	//��Ч
	//btn.Detach();

	return 0;
}

LRESULT PP1_FontSet::OnCheckAllFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);		//�ؼ�to��Ա������ȱʡΪFALSE-�������ؼ�

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

LRESULT PP1_FontSet::OnSelchangeCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	m_nComboCurSel = m_comboPreSet.GetCurSel();	//��Ͽ�ѡ��
	switch (m_nComboCurSel)
	{
	case 0:		//0 �������ʱ�ľ�������
		mySetFont(m_metrics, m_iconFont, m_tagSetOld);
		theUpdateDisplay();
		break;
	case 1:		//1 ��һ������
		//tagIS.nHS = tagIS.nVS = -1;		//δ�������õı�־
		if (-1 != m_tagSetLast.tagIS.nHS) {
			m_tagSetCur = m_tagSetLast;	//��mySetFont2()�У�����ѵ�ǰ����д��m_tagSetLast�����������ȱ���
			mySetFont(m_metrics, m_iconFont, m_tagSetCur);
			theUpdateDisplay();
		}
		break;
	case 2:		//2 Win8.x����
		mySetFont(m_metrics, m_iconFont, m_tagSetWin8);
		theUpdateDisplay();
		break;
	case 3:		//3 Win10����
		mySetFont(m_metrics, m_iconFont, m_tagSetWin10);
		theUpdateDisplay();
		break;
	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// noMeiryoUI235Dlg.cpp

//�ɹ������CEdit��ý���ʱȥ��ȫѡ����궨λ�������յ�
//����ı���ý��㡣��Ӧ�ɹ�
LRESULT PP1_FontSet::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//�ؼ����ڣ�����m_spinXXX֮ǰ��Ӧ�������ж�m_spinXXX�Ƿ���һ�����ڡ������쳣����
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//���������ʽ������ȷ���ã�WTL�в��� -> ����
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// ���ָ�ʽֻ�ܵ���IsWindow()������SetSel()ʧ��
		//ֻ�е�GetDlgItem(nID)�Ǵ���ʱ�����ܽ����󶨵�edit�����򱨴�
		CEditImpl edit;			//������������Ӧ�÷���GetFoucs�¼���Ӧ��
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// ������˸���Ƚ�ֹ�ػ�
		edit.SetSel(-1, -1);		// 0 ��ʾ��㣬-1 ��ʾ�յ�
								//edit.PostMessage(EM_SETSEL, 0, 0);		//��edit.SetSel(0, 0)����
		edit.SetRedraw(TRUE);	// �����ػ�
		edit.Invalidate(TRUE);	// �ػ�Edit
		edit.Detach();

		//::SendMessageW(GetDlgItem(nID), WM_KEYDOWN, VK_END, 0);		//��궨λ��ĩβ
	}

	return 0;
}

//�޸�ֻ��CEdit����ɫ
LRESULT PP1_FontSet::OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	//if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
	if (IsCtrl((HWND)lParam, L"Edit"))				//�ж��Ƿ���CEdit�ؼ�
	{
		//::SetTextColor(hdc, RGB(255, 0, 0));		//����������ɫΪ��ɫ�����к�ɲ鿴�Ƿ���Ч��
		::SetTextColor(hdc, RGB(0, 0, 0));			//����������ɫ
		::SetBkColor(hdc, RGB(255, 255, 255));		//�������ֱ���ɫ

		::SelectObject(hdc, ::GetStockObject(DC_BRUSH));	//ѡ��NULL_BRUSH�������༭���������л�ɫ����
		SetDCBrushColor(hdc, RGB(255, 255, 255));	//���ñ༭�򱳾�ɫ
													//::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));	//ѡ��NULL_BRUSH�������༭���������л�ɫ����
													//::SelectObject(hdc, ::GetStockObject(WHITE_BRUSH));		//ѡ��WHITE_BRUSH�������༭���������û�л�ɫ����

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

	//CString::Find		��һ���ϴ���ַ����в����ַ������ַ���
	//����ֵ��	���ش�CString����������Ҫ�����ַ������ַ�ƥ��ĵ�һ���ַ��Ĵ��㿪ʼ��������
	//		���û���ҵ����ַ������ַ��򷵻� - 1��
	if (-1 == str.Find(szClassName))
	{
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumeration
}

//������߶Ȼ�ȡ�����С��
int PP1_FontSet::getFontSize(LONG lFontHight)
{
	//��������С��������߶�H�������СS��Ҫ���л��㣬
	//H = -(S * dpiY / 72)
	//S = -(H * 72 / dpiY)
	//MulDiv(a, b, c) ���Ǽ��� a * b / c�������������
	//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

	int dpiY = getDPI();		//��ȡDPI��
	int iSzie = -(lFontHight * 72 / dpiY);
	return iSzie;
}

//�������С��ȡ����߶ȡ�
LONG PP1_FontSet::getFontHight(int lFontSize)
{
	//��������С��������߶�H�������СS��Ҫ���л��㣬
	//H = -(S * dpiY / 72)
	//S = -(H * 72 / dpiY)
	//MulDiv(a, b, c) ���Ǽ��� a * b / c�������������
	//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

	int dpiY = getDPI();		//��ȡDPI��
	int lFontHight = -(lFontSize * dpiY / 72);
	return lFontHight;
}

//��ȡ��ǰ��ʾ����
void PP1_FontSet::getActualFont(void)
{
	// ��ȡͼ�������������Ϣ��
	m_metrics.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICSW),
		&m_metrics,
		0);

	// ��ȡͼ���������Ϣ��
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONTW),
		&m_iconFont,
		0);

	//���浱ǰ����������Ϣ
	SetAllFont(m_metrics, m_iconFont);

	// ��õ�ǰͼ����
	if (!GetIconSpacing(m_tagIScur))
	{	//��ȡ���ɹ���ʹ��Ĭ��ֵ
		m_tagIScur.nHS = 80;
		m_tagIScur.nVS = 48;
	}

	//���浱ǰ��ʾ���ã���Ҫ�������³�Ա
	SaveCurSet(m_tagSetCur);
	m_tagSetOld = m_tagSetCur;
}

// ���˵��������ϢӦ���������������Ϣ��
void PP1_FontSet::SetAllFont(NONCLIENTMETRICSW metrics, LOGFONTW iconFont)
{
	m_metricsAll = metrics;
	m_iconFontAll = iconFont;
	m_metricsAll.lfCaptionFont = m_metricsAll.lfMenuFont;
	m_iconFontAll = m_metricsAll.lfMenuFont;
	m_metricsAll.lfMessageFont = m_metricsAll.lfMenuFont;
	m_metricsAll.lfSmCaptionFont = m_metricsAll.lfMenuFont;
	m_metricsAll.lfStatusFont = m_metricsAll.lfMenuFont;
}

// ����ǰ��ʾ���ñ��浽tagSet��
void PP1_FontSet::SaveCurSet(CPreset& tagSet)
{
	//���浱ǰ��ʾ���ã���Ҫ�������³�Ա
	tagSet.metrics = m_metrics;
	tagSet.iconFont = m_iconFont;
	tagSet.metricsAll = m_metricsAll;
	tagSet.iconFontAll = m_iconFontAll;
	tagSet.tagIS = m_tagIScur;
}

/**
* �t�H���g�\�����X�V����B
*/
void PP1_FontSet::theUpdateDisplay(void)
{
	// �t�H���g���A�|�C���g���\����������쐬����B
	m_strAllFontName = m_metricsAll.lfMenuFont.lfFaceName;
	m_strAllFontName += L"    " + itos(getFontPointInt(&(m_metricsAll.lfMenuFont), m_hWnd)) + L"pt";

	m_strTitleFontName = m_metrics.lfCaptionFont.lfFaceName;
	m_strTitleFontName += L"    " + itos(getFontPointInt(&(m_metrics.lfCaptionFont), m_hWnd)) + L"pt";

	m_strIconFontName = m_iconFont.lfFaceName;
	m_strIconFontName += L"    " + itos(getFontPointInt(&m_iconFont, m_hWnd)) + L"pt";

	// ���j���[�ƑI������
	m_strMenuFontName = m_metrics.lfMenuFont.lfFaceName;
	m_strMenuFontName += L"    " + itos(getFontPointInt(&m_metrics.lfMenuFont, m_hWnd)) + L"pt";

	m_strMessageFontName = m_metrics.lfMessageFont.lfFaceName;
	m_strMessageFontName += L"    " + itos(getFontPointInt(&m_metrics.lfMessageFont, m_hWnd)) + L"pt";

	m_strPaletteFontName = m_metrics.lfSmCaptionFont.lfFaceName;
	m_strPaletteFontName += L"    " + itos(getFontPointInt(&m_metrics.lfSmCaptionFont, m_hWnd)) + L"pt";

	m_strTipFontName = m_metrics.lfStatusFont.lfFaceName;
	m_strTipFontName += L"    " + itos(getFontPointInt(&m_metrics.lfStatusFont, m_hWnd)) + L"pt";

	//����ͼ����
	if (m_spinHS.IsWindow() && m_spinVS.IsWindow())
	{
		m_spinHS.SetPos(m_tagIScur.nHS);
		m_spinVS.SetPos(m_tagIScur.nVS);
	}

	//UpdateData(false);
	DoDataExchange();

	// �I�������t�H���g���e�L�X�g�{�b�N�X�ɐݒ肷��B
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

	if (m_fontMenu != NULL) {
		DeleteObject(m_fontMenu);
	}
	m_fontMenu = createFont(&m_metrics.lfMenuFont);
	m_editMenuFont.SetFont(m_fontMenu);

	if (m_fontMessage != NULL) {
		DeleteObject(m_fontMessage);
	}
	m_fontMessage = createFont(&m_metrics.lfMessageFont);
	m_editMessageFont.SetFont(m_fontMessage);

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
}

int PP1_FontSet::getFontPointInt(LOGFONTW *font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	bool WIN8_SIZE = FALSE;
	if (WIN8_SIZE) {
		// ����Windows 8��ʾ�������
		if ((point > 10) || (point < 8)) {
			return (int)point;
		}
		else {
			// �����Windows 7һ���ߴ�10pt��
			// Windows 7��ǰ���Q 
			if (point - abs((int)point) > 0.49) {
				return (int)point + 1;
			}
			else {
				return (int)point;
			}
		}
	}
	else {
		// Windows 7��ǰ���Q 
		if (point - abs((int)point) > 0.49) {
			return (int)point + 1;
		}
		else {
			return (int)point;
		}
	}
}

/**
*������������������Ӧ�ĵ�����
*��Windows 7 / ͨ�öԻ�����ݣ�
*
* @param		font ������Ϣ
* @param		hWnd Window���
* @return	�����С
*/
double PP1_FontSet::getFontPoint(LOGFONTW *font, HWND hWnd)
{
	//����һ������
	HFONT hFont = CreateFontIndirect(font);

	//���Լ��Ĵ��ھ���������豸������
	//�������塣
	//HDC dc = GetDC(hWnd);
	//SelectFont(dc, hFont);
	CClientDC dc(m_hWnd);
	HFONT prevFont = dc.SelectFont(hFont);

	//���豸�������л�ȡTEXTMETRIC��
	TEXTMETRIC metric;
	GetTextMetrics(dc, &metric);

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);

	//CClientDC��(1)���ͻ����豸�����ģ����ڿͻ�������������ض����ڹ�����
	//	�����ÿ����߷���Ŀ�괰���пͻ������乹�캯���а�����GetDC, ���������а�����ReleaseDC
	//ReleaseDC(hWnd, dc);
	DeleteObject(hFont);

	int height;
	if (font->lfHeight < 0) {
		//���Ϊ������lfHeight�����屾��ĸ߶ȡ�
		height = 0 - font->lfHeight;
	}
	else if (font->lfHeight > 0) {
		//����������������Ѿ������ڲ�ǰ������ȥ�����
		height = font->lfHeight - metric.tmInternalLeading;
	}
	else {
		//���Ϊ0������������������л�ȡĬ�ϴ�С��
		height = metric.tmAscent + metric.tmDescent - metric.tmInternalLeading;
	}

	double point = (double)height * 72 / logPixelY;

	return point;
}

#define APP_FONTSIZE 11		//11ƫ��
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

//�趨 ��ť��Ӧ IDOK
LRESULT PP1_FontSet::OnSet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 

	LOGFONTW iconFont = m_iconFont;
	NONCLIENTMETRICSW metrics = m_metrics;

	DoDataExchange(TRUE);		//�ؼ�to��Ա������ȱʡΪFALSE-�������ؼ�

	if (m_iCheckAllfont)
	{
		theSetFont(&m_metricsAll, &m_iconFontAll);

		memcpy(&m_metrics, &m_metricsAll, sizeof(NONCLIENTMETRICSW));
		memcpy(&m_iconFont, &m_iconFontAll, sizeof(LOGFONTW));

		theUpdateDisplay();
	}
	else if (m_iCheckTitle || m_iCheckIcon || m_iCheckMenu || m_iCheckMessage || m_iCheckPalette || m_iCheckTip)
	{
		if (m_iCheckTitle) metrics.lfCaptionFont = m_metrics.lfCaptionFont;
		if (m_iCheckIcon) iconFont = m_iconFont;
		if (m_iCheckMenu) metrics.lfMenuFont = m_metrics.lfMenuFont;
		if (m_iCheckMessage) metrics.lfMessageFont = m_metrics.lfMessageFont;
		if (m_iCheckPalette) metrics.lfSmCaptionFont = m_metrics.lfSmCaptionFont;
		if (m_iCheckTip) metrics.lfStatusFont = m_metrics.lfStatusFont;

		//Ӧ���������ã�ˢ������
		theSetFont(&metrics, &iconFont);
	}

	//Ӧ��ͼ�������ã�ˢ������
	if (m_iCheckHS || m_iCheckVS)
	{
		if (m_iCheckHS) {
			unsigned nHS = m_spinHS.GetPos();
			if (nHS >= 0 && nHS <= 150) m_tagIScur.nHS = nHS;
		}
		if (m_iCheckVS) {
			unsigned nVS = m_spinVS.GetPos();
			if (nVS >= 0 && nVS <= 150) m_tagIScur.nVS = nVS;
		}
		SetIconSpacing(m_tagIScur.nHS, m_tagIScur.nVS);
	}

	return 0;
}

//�趨���˳� ��ť��Ӧ IDM_SET_EXIT
LRESULT PP1_FontSet::OnSetExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//theSetFont(&m_metrics, &m_iconFont);
	//EndDialog(wID);

	return 0;
}

//�趨ȫ�� ��ť��Ӧ ID_SET_ALL
LRESULT PP1_FontSet::OnSetAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#if 0
	// ����ďc�����p�t�H���g���w�肵�Ȃ��悤�₢���킹���s���B
	if (metricsAll.lfMenuFont.lfFaceName[0] == _T('@')) {
		int answer = MessageBox(hWnd,
			L"�c�����p�t�H���g(���O��@�Ŏn�܂�t�H���g)��\n�w�肳��Ă��܂�����낵���ł����H"),
			L"�m�F"),
			MB_ICONQUESTION | MB_YESNO);
			if (answer != IDYES) {
				return;
			}
	}
#endif

	//// �t�H���g�ύX�����{����B
	//theSetFont(&m_metricsAll, &m_iconFontAll);

	//memcpy(&m_metrics, &m_metricsAll, sizeof(NONCLIENTMETRICSW));
	//memcpy(&m_iconFont, &m_iconFontAll, sizeof(LOGFONTW));

	//// �\�����X�V����B
	//theUpdateDisplay();

	return 0;
}

//LRESULT PP1_FontSet::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	EndDialog(wID);
//	return 0;
//}

LRESULT PP1_FontSet::OnSelFont(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	DoDataExchange(TRUE);	//�ؼ�to��Ա�������Ա�����ָ���ȱʡΪFALSE-�������ؼ���

	LOGFONTW logFont;
	FillMemory(&logFont, sizeof(LOGFONTW), 0x00);

	try {//������ǰ������
		LPLOGFONTW pTargetFont = mapSelFont[wID].second;		//����1��20���е�switch (type)���

		CFontDialog dlg;
		dlg.SetLogFont(pTargetFont);		//������ǰ������
		if (IDOK == dlg.DoModal())
		{
			logFont = *dlg.m_cf.lpLogFont;
		}
	}
	catch (...) {
		::MessageBox(m_hWnd, L"Internal error in font selection dialog.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	//����ѡ����������m_metricsAll��m_iconFontAll��m_metrics��m_iconFont��������ʾ����
	enum fontType type = mapSelFont[wID].first;	//����1��20���е�switch (wID)���
	if (logFont.lfFaceName[0] != _T('\0')) {
		switch (type) {
		case allFont:
			m_metricsAll.lfCaptionFont = logFont;
			m_iconFontAll = logFont;
			//m_metricsAll.lfMenuFont = logFont;
			m_metricsAll.lfMessageFont = logFont;
			m_metricsAll.lfSmCaptionFont = logFont;
			m_metricsAll.lfStatusFont = logFont;

			ChangeFont(m_metricsAll.lfMenuFont, logFont, m_strAllFontName, m_fontAll, m_editAllFont);
			break;
		case titleFont:
			ChangeFont(m_metrics.lfCaptionFont, logFont, m_strTitleFontName, m_fontTitle, m_editTitleFont);
			break;
		case iconFont:
			ChangeFont(m_iconFont, logFont, m_strIconFontName, m_fontIcon, m_editIconFont);
			break;
		case menuFont:
			ChangeFont(m_metrics.lfMenuFont, logFont, m_strMenuFontName, m_fontMenu, m_editMenuFont);
			break;
		case messageFont:
			ChangeFont(m_metrics.lfMessageFont, logFont, m_strMessageFontName, m_fontMessage, m_editMessageFont);
			break;
		case paletteFont:	//lfSmCaptionFont ��ɫ�����Palette Title���塣small caption font
			ChangeFont(m_metrics.lfSmCaptionFont, logFont, m_strPaletteFontName, m_fontPalette, m_editPaletteFont);
			break;
		case tipFont:		//����״̬��tooltips���塣 the font used in status bars and tooltips
			ChangeFont(m_metrics.lfStatusFont, logFont, m_strTipFontName, m_fontTip, m_editTipFont);
			break;
		}
	}

	//����ѡ�����󣬱��浱ǰ��ʾ����
	SaveCurSet(m_tagSetCur);

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

	// ͼ����������
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

	// ����ͼ��������������á����������߳��趨����
	//if (GetMenuState(GetMenu(), IDM_UNIQ_THREAD, MF_BYCOMMAND)) {
	if (1) {
		// UI����һ���߳�SystemParametersInfo(SPI_SETNONCLIENTMETRICS) ������
		g_pFontMetrics = fontMetrics;	//��ʼ�������̵߳�ȫ�ֲ���

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
										// setOnThread ���붨��Ϊȫ�ֺ��������ܶ���Ϊ���Ա���������򱨴����£�
										//(698): error C3867: 'PP1_FontSet::setOnThread': 
										//	non-standard syntax; use '&' to create a pointer to member
		//����ִ���������õ��߳�
		uintptr_t startResult = _beginthreadex(NULL, 0, setOnThread, NULL, 0, NULL);
		if (startResult != 0) {
			// �ɹ������߳�ʱ�ȴ��߳���ֹ��
			HANDLE handle;
			handle = (HANDLE)startResult;

			// �ȴ���Լ5����
			DWORD waitResult = WaitForSingleObject(handle, 5000);
			if (waitResult == WAIT_TIMEOUT) {
				// ����߳�û����ֹ����ʲôҲ�����ˣ��������˳��̡߳�
				TerminateThread(handle, 0);
			}
			CloseHandle(handle);
		}
	}
	else {
		// ����UI��ͬ���߳�������
		SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICSW),
			fontMetrics,
			SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	}

	//�����л���ڹ㲥ϵͳ��λ������Ϣ
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

	//ͨ��������ɫˢ����Ļ��
	//���ǣ���IObit StartMenu 8����ʱ��setSysColors
	//�Ҿ��������ã���Ϊ�ҵ���ʱ��Ӧ�ᶪʧ��
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
	// ���ù�������ע�⣺WTL�зָ��Ϊ\0 ,���� | 
	wchar_t szFilter[] = L"�����ļ� (*.ini)\0*.ini\0�����ļ� (*.*)\0*.*\0\0";
	CString strFilePath;
	rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 0);	//������Ч

																									// ������ļ��Ի���
	CFileDialog fileDlg(TRUE, L"ini", strFilePath, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_READONLY, szFilter, m_hWnd);

	fileDlg.m_ofn.lpstrInitialDir = strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť�����ѡ����ļ�·��  
		strFilePath = fileDlg.m_ofn.lpstrFile;					//��ô򿪵�ȫ·��(���ļ���)
		rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);

		if (FALSE == loadFontInfo(strFilePath)) {
			::MessageBox(m_hWnd, L"�޷�������������", L"����", MB_OK | MB_ICONEXCLAMATION);
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
	LOGFONT iconFont;
	LOGFONT menuFont;
	LOGFONT messageFont;
	LOGFONT smCaptionFont;
	LOGFONT statusFont;

	loadResult = loadFont(filename, L"TitleFont", &captionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"IconFont", &iconFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"MenuFont", &menuFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"MessageFont", &messageFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"PaletteFont", &smCaptionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, L"TipFont", &statusFont);
	if (!loadResult) {
		return FALSE;
	}

	m_metrics.lfCaptionFont = captionFont;
	m_iconFont = iconFont;
	m_metrics.lfMenuFont = menuFont;
	m_metrics.lfMessageFont = messageFont;
	m_metrics.lfSmCaptionFont = smCaptionFont;
	m_metrics.lfStatusFont = statusFont;

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
	// ���ù�������ע�⣺WTL�зָ��Ϊ\0 ,���� | 
	wchar_t szFilter[] = L"�����ļ� (*.ini)\0*.ini\0�����ļ� (*.*)\0*.*\0\0";
	CString strFilePath, strFileName;

	// �Ӹ���ȫ·����ȡ�ļ�����·����
	//iFlag:		0 ���ش���չ�����ļ�����1 ���ز�����չ�����ļ�����
	//			2 ���ز����ļ�����·��(ĩβ��б��)��3 ���ز����ļ�����·��(ĩβ��б��)
	//CString getPathPart(CString strPath, int iFlag)

	rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 0);	//������Ч
	if (strFilePath.IsEmpty())
	{
		strFilePath = getCurDir(1);		//1:	//���ص�ǰĿ¼(ĩβ��б��)

										//��ò����ļ�����·��(ĩβ��б��)�еĲ��ظ�Ψһ�ļ�����(������չ��)
		strFileName = getUniqFileName(strFilePath, L"mySysFontSet");
	}
	else
	{
		strFileName = getPathPart(strFilePath, 1);	//1 ���ز�����չ�����ļ���
		strFilePath = getPathPart(strFilePath, 2);	//2 ���ز����ļ�����·��(ĩβ��б��)

													//��ò����ļ�����·��(ĩβ��б��)�еĲ��ظ�Ψһ�ļ�����(������չ��)
		strFileName = getUniqFileName(strFilePath, strFileName);
	}

	// ���챣���ļ��Ի��� | OFN_NOCHANGEDIR
	CFileDialog fileDlg(FALSE, L"ini", strFileName + L".ini", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, m_hWnd);

	fileDlg.m_ofn.lpstrInitialDir = strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť�����ѡ����ļ�·��  
		strFilePath = fileDlg.m_ofn.lpstrFile;					//��ô򿪵�ȫ·��(���ļ���)
		rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);

		if (FALSE == startSaveFont(strFilePath)) {
			::MessageBox(m_hWnd, L"�޷�������������", L"����", MB_OK | MB_ICONEXCLAMATION);
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
	saveResult = saveFont(filename, L"MenuFont", &m_metrics.lfMenuFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"MessageFont", &m_metrics.lfMessageFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"PaletteFont", &m_metrics.lfSmCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, L"TipFont", &m_metrics.lfStatusFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveIS(filename, L"IconSpacing", &m_tagIScur);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = savePreset(filename, L"Win8xPreset", m_tagSetWin8);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = savePreset(filename, L"Win10Preset", m_tagSetWin10);
	if (!saveResult) {
		return FALSE;
	}

	return TRUE;
}

//Save Windows Preset
BOOL PP1_FontSet::savePreset(CString filename, CString section, CPreset& tagSet)
{
	BOOL bRet;

	// д����������ѭ����ֵ
	CString str;
	for (auto& rcn2 : tagSet.vecRCN2) {
		for (auto& rcn1 : tagSet.vecRCN1) {
			str = rcn1 + L"_" + rcn2 + L"_" + tagSet.strRCN3;
			if (rcn2 == tagSet.vecRCN2[0]) {			// д��������������
				bRet = WritePrivateProfileString(
					section, str, tagSet.mapRCN[rcn1].m0_strFace, filename);
				if (!bRet) { return 0; }
			}
			else if (rcn2 == tagSet.vecRCN2[1]) {	// д�������Сѭ��
				bRet = WritePrivateProfileString(
					section, str,  itos(getFontSize(*tagSet.mapRCN[rcn1].m1_lHeight)), filename);
				if (!bRet) { return 0; }
			}
			else if (rcn2 == tagSet.vecRCN2[2]) {	// д���ַ���ѭ��
				bRet = WritePrivateProfileString(
					section, str, itos(*tagSet.mapRCN[rcn1].m2_bCharset), filename);
				if (!bRet) { return 0; }
			}
		}
	}

	// д��ͼ����
	bRet = WritePrivateProfileString(
		section, tagSet.vecIS[0] + L"_" + tagSet.strRCN3, itos(tagSet.tagIS.nHS), filename);
	if (!bRet) { return FALSE; }

	bRet = WritePrivateProfileString(
		section, tagSet.vecIS[1] + L"_" + tagSet.strRCN3, itos(tagSet.tagIS.nVS), filename);
	if (!bRet) { return FALSE; }

	//// ��ȡ��������ѭ����ֵ
	//CString str;
	//for (auto& rcn2 : tagSet.vecRCN2) {
	//	for (auto& rcn1 : tagSet.vecRCN1) {
	//		str = rcn1 + L"_" + rcn2 + L"_" + tagSet.strRCN3;
	//		if (rcn2 == tagSet.vecRCN2[0]) {			// ��������������ֵ
	//			if (readFontFace(tagSet.mapRCN[rcn1].m0_strFace, file, str) == 0) { return 0; }
	//		}
	//		else if (rcn2 == tagSet.vecRCN2[1]) {	// �����Сѭ����ֵ
	//			if (readFontSize(tagSet.mapRCN[rcn1].m1_lHeight, file, str) == 0) { return 0; }
	//		}
	//		else if (rcn2 == tagSet.vecRCN2[2]) {	// �ַ���ѭ����ֵ
	//			if (readFontCharset(tagSet.mapRCN[rcn1].m2_bCharset, file, str) == 0) { return 0; }
	//		}
	//	}
	//}

	////��������������Ϣ
	//tagSet.SetAllFont();

	//// ��ȡͼ���ࡣ��ȡ���ɹ���ʹ��Ĭ��ֵ
	//if (readIconSpacing(tagSet.tagIS.nHS, file, tagSet.vecIS[0] + L"_" + tagSet.strRCN3) == 0) {
	//	tagSet.tagIS.nHS = 80;
	//}
	//if (readIconSpacing(tagSet.tagIS.nVS, file, tagSet.vecIS[1] + L"_" + tagSet.strRCN3) == 0) {
	//	tagSet.tagIS.nVS = 48;
	//}

	return bRet;
}

//Save Icon Sapcing
BOOL PP1_FontSet::saveIS(CString filename, CString section, TagIS* tagIS)
{
	BOOL bRet;

	bRet = WritePrivateProfileString(section,
		L"IconHorizontalSpacing",
		itos(tagIS->nHS),
		filename);
	if (!bRet) {
		return FALSE;
	}

	bRet = WritePrivateProfileString(section,
		L"IconVerticalSpacing",
		itos(tagIS->nVS),
		filename);
	if (!bRet) {
		return FALSE;
	}

	return bRet;
}

//saveResult = saveFont(filename, L"TitleFont", &m_metrics.lfCaptionFont);
BOOL PP1_FontSet::saveFont(CString filename, CString section, LOGFONT* font)
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

//int PP1_FontSet::mySetFontItem2(LOGFONTW& font, wchar_t const* pFaceName, LONG& lHeight, BYTE& bCharSet)
int PP1_FontSet::mySetFontItem(LOGFONTW& dstFont, LOGFONTW& srcFont)
{
	memset(&dstFont, 0, sizeof(LOGFONTW));
	wcscpy_s(dstFont.lfFaceName, srcFont.lfFaceName);
	dstFont.lfHeight = srcFont.lfHeight;
	dstFont.lfWeight = 400;
	dstFont.lfCharSet = srcFont.lfCharSet;
	dstFont.lfQuality = 5;

	return 0;
}
int PP1_FontSet::mySetFont(NONCLIENTMETRICSW& metrics, LOGFONTW& iconFont, CPreset& tagSet)
{
	//Ӧ��������ǰ�����浱ǰ��ʾ���õ�m_tagSetLast
	SaveCurSet(m_tagSetLast);

	//Ϊ�˱��ֳ�����֮���NONCLIENTMETRICS�ĵ�ǰֵ������NONCLIENTMETRICS�����ݡ�
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);

	mySetFontItem(metrics.lfCaptionFont, tagSet.metrics.lfCaptionFont);
	mySetFontItem(iconFont, tagSet.iconFont);
	mySetFontItem(metrics.lfMenuFont, tagSet.metrics.lfMenuFont);
	mySetFontItem(metrics.lfMessageFont, tagSet.metrics.lfMessageFont);
	mySetFontItem(metrics.lfSmCaptionFont, tagSet.metrics.lfSmCaptionFont);
	mySetFontItem(metrics.lfStatusFont, tagSet.metrics.lfStatusFont);

	m_tagIScur.nHS = tagSet.tagIS.nHS;
	m_tagIScur.nVS = tagSet.tagIS.nVS;

	//Ӧ��������ǰ���������ñ���Ϊ��ǰ��ʾ����
	SaveCurSet(m_tagSetCur);

	return 0;
}

/**
*����Windows 8 / 8.1��Ԥ��ֵ��
*/
LRESULT PP1_FontSet::OnSet8(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	//mySetFont(m_metrics, m_iconFont, tagFontWin8);
	mySetFont(m_metrics, m_iconFont, m_tagSetWin8);

	// ������ʾ��
	theUpdateDisplay();

	return 0;
}

/**
*����Windows 10��Ԥ��ֵ��
*/
LRESULT PP1_FontSet::OnSet10(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	//mySetFont(m_metrics, m_iconFont, tagFontWin10);
	mySetFont(m_metrics, m_iconFont, m_tagSetWin10);

	// ������ʾ��
	theUpdateDisplay();

	return 0;
}

/**
*����Windows Current��Ԥ��ֵ��
*/
LRESULT PP1_FontSet::OnSetCurrent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int PP1_FontSet::SetFont(NONCLIENTMETRICSW& metrics, LOGFONTW iconFont, TagFont& tagFont)
	//mySetFont(m_metrics, m_iconFont, tagFontCur);
	mySetFont(m_metrics, m_iconFont, m_tagSetCur);

	// ������ʾ��
	theUpdateDisplay();

	return 0;
}

LRESULT PP1_FontSet::OnUniqThread(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//UISetCheck(IDM_UNIQ_THREAD, !b);	//��UISetCheck()�Ƚ��鷳
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

//map<unsigned, pair<enum fontType, LPLOGFONTW>> mapSelFont;
//����ѡ��������ʼ����
void PP1_FontSet::initSelFont(void)
{
	mapSelFont[IDB_SEL_ALLFONT] = make_pair(allFont, &m_metricsAll.lfMenuFont);
	mapSelFont[IDB_SEL_TITLE] = make_pair(titleFont, &m_metrics.lfCaptionFont);
	mapSelFont[IDB_SEL_ICON] = make_pair(iconFont, &m_iconFont);
	mapSelFont[IDB_SEL_MENU] = make_pair(menuFont, &m_metrics.lfMenuFont);
	mapSelFont[IDB_SEL_MESSAGE] = make_pair(messageFont, &m_metrics.lfMessageFont);
	mapSelFont[IDB_SEL_PALETTE] = make_pair(paletteFont, &m_metrics.lfSmCaptionFont);
	mapSelFont[IDB_SEL_TIP] = make_pair(tipFont, &m_metrics.lfStatusFont);
}

//���ǽ���ÿ�ֹ������Խ����жϣ�������ÿ�ֹ������Խ��г�ʼ����
void PP1_FontSet::initializeLocale(void)
{
	//����ѡ��������ʼ����tag�ǽṹ��struct��д��ǰ׺
	initSelFont();

	CString langPath;

	//langPath = getCurDir(1);		//g:\MyVC2017\noMeiryoUI235\Debug		//ĩβ��б��
	//langPath = langPath.Left(langPath.ReverseFind(L'\\') + 1);	//g:\MyVC2017\noMeiryoUI235\ //ĩβ��б��
	//langPath = langPath + L"lang\\" + L"English.lng";	//L"G:\\MyVC2017\\noMeiryoUI235\\lang\\English.lng"

	//if (0 == readFontResource(langPath, m_tagSetWin8)) { has8Preset = false; }
	//if (0 == readFontResource(langPath, m_tagSetWin10)) { has10Preset = false; }

	//�ڲ��Զ�Ԥ������
	if (0 == m_tagSetWin8.getPreset(m_tagSetWin8.vecWin8xPreSet)) { has8Preset = false; }
	if (0 == m_tagSetWin10.getPreset(m_tagSetWin10.vecWin10PreSet)) { has10Preset = false; }

	//����Win7���ݷ������˵��Ƿ����
	//warning C4996: 'GetVersion': was declared deprecated
	//DWORD dwVersion = GetVersion();
	//���
	//BOOL VerifyVersionInfoW(
	//	LPOSVERSIONINFOEXW lpVersionInformation,
	//	DWORD              dwTypeMask,
	//	DWORDLONG          dwlConditionMask
	//);

	// ���Windowsϵͳ���ƺͰ汾(֧��Win95��Win10��WinNT4.0��WinSrv2016)
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
*������Դ��Ŀ
*
* @param�ļ���Դ�ļ���
* @param������
* @param��ȡʧ��ʱ���ַ���
*/
void PP1_FontSet::readResourceItem(CString file, CString key, CString fallback)
{
	wchar_t buf[255];
	int len;

	//��ȡINI�ļ��� ������ļ���Unicode�汾��API���Ƿ�Unicode�ģ�������ÿ�����Ե��ַ������ļ���
	len = GetPrivateProfileString(_T("RESOURCE"), key, _T(""), buf, 255, file);
	if (len > 0) {
		langResource.push_back(buf);
	}
	else {
		langResource.push_back(fallback);
	}
}

//��ʼ������Դ��@param�ļ���Դ�ļ���
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
	readResourceItem(file, L"DLG_TIP", L"Tip");
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

//����Ԥ����Դ
int PP1_FontSet::readFontResource(CString file, CPreset& tagSet)
{
	// ��ȡ��������ѭ����ֵ
	CString str;
	int iRet;
	for (auto& rcn2 : tagSet.vecRCN2) {
		for (auto& rcn1 : tagSet.vecRCN1) {
			str = rcn1 + L"_" + rcn2 + L"_" + tagSet.strRCN3;
			if (rcn2 == tagSet.vecRCN2[0]) {			// ��������������ֵ
				iRet = GetPrivateProfileString(L"PRESET", str, L"", tagSet.mapRCN[rcn1].m0_strFace, 255, file);
				if (0 == iRet) { return 0; }
			}
			else if (rcn2 == tagSet.vecRCN2[1]) {	// �����Сѭ����ֵ
				iRet = GetPrivateProfileInt(L"PRESET", str, 0, file);
				if (0 == iRet) { return 0; }
				*tagSet.mapRCN[rcn1].m1_lHeight = getFontHight(iRet);	//����õ�iSizeת��Ϊ����߷���
			}
			else if (rcn2 == tagSet.vecRCN2[2]) {	// �ַ���ѭ����ֵ
				iRet = GetPrivateProfileInt(L"PRESET", str, 0, file);
				if (0 == iRet) { return 0; }
				*tagSet.mapRCN[rcn1].m2_bCharset = iRet;
			}
		}
	}

	//��������������Ϣ
	tagSet.SetAllFont();

	// ��ȡͼ���ࡣ��ȡ���ɹ���ʹ��Ĭ��ֵ
	tagSet.tagIS.nHS = iRet = GetPrivateProfileInt(L"PRESET", tagSet.vecIS[0] + L"_" + tagSet.strRCN3, 0, file);
	if (0 == iRet) { tagSet.tagIS.nHS = 80; }

	tagSet.tagIS.nVS = iRet = GetPrivateProfileInt(L"PRESET", tagSet.vecIS[1] + L"_" + tagSet.strRCN3, 0, file);
	if (0 == iRet) { tagSet.tagIS.nVS = 48; }

	return 0;
}

//Ӧ�����ã�ˢ������
LRESULT PP1_FontSet::SetIconSpacing(unsigned iHS, unsigned iVS, BOOL bRefresh)
{
	int iRet;

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
	iRet = ::SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICSW), &im, 0);

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
	iRet = ::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, iHS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	iRet = ::SystemParametersInfo(SPI_ICONVERTICALSPACING, iVS + 32, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

	//ˢ�����棬������Ч
	if (bRefresh)
	{
		iRet = ::SystemParametersInfo(SPI_SETICONMETRICS, sizeof(ICONMETRICSW), &im, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
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
LRESULT PP1_FontSet::GetIconSpacing(TagIS& tagIS)
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

	tagIS.nHS = im.iHorzSpacing - 32;	//Win7��������λ����Ҫ-32
	tagIS.nVS = im.iVertSpacing - 32;	//Win7��������λ����Ҫ-32

	if (tagIS.nHS >= 0 && tagIS.nHS <= 150 &&	// �������ֶ�λ��0-150֮��
		tagIS.nVS >= 0 && tagIS.nVS <= 150)
	{	//��ȡ��ǰͼ����ɹ�
		nRet = 1;
	}
	else
	{
		nRet = 0;
	}

	return nRet;
}

//��ȡ��ǰͼ����
LRESULT PP1_FontSet::GetIconSpacingOld(vector<unsigned>& vecIS)
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

	vecIS[0] = im.iHorzSpacing - 32;	//Win7��������λ����Ҫ-32
	vecIS[1] = im.iVertSpacing - 32;	//Win7��������λ����Ҫ-32

	if (vecIS[0] >= 0 && vecIS[0] <= 150 &&	// �������ֶ�λ��0-150֮��
		vecIS[1] >= 0 && vecIS[1] <= 150)
	{	//��ȡ��ǰͼ����ɹ�
		nRet = 1;
	}
	else
	{
		nRet = 0;
	}

	return nRet;
}

//���浱ǰ��ʾ���õ��ṹ�����
BOOL PP1_FontSet::SaveCurSetToTag()
{
	return true;
}

//���浱ǰ��ʾ���õ��ļ�
BOOL PP1_FontSet::SaveCurSetToFile()
{
	return true;
}
