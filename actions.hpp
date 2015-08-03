#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "timer.hpp"

#include <QtGui/QAction>

class ActionBase : public QAction
{
    Q_OBJECT
public:
    ActionBase(const QString &title)
        : QAction { title, nullptr }
    {
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private:
    virtual void operator()() = 0;

private Q_SLOTS:
    void onTriggered() { (*this)(); }
};

class StopAction : public ActionBase
{
    Q_OBJECT
public:
    StopAction(Timer &timer);

private:
    void operator()() override;

private Q_SLOTS:
    void onTimerStateChanged(const TimerState);

private:
    Timer &m_timer;
};

class SuspendAction : public ActionBase
{
    Q_OBJECT
public:
    SuspendAction(Timer &timer);

private:
    void operator()() override;

private Q_SLOTS:
    void onTimerStateChanged(const TimerState);

private:
    Timer &m_timer;
};

class ResumeAction : public ActionBase
{
    Q_OBJECT
public:
    ResumeAction(Timer &timer);

private:
    void operator()() override;

private Q_SLOTS:
    void onTimerStateChanged(const TimerState);

private:
    Timer &m_timer;
};

class QuitAction : public ActionBase
{
public:
    QuitAction(QApplication&);

private:
    void operator()() override;

private:
    QApplication &m_app;
};

class PropertyAction : public ActionBase
{
public:
    PropertyAction();

private:
    void operator()() override;
};

class AboutAction : public ActionBase
{
public:
    AboutAction();

private:
    void operator()() override;
};

class Actions
{
public:
    Actions(QApplication&, Timer&);

    QAction &stop()     { return m_stopAction; };
    QAction &suspend()  { return m_suspendAction; }
    QAction &resume()   { return m_resumeAction; }
    QAction &quit()     { return m_quitAction; }
    QAction &property() { return m_propertyAction; }
    QAction &about()    { return m_aboutAction; }

private:
    StopAction     m_stopAction;
    SuspendAction  m_suspendAction;
    ResumeAction   m_resumeAction;
    QuitAction     m_quitAction;
    PropertyAction m_propertyAction;
    AboutAction    m_aboutAction;
};

#endif // ACTIONS_HPP
