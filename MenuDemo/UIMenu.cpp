#include "StdAfx.h"
#include "UICrack.h"
#include "UIMenu.h"

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
ContextMenuObserver s_context_menu_observer;

// MenuUI
const TCHAR* const kMenuUIClassName = _T("MenuUI");
const TCHAR* const kMenuUIInterfaceName = _T("Menu");

CMenuUI::CMenuUI()
{
	if (GetHeader() != NULL)
		GetHeader()->SetVisible(false);
}

CMenuUI::~CMenuUI()
{}

LPCTSTR CMenuUI::GetClass() const
{
    return kMenuUIClassName;
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuUIInterfaceName) == 0 ) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

void CMenuUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN)
	{
		if (IsKeyboardEnabled() && IsEnabled()) {
			switch (event.chKey) {
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
				if (m_iCurSel != -1)
				{
					CMenuElementUI* pItem = static_cast<CMenuElementUI*>(GetItemAt(m_iCurSel));
					if (NULL != pItem && NULL != pItem->GetMenuWnd())
					{
						pItem->GetMenuWnd()->m_pLayout->DoEvent(event);
						return;
					}
				}
			}
			switch (event.chKey) {
			case VK_UP:
				if (0 == m_iCurSel)
					SelectItem(FindSelectable(GetCount() - 1, true), true);
				else
					SelectItem(FindSelectable(m_iCurSel - 1, false), true);
				break;
			case VK_DOWN:
				if (GetCount() - 1 == m_iCurSel)
					SelectItem(FindSelectable(0, false), true);
				else
					SelectItem(FindSelectable(m_iCurSel + 1, true), true);
				break;
			case VK_HOME:
				SelectItem(FindSelectable(0, false), true); break;
			case VK_END:
				SelectItem(FindSelectable(GetCount() - 1, true), true); break;
			case VK_RIGHT:
				if (m_iCurSel != -1)
				{
					CMenuElementUI* pItem = static_cast<CMenuElementUI*>(GetItemAt(m_iCurSel));
					if (NULL != pItem && !pItem->HasSupMenu())
						break;
				}
			case VK_RETURN:
				if (m_iCurSel != -1) GetItemAt(m_iCurSel)->Activate(); break;
			}
			return;
		}
	}
	return CListUI::DoEvent(event);
}

bool CMenuUI::Add(CControlUI* pControl)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i)
	{
		if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL)
		{
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
		}
	}
	return CListUI::Add(pControl);
}

bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i)
	{
		if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL)
		{
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
		}
	}
	return CListUI::AddAt(pControl, iIndex);
}

int CMenuUI::GetItemIndex(CControlUI* pControl) const
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return -1;

	return CListUI::GetItemIndex(pControl);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	return CListUI::SetItemIndex(pControl, iIndex);
}

bool CMenuUI::Remove(CControlUI* pControl, bool bDoNotDestroy)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	return CListUI::Remove(pControl, bDoNotDestroy);
}

SIZE CMenuUI::EstimateSize(SIZE szAvailable)
{
	int cxFixed = 0;
    int cyFixed = 0;
    for( int it = 0; it < GetCount(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
        if( !pControl->IsVisible() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        cyFixed += sz.cy;
		if( cxFixed < sz.cx )
			cxFixed = sz.cx;
    }
    return CDuiSize(cxFixed, cyFixed);
}

int CMenuUI::FindSelectable(int iIndex, bool bForward /*= true*/) const
{
	if (GetCount() == 0) return -1;
	iIndex = CLAMP(iIndex, 0, GetCount() - 1);
	if (bForward) {
		for (int i = iIndex; i < GetCount(); i++) {
			CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName));
			if (pMenuItem != NULL && pMenuItem->IsVisible() && pMenuItem->IsEnabled() && !pMenuItem->IsSeparator()) return i;
		}
		return -1;
	}
	else {
		for (int i = iIndex; i >= 0; --i) {
			CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName));
			if (pMenuItem != NULL && pMenuItem->IsVisible() && pMenuItem->IsEnabled() && !pMenuItem->IsSeparator()) return i;
		}
		return -1;
	}
}

bool CMenuUI::SelectItem(int iIndex, bool bTakeFocus, bool bTriggerEvent)
{
	if (iIndex == m_iCurSel) return true;

	int iOldSel = m_iCurSel;
	// We should first unselect the currently selected item
	if (m_iCurSel >= 0) {
		CControlUI* pControl = GetItemAt(m_iCurSel);
		if (pControl != NULL) {
			CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
			if (pMenuItem != NULL) pMenuItem->Select(false, bTriggerEvent);
		}

		m_iCurSel = -1;
	}
	if (iIndex < 0) return false;

	CControlUI* pControl = GetItemAt(iIndex);
	if (pControl == NULL) return false;
	if (!pControl->IsVisible()) return false;
	if (!pControl->IsEnabled()) return false;

	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL) return false;
	m_iCurSel = iIndex;
	if (!pMenuItem->Select(true, bTriggerEvent)) {
		m_iCurSel = -1;
		return false;
	}
	EnsureVisible(m_iCurSel);
	if (bTakeFocus) pControl->SetFocus();
	if (m_pManager != NULL && bTriggerEvent) {
		m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, m_iCurSel, iOldSel);
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////
//
class CMenuBuilderCallback: public IDialogBuilderCallback
{
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, kMenuUIInterfaceName) == 0)
		{
			return new CMenuUI();
		}
		else if (_tcsicmp(pstrClass, kMenuElementUIInterfaceName) == 0)
		{
			return new CMenuElementUI();
		}
		return NULL;
	}
};

CMenuWnd::CMenuWnd(HWND hParent):
m_hParent(hParent),
m_pOwner(NULL),
m_pLayout(),
m_xml(_T(""))
{}

CMenuWnd::~CMenuWnd()
{
}

BOOL CMenuWnd::Receive(ContextMenuParam param)
{
	switch (param.wParam)
	{
	case 1:
		Close();
		break;
	case 2:
		{
			HWND hParent = GetParent(m_hWnd);
			while (hParent != NULL)
			{
				if (hParent == param.hWnd)
				{
					Close();
					break;
				}
				hParent = GetParent(hParent);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CMenuWnd::Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point)
{
	m_BasedPoint = point;
    m_pOwner = pOwner;
    m_pLayout = NULL;

	if (pSkinType != NULL)
		m_sType = pSkinType;

	m_xml = xml;

	s_context_menu_observer.AddReceiver(this);

	Create((m_pOwner == NULL) ? m_hParent : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif	
}

LPCTSTR CMenuWnd::GetWindowClassName() const
{
    return _T("MenuWnd");
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	if( m_pOwner != NULL ) {
		for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
			if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)) != NULL ) {
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
			}
		}
		//m_pOwner->m_pWindow = NULL;
		m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
		m_pOwner->Invalidate();
		m_pOwner->ReleaseMenuWnd();
	}
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_CREATE ) {
		if( m_pOwner != NULL) {
			LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
			RECT rcClient;
			::GetClientRect(*this, &rcClient);
			::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
				rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

			m_pm.Init(m_hWnd);
			m_pm.AddNotifier(this);
			// The trick is to add the items to the new container. Their owner gets
			// reassigned by this operation - which is why it is important to reassign
			// the items back to the righfull owner/manager when the window closes.
			m_pLayout = new CMenuUI();
			m_pLayout->SetManager(&m_pm, NULL, true);
			LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(kMenuUIInterfaceName);
			if( pDefaultAttributes ) {
				m_pLayout->SetAttributeList(pDefaultAttributes);
				m_pm.AddDefaultAttributeList(kMenuUIInterfaceName, pDefaultAttributes);
			}
			pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(EXPLANDICON);
			if (pDefaultAttributes) {
				m_pm.AddDefaultAttributeList(EXPLANDICON,pDefaultAttributes);
			}
			m_pLayout->SetBkColor(0xFFFFFFFF);
			m_pLayout->SetBorderColor(0xFF85E4FF);
			m_pLayout->SetBorderSize(0);
			m_pLayout->SetAutoDestroy(false);
			m_pLayout->EnableScrollBar();
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
				if(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ){
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
					m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
				}
			}
			m_pm.AttachDialog(m_pLayout);

			// Position the popup window in absolute space
			RECT rcOwner = m_pOwner->GetPos();
			RECT rc = rcOwner;

			int cxFixed = 0;
			int cyFixed = 0;

#if defined(WIN32) && !defined(UNDER_CE)
			MONITORINFO oMonitor = {}; 
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
			CDuiRect rcWork = oMonitor.rcWork;
#else
			CDuiRect rcWork;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
			SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

			for( int it = 0; it < m_pOwner->GetCount(); it++ ) {
				if(m_pOwner->GetItemAt(it)->GetInterface(kMenuElementUIInterfaceName) != NULL ){
					CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->GetItemAt(it));
					SIZE sz = pControl->EstimateSize(szAvailable);
					cyFixed += sz.cy;

					if( cxFixed < sz.cx )
						cxFixed = sz.cx;
				}
			}
			cyFixed += 4;
			cxFixed += 4;

			RECT rcWindow;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

			rc.top = rcOwner.top;
			rc.bottom = rc.top + cyFixed;
			::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
			rc.left = rcWindow.right;
			rc.right = rc.left + cxFixed;
			rc.right += 2;

			bool bReachBottom = false;
			bool bReachRight = false;
			LONG chRightAlgin = 0;
			LONG chBottomAlgin = 0;

			RECT rcPreWindow = {0};
			ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
			ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
			while( pReceiver != NULL ) {
				CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
				if( pContextMenu != NULL ) {
					GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

					bReachRight = rcPreWindow.left >= rcWindow.right;
					bReachBottom = rcPreWindow.top >= rcWindow.bottom;
					if( pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow() 
						||  bReachBottom || bReachRight )
						break;
				}
				pReceiver = iterator.next();
			}

			if (bReachBottom)
			{
				rc.bottom = rcWindow.top;
				rc.top = rc.bottom - cyFixed;
			}

			if (bReachRight)
			{
				rc.right = rcWindow.left;
				rc.left = rc.right - cxFixed;
			}

			if( rc.bottom > rcWork.bottom )
			{
				rc.bottom = rc.top;
				rc.top = rc.bottom - cyFixed;
			}

			if (rc.right > rcWork.right)
			{
				rc.right = rcWindow.left;
				rc.left = rc.right - cxFixed;

				rc.top = rcWindow.bottom;
				rc.bottom = rc.top + cyFixed;
			}

			if( rc.top < rcWork.top )
			{
				rc.top = rcOwner.top;
				rc.bottom = rc.top + cyFixed;
			}

			if (rc.left < rcWork.left)
			{
				rc.left = rcWindow.right;
				rc.right = rc.left + cxFixed;
			}

			MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
		}
		else {
			m_pm.Init(m_hWnd);
			m_pm.AddNotifier(this);
			CDialogBuilder builder;
			CMenuBuilderCallback menuCallback;

			CControlUI* pRoot = builder.Create(m_xml, m_sType.GetData(), &menuCallback, &m_pm);
			m_pm.AttachDialog(pRoot);
			m_pLayout = static_cast<CMenuUI*>(pRoot);
#if defined(WIN32) && !defined(UNDER_CE)
			MONITORINFO oMonitor = {}; 
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
			CDuiRect rcWork = oMonitor.rcWork;
#else
			CDuiRect rcWork;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
			SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
			szAvailable = pRoot->EstimateSize(szAvailable);
			m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

			DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;

			SIZE szInit = m_pm.GetInitSize();
			CDuiRect rc;
			CDuiPoint point = m_BasedPoint;
			rc.left = point.x;
			rc.top = point.y;
			rc.right = rc.left + szInit.cx;
			rc.bottom = rc.top + szInit.cy;

			int nWidth = rc.GetWidth();
			int nHeight = rc.GetHeight();

			if (dwAlignment & eMenuAlignment_Right)
			{
				rc.right = point.x;
				rc.left = rc.right - nWidth;
			}

			if (dwAlignment & eMenuAlignment_Bottom)
			{
				rc.bottom = point.y;
				rc.top = rc.bottom - nHeight;
			}

			SetForegroundWindow(m_hWnd);
			MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
			SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_SHOWWINDOW);
		}
		return 0;
    }
    else if( uMsg == WM_CLOSE ) {
		if( m_pOwner != NULL )
		{
			m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
			m_pOwner->SetPos(m_pOwner->GetPos(), false);
			m_pOwner->SetFocus();
		}
	}
	else if( uMsg == WM_RBUTTONDOWN || uMsg == WM_CONTEXTMENU || uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDBLCLK )
	{
		return 0L;
	}
	else if( uMsg == WM_KILLFOCUS )
	{
		HWND hFocusWnd = (HWND)wParam;

		BOOL bInMenuWindowList = FALSE;
		ContextMenuParam param;
		param.hWnd = GetHWND();

		ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
		ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
				bInMenuWindowList = TRUE;
				break;
			}
			pReceiver = iterator.next();
		}

		if( !bInMenuWindowList ) {
			param.wParam = 1;
			s_context_menu_observer.RBroadcast(param);

			return 0;
		}
	}
	else if( uMsg == WM_KEYDOWN)
	{
		if( wParam == VK_ESCAPE)
		{
			Close();
		}
		if (OnKeyDown(uMsg, wParam, lParam))
			return 0;
	}

    LRESULT lRes = 0;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pLayout)
		return 0;
	if (wParam == VK_LEFT && NULL != m_pOwner)
	{
		Close();
		return TRUE;
	}
	TEventUI event = { 0 };
	event.Type = UIEVENT_KEYDOWN;
	event.pSender = m_pLayout;
	event.wParam = wParam;
	event.lParam = lParam;
	event.chKey = (TCHAR)wParam;
	event.dwTimestamp = ::GetTickCount();
	m_pLayout->Event(event);
	return TRUE;
}

void CMenuWnd::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0 || _tcsicmp(msg.sType, DUI_MSGTYPE_ITEMCLICK) == 0)
	{ 
		ContextMenuParam param = { 0 };
		param.Msg = msg;
		s_context_menu_observer.Broadcast(param);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
const TCHAR* const kMenuElementUIClassName = _T("MenuElement");
const TCHAR* const kMenuElementUIInterfaceName = _T("MenuElement");

LPCTSTR EXPLANDICON = _T("ExplandIcon");

CMenuElementUI::CMenuElementUI():
m_pWindow(NULL), m_bSeparator(false), m_dwSeparatorColor(0xFFBCBFC4)
{
	m_cxyFixed.cy = 25;
	m_bMouseChildEnabled = true;
	m_bKeyboardEnabled = true;
	m_szIcon = { 0, 0 };
	m_rcSeparatorPadding = { ICON_WIDTH, 0, 5, 0 };
	//SetMouseChildEnabled(false);
}

CMenuElementUI::~CMenuElementUI()
{
	ReleaseMenuWnd();
}

LPCTSTR CMenuElementUI::GetClass() const
{
	return kMenuElementUIClassName;
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuElementUIInterfaceName) == 0 ) return static_cast<CMenuElementUI*>(this);    
    return CListContainerElementUI::GetInterface(pstrName);
}

bool CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
    RECT rcTemp = { 0 };
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;

    CRenderClip clip;
    CRenderClip::GenerateClip(hDC, rcTemp, clip);
    CMenuElementUI::DrawItemBk(hDC, m_rcItem);

	if (m_bSeparator)
	{
		RECT rcSeparator = m_rcItem;
		if ((m_rcSeparatorPadding.left + m_rcSeparatorPadding.right) < GetWidth())
		{
			rcSeparator.left += m_rcSeparatorPadding.left;
			rcSeparator.right -= m_rcSeparatorPadding.right;
		}
		if ((m_rcSeparatorPadding.top + m_rcSeparatorPadding.bottom) < GetHeight())
		{ 
			rcSeparator.top += m_rcSeparatorPadding.top;
			rcSeparator.bottom -= m_rcSeparatorPadding.bottom;
		}
		rcSeparator.top = (rcSeparator.top + rcSeparator.bottom) / 2;
		rcSeparator.bottom = rcSeparator.top;
		CRenderEngine::DrawLine(hDC, rcSeparator, 1, GetAdjustColor(m_dwSeparatorColor), PS_SOLID);
	}
	else
	{
		DrawItemText(hDC, m_rcItem);
		//draw icon
		if (0 < m_szIcon.cx && 0 < m_szIcon.cy)
		{ 
			RECT rcIcon = m_rcItem;
			rcIcon.right = rcIcon.left + ICON_WIDTH;
			int nxOffset = (ICON_WIDTH - m_szIcon.cx) / 2;
			int nyOffset = (GetHeight() - m_szIcon.cy) / 2;
			if (0 < nxOffset)
			{
				rcIcon.left = m_rcItem.left + nxOffset;
				rcIcon.right = rcIcon.left + m_szIcon.cx;
			}
			if (0 < nyOffset)
			{
				rcIcon.top = m_rcItem.top + nyOffset;
				rcIcon.bottom = rcIcon.top + m_szIcon.cy;
			}
			CRenderEngine::DrawImage(hDC, m_pManager, rcIcon, m_rcPaint, m_diIcon);
		}
		//draw explandicon
		if (HasSupMenu())
		{
			TDrawInfo diExplandIcon = {};
			LPCTSTR sExplandIcon = m_pManager->GetDefaultAttributeList(EXPLANDICON);
			if (sExplandIcon)
				diExplandIcon.sDrawString = sExplandIcon;
			if (!diExplandIcon.sDrawString.IsEmpty())
			{
				RECT rcExplandIcon = m_rcItem;
				rcExplandIcon.left = m_rcItem.right - EXPLANDICON_SIZE;
				rcExplandIcon.top = m_rcItem.top + (GetHeight() - EXPLANDICON_SIZE) / 2;
				rcExplandIcon.bottom = rcExplandIcon.top + EXPLANDICON_SIZE;
				CRenderEngine::DrawImage(hDC, m_pManager, rcExplandIcon, m_rcPaint, diExplandIcon);
			}
		}
	}

    if( m_items.GetSize() > 0 ) {
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( pControl == pStopControl ) return false;
                if( !pControl->IsVisible() ) continue;
                if( pControl->GetInterface(kMenuElementUIInterfaceName) != NULL ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                }
            }
        }
        else {
            CRenderClip childClip;
            CRenderClip::GenerateClip(hDC, rcTemp, childClip);
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( pControl == pStopControl ) return false;
                if( !pControl->IsVisible() ) continue;
                if( pControl->GetInterface(kMenuElementUIInterfaceName) != NULL ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    CRenderClip::UseOldClipBegin(hDC, childClip);
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                    CRenderClip::UseOldClipEnd(hDC, childClip);
                }
                else {
                    if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL ) {
        if( m_pVerticalScrollBar == pStopControl ) return false;
        if (m_pVerticalScrollBar->IsVisible()) {
            if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
                if( !m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
            }
        }
    }

    if( m_pHorizontalScrollBar != NULL ) {
        if( m_pHorizontalScrollBar == pStopControl ) return false;
        if (m_pHorizontalScrollBar->IsVisible()) {
            if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
                if( !m_pHorizontalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
            }
        }
    }
    return true;
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_sText.IsEmpty() ) return;

    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    if( pInfo->bShowHtml )
        CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        NULL, NULL, nLinks, pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
    else
        CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}


SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0};
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if( cXY.cx < sz.cx )
			cXY.cx = sz.cx;
	}
	if(cXY.cy == 0) {
		TListInfoUI* pInfo = m_pOwner->GetListInfo();

		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}

		RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
		rcText.left += pInfo->rcTextPadding.left;
		rcText.right -= pInfo->rcTextPadding.right;
		if( pInfo->bShowHtml ) {   
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
		}
		else {
			CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
		}
		cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + 20;
		cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
	}

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
	return cXY;
}

void CMenuElementUI::DoEvent(TEventUI& event)
{
	if (m_bSeparator)
		return;
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if (IsEnabled()){
			CListContainerElementUI::DoEvent(event);
			if (m_pWindow) return;

			if (HasSupMenu())
			{
				m_pOwner->SelectItem(GetIndex(), false);
				SetSupMenuVisible();
				CreateMenuWnd();
				m_pWindow->m_pLayout->SetFocus();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 2;
				s_context_menu_observer.RBroadcast(param);
				m_pOwner->SelectItem(GetIndex(), true);
			}
		}
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if (IsEnabled()){
			CListContainerElementUI::DoEvent(event);

			if( m_pWindow ) return;

			if (HasSupMenu())
			{
				SetSupMenuVisible();
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 1;
				s_context_menu_observer.RBroadcast(param);
			}
        }
        return;
    }

    CListContainerElementUI::DoEvent(event);
}

void CMenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("icon")) == 0)
	{
		if (m_diIcon.sDrawString == pstrValue && m_diIcon.pImageInfo != NULL) return;
		m_diIcon.Clear();
		m_diIcon.sDrawString = pstrValue;
		Invalidate();
	}
	else if (_tcscmp(pstrName, _T("iconsize")) == 0)
	{
		LPTSTR pstr = NULL;
		m_szIcon.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		m_szIcon.cy = _tcstol(pstr + 1, &pstr, 10);   ASSERT(pstr);
	}
	else if (_tcscmp(pstrName, _T("separator")) == 0)
	{
		m_bSeparator = (_tcscmp(pstrValue, _T("true")) == 0);
		if (0 == GetHeight())
			SetFixedHeight(m_bSeparator ? 6 : 25);
	}
	else if (_tcscmp(pstrName, _T("separatorcolor")) == 0)
	{
		while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwSeparatorColor = _tcstoul(pstrValue, &pstr, 16);
	}
	else if (_tcscmp(pstrName, _T("separatorpadding")) == 0) {
		LPTSTR pstr = NULL;
		m_rcSeparatorPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		m_rcSeparatorPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		m_rcSeparatorPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		m_rcSeparatorPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
	}
	else CListContainerElementUI::SetAttribute(pstrName, pstrValue);
}

bool CMenuElementUI::Activate()
{
	if (CListContainerElementUI::Activate() && m_bSelected && !m_bSeparator)
	{
		if( m_pWindow ) return true;

		if (HasSupMenu())
		{
			SetSupMenuVisible();
			CreateMenuWnd();
			m_pWindow->m_pLayout->SetFocus();
			m_pWindow->m_pLayout->SelectItem(0, true);
		}
		else
		{
			TNotifyUI msg = {};
			msg.pSender = this;
			msg.sType = DUI_MSGTYPE_ITEMCLICK;
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 1;
			param.Msg = msg;
			s_context_menu_observer.RBroadcast(param);
		}

		return true;
	}
	return false;
}

CMenuWnd* CMenuElementUI::GetMenuWnd()
{
	return m_pWindow;
}

void CMenuElementUI::CreateMenuWnd()
{
	if( m_pWindow ) return;

	m_pWindow = new CMenuWnd(m_pManager->GetPaintWindow());
	ASSERT(m_pWindow);

	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 2;
	s_context_menu_observer.RBroadcast(param);

	m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), _T(""), CDuiPoint());
}

void CMenuElementUI::ReleaseMenuWnd()
{
	if (m_pWindow)
		delete m_pWindow;
	m_pWindow = NULL;
}

bool CMenuElementUI::HasSupMenu()
{
	bool hasSubMenu = false;
	for (int i = 0; i < m_items.GetSize(); ++i)
	{
		if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL)
		{
			hasSubMenu = true;
			break;
		}
	}
	return hasSubMenu;
}

void CMenuElementUI::SetSupMenuVisible()
{
	for (int i = 0; i < m_items.GetSize(); ++i)
	{
		if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL)
		{
			(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
			(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);
		}
	}
}

bool CMenuElementUI::IsSeparator()
{
	return m_bSeparator;
}

} // namespace DuiLib
