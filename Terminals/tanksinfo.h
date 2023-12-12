#ifndef TANKSINFO_H
#define TANKSINFO_H

#include "Terminals//tankproperty.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class TanksInfo : public QObject
{
    Q_OBJECT
public:
    explicit TanksInfo(int termID, QSqlDatabase db, QObject *parent = nullptr);

signals:
    void queryFinished(QList<TankProperty> list);
    void finished();
public slots:
    void createTanksQuery();
private:
    int terminalID;
    QSqlDatabase dbC;
};

#endif // TANKSINFO_H
