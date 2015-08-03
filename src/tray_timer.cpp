#include "tray_timer.hpp"

#include <cassert>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QToolTip>

TrayTimer::
TrayTimer(QApplication &app)
    : m_clockIcon     { ":/images/Clock-icon.png" }
    , m_actions       { app, m_timer }
    , m_settings      { app }
    , m_timerMenu     { m_settings, m_timer }
    , m_runningMenu   { m_actions }
    , m_suspendedMenu { m_actions }
    , m_contextMenu   { m_actions }
{
    m_sysTray.setIcon(m_clockIcon);
    m_sysTray.show();

    this->connect(&m_timer, SIGNAL(started(const size_t)),
                  this,       SLOT(onStarted(const size_t)));
    this->connect(&m_timer, SIGNAL(stopped()),
                  this,       SLOT(onStopped()));
    this->connect(&m_timer, SIGNAL(suspended()),
                  this,       SLOT(onSuspended()));
    this->connect(&m_timer, SIGNAL(resumed()),
                  this,       SLOT(onResumed()));
    this->connect(&m_timer, SIGNAL(finished()),
                  this,       SLOT(onFinished()));
    this->connect(&m_timer, SIGNAL(tic()),
                  this,       SLOT(onTic()));

    this->connect(
        &m_sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
              this,   SLOT(onSysTrayActivated(QSystemTrayIcon::ActivationReason))
    );
}

void TrayTimer::
onStarted(const size_t secs)
{
    m_progressIcon.setMaximum(secs);
}

void TrayTimer::
onStopped()
{
    m_sysTray.setIcon(m_clockIcon);
    m_sysTray.setToolTip("");
}

void TrayTimer::
onSuspended()
{
    m_progressIcon.setSuspended(true);
    onTic();
}

void TrayTimer::
onResumed()
{
    m_progressIcon.setSuspended(false);
    onTic();
}

void TrayTimer::
onFinished()
{
    QMessageBox::information(
        nullptr, "Timer",
        "Beeep! :)\n Time is up for the alarm.");
}

void TrayTimer::
onTic()
{
    m_progressIcon.setCurrent(m_timer.reminder());
    m_sysTray.setIcon(m_progressIcon);

    const auto &tipText = QString("%1 sec").arg(m_timer.reminder());
    m_sysTray.setToolTip(tipText);

    // We need to do this in order to achieve constant tooltip update.
    if (m_sysTray.geometry().contains(QCursor::pos())) {
        QToolTip::showText(QCursor::pos(), tipText);
    }
    else {
        QToolTip::hideText();
    }
}

void TrayTimer::
onSysTrayActivated(const QSystemTrayIcon::ActivationReason reason)
{
    const auto &pos = QCursor::pos();

    switch (reason) {
    case QSystemTrayIcon::Trigger:
        switch (m_timer.status()) {
        case TimerState::Idle:
            m_timerMenu.popup(pos);
            break;
        case TimerState::Running:
            m_runningMenu.popup(pos);
            break;
        case TimerState::Suspended:
            m_suspendedMenu.popup(pos);
            break;
        default:
            assert(false);
        }
        break;
    case QSystemTrayIcon::Context:
        m_contextMenu.popup(pos);
        break;
    default:
        break;
    }
}
