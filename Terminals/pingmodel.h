#ifndef PINGMODEL_H
#define PINGMODEL_H
#include <QObject>
#include <QProcess>

class PingModel : public QObject
{
    Q_OBJECT
public:
    explicit PingModel(QObject *parent = nullptr);
    ~PingModel();

    void start_command();
    bool is_running();
    bool finished();
    
signals:
    void signalSendOutPing(QByteArray);
public slots:
    void verifyStatus();
    void readResult();
    void getHostName(QString *hName);
private:
    QProcess *ping;
    bool running;
    QString *pingHost;

};

#endif // PINGMODEL_H
