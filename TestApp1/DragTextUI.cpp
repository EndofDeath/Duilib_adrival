#include "stdafx.h"
#include "DragTextUI.h"

LPCTSTR UI_DRAGTEXT = _T("DragText");

CDragTextUI::CDragTextUI()
{
}


CDragTextUI::~CDragTextUI()
{
}

LPCTSTR CDragTextUI::GetClass() const
{
	return UI_DRAGTEXT;
}

LPVOID CDragTextUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, UI_DRAGTEXT) == 0) return static_cast<CDragTextUI*>(this);
	return CTextUI::GetInterface(pstrName);
}

void CDragTextUI::DoEvent(TEventUI& event)
{
	{
		if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
		{
			m_ptLastMouse = event.ptMouse;
			m_rcNewPos = m_rcItem;
			if (m_pManager) m_pManager->AddPostPaint(this);
			return;
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			RECT rcParent = m_pParent->GetPos();
			m_rcNewPos.left -= rcParent.left;
			m_rcNewPos.right -= rcParent.left;
			m_rcNewPos.top -= rcParent.top;
			m_rcNewPos.bottom -= rcParent.top;
				SetPos(m_rcNewPos);
				if (m_pManager) m_pManager->RemovePostPaint(this);
				NeedParentUpdate();
				return;
			
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			{
				LONG cx = event.ptMouse.x - m_ptLastMouse.x;
				LONG cy = event.ptMouse.y - m_ptLastMouse.y;
				m_ptLastMouse = event.ptMouse;
				RECT rc = m_rcNewPos;
				rc.left += cx;
				rc.right += cx;
				rc.top += cy;
				rc.bottom += cy;

				CDuiRect rcInvalidate = m_rcNewPos;
				m_rcNewPos = rc;
				
				rcInvalidate.Join(m_rcNewPos);
				if (m_pManager) m_pManager->Invalidate(rcInvalidate);
				
				return;
			}
		}
		if (event.Type == UIEVENT_SETCURSOR)
		{
			if (IsEnabled()) {
				::SetCursor(::LoadCursor(NULL, IDC_HAND));
				return;
			}
		}
	}
	CTextUI::DoEvent(event);
}

void CDragTextUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
	int nWidth = GetWidth();
	int nHeight = GetHeight();
	RECT rcDropPos = m_rcNewPos;
	RECT rcParent = m_pParent->GetPos();
	if (m_rcNewPos.left < rcParent.left)
	{
		rcDropPos.left = rcParent.left;
		rcDropPos.right = rcDropPos.left + nWidth;
	}
	if (m_rcNewPos.right > rcParent.right)
	{
		rcDropPos.right = rcParent.right;
		rcDropPos.left = rcDropPos.right - nWidth;
	}
	if (m_rcNewPos.top < rcParent.top)
	{
		rcDropPos.top = rcParent.top;
		rcDropPos.bottom = rcDropPos.top + nHeight;
	}
	if (m_rcNewPos.bottom > rcParent.bottom)
	{
		rcDropPos.bottom = rcParent.bottom;
		rcDropPos.top = rcDropPos.bottom - nHeight;
	}
	CRenderEngine::DrawColor(hDC, rcDropPos, 0xAA000000);
	m_rcNewPos = rcDropPos;
}