// PP2_Readme.cpp: implementation of the PP2_Readme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PP2_Readme.h"

//�滻RadminMinfo���IDC_EDIT_README�Ĵ��ڹ��̡�����IDC_EDIT_README�Ĵ����а��س�����ر����Ա��Ի���
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

	//������뱨��error C2065: 'GWL_WNDPROC': undeclared identifier������ΪVS2017Ĭ����x64���룬��Ϊx86����㲻�ᱨ��
	//�滻RadminMinfo���IDC_EDIT_README�Ĵ��ڹ��̡�����IDC_EDIT_README�Ĵ����а��س�����ر����Ա��Ի���
	if (NULL == winproc_prev_Readme)
		winproc_prev_Readme = (WNDPROC)::SetWindowLong(GetDlgItem(IDC_EDIT_README), GWL_WNDPROC, (LONG)WinProc_Readme);

	// Init dialog resizing
	DlgResize_Init();

	//m_str = L"test";
	GetChinsesHelp();
	//GetEnglishHelp();

	DoDataExchange();
	//::SetFocus(GetDlgItem(IDC_EDIT5));	//��Ч

	//�Ի����и��ؼ�������Static Text�ı���ʾ���������������ͣ��ʾ
	//m_editReadme.SetTooltipText(L"ChromePortable.exe�������˵��");

	////Ϊ����ҳ��Ӳ˵��ɹ�����Ч
	//CMenu menu;
	//menu.LoadMenuW(IDR_MENU1);
	//SetMenu(menu);
	//menu.Detach();		//�þֲ���������Detach()
	//Invalidate();		//ˢ����ʾ

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
	//::PostMessage(GetDlgItem(IDC_EDIT_README), WM_SETFOCUS, 0, 0);		//��Ч�����÷�ʽ��CEdit��ý���������������
	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_KEYDOWN, VK_HOME, 0);	//��Ч

	//----------------------------------------
	// Set the Cancel button as the default button
	//SendMessage(DM_SETDEFID, IDCANCEL);	// MFC ����SendMessage��Ч��WTL ����Ч
	//PostMessage(DM_SETDEFID, IDCANCEL);		// WTL �б�����PostMessage����Ч
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
	WM_SETFOCUS��SetFocus()������

	SetFocus()�����3���£�
	(1). ��ʧȥ���̽���Ĵ��ڷ���WM_KILLFOCUS��Ϣ��
	(2). ����ռ��̽���Ĵ��ڷ���WM_SETFOCUS��Ϣ��
	(3). ͬʱ������ս���Ĵ��ڻ��丸���ڡ�
	���Խ�����WM_SETFOCUS��Ϣ��ϣ�����ý���Ĵ��ڻ����У���������ԭ���н���Ĵ��ڷ���WM_KILLFOCUS��Ϣ��
	��Ҫ����ս���Ĵ��ڻ��丸���ڷ���WM_ACTIVATE��Ϣ�����

	���ý���SetfFocus()����ִ�лᴥ��WM_SETFOCuS��Ϣ
	The SetFocus function sends a WM_KILLFOCUS message to the window that loses the keyboard focus and a 
	WM_SETFOCUS message to the window that receives the keyboard focus. It also activates either the window 
	that receives the focus or the parent of the window that receives the focus.

	��ȡ����GetFocus()

	WM_SETFOCUS��WM_ACTIVE������ʲô��
	����ͻ�ý��㻹��������ģ��������һ�����ڣ�����Ǽ����ˣ��������Ӵ��ڣ���ť��ʲô��Ҳ��Ҫ��ý���ģ���ô���ʱ������������Ǽ���ģ��ؼ��ǻ�ý���ġ����ܷ���˵��

	*/

	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_SETFOCUS, 0, 0);		//��Ч�����÷�ʽ��CEdit��ý���������������
	//::PostMessage(GetDlgItem(IDC_EDIT5), WM_KEYDOWN, VK_HOME, 0);	//��Ч

	/*
	Ĭ������£����Ա�������ҳÿ����ʾʱ���������ݵ�CEdit�ؼ��ǵ�һTabstop��
	��CEdit�����ݽ���ѡ��״̬��

	Ҫ��������ҳ��ʾʱ�������ݵ�CEdit�ؼ���ý��㣬�����ݲ���ѡ�У�������һ����ʾʱ�Ĺ��λ��
	�������������

	1��������ҳ�����õ�һTabStopΪ�����ؼ�������CEdit�ؼ���
		��Ҫ����Label�ؼ���Tab������������TabstopΪTrue

	2�������Զ�����Ϣ��BOOL PP2_Readme::OnSetActive()�з���

	3�����Զ�����Ϣ�н�����ҳ�Ľ������õ�CEdit�ؼ���
	
	4����������ҳ���ֺ��ֹ���Tab���������л���CEdit�ؼ���CEdit�ؼ������ݻᱻѡ��

	*/
	
	m_editReadme.SetFocus();	//��Ч���÷�ʽCEdit��ý�����������ȷ
	//GetDlgItem(IDC_EDIT_README).SetFocus();	//��Ч���÷�ʽCEdit��ý�����������ȷ
	//GetDlgItem(IDC_EDIT_README).PostMessage(EM_SETSEL, 0, 0);	//�����Ὣ��궨λ����ǰ�棬�޷�������һ��λ��

	HWND hwndParent = ::GetParent(m_hWnd);
	::EnableWindow(::GetDlgItem(hwndParent, IDB_LOAD_SET), FALSE);
	::EnableWindow(::GetDlgItem(hwndParent, IDB_SAVE_SET), FALSE);
	//::EnableWindow(::GetDlgItem(hwndParent, IDC_BTN_APPLY), FALSE);
	::EnableWindow(::GetDlgItem(hwndParent, IDOK), FALSE);

	return 0;
}

//�ɹ������CEdit��ý���ʱȥ��ȫѡ����궨λ�������յ�
//����ı���ý��㡣��Ӧ�ɹ�
LRESULT PP2_Readme::OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//�ؼ����ڣ�����m_spinXXX֮ǰ��Ӧ�������ж�m_spinXXX�Ƿ���һ�����ڡ������쳣����
	if ((CEditImpl*)GetDlgItem(nID).IsWindow())	//���������ʽ������ȷ���ã�WTL�в��� -> ����
	{
		//(CEditImpl*)GetDlgItem(nID).SetSel(0, 0);	// ���ָ�ʽֻ�ܵ���IsWindow()������SetSel()ʧ��
		//ֻ�е�GetDlgItem(nID)�Ǵ���ʱ�����ܽ����󶨵�edit�����򱨴�
		CEditImpl edit;			//������������Ӧ�÷���GetFoucs�¼���Ӧ��
		edit.Attach(GetDlgItem(nID));
		edit.SetRedraw(FALSE);	// ������˸���Ƚ�ֹ�ػ�
		edit.SetSel(0, 0);		// 0 ��ʾ��㣬-1 ��ʾ�յ�
		//edit.PostMessage(EM_SETSEL, 0, 0);		//��edit.SetSel(0, 0)����
		edit.SetRedraw(TRUE);	// �����ػ�
		edit.Invalidate(TRUE);	// �ػ�Edit
		edit.Detach();
	}

	return 0;
}

//�޸�ֻ��CEdit����ɫ
LRESULT PP2_Readme::OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
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

//�޸���ͨCEdit����ɫ
LRESULT PP2_Readme::OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)lParam == GetDlgItem(IDC_EDIT_README))
	{
		HDC hdc = (HDC)wParam;
		::SetTextColor(hdc, RGB(0, 0, 0));		//����������ɫ
		::SetBkColor(hdc, RGB(255, 255, 255));	//���ñ���ɫ
												//::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));	//ѡ��NULL_BRUSH�������༭���������л�ɫ����
		::SelectObject(hdc, ::GetStockObject(WHITE_BRUSH));		//ѡ��WHITE_BRUSH�������༭���������û�л�ɫ����
		bHandled = TRUE;
		return (HRESULT)::GetCurrentObject(hdc, OBJ_BRUSH);
	}

	return 0;
}

void PP2_Readme::GetChinsesHelp()
{
	m_strReadme =
		L"  IconSpacing.exe v1.0 ����˵��\r\n\
  Ѹ��ͼ�������ó���v1.0 - ybmj@vip.163.com 20180812\r\n\
\r\n\
  (1)��������;���������Windows����ͼ����\r\n\
  WinXP��Win7�û�����ϵͳ�ṩ�ĸ߼�����������޸�����ͼ���࣬���ǣ�Win8��Win10ϵͳȴû���ṩ��һ���ܡ���Ϥϵͳ���û���Ȼ����ͨ���޸�ע���bat��vbs�ȷ�ʽ��������ͼ���࣬����Щ��������Ҫע�������µ�¼�������������������Ч��\r\n\
  �ó��� IconSpacing.exe ���ǿ������Windows����ͼ�����С����������Ϊ��Ѹ��ͼ�������ó��򡱡��ó����ṩ���ڽ��桢����������ʹ�÷�ʽ�����ڽ������ݡ�һĿ��Ȼ����ͨ�û�Ҳ������ʹ�ã����ң����ú�����ע�����������������Ч�������з�ʽ������WinPE����Ҫ�Զ���������ͼ����������\r\n\
\r\n\
  (2)��ʹ�û���\r\n\
  �ó������VS2017 WTL10 x86����(Windows Template Library��http://wtl.sourceforge.net/ )�������С�������ɶ������У�֧��32λ��64λ��WinXp��Vista��Win7��Win8.x��Win10����ӦWindows Server��Ȳ���ϵͳ��\r\n\
\r\n\
  (3)�����ڽ�������������ͼ����\r\n\
  ��Windows��ֱ������ IconSpacing.exe ����������ڽ��棬���������ʾ����ͼ����ĵ�ǰֵ�����öԻ������ݡ�һĿ��Ȼ����ͨ�û�Ҳ�ɿ��ٷ������������ͼ���࣬���ҿ���������Ч��\r\n\
\r\n\
  (4)�������з�ʽ��������ͼ����\r\n\
  ��WinPE����Ҫ�Զ���������ͼ�����ʱ�򣬿���ʹ�������з�ʽ���û��ɰ����¸�ʽ�Ĳ��������������в��������ִ�Сд����������ͼ������������ˢ�����棬��������� -N ������[]�ӷ����ŵ�Ϊ��ѡ������()��Բ���ŵ�Ϊ��ѡ��������(ֻ�ܵ���ʹ��)��(�޲�)�ǲ����κβ�����\r\n\
  [-Hxxx] ��������ͼ��ˮƽ�������Ϊxxx��xxx��0��150֮���3λ������(Win7��������λ)��ֻ���ò���������[-Vxxx]����ʱ����ֻ�޸�����ͼ���ˮƽ��ࣻ\r\n\
  [-Vxxx] ��������ͼ�괹ֱ�������Ϊxxx��xxx��0��150֮���3λ������(Win7��������λ)��ֻ���ò���������[-Hxxx]����ʱ����ֻ�޸�����ͼ��Ĵ�ֱ��ࣻ\r\n\
  [-N]    ����������ͼ����󣬲�����ˢ�����档��Ҫ���µ�¼�������������������Ч��\r\n\
  (-R)    �����ı�����ͼ���ֻ࣬ˢ�����棻\r\n\
  (-?)    ����ʾ���öԻ���ʹ˰�����Ϣ(��ǰѡ�)������ʾ����ͼ����ĵ�ǰֵ��\r\n\
  (�޲�)  ����ʾ���öԻ���(��ǰѡ�)�ʹ˰�����Ϣ����ʾ����ͼ����ĵ�ǰֵ��\r\n\
\r\n\
  (5)������ʹ��ʵ��\r\n\
  �� ������ͼ��ˮƽ�������Ϊ76����ֱ�������Ϊ48���������ú�����ˢ�����档\r\n\
  IconSpacing.exe -H76 -V48\r\n\
\r\n\
  �� ������ͼ��ˮƽ�������Ϊ80����ֱ�������Ϊ50�����ú�����ˢ�����档\r\n\
  IconSpacing.exe -V50 -H80 -N\r\n\
\r\n\
  �� ����ͼ���ˮƽ��ౣ�ֲ��䡢��ֱ�������Ϊ56���������ú�����ˢ�����档\r\n\
  IconSpacing.exe -V56\r\n\
\r\n\
  �� ���ı�����ͼ���ֻ࣬������ˢ�����档\r\n\
  IconSpacing.exe -R\r\n\
\r\n\
  �� ��ʾ���öԻ���ʹ˰�����Ϣ(��ǰѡ�)����ȡ����ͼ����ĵ�ǰֵ��\r\n\
  IconSpacing.exe -?\r\n\
\r\n\
  �� ��ʾ���öԻ���(��ǰѡ�)�ʹ˰�����Ϣ����ȡ����ͼ����ĵ�ǰֵ��\r\n\
  IconSpacing.exe\r\n\
\r\n\
  (6)����������\r\n\
  �û�����������ѡ�øó�����ת����ע����������һ�к�������߲��е��κ����Σ�\r\n\
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
