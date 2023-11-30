#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <QObject>

class ConnectionData : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionData(int ID, QObject *parent = nullptr);

    QString getHostName() const;
    void setHostName(const QString &newHostName);

    int getPort() const;
    void setPort(int newPort);

    QString getPassVNC() const;

    int getTernID() const;
    void setTernID(int newTernID);

signals:
    void ternIDChanged();

    void hostNameChanged();

    void portChanged();

private:
    int ternID;
    QString hostName;
    int port;
    QString passVNC;

    Q_PROPERTY(QString hostName READ getHostName WRITE setHostName NOTIFY hostNameChanged FINAL)
    Q_PROPERTY(int port READ getPort WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(QString passVNC READ getPassVNC CONSTANT FINAL)
    Q_PROPERTY(int ternID READ getTernID WRITE setTernID NOTIFY ternIDChanged FINAL)
};

#endif // CONNECTIONDATA_H
