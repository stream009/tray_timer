#ifndef TIMER_LIST_ACTIONS_HPP
#define TIMER_LIST_ACTIONS_HPP

#include <cassert>
#include <memory>

#include <QtCore/QObject>

class QAction;
class QItemSelectionModel;

class TimerListModel;

class TimerListActions : public QObject
{
    Q_OBJECT
    class UpAction;
    class DownAction;
    class EditAction;
    class AddAction;
    class RemoveAction;
public:
    TimerListActions(TimerListModel&, QItemSelectionModel&);
    ~TimerListActions();

    QAction &up();
    QAction &down();
    QAction &edit();
    QAction &add();
    QAction &remove();

private Q_SLOTS:
    void onSelectionChanged();

private:
    void validate() const
    {
        assert(m_upAction);
        assert(m_downAction);
        assert(m_editAction);
        assert(m_addAction);
        assert(m_removeAction);
    }

private:
    TimerListModel                &m_model;
    QItemSelectionModel           &m_selection;
    std::unique_ptr<UpAction>     m_upAction;
    std::unique_ptr<DownAction>   m_downAction;
    std::unique_ptr<EditAction>   m_editAction;
    std::unique_ptr<AddAction>    m_addAction;
    std::unique_ptr<RemoveAction> m_removeAction;
};

#endif // TIMER_LIST_ACTIONS_HPP
