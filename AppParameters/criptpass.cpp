#include "criptpass.h"
#include "AppParameters/AppParameters.h"
#include "AppParameters/qaesencryption.h"

#include <QCryptographicHash>


CriptPass::CriptPass()
{
    key = AppParameters::KEY_SOLT;
    iv = AppParameters::VEKTOR_KEY;
    hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
}

QString CriptPass::criptPass(QString password)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray encodeText = encryption.encode(password.toLocal8Bit(), hashKey, hashIV);
    auto result = QString(encodeText.toBase64());
    return result;
}

QString CriptPass::decriptPass(QString password)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray encodeText = QByteArray::fromBase64(password.toUtf8());
    QByteArray decodeText = encryption.decode(encodeText, hashKey, hashIV);
    auto result = QString(encryption.removePadding(decodeText));
    return result;
}

QString CriptPass::cryptVNCPass(QString termID, QString pass)
{
    int desiredLength = 5;
    while (termID.length() < desiredLength) {
        termID.prepend('0');
    }
    QString password = termID.right(3)+pass+termID.left(2);
    password = criptPass(password);
    return password;
}

QString CriptPass::decryptVNCPass(QString pass)
{
    QString password = decriptPass(pass);
    password = password.mid(3, password.length()-5);
    return password;
}


