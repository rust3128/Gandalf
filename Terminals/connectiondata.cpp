#include "connectiondata.h"

ConnectionData::ConnectionData(int ID, QObject *parent)
    : QObject{parent},
    ternID(ID)
{}



QString ConnectionData::getHostName() const
{
    return hostName;
}

void ConnectionData::setHostName(const QString &newHostName)
{
    if (hostName == newHostName)
        return;
    hostName = newHostName;
    emit hostNameChanged();
}

int ConnectionData::getPort() const
{
    return port;
}

void ConnectionData::setPort(int newPort)
{
    if (port == newPort)
        return;
    port = newPort;
    emit portChanged();
}

QString ConnectionData::getPassVNC() const
{
    return passVNC;
}

int ConnectionData::getTernID() const
{
    return ternID;
}

void ConnectionData::setTernID(int newTernID)
{
    if (ternID == newTernID)
        return;
    ternID = newTernID;
    emit ternIDChanged();
}

