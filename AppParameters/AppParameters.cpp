// AppParameters.cpp

#include "appparameters.h"
#include <QSettings>

const QString AppParameters::CONFIG_FILE_NAME = "Gandalf.ini";          // Файл настроек
const QString AppParameters::LOG_FILE_NAME = "Gandalf.log";             // Лог файл
const QString AppParameters::KEY_SOLT = "SapForever";
const QString AppParameters::VEKTOR_KEY = "Poltava1970Rust";

AppParameters::AppParameters() {
    setDefaultParameters(); // Ініціалізація параметрів за замовчуванням
    readDatabaseParametersFromIniFile(); // Зчитування параметрів підключення до бази даних
}

AppParameters& AppParameters::instance() {
    static AppParameters instance;
    return instance;
}

void AppParameters::setDefaultParameters() {
    // Встановити константи за замовчуванням
//    parameters["logFileName"] = LOG_FILE_NAME;
    // Додати інші параметри за замовчуванням
}

void AppParameters::setParameter(const QString& paramName, const QString& paramValue) {
    parameters[paramName] = paramValue;
}

QString AppParameters::getParameter(const QString& paramName) const {
    return parameters.value(paramName, QString());
}

void AppParameters::readDatabaseParametersFromIniFile() {
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QString dbHostName = settings.value("Database/HostName").toString();
    QString dbDatabasePort = settings.value("Database/Port").toString();
    QString dbDatabaseName = settings.value("Database/DatabaseName").toString();
    QString dbUserName = settings.value("Database/UserName").toString();
    QString dbPassword = settings.value("Database/Password").toString();

    // Встановити параметри підключення до бази даних
    setParameter("dbHostName", dbHostName);
    setParameter("dbDatabasePort", dbDatabasePort);
    setParameter("dbDatabaseName", dbDatabaseName);
    setParameter("dbUserName", dbUserName);
    setParameter("dbPassword", dbPassword);
}
