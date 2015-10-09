#include "timer_menu.hpp"

#include "settings.hpp"

#include <cassert>

#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtGui/QAction>

TimerMenu::
TimerMenu(Settings &settings, Timer &timer)
    : m_settings { settings }
    , m_timer { timer }
{
    this->connect(&m_timer, SIGNAL(stateChanged(TimerState)),
                  this,       SLOT(onTimerStateChanged(TimerState)));
    this->connect(this, SIGNAL(aboutToShow()),
                  this,   SLOT(onAboutToShow()));
}

void TimerMenu::
addTimer(const QString &label, const size_t seconds)
{
    auto* const action = new QAction { label, this };
    action->setData(seconds);
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onTriggered()));
    this->addAction(action);
}

void TimerMenu::
onTriggered()
{
    auto* const action = dynamic_cast<QAction*>(this->sender());
    assert(action);

    const auto secs = action->data().toUInt();
    m_timer.start(secs);
}

void TimerMenu::
onTimerStateChanged(const TimerState state)
{
    if (state == TimerState::Idle) {
        this->setEnabled(true);
    }
    else {
        this->setEnabled(false);
    }
}

void TimerMenu::
onAboutToShow()
{
    this->clear();

    for (const auto &timer: m_settings.timers()) {
        addTimer(timer.name, timer.duration);
    }
}
