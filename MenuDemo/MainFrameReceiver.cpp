#include "stdafx.h"
#include "MainFrameReceiver.h"

MainFrameReceiver::MainFrameReceiver()
	:m_pNotifier(NULL)
{
}

MainFrameReceiver::~MainFrameReceiver()
{
	RemoveNotify();
}

void MainFrameReceiver::AddNotify(INotifyUI* pNotifier)
{
	m_pNotifier = pNotifier;
}

void MainFrameReceiver::RemoveNotify()
{
	m_pNotifier = NULL;
}

BOOL MainFrameReceiver::Receive(ContextMenuParam param)
{
	if (m_pNotifier)
		m_pNotifier->Notify(param.Msg);
	return TRUE;
}