#ifndef POPUP_H
#define POPUP_H
 
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
    explicit PopupWindow(QWidget *parent = 0);
    ~PopupWindow() override = default;
protected:
    void paintEvent(QPaintEvent *event);    // The background will be drawn through the redraw method
 
public slots:
    void setText(const QString &t_title, const QString &t_message); // Setting text notification
    void show();                            /* own widget displaying method 
                                             * It is necessary to pre-animation settings
                                             * */
    void moveUp(const int x);
private slots:
    void hideAnimation();                   // Slot start the animation hide
 
private:
    QLabel m_message, m_title;
    QGridLayout m_layout;
    QPropertyAnimation m_opacityAnimation;
    QPropertyAnimation m_movementAnimation;
    float m_popupOpacity;
    int m_popupMoveup;
    QTimer *m_timer;
};
 
#endif // POPUP_H
