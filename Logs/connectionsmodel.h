#ifndef CONNECTIONSMODEL_H
#define CONNECTIONSMODEL_H

#include <QSqlQueryModel>

class ConnectionsModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ConnectionsModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // CONNECTIONSMODEL_H
