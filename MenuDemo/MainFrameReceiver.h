#pragma once

#include "UIMenu.h"

class MainFrameReceiver :public ContextMenuReceiver
{
public:
	MainFrameReceiver();
	~MainFrameReceiver();

	void AddNotify(INotifyUI* pNotifier);
	void RemoveNotify();
	BOOL Receive(ContextMenuParam param);
private:
	INotifyUI* m_pNotifier;
};

