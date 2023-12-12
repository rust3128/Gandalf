#include "LogginCategories/loggincategories.h"
#include "dispenserinfo.h"

DispenserInfo::DispenserInfo(int termID, QSqlDatabase db, QObject *parent)
    : QObject{parent} ,
    m_termID(termID) ,
    m_db(db)
{

}

void DispenserInfo::slotCreateDyspInfo()
{
    QList<DispenserProperty> disp;
    QList<PunpProperty> pump;

    QSqlQuery q(m_db);
    q.prepare("SELECT d.dispenser_id, p.name, d.channelport, d.channelspeed, d.netaddress from dispensers d "
              "LEFT join protocols p ON p.protocol_id = d.protocol_id "
              "where d.terminal_id = ? and d.isactive = 'T' and "
              "p.postype_id = (SELECT s.postype_id from POSS s WHERE s.terminal_id = ? AND s.pos_id =1)");
    q.bindValue(0, m_termID);
    q.bindValue(1, m_termID);
    if(!q.exec()) {
         qCritical(logCritical()) << tr("Не удалось получить информацию по ТРК.") << q.lastError().text();
    } else {
         while (q.next()) {
             DispenserProperty prop(q.value(0).toInt(),q.value(1).toString(),q.value(2).toInt(),q.value(3).toInt(),q.value(4).toInt());
             disp.append(prop);
         }
    }
    q.clear();
    QSqlQuery qP(m_db);
    qP.prepare("select t.dispenser_id, t.trk_id as punp_id, t.tank_id, f.shortname from trks t "
              "left join tanks s on s.tank_id = t.tank_id "
              "left join fuels f on f.fuel_id = s.fuel_id "
              "where t.terminal_id= :term1 AND s.terminal_id = :term2 and t.isactive = 'T' "
              "order by t.dispenser_id, t.trk_id");
    qP.bindValue(0, m_termID);
    qP.bindValue(1, m_termID);
    if(!qP.exec()) {
         qCritical(logCritical()) << tr("Не удалось получить информацию по пистолетам.") << qP.lastError().text();
    } else {
         while (qP.next()) {
             PunpProperty prop(qP.value(0).toInt(),qP.value(1).toInt(),qP.value(2).toInt(),qP.value(3).toString());
             pump.append(prop);

         }
    }
    emit queryFinished(disp,pump);
    emit finished();
}

