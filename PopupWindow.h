#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDebug>
#include <iostream>

#define POP_MSG_BOX

#ifdef POP_MSG_BOX
	#define PMB_TRACK(x) std::cout << x << std::endl;
#else
	#define  PMB_TRACK(x)
#endif


namespace PopMsgBox
{

class PopupWindow : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float t_opacity);
    float getPopupOpacity() const;
	/*
     * splits text by <t_line_length> characters per line
     */
    QString textSplitter(const QString& t_message, unsigned int t_line_length = 50) const;
public:
    explicit PopupWindow(
        int t_displayDuration, 
        const QEasingCurve& t_movingCurve = QEasingCurve::InOutSine, 
        int t_appearanceDuration = 1000, 
        QWidget *parent = nullptr);

    ~PopupWindow() override
    {
        PMB_TRACK("~PopupWindow()")
    }
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
signals:
    void s_motionStarted(QObject*);                   //Signals when starts the motion
    void s_motionFinished(QObject*);                  //Signals when the motion is finished
protected:
    void paintEvent(QPaintEvent *event);    // The background will be drawn through the redraw method
 
public slots:
    void createMessage(const QString &t_title, const QString &t_message, const QColor& t_color = QColor(0,0,0,180)); // Setting text notification
    void show();                            /* own widget displaying method 
                                             * It is necessary to pre-animation settings
                                             * */
    void moveUp(int x);
private slots:
    void hideAnimation();                   // Slot start the animation hide
 
private:
    int m_displayDuration;
    QEasingCurve m_movingCurve;
    int m_appearanceDuration;
    QColor m_color;
    QLabel m_message, m_title;
    QGridLayout m_layout;
    QPropertyAnimation m_opacityAnimation;
    QPropertyAnimation m_movementAnimation;
    float m_popupOpacity;
    int m_popupMoveup;
    QTimer *m_timer;
};

}