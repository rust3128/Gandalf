#include "mainwindow.h"
#include "qdatetime.h"
#include "ui_mainwindow.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "Users/userprofiledialog.h"
#include "AppParameters/parametersdialog.h"
#include "AppParameters/criptpass.h"
#include "Terminals/searchform.h"
#include "Terminals/objectform.h"
#include "Deploys/getdeploys.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QThread>
#include <QPropertyAnimation>

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
    createConnections();

    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&MainWindow::deploysShow);
    timer->setInterval(ui->spinBoxInterval->value()*60000);
    timer->start(); // И запустим таймер
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
    ui->tabWidgetTerminals->hide();
//    QLabel *labelStatus =new QLabel(strTitle);
//    ui->statusbar->addWidget(labelStatus);
    ui->splitter->setStyleSheet(
        "QSplitter::handle:vertical { background-color: #3498db; border: 1px solid #2980b9; }"
        "QSplitter::handle:horizontal { background-color: #3498db; border: 1px solid #2980b9; }"
        );
    ui->splitter->setStretchFactor(0,4);
    ui->splitter->setStretchFactor(1,1);
    deploysShow();


}

void MainWindow::getListAZS()
{
    QSqlDatabase db = QSqlDatabase::database("mpos");
    QSqlQuery q(db);
    QString strSql = QString("select t.terminal_id, (select o.name from terminals o where o.terminal_id = t.owner_id) as reg, t.adress, t.phone, TRIM(t.ownersystem_id), TRIM(t.owner_id) from terminals t "
                             "where t.terminal_id BETWEEN %1 and %2 and t.terminaltype = 3 and t.isactive='T' and t.iswork ='T' "
                             "order by t.terminal_id")
                         .arg(AppParameters::instance().getParameter("minTerminalID"))
                         .arg(AppParameters::instance().getParameter("maxTerminalID"));
    qInfo(logInfo()) << "strSQL" << strSql;

    // q.bindValue(0, AppParameters::instance().getParameter("minTerminalID").toInt());
    // q.bindValue(1, AppParameters::instance().getParameter("maxTerminalID").toInt());
    if(!q.exec(strSql)) {
        qCritical(logCritical()) << tr("Не удалось получить данные по терминалах.") << q.lastError().text();
        return;
    }
    while (q.next()) {
        QSharedPointer<TermData> azs = QSharedPointer<TermData>::create();
        qInfo(logInfo()) << "terminal added" << q.value(0).toInt();
        azs->setTerminalID(q.value(0).toInt());
        azs->setOwnerName(q.value(1).toString());
        azs->setAdress(q.value(2).toString());
        azs->setPhone(q.value(3).toString());
        azs->setOwnerSystemID(q.value(4).toInt());
        azs->setOwnerID(q.value(5).toInt());
        listAzs.insert(azs->getTerminalID(), azs);
//        qDebug() << "Value:" << listAzs.value(azs.getTerminalID())->getAdress();
    }
    q.finish();
    db.commit();
    QString strCountAzs = tr("В списке %1 АЗС").arg(listAzs.size());
    qInfo(logInfo()) << tr("Загружено записей об азс - ") << strCountAzs;
}

void MainWindow::createConnections()
{
    connect(ui->widgetSearch,&SearchForm::signalSendSearchTerminalID,this,&MainWindow::slotGetTerminalID);
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

void MainWindow::slotGetTerminalID(int terminalID)
{
    qInfo(logInfo()) << "Seaching ID:" << terminalID;


    for(qsizetype i =0; i<ui->tabWidgetTerminals->count();++i){
        if(ui->tabWidgetTerminals->tabText(i) == QString::number(terminalID)){
            ui->tabWidgetTerminals->setCurrentIndex(i);
            return;
        }
    }
    if(listAzs.contains(terminalID)){
        QSharedPointer<TermData> termData = listAzs.value(terminalID);
        ObjectForm *objForm = new ObjectForm(termData);
        int tabIdx = ui->tabWidgetTerminals->addTab(objForm, QString::number(terminalID));
        ui->tabWidgetTerminals->setCurrentIndex(tabIdx);
        ui->tabWidgetTerminals->show();
    } else {
        qInfo(logInfo()) << "Not fount terminal ID:" << terminalID;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Терминал <b>%1</b> не найден в базе данных.").arg(terminalID));
        msgBox.setInformativeText(QApplication::tr("Проверте правильность номера терминала или его статус в базе данных. Убедитесь что он в работе и активен."));
        msgBox.setStandardButtons(QMessageBox::Yes );
        msgBox.setDetailedText(tr("Центральный МПос. Меню Настрока->Редактирование справочника терминалов."));
        msgBox.exec();
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


void MainWindow::on_tabWidgetTerminals_tabCloseRequested(int index)
{
    ui->tabWidgetTerminals->setCurrentIndex(index);
    ui->tabWidgetTerminals->currentWidget()->deleteLater();
    if(ui->tabWidgetTerminals->count()==0)
        ui->tabWidgetTerminals->hide();
}

void MainWindow::deploysShow()
{
    // Создаем объект класса и передаем ему параметры
    GetDeploys *getDeps = new GetDeploys(ui->spinBoxPorog->value());
    // Создаем поток в которм будут производиться наша выборка
    QThread *thread = new QThread();
    // Перемещаем объект класса в поток
    getDeps->moveToThread(thread);

    //// Сигналы и слоты для взаимидействия с потоком

    // при старте потока выполняем некоторые действия в текущем потоке.
    // В моем случае на просто засекаю начало выбоки данных
    connect(thread,&QThread::started,this,&MainWindow::slotStartGetDeploys);
    // При старте потока начинаем выборку данных
    connect(thread,&QThread::started,getDeps,&GetDeploys::createListDeploys);
    // Передача сообщения об ошибке при подключении к БД
    connect(getDeps, &GetDeploys::signalError, this, &MainWindow::slotErrorGetDeploys);
    // Передача результирующего объекта QVertor из дочернего потока в основной
    connect(getDeps, &GetDeploys::signalSendDeployList, this, &MainWindow::slotGetDeploys);
    // Окончание работы потока по завершению выбрки данных
    connect(getDeps, &GetDeploys::finish,thread,&QThread::quit);
    // Удаляем объект в потоке
    connect(getDeps, &GetDeploys::finish,getDeps, &GetDeploys::deleteLater);
    // Вы полняем действия по в основном потоке после завершения дочернего
    connect(getDeps, &GetDeploys::finish,this,&MainWindow::slotFinishGetDeploys);
    // Прощаемся с дочерним потоком
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    // Запускаем поток
    thread->start();
}

void MainWindow::slotErrorGetDeploys(QString message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Ошибка подключения."));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Произошла ошибка при подключении к центральной базе данных!"));
    msgBox.setInformativeText(tr("Текст ошибки:\n")+message);
    QString strDetali = QString("Server:\t"+AppParameters::instance().getParameter("mposHostName")+":"+AppParameters::instance().getParameter("mposPort")+
                                "\nDatabase:\t"+AppParameters::instance().getParameter("mposDB")+
                                "\nUser:\t"+AppParameters::instance().getParameter("mposUser"));
    msgBox.setDetailedText(strDetali);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::slotStartGetDeploys()
{
    showDeploysData(false);
}

void MainWindow::slotGetDeploys(QVector<DeployData> dp)
{
    deploys = dp;
    QString mess;
    if(deploys.size() == 0){
        ui->toolBox->setItemIcon(0,QIcon(":/Images/green_icon.png"));
        mess = " Обмен с АЗС - Нет проблем с отзвонами! ";
    } else {
        mess = QString("Обмен с АЗС - %1 АЗС нет данных за последние %2")
                           .arg(deploys.size())
                           .arg(ui->spinBoxPorog->text());
        ui->toolBox->setItemIcon(0,QIcon(":/Images/alert_icon.png"));

    }
    ui->toolBox->setItemText(0,"Статисика обмена с АЗС");
    setMarqueeText(mess);
}

void MainWindow::setMarqueeText(const QString& text)
{
    QLabel *marqueeLabel = ui->labelDepInfo;
    marqueeLabel->setText(text);

    QTimer *marqueeTimer = new QTimer(this);
    connect(marqueeTimer, &QTimer::timeout, [=]() {
        QString marqueeText = marqueeLabel->text();
        marqueeText.push_back(marqueeText.at(0));
        marqueeText.remove(0, 1);
        marqueeLabel->setText(marqueeText);
    });

    marqueeTimer->start(300);  // Змініть цей інтервал в залежності від швидкості бігучої стрічки
}

void MainWindow::slotFinishGetDeploys()
{
    ui->labelLastDeploys->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    showDeploysData(true);

    depModel = new DeploysModel(deploys);
    proxyDep = new QSortFilterProxyModel();
    proxyDep->setSourceModel(depModel);
    ui->tableViewDeploys->setModel(proxyDep);
    ui->tableViewDeploys->resizeColumnsToContents();
    ui->tableViewDeploys->verticalHeader()->setDefaultSectionSize(ui->tableViewDeploys->verticalHeader()->minimumSectionSize());
}

void MainWindow::showDeploysData(bool show)
{
    ui->labelWaiting->setHidden(show);
    ui->labelDepInfo->setVisible(show);
    ui->labelLastDeploys->setVisible(show);
    ui->tableViewDeploys->setVisible(show);
    ui->labelDap2->setVisible(show);
    ui->spinBoxInterval->setVisible(show);
    ui->labelDep3->setVisible(show);
    ui->spinBoxPorog->setVisible(show);
    ui->pushButtonRefreshDeploys->setVisible(show);
}

void MainWindow::on_action_AboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


void MainWindow::on_actionAbout_triggered()
{
    QDate versionDate = QDate::fromString(__DATE__,"MMM dd yyyy");
    if (!versionDate.isValid())
    {
        versionDate = QDate::fromString(__DATE__,"MMM d yyyy");
    }
    QString aboutText = QString("Version: %1\nBuild date: %2")
                            .arg(QApplication::applicationVersion())
                            .arg(versionDate.toString("dd.MM.yyyy"));
    qDebug() << aboutText;
    QMessageBox::about(this, tr("Про програму"), aboutText);
}


void MainWindow::on_pushButtonRefreshDeploys_clicked()
{
    deploysShow();
}

