#include "timer_list_model.hpp"

#include <cassert>

#include <QtCore/QModelIndex>
#include <QtCore/QTime>

TimerListModel::
TimerListModel(Settings &settings)
    : m_settings { settings }
{
    load();
    setUpSignals();
}

QModelIndex TimerListModel::
index(int row, int column,
      const QModelIndex &parent) const
{
    assert(parent == QModelIndex {});
    return this->createIndex(row, column);
}

QModelIndex TimerListModel::
parent(const QModelIndex&) const
{
    return QModelIndex {};
}

int TimerListModel::
rowCount(const QModelIndex &parent) const
{
    if (parent != QModelIndex {}) return 0;
    return m_timers.size();
}

int TimerListModel::
columnCount(const QModelIndex &parent) const
{
    assert(parent == QModelIndex {});
    return 2;
}

QVariant TimerListModel::
headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return {};
    assert(orientation == Qt::Horizontal);

    switch (section) {
    case 0:
        return "Name";
        break;
    case 1:
        return "Duration";
        break;
    default:
        assert(false);
    }
}

QVariant TimerListModel::
data(const QModelIndex &index, int role) const
{
    assert(index.column() < 2);

    const auto &timer = m_timers[index.row()];
    const auto &col = index.column();

    if (col == 0 && role == Qt::DisplayRole) {
        return timer.name;
    }
    else if (col == 1) {
        if (role == Qt::DisplayRole) {
            QTime t { 0, 0, 0 };
            t = t.addSecs(timer.duration);
            return t.toString("h'h' m'm' s's'");
        }
        else if (role == Qt::EditRole) {
            return timer.duration;
        }
    }

    return {};
}

bool TimerListModel::
removeRows(int row, int count, const QModelIndex &parent)
{
    assert(row >= 0);
    assert(count >= 0);
    assert(static_cast<size_t>(row) < m_timers.size());
    assert(static_cast<size_t>(row + count) <= m_timers.size());

    const auto begin = m_timers.begin() + row;
    const auto end = begin + count;

    this->beginRemoveRows(parent, row, row + count - 1);

    m_timers.erase(begin, end);

    this->endRemoveRows();

    return true;
}

QModelIndex TimerListModel::
moveUpRow(const QModelIndex &index)
{
    assert(!m_timers.empty());

    const auto row = index.row();
    assert(row != 0);
    this->beginRemoveRows(index.parent(), row, row);

    const auto timer = m_timers[row]; // copy
    const auto oldIt = m_timers.begin() + row;
    m_timers.erase(oldIt);

    this->endRemoveRows();

    const auto newRow = row - 1;
    assert(newRow >= 0);
    this->beginInsertRows(index.parent(), newRow, newRow);

    const auto newIt = m_timers.begin() + newRow;
    m_timers.emplace(newIt, timer);

    this->endInsertRows();

    return this->index(newRow, 0, index.parent());
}

QModelIndex TimerListModel::
moveDownRow(const QModelIndex &index)
{
    assert(!m_timers.empty());

    const auto row = index.row();
    assert(row >= 0);
    assert(static_cast<size_t>(row) != m_timers.size()-1);
    this->beginRemoveRows(index.parent(), row, row);

    const auto timer = m_timers[row]; // copy
    const auto oldIt = m_timers.begin() + row;
    m_timers.erase(oldIt);

    this->endRemoveRows();

    const auto newRow = row + 1;
    assert(newRow >= 0);
    assert(static_cast<size_t>(newRow) <= m_timers.size());
    this->beginInsertRows(index.parent(), newRow, newRow);

    const auto newIt = m_timers.begin() + newRow;
    m_timers.emplace(newIt, timer);

    this->endInsertRows();

    return this->index(newRow, 0, index.parent());
}

void TimerListModel::
addTimer(const QString &name, const QTime &duration)
{
    const auto pos = this->rowCount();

    this->beginInsertRows({}, pos, pos);

    const auto secs = duration.hour() * 60 * 60 +
                      duration.minute() * 60 + duration.second();
    m_timers.emplace(m_timers.begin() + pos, name, secs);

    this->endInsertRows();
}

void TimerListModel::
load()
{
    m_timers = m_settings.timers();
}

void TimerListModel::
save()
{
    m_settings.setTimers(m_timers);
}

void TimerListModel::
setUpSignals()
{
    this->connect(
        this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        this, SIGNAL(changed())
    );
    this->connect(
        this, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
        this, SIGNAL(changed())
    );
    this->connect(
        this, SIGNAL(rowsMoved(const QModelIndex&, int, int,
                               const QModelIndex&, int)),
        this, SIGNAL(changed())
    );
    this->connect(
        this, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
        this, SIGNAL(changed())
    );
}
