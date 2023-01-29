#pragma once
 
#include <QObject>

class PopupWindowContainer;
class PopupWindow;

class MotionWatchdog : public QObject
{
    Q_OBJECT
	QVector<PopupWindow*> m_observables;
public:
    //RestorationWatchdog(QtNodes::FlowScene* t_scene, QObject* t_parent = nullptr) : QObject(t_parent)
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
