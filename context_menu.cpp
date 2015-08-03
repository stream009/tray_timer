#include "context_menu.hpp"

ContextMenu::
ContextMenu(Actions &actions)
{
    this->addAction(&actions.property());
    this->addAction(&actions.about());
    this->addSeparator();
    this->addAction(&actions.quit());
}
