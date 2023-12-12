
#ifndef DISPENSERINFO_H
#define DISPENSERINFO_H

#include "Terminals//dispenserproperty.h"
#include "Terminals//punpproperty.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


class DispenserInfo : public QObject
{
    Q_OBJECT
public:
    explicit DispenserInfo(int termID, QSqlDatabase db, QObject *parent = nullptr);

signals:
    void queryFinished(QList<DispenserProperty> disp, QList<PunpProperty> pump);
    void finished();
public slots:
    void slotCreateDyspInfo();
private:
    int m_termID;
    QSqlDatabase m_db;
};

#endif // DISPENSERINFO_H
