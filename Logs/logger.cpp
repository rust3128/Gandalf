#include "logger.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include <QTextStream>
#include <QDir>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>

QMutex Logger::m_mutex;

Logger::Logger(const LogData &logData) : m_logData(logData)
{
    m_logDirectory = AppParameters::LOG_DIRECTORY;
    QDir().mkpath(m_logDirectory);

    // Генеруємо ім'я файлу для текстового формату
    QString logFileName = generateLogFileName() + ".log";
    m_file.setFileName(logFileName);

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qCritical(logCritical()) << QApplication::tr("Невозможно открыть файл логирования");
    }

    // Генеруємо ім'я файлу для бінарного формату
    QString binFileName = generateLogFileName() + ".gnd";
    m_binFile.setFileName(binFileName);

    if (!m_binFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qCritical(logCritical()) << QApplication::tr("Невозможно открыть бинарный файл логирования");
    }
}

Logger::~Logger()
{
    if (m_file.isOpen())
    {
        m_file.close();
    }

    if (m_binFile.isOpen())
    {
        m_binFile.close();
    }
}

void Logger::writeToLog()
{
    QString logFileName = generateLogFileName();

    writeToFile(logFileName + ".log");
    writeToBinFile(logFileName + ".gnd");
    writeToDB();
}

QString Logger::generateLogFileName() const
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return m_logDirectory + "/" + currentDateTime.toString("yyyyMM");
}

void Logger::writeToFile(const QString &fileName)
{
    QMutexLocker locker(&m_mutex);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qCritical(logCritical()) << QApplication::tr("Невозможно открыть файл логирования");
        return;
    }

    QTextStream stream(&file);

    QStringList message;
    message << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    message << QString::number(m_logData.userID());
    message << QString::number(m_logData.terminalID());
    message << QString::number(m_logData.posID());
    message << QString::number(m_logData.logTypeID());
    message << m_logData.comments();

    stream << message.join("|") << "\n";

    file.close();
}

void Logger::writeToBinFile(const QString &fileName)
{
    QMutexLocker locker(&m_mutex);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qCritical(logCritical()) << QApplication::tr("Невозможно открыть файл логирования");
        return;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << QDateTime::currentDateTime();
    stream << m_logData.userID();
    stream << m_logData.terminalID();
    stream << m_logData.posID();
    stream << m_logData.logTypeID();
    stream << m_logData.comments();

    file.flush();
    file.close();
}

void Logger::writeToDB()
{
    QSqlQuery q;
    QString strSQL = QString("INSERT INTO LOGS (DAT, USER_ID, TERMINAL_ID, POS_ID, LOGTYPE_ID, COMMENTS) "
                             "VALUES (CURRENT_TIMESTAMP, %1, %2, %3, %4, '%5')")
                         .arg(m_logData.userID())
                         .arg(m_logData.terminalID())
                         .arg(m_logData.posID())
                         .arg(m_logData.logTypeID())
                         .arg(m_logData.comments());

    if (!q.exec(strSQL))
    {
        qCritical(logCritical()) << QApplication::tr("Не удалось записать лог в базу данных.") << q.lastError().text();
    }
    q.finish();
}
