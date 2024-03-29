// MFCDemoDlg.h : 头文件
//
#include "WndDui.h"
#pragma once

#include "SkinZipres\resource.h"

using DuiLib::CControlUI;
using DuiLib::CContainerUI;
using DuiLib::CDialogBuilder;
using DuiLib::IDialogBuilderCallback;
using DuiLib::CTabLayoutUI;

class ComputerExamineUI : public CContainerUI
{
public:
	ComputerExamineUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("360SafeSkin\\ComputerExamine.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};

class CDialogUI :public CWndDui
{
public:
	CDialogUI(){};
	~CDialogUI(){};
	LPCTSTR GetWindowClassName() const { return _T("UIDialog"); }
	CDuiString GetSkinFile()
	{ 
		return _T("360SafeSkin\\skin.xml");
	}
	CDuiString GetZIPFileName() const
	{
		return _T("360SafeSkin.zip");
	}

	LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
	}

	void PreTakeWindow()
	{
		SetResourceType(4);
	}

	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcscmp(pstrClass, _T("ComputerExamine")) == 0) return new ComputerExamineUI;
		return NULL;
	}
};

class CDialogUI2 :public CWndDui, public DuiLib::IListCallbackUI
{
public:
	CDialogUI2(){}
	LPCTSTR GetWindowClassName() const { return _T("UIDialog2"); }
	CDuiString GetSkinFile()
	{
		return _T("skin.xml");
	}
	CDuiString GetZIPFileName() const
	{
		return _T("ListRes.zip");
	}

	LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES2);
	}

	void PreTakeWindow()
	{
		SetResourceType(4);
	}

	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
	{
		TCHAR szBuf[MAX_PATH] = { 0 };
		switch (iSubItem)
		{
		case 0:
			_stprintf_s(szBuf, _T("%d"), iIndex);
			break;
		case 1:
		{
			_stprintf_s(szBuf, _T("test%d"), iIndex);
		}
		break;
		case 2:
		{
			_stprintf_s(szBuf, _T("string%d"), iIndex);
		}
		break;
		}
		pControl->SetUserData(szBuf);
		return pControl->GetUserData();
	}
};

// CMFCDemoDlg 对话框
class CMFCDemoDlg : public CDialog
{
// 构造
public:
	CMFCDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_MFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
private:
	CDialogUI m_dlgWnd;
	CDialogUI2 m_dlg;
	HINSTANCE m_hResource;
// 实现
protected:

	HICON m_hIcon;

	// 生成的消息映射函数
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();
	afx_msg void OnClose();

	void Notify(TNotifyUI& msg);
	void Notify2(TNotifyUI& msg);

	void DoSearch();
};
