#include "tray_timer.hpp"

#include <cassert>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QToolTip>

#include <QtCore/QDebug>
#include <QtCore/QElapsedTimer>

TrayTimer::
TrayTimer(QApplication &app)
    : m_clockIcon     { ":/images/Clock-icon.png" }
    , m_settings      { app }
    , m_actions       { app, m_timer, m_settings }
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
                  this,       SLOT(onTic()));
    this->connect(&m_timer, SIGNAL(resumed()),
                  this,       SLOT(onTic()));
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
onFinished()
{
    QMessageBox::information(
        nullptr, "Timer",
        "Beeep! :)\n Time is up for the alarm.");
}

void TrayTimer::
onTic()
{
    const auto &seconds = m_timer.reminder();

    m_progressIcon.setCurrent(seconds);

    changeColor();

    QFont font { m_settings.fontName(),
                 static_cast<int>(m_settings.fontSize()) };
    m_progressIcon.setFont(font);

    m_sysTray.setIcon(m_progressIcon);

    const auto &tipText = QString("%1 sec").arg(seconds);
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

void TrayTimer::
changeColor()
{
    if (m_timer.status() == TimerState::Suspended) {
        m_progressIcon.setColor(m_settings.suspendedColor());
    }
    else {
        const double ratio =
            static_cast<double>(m_timer.reminder()) / m_timer.timeOut();
        if (ratio <= 0.2) {
            m_progressIcon.setColor(m_settings.lessThan20Color());
        }
        else if (ratio <= 0.5) {
            m_progressIcon.setColor(m_settings.lessThan50Color());
        }
        else {
            m_progressIcon.setColor(m_settings.normalColor());
        }
    }
}
