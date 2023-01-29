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

    explicit PopupWindowContainer(
        int t_displayDuration, 
        const QEasingCurve& t_movingCurve = QEasingCurve::InOutSine, 
        int t_appearanceDuration = 1000, 
        QObject* parent = nullptr);

    ~PopupWindowContainer() override;
    /*
     * duration of message displaying
     */
    void setDisplayDuration(int t_duration);

    int displayDuration() const;
    /*
     * type of animation moving
     */
    void setMovingCurve(const QEasingCurve& t_curve);

    QEasingCurve movingCurve() const;
    /*
     * duration of message appearance
     */
    void setAppearanceDuration(int t_duration);

    int appearanceDuration() const;

    bool pushMessage(
        const QString& t_title,
        const QString& t_message,
        const QColor& t_color = QColor(0, 0, 0, 180));

    bool pushMessage(
        const QString& t_title,
        const QString& t_message,
        MessageType t_messageType = MessageType::Info);

    static QColor convert(MessageType t_messageType);
public slots:
    void onMotionFinished();
    void onMotionStarted();
};

class MotionWatchdog : public QObject
{
    Q_OBJECT
        QVector<PopupWindow*> m_observables;
public:
    //RestorationWatchdog(QtNodes::FlowScene* t_scene, QObject* t_parent = nullptr) : QObject(t_parent)
    MotionWatchdog(PopupWindowContainer* t_popupWindowContainer, QObject* t_parent = nullptr) : QObject(t_parent)
    {

        connect(this, &MotionWatchdog::s_motionStarted, t_popupWindowContainer, &PopupWindowContainer::onMotionStarted, Qt::QueuedConnection);
        connect(this, &MotionWatchdog::s_motionFinished, t_popupWindowContainer, &PopupWindowContainer::onMotionFinished, Qt::QueuedConnection);
    }
	void keepTrack(PopupWindow* t_popupWindow)
    {
        connect(t_popupWindow, &PopupWindow::s_motionStarted, this, &MotionWatchdog::onMotionStarted, Qt::QueuedConnection);
        connect(t_popupWindow, &PopupWindow::s_motionFinished, this, &MotionWatchdog::onMotionFinished, Qt::QueuedConnection);
        connect(t_popupWindow, &PopupWindow::destroyed, this, [this](QObject* t_destroyed)
        {
                const auto casted = qobject_cast<PopupWindow*>(t_destroyed);
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
public slots:
    void clear()
    {
        m_observables.clear();
    }
    void onMotionStarted(PopupWindow* t_popupWindow)
    {
        if (m_observables.empty())
        {
            emit s_motionStarted();
        }
        m_observables.push_back(t_popupWindow);
    }
    void onMotionFinished(PopupWindow* t_popupWindow)
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
signals:
    void s_motionFinished();
    void s_motionStarted();
};
