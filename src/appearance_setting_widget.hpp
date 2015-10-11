#ifndef APPEARANCE_SETTING_WIDGET_HPP
#define APPEARANCE_SETTING_WIDGET_HPP

#include <memory>

#include <QtGui/QWidget>

class Settings;

class AppearanceSettingWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
    class ColorPanel;
    class FontPanel;

public:
    AppearanceSettingWidget(Settings&);
    ~AppearanceSettingWidget() override;

    void save();

Q_SIGNALS:
    void changed();

private:
    void connectWidgets();

private:
    std::shared_ptr<ColorPanel> m_colorPanel = nullptr;
    std::shared_ptr<FontPanel> m_fontPanel = nullptr;
};

#endif // APPEARANCE_SETTING_WIDGET_HPP
