#include "MotionWatchdog.h"
#include "PopupWindowContainer.h"

MotionWatchdog::MotionWatchdog(PopupWindowContainer* t_popupWindowContainer): QObject(t_popupWindowContainer)
{

	connect(this, &MotionWatchdog::s_motionStarted, t_popupWindowContainer, &PopupWindowContainer::onMotionStarted, Qt::QueuedConnection);
	connect(this, &MotionWatchdog::s_motionFinished, t_popupWindowContainer, &PopupWindowContainer::onMotionFinished, Qt::QueuedConnection);
}

void MotionWatchdog::keepTrack(PopupWindow* t_popupWindow)
{
	connect(t_popupWindow, &PopupWindow::s_motionStarted, this, &MotionWatchdog::onMotionStarted, Qt::QueuedConnection);
	connect(t_popupWindow, &PopupWindow::s_motionFinished, this, &MotionWatchdog::onMotionFinished, Qt::QueuedConnection);
	connect(t_popupWindow, &PopupWindow::destroyed, this, [this](QObject* t_destroyed)
	{
		const auto casted = static_cast<PopupWindow*>(t_destroyed);//temporary kludge with this nasty static_cast
		if (casted) 
		{
			onMotionFinished(casted);
		}
		else
		{
			//aln.todo handle error
		}
	}, Qt::QueuedConnection);
}

void MotionWatchdog::clear()
{
	m_observables.clear();
}

void MotionWatchdog::onMotionStarted(PopupWindow* t_popupWindow)
{
	if (m_observables.empty())
	{
		emit s_motionStarted();
	}
	m_observables.push_back(t_popupWindow);
}

void MotionWatchdog::onMotionFinished(PopupWindow* t_popupWindow)
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