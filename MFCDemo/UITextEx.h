#ifndef UITEXTEX
#define UITEXTEX

#pragma once

extern LPCTSTR UI_TEXT_EX;

namespace DuiLib
{ 

class CTextExUI :public CTextUI
{
public:
	CTextExUI() :m_bUseAttributeList(false){}
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
private:
	bool m_bUseAttributeList;//∑¿÷π∂‡¥ŒΩ‚ŒˆAttributeList
};

}

#endif