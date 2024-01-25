// Оголосіть вашу модель у файлі, наприклад, TerminalListModel.h

#ifndef TERMINALLISTMODEL_H
#define TERMINALLISTMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QSharedPointer>
#include "Terminals/termdata.h"

class TerminalListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TerminalListModel(const QMap<int, QSharedPointer<TermData>>& terminalMap, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QMap<int, QSharedPointer<TermData>> terminalMap;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // TERMINALLISTMODEL_H
