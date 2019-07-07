#ifndef UIBUTTONEX
#define UIBUTTONEX

#pragma once

extern LPCTSTR UI_BUTTON_EX;

namespace DuiLib
{

class CButtonExUI :public CButtonUI
{
public:
	CButtonExUI() :m_bUseAttributeList(false){}
	
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
private:
	bool m_bUseAttributeList;//∑¿÷π∂‡¥ŒΩ‚ŒˆAttributeList
};

}

#endif