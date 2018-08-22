// PP0_PropertySheet.h: interface for the PP0_PropertySheet class.
#pragma once

//#include "PP1_IconSapcing.h"
#include "PP1_FontSet.h"
#include "PP2_Readme.h"
#include "CButtonImpl.h"
#include <vector>
#include <string>
using std::vector; using std::string;

//#include "ChromeFunctions.h"
//�������ChromeFunctions.h������ChromePortable.cpp�İ���ChromeFunctions.h�����ظ��������⡣
//ֻ��ͨ����������Ҫʹ�õĺ�������Ϊ�ⲿ�����ķ�ʽ���
// UNICODE CString�ַ���תstring�ַ�����Quote�����Ч��
extern string& CStringToStringQ(const CString& wstr, string& str);
//CString Trim���������ò����ͷ���ֵ Quote
extern CString& CTrimQ(CString& str);
// ��·������ļ���
extern CString GetFileNameFromPath(const CString& strPath);
//best-CString��split������strSep�����Ƕ���ַ���WIn32Con_UNICDOE_01-CStringSplit���Գɹ�.
extern vector<CString>& CStringSplit(vector<CString>& vecResult, const CString& str, CString strSep);

class PP0_PropertySheet : public CPropertySheetImpl<PP0_PropertySheet>
{
public:
	// Construction
	PP0_PropertySheet(_U_STRINGorID title = (LPCTSTR)NULL,
		UINT uStartPage = 0, HWND hWndParent = NULL);

	// Maps
	BEGIN_MSG_MAP(PP0_PropertySheet)
		//MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		COMMAND_ID_HANDLER(IDOK, OnApplyAll)					// Ӧ�����ã��˳����ý���
		//COMMAND_ID_HANDLER(IDM_ABOUT, OnAppAbout)				// ��Ӧ���ڲ˵���
		//COMMAND_HANDLER(IDOK, BN_CLICKED, OnSaveAllToFile)
		COMMAND_HANDLER(IDC_BTN_APPLY, BN_CLICKED, OnApply)		// Ӧ�����ã����˳����ý���
		COMMAND_HANDLER(IDC_BTN_LOAD_SET_FROM_FILE, BN_CLICKED, OnLoadSetFromFile)
		//COMMAND_HANDLER(IDC_BTN_LOAD_NEW_VALUE, BN_CLICKED, OnLoadDefaultAll)
		//COMMAND_HANDLER(IDC_BTN_LOAD_NEW_VALUE, BN_CLICKED, OnLoadNewValue)// �����µ����ã����˳����ý���
		//COMMAND_HANDLER(IDC_BTN_LOAD_OLD_VALUE, BN_CLICKED, OnLoadOldValue)// �ָ��������ã����˳����ý���
		//COMMAND_ID_HANDLER_EX(IDC_BTN_RELOAD_SET_FROM_FILE, OnReloadSetFromFile)
		CHAIN_MSG_MAP(CPropertySheetImpl<PP0_PropertySheet>)
	END_MSG_MAP()

	//bool OnSaveAllToFile(CString strFileName);
	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//����ȫ���޸ģ����˳����ý���
	LRESULT OnApplyAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	//LRESULT OnLoadNewValue(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//{
	//	// TODO: Add validation code
	//	int nActiveIndex = GetActiveIndex();

	//	//�����µ�ֵ
	//	m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iNewHS);
	//	m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iNewVS);

	//	//���ý���
	//	::SetFocus(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS));	//��Ч���÷�ʽCEdit��ý�����������ȷ
	//	::SendMessage(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//����һ��End������ȡ��ѡ��״̬

	//	return 0;
	//}

	//LRESULT OnLoadOldValue(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//{
	//	int nActiveIndex = GetActiveIndex();

	//	//���û���ǰ����ֵ�����ֵ��ͬ���򱣴�Ϊ��ֵ�������á��µ�ֵ����ť
	//	if ((m_pp1IconSapcing.m_spinHS.GetPos() != m_pp1IconSapcing.m_iOldHS) ||
	//		(m_pp1IconSapcing.m_spinVS.GetPos() != m_pp1IconSapcing.m_iOldVS))
	//	{
	//		//ʹ��CUpDownCtrl�ؼ�����̼�෽��
	//		m_pp1IconSapcing.m_iNewHS = m_pp1IconSapcing.m_spinHS.GetPos();
	//		m_pp1IconSapcing.m_iNewVS = m_pp1IconSapcing.m_spinVS.GetPos();
	//		m_pp1IconSapcing.UpdatelistNewInfo();		//����m_listNewInfo��Ϣ
	//		::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_LOAD_NEW_VALUE), TRUE);	//2	m_btnLoadDefaultCurrPage
	//	}

	//	//���ؾ���ֵ
	//	m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iOldHS);
	//	m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iOldVS);

	//	//���ý���
	//	::SetFocus(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS));	//��Ч���÷�ʽCEdit��ý�����������ȷ
	//	::SendMessage(m_pp1IconSapcing.GetDlgItem(IDC_EDIT_HS), WM_KEYDOWN, VK_END, 0);	//����һ��End������ȡ��ѡ��״̬

	//	//���ü��ؾ���ֵ�������µ�ֵ��־
	//	//m_iFlag = 1;		//���ؾ���ֵ�������µ�ֵ��־��1 (��ʼ)���ؾ���ֵ��0 �����µ�ֵ

	//	//��������˼·�����߼����󡣲�����Ϣ��Ӧ��ֻ������һ����ť����
	//	//�ؼ����ڣ�����m_spinXXX֮ǰ��Ӧ�������ж�m_spinXXX�Ƿ���һ�����ڡ������쳣����
	//	//if (m_pp1IconSapcing.m_spinHS.IsWindow() && m_pp1IconSapcing.m_spinVS.IsWindow())
	//	//{
	//	//	if (0 == m_pp1IconSapcing.m_iFlag)		//0 (��ʼ)���ؾ���ֵ
	//	//	{
	//	//		m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iOldHS);
	//	//		m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iOldVS);
	//	//	}
	//	//	else						//1 �����µ�ֵ
	//	//	{
	//	//		m_pp1IconSapcing.m_spinHS.SetPos(m_pp1IconSapcing.m_iNewHS);
	//	//		m_pp1IconSapcing.m_spinVS.SetPos(m_pp1IconSapcing.m_iNewVS);
	//	//	}
	//	//	m_pp1IconSapcing.OnEnChangeEdit(0, 0, CWindow());
	//	//}

	//	return 0;
	//}

	LRESULT OnLoadDefaultAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		// ��������ҳ���¼����Ѹ��µ�����
		m_pp1FontSet.m_bReInit = TRUE;
		m_pp2Readme.m_bReInit = TRUE;		//�ѽ���������¼��ر������⡣����ֻ�����ɲ����¼��ء�

		if (m_pp1FontSet.m_hWnd)			//�������ҳ�Ƿ����
		{
			m_pp1FontSet.OnInitDialog(NULL, NULL);
		}

		SetActivePage(GetActiveIndex());		//ˢ�µ�ǰ����ҳ

		return 0;
	}

	//void OnReloadSetFromFile(UINT uCode, int nID, HWND hwncCtrl)
	LRESULT OnLoadSetFromFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		m_pp2Readme.m_bReInit = TRUE;		//�ѽ���������¼��ر������⡣����ֻ�����ɲ����¼��ء�
		m_pp1FontSet.m_bReInit = TRUE;

		SetActivePage(GetActiveIndex());		//ˢ�µ�ǰ����ҳ
		return 0;
	}

	// Message handlers
	void OnShowWindow(BOOL bShowing, int nReason);
	//BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	BOOL OnSheetInitialized();

	// Property pages
	//PP1_IconSapcing m_pp1IconSapcing;
	PP2_Readme m_pp2Readme;
	PP1_FontSet m_pp1FontSet;
	CPropertyPage<IDD_ABOUTBOX> m_ppAbout;
	CButtonImpl m_ButtonCopy;

	CButtonImpl m_btnLoadOldValue;
	CButtonImpl m_btnLoadNewValue;
	CButtonImpl m_btnApply;
	CButtonImpl m_btnApplyQuit;
	CButtonImpl m_btnCancle;

	// Implementation
protected:
	bool m_bCentered;
};
