// CComboImpl.h
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CComboImpl_H__C01_C67_o24_m75_b32_o47_00000463__INCLUDED_)
#define AFX_CComboImpl_H__C01_C67_o24_m75_b32_o47_00000463__INCLUDED_

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// CComboImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CComboImpl : public CWindowImpl<CComboImpl, CComboBox>
{
public:
    CComboImpl();
    virtual ~CComboImpl();
    
    void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	CComboImpl& operator=( HWND hWnd );
	
protected:
    BEGIN_MSG_MAP(CComboImpl)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MSG_WM_MOUSEHOVER(OnMouseHover)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
		
	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnMouseHover(WPARAM wParam, CPoint point);
    void OnMouseLeave();
	
	BOOL SubclassWindow(HWND hWnd);

private:
    CToolTipCtrl m_toolTip;
    BOOL m_bHovering;
    void InitToolTip();
};

#endif // !defined(AFX_CComboImpl_H__C01_C67_o24_m75_b32_o47_00000463__INCLUDED_)

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
