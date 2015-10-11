#include "timer_list_widget.hpp"

#include "action_button.hpp"
#include "settings.hpp"
#include "timer_list_actions.hpp"
#include "timer_list_model.hpp"

#include <cassert>

#include <QtGui/QBoxLayout>
#include <QtGui/QDialog>
#include <QtGui/QTreeView>

TimerListWidget::
TimerListWidget(Settings &settings)
    : m_model { new TimerListModel { settings } }
{
    assert(m_model);
    auto* const layout = new QHBoxLayout { this };
    assert(layout);
    assert(layout->parent() == this);

    m_listView.reset(new QTreeView);
    assert(m_listView);
    m_listView->setRootIsDecorated(false);
    m_listView->setModel(m_model.get());
    layout->addWidget(m_listView.get());

    auto* const selection = m_listView->selectionModel();
    assert(selection);

    m_actions.reset(new TimerListActions { *m_model, *selection });
    assert(m_actions);

    auto &&buttonLayout = createButtonPanel();

    layout->addLayout(&buttonLayout);
    assert(buttonLayout.parent() == layout);

    this->connect(m_model.get(), SIGNAL(changed()),
                  this,          SIGNAL(changed()));
}

TimerListWidget::~TimerListWidget() = default;

void TimerListWidget::
save()
{
    m_model->save();
}

QLayout &TimerListWidget::
createButtonPanel()
{
    auto* const buttonLayout = new QVBoxLayout {};
    assert(buttonLayout);
    buttonLayout->addStretch();

    m_upButton.reset(new ActionButton);
    assert(m_upButton);
    m_upButton->setAction(m_actions->up());
    buttonLayout->addWidget(m_upButton.get());

    m_addButton.reset(new ActionButton);
    assert(m_addButton);
    m_addButton->setAction(m_actions->add());
    buttonLayout->addWidget(m_addButton.get());

    m_editButton.reset(new ActionButton);
    assert(m_editButton);
    m_editButton->setAction(m_actions->edit());
    buttonLayout->addWidget(m_editButton.get());

    m_removeButton.reset(new ActionButton);
    assert(m_removeButton);
    m_removeButton->setAction(m_actions->remove());
    buttonLayout->addWidget(m_removeButton.get());

    m_downButton.reset(new ActionButton);
    assert(m_downButton);
    m_downButton->setAction(m_actions->down());
    buttonLayout->addWidget(m_downButton.get());

    buttonLayout->addStretch();

    return *buttonLayout;
}
