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
    : QSettings { QSettings::IniFormat,
                  QSettings::UserScope,
                  app.organizationName(),
                  app.applicationName() }
{
    loadDefaultTimers(*this);
}

std::vector<Settings::Timer> Settings::
timers()
{
    std::vector<Timer> result;

    const auto count = this->beginReadArray("Timer");

    for (auto i = 0; i < count; ++i) {
        this->setArrayIndex(i);

        const auto &name = this->value("Name").toString();
        assert(!name.isNull());

        const auto duration = this->value("Duration").toUInt();

        result.emplace_back(name, duration);
    }

    this->endArray();

    return result;
}
