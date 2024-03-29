// MFCDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDemoApp

BEGIN_MESSAGE_MAP(CMFCDemoApp, CWinApp)
END_MESSAGE_MAP()


// CMFCDemoApp 构造

CMFCDemoApp::CMFCDemoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
#ifdef _WINDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
}


// 唯一的一个 CMFCDemoApp 对象

CMFCDemoApp theApp;


// CMFCDemoApp 初始化

BOOL CMFCDemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	
	CMFCDemoDlg *pdlg = new CMFCDemoDlg;
	m_pMainWnd = pdlg;
	pdlg->Create(IDD_MFCDEMO_DIALOG);
	pdlg->ShowWindow(SW_SHOW);

	//返回 True 启动消息泵,返回 FALSE 以退出应用程序，
	return TRUE;
}

int CMFCDemoApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	if (NULL == m_pMainWnd)
	{ 
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}
	return CWinApp::ExitInstance();
}