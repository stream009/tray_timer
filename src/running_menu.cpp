#include "running_menu.hpp"

RunningMenu::
RunningMenu(Actions &actions)
{
    this->addAction(&actions.suspend());
    this->addAction(&actions.stop());
}
