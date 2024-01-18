#include "numericSortProxyModel.h"

NumericSortProxyModel::NumericSortProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool NumericSortProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    bool canConvertToInt = false;
    bool canConvertToDouble = false;

    if (left.column() == 2) {
        int leftValue = leftData.toInt(&canConvertToInt);
        int rightValue = rightData.toInt(&canConvertToDouble);

        if (canConvertToInt && canConvertToDouble) {
            return leftValue < rightValue;
        }
    } else {
        canConvertToInt = leftData.canConvert(QMetaType::Int);
        canConvertToDouble = leftData.canConvert(QMetaType::Double);
    }

    if (canConvertToInt || canConvertToDouble) {
        return leftData.toDouble() < rightData.toDouble();
    }

    return QSortFilterProxyModel::lessThan(left, right);
}
