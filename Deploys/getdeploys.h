#ifndef GETDEPLOYS_H
#define GETDEPLOYS_H

#include "Deploys/deploydata.h"
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>


class GetDeploys : public QObject
{
    Q_OBJECT
public:
    explicit GetDeploys(int _porog,  QObject *parent = nullptr);

    const QVector<DeployData> &getDeployList() const;

signals:
    // Сигнал дя отправки данных в основной поток для дальнейшей их обработки
    void signalSendDeployList(QVector<DeployData>);
    // Сигнал об ошибке подключения к БД
    void signalError(QString);
    // Сигнал об окончании выборки данных
    void finish();
public slots:
    // Слот для начала выбоки данных, вызываемый из основного потока
    void createListDeploys();
private:
    int m_porog;
    QVector<DeployData> deployList;
    QStringList m_connData;
    Q_PROPERTY(QVector<DeployData> deployList READ getDeployList CONSTANT)

};

#endif // GETDEPLOYS_H
