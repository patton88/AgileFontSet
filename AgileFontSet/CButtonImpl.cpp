// CButtonImpl.cpp
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"
#include "CButtonImpl.h"

CButtonImpl::CButtonImpl()
{
    m_bHovering = FALSE;
}

CButtonImpl::~CButtonImpl()
{
}

void CButtonImpl::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
    if(NULL == lpszText)
        return;
	
    InitToolTip();
    if (m_toolTip.GetToolCount() == 0)
	{
        CRect rectBtn; 
        GetClientRect(rectBtn);
        m_toolTip.AddTool(*this, lpszText, rectBtn, 1);
    }
    m_toolTip.UpdateTipText(lpszText, *this, 1);
	m_toolTip.SetDelayTime(TTDT_AUTOPOP, 30000);//���30�롣������ʾǰ���ӳ�ʱ�䣬�Ǳ��衣ȱʡ��500ms
    m_toolTip.Activate(bActivate);
}

void CButtonImpl::InitToolTip()
{
    if(NULL == m_toolTip.m_hWnd)
	{
        m_toolTip.Create(*this);
        m_toolTip.Activate(FALSE);
        m_toolTip.SetMaxTipWidth(400);
    }
}

LRESULT CButtonImpl::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // track hover msg and leave msg
    if(WM_MOUSEMOVE == uMsg)
	{
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = 20;
        tme.hwndTrack = m_hWnd;
        _TrackMouseEvent(&tme);
    }
	
    if(m_toolTip.IsWindow())
        m_toolTip.RelayEvent((LPMSG)m_pCurrentMsg);
	
    bHandled = FALSE;
    return FALSE;
}

void CButtonImpl::OnMouseHover(WPARAM wParam, CPoint point)
{
    if(!m_bHovering)
	{
        m_bHovering = TRUE;
        //Invalidate();
    }
	SetMsgHandled(FALSE);
}

void CButtonImpl::OnMouseLeave()
{
	// ���������䣺Ҫ���ж����Ƿ����m_toolTip���ڣ�����δ���toolTip��button���������
	if(m_toolTip.IsWindow())
	{
		m_toolTip.Activate(FALSE);
		m_toolTip.Activate(TRUE);
	}
	
	m_bHovering = FALSE;
	SetMsgHandled(FALSE);
	//Invalidate();
}

CButtonImpl& CButtonImpl::operator=( HWND hWnd )
{
	//m_hWnd = hWnd;
	CWindowImpl<CButtonImpl, CButton>::SubclassWindow(hWnd);
	return *this;
}