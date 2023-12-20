#include "logger.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include <QTextStream>
#include <QDir>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>


QMutex Logger::m_mutex;

Logger::Logger(const LogData &logData) :  m_logData(logData)
{
    m_logDirectory = AppParameters::LOG_DIRECTORY;
    // Створюємо директорію, якщо вона ще не існує
    QDir().mkpath(m_logDirectory);

    // Генеруємо ім'я файлу при створенні об'єкта Logger
    m_file.setFileName(generateLogFileName());

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qCritical(logCritical()) << QApplication::tr("Невозможно открыть файл логирования");
    }
}

Logger::~Logger()
{
    if (m_file.isOpen())
    {
        m_file.close();
    }
}

void Logger::writeToLog()
{
    writeToFile();
    writeToDB();
}

QString Logger::generateLogFileName() const
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return m_logDirectory + "/" + currentDateTime.toString("yyyyMM") + ".log";
}

void Logger::writeToFile()
{
    QMutexLocker locker(&m_mutex);

    // Перевірка, чи місяць змінився, і створення нового файлу
    QString currentFileName = generateLogFileName();
    if (currentFileName != m_file.fileName())
    {
        m_file.close();
        m_file.setFileName(currentFileName);

        if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            qCritical(logCritical()) << QApplication::tr("Невозможно открыть файл логирования");
            return;
        }
    }

    if (m_file.isOpen())
    {
        QStringList message;
        message << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
        message << QString::number(m_logData.userID());
        message << QString::number(m_logData.terminalID());
        message << QString::number(m_logData.posID());
        message << QString::number(m_logData.logTypeID());
        message << m_logData.comments();
        QTextStream stream(&m_file);
        stream << message.join("|") << "\n";
    }
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
 //   qInfo(logInfo()) << "Str SQL" << strSQL;
    if(!q.exec(strSQL)){
        qCritical(logCritical()) << QApplication::tr("Не удалось записать лог в базу данных.") << q.lastError().text();
    }
    q.finish();
}
