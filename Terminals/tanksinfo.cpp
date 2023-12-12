#include "tanksinfo.h"
#include "LogginCategories/loggincategories.h"


#include <QSqlQuery>
#include <QSqlError>


TanksInfo::TanksInfo(int termID, QSqlDatabase db, QObject *parent)
    : QObject{parent},
      terminalID(termID),
      dbC(db)
{

}

void TanksInfo::createTanksQuery()
{
    QList<TankProperty> tankPropertyList;
    QSqlQuery q(dbC);
    q.prepare("select t.tank_id, t.fuel_id, f.shortname, f.name, t.maxvalue, t.minvalue, t.deadmax, t.deadmin, t.tubeamount from tanks t "
              "left join fuels f on f.fuel_id = t.fuel_id "
              "where t.terminal_id= :termID and t.isactive = 'T' "
              "order by t.tank_id");
    q.bindValue(0,terminalID);
    if(!q.exec()){
        qCritical(logCritical()) << tr("Не удалось получить информацию по резервуарам.") << q.lastError().text();
    } else {
        while (q.next()) {
            TankProperty prop(q.value(0).toInt(),q.value(1).toInt(),q.value(2).toString(),q.value(3).toString(),
                              q.value(4).toFloat(),q.value(5).toFloat(),q.value(6).toFloat(),q.value(7).toFloat(),q.value(8).toFloat());
            tankPropertyList.append(prop);
        }
    }
    emit queryFinished(tankPropertyList);
    emit finished();
}
