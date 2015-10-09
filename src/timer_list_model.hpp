#ifndef TIMER_LIST_MODEL_HPP
#define TIMER_LIST_MODEL_HPP

#include <QtCore/QAbstractItemModel>

#include "settings.hpp"

class QModelIndex;
class QTime;

class TimerListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TimerListModel(Settings &settings);

    // @override QAbstractItemModel
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex {}) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int rowCount(const QModelIndex &parent = QModelIndex {}) const override;
    int columnCount(const QModelIndex &parent = QModelIndex {}) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex {}) override;

    // modifier
    QModelIndex moveUpRow(const QModelIndex &index);
    QModelIndex moveDownRow(const QModelIndex &index);
    void addTimer(const QString &name, const QTime &duration);

Q_SIGNALS:
    void changed();

public Q_SLOTS:
    void save();

private:
    void load();
    void setUpSignals();

private:
    Settings &m_settings;
    std::vector<Settings::Timer> m_timers;
};

#endif // TIMER_LIST_MODEL_HPP
