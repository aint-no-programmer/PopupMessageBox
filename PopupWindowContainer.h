#pragma once
 
#include <QObject>

#include "PopupWindow.h"

class PopupWindowContainer : public QObject
{
    Q_OBJECT
	QVector<PopupWindow*> m_popupWindows;

    int m_displayDuration;
    QEasingCurve m_movingCurve;
    int m_appearanceDuration;
public:
    enum class MessageType
    {
	    Info = 0,
        Warning,
        Error,
        None = 255
    };

    explicit PopupWindowContainer(int t_displayDuration, const QEasingCurve& t_movingCurve = QEasingCurve::InOutSine, int t_appearanceDuration = 1000, QObject* parent = nullptr)
		: QObject(parent),
        m_displayDuration(t_displayDuration),
        m_movingCurve(t_movingCurve),
        m_appearanceDuration(t_appearanceDuration)
    {
	    
    }

    ~PopupWindowContainer() override
    {
		for (const auto& e : m_popupWindows)
		{
            e->deleteLater();
		}
    }
    /*
     * duration of message displaying
     */
    void setDisplayDuration(int t_duration)
    {
        m_displayDuration = t_duration;
    }
    int displayDuration() const
    {
        return m_displayDuration;
    }
    /*
     * type of animation moving
     */
    void setMovingCurve(QEasingCurve t_curve)
    {
        m_movingCurve = t_curve;
    }
    QEasingCurve movingCurve() const
    {
        return m_movingCurve;
    }
    /*
     * duration of message appearance
     */
    void setAppearanceDuration(int t_duration)
    {
        m_appearanceDuration = t_duration;
    }
    int appearanceDuration() const
    {
        return m_appearanceDuration;
    }
    bool pushMessage(
        const QString& t_title,
        const QString& t_message,
        const QColor& t_color = QColor(0, 0, 0, 180))
    {
        if (!t_color.isValid())
        {
			//aln.todo error handling here
            return false;
        }

        auto popupWindow = new PopupWindow(m_displayDuration, m_movingCurve, m_appearanceDuration);
        popupWindow->createMessage(t_title, t_message, t_color);

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

    bool pushMessage(
        const QString& t_title,
        const QString& t_message,
        MessageType t_messageType = MessageType::Info)
    {
        return pushMessage(t_title, t_message, convert(t_messageType));
    }
    static QColor convert(MessageType t_messageType)
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
};
