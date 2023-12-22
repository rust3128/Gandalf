#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QSqlTableModel>

class UsersModel : public QSqlTableModel {
    Q_OBJECT

public:
    UsersModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // USERSMODEL_H
