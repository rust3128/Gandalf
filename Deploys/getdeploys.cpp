#include "getdeploys.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/AppParameters.h"
#include "AppParameters/criptpass.h"


GetDeploys::GetDeploys(int _porog, QObject *parent)
    : QObject{parent},
      m_porog(_porog)
{

}

const QVector<DeployData> &GetDeploys::getDeployList() const
{
    return deployList;
}

void GetDeploys::createListDeploys()
{
    QSqlDatabase dbth = QSqlDatabase::addDatabase("QIBASE","centrDBThread");
    CriptPass crP;
    dbth.setHostName(AppParameters::instance().getParameter("mposHostName"));
    dbth.setPort(AppParameters::instance().getParameter("mposPort").toInt());
    dbth.setDatabaseName(AppParameters::instance().getParameter("mposDB"));
    dbth.setUserName(AppParameters::instance().getParameter("mposUser"));
    dbth.setPassword(crP.decriptPass(AppParameters::instance().getParameter("mposPass")));


    if(!dbth.open()) {
        qCritical(logCritical()) << Q_FUNC_INFO << "Не могу подключится к центральной базе"
                                 << Qt::endl << dbth.lastError().text();
        emit signalError(dbth.lastError().text());
        return;
    }
    // Привязыем запрос к соединению
    QSqlQuery q = QSqlQuery(dbth);
    q.prepare("select s.terminal_id, s.doper, "
              "datediff(minute,s.doper,current_timestamp) as deploy "
              "from "
                "(select t.terminal_id, "
                   "(select first 1 doper from mgt$packets p "
                   "where p.terminal_id = t.terminal_id and p.apply = 'T' "
                   "order by p.doper desc) "
                "from terminals t "
                "where t.terminaltype = 3 and t.isactive = 'T' and t.iswork = 'T') s "
              "where "
              "(datediff(minute,s.doper,current_timestamp) > :porog) "
              "order by s.doper");
    q.bindValue(0, m_porog);

    if(!q.exec()) {
            qInfo(logInfo()) << "Errog get deploys" << q.lastError().text();
            emit finish();
    }
    DeployData dpDat;
    while(q.next()){
        dpDat.setTerminalID(q.value(0).toInt());
        dpDat.setLastData(q.value(1).toDateTime());
        dpDat.setDeployTime(q.value(2).toInt());
        deployList.append(dpDat);
    }
    emit signalSendDeployList(deployList);
    q.finish(); // заканчиваю работу запроса

    dbth.commit(); // commit
    dbth.close(); // close
    dbth = QSqlDatabase();
    QSqlDatabase::removeDatabase("centrDBThread"); ///Удаляю подключение
    emit finish();
}
