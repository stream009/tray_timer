#include "settings.hpp"

#include <cassert>

#include <QtGui/QApplication>

static struct Settings::Timer defaultTimers[] = {
    { "30s"     , 30 },
    { "5m 0s"   , 300 },
    { "1h 0m 0s", 3600 },
};
static const size_t numDefaultTimers =
    sizeof(defaultTimers) / sizeof(defaultTimers[0]);

static void
loadDefaultTimers(QSettings &settings)
{
    const auto size = settings.beginReadArray("Timer");
    settings.endArray();
    if (size > 0) return;

    settings.beginWriteArray("Timer");
    for (auto i = 0u; i < numDefaultTimers; ++i) {
        settings.setArrayIndex(i);

        settings.setValue("Name", defaultTimers[i].name);
        settings.setValue("Duration", defaultTimers[i].duration);
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
timers()
{
    std::vector<Timer> result;

    const auto count = m_settings.beginReadArray("Timer");

    for (auto i = 0; i < count; ++i) {
        m_settings.setArrayIndex(i);

        const auto &name = m_settings.value("Name").toString();
        assert(!name.isNull());

        const auto duration = m_settings.value("Duration").toUInt();

        result.emplace_back(name, duration);
    }

    m_settings.endArray();

    return result;
}

void Settings::
setTimers(const std::vector<Timer> &timers)
{
    m_settings.beginWriteArray("Timer");

    for (auto i = 0u; i < timers.size(); ++i) {
        m_settings.setArrayIndex(i);

        m_settings.setValue("Name", timers[i].name);
        m_settings.setValue("Duration", timers[i].duration);
    }

    m_settings.endArray();
}
