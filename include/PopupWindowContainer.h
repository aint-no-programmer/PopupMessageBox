#pragma once
 
#include <QObject>

#include "PopupWindow.h"
#include "MotionWatchdog.h"

namespace pmb
{

class PopupWindowContainer : public QObject
{
    Q_OBJECT
	QVector<PopupWindow*> m_popupWindows;
    MotionWatchdog m_motionWatchDog;
    bool m_onMotion;

    int m_displayDuration;
    QEasingCurve m_movingCurve;
    int m_appearanceDuration;
public:
    explicit PopupWindowContainer(
        int t_displayDuration, 
        const QEasingCurve& t_movingCurve = QEasingCurve::InOutSine, 
        int t_appearanceDuration = 1000, 
        QObject* parent = nullptr);

    ~PopupWindowContainer() override;

	/*
     * are PopWindows on motion at the moment  
     */
    bool isOnMotion() const;

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

    /*
     * add another message 
     */
    bool pushMessage(
        const QString& t_title,
        const QString& t_message,
        const QColor& t_color = QColor(0, 0, 0, 180));
signals:
    void s_onMotion(bool);
public slots:
    void onMotionFinished();
    void onMotionStarted();
};

}