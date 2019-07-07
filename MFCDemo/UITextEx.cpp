#include "stdafx.h"
#include "UITextEx.h"

LPCTSTR UI_TEXT_EX = _T("TextEx");

namespace DuiLib
{

	LPCTSTR CTextExUI::GetClass() const
	{
		return UI_TEXT_EX;
	}

	LPVOID CTextExUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, UI_TEXT_EX) == 0) return static_cast<CTextExUI*>(this);
		return CTextUI::GetInterface(pstrName);
	}

	void CTextExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
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
			CTextUI::SetAttribute(pstrName, pstrValue);
		}
	}

}