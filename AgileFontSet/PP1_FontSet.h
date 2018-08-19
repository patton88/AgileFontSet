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
#define WM_SETPAGEFOCUS17 WM_APP + 11//必须响应此消息才能调用OnSetPageFocus()禁用启用按钮

	//处理ID在IDC_MONDAY和IDC_FRIDAY之间的控件发送的BN_CLICKED通知消息。
	//COMMAND_RANGE_CODE_HANDLER_EX(IDC_MONDAY, IDC_FRIDAY, BN_CLICKED, OnDayClicked)

	// Maps
	BEGIN_MSG_MAP(PP1_FontSet)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_SETPAGEFOCUS17, OnSetPageFocus)//必须响应此消息才能调用OnSetPageFocus()禁用启用按钮
		CHAIN_MSG_MAP(CPropertyPageImpl<PP1_FontSet>)
		CHAIN_MSG_MAP(CDialogResize<PP1_FontSet>)
	END_MSG_MAP()

	//注意：要实现标签控件自动缩放，必须并为其指定独立ID号
	BEGIN_DLGRESIZE_MAP(PP1_FontSet)
		//DLGRESIZE_CONTROL(IDC_TEXT1, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_EDIT_AllSetText, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		//DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
		//DLGRESIZE_CONTROL(IDC_TREE, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_RELATIONSHIP_TREE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(PP1_FontSet)
		//DDX_CHECK(IDC_CHECK_REFRESH, m_iRefresh)		// DDX_CHECK只能与int类型变量交换数据

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

	//监测文本改变
	LRESULT OnEnChangeEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//监测文本获得焦点
	LRESULT OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//LONG OnSetPageFocus(UINT wParam, LONG lParam);
	LRESULT OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	// DDX variables
	CString m_strChrome;
	CEditImpl m_editChrome;
	BOOL m_bReInit;

	//Windows内建控件CSpinButtonCtrl的WTL封装类为：CUpDownCtrl
	CUpDownCtrl m_spinHS;
	CUpDownCtrl m_spinVS;
	CEditImpl m_editHS;
	CEditImpl m_editVS;

	//测试EN_CHANGE消息响应
	CUpDownCtrl m_spin1;
	CEditImpl m_edit1;

	int m_iOldHS;
	int m_iOldVS;
};
