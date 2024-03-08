// AppParameters.h

#ifndef APPPARAMETERS_H
#define APPPARAMETERS_H

#include <QString>
#include <QMap>

class AppParameters {
public:
    static AppParameters& instance();
    void setParameter(const QString& paramName, const QString& paramValue);
    QString getParameter(const QString& paramName) const;

    // Додати нові константи
    static const QString CONFIG_FILE_NAME;
    static const QString LOG_FILE_NAME;
    static const QString KEY_SOLT;
    static const QString VEKTOR_KEY;
    static const QMap<int, QString> TEMPLATE_HOSTNAME;
    static const QString LOG_DIRECTORY;
    //Типи логів
    enum LogType {
        Connect = 1,
        Disconnect,
        UpdateUserProfile,
        ChangeVNCClient,
        ChangePassVNC,
        SendMessage
        // Додайте інші типи логу, якщо потрібно
    };
    static const LogType LOG_TYPE_CONNECT;
    static const LogType LOG_TYPE_DISCONNECT;
    static const LogType LOG_TYPE_UPDATE_USER_PROFILE;
    static const LogType LOG_TYPE_CHANGE_VNC_CLIENT;
    static const LogType LOG_TYPE_CHANGE_VNC_PASS;
    static const LogType LOG_TYPE_SEND_MESSAGE;

    static const QMap<int, QString> TEMPLATE_VNC_PASS;

    //VNC клієнти
    enum clientVNC {
        TightVNC = 1,
        UltraVNC
    };
    static const clientVNC TIGHT_CLIENT_VNC;
    static const clientVNC ULTRA_CLIENT_VNC;
    // Додати метод для встановлення значень за замовчуванням
    void setDefaultParameters();

    // Додати метод для зчитування параметрів підключення до бази даних з INI-файлу
    void readDatabaseParametersFromIniFile();

private:
    QMap<QString, QString> parameters;
    AppParameters();
};

#endif // APPPARAMETERS_H
