#include "PopupWindowContainer.h"
#include <QDebug>

PopupWindowContainer::PopupWindowContainer(int t_displayDuration, const QEasingCurve& t_movingCurve,
	int t_appearanceDuration, QObject* parent): QObject(parent),
												m_motionWatchDog(this),
												m_onMotion{false},
	                                            m_displayDuration(t_displayDuration),
	                                            m_movingCurve(t_movingCurve),
	                                            m_appearanceDuration(t_appearanceDuration)
{

}

PopupWindowContainer::~PopupWindowContainer()
{
	for (const auto& e : m_popupWindows)
	{
		e->deleteLater();
	}
}

void PopupWindowContainer::setDisplayDuration(int t_duration)
{
	m_displayDuration = t_duration;
}

int PopupWindowContainer::displayDuration() const
{
	return m_displayDuration;
}

void PopupWindowContainer::setMovingCurve(const QEasingCurve& t_curve)
{
	m_movingCurve = t_curve;
}

QEasingCurve PopupWindowContainer::movingCurve() const
{
	return m_movingCurve;
}

void PopupWindowContainer::setAppearanceDuration(int t_duration)
{
	m_appearanceDuration = t_duration;
}

int PopupWindowContainer::appearanceDuration() const
{
	return m_appearanceDuration;
}

bool PopupWindowContainer::pushMessage(const QString& t_title, const QString& t_message, const QColor& t_color)
{
	if (!t_color.isValid())
	{
		//aln.todo error handling here
		return false;
	}

	//create popup
	auto popupWindow = new PopupWindow(m_displayDuration, m_movingCurve, m_appearanceDuration);
	m_motionWatchDog.keepTrack(popupWindow);

	//create message
	popupWindow->createMessage(t_title, t_message, t_color);
	//move existing messages
	for (const auto& e : m_popupWindows)
	{
		e->moveUp(popupWindow->height());
	}

	popupWindow->show();

	m_popupWindows.push_front(popupWindow);
	connect(popupWindow, &PopupWindow::destroyed, [this, popupWindow]() {
		m_popupWindows.removeOne(popupWindow);
	});

	return true;
}

bool PopupWindowContainer::pushMessage(const QString& t_title, const QString& t_message, MessageType t_messageType)
{
	return pushMessage(t_title, t_message, convert(t_messageType));
}

QColor PopupWindowContainer::convert(MessageType t_messageType)
{
	switch(t_messageType)
	{
	case MessageType::Info:     return QColor(Qt::darkBlue);//QColor(0, 0, 0, 180);
	case MessageType::Warning:  return QColor(Qt::darkYellow);
	case MessageType::Error:    return QColor(Qt::darkRed);
	case MessageType::None:     //to default
	default:
		{
			//error case
			return {};  //invalid QColor
		}
	}
}

void PopupWindowContainer::onMotionFinished()
{
	m_onMotion = false;
	emit s_onMotion(m_onMotion);
}

void PopupWindowContainer::onMotionStarted()
{
	m_onMotion = true;
	emit s_onMotion(m_onMotion);
}
