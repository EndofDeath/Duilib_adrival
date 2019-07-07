#pragma once

using DuiLib::CTextUI;
extern LPCTSTR UI_DRAGTEXT;
class CDragTextUI :public CTextUI
{
public:
	CDragTextUI();
	~CDragTextUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);
	void DoPostPaint(HDC hDC, const RECT& rcPaint);
private:
	POINT m_ptLastMouse;
	CDuiRect m_rcNewPos;
};

