#ifndef SUSPENDED_MENU_HPP
#define SUSPENDED_MENU_HPP

#include "actions.hpp"

#include <QtGui/QMenu>

class SuspendedMenu : public QMenu
{
public:
    SuspendedMenu(Actions&);
};

#endif // SUSPENDED_MENU_HPP
