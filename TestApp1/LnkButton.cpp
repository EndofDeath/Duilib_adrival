#include "stdafx.h"
#include "LnkButton.h"

const TCHAR LNK_BUTTON[] = _T("LnkButton");

namespace Duilib{


LPCTSTR LnkButton::GetClass() const
{
	return LNK_BUTTON;
}

LPVOID LnkButton::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, LNK_BUTTON) == 0) return static_cast<LnkButton*>(this);
	return CButtonUI::GetInterface(pstrName);
}

LPCTSTR LnkButton::GetLnk() const
{
	return m_sLnk;
}

void LnkButton::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("Lnk")) == 0) SetLnk(pstrValue);
	else
	{
		CButtonUI::SetAttribute(pstrName, pstrValue);
	}
}

void LnkButton::SetLnk(LPCTSTR pstrLnk)
{
	m_sLnk = pstrLnk;
}

}