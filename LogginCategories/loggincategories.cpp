#include "loggincategories.h"

Q_LOGGING_CATEGORY(logDebug,    "Debug")
Q_LOGGING_CATEGORY(logInfo,     "Info")
Q_LOGGING_CATEGORY(logWarning,  "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")
Q_LOGGING_CATEGORY(logFatal,    "Fatal")

void MyMessage::showNotification(QMessageBox::Icon icon, const QString &title, const QString &text,
                                 const QString &informativeText, const QString &detailedText) {
    QMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setDetailedText(detailedText);
    msgBox.setWindowTitle(title);
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

// Використання MyMessage::showNotification
//MyMessage::showNotification(QMessageBox::Information, "Заголовок", "Текст повідомлення", "Інформація", "Деталі");
