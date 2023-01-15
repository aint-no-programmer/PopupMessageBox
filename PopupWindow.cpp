#include "PopupWindow.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>

PopupWindow::PopupWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |        // Disable window decoration
                   Qt::Tool |                       // Discard display in a separate window
                   Qt::WindowStaysOnTopHint);       // Set on top of all windows
    setAttribute(Qt::WA_TranslucentBackground);     // Indicates that the background will be transparent
    setAttribute(Qt::WA_ShowWithoutActivating);     // At the show, the widget does not get the focus automatically

    m_opacityAnimation.setTargetObject(this);                // Set the target animation
    m_opacityAnimation.setPropertyName("popupOpacity");
    m_movementAnimation.setTargetObject(this);
    m_movementAnimation.setPropertyName("pos");

    m_layout.addWidget(&m_title, 0, 0);
    //m_title.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_title.setStyleSheet("QLabel { color : white; "
                        "font-weight: bold;"
                        "font-size: 10pt;"
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");

//    m_label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_message.setStyleSheet("QLabel { color : white; "
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");
//    m_message.setMaximumSize(500, 100);
    m_layout.addWidget(&m_message, 1, 0);
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

    painter.setBrush(QBrush(m_color));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopupWindow::createMessage(const QString &t_title, const QString &t_message, const QColor &t_color)
{
    auto wordWrapper = [](const QString& t_message, unsigned int t_line_length = 50){
        QString output_msg;
        unsigned int counter = 0;
        for (const auto& it : t_message.split(" "))
        {
            counter += it.size();
            output_msg.append(it).append(' ');
            if (counter > t_line_length)
            {
                output_msg.append(" \n");
                counter = 0;
            }
        }

        return output_msg;
    };

    if (m_color != t_color)
    {
        m_color = t_color;
    }

    m_title.setText(wordWrapper(t_title, 30));
    m_message.setText(wordWrapper(t_message));      // Set the text in the Label
    adjustSize();                                   // With the recalculation notice sizes
}

void PopupWindow::show()
{
    setWindowOpacity(0.0);  // Set the transparency to zero

    m_opacityAnimation.setDuration(1000);     // Configuring the duration of the animation
    m_opacityAnimation.setStartValue(0.0);   // The start value is 0 (fully transparent widget)
    m_opacityAnimation.setEndValue(1.0);     // End - completely opaque widget

    setGeometry(QApplication::primaryScreen()->availableGeometry().width() - width(),
                QApplication::primaryScreen()->availableGeometry().height() - height(),
                width(),
                height());
    QWidget::show();

    m_opacityAnimation.start();
    m_timer->start(10000);
}

void PopupWindow::moveUp(const int x)
{
    m_movementAnimation.setEndValue(QPoint(pos().x(), pos().y() - x));
    m_movementAnimation.setEasingCurve(QEasingCurve::OutElastic);
    m_movementAnimation.start();
}

void PopupWindow::hideAnimation()
{
    m_timer->stop();
    m_opacityAnimation.setDuration(1000);
    m_opacityAnimation.setStartValue(1.0);
    m_opacityAnimation.setEndValue(0.0);
    m_opacityAnimation.start();

    connect(&m_opacityAnimation, &QPropertyAnimation::finished, this, &QWidget::deleteLater);
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
