#include "suspended_menu.hpp"

SuspendedMenu::
SuspendedMenu(Actions &actions)
{
    this->addAction(&actions.resume());
    this->addAction(&actions.stop());
}
