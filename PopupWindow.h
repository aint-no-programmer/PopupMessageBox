#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopupWindow : public QWidget
{
    Q_OBJECT
 
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
 
    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;
 
public:
    explicit PopupWindow(QWidget *parent = nullptr);
    ~PopupWindow() override = default;
signals:
    void s_movedUp();                       // Signals when moving up is finished
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
    QColor m_color{0,0,0,180};
    QLabel m_message, m_title;
    QGridLayout m_layout;
    QPropertyAnimation m_opacityAnimation;
    QPropertyAnimation m_movementAnimation;
    float m_popupOpacity;
    int m_popupMoveup;
    QTimer *m_timer;
};
