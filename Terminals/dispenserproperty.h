
#ifndef DISPENSERPROPERTY_H
#define DISPENSERPROPERTY_H

#include <QString>


class DispenserProperty
{
public:
    DispenserProperty(int dispenser_id, QString portName, int chanelProt, int chanelSpeed, int netAdress);
    int dispenser_id() const;
    QString portName() const;
    int chanelProt() const;
    int chanelSpeed() const;
    int netAdress() const;

private:
    int m_dispenser_id;
    QString m_portName;
    int m_chanelProt;
    int m_chanelSpeed;
    int m_netAdress;
};

#endif // DISPENSERPROPERTY_H
