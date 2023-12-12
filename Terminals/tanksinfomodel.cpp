
#include "tanksinfomodel.h"

TanksInfoModel::TanksInfoModel(QList<TankProperty> tanks, QObject *parent)
    : QAbstractTableModel{parent}
    , m_tanks(tanks)
{

}



int TanksInfoModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_tanks.size();
}

int TanksInfoModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 9;
}

QVariant TanksInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        const auto& data = m_tanks[index.row()];
        switch (index.column()) {
        case 0:
            return data.tankID();
            break;
        case 1:
            return data.fuelID();
            break;
        case 2:
            return data.shortName();
            break;
        case 3:
            return data.name();
            break;
        case 4:
            return data.maxValue();
            break;
        case 5:
            return data.minValue();
            break;
        case 6:
            return data.deadMax();
            break;
        case 7:
            return data.deadMin();
            break;
        case 8:
            return data.tubeAmount();
            break;
        default:
            break;
        }
    } else if (role == Qt::TextAlignmentRole) {
        return QVariant::fromValue(dataAlignment(index));
    }

    return QVariant();
}

QVariant TanksInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("№\nрезервуара");
            break;
        case 1:
            return tr("Код\nтопива");
            break;
        case 2:
            return tr("Кратко");
            break;
        case 3:
            return tr("Наименование");
            break;
        case 4:
            return tr("MAX\nрезервуара");
            break;
        case 5:
            return tr("MIN\nрезервуара");
            break;
        case 6:
            return tr("MAX\nуровнемера");
            break;
        case 7:
            return tr("MIN\nуровнемера");
            break;
        case 8:
            return tr("Объем\nтрубопрвода");
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QFlags<Qt::AlignmentFlag> TanksInfoModel::dataAlignment(const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
    case 1:
        return Qt::AlignCenter;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return Qt::AlignRight | Qt::AlignVCenter;
        break;
    default:
        break;
    }
    return Qt::AlignLeft | Qt::AlignVCenter;
}
