#ifndef LOGGINCATEGORIES_H
#define LOGGINCATEGORIES_H

#include <QLoggingCategory>
#include <QMessageBox>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)
Q_DECLARE_LOGGING_CATEGORY(logFatall)

class MyMessage {
public:
    static void showNotification(QMessageBox::Icon icon, const QString &title, const QString &text,
                                 const QString &informativeText = "", const QString &detailedText = "");
};

#endif // LOGGINCATEGORIES_H
