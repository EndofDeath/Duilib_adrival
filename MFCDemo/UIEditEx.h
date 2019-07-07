#ifndef UIEDITEX
#define UIEDITEX

#pragma once

extern LPCTSTR UI_EDIT_EX;

namespace DuiLib
{ 

class CEditExUI :public CEditUI
{
public:
	CEditExUI() :m_bUseAttributeList(false){}
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
private:
	bool m_bUseAttributeList;//∑¿÷π∂‡¥ŒΩ‚ŒˆAttributeList
};

}

#endif