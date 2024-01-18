#ifndef NUMERICSORTPROXYMODEL_H
#define NUMERICSORTPROXYMODEL_H

#include <QSortFilterProxyModel>

class NumericSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    NumericSortProxyModel(QObject *parent = nullptr);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // NUMERICSORTPROXYMODEL_H
