#ifndef CONTEXT_MENU_HPP
#define CONTEXT_MENU_HPP

#include "actions.hpp"

#include <QtGui/QMenu>

class ContextMenu : public QMenu
{
public:
    ContextMenu(Actions&);
};

#endif // CONTEXT_MENU_HPP
