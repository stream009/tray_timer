#include "action_button.hpp"

#include <cassert>

#include <QtGui/QAction>

void ActionButton::
setAction(QAction &action)
{
    if (m_action) {
        this->disconnect(this,     SIGNAL(clicked()),
                         m_action,   SLOT(trigger()));
        this->disconnect(m_action, SIGNAL(changed()),
                         this,       SLOT(onActionChanged()));
    }
    m_action = &action;
    this->connect(this,     SIGNAL(clicked()),
                  m_action,   SLOT(trigger()));
    this->connect(m_action, SIGNAL(changed()),
                  this,       SLOT(onActionChanged()));

    onActionChanged();
}

void ActionButton::
onActionChanged()
{
    assert(m_action);

    this->setCheckable(m_action->isCheckable());
    this->setAutoRepeat(m_action->autoRepeat());
    this->setChecked(m_action->isChecked());
    this->setEnabled(m_action->isEnabled());
    this->setFont(m_action->font());
    this->setIcon(m_action->icon());
    this->setShortcut(m_action->shortcut());
    this->setText(m_action->text());
    // This line causes trouble.
    //this->setVisible(m_action->isVisible());
}
