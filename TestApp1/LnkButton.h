#pragma once

//#include "StdAfx.h"

namespace Duilib{

class LnkButton :public CButtonUI
{
public:
	LnkButton()=default;

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	LPCTSTR GetLnk() const;
	
private:
	void SetLnk(LPCTSTR pstrLnk);

	CDuiString m_sLnk;
};


}