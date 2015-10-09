#ifndef TIMER_DIALOG_HPP
#define TIMER_DIALOG_HPP

#include <memory>

#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtGui/QDialog>

namespace Ui { class TimerDialog; }

class QPushButton;

class TimerDialog : public QDialog
{
    Q_OBJECT
public:
    TimerDialog(const QString &name = {}, const QTime &duration = {});
    ~TimerDialog();

    // accessor
    QString name() const;
    QTime duration() const;

    // modifier
    void setName(const QString&);
    void setDuration(const QTime&);

private:
    QPushButton &okButton();

private Q_SLOTS:
    void validate();

private:
    std::unique_ptr<Ui::TimerDialog> m_ui;
};

#endif // TIMER_DIALOG_HPP
