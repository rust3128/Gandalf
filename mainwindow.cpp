#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "Users/userprofiledialog.h"
#include "AppParameters/parametersdialog.h"
#include "AppParameters/criptpass.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!openMposDB()) {
        on_actionParametrs_triggered();
    }
    getListAZS();
    createUI();
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    QString strTitle = tr("Маг не приходит поздно, %1, и рано тоже не приходит. Он появляется тогда, когда положено.")
                           .arg((AppParameters::instance().getParameter("userFIO").isEmpty()) ? tr("Неизвестный") : AppParameters::instance().getParameter("userFIO"));
    this->setWindowTitle("Gamdalf "+strTitle);
//    QLabel *labelStatus =new QLabel(strTitle);
//    ui->statusbar->addWidget(labelStatus);
}

void MainWindow::getListAZS()
{
    QSqlDatabase db = QSqlDatabase::database("mpos");
    QSqlQuery q(db);
    q.prepare("select t.terminal_id, (select o.name from terminals o where o.terminal_id = t.owner_id) as reg,  t.adress, t.phone, TRIM(t.ownersystem_id), TRIM(t.owner_id)  from terminals t "
                  "where t.terminaltype = 3 and t.isactive='T' and t.iswork ='T' "
                  "order by t.terminal_id");
    if(!q.exec()) {
        qCritical(logCritical()) << tr("Не удалось получить данные по терминалах.") << q.lastError().text();
        return;
    }
    while (q.next()) {
        TermData azs;
        azs.setTerminalID(q.value(0).toInt());
        azs.setOwnerName(q.value(1).toString());
        azs.setAdress(q.value(2).toString());
        azs.setPhone(q.value(3).toString());
        azs.setOwnerSystemID(q.value(4).toInt());
        azs.setOwnerID(q.value(5).toInt());

        listAzs.insert(azs.getTerminalID(), &azs);
    }
    q.finish();
    db.commit();
    QString strCountAzs = tr("В списке %1 АЗС").arg(listAzs.size());
    //    QLabel *labelStatus =new QLabel(strle);
    //    ui->statusbar->addWidget(labelStatus);

    qInfo(logInfo()) << tr("Загружено записей об азс - ") << listAzs.size();
}



void MainWindow::on_actionUserProfile_triggered()
{
    UserProfileDialog *userProfileDlg = new UserProfileDialog(AppParameters::instance().getParameter("userID").toInt());
    userProfileDlg->exec();
}


void MainWindow::on_actionParametrs_triggered()
{
    ParametersDialog *paramDlg = new ParametersDialog();
    if(paramDlg->exec() == QDialog::Accepted) {
        if(!openMposDB()) {
            on_actionParametrs_triggered();
        }
    }
}


bool MainWindow::openMposDB()
{
    bool result = false;

    // Встановити з'єднання з базою даних, використовуючи параметри з AppParameters
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE","mpos");
    db.setHostName(AppParameters::instance().getParameter("mposHostName"));
    db.setPort(AppParameters::instance().getParameter("mposPort").toInt());
    db.setDatabaseName(AppParameters::instance().getParameter("mposDB"));
    db.setUserName(AppParameters::instance().getParameter("mposUser"));
    CriptPass crP;
    db.setPassword(crP.decriptPass(AppParameters::instance().getParameter("mposPass")));
    if(db.open()){
        qInfo(logInfo()) << tr("Центрльная база данных MPos успешно открыта.");
        result = true;
    } else {
        qCritical(logCritical()) << QApplication::tr("Ошибка открытия базы данных MPos:") << db.lastError().text();
            QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QApplication::tr("Ошибка подключения к базе данных MPos!"));
        msgBox.setInformativeText(QApplication::tr("Необходимо проверить настройки подключения."));
        msgBox.setStandardButtons(QMessageBox::Yes );
        msgBox.setDetailedText(db.lastError().text());
        msgBox.exec();
        result = false;
    }
    return result;
}

