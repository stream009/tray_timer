#ifndef TIMER_HPP
#define TIMER_HPP

#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QTimer>

enum class TimerState {
    Idle, Running, Suspended,
};

class Timer : public QObject
{
    Q_OBJECT
public:
    Timer();

    void start(const size_t timeOut);
    void stop();
    void suspend();
    void resume();

    TimerState status() const { return m_state; }

    size_t reminder() const
    {
        return m_timeOutSec - m_currentSec;
    }

Q_SIGNALS:
    void started(const size_t timeOut) const;
    void stopped() const;
    void suspended() const;
    void resumed() const;
    void finished() const;
    void tic() const;
    void stateChanged(const TimerState);

private Q_SLOTS:
    void onTic();

private:
    void changeState(const TimerState);

private:
    QTimer m_timer;
    size_t m_currentSec = 0;
    size_t m_timeOutSec = 1;
    TimerState m_state = TimerState::Idle;
};

#endif // TIMER_HPP
