#ifndef CONNSTATUSVNC_H
#define CONNSTATUSVNC_H

#include <QObject>
struct ConnStatus {
    bool isConnected;
    int buttonID;
};

class ConnStatusVNC : public QObject
{
    Q_OBJECT
public:
    explicit ConnStatusVNC(QString IP, int port, int bID, QObject *parent = nullptr);

signals:
    void signalSendStatus(const ConnStatus &status);
    void finish();
public slots:
    void getConnStatus();
private:
    QString ipAdress;
    int portVNC;
    int buttonID;
    ConnStatus status;
};

#endif // CONNSTATUSVNC_H
