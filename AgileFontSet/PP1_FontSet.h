// PP3_FontSet dialog

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "CEditImpl.h"

class PP1_FontSet :
	public CPropertyPageImpl<PP1_FontSet>,
	public CWinDataExchange<PP1_FontSet>,
	public CDialogResize<PP1_FontSet>
{
public:
	enum { IDD = IDD_PP1_FontSet };

	// Construction
	PP1_FontSet();

// this is the user-defined message
#define WM_SETPAGEFOCUS17 WM_APP + 11//������Ӧ����Ϣ���ܵ���OnSetPageFocus()�������ð�ť

	//����ID��IDC_MONDAY��IDC_FRIDAY֮��Ŀؼ����͵�BN_CLICKED֪ͨ��Ϣ��
	//COMMAND_RANGE_CODE_HANDLER_EX(IDC_MONDAY, IDC_FRIDAY, BN_CLICKED, OnDayClicked)

	// Maps
	BEGIN_MSG_MAP(PP1_FontSet)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_SETPAGEFOCUS17, OnSetPageFocus)//������Ӧ����Ϣ���ܵ���OnSetPageFocus()�������ð�ť
		CHAIN_MSG_MAP(CPropertyPageImpl<PP1_FontSet>)
		CHAIN_MSG_MAP(CDialogResize<PP1_FontSet>)
	END_MSG_MAP()

	//ע�⣺Ҫʵ�ֱ�ǩ�ؼ��Զ����ţ����벢Ϊ��ָ������ID��
	BEGIN_DLGRESIZE_MAP(PP1_FontSet)
		//DLGRESIZE_CONTROL(IDC_TEXT1, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_EDIT_AllSetText, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		//DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
		//DLGRESIZE_CONTROL(IDC_TREE, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_RELATIONSHIP_TREE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(PP1_FontSet)
		//DDX_CHECK(IDC_CHECK_REFRESH, m_iRefresh)		// DDX_CHECKֻ����int���ͱ�����������

		//DDX_TEXT(IDC_EDIT_AllSetText, m_strChrome)
		//DDX_CONTROL(IDC_EDIT_AllSetText, m_editChrome)
		//DDX_INT(IDC_EDIT_HS, m_iHS)
		//DDX_INT(IDC_EDIT_VS, m_iVS)
		//DDX_CONTROL(IDC_SPIN_HS, m_spinHS)
		//DDX_CONTROL(IDC_SPIN_VS, m_spinVS)
	END_DDX_MAP()

	// Message handlers
	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	BOOL OnSetActive();

	//����ı��ı�
	LRESULT OnEnChangeEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//����ı���ý���
	LRESULT OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//LONG OnSetPageFocus(UINT wParam, LONG lParam);
	LRESULT OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	// DDX variables
	CString m_strChrome;
	CEditImpl m_editChrome;
	BOOL m_bReInit;

	//Windows�ڽ��ؼ�CSpinButtonCtrl��WTL��װ��Ϊ��CUpDownCtrl
	CUpDownCtrl m_spinHS;
	CUpDownCtrl m_spinVS;
	CEditImpl m_editHS;
	CEditImpl m_editVS;

	//����EN_CHANGE��Ϣ��Ӧ
	CUpDownCtrl m_spin1;
	CEditImpl m_edit1;

	int m_iOldHS;
	int m_iOldVS;
};
