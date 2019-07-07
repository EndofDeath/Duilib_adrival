//
// main_frame.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

#include "resource.h"
#include "main_frame.hpp"
#include "UIMenu.h"
#include "MainFrameReceiver.h"

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

static MainFrameReceiver s_Receiver;

MainFrame::MainFrame()
	:m_pMenu(NULL)
{}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString MainFrame::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = {0};
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_SKINXML);
	return szBuf;
}

CDuiString MainFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return WindowImplBase::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::InitWindow()
{
	s_Receiver.AddNotify(this);
	s_context_menu_observer.AddReceiver(&s_Receiver);
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, kClick) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btn_menu")) == 0)	
		{
			STRINGorID xml(IDR_XML_MENU);	
			TrackPopupMenu(xml, msg.ptMouse);
		}
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_ITEMCLICK) == 0)
	{
		if (_tcsicmp(msg.pSender->GetClass(), kMenuElementUIClassName) == 0)
		{
			CDuiString strText = msg.pSender->GetText();
			//MessageBox(m_hWnd, strText, DUI_MSGTYPE_ITEMCLICK, MB_OK);
		}
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_MENU) == 0)
	{
		STRINGorID xml(IDR_XML_MENU);
		TrackPopupMenu(xml, msg.ptMouse);
	}
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void MainFrame::TrackPopupMenu(STRINGorID xml, POINT point)
{
	if (m_pMenu)
		delete m_pMenu;
	m_pMenu = new CMenuWnd(m_hWnd);
	ClientToScreen(m_hWnd, &point);
	m_pMenu->Init(NULL, xml, _T("xml"), point);
}