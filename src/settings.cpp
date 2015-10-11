#include "settings.hpp"

#include <cassert>

#include <QtCore/QVariant>
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QFont>

namespace timer_key {

    static constexpr const char* const array = "Timer";
    static constexpr const char* const name = "Name";
    static constexpr const char* const duration = "Duration";

} // namespace timer_key

static struct Settings::Timer defaultTimers[] = {
    { "30s"     , 30 },
    { "5m 0s"   , 300 },
    { "1h 0m 0s", 3600 },
};

static const size_t numDefaultTimers =
    sizeof(defaultTimers) / sizeof(defaultTimers[0]);

namespace color_key {

    static const char* const group = "Color";
    static const char* const normal = "Normal";
    static const char* const suspended = "Suspended";
    static const char* const lessThan50 = "lessThan50";
    static const char* const lessThan20 = "lessThan20";

} // namespace color_key

namespace color_default {

    static const QColor normal = Qt::green;
    static const QColor suspended = Qt::blue;
    static const QColor lessThan50 = Qt::yellow;
    static const QColor lessThan20 = Qt::red;

} // namespace default_color

namespace font_key {

    static const char* const group = "Font";
    static const char* const name = "Name";
    static const char* const size = "Size";

} // namespace font_key

static void
loadDefaultTimers(QSettings &settings)
{
    const auto size = settings.beginReadArray(timer_key::array);
    settings.endArray();
    if (size > 0) return;

    settings.beginWriteArray(timer_key::array);
    for (auto i = 0u; i < numDefaultTimers; ++i) {
        settings.setArrayIndex(i);

        settings.setValue(timer_key::name, defaultTimers[i].name);
        settings.setValue(timer_key::duration, defaultTimers[i].duration);
    }

    settings.endArray();
}

Settings::
Settings(QApplication &app)
    : m_settings { QSettings::IniFormat,
                   QSettings::UserScope,
                   app.organizationName(),
                   app.applicationName() }
{
    loadDefaultTimers(m_settings);
}

std::vector<Settings::Timer> Settings::
timers() const
{
    std::vector<Timer> result;

    const auto count = m_settings.beginReadArray(timer_key::array);

    for (auto i = 0; i < count; ++i) {
        m_settings.setArrayIndex(i);

        const auto &name = m_settings.value(timer_key::name).toString();
        assert(!name.isNull());

        const auto duration = m_settings.value(timer_key::duration).toUInt();

        result.emplace_back(name, duration);
    }

    m_settings.endArray();

    return result;
}

void Settings::
setTimers(const std::vector<Timer> &timers)
{
    m_settings.beginWriteArray(timer_key::array);

    for (auto i = 0u; i < timers.size(); ++i) {
        m_settings.setArrayIndex(i);

        m_settings.setValue(timer_key::name, timers[i].name);
        m_settings.setValue(timer_key::duration, timers[i].duration);
    }

    m_settings.endArray();
}

QColor Settings::
normalColor() const
{
    return value<QColor>(color_key::group,
                         color_key::normal, color_default::normal);
}

QColor Settings::
suspendedColor() const
{
    return value<QColor>(color_key::group,
                         color_key::suspended, color_default::suspended);
}

QColor Settings::
lessThan50Color() const
{
    return value<QColor>(color_key::group,
                         color_key::lessThan50, color_default::lessThan50);
}

QColor Settings::
lessThan20Color() const
{
    return value<QColor>(color_key::group,
                         color_key::lessThan20, color_default::lessThan20);
}

size_t Settings::
fontSize() const
{
    static QFont font;
    return value<size_t>(font_key::group,
                         font_key::size, font.pointSize());
}

QString Settings::
fontName() const
{
    static QFont font;
    return value<QString>(font_key::group,
                          font_key::name, font.family());
}

void Settings::
setNormalColor(const QColor &color)
{
    setValue(color_key::group, color_key::normal, color);
}

void Settings::
setSuspendedColor(const QColor &color)
{
    setValue(color_key::group, color_key::suspended, color);
}

void Settings::
setLessThan50Color(const QColor &color)
{
    setValue(color_key::group, color_key::lessThan50, color);
}

void Settings::
setLessThan20Color(const QColor &color)
{
    setValue(color_key::group, color_key::lessThan20, color);
}

void Settings::
setFontSize(const size_t size)
{
    setValue(font_key::group, font_key::size, size);
    Q_EMIT fontChanged();
}

void Settings::
setFontName(const QString &name)
{
    setValue(font_key::group, font_key::name, name);
    Q_EMIT fontChanged();
}

template<typename T>
T Settings::
value(const QString &groupKey, const QString &key,
                               const QVariant &defaultValue/*= {}*/) const
{
    m_settings.beginGroup(groupKey);
    const auto &result = m_settings.value(key, defaultValue).value<T>();

    m_settings.endGroup();
    return result;
}

void Settings::
setValue(const QString &groupKey, const QString &key, const QVariant &value)
{
    m_settings.beginGroup(groupKey);
    m_settings.setValue(key, value);
    m_settings.endGroup();
}
