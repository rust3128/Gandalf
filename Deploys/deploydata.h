#ifndef DEPLOYDATA_H
#define DEPLOYDATA_H

#include <QDateTime>

class DeployData
{
public:
    DeployData();

    int getTerminalID() const;
    void setTerminalID(int newTerminalID);

    const QDateTime &getLastData() const;
    void setLastData(const QDateTime &newLastData);

    int getDeployTime() const;
    void setDeployTime(int newDeployTime);

private:
    int terminalID;
    QDateTime lastData;
    int deployTime;
};

#endif // DEPLOYDATA_H
