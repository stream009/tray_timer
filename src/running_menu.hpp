#ifndef RUNNING_MENU_HPP
#define RUNNING_MENU_HPP

#include "actions.hpp"

#include <QtGui/QMenu>

class RunningMenu : public QMenu
{
public:
    RunningMenu(Actions&);
};

#endif // RUNNING_MENU_HPP
