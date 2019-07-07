#include "stdafx.h"
#include "WndDui.h"

INotifyCallBack* CreateNotifyCallBack(void(*pF)(TNotifyUI&))
{
	return new NotifyFunCallBack(pF);
}

bool CWndDui::TakeWindow(HWND hWnd)
{
	if (NULL == Subclass(hWnd)) return false;
	PreTakeWindow();
	DoCreate();
	return true;
}

void CWndDui::UnTakeWindow()
{
	m_PaintManager.RemoveMessageFilter(this);
	OnFinalMessage(NULL);
	m_PaintManager.Term();
	Unsubclass();
}

void CWndDui::PreTakeWindow()
{
	SetResourceType();
}

LRESULT CWndDui::DoCreate()
{
	using namespace DuiLib;
	if (NULL == m_PaintManager.GetInstance())
		m_PaintManager.SetInstance(reinterpret_cast<HINSTANCE>(&__ImageBase));

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);

	CDialogBuilder builder;
	CDuiString strResourcePath = m_PaintManager.GetResourcePath();
	if (strResourcePath.IsEmpty())
	{
		strResourcePath = m_PaintManager.GetInstancePath();
		strResourcePath += GetSkinFolder().GetData();
	}
	m_PaintManager.SetResourcePath(strResourcePath.GetData());

	switch (GetResourceType())
	{
	case UILIB_ZIP:
		m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
		break;
	case UILIB_ZIPRESOURCE:
	{
		HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
		if (hResource == NULL)
			return 0L;
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
		if (hGlobal == NULL)
		{
#if defined(WIN32) && !defined(UNDER_CE)
			::FreeResource(hResource);
#endif
			return 0L;
		}
		dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
		if (dwSize == 0)
			return 0L;
		m_lpResourceZIPBuffer = new BYTE[dwSize];
		if (m_lpResourceZIPBuffer != NULL)
		{
			::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
		}
#if defined(WIN32) && !defined(UNDER_CE)
		::FreeResource(hResource);
#endif
		m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
	}
	break;
	}

	CControlUI* pRoot = NULL;
	if (GetResourceType() == UILIB_RESOURCE)
	{
		STRINGorID xml(_ttoi(GetSkinFile().GetData()));
		pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
	}
	else
		pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
	ASSERT(pRoot);
	if (pRoot == NULL)
	{
		MessageBox(NULL, _T("Load Xml file faild!"), _T("Duilib"), MB_OK | MB_ICONERROR);
		ExitProcess(1);
		return 0;
	}
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	InitWindow();
	return 0;
}

void CWndDui::InitWindow()
{
	CenterWindow();
	ShowWindow();
	m_PaintManager.Invalidate();
}

void CWndDui::SetResourceType(UINT uType)
{
	switch (uType)
	{
	case 1:_type = DuiLib::UILIB_RESOURCETYPE::UILIB_FILE; break;
	case 2:_type = DuiLib::UILIB_RESOURCETYPE::UILIB_ZIP; break;
	case 3:_type = DuiLib::UILIB_RESOURCETYPE::UILIB_RESOURCE; break;
	case 4:_type = DuiLib::UILIB_RESOURCETYPE::UILIB_ZIPRESOURCE; break;
	default:
		_type = DuiLib::UILIB_RESOURCETYPE::UILIB_FILE;
		break;
	}
}

void CWndDui::AddNotifyCustom(void(*pF)(TNotifyUI&))
{
	RemoveNotifyCustom();
	m_pINotifyCustom = CreateNotifyCallBack(pF);
}

void CWndDui::RemoveNotifyCustom()
{
	if (NULL != m_pINotifyCustom)
		delete m_pINotifyCustom;
	m_pINotifyCustom = NULL;
}

void CWndDui::NotifyCustom(TNotifyUI& msg)
{
	if (NULL != m_pINotifyCustom)
		m_pINotifyCustom->DoNotify(msg);
}

void CWndDui::Notify(TNotifyUI& msg)
{
	NotifyCustom(msg);
	NotifyPump(msg);
}