#pragma once
 
#include <QObject>

#include "PopupWindow.h"

class PopupWindowContainer : public QObject
{
    Q_OBJECT
	QVector<PopupWindow*> m_popupWindows;
public:
    enum class MessageType
    {
	    Info = 0,
        Warning,
        Error,
        None = 255
    };

    explicit PopupWindowContainer(QObject* parent = nullptr) : QObject(parent)
    {
	    
    }

    ~PopupWindowContainer() override
    {
		for (const auto& e : m_popupWindows)
		{
            e->deleteLater();
		}
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

        auto popupWindow = new PopupWindow;
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
        case MessageType::Info:     return QColor(Qt::blue);//QColor(0, 0, 0, 180);
        case MessageType::Warning:  return QColor(Qt::yellow);
        case MessageType::Error:    return QColor(Qt::red);
        case MessageType::None:     //to default
        default:
	        {
		        //error case
				return {};  //invalid QColor
	        }
	    }
    }
};
