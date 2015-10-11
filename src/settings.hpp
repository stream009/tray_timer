#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>

#include <QtCore/QObject>
#include <QtCore/QSettings>

class QApplication;

class Settings : public QObject
{
    Q_OBJECT
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
    std::vector<Timer> timers() const;
    QColor normalColor() const;
    QColor suspendedColor() const;
    QColor lessThan50Color() const;
    QColor lessThan20Color() const;
    size_t fontSize() const;
    QString fontName() const;

    // modifier
    void setTimers(const std::vector<Timer> &);
    void setNormalColor(const QColor&);
    void setSuspendedColor(const QColor&);
    void setLessThan50Color(const QColor&);
    void setLessThan20Color(const QColor&);
    void setFontSize(const size_t);
    void setFontName(const QString&);

Q_SIGNALS:
    void fontChanged();

private:
    template<typename T>
    T value(const QString &groupKey, const QString &key,
                                     const QVariant &defaultValue = {}) const;

    void setValue(const QString &groupKey, const QString &key,
                                           const QVariant &value);

private:
    mutable QSettings m_settings;
};

#endif // SETTINGS_HPP
