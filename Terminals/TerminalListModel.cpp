// Реалізуйте вашу модель у файлі, наприклад, TerminalListModel.cpp

#include "TerminalListModel.h"

TerminalListModel::TerminalListModel(const QMap<int, QSharedPointer<TermData>>& terminalMap, QObject *parent)
    : QAbstractTableModel(parent), terminalMap(terminalMap)
{
}

int TerminalListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return terminalMap.size();
}

int TerminalListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // Припустимо, що в вашому класі TermData є 3 поля для відображення у різних колонках
    return 2;
}

QVariant TerminalListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (row >= 0 && row < terminalMap.size()) {
        const QSharedPointer<TermData>& termData = terminalMap.values().at(row);

        // Виберіть, які дані ви хочете відображати в кожній колонці
        if (col == 0 && role == Qt::DisplayRole) {
            return termData->getTerminalID();
        } else if (col == 1 && role == Qt::DisplayRole) {
            return termData->getNameAZS();
        }
    }

    return QVariant();
}
