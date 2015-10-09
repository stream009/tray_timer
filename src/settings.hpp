#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>

#include <QtCore/QSettings>

class QApplication;

class Settings
{
public:
    struct Timer {
        QString name;
        size_t duration;

        Timer(const QString &n, const size_t d)
            : name { n }, duration { d } {}
    };
public:
    Settings(QApplication&);

    // accessor
    std::vector<Timer> timers();

    // modifier
    void setTimers(const std::vector<Timer> &);

private:
    QSettings m_settings;
};

#endif // SETTINGS_HPP
