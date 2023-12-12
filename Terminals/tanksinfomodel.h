
#ifndef TANKSINFOMODEL_H
#define TANKSINFOMODEL_H

#include "Terminals//tankproperty.h"

#include <QAbstractTableModel>



class TanksInfoModel : public QAbstractTableModel
{
public:
    explicit TanksInfoModel(QList<TankProperty> tanks, QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QFlags<Qt::AlignmentFlag> dataAlignment(const QModelIndex &index) const;
private:
    QList<TankProperty> m_tanks;
};

#endif // TANKSINFOMODEL_H
