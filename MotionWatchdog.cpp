#include "MotionWatchdog.h"
#include "PopupWindowContainer.h"

PopMsgBox::MotionWatchdog::MotionWatchdog(PopupWindowContainer* t_popupWindowContainer): QObject(t_popupWindowContainer)
{

	connect(this, &MotionWatchdog::s_motionStarted, t_popupWindowContainer, &PopupWindowContainer::onMotionStarted);
	connect(this, &MotionWatchdog::s_motionFinished, t_popupWindowContainer, &PopupWindowContainer::onMotionFinished);
}

void PopMsgBox::MotionWatchdog::keepTrack(PopupWindow* t_popupWindow) const
{
	connect(t_popupWindow, &PopupWindow::s_motionStarted, this, &MotionWatchdog::onMotionStarted);
	connect(t_popupWindow, &PopupWindow::s_motionFinished, this, &MotionWatchdog::onMotionFinished);
	connect(t_popupWindow, &PopupWindow::destroyed, this, &MotionWatchdog::onMotionFinished);
}

void PopMsgBox::MotionWatchdog::clear()
{
	m_observables.clear();
}

void PopMsgBox::MotionWatchdog::onMotionStarted(QObject* t_popupWindow)
{
	if (m_observables.empty())
	{
		emit s_motionStarted();
	}
	m_observables.push_back(t_popupWindow);
}

void PopMsgBox::MotionWatchdog::onMotionFinished(QObject* t_popupWindow)
{
	//remove from vector
	const int pos = m_observables.indexOf(t_popupWindow);

	if (pos == -1)
	{
		//aln.todo handle this "No such pointer in <m_observables> container."
		return;
	}

	m_observables.remove(pos);

	if (m_observables.empty())
	{
		emit s_motionFinished();
	}
}
