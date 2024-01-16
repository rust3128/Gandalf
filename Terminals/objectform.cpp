// objectform.cpp

#include "objectform.h"
#include "ui_objectform.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"
#include "Terminals/tanksinfo.h"
#include "Terminals/dispenserinfo.h"
#include "Logs/logger.h"



#include <QClipboard>
#include <QToolTip>
#include <QTextDecoder>
#include <QLineEdit>
#include <QThread>
#include <QPushButton>
#include <QtConcurrent>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>



ObjectForm::ObjectForm(QSharedPointer<TermData> tData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectForm),
    m_termData(tData)
{
    ui->setupUi(this);


    dbCenter = QSqlDatabase::database("mpos");
    createConnList();
    getTypeVNCClient();
    createUI();

    сheckingСonnections();
    createConnections();
    tanksTabShow();
    trkTabShow();
}

ObjectForm::~ObjectForm()
{
    pingModel->deleteLater();
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
    //Типи клієнтів VNC
    ui->comboBoxTypeVNC->addItem(QIcon(":/Images/tightvnc-logo.png"),"TightVNC", AppParameters::instance().TIGHT_CLIENT_VNC);
    ui->comboBoxTypeVNC->addItem(QIcon(":/Images/UltraVNC_Icon.png"),"UltraVNC", AppParameters::instance().ULTRA_CLIENT_VNC);

    // Встановіть індекс для вибору
    ui->comboBoxTypeVNC->setCurrentText(m_typeVNC);

    ui->labelLastPackage->setText(getLastPackage());

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

void ObjectForm::getTypeVNCClient()
{
    QString queryText = "SELECT t.type_vnc FROM type_exception_vnc t WHERE t.terminal_id = ?";

    QSqlQuery query;
    query.prepare(queryText);
    query.bindValue(0, m_termData->getTerminalID());

    if (query.exec() && query.next()) {
        // Записів є, обробляємо результат
        int typeVNC = query.value(0).toInt();  // Отримуємо значення типу VNC
        // Обробка значення typeVNC
        qDebug() << "Type VNC: " << typeVNC;
        switch (typeVNC) {
        case 1:
            m_typeVNC = "TightVNC";
            break;
        case 2:
            m_typeVNC = "UltraVNC";
            break;
        default:
            break;
        }
    } else {
        // Немає записів, обробляємо цю ситуацію
        qDebug() << "Немає записів для terminalId" << m_termData->getTerminalID();
        m_typeVNC = AppParameters::instance().getParameter("typeVNC");
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
    buttonMap.clear();
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
    QString passVNC;
    QString queryText = "SELECT p.passvnc from pass_exception_vnc p WHERE p.terminal_id = ?";
    CriptPass crP;
    QSqlQuery query;
    query.prepare(queryText);
    query.bindValue(0, m_termData->getTerminalID());

    if (query.exec() && query.next()){
        passVNC = crP.decryptVNCPass(query.value(0).toString());
    } else {
        if(AppParameters::instance().getParameter("useTemplatePassVNC").toInt()){
            switch (AppParameters::instance().getParameter("templatеVNCPass").toInt()) {
            case 0:
                passVNC = genPassVNCUkrnafta();
                break;
            default:
                break;
            }
        } else {
            passVNC = crP.decriptPass(AppParameters::instance().getParameter("defaultVNCPass"));
        }
    }
    return passVNC;
}

QString ObjectForm::genPassVNCUkrnafta()
{
    QString passVNCUkrnafta = AppParameters::instance().getParameter("TemplPassPrefix").trimmed();
    QString termID = QString::number(m_termData->getTerminalID());
    passVNCUkrnafta += termID.rightJustified(5,'0');
    passVNCUkrnafta += AppParameters::instance().getParameter("TemplPassSufix").trimmed();
    return passVNCUkrnafta;
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
        pingModel->finished();
        pingModel->deleteLater();
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
    int posID=button->getButtonID()+1;
    LogData ld(AppParameters::instance().getParameter("userID").toInt(), m_termData->getTerminalID(), posID, AppParameters::LOG_TYPE_CONNECT,"");
    Logger log(ld);
    qInfo(logInfo()) << "Pushed" << connList.at(button->getButtonID())->getHostName()+":"+QString::number(connList.at(button->getButtonID())->getPort());

    QString clientVNCpath;
 //   QString VNCType = AppParameters::instance().getParameter("typeVNC");
    QString hostName = connList.at(button->getButtonID())->getHostName();
    int port = connList.at(button->getButtonID())->getPort();
    QString passVNC = connList.at(button->getButtonID())->getPassVNC();
    // Аргументи для передачі
    QStringList arguments;
    if (m_typeVNC == "TightVNC") {
        clientVNCpath = AppParameters::instance().getParameter("clientVNCPath");
        arguments << QString("-host=%1").arg(hostName) << QString("-port=%2").arg(port) << QString("-password=%3").arg(passVNC);
    } else if (m_typeVNC == "UltraVNC") {
        clientVNCpath = AppParameters::instance().getParameter("clientUltraVNCPath");
        arguments << QString("-password %3").arg(passVNC) << QString("%1::%2").arg(hostName).arg(port);
    } else {
        // Обробка невідомого типу VNC, якщо потрібно
    }

    QFile file(clientVNCpath);
    if(!file.exists()){
        QMessageBox::critical(this, tr("Ошибка"), tr("Файл запуска VNC клиента не найден:\n")+clientVNCpath+tr("\nПроверьте настройки приложения и наличие файла по указанному пути."));
        return;
    }
    qInfo(logInfo()) << "VNC arguments" << arguments;

    // Створення об'єкту QProcess
    myProcess = new QProcess();

    // Підключення слоту до сигналу finished()
    connect(myProcess, &QProcess::finished, this, [this, ld](int exitCode, QProcess::ExitStatus exitStatus) {
        slotVNCProcessFinished(exitCode, exitStatus, ld);
    });


    // Запуск програми з аргументами
    myProcess->start(clientVNCpath, arguments);

    log.writeToLog();
}

void ObjectForm::slotVNCProcessFinished(int exitCode, QProcess::ExitStatus exitStatus, LogData _ld)
{
    _ld.setLogTypeID(AppParameters::LOG_TYPE_DISCONNECT);
    Logger lg(_ld);
    lg.writeToLog();
    qDebug() << "vncviewer.exe finished with exit code:" << exitCode;
    qDebug() << "Exit status:" << (exitStatus == QProcess::NormalExit ? "Normal Exit" : "Crash Exit");
}

void ObjectForm::on_pushButtonRefreshAcces_clicked()
{
    QVBoxLayout *layout = ui->verticalLayoutButton;
    QList<int> sortedButtonIDs = buttonMap.keys();
    // Видаліть тільки об'єкти ButtonVNC
    for (const int &buttonID : sortedButtonIDs) {
        ButtonVNC *button = buttonMap.value(buttonID);
        if (button) {
            layout->removeWidget(button); // Видалити кнопку з макету
            delete button; // Видалити об'єкт кнопки
        }
    }
    buttonMap.clear();
    сheckingСonnections();

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



void ObjectForm::on_comboBoxTypeVNC_activated(int index)
{
    QString selectedText = ui->comboBoxTypeVNC->itemText(index);
    if (selectedText != m_typeVNC) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Изменение типа клиента VNC"),
                                      tr("Вы действительно хотите изменить тип клиента VNC для данной АЗС?"), QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Користувач вибрав "Так"
            // Тут виконайте дії, які потрібно виконати при зміні
            m_typeVNC = selectedText;  // Оновити значення typeVNC
            if(m_typeVNC != AppParameters::instance().getParameter("typeVNC")){
                writeExeptionVNC(m_typeVNC);
            } else {
                QString queryText = "DELETE FROM type_exception_vnc t WHERE t.terminal_id = ?";
                QSqlQuery query;
                query.prepare(queryText);
                query.bindValue(0, m_termData->getTerminalID());
                query.exec();
                query.finish();
                LogData _ld(AppParameters::instance().getParameter("userID").toInt(), m_termData->getTerminalID(), 0, AppParameters::LOG_TYPE_CHANGE_VNC_CLIENT, m_typeVNC + tr(" - По умолчанию"));
                Logger _log(_ld);
                _log.writeToLog();
            }
            // Додайте додаткову логіку, яку вам потрібно виконати
        } else {
            // Користувач вибрав "Ні" або натиснув "Відмінити"
            // Тут виконайте дії, які потрібно виконати, якщо зміна відмінена
            // Наприклад, не змінюйте значення typeVNC
        }
    }

}

void ObjectForm::writeExeptionVNC(QString typeVNC)
{
    QSqlQuery q;
    int codeVNC = (typeVNC == "TightVNC") ? AppParameters::TIGHT_CLIENT_VNC : AppParameters::ULTRA_CLIENT_VNC;
    QString strSQL = QString("INSERT INTO TYPE_EXCEPTION_VNC (TERMINAL_ID, TYPE_VNC) VALUES (%1, %2)")
        .arg(m_termData->getTerminalID())
        .arg(codeVNC);
    qInfo(logInfo()) << "sqrSQL" << strSQL;
    if(!q.exec(strSQL)) {
        qCritical(logCritical()) << tr("Не удалоь записать тип клиента в таблицу исключений.") << q.lastError().text();
    }
    q.finish();
    LogData _ld(AppParameters::instance().getParameter("userID").toInt(), m_termData->getTerminalID(), 0, AppParameters::LOG_TYPE_CHANGE_VNC_CLIENT, typeVNC);
    Logger _log(_ld);
    _log.writeToLog();
}


void ObjectForm::on_pushButtonChangePassVNC_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Изменение пароля VNC"),
                                  tr("Вы действительно хотите изменить пароль VNC для подключения к данной АЗС?"), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool ok;
        QInputDialog inputDialog;
        inputDialog.setWindowTitle(QString::number(m_termData->getTerminalID())+tr(" Изменение пароля"));
        inputDialog.setLabelText(tr("Введите новый пароль для VNC:"));
        inputDialog.setInputMode(QInputDialog::TextInput);
        inputDialog.setTextEchoMode(QLineEdit::Password);  // Встановлення режиму паролю
        inputDialog.setTextValue("");  // Початкове значення

        ok = inputDialog.exec();

        if (ok) {
            QString password = inputDialog.textValue().left(8);  // Обмеження довжини тексту
            if (!password.isEmpty()) {
                // Введений пароль успішно отриманий
                qDebug() << "Ви ввели пароль: " << password;
                writeExceptionPass(password);
            } else {
                // Введено порожній пароль
                qDebug() << "Введено порожній пароль";
                QMessageBox::information(this,tr("Внимание!"),tr("Пароль был пустым. Изменения не выполнятся."));
                return;
            }
        } else {
            // Введення скасовано
            qDebug() << "Введення скасовано";
            return;
        }
    }
}

void ObjectForm::writeExceptionPass(QString passVNC) {
    QSqlDatabase db = QSqlDatabase::database();  // Отримати існуюче підключення до бази даних

    if (!db.isOpen()) {
        qDebug() << "Помилка: підключення до бази даних відсутнє.";
        return;
    }
    QString termID = QString::number(m_termData->getTerminalID());
    CriptPass crP;
    QString strSQL = QString("UPDATE OR INSERT INTO PASS_EXCEPTION_VNC (TERMINAL_ID, PASSVNC) VALUES (%1, '%2') MATCHING (TERMINAL_ID); ")
                         .arg(m_termData->getTerminalID())
                         .arg(crP.cryptVNCPass(termID, passVNC));

    qDebug() << strSQL;
    QSqlQuery query;

    if (!query.exec(strSQL)) {
        qDebug() << "Помилка виконання SQL-запиту:" << query.lastError().text();
        return;
    }
    db.commit();
    LogData _ld(AppParameters::instance().getParameter("userID").toInt(), m_termData->getTerminalID(), 0, AppParameters::LOG_TYPE_CHANGE_VNC_PASS, "");
    Logger _log(_ld);
    _log.writeToLog();
    qDebug() << "Запис виконано успішно.";

    db.commit();
}

QString ObjectForm::getLastPackage()
{
    QString depMessages;
    QSqlQuery q(dbCenter);
    q.prepare("select first 1 p.doper from mgt$packets p where p.terminal_id = :term  and p.apply='T' order by p.doper desc");
    q.bindValue(":term", m_termData->getTerminalID());
    if(!q.exec()){
        qCritical(logCritical()) << "Не могу получить данные об отзвоне" << q.lastError().text();
        return "Не могу получить данные об отзвоне";
    }
    q.next();

    quint64 secDeploy = q.value(0).toDateTime().secsTo(QDateTime::currentDateTime());
    const qint64 SIX_HOURS = 6 * 60 * 60;  // 6 годин у секундах

    if (secDeploy > SIX_HOURS) {
        ui->labelLastPackage->setStyleSheet("color: red;");
    } else {
        ui->labelLastPackage->setStyleSheet("color: green;");
    }
    const qint64 DAY = 86400;
    qint64 days = secDeploy / DAY;
    QTime t = QTime(0,0).addSecs(secDeploy % DAY);

    depMessages = QString(tr("Крайняя передача данных %1\n"))
                              .arg(q.value(0).toDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    depMessages += tr("%1 дн %2 ч %3 м %4 с назад.")
                       .arg(days).arg(t.hour()).arg(t.minute()).arg(t.second());

    q.finish();
    dbCenter.commit();
    return depMessages;
}
