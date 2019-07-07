#include "stdafx.h"
#include "UIButtonEx.h"

LPCTSTR UI_BUTTON_EX = _T("ButtonEx");

namespace DuiLib
{

	LPCTSTR CButtonExUI::GetClass() const
	{
		return UI_BUTTON_EX;
	}

	LPVOID CButtonExUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, UI_BUTTON_EX) == 0) return static_cast<CButtonExUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void CButtonExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("AttributeList")) == 0)
		{
			if (m_pManager && !m_bUseAttributeList) {
				LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(pstrValue);
				if (pDefaultAttributes) {
					SetAttributeList(pDefaultAttributes);
				}
				m_bUseAttributeList = true;
			}
		}
		else
		{
			CButtonUI::SetAttribute(pstrName, pstrValue);
		}
	}

}