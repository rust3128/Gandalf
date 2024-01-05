#ifndef CRIPTPASS_H
#define CRIPTPASS_H

#include <QString>

class CriptPass
{
public:
    CriptPass();
    QString criptPass(QString password);
    QString decriptPass(QString password);
 private:
    QString key;
    QString iv;
    QByteArray hashKey;
    QByteArray hashIV;
};

#endif // CRIPTPASS_H
