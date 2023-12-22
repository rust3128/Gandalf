// AppParameters.cpp

#include "appparameters.h"
#include "DbSettingsDialog/dbsettingsdialog.h"
#include "AppParameters/criptpass.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"
#include <QSettings>
#include <QFile>
#include <QApplication>

const QString AppParameters::CONFIG_FILE_NAME = "Gandalf.ini";          // Файл настроек
const QString AppParameters::LOG_FILE_NAME = "Gandalf.log";             // Лог файл
const QString AppParameters::KEY_SOLT = "SapForever";
const QString AppParameters::VEKTOR_KEY = "Poltava1970Rust";
const QString AppParameters::LOG_DIRECTORY = "D:/MPos3/Logs";
//Типи гкекрайії адреси підключень
const QMap<int, QString> AppParameters::TEMPLATE_HOSTNAME = {
    {0, "Avias"},
    {1, "UkrNafta"},
    {2, "Marshal"},
    {3, "Database"}
};
// Типи логів
const AppParameters::LogType AppParameters::LOG_TYPE_CONNECT = AppParameters::Connect;
const AppParameters::LogType AppParameters::LOG_TYPE_DISCONNECT = AppParameters::Disconnect;
const AppParameters::LogType AppParameters::LOG_TYPE_UPDATE_USER_PROFILE = AppParameters::UpdateUserProfile;
// Типи генерації паролів VNC
const QMap<int, QString> AppParameters::TEMPLATE_VNC_PASS = {
    {0, "UkrNafta"},
    {1, "Database"}
};
AppParameters::AppParameters() {
    setDefaultParameters(); // Ініціалізація параметрів за замовчуванням
    if(QFile(CONFIG_FILE_NAME).exists()){
        readDatabaseParametersFromIniFile(); // Зчитування параметрів підключення до бази даних
    } else {
        DbSettingsDialog *dlgSetting = new DbSettingsDialog(true);
        dlgSetting->exec();
    }
}

AppParameters& AppParameters::instance() {
    static AppParameters instance;
    return instance;
}

void AppParameters::setDefaultParameters() {
    CriptPass crP;
    // Встановити константи за замовчуванням
//    parameters["logFileName"] = LOG_FILE_NAME;
    // Додати інші параметри за замовчуванням
    parameters["minTerminalID"] = "1000";
    parameters["maxTerminalID"] = "99999";
    parameters["showKodZem"] = "1";
    parameters["templatеHostname"] = "-1";
    parameters["maxCountPC"] = "4";                             //Максимальна кількість ПК на АЗС
    parameters["typeVNC"] = "TightVNC";                         //Тип сервера на АЗС
    parameters["defaultVNCPass"] = crP.criptPass("88888888");   //Пароль VNC за замовчуванням
    parameters["useTemplatePassVNC"] = "0";                     //Використовувати шаблон для пароля (0-ні, 1-Так)

}

void AppParameters::setParameter(const QString& paramName, const QString& paramValue) {
    parameters[paramName] = paramValue;
}

QString AppParameters::getParameter(const QString& paramName) const {
    QString retValue;
    if(parameters.contains(paramName)){
        retValue = parameters.value(paramName, QString());
    } else {
        qCritical(logCritical()) << QApplication::tr("Не возможно найти параметр по ключу") << paramName;
        retValue.clear();
    }
    return retValue;
}

void AppParameters::readDatabaseParametersFromIniFile() {
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QString dbHostName = settings.value("Database/HostName").toString();
    QString dbDatabasePort = settings.value("Database/Port").toString();
    QString dbDatabaseName = settings.value("Database/DatabaseName").toString();
    QString dbUserName = settings.value("Database/UserName").toString();
    CriptPass crP;
    QString dbPassword = crP.decriptPass(settings.value("Database/Password").toString());

    // Встановити параметри підключення до бази даних
    setParameter("dbHostName", dbHostName);
    setParameter("dbDatabasePort", dbDatabasePort);
    setParameter("dbDatabaseName", dbDatabaseName);
    setParameter("dbUserName", dbUserName);
    setParameter("dbPassword", dbPassword);
}
