#ifndef LOGDATA_H
#define LOGDATA_H

#include <QString>

class LogData
{
public:
    LogData(int userID, int terminalID, int posID, int logTypeID, QString comments);
    int userID() const;
    int terminalID() const;
    int logTypeID() const;
    QString comments() const;

    int posID() const;


    void setLogTypeID(int newLogTypeID);

private:
    int m_userID;
    int m_terminalID;
    int m_posID;
    int m_logTypeID;
    QString m_comments;
};

#endif // LOGDATA_H
