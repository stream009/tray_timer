#ifndef APPEARANCE_SETTING_WIDGET_HPP
#define APPEARANCE_SETTING_WIDGET_HPP

#include <memory>

#include <QtGui/QWidget>

namespace Ui { class AppearanceSettingWidget; }

class AppearanceSettingWidget : public QWidget
{
    using Base = QWidget;
public:
    AppearanceSettingWidget();
    ~AppearanceSettingWidget();

private:
    std::unique_ptr<Ui::AppearanceSettingWidget> m_ui;
};

#endif // APPEARANCE_SETTING_WIDGET_HPP

