#ifndef POPUP_H
#define POPUP_H
 
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopUp : public QWidget
{
    Q_OBJECT
 
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
    Q_PROPERTY(int popupMoveup  READ getPopupMoveup  WRITE setPopupMoveup)
 
    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

    //my addition
    void setPopupMoveup(int t_popupMoveup)
    {
        if (m_popupMoveup == t_popupMoveup) return;
        m_popupMoveup = t_popupMoveup;
        emit s_moved();
    }
    int getPopupMoveup() const
    {
        return m_popupMoveup;
    }
 
public:
    explicit PopUp(QWidget *parent = 0);
    ~PopUp() override
    {
        hideAnimation();
    }
signals:
    void s_moved();
protected:
    void paintEvent(QPaintEvent *event);    // The background will be drawn through the redraw method
 
public slots:
    void setPopupText(const QString& text); // Setting text notification
    void show();                            /* own widget displaying method 
                                             * It is necessary to pre-animation settings
                                             * */
    void moveUp(const int x);
private slots:
    void onMoved(){
        move(x(), m_animation_moveup.currentValue().toInt());
    }
    void hideAnimation();                   // Slot start the animation hide
    void hide();                            /* At the end of the animation, it is checked in a given slot,
                                             * Does the widget visible, or to hide
                                             * */
 
private:
    QLabel label;           
    QGridLayout layout;     
    QPropertyAnimation animation;
    QPropertyAnimation m_animation_moveup;
    float popupOpacity;     
    int m_popupMoveup;
    QTimer *timer;          
};
 
#endif // POPUP_H
