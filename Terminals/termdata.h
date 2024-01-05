#ifndef TERMDATA_H
#define TERMDATA_H

#include <QObject>

class TermData : public QObject
{
    Q_OBJECT
public:
    explicit TermData(QObject *parent = nullptr);
    int getTerminalID() const;
    void setTerminalID(int newTerminalID);

    QString getAdress() const;
    void setAdress(const QString &newAdress);

    QString getPhone() const;
    void setPhone(const QString &newPhone);

    int getOwnerSystemID() const;
    void setOwnerSystemID(int newOwnerSystemID);

    int getOwnerID() const;
    void setOwnerID(int newOwnerID);

    QString getOwnerName() const;
    void setOwnerName(const QString &newOwnerName);

    QString getNameAZS() const;
    void setNameAZS(const QString &newNameAZS);

signals:
    void terminalIDChanged();
    void adressChanged();
    void phoneChanged();
    void ownerSystemIDChanged();
    void ownerIDChanged();
    void ownerNameChanged();

    void nameAZSChanged();

private:
    int terminalID;     //Terminal ID
    QString nameAZS;
    QString adress;
    QString phone;
    int ownerSystemID;  // Код зовнішньої системи
    int ownerID;        // Код регіону
    QString ownerName;  //Назва регіону

    Q_PROPERTY(int terminalID READ getTerminalID WRITE setTerminalID NOTIFY terminalIDChanged FINAL)
    Q_PROPERTY(QString adress READ getAdress WRITE setAdress NOTIFY adressChanged FINAL)
    Q_PROPERTY(QString phone READ getPhone WRITE setPhone NOTIFY phoneChanged FINAL)
    Q_PROPERTY(int ownerSystemID READ getOwnerSystemID WRITE setOwnerSystemID NOTIFY ownerSystemIDChanged FINAL)
    Q_PROPERTY(int ownerID READ getOwnerID WRITE setOwnerID NOTIFY ownerIDChanged FINAL)
    Q_PROPERTY(QString ownerName READ getOwnerName WRITE setOwnerName NOTIFY ownerNameChanged FINAL)
    Q_PROPERTY(QString nameAZS READ getNameAZS WRITE setNameAZS NOTIFY nameAZSChanged FINAL)
};

#endif // TERMDATA_H
