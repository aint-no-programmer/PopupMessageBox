#pragma once
 
#include <QObject>

namespace PopMsgBox
{

class PopupWindowContainer;
class PopupWindow;

class MotionWatchdog : public QObject
{
    Q_OBJECT
	QVector<PopupWindow*> m_observables;
public:
    MotionWatchdog(PopupWindowContainer* t_popupWindowContainer);

	void keepTrack(PopupWindow* t_popupWindow);
public slots:
    void clear();
    void onMotionStarted(PopupWindow* t_popupWindow);
    void onMotionFinished(PopupWindow* t_popupWindow);
signals:
    void s_motionFinished();
    void s_motionStarted();
};

}