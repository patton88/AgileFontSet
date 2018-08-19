// PP2_Readme dialog

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "CEditImpl.h"

class PP2_Readme :
	public CPropertyPageImpl<PP2_Readme>,
	public CWinDataExchange<PP2_Readme>,
	public CDialogResize<PP2_Readme>
{
public:
	enum { IDD = IDD_PP2_README };

	// Construction
	PP2_Readme();

	// this is the user-defined message
#define WM_SETPAGEFOCUS11 WM_APP + 12

	// Maps
	BEGIN_MSG_MAP(PP2_Readme)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_SETPAGEFOCUS11, OnSetPageFocus)
		COMMAND_HANDLER_EX(IDC_EDIT_README, EN_SETFOCUS, OnEnSetfoucsEdit)	//����ı���ý���

		//����Ϣ��Ӧֻ���޸�CEdit����ɫ���򵥽��������һ�����õġ������ݡ���Tabstop�������ӵľ�̬�ı���
		//����ԭ���ǣ���PP0_PropertySheet����PP2_Readme����ҳ�������ÿؼ�ʱ���Զ���������ҳ��Ϊ��ҽ���״̬
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorDlg)

		//�����δ����Theme�Ļ�����ֻ��CEdit�ؼ��ı���Ϊ��ɫ�����⣬������Ч
		//MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)			//��ͨ��CEdit
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEditOnly)		//ֻ��״̬��CEdit

		CHAIN_MSG_MAP(CPropertyPageImpl<PP2_Readme>)
		CHAIN_MSG_MAP(CDialogResize<PP2_Readme>)
	END_MSG_MAP()

	/*���뱨��
	1>d:\program files (x86)\microsoft visual studio 12.0\vc\wtl\include\atlframe.h(3355): error C2039: 'GetDlgResizeMap' : is not a member of 'RadminMinfo'
	1>          g:\myvccode\chromeportable\chromeportable\radminminfo.h(12) : see declaration of 'RadminMinfo'
	1>          d:\program files (x86)\microsoft visual studio 12.0\vc\wtl\include\atlframe.h(3260) : while compiling class template member function 'void WTL::CDialogResize<RadminMinfo>::DlgResize_Init(bool,bool,DWORD)'
	1>          g:\myvccode\chromeportable\chromeportable\radminminfo.cpp(32) : see reference to function template instantiation 'void WTL::CDialogResize<RadminMinfo>::DlgResize_Init(bool,bool,DWORD)' being compiled
	1>          g:\myvccode\chromeportable\chromeportable\radminminfo.h(15) : see reference to class template instantiation 'WTL::CDialogResize<RadminMinfo>' being compiled

	�����������ּ��������ӳ���
	BEGIN_DLGRESIZE_MAP(CUrlDlg)

	END_DLGRESIZE_MAP()
	��������ͽ����, ����ɹ��ˡ�
	*/

	//ע�⣺Ҫʵ�ֱ�ǩ�ؼ��Զ����ţ�����Ϊ��ָ������ID��
	BEGIN_DLGRESIZE_MAP(PP2_Readme)
		//DLGRESIZE_CONTROL(IDC_TEXT1, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_README, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		//DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
		//DLGRESIZE_CONTROL(IDC_TREE, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_RELATIONSHIP_TREE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(PP2_Readme)
		DDX_TEXT(IDC_EDIT_README, m_strReadme)
		DDX_CONTROL(IDC_EDIT_README, m_editReadme)
	END_DDX_MAP()

	// Message handlers
	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	BOOL OnSetActive();
	//LONG OnSetPageFocus(UINT wParam, LONG lParam);
	LRESULT OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//����ı���ý��㡣��Ӧ�ɹ�
	LRESULT OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl);
	//�޸�ֻ��CEdit����ɫ
	LRESULT OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�޸���ͨCEdit����ɫ
	LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void GetEnglishHelp();
	void GetChinsesHelp();

	// DDX variables
	CString m_strReadme;
	CEditImpl m_editReadme;
	BOOL m_bReInit;
};
