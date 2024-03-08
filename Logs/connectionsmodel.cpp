#include "connectionsmodel.h"
#include <QBrush>

ConnectionsModel::ConnectionsModel(QObject *parent)
    : QSqlQueryModel{parent}
{}


QVariant ConnectionsModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::BackgroundRole) {
        int value = QSqlQueryModel::data(index.siblingAtColumn(4)).toInt();
        QColor backgroundColor;
        switch (value) {
        case 1:
            backgroundColor = QColor(200, 255, 200);
            break;
        case 2:
            backgroundColor = QColor(255, 200, 200);
            break;
        default:
            backgroundColor = QColor(255,255,255);
            break;
        }
        return QBrush(backgroundColor);
    } else if(role == Qt::DisplayRole && index.column() == 4) {
        int value = QSqlQueryModel::data(index).toInt();
        switch (value) {
        case 1:
            return tr("Подключение");
            break;
        case 2:
            return tr("Отключение");
            break;
        case 6:
            return tr("Сообщение");
            break;
        default:
            break;
        }
    } else if(role == Qt::DisplayRole && index.column() == 3){
        QString fio = QSqlQueryModel::data(index).toString();
        if(fio.isEmpty()) {
            QModelIndex idx = QSqlQueryModel::index(index.row(), 6);
            return QSqlQueryModel::data(idx,role);
        }
    }
    return QSqlQueryModel::data(index, role);
}
