#ifndef TIMER_LIST_WIDGET_HPP
#define TIMER_LIST_WIDGET_HPP

#include <cassert>
#include <memory>

#include <QtGui/QWidget>

class QTreeView;

class ActionButton;
class PropertyDialog;
class Settings;
class TimerListActions;
class TimerListModel;

class TimerListWidget : public QWidget
{
public:
    TimerListWidget(PropertyDialog&, Settings&);
    ~TimerListWidget();

private:
    QLayout &createButtonPanel();

    void validate()
    {
        assert(m_model);
        assert(m_actions);
        assert(m_listView);
        assert(m_upButton);
        assert(m_downButton);
        assert(m_editButton);
        assert(m_addButton);
        assert(m_removeButton);
    }

private:
    std::unique_ptr<TimerListModel>   m_model;
    std::unique_ptr<TimerListActions> m_actions;
    std::unique_ptr<QTreeView>        m_listView;
    std::unique_ptr<ActionButton>     m_upButton;
    std::unique_ptr<ActionButton>     m_downButton;
    std::unique_ptr<ActionButton>     m_editButton;
    std::unique_ptr<ActionButton>     m_addButton;
    std::unique_ptr<ActionButton>     m_removeButton;
};

#endif // TIMER_LIST_WIDGET_HPP
