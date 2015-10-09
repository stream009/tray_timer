#include "timer_list_actions.hpp"

#include "timer_list_model.hpp"

#include <cassert>

#include <QtGui/QAction>
#include <QtGui/QItemSelectionModel>

class TimerListActions::UpAction : public QAction
{
    Q_OBJECT
public:
    UpAction(TimerListModel &model,
             QItemSelectionModel &selection)
        : QAction { nullptr }
        , m_model { model }
        , m_selection { selection }
    {
        this->setText("Up");
        this->setIcon(QIcon { ":/images/up.png" });
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private Q_SLOTS:
    void onTriggered()
    {
        const auto &selectedRows = m_selection.selectedRows();
        assert(!selectedRows.empty());

        const auto &selected = selectedRows.front();
        assert(selected.row() != 0);

        const auto &newIndex = m_model.moveUpRow(selected);
        m_selection.clear();
        m_selection.select(newIndex,
                QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }

private:
    TimerListModel &m_model;
    QItemSelectionModel &m_selection;
};

class TimerListActions::DownAction : public QAction
{
    Q_OBJECT
public:
    DownAction(TimerListModel &model,
               QItemSelectionModel &selection)
        : QAction { nullptr }
        , m_model { model }
        , m_selection { selection }
    {
        this->setText("Down");
        this->setIcon(QIcon { ":/images/down.png" });
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private Q_SLOTS:
    void onTriggered()
    {
        const auto &selectedRows = m_selection.selectedRows();
        assert(!selectedRows.empty());

        const auto &selected = selectedRows.front();
        assert(selected.row() != m_model.rowCount()-1);

        const auto &newIndex = m_model.moveDownRow(selected);
        m_selection.clear();
        m_selection.select(newIndex,
                QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }

private:
    TimerListModel &m_model;
    QItemSelectionModel &m_selection;
};

#include "timer_dialog.hpp"

#include <QtCore/QTime>

class TimerListActions::EditAction : public QAction
{
    Q_OBJECT
public:
    EditAction(TimerListModel &model,
               QItemSelectionModel &selection)
        : QAction { nullptr }
        , m_model { model }
        , m_selection { selection }
    {
        this->setText("Edit");
        this->setIcon(QIcon { ":/images/edit.png" });
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private Q_SLOTS:
    void onTriggered()
    {
        const auto &selectedRows = m_selection.selectedRows();
        assert(!selectedRows.empty());

        const auto &selected = selectedRows.front();
        const auto &row = selected.row();

        const auto &name = m_model.data(m_model.index(row, 0));
        const auto &duration =
                m_model.data(m_model.index(row, 1), Qt::EditRole).toInt();

        QTime t;
        t = t.addSecs(duration);
        TimerDialog dialog { name.toString(), t };

        dialog.setWindowTitle("Edit Timer");
        dialog.exec();
    }

private:
    TimerListModel &m_model;
    QItemSelectionModel &m_selection;
};

class TimerListActions::AddAction : public QAction
{
    Q_OBJECT
public:
    AddAction(TimerListModel &model)
        : QAction { nullptr }
        , m_model { model }
    {
        this->setText("Add");
        this->setIcon(QIcon { ":/images/edit_add.png" });
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private Q_SLOTS:
    void onTriggered()
    {
        TimerDialog dialog;
        dialog.setWindowTitle("Add Timer");
        if (dialog.exec() == QDialog::Rejected) return;

        const auto &name = dialog.name();
        const auto &duration = dialog.duration();

        m_model.addTimer(name, duration);
    }

private:
    TimerListModel &m_model;
};

class TimerListActions::RemoveAction : public QAction
{
    Q_OBJECT
public:
    RemoveAction(TimerListModel &model,
                 QItemSelectionModel &selection)
        : QAction { nullptr }
        , m_model { model }
        , m_selection { selection }
    {
        this->setText("Remove");
        this->setIcon(QIcon { ":/images/edit_remove.png" });
        this->connect(this, SIGNAL(triggered()),
                      this,   SLOT(onTriggered()));
    }

private Q_SLOTS:
    void onTriggered()
    {
        const auto &selectedRows = m_selection.selectedRows();
        assert(!selectedRows.empty());

        const auto &selected = selectedRows.front();

        const auto rv =
            m_model.removeRow(selected.row(), selected.parent());
        assert(rv);
    }

private:
    TimerListModel &m_model;
    QItemSelectionModel &m_selection;
};

#include "timer_list_actions.moc"

TimerListActions::
TimerListActions(TimerListModel &model, QItemSelectionModel &selection)
    : m_model        { model }
    , m_selection    { selection }
    , m_upAction     { new UpAction     { model, selection } }
    , m_downAction   { new DownAction   { model, selection } }
    , m_editAction   { new EditAction   { model, selection } }
    , m_addAction    { new AddAction    { model } }
    , m_removeAction { new RemoveAction { model, selection } }
{
    this->connect(&selection, SIGNAL(selectionChanged(const QItemSelection&,
                                                      const QItemSelection&)),
                  this,         SLOT(onSelectionChanged()));
    onSelectionChanged();
}

TimerListActions::~TimerListActions() = default;

QAction &TimerListActions::up() { return *m_upAction; }
QAction &TimerListActions::down() { return *m_downAction; }
QAction &TimerListActions::edit() { return *m_editAction; }
QAction &TimerListActions::add() { return *m_addAction; }
QAction &TimerListActions::remove() { return *m_removeAction; }

void TimerListActions::
onSelectionChanged()
{
    const auto &rows = m_selection.selectedRows();

    m_upAction->setEnabled(!rows.empty() && rows.front().row() != 0);
    m_downAction->setEnabled(
           !rows.empty() && rows.front().row() != m_model.rowCount()-1);
    m_editAction->setEnabled(!rows.empty());
    m_removeAction->setEnabled(!rows.empty());
}
