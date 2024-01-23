#include "connstatusvnc.h"
#include <QTcpSocket>
#include <QThread>

ConnStatusVNC::ConnStatusVNC(QString IP, int port, int bID, QObject *parent)
    : QObject{parent},
    ipAdress(IP),
    portVNC(port),
    buttonID(bID)
{
    status.buttonID = buttonID;
    status.isConnected = false;
}

void ConnStatusVNC::getConnStatus()
{
    QTcpSocket *tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(ipAdress,portVNC);
    if(tcpSocket->waitForConnected(2000)) {
        status.isConnected = true;
    }
    tcpSocket->abort();
#ifdef QT_DEBUG
    QThread::msleep(2000*buttonID);
    status.isConnected = true;
#endif
        //Отладка все подключения доступны
    emit signalSendStatus(status);
    emit finish();
}
