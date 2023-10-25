#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getUserID();

 }

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    // Викликаємо показ діалогу після показу головного вікна
    QTimer::singleShot(0, this, [this](){
        if (isNewUser) {
            UserProfileDialog dialog(AppParameters::instance().getParameter("userID").toInt(), this);
            if (dialog.exec() == QDialog::Accepted) {
                // Обробка результатів введення додаткових даних
            }
        }
    });
    updateUserData();
    createUI();
}

void MainWindow::getUserID()
{
    QString qUsername = QString::fromLocal8Bit (qgetenv ("USERNAME").constData ()).toUtf8 ();
    QSqlQuery q;
    q.prepare("EXECUTE PROCEDURE get_userid(:userName)");
    q.bindValue(":userName",qUsername);
    if(!q.exec()){
        qCritical(logCritical()) << tr("Не удалось получиь ID  пользователя.") << q.lastError().text();
    }
    q.next();
    isNewUser = q.value(1).toBool();
    AppParameters::instance().setParameter("userID",q.value(0).toString());
    q.exec("commit work");
}

void MainWindow::updateUserData()
{
    QSqlQuery q;
    q.prepare("SELECT u.user_login, u.user_fio, u.mob_phone, u.sip_phone, u.email, u.lang_ui, u.user_role, u.isactive "
              "FROM users u WHERE user_id = ?");
    q.bindValue(0, AppParameters::instance().getParameter("userID").toInt());
    if(!q.exec()) {
        qCritical(logCritical()) << tr("Не удалось получить данные пользователя") << q.lastError().text();
    }
    q.next();
    userRole = q.value(6).toInt();
    AppParameters::instance().setParameter("userLogin",q.value(0).toString());
    AppParameters::instance().setParameter("userFIO",q.value(1).toString());
    AppParameters::instance().setParameter("userRole", QString::number(userRole));
    AppParameters::instance().setParameter("langUI", q.value(5).toString());
}

void MainWindow::createUI()
{
    qInfo(logInfo()) << AppParameters::instance().getParameter("userLogin") << AppParameters::instance().getParameter("userFIO");
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    QLabel *labelUserName = new QLabel();
    labelFlag = new QLabel();
    labelUserName->setText(AppParameters::instance().getParameter("userLogin")+", "+AppParameters::instance().getParameter("userFIO"));
    int langUI = AppParameters::instance().getParameter("langUI").toInt();
    switch (langUI) {
    case 1:
        labelFlag->setPixmap(QPixmap(":/Images/flag_ua_ukraine.png"));
        break;
    case 2:
        labelFlag->setPixmap(QPixmap(":/Images/flag_usa.png"));
        break;
    case 3:
        labelFlag->setPixmap(QPixmap(":/Images/flag_ru.png"));
        break;
    default:
        break;
    }
    labelFlag->setToolTip(tr("Язык интерефейса"));
    int statusBarHeight = ui->statusbar->height();
    labelFlag->setFixedSize(statusBarHeight*1.2, statusBarHeight*0.8);
    labelFlag->setScaledContents(true);

    ui->statusbar->addWidget(labelUserName);
    ui->statusbar->addPermanentWidget(labelFlag);

    // Створення контекстного меню для labelFlag
    QMenu *contextMenu = new QMenu(labelFlag);

    // Створення дій для зміни мови
    QAction *actionUkrainian = new QAction(QIcon(":/Images/flag_ua_ukraine.png"), "Українська");
    QAction *actionEnglish = new QAction(QIcon(":/Images/flag_usa.png"), "English");
    QAction *actionRussian = new QAction(QIcon(":/Images/flag_ru.png"), "Русский");

    // Підключення слотів до дій для обробки їхніх натискань
    connect(actionUkrainian, &QAction::triggered, this, &MainWindow::changeLanguageToUkrainian);
    connect(actionEnglish, &QAction::triggered, this, &MainWindow::changeLanguageToEnglish);
    connect(actionRussian, &QAction::triggered, this, &MainWindow::changeLanguageToRussian);

    // Додавання дій до контекстного меню
    contextMenu->addAction(actionUkrainian);
    contextMenu->addAction(actionEnglish);
    contextMenu->addAction(actionRussian);

    // Зазначення обраної мови
    if (langUI == 1) {
        actionUkrainian->setCheckable(true);
        actionUkrainian->setChecked(true);
    } else if (langUI == 2) {
        actionEnglish->setCheckable(true);
        actionEnglish->setChecked(true);
    } else if (langUI == 3) {
        actionRussian->setCheckable(true);
        actionRussian->setChecked(true);
    }

    // Встановлення контекстного меню для labelFlag
    labelFlag->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(labelFlag, &QLabel::customContextMenuRequested, [contextMenu, this](const QPoint &pos) {
        contextMenu->exec(labelFlag->mapToGlobal(pos));
    });
}

void MainWindow::changeLanguageToUkrainian() {
    if (appTranslator.load(":/Gandalf_uk.qm") && qtTranslator.load(":/qtbase_uk.qm")) {
        qApp->installTranslator(&appTranslator);
        qApp->installTranslator(&qtTranslator);
    }
    changeLangUI(1);
}

void MainWindow::changeLanguageToEnglish() {
    if (appTranslator.load(":/Gandalf_en.qm") && qtTranslator.load(":/qtbase_en.qm")) {
        qApp->installTranslator(&appTranslator);
        qApp->installTranslator(&qtTranslator);
    }
    changeLangUI(2);
}

void MainWindow::changeLanguageToRussian() {
    if (appTranslator.load(":/Gandalf_ru.qm") && qtTranslator.load(":/qtbase_ru.qm")) {
        qApp->installTranslator(&appTranslator);
        qApp->installTranslator(&qtTranslator);
    }
    changeLangUI(3);
}


void MainWindow::changeLangUI(int langCode)
{
    labelFlag->clear(); // Очищаємо попередню іконку
    switch (langCode) {
    case 1:
        labelFlag->setPixmap(QPixmap(":/Images/flag_ua_ukraine.png"));
        break;
    case 2:
        labelFlag->setPixmap(QPixmap(":/Images/flag_usa.png"));
        break;
    case 3:
        labelFlag->setPixmap(QPixmap(":/Images/flag_ru.png"));
        break;
    default:
        break;
    }
    AppParameters::instance().setParameter("langUI", QString::number(langCode));
    QSqlQuery q;
    q.prepare("UPDATE users SET lang_ui = ? WHERE user_id = ?");
    q.bindValue(0, langCode);
    q.bindValue(1, AppParameters::instance().getParameter("userID").toInt());
    if(!q.exec()){
        qCritical(logCritical()) << tr("Не удалосьобновить код языка интерфейса") << q.lastError().text();
    }
    int statusBarHeight = ui->statusbar->height();
    labelFlag->setFixedSize(statusBarHeight*1.2, statusBarHeight*0.8);
    labelFlag->setScaledContents(true);

}
//    AppParameters::instance().setParameter("paramA", paramA);
