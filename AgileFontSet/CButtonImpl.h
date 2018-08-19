// CButtonImpl.h
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CButtonImpl_H__C72_B65_u46_t27_t93_o79_00014831__INCLUDED_)
#define AFX_CButtonImpl_H__C72_B65_u46_t27_t93_o79_00014831__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CButtonImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CButtonImpl : public CWindowImpl<CButtonImpl, CButton>
{
public:
    CButtonImpl();
    virtual ~CButtonImpl();
    
    void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	CButtonImpl& operator=( HWND hWnd );

protected:
    BEGIN_MSG_MAP(CButtonImpl)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MSG_WM_MOUSEHOVER(OnMouseHover)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
	END_MSG_MAP()

	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnMouseHover(WPARAM wParam, CPoint point);
    void OnMouseLeave();

private:
    CToolTipCtrl m_toolTip;
    BOOL m_bHovering;
    void InitToolTip();
};

#endif // !defined(AFX_CButtonImpl_H__C72_B65_u46_t27_t93_o79_00014831__INCLUDED_)

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
