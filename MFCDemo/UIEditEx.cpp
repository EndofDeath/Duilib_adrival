#include "stdafx.h"
#include "UIEditEx.h"

LPCTSTR UI_EDIT_EX = _T("EditEx");

namespace DuiLib
{

	LPCTSTR CEditExUI::GetClass() const
	{
		return UI_EDIT_EX;
	}

	LPVOID CEditExUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, UI_EDIT_EX) == 0) return static_cast<CEditExUI*>(this);
		return CEditUI::GetInterface(pstrName);
	}

	void CEditExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
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
			CEditUI::SetAttribute(pstrName, pstrValue);
		}
	}

}