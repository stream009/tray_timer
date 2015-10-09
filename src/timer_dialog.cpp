#include "timer_dialog.hpp"

#include "ui_timer_dialog.h"

#include <cassert>

#include <QtGui/QPushButton>

TimerDialog::
TimerDialog(const QString &name, const QTime &duration)
    : m_ui { new Ui::TimerDialog }
{
    assert(m_ui);
    m_ui->setupUi(this);

    setName(name);
    setDuration(duration);

    this->connect(m_ui->name, SIGNAL(textEdited(const QString&)),
                  this,         SLOT(validate()));
    this->connect(m_ui->duration, SIGNAL(timeChanged(const QTime&)),
                  this,             SLOT(validate()));

    validate();
};

TimerDialog::~TimerDialog() = default;

QString TimerDialog::
name() const
{
    return m_ui->name->text();
}

QTime TimerDialog::
duration() const
{
    return m_ui->duration->time();
}

void TimerDialog::
setName(const QString &name)
{
    m_ui->name->setText(name);
}

void TimerDialog::
setDuration(const QTime &time)
{
    m_ui->duration->setTime(time);
}

QPushButton &TimerDialog::
okButton()
{
    auto* const button = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    assert(button);
    return *button;
}

void TimerDialog::
validate()
{
    const auto valid =
                !name().isEmpty() && duration() > QTime { 0, 0, 0, 0 };
    okButton().setEnabled(valid);
}
