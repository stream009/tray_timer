#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>

#include <QtCore/QSettings>

class QApplication;

class Settings : public QSettings
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

    std::vector<Timer> timers();
};

#endif // SETTINGS_HPP
