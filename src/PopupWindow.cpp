#include "PopupWindow.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>

pmb::PopupWindow::PopupWindow(int t_displayDuration, const QEasingCurve& t_movingCurve, int t_appearanceDuration, QWidget* parent)
    : QWidget(parent),
	m_displayDuration(t_displayDuration),
	m_movingCurve(t_movingCurve),
	m_appearanceDuration(t_appearanceDuration)
{
    setWindowFlags(Qt::FramelessWindowHint |        // Disable window decoration
                   Qt::ToolTip |                    /* Discard display in a separate window (like as Qt::Tool)
                                                     * With attribute <Qt::WA_X11NetWmWindowTypeToolBar> provide non multi notification tabs in toolbar on Linux (like as Qt::SubWindow)
                                                     * Also provides non-disappearing menu (QWidget::contextMenuEvent) in MainWindow during notification
                                                     */
                   Qt::WindowStaysOnTopHint);       // Set on top of all windows
    setAttribute(Qt::WA_TranslucentBackground);     // Indicates that the background will be transparent
    setAttribute(Qt::WA_ShowWithoutActivating);     // At the show, the widget does not get the focus automatically
    setAttribute(Qt::WA_X11NetWmWindowTypeToolBar); // see above about <Qt::SubWindow>

    m_opacityAnimation.setTargetObject(this);                // Set the target animation
    m_opacityAnimation.setPropertyName("popupOpacity");
    m_movementAnimation.setTargetObject(this);
    m_movementAnimation.setPropertyName("pos");

    connect(&m_movementAnimation, &QPropertyAnimation::stateChanged, this, [this](QAbstractAnimation::State t_newState, QAbstractAnimation::State t_oldState)
    {
        if (t_newState == QAbstractAnimation::State::Running)
        {
            emit s_motionStarted(this);
        }
    });
    connect(&m_movementAnimation, &QPropertyAnimation::finished, this, [this]()
    {
    	PMB_TRACE("QPropertyAnimation::finished -> PopupWindow::s_motionFinished")
	    emit s_motionFinished(this);
    });

    m_layout.addWidget(&m_title, 0, 0);
    //m_title.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_title.setStyleSheet("QLabel { color : white; "
                        "font-weight: bold;"
                        "font-size: 10pt;"
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");

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

void pmb::PopupWindow::setDisplayDuration(int t_duration)
{
	m_displayDuration = t_duration;
}

int pmb::PopupWindow::displayDuration() const
{
	return m_displayDuration;
}

void pmb::PopupWindow::setMovingCurve(const QEasingCurve& t_curve)
{
	m_movingCurve = t_curve;
}

QEasingCurve pmb::PopupWindow::movingCurve() const
{
	return m_movingCurve;
}

void pmb::PopupWindow::setAppearanceDuration(int t_duration)
{
	m_appearanceDuration = t_duration;
}

int pmb::PopupWindow::appearanceDuration() const
{
	return m_appearanceDuration;
}

void pmb::PopupWindow::paintEvent(QPaintEvent *event)
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

void pmb::PopupWindow::createMessage(const QString &t_title, const QString &t_message, const QColor &t_color)
{

    if (m_color != t_color)
    {
        m_color = t_color;
    }
    m_title.setText(textSplitter(t_title, 30));
    m_message.setText(textSplitter(t_message));      // Set the text in the Label
    adjustSize();                                   // With the recalculation notice sizes
}

void pmb::PopupWindow::show()
{
    setWindowOpacity(0.0);  // Set the transparency to zero

    m_opacityAnimation.setDuration(m_appearanceDuration);     // Configuring the duration of the animation
    m_opacityAnimation.setStartValue(0.0);   // The start value is 0 (fully transparent widget)
    m_opacityAnimation.setEndValue(1.0);     // End - completely opaque widget

    setGeometry(QApplication::primaryScreen()->availableGeometry().width() - width(),
                QApplication::primaryScreen()->availableGeometry().height() - height(),
                width(),
                height());
    QWidget::show();

    m_opacityAnimation.start();
    m_timer->start(m_displayDuration);
}

void pmb::PopupWindow::moveUp(int x)
{
    m_movementAnimation.setEndValue(QPoint(pos().x(), pos().y() - x));
    m_movementAnimation.setEasingCurve(m_movingCurve);
    m_movementAnimation.setDuration(m_appearanceDuration);
	m_movementAnimation.start();
    PMB_TRACE("-> m_movementAnimation.start()")
}

void pmb::PopupWindow::hideAnimation()
{
    PMB_TRACE("PopupWindow::hideAnimation()")
    m_timer->stop();
    m_opacityAnimation.setDuration(m_appearanceDuration);
    m_opacityAnimation.setStartValue(1.0);
    m_opacityAnimation.setEndValue(0.0);
    m_opacityAnimation.start();
    
    connect(&m_opacityAnimation, &QPropertyAnimation::finished, this, &QWidget::deleteLater);
}

void pmb::PopupWindow::setPopupOpacity(float t_opacity)
{
    m_popupOpacity = t_opacity;

    setWindowOpacity(t_opacity);
}

float pmb::PopupWindow::getPopupOpacity() const
{
    return m_popupOpacity;
}

QString pmb::PopupWindow::textSplitter(const QString& t_message, unsigned t_line_length) const
{
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
}