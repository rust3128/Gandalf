
#include "dispenserproperty.h"

DispenserProperty::DispenserProperty(int dispenser_id, QString portName, int chanelProt, int chanelSpeed, int netAdress) :
    m_dispenser_id(dispenser_id), m_portName(portName), m_chanelProt(chanelProt), m_chanelSpeed(chanelSpeed), m_netAdress(netAdress)
{

}

int DispenserProperty::dispenser_id() const
{
    return m_dispenser_id;
}

QString DispenserProperty::portName() const
{
    return m_portName;
}

int DispenserProperty::chanelProt() const
{
    return m_chanelProt;
}

int DispenserProperty::chanelSpeed() const
{
    return m_chanelSpeed;
}

int DispenserProperty::netAdress() const
{
    return m_netAdress;
}

