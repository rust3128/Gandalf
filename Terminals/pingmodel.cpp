#include <QString>
#include <QDebug>
#include "pingmodel.h"


PingModel::PingModel(QObject *parent) :
    QObject(parent), running(false)
{
    ping = new QProcess(this);
    connect(ping, SIGNAL(started()), this, SLOT(verifyStatus()));
    connect(ping, SIGNAL(finished(int)), this, SLOT(readResult()));
//    ping->setProcessChannelMode(QProcess::MergedChannels);
}

PingModel::~PingModel(){
    ping->deleteLater();
}

void PingModel::verifyStatus(){
    if(ping->isReadable()){
        qDebug() << "read on ...";
        connect(ping, SIGNAL(readyRead()), this, SLOT(readResult()));
        if(ping->canReadLine()){
            qDebug() << "LINE read on ...";
        }
    }else{
        qDebug() << "not able to read ...";
    }
}

void PingModel::readResult(){
    running = false;
    emit signalSendOutPing(ping->readLine());

}

void PingModel::getHostName(QString *hName)
{
    pingHost = hName;
}

void PingModel::start_command(){
    if(ping){
        QString command = "ping";
        QStringList args;
#ifdef Q_OS_WIN
        args << "-w" <<  "3" << "-t" << *pingHost;
#else
        args << *pingHost;
#endif
//        qDebug() << "command" << command << "args" << args;
        ping->start(command, args);
        ping->waitForStarted(7000);
        running = true;
//        ping->waitForFinished(5000);
    }
}

bool PingModel::is_running(){
    return running;
}

bool PingModel::finished(){
    return ping->atEnd();
}
