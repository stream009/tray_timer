#include "actions.hpp"

#include "ui_about_dialog.h"

#include <QtGui/QApplication>
#include <QtGui/QDialog>

StopAction::
StopAction(Timer &timer)
    : ActionBase { "&Stop" }
    , m_timer { timer }
{
    this->connect(&timer, SIGNAL(stateChanged(TimerState)),
                  this,     SLOT(onTimerStateChanged(TimerState)));
}

void StopAction::
operator()()
{
    m_timer.stop();
}

void StopAction::
onTimerStateChanged(const TimerState state)
{
    if (state == TimerState::Running ||
        state == TimerState::Suspended)
    {
        this->setEnabled(true);
    }
    else {
        this->setEnabled(false);
    }
}

SuspendAction::
SuspendAction(Timer &timer)
    : ActionBase { "Sus&pend" }
    , m_timer { timer }
{
    this->connect(&timer, SIGNAL(stateChanged(TimerState)),
                  this,     SLOT(onTimerStateChanged(TimerState)));
}

void SuspendAction::
operator()()
{
    m_timer.suspend();
}

void SuspendAction::
onTimerStateChanged(const TimerState state)
{
    if (state == TimerState::Running) {
        this->setEnabled(true);
    }
    else {
        this->setEnabled(false);
    }
}

ResumeAction::
ResumeAction(Timer &timer)
    : ActionBase { "&Resume" }
    , m_timer { timer }
{
    this->connect(&timer, SIGNAL(stateChanged(TimerState)),
                  this,     SLOT(onTimerStateChanged(TimerState)));
}

void ResumeAction::
operator()()
{
    m_timer.resume();
}

void ResumeAction::
onTimerStateChanged(const TimerState state)
{
    if (state == TimerState::Suspended) {
        this->setEnabled(true);
    }
    else {
        this->setEnabled(false);
    }
}

QuitAction::
QuitAction(QApplication &app)
    : ActionBase { "&Quit" }
    , m_app { app }
{}

void QuitAction::
operator()()
{
    m_app.exit(0);
}

PropertyAction::
PropertyAction()
    : ActionBase { "&Property" }
{}

void PropertyAction::
operator()()
{
}

AboutAction::
AboutAction()
    : ActionBase { "&Abuot" }
{}

void AboutAction::
operator()()
{
    QDialog dialog;
    Ui::AboutDialog ui;
    ui.setupUi(&dialog);
    dialog.setFixedSize(dialog.size());
    dialog.exec();
}

Actions::
Actions(QApplication &app, Timer &timer)
    : m_stopAction    { timer }
    , m_suspendAction { timer }
    , m_resumeAction  { timer }
    , m_quitAction    { app }
{}
