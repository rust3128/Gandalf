#include "Users/UsersModel.h"
#include <QMap>
#include <QBrush>


UsersModel::UsersModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db) {}

QVariant UsersModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::BackgroundRole) {
        int value = QSqlTableModel::data(index.siblingAtColumn(6)).toInt();
        QColor backgroundColor;
        switch (value) {
        case 0:
            backgroundColor = QColor(255, 200, 200); // Кольор для ролі "Адмін"
            break;
        case 1:
            backgroundColor = QColor(200, 255, 200); // Кольор для ролі "Менеджер"
            break;
        case 2:
            backgroundColor = QColor(200, 200, 255); // Кольор для ролі "Користувач"
            break;
        default:
            break;
        }
        return QBrush(backgroundColor);
    } else if (role == Qt::DisplayRole && index.column() == 6) {
        int value = QSqlTableModel::data(index).toInt();
        switch (value) {
        case 0:
            return tr("Админ");
        case 1:
            return tr("Менеджер");
        case 2:
            return tr("Пользователь");
        default:
            break;
        }
    } else if (role == Qt::DisplayRole && index.column() == 7) {
        int value = QSqlTableModel::data(index).toInt();
        switch (value) {
        case 0:
            return tr("Нет");
        case 1:
            return tr("Да");
        default:
            break;
        }
    }

    return QSqlTableModel::data(index, role);
}



