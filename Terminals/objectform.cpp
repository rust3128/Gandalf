// objectform.cpp

#include "objectform.h"
#include "ui_objectform.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"
#include "Terminals/tanksinfo.h"
#include "Terminals/dispenserinfo.h"



#include <QClipboard>
#include <QToolTip>
#include <QTextDecoder>
#include <QLineEdit>
#include <QThread>
#include <QPushButton>
#include <QtConcurrent>
#include <QFile>
#include <QMessageBox>



ObjectForm::ObjectForm(QSharedPointer<TermData> tData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectForm),
    m_termData(tData)
{
    ui->setupUi(this);


    dbCenter = QSqlDatabase::database("mpos");
    createConnList();
    createUI();
    сheckingСonnections();
    createConnections();
    tanksTabShow();
    trkTabShow();
}

ObjectForm::~ObjectForm()
{
    delete ui;
}



void ObjectForm::createUI()
{
    ui->labelAZSNumber->setText(("АЗС № "+QString::number(m_termData->getTerminalID())));
    if(AppParameters::instance().getParameter("showKodZem").toInt()){
        ui->labelAddress->setText(m_termData->getAdress()+tr("\nКод земельного участка: ")+QString::number(m_termData->getOwnerSystemID()));
    } else {
        ui->labelAddress->setText(m_termData->getAdress())    ;
    }
    ui->labelPhone->setText(m_termData->getPhone());
    ui->lineEditPingAdress->setText(connList[0]->getHostName());
    ui->tabWidget->setCurrentIndex(0);
}

void ObjectForm::createConnList()
{
    switch (AppParameters::instance().getParameter("templatеHostname").toInt()) {
    case 0:
        //Avias
        connListAvias();
        break;
    default:
        break;
    }

}

void ObjectForm::connListAvias()
{
    connList.clear();
    qInfo(logInfo()) << "Generate Avias. Term ID" << m_termData->getTerminalID();
    int termID=m_termData->getTerminalID();
    for(int i=0; i<AppParameters::instance().getParameter("maxCountPC").toInt(); ++i){
        QSharedPointer<ConnectionData> conTemp = QSharedPointer<ConnectionData>::create(termID);
        QString tempHostname = "azs" + QString::number(m_termData->getOwnerSystemID()) + ".unit.local";
        conTemp->setHostName(tempHostname);
        conTemp->setPort(15901 + i);
        conTemp->setPassVNC(getVNCPassword());
        connList.append(conTemp);
    }
    return;
}

void ObjectForm::сheckingСonnections()
{
    on_toolButtonPingAddres_clicked();
    addButtonConnections();


}


void ObjectForm::createConnections()
{
    connect(ui->lineEditPingAdress,&QLineEdit::returnPressed,this,&ObjectForm::on_toolButtonPingAddres_clicked);
    connect(ui->toolButtonClearPing,&QToolButton::clicked,this,&ObjectForm::on_toolButtonPingAddres_clicked);
}

void ObjectForm::addButtonConnections()
{
    aviableConnections = false;
    ui->progressBar->show();
    threadCount = connList.size();
    ui->progressBar->setMaximum(threadCount);
    ui->progressBar->setValue(0);
    for (int i = 0; i < connList.size(); ++i) {
        ConnStatusVNC *statusVNC = new ConnStatusVNC(connList[i]->getHostName(), connList[i]->getPort(), i);
        QThread *thread = new QThread();
        statusVNC->moveToThread(thread);
        //Connections
        connect(thread,&QThread::started,statusVNC,&ConnStatusVNC::getConnStatus);
        connect(statusVNC,&ConnStatusVNC::signalSendStatus,this,&ObjectForm::slotGetStatus);
        connect(statusVNC,&ConnStatusVNC::finish,thread,&QThread::quit);
        connect(statusVNC,&ConnStatusVNC::finish,statusVNC,&ConnStatusVNC::deleteLater);
        connect(statusVNC,&ConnStatusVNC::finish,this,&ObjectForm::slotFinishConStatus);
        connect(thread,&QThread::finished,thread,&QThread::deleteLater);
        thread->start();
    }
}



void ObjectForm::slotGetStatus(const ConnStatus &status)
{
    if(status.isConnected){
        ButtonVNC *butVNC = new ButtonVNC(status.buttonID);

        butVNC->setToolTip(connList.at(status.buttonID)->getHostName()+":"+QString::number(connList.at(status.buttonID)->getPort()));
        butVNC->setText(tr("Подключиться"));

        // Створення об'єкта шрифта
        QFont font = butVNC->font();
        // Встановлення жирного стилю шрифта
        font.setBold(true);
//        font.setPointSize(10);
        // Встановлення шрифта для кнопки
//        butVNC->setFont(font);
        QString imagePath;
        QString description;
        switch (status.buttonID) {
        case 0:
            imagePath = ":/Images/pos1.png";
            description = tr("1-е рабочее место");
            break;
        case 1:
            imagePath = ":/Images/pos2.png";
            description = tr("2-е рабочее место");
            break;
        case 2:
            imagePath = ":/Images/pos3.png";
            description = tr("3-е рабочее место");
            break;
        case 3:
            imagePath = ":/Images/tovaroved.png";
            description = tr("Товаровед/Директор");
            break;
        default:
            // Зображення за замовчуванням, якщо buttonID не відповідає жодному варіанту
            imagePath = ":/Images/pc_icon.png";
            description = tr("Рабочее место");
            break;
        }
        QIcon buttonIcon(imagePath);
        butVNC->setIcon(buttonIcon);
        butVNC->setIconSize(QSize(48,48));
        butVNC->setDescription(description);
        connect(butVNC,&QAbstractButton::clicked,this,&ObjectForm::slotVNCButtonClicked);
        buttonMap.insert(status.buttonID,butVNC);
//        layoutButton->addWidget(butVNC);
        aviableConnections = true;
    }
    int pbValue = ui->progressBar->value()+1;
    ui->progressBar->setValue(pbValue);

}

void ObjectForm::slotFinishConStatus()
{
    --threadCount;

    if(threadCount == 0){
        if(aviableConnections){
            ui->progressBar->hide();

            // Отримання впорядкованого списку buttonID
            QList<int> sortedButtonIDs = buttonMap.keys();
            std::sort(sortedButtonIDs.begin(), sortedButtonIDs.end());
            ui->verticalLayoutButton->setAlignment(Qt::AlignTop);
            // Додавання кнопок в layoutButton у визначеному порядку
            for (const int &buttonID : sortedButtonIDs) {
                ButtonVNC *button = buttonMap.value(buttonID);
                if (button) {
                    ui->verticalLayoutButton->addWidget(button);
                }
            }
        } else {
            QLabel *noConnectionsLabel = new QLabel(tr("Нет доступных\nподключений!"), ui->groupBoxConnections);
            noConnectionsLabel->setStyleSheet("font-size: 16pt; color: red;");
            noConnectionsLabel->setAlignment(Qt::AlignCenter);
            ui->verticalLayoutButton->addWidget(noConnectionsLabel);
        }
    }
}


QString ObjectForm::getVNCPassword()
{
    CriptPass crP;
    return crP.decriptPass(AppParameters::instance().getParameter("defaultVNCPass"));
}


void ObjectForm::on_toolButtonClipboard_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipTitle = ui->labelAZSNumber->text()+"\n"+ui->labelAddress->text()+"\n"+ui->labelPhone->text();
    clipboard->setText(clipTitle);
    QToolTip::showText(ui->toolButtonClipboard->mapToGlobal(ui->toolButtonClipboard->rect().center()), tr("Адрес скопирован\nв буфер обмена."));
}


void ObjectForm::on_toolButtonAdres2Clip_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->lineEditPingAdress->text().trimmed());
    QToolTip::showText(ui->toolButtonAdres2Clip->mapToGlobal(ui->toolButtonAdres2Clip->rect().center()), tr("Адрес скопирован\nв буфер обмена."));
}

void ObjectForm::slotGetPingString(QByteArray output)
{
    QTextDecoder *decoder = QTextCodec::codecForName("CP866")->makeDecoder();
    QString pStr = decoder->toUnicode(output);
    ui->plainTextEditPing->appendPlainText(pStr.trimmed());
    delete decoder;

}


void ObjectForm::on_toolButtonPingAddres_clicked()
{
    ui->plainTextEditPing->clear();
    // Перевірка, чи існує об'єкт
    if (!pingModel) {
        // Об'єкт не існує, створення нового
        pingModel = new PingModel();
    } else {
        // Об'єкт існує, видалення та створення нового
        delete pingModel;
        pingModel = new PingModel();
    }
    pingModel = new PingModel();
    connect(pingModel,&PingModel::signalSendOutPing,this,&ObjectForm::slotGetPingString);
    connect(this,&ObjectForm::signalSendHost,pingModel,&PingModel::getHostName);
    QString firstHostName = ui->lineEditPingAdress->text().trimmed();
    emit signalSendHost(&firstHostName);
    pingModel->start_command();
}

void ObjectForm::slotVNCButtonClicked()
{
    ButtonVNC *button = static_cast<ButtonVNC*>(sender());
    qInfo(logInfo()) << "Pushed" << connList.at(button->getButtonID())->getHostName()+":"+QString::number(connList.at(button->getButtonID())->getPort());
    QString clientVNCpath = AppParameters::instance().getParameter("clientVNCPath");
    QFile file(clientVNCpath);
    if(!file.exists()){
        QMessageBox::critical(this, tr("Ошибка"), tr("Файл запуска VNC клиента не найден:\n")+clientVNCpath+tr("\nПроверьте настройки приложения и наличие файла по указанному пути."));
        return;
    }
    QString hostName = connList.at(button->getButtonID())->getHostName();
    int port = connList.at(button->getButtonID())->getPort();
    QString passVNC = connList.at(button->getButtonID())->getPassVNC();
    // Аргументи для передачі
    QStringList arguments;
    arguments << QString("-host=%1").arg(hostName) << QString("-port=%2").arg(port) << QString("-password=%3").arg(passVNC);

    // Створення об'єкту QProcess
    myProcess = new QProcess();

    // Підключення слоту до сигналу finished()
    QObject::connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, &ObjectForm::slotVNCProcessFinished);

    // Запуск програми з аргументами
    myProcess->start(clientVNCpath, arguments);
}

void ObjectForm::slotVNCProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "vncviewer.exe finished with exit code:" << exitCode;
    qDebug() << "Exit status:" << (exitStatus == QProcess::NormalExit ? "Normal Exit" : "Crash Exit");
}


void ObjectForm::tanksTabShow()
{
    TanksInfo *tanksInfo = new TanksInfo(m_termData->getTerminalID(), dbCenter);

    QThread *thread = new QThread();

    tanksInfo->moveToThread(thread);

    connect(thread,&QThread::started,this,&ObjectForm::slotStartGetTanksInfo);
    connect(thread,&QThread::started,tanksInfo,&TanksInfo::createTanksQuery);

    connect(tanksInfo,&TanksInfo::queryFinished,this,&ObjectForm::slotGetQueryTanks);
    connect(tanksInfo,&TanksInfo::finished,thread,&QThread::quit);
    connect(tanksInfo,&TanksInfo::finished,tanksInfo,&TanksInfo::deleteLater);
    connect(tanksInfo,&TanksInfo::finished,this,&ObjectForm::slotFinishGetTanks);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    thread->start();

}

void ObjectForm::slotStartGetTanksInfo()
{
    ui->tabWidget->setTabIcon(1,QIcon(":/Images/waiting_icon.png"));
    ui->tableViewTanks->hide();
    ui->labelWaitingTanks->show();

}

void ObjectForm::slotFinishGetTanks()
{
    ui->tabWidget->setTabIcon(1,QIcon());
    ui->tableViewTanks->show();
    ui->labelWaitingTanks->hide();
}

void ObjectForm::slotGetQueryTanks(QList<TankProperty> list)
{
    modelTanks = new TanksInfoModel(list);
    ui->tableViewTanks->setModel(modelTanks);
    ui->tableViewTanks->resizeColumnsToContents();
}


void ObjectForm::trkTabShow()
{
    DispenserInfo *dispInfo = new DispenserInfo(m_termData->getTerminalID(), dbCenter);
    QThread *thread = new QThread();
    dispInfo->moveToThread(thread);

    connect(thread,&QThread::started,this,&ObjectForm::slotStartGetDispInfo);
    connect(thread,&QThread::started,dispInfo,&DispenserInfo::slotCreateDyspInfo);
    connect(dispInfo,&DispenserInfo::queryFinished,this,&ObjectForm::slotGetQueryDisp);
    connect(dispInfo,&DispenserInfo::finished,thread,&QThread::quit);
    connect(dispInfo,&DispenserInfo::finished,dispInfo,&DispenserInfo::deleteLater);
    connect(dispInfo,&DispenserInfo::finished,this,&ObjectForm::slotFinishGetDispInfo);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    thread->start();
}

void ObjectForm::slotStartGetDispInfo()
{
    ui->tabWidget->setTabIcon(2,QIcon(":/Images/waiting_icon.png"));
    ui->treeWidgetTRK->hide();
    ui->labelWaitingDisp->show();
}

void ObjectForm::slotFinishGetDispInfo()
{
    ui->tabWidget->setTabIcon(2,QIcon());
    ui->treeWidgetTRK->show();
    ui->labelWaitingDisp->hide();
}

void ObjectForm::slotGetQueryDisp(QList<DispenserProperty> disp, QList<PunpProperty> pump)
{
    // Очищаем QTreeWidget перед добавлением новых элементов
    ui->treeWidgetTRK->clear();


    // Определяем количество столбцов
    int columnCount = 5;
    ui->treeWidgetTRK->setColumnCount(columnCount);


    // Устанавливаем заголовки столбцов
    QStringList headerLabels;
    headerLabels << tr("№ ТРК") << tr("Протокол") << tr("Порт") << tr("Скорость") << tr("Адрес");

    ui->treeWidgetTRK->setHeaderLabels(headerLabels);

    // Добавляем элементы в QTreeWidget
    QMap<int, QTreeWidgetItem*> items;
    for (const auto &d : disp) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidgetTRK);
        item->setText(0, QString::number(d.dispenser_id()));
        item->setText(1, d.portName());
        item->setText(2, QString::number(d.chanelProt()));
        item->setText(3, QString::number(d.chanelSpeed()));
        item->setText(4, QString::number(d.netAdress()));
        items[d.dispenser_id()] = item;
    }
    for (const auto &p : pump) {
        QTreeWidgetItem* parentItem = items[p.disp_id()];
        if (!parentItem) continue; // Пропустить pump, если не нашли соответствующий dispenser
        if (parentItem->childCount() == 0) { // Добавляем названия столбцов
            QStringList headerLabels;
            headerLabels << tr("Пистолет") << tr("Резервуар") << tr("Топливо");
                QTreeWidgetItem* headerItem = new QTreeWidgetItem(headerLabels);
            for (int i = 0; i < headerLabels.size(); ++i) {
                headerItem->setBackground(i, Qt::gray);
                headerItem->setForeground(i, Qt::white);
            }
            parentItem->addChild(headerItem);
            headerItem->setFlags(headerItem->flags() & ~Qt::ItemIsEditable); // Флаг "не редактируемый"
        }
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, QString::number(p.pump_id()));
        item->setText(1, QString::number(p.tank_id()));
        item->setText(2, p.shortName());
    }
    for (int i = 0; i < ui->treeWidgetTRK->columnCount(); ++i) {
        ui->treeWidgetTRK->resizeColumnToContents(i);
    }
}
