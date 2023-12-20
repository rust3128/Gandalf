#include "logdata.h"

LogData::LogData(int userID, int terminalID, int posID, int logTypeID, QString comments) :
    m_userID(userID),
    m_terminalID(terminalID),
    m_posID(posID),
    m_logTypeID(logTypeID),
    m_comments(comments)
{

}

int LogData::userID() const
{
    return m_userID;
}

int LogData::terminalID() const
{
    return m_terminalID;
}

int LogData::logTypeID() const
{
    return m_logTypeID;
}

QString LogData::comments() const
{
    return m_comments;
}

int LogData::posID() const
{
    return m_posID;
}

void LogData::setLogTypeID(int newLogTypeID)
{
    m_logTypeID = newLogTypeID;
}




