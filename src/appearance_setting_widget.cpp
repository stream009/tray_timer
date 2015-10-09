#include "appearance_setting_widget.hpp"

#include "ui_appearance_setting_widget.h"

AppearanceSettingWidget::
AppearanceSettingWidget()
    : m_ui { new Ui::AppearanceSettingWidget }
{
    m_ui->setupUi(this);
}

AppearanceSettingWidget::~AppearanceSettingWidget() = default;
