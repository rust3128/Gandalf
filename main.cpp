#include "mainwindow.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/AppParameters.h"
#include "DbSettingsDialog/dbsettingsdialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


static QScopedPointer<QFile>   m_logFile;
bool openDatabaseConnection();
void readParametersFromDatabase();

// Объявление обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем файл логирования
    m_logFile.reset(new QFile(AppParameters::LOG_FILE_NAME));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);

    int lang = 1;
    QTranslator appTranslator;
    QTranslator qtTranslator;
    QString langName;

    switch (lang) {
    case 1: // Українська
        langName = "uk";
        break;
    case 2: // Російська
        langName = "ru";
        break;
    case 3: // Англійська
        langName = "en";
        break;
    default: // За замовчуванням (якщо lang не відповідає жодному з варіантів)
        langName = "en";
    }

    if (appTranslator.load(":/Gandalf_" + langName + ".qm")) {
        a.installTranslator(&appTranslator);
    } else {
        // Обробка помилки завантаження перекладу додатку
        qDebug() << "Помилка завантаження перекладу для додатку.";
    }

    if (qtTranslator.load(":/Translations/qtbase_" + langName +".qm")) {
        a.installTranslator(&qtTranslator);
    } else {
        // Обробка помилки завантаження перекладу Qt
        qDebug() << "Помилка завантаження перекладу для Qt.";
    }

    // Відкрити базу даних
    if (openDatabaseConnection()) {
        // З'єднання з базою даних вдало встановлено

        // Прочитати параметри з бази даних
        readParametersFromDatabase();
    } else {
        qCritical(logCritical()) << QApplication::tr("База данных приложения не открыта. Завершение работы.");
        return 1;
    }

    MainWindow w;
    qInfo(logInfo()) << QApplication::tr("Запуск гланого окна приложения. Язык интерфейса:") + langName.toUpper();
    w.show();
    return a.exec();
}

// Функція для відкриття бази даних
bool openDatabaseConnection() {
    // Встановити з'єднання з базою даних, використовуючи параметри з AppParameters
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
    db.setHostName(AppParameters::instance().getParameter("dbHostName"));
    db.setPort(AppParameters::instance().getParameter("dbDatabasePort").toInt());
    db.setDatabaseName(AppParameters::instance().getParameter("dbDatabaseName"));
    db.setUserName(AppParameters::instance().getParameter("dbUserName"));
    db.setPassword(AppParameters::instance().getParameter("dbPassword"));

    if(db.open()){
        return true;
    } else {
        qCritical(logCritical()) << QApplication::tr("Ошибка открытия базы данных:") << db.lastError().text();
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QApplication::tr("Ошибка подключения к базе данных!"));
        msgBox.setInformativeText(QApplication::tr("Проверить настройки подключения?"));
        msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
        msgBox.setDetailedText(db.lastError().text());
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes){
            DbSettingsDialog *dlgSetting = new DbSettingsDialog(false);
            dlgSetting->exec();
        }
        return false;
    }
}

// Функція для читання параметрів з бази даних
void readParametersFromDatabase() {
    // Виконати запити SQL для отримання параметрів
    QSqlQuery query;
    query.exec("SELECT param_name, param_value FROM your_parameters_table");

    // Отримати та встановити параметри в AppParameters
    while (query.next()) {
        QString paramName = query.value(0).toString();
        QString paramValue = query.value(1).toString();
        AppParameters::instance().setParameter(paramName, paramValue);
    }
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:
        out << "[INF] ";
        console << "Info:     " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtDebugMsg:
        out << "[DBG] " ;
        console << "Debug:    " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtWarningMsg:
        out << "[WRN] ";
        console << "Warning:  " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] "  << Qt::endl;
        break;
    case QtCriticalMsg:
        out << "[CRT] ";
        console << "Critical: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtFatalMsg:
        out << "[FTL] ";
        console << "Fatality: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }

    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg;
    out << "  ***[" << context.function << " File: " << context.file << " Line: " << context.line << "]***" << Qt::endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}

