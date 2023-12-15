#include "deploydata.h"

DeployData::DeployData()
{

}

int DeployData::getTerminalID() const
{
    return terminalID;
}

void DeployData::setTerminalID(int newTerminalID)
{
    terminalID = newTerminalID;
}

const QDateTime &DeployData::getLastData() const
{
    return lastData;
}

void DeployData::setLastData(const QDateTime &newLastData)
{
    lastData = newLastData;
}

int DeployData::getDeployTime() const
{
    return deployTime;
}

void DeployData::setDeployTime(int newDeployTime)
{
    deployTime = newDeployTime;
}
