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

    // Додати метод для встановлення значень за замовчуванням
    void setDefaultParameters();

    // Додати метод для зчитування параметрів підключення до бази даних з INI-файлу
    void readDatabaseParametersFromIniFile();

private:
    QMap<QString, QString> parameters;
    AppParameters();
};

#endif // APPPARAMETERS_H
