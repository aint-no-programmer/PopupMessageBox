#include "PopupWindowContainer.h"
#include <QDebug>

pmb::PopupWindowContainer::PopupWindowContainer(int t_displayDuration, const QEasingCurve& t_movingCurve,
	int t_appearanceDuration, QObject* parent): QObject(parent),
												m_motionWatchDog(this),
												m_onMotion{false},
	                                            m_displayDuration(t_displayDuration),
	                                            m_movingCurve(t_movingCurve),
	                                            m_appearanceDuration(t_appearanceDuration)
{

}

pmb::PopupWindowContainer::~PopupWindowContainer()
{
	for (const auto& e : m_popupWindows)
	{
		if (!e->disconnect())
		{
			qWarning() << "This signal isn't disconnected.";
		}
		e->deleteLater();
	}

	PMB_TRACE("~PopupWindowContainer()")
}

bool pmb::PopupWindowContainer::isOnMotion() const
{
	return m_onMotion;
}

void pmb::PopupWindowContainer::setDisplayDuration(int t_duration)
{
	m_displayDuration = t_duration;
}

int pmb::PopupWindowContainer::displayDuration() const
{
	return m_displayDuration;
}

void pmb::PopupWindowContainer::setMovingCurve(const QEasingCurve& t_curve)
{
	m_movingCurve = t_curve;
}

QEasingCurve pmb::PopupWindowContainer::movingCurve() const
{
	return m_movingCurve;
}

void pmb::PopupWindowContainer::setAppearanceDuration(int t_duration)
{
	m_appearanceDuration = t_duration;
}

int pmb::PopupWindowContainer::appearanceDuration() const
{
	return m_appearanceDuration;
}

bool pmb::PopupWindowContainer::pushMessage(const QString& t_title, const QString& t_message, const QColor& t_color)
{
	if (!t_color.isValid())
	{
		qWarning() << "Invalid color";
		return false;
	}

	PMB_TRACE("*	create popup")
	//create popup
	auto popupWindow = new PopupWindow(m_displayDuration, m_movingCurve, m_appearanceDuration);
	m_motionWatchDog.keepTrack(popupWindow);

	PMB_TRACE("*	create message")
	//create message
	popupWindow->createMessage(t_title, t_message, t_color);
	PMB_TRACE("*	move existing messages")
	//move existing messages
	for (const auto& e : m_popupWindows)
	{
		e->moveUp(popupWindow->height());
	}

	PMB_TRACE("*	popupWindow->show()")
	popupWindow->show();

	PMB_TRACE("*	m_popupWindows.push_front(popupWindow)")
	m_popupWindows.push_front(popupWindow);
	connect(popupWindow, &PopupWindow::destroyed, [this, popupWindow]() {
		PMB_TRACE("*	m_popupWindows.removeOne(popupWindow)");
		m_popupWindows.removeOne(popupWindow);
	});

	return true;
}

void pmb::PopupWindowContainer::onMotionFinished()
{
	m_onMotion = false;
	emit s_onMotion(m_onMotion);
}

void pmb::PopupWindowContainer::onMotionStarted()
{
	m_onMotion = true;
	emit s_onMotion(m_onMotion);
}
