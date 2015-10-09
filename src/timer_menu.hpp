#ifndef TIMER_MENU_HPP
#define TIMER_MENU_HPP

#include "timer.hpp"

#include <QtGui/QMenu>

class QString;

class Settings;

class TimerMenu : public QMenu
{
    Q_OBJECT
public:
    TimerMenu(Settings &settings, Timer &timer);

private:
    void addTimer(const QString &label, const size_t seconds);

private Q_SLOTS:
    void onTriggered();
    void onTimerStateChanged(const TimerState);
    void onAboutToShow();

private:
    Settings &m_settings;
    Timer &m_timer;
};

#endif // TIMER_MENU_HPP
