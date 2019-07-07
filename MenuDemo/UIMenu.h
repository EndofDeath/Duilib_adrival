#ifndef __UIMENU_H__
#define __UIMENU_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "observer_impl_base.hpp"

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
struct ContextMenuParam
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
	TNotifyUI Msg;
};

enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;

extern ContextMenuObserver s_context_menu_observer;

// MenuUI
extern const TCHAR* const kMenuUIClassName;// = _T("MenuUI");
extern const TCHAR* const kMenuUIInterfaceName;// = _T("Menu");

class CListUI;
class CMenuUI : public CListUI
{
public:
	CMenuUI();
	~CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoEvent(TEventUI& event);

    virtual bool Add(CControlUI* pControl);
    virtual bool AddAt(CControlUI* pControl, int iIndex);

    virtual int GetItemIndex(CControlUI* pControl) const;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
    virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);

	SIZE EstimateSize(SIZE szAvailable);
	int FindSelectable(int iIndex, bool bForward = true) const;
	bool SelectItem(int iIndex, bool bTakeFocus = false, bool bTriggerEvent = true);
};

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
extern const TCHAR* const kMenuElementUIClassName;// = _T("MenuElement");
extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);
extern LPCTSTR EXPLANDICON;
class CMenuElementUI;
class CMenuWnd : public CWindowWnd, public ContextMenuReceiver, public INotifyUI
{
public:
	CMenuWnd(HWND hParent = NULL);
	~CMenuWnd();
    void Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(HWND hWnd);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL Receive(ContextMenuParam param);
	void Notify(TNotifyUI& msg);
public:
	HWND m_hParent;
	POINT m_BasedPoint;
	STRINGorID m_xml;
	CDuiString m_sType;
    CPaintManagerUI m_pm;
    CMenuElementUI* m_pOwner;
    CMenuUI* m_pLayout;
};

class CListContainerElementUI;
class CMenuElementUI : public CListContainerElementUI
{
	friend CMenuWnd;
public:
    CMenuElementUI();
	~CMenuElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	void DrawItemText(HDC hDC, const RECT& rcItem);

	SIZE EstimateSize(SIZE szAvailable);

	bool Activate();

	void DoEvent(TEventUI& event);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	CMenuWnd* GetMenuWnd();

	void CreateMenuWnd();
	void ReleaseMenuWnd();
	bool HasSupMenu();

	bool IsSeparator();

	static const int ICON_WIDTH = 30;
	static const int EXPLANDICON_SIZE = 20;
protected:
	void SetSupMenuVisible();
	CMenuWnd* m_pWindow;
	TDrawInfo m_diIcon;
	SIZE m_szIcon;
	bool m_bSeparator;
	DWORD m_dwSeparatorColor;
	RECT m_rcSeparatorPadding;
};

} // namespace DuiLib

#endif // __UIMENU_H__
