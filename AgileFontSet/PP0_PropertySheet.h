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
		COMMAND_ID_HANDLER(IDOK, OnApply)						// Ӧ�����ã����˳����ý���
		//COMMAND_HANDLER(IDB_LOAD_SET, BN_CLICKED, OnLoadSetFromFile)
		COMMAND_ID_HANDLER(IDB_SAVE_SET, OnSave)
		COMMAND_ID_HANDLER(IDB_LOAD_SET, OnLoad)
		CHAIN_MSG_MAP(CPropertySheetImpl<PP0_PropertySheet>)
	END_MSG_MAP()

	//bool OnSaveAllToFile(CString strFileName);
	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	//����ȫ���޸ģ����˳����ý���
	//LRESULT OnApplyAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLoad(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
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
