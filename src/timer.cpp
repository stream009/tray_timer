#include "timer.hpp"

Timer::
Timer()
{
    m_timer.setInterval(1000);
    this->connect(&m_timer, SIGNAL(timeout()),
                  this,       SLOT(onTic()));
}

void Timer::
start(const size_t timeOut)
{
    assert(m_state == TimerState::Idle);
    assert(!m_timer.isActive());

    m_timeOutSec = timeOut;
    m_currentSec = 0;
    m_timer.start();
    changeState(TimerState::Running);

    Q_EMIT started(timeOut);
}

void Timer::
stop()
{
    assert(m_state == TimerState::Running || m_state == TimerState::Suspended);

    m_timer.stop();
    m_timeOutSec = m_currentSec = 0;
    changeState(TimerState::Idle);

    Q_EMIT stopped();
}

void Timer::
suspend()
{
    assert(m_state == TimerState::Running);
    assert(m_timer.isActive());

    m_timer.stop();
    changeState(TimerState::Suspended);

    Q_EMIT suspended();
}

void Timer::
resume()
{
    assert(m_state == TimerState::Suspended);
    assert(!m_timer.isActive());

    m_timer.start();
    changeState(TimerState::Running);

    Q_EMIT resumed();
}

void Timer::
onTic()
{
    ++m_currentSec;

    if (m_currentSec >= m_timeOutSec) {
        stop();
        Q_EMIT finished();
    }
    else {
        Q_EMIT tic();
    }

    assert(m_currentSec <= m_timeOutSec);
}

void Timer::
changeState(const TimerState state)
{
    m_state = state;
    Q_EMIT stateChanged(state);
}
