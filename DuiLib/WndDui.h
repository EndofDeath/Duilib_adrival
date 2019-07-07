#ifndef CWND_DUI
#define CWND_DUI
#pragma once

using DuiLib::CDuiString;
using DuiLib::TNotifyUI;
using DuiLib::CControlUI;
using DuiLib::INotifyUI;

class INotifyCallBack
{
public:
	virtual void DoNotify(TNotifyUI& msg) = 0;
};

class NotifyFunCallBack :public INotifyCallBack
{
	typedef void(*pNotify)(TNotifyUI& msg);
public:
	NotifyFunCallBack(pNotify pF) :_Fun(pF){}
	void DoNotify(TNotifyUI& msg)
	{
		_Fun(msg);
	}
private:
	pNotify _Fun;
};

INotifyCallBack* CreateNotifyCallBack(void(*pF)(TNotifyUI&));

template<class T>
class NotifyMemFunCallBack :public INotifyCallBack
{
	typedef void(T::*pNotify)(TNotifyUI& msg);
public:
	NotifyMemFunCallBack(T* p, pNotify pF) :_pObject(p), _MemFun(pF){}
	void DoNotify(TNotifyUI& msg)
	{
		(_pObject->*_MemFun)(msg);
	}
private:
	T* _pObject;
	pNotify _MemFun;
};

template<class T>
INotifyCallBack* CreateNotifyCallBack(T* pObject, void(T::*pF)(TNotifyUI&))
{
	return new NotifyMemFunCallBack<T>(pObject, pF);
}

template<class T>
class NotifyFunctorCallBack :public INotifyCallBack
{
public:
	NotifyFunctorCallBack(T* p) :_pFunctor(p){}
	void DoNotify(TNotifyUI& msg)
	{
		(*_pFunctor)(msg);
	}
private:
	T* _pFunctor;
};

template<class T>
INotifyCallBack* CreateNotifyCallBack(T* pObject)
{
	return new NotifyFunctorCallBack<T>(pObject);
}

class CWndDui:public DuiLib::WindowImplBase
{
public:
	CWndDui() :_type(DuiLib::UILIB_RESOURCETYPE::UILIB_FILE), m_pINotifyCustom(NULL){}
	virtual ~CWndDui(){RemoveNotifyCustom(); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNcRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	
	bool TakeWindow(HWND hWnd);
	void UnTakeWindow();

	void AddNotifyCustom(void(*pF)(TNotifyUI&));
	void RemoveNotifyCustom();

	template<class T>
	void AddNotifyCustom(T* pObject)
	{
		RemoveNotifyCustom();
		m_pINotifyCustom = CreateNotifyCallBack(pObject);
	}

	template<class T>
	void AddNotifyCustom(T* pObject, void(T::*pF)(TNotifyUI&))
	{
		RemoveNotifyCustom();
		m_pINotifyCustom = CreateNotifyCallBack(pObject, pF);
	}

	CControlUI* FindControl(LPCTSTR pstrName) const{ return m_PaintManager.FindControl(pstrName); }
protected:
	virtual CDuiString GetSkinFolder(){ return _T(""); }
	virtual CDuiString GetSkinFile() = 0;
	virtual LPCTSTR GetWindowClassName(void) const = 0;
	virtual void Notify(TNotifyUI& msg);
	//Call Custom Function
	void NotifyCustom(TNotifyUI& msg);
	virtual void PreTakeWindow();
	//Load Skin and Init Window
	LRESULT DoCreate();
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const{ return _type; }
	//1: file
	//2: zip file
	//3: xml string
	//4: zip resource(ZIPRES)
	void SetResourceType(UINT uType = 1);
private:
	DuiLib::UILIB_RESOURCETYPE _type;
	INotifyCallBack* m_pINotifyCustom;
};

#endif