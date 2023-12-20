#ifndef LOGGER_H
#define LOGGER_H

#include "Logs/logdata.h"

#include <QString>
#include <QMutex>
#include <QDateTime>
#include <QFile>

class Logger
{
public:
    Logger(const LogData &logData);
    ~Logger();

    void writeToLog();

private:
    QString generateLogFileName() const;
private:
    void writeToFile();
    void writeToDB();
private:
    QString m_logDirectory;
    QFile m_file;
    static QMutex m_mutex;
    LogData m_logData;
};

#endif // LOGGER_H
