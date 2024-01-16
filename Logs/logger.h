#ifndef LOGGER_H
#define LOGGER_H
#include <Logs/logdata.h>

#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QDataStream>

class LogData;

class Logger
{
public:
    Logger(const LogData &logData);
    ~Logger();

    void writeToLog();
    QString generateLogFileName() const;

private:
    void writeToFile(const QString &fileName);
    void writeToBinFile(const QString &fileName);
    void writeToDB();

    static QMutex m_mutex;

    QFile m_file;
    QFile m_binFile;

    QString m_logDirectory;
    LogData m_logData;
};

#endif // LOGGER_H
