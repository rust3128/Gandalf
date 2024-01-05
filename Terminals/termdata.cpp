#include "termdata.h"

TermData::TermData(QObject *parent)
    : QObject{parent}
{

}

int TermData::getTerminalID() const
{
    return terminalID;
}

void TermData::setTerminalID(int newTerminalID)
{
    if (terminalID == newTerminalID)
        return;
    terminalID = newTerminalID;
    emit terminalIDChanged();
}

QString TermData::getAdress() const
{
    return adress;
}

void TermData::setAdress(const QString &newAdress)
{
    if (adress == newAdress)
        return;
    adress = newAdress;
    emit adressChanged();
}

QString TermData::getPhone() const
{
    return phone;
}

void TermData::setPhone(const QString &newPhone)
{
    if (phone == newPhone)
        return;
    phone = newPhone;
    emit phoneChanged();
}

int TermData::getOwnerSystemID() const
{
    return ownerSystemID;
}

void TermData::setOwnerSystemID(int newOwnerSystemID)
{
    if (ownerSystemID == newOwnerSystemID)
        return;
    ownerSystemID = newOwnerSystemID;
    emit ownerSystemIDChanged();
}

int TermData::getOwnerID() const
{
    return ownerID;
}

void TermData::setOwnerID(int newOwnerID)
{
    if (ownerID == newOwnerID)
        return;
    ownerID = newOwnerID;
    emit ownerIDChanged();
}

QString TermData::getOwnerName() const
{
    return ownerName;
}

void TermData::setOwnerName(const QString &newOwnerName)
{
    if (ownerName == newOwnerName)
        return;
    ownerName = newOwnerName;
    emit ownerNameChanged();
}

QString TermData::getNameAZS() const
{
    return nameAZS;
}

void TermData::setNameAZS(const QString &newNameAZS)
{
    if (nameAZS == newNameAZS)
        return;
    nameAZS = newNameAZS;
    emit nameAZSChanged();
}

