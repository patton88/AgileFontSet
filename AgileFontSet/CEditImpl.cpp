// CEditImpl.cpp
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"
#include "CEditImpl.h"

CEditImpl::CEditImpl()
{
    m_bHovering = FALSE;
}

CEditImpl::~CEditImpl()
{
}

void CEditImpl::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
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
	m_toolTip.SetDelayTime(TTDT_RESHOW, 1000);	//���ͣ����1�뵯����ʾ�������ʾ��������ʧ���ٵ���������
							//MSG_WM_SETCURSOR��Ϣ��ӦҲ���ܻᵼ����ʾ��Ϣ��˸��ʧ������ȥ��
    m_toolTip.Activate(bActivate);
}

void CEditImpl::InitToolTip()
{
    if(NULL == m_toolTip.m_hWnd)
	{
        m_toolTip.Create(*this);
        m_toolTip.Activate(FALSE);
        m_toolTip.SetMaxTipWidth(400);
    }
}

LRESULT CEditImpl::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CEditImpl::OnMouseHover(WPARAM wParam, CPoint point)
{
    if(!m_bHovering)
	{
        m_bHovering = TRUE;
        //Invalidate();
    }
	SetMsgHandled(FALSE);
}

void CEditImpl::OnMouseLeave()
{
	// ���������䣺Ҫ���ж����Ƿ����m_toolTip���ڣ�����δ���toolTip��button���������
	if(m_toolTip.IsWindow())
	{
		m_toolTip.Activate(FALSE);
		m_toolTip.Activate(TRUE);
	}
	
	m_bHovering = FALSE;
	//Invalidate();
	SetMsgHandled(FALSE);
}
