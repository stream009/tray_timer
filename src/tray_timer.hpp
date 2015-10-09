#ifndef TRAY_TIMER_HPP
#define TRAY_TIMER_HPP

#include "context_menu.hpp"
#include "progress_icon.hpp"
#include "running_menu.hpp"
#include "settings.hpp"
#include "suspended_menu.hpp"
#include "timer.hpp"
#include "timer_menu.hpp"

#include <QtCore/QObject>
#include <QtGui/QIcon>
#include <QtGui/QSystemTrayIcon>

class QApplication;

class TrayTimer : public QObject
{
    Q_OBJECT
public:
    TrayTimer(QApplication&);

private Q_SLOTS:
    void onStarted(const size_t secs);
    void onStopped();
    void onSuspended();
    void onResumed();
    void onFinished();
    void onTic();
    void onSysTrayActivated(const QSystemTrayIcon::ActivationReason);

private:
    QSystemTrayIcon m_sysTray;
    QIcon           m_clockIcon;
    ProgressIcon    m_progressIcon;
    Timer           m_timer;
    Settings        m_settings;
    Actions         m_actions;
    TimerMenu       m_timerMenu;
    RunningMenu     m_runningMenu;
    SuspendedMenu   m_suspendedMenu;
    ContextMenu     m_contextMenu;
};

#endif //TRAY_TIMER_HPP
