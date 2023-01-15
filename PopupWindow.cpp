#include "PopupWindow.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>




#include <QDesktopWidget>
#include <QThread>

PopupWindow::PopupWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |        // Disable window decoration
                   Qt::Tool |                       // Discard display in a separate window
                   Qt::WindowStaysOnTopHint);       // Set on top of all windows
    setAttribute(Qt::WA_TranslucentBackground);     // Indicates that the background will be transparent
    setAttribute(Qt::WA_ShowWithoutActivating);     // At the show, the widget does not get the focus automatically

    m_animation.setTargetObject(this);                // Set the target animation
    m_animation.setPropertyName("popupOpacity");

    m_label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_label.setStyleSheet("QLabel { color : white; "
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");

    m_layout.addWidget(&m_label, 0, 0);
    setLayout(&m_layout);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PopupWindow::hideAnimation);
}

void PopupWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopupWindow::setPopupText(const QString &text)
{
    m_label.setText(text);    // Set the text in the Label
    adjustSize();           // With the recalculation notice sizes
}

void PopupWindow::show()
{
    setWindowOpacity(0.0);  // Set the transparency to zero

    m_animation.setDuration(1000);     // Configuring the duration of the animation
    m_animation.setStartValue(0.0);   // The start value is 0 (fully transparent widget)
    m_animation.setEndValue(1.0);     // End - completely opaque widget

    setGeometry(QApplication::primaryScreen()->availableGeometry().width() - width(),
                QApplication::primaryScreen()->availableGeometry().height() - height(),
                width(),
                height());
    QWidget::show();

    m_animation.start();
    m_timer->start(10000);
}

void PopupWindow::moveUp(const int x)
{
    const auto movementAnimation = new QPropertyAnimation(this, "pos");
    movementAnimation->setEndValue(QPoint(pos().x(), pos().y() - x));
    movementAnimation->setEasingCurve(QEasingCurve::OutElastic);
    movementAnimation->start();
}

void PopupWindow::hideAnimation()
{
    m_timer->stop();
    m_animation.setDuration(1000);
    m_animation.setStartValue(1.0);
    m_animation.setEndValue(0.0);
    m_animation.start();

    connect(&m_animation, &QPropertyAnimation::finished, this, &QWidget::deleteLater);
}

void PopupWindow::setPopupOpacity(float opacity)
{
    m_popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopupWindow::getPopupOpacity() const
{
    return m_popupOpacity;
}
