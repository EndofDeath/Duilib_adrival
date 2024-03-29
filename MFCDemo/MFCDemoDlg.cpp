// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using DuiLib::CPaintManagerUI;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMFCDemoDlg 对话框

CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONTEST, &CMFCDemoDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFCDemoDlg 消息处理程序

BOOL CMFCDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hResource = LoadLibrary("SkinZipres.dll");
	CPaintManagerUI::SetResourceDll(m_hResource);
	m_dlgWnd.TakeWindow(this->GetSafeHwnd());
	m_dlgWnd.AddNotifyCustom(this,&CMFCDemoDlg::Notify);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCDemoDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (IDYES != AfxMessageBox("change view!", MB_YESNO))
		return;
	m_dlgWnd.UnTakeWindow();
	m_dlg.TakeWindow(this->GetSafeHwnd());
	m_dlg.AddNotifyCustom(this, &CMFCDemoDlg::Notify2);
}

void CMFCDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	FreeLibrary(m_hResource);
	PostQuitMessage(0);
}

void CMFCDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	CDialog::OnClose();
}

void CMFCDemoDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName().Compare(_T("menubtn")) == 0)
		{
			OnBnClickedButton1();
			return;
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_dlgWnd.FindControl(_T("switch")));
		if (name == _T("examine"))
			pControl->SelectItem(0);
		else if (name == _T("trojan"))
			pControl->SelectItem(1);
		else if (name == _T("plugins"))
			pControl->SelectItem(2);
		else if (name == _T("vulnerability"))
			pControl->SelectItem(3);
		else if (name == _T("rubbish"))
			pControl->SelectItem(4);
		else if (name == _T("cleanup"))
			pControl->SelectItem(5);
		else if (name == _T("fix"))
			pControl->SelectItem(6);
		else if (name == _T("tool"))
			pControl->SelectItem(7);
	}
}

void CMFCDemoDlg::DoSearch()
{
	using namespace DuiLib;

	CListUI* pList = static_cast<CListUI*>(m_dlg.FindControl(_T("domainlist")));
	pList->SetTextCallback(&m_dlg);
	for (int i = 0; i<10; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		if (pListElement != NULL)
		{
			if (pList) pList->Add(pListElement);
		}
	}
}

static DWORD WINAPI Search(LPVOID lpParameter)
{
	CMFCDemoDlg* p = (CMFCDemoDlg*)lpParameter;
	p->DoSearch();
	return 0;
}

void CMFCDemoDlg::Notify2(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn"))
		{
			CreateThread(NULL, 0, Search, (LPVOID)this, 0, NULL);	
		}
	}
}