#pragma once
 
#include <QObject>
#include <QVector>

namespace pmb
{

class PopupWindowContainer;
class PopupWindow;

class MotionWatchdog : public QObject
{
    Q_OBJECT
	QVector<QObject*> m_observables;
public:
    MotionWatchdog(PopupWindowContainer* t_popupWindowContainer);
    ~MotionWatchdog() override = default;

	void keepTrack(PopupWindow* t_popupWindow) const;
public slots:
    void clear();
    void onMotionStarted(QObject* t_popupWindow);
    void onMotionFinished(QObject* t_popupWindow);
signals:
    void s_motionFinished();
    void s_motionStarted();
};

}