#include "deploysmodel.h"

DeploysModel::DeploysModel(const QVector<DeployData> vek, QObject *parent)
    : QAbstractTableModel{parent},
      depDat(vek)
{

}


int DeploysModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return depDat.size();
}

int DeploysModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant DeploysModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) { return QVariant(); }
    DeployData d = depDat[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return d.getTerminalID();                 // 1-й столбец Terminal ID
        case 1: return d.getLastData().toString("dd.MM.yyyy hh:mm:ss");                   // 2-й столбец Последние данные
 //       case 2: return QString::number(d.getDeployTime()/60)+" час. "+QString::number(d.getDeployTime()%60)+" мин.";      // 3-й столбец Время неотзвона
        case 2: return QString::number(d.getDeployTime());
        default: break;
        }
      break;
    case Qt::TextAlignmentRole:
        if(index.column() == 0 || index.column() == 2 )
            return int(Qt::AlignRight | Qt::AlignVCenter);
        break;
    default:
        break;
    }

     return QVariant();
}

QVariant DeploysModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Cоздаем заголовки столбцов модели
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case 0:
        return tr( "АЗС" );
    case 1:
        return tr( "Последние\nданные" );
    case 2:
        return tr( "Нет данных\nмин." );
    }

   return QAbstractTableModel::headerData(section, orientation, role);
}
