#ifndef DEPLOYSMODEL_H
#define DEPLOYSMODEL_H

#include "Deploys/deploydata.h"

#include <QAbstractTableModel>
#include <QObject>

class DeploysModel : public QAbstractTableModel
{
    Q_OBJECT
    QVector<DeployData> depDat;
public:
    explicit DeploysModel(const QVector<DeployData> vek, QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // DEPLOYSMODEL_H
