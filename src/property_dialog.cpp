#include "property_dialog.hpp"

#include "appearance_setting_widget.hpp"
#include "settings.hpp"
#include "timer_list_model.hpp"
#include "timer_list_widget.hpp"

#include <cassert>

#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>

PropertyDialog::
PropertyDialog(Settings &settings)
    : m_settings   { settings }
    , m_timer      { new TimerListWidget { m_settings } }
    , m_appearance { new AppearanceSettingWidget { m_settings } }
{
    this->resize(500, 300);

    auto* const layout = new QVBoxLayout { this };

    layout->addWidget(&createTabWidget());

    createButtonBox();
    assert(m_buttonBox);
    layout->addWidget(m_buttonBox.get());

    okButton().setEnabled(false);

    connectWidgets();
}

PropertyDialog::~PropertyDialog() = default;

QTabWidget &PropertyDialog::
createTabWidget()
{
    auto* const tab = new QTabWidget { this };
    assert(tab);

    tab->addTab(m_timer.get(), "Timer");
    tab->addTab(m_appearance.get(), "Appearance");

    return *tab;
}

void PropertyDialog::
createButtonBox()
{
    m_buttonBox.reset(new QDialogButtonBox {
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal
    });
    assert(m_buttonBox);

    this->connect(m_buttonBox.get(), SIGNAL(accepted()),
                  this,                SLOT(accept()));
    this->connect(m_buttonBox.get(), SIGNAL(rejected()),
                  this,                SLOT(reject()));
}

QPushButton &PropertyDialog::
okButton()
{
    auto* const button = m_buttonBox->button(QDialogButtonBox::Ok);
    assert(button);
    return *button;
}

void PropertyDialog::
onAccepted()
{
    m_timer->save();
    m_appearance->save();
}

void PropertyDialog::
onPropertyChanged()
{
    okButton().setEnabled(true);
}

void PropertyDialog::
connectWidgets()
{
    this->connect(m_timer.get(), SIGNAL(changed()),
                  this,            SLOT(onPropertyChanged()));
    this->connect(m_appearance.get(), SIGNAL(changed()),
                  this,                 SLOT(onPropertyChanged()));
    this->connect(this, SIGNAL(accepted()),
                  this,   SLOT(onAccepted()));
}
