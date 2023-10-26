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
void readUserProfile();

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

    // Відкрити базу даних
    if (openDatabaseConnection()) {
        // З'єднання з базою даних вдало встановлено

        // Прочитати параметри з бази даних
        readParametersFromDatabase();
    } else {
        qCritical(logCritical()) << QApplication::tr("База данных приложения не открыта. Завершение работы.");
        return 1;
    }

    QString qUsername = QString::fromLocal8Bit (qgetenv ("USERNAME").constData ()).toUtf8 ();
    QSqlQuery q;
    q.prepare("EXECUTE PROCEDURE get_userid(:userName)");
    q.bindValue(":userName",qUsername);
    if(!q.exec()){
        qCritical(logCritical()) << QApplication::tr("Не удалось получиь ID  пользователя.") << q.lastError().text();
    }
    q.next();
    bool isNewUser = q.value(1).toBool();
    AppParameters::instance().setParameter("userID",q.value(0).toString());
    q.exec("commit work");
    q.finish();

    if (isNewUser) {
        UserProfileDialog dialog(AppParameters::instance().getParameter("userID").toInt());
        if (dialog.exec() == QDialog::Accepted) {
            // Обробка результатів введення додаткових даних
        }
    }
    readUserProfile();

    int lang = AppParameters::instance().getParameter("langUI").toInt();
    QTranslator appTranslator;
    QTranslator qtTranslator;
    QString langName;

    switch (lang) {
    case 1: // Українська
        langName = "uk";
        break;
    case 2: // Російська
        langName = "en";
        break;
    case 3: // Англійська
        langName = "ru";
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



    MainWindow w;
    qInfo(logInfo()) << QApplication::tr("Запуск гланого окна приложения. Язык интерфейса:") + langName.toUpper()
                     << QApplication::tr("Пользователь: ") << AppParameters::instance().getParameter("userLogin");
    w.show();
    return a.exec();
}
//Функція отримання профілю користувача
void readUserProfile(){
    QSqlQuery q;
    q.prepare("SELECT u.user_login, u.user_fio, u.mob_phone, u.sip_phone, u.email, u.lang_ui, u.user_role, u.isactive "
              "FROM users u WHERE user_id = ?");
    q.bindValue(0, AppParameters::instance().getParameter("userID").toInt());
    if(!q.exec()) {
        qCritical(logCritical()) << QApplication::tr("Не удалось получить данные пользователя") << q.lastError().text();
    }
    q.next();
    AppParameters::instance().setParameter("userLogin",q.value(0).toString());
    AppParameters::instance().setParameter("userFIO",q.value(1).toString());
    AppParameters::instance().setParameter("mobPhone",q.value(2).toString());
    AppParameters::instance().setParameter("sipPhone",q.value(3).toString());
    AppParameters::instance().setParameter("userEmail",q.value(4).toString());
    AppParameters::instance().setParameter("langUI",q.value(5).toString());
    AppParameters::instance().setParameter("userRole",q.value(6).toString());
    AppParameters::instance().setParameter("isActive",q.value(7).toString());
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

