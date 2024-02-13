#include "sendmessagedialog.h"
#include "ui_sendmessagedialog.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"

#include <QClipboard>
#include <QSqlQuery>
#include <QSqlError>

SendMessageDialog::SendMessageDialog(int termID, QString dbHost, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SendMessageDialog)
    , terminalID(termID)
    , dbHostname(dbHost)
{
    ui->setupUi(this);

    connect(ui->plainTextEditMessage, &QPlainTextEdit::textChanged, this, &SendMessageDialog::on_plainTextEditMessage_textChanged);




}

SendMessageDialog::~SendMessageDialog()
{
    delete ui;
}

void SendMessageDialog::createUI()
{
    readDBConnections();
    ui->lineEditServerName->setText(dbHostname);
    ui->spinBoxPort->setValue(dbPort);
    ui->lineEditDBFileName->setText(dbDatabase);
    ui->lineEditUser->setText(dbUser);
    qInfo(logInfo()) << "DB Pass" << dbPass;
    ui->lineEditPass->setText(dbPass);
    ui->lineEditPhone->setText(AppParameters::instance().getParameter("hotlinePhone"));
    defMessage =  tr("Это техподдержка. Позвоните по указанным телефонам. ");
    if(ui->checkBox->isChecked()){
        ui->plainTextEditMessage->setPlainText(defMessage+ui->lineEditPhone->text());
    } else {
        ui->plainTextEditMessage->setPlainText(defMessage);
    }
    ui->pushButtonSendMessage->setEnabled(false);

}

void SendMessageDialog::readDBConnections()
{
    QSqlQuery query;

    if (query.prepare("SELECT t.hostname, t.port, t.dbase, t.dbuser, t.pass FROM terminalsdb t WHERE t.terminal_id = :id")) {
        query.bindValue(":id", terminalID);
        if (query.exec()) {
            if (query.next()) {
                // Запис існує, отримаємо дані
                dbHostname = query.value(0).toString();  // Перший стовпець (hostname)
                dbPort = query.value(1).toInt();  // Другий стовпець (port)
                dbDatabase = query.value(2).toString();  // Третій стовпець (dbase)
                dbUser = query.value(3).toString();  // Четвертий стовпець (dbuser)
                dbPass = query.value(4).toString();  // П'ятий стовпець (pass)
                CriptPass crP;
                dbPass = crP.decriptPass(dbPass);
            } else {
                // Запис не існує, виконайте інші дії
                createDBconnections();
            }
        } else {
            // Помилка виконання запиту
            qCritical(logCritical()) << tr("Не удалось получить запись о подключении к БД АЗС") << query.lastError().text();
        }
    } else {
        // Помилка підготовки запиту або прив'язки значень
        qDebug() << "Помилка підготовки запиту або прив'язки значень:" << query.lastError().text();
    }


}

void SendMessageDialog::createDBconnections()
{
    switch (AppParameters::instance().getParameter("templatеHostname").toInt()) {
    case 0:
        //Avias

        break;
    case 1:
        dbConnUrknafta();
        break;
    case 2:
        //Marshal
        break;
    case 3:
        //Database
        dbConnDatabase();
        break;
    default:
        break;
    }


}



void SendMessageDialog::dbConnUrknafta()
{
    dbPort = 3050;
    QString strTermID = QString::number(terminalID);
    dbPass = "PaSSun" + strTermID.left(strTermID.length() - 3).rightJustified(2, '0');
}

void SendMessageDialog::dbConnDatabase()
{
    dbPort = 3050;
    dbPass = "masterkey";
}

void SendMessageDialog::writeDBParametrs()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    CriptPass crP;
    // Підготовка SQL-запиту
    QString sqlQuery = "UPDATE OR INSERT INTO TERMINALSDB "
                       "(TERMINAL_ID, HOSTNAME, PORT, DBASE, DBUSER, PASS) "
                       "VALUES (:terminalId, :hostname, :port, :dbase, :dbuser, :pass) "
                       "MATCHING (TERMINAL_ID)";

    // Виконання підготовленого SQL-запиту
    query.prepare(sqlQuery);
    query.bindValue(":terminalId", terminalID);
    query.bindValue(":hostname", ui->lineEditServerName->text().trimmed());
    query.bindValue(":port", ui->spinBoxPort->value());
    query.bindValue(":dbase", ui->lineEditDBFileName->text().trimmed());
    query.bindValue(":dbuser", ui->lineEditUser->text().trimmed());
    query.bindValue(":pass", crP.criptPass(ui->lineEditPass->text().trimmed()));

    if (query.exec()) {
        qDebug() << "Query executed successfully!";
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
    db.commit();
}

void SendMessageDialog::dbAzsOpen()
{
    QSqlDatabase dbT = QSqlDatabase::addDatabase("QIBASE","dbazs");
    dbT.setHostName(ui->lineEditServerName->text().trimmed());
    dbT.setPort(ui->spinBoxPort->value());
    dbT.setDatabaseName(ui->lineEditDBFileName->text().trimmed());
    dbT.setUserName(ui->lineEditUser->text().trimmed());
    dbT.setPassword(ui->lineEditPass->text().trimmed());

    if(dbT.open()){
        ui->labelSatusConnections->setPixmap(QPixmap(":/Images/connect_established.png"));
        ui->pushButtonSendMessage->setEnabled(true);
        ui->pushButtonSendMessage->setToolTip(tr("Отправить сообщение"));
        writeDBParametrs();
    } else {
        ui->labelSatusConnections->setPixmap(QPixmap(":/Images/connect_no_icon.png"));
        ui->pushButtonSendMessage->setEnabled(false);
        ui->pushButtonSendMessage->setToolTip(tr("Не возможно отправить сообщение. Отсутсвует соединение с базой данных АЗС."));
        MyMessage::showNotification(QMessageBox::Information, tr("Внимание"), tr("Не возможно подключиться к базе данных АЗС!"), tr("Проверте настройки подключение и повторите попытку!"), dbT.lastError().text());
    }
}

void SendMessageDialog::on_buttonBox_rejected()
{
    this->reject();
}

void SendMessageDialog::on_plainTextEditMessage_textChanged()
{
    // Оновлення лічильника залишених символів
    int remainingCharacters = 100 - ui->plainTextEditMessage->toPlainText().length();
    ui->labelCharacterCount->setText(tr("Осталось символов: %1").arg(remainingCharacters));

    if (ui->plainTextEditMessage->toPlainText().length() > 100) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введено слишком много символов!"));
        ui->plainTextEditMessage->setPlainText(ui->plainTextEditMessage->toPlainText().left(100));  // Обмеження тексту до 100 символів
    }
}


void SendMessageDialog::on_checkBox_stateChanged(int state)
{
    QString strTemp = ui->plainTextEditMessage->toPlainText();
    if(state == Qt::Checked){
        ui->plainTextEditMessage->setPlainText(strTemp+ui->lineEditPhone->text());
    } else {
        strTemp.replace(ui->lineEditPhone->text(), "");
        ui->plainTextEditMessage->setPlainText(strTemp);
    }
}


void SendMessageDialog::on_pushButtonSendMessage_clicked()
{
    if(ui->plainTextEditMessage->toPlainText().length()>100) {
        QMessageBox::warning(this, tr("Ошибка"), tr("В сообщении слишком много символов! Максимум 100!"));
        return;
    }
    QString strSQL = QString("EXECUTE PROCEDURE show_operator_message('%1');").arg(ui->plainTextEditMessage->toPlainText());

    qInfo(logInfo()) << "STR show" << strSQL;
    QSqlDatabase dbT = QSqlDatabase::database("dbazs");
    QSqlQuery q = QSqlQuery(dbT);
    q.prepare(strSQL);
    if(!q.exec()) {
        MyMessage::showNotification(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось отправить сообщениена АЗС!"),
                                    tr("Проверте настройки подключение и повторите попытку!"), q.lastError().text());
        qCritical(logCritical()) << "Dont send Message" <<  q.lastError().text();
    } else {
        q.finish();
        dbT.commit();
        QMessageBox::information(this, tr("Внимание"), tr("Сообщение успешно отправлено!"));
    }

}




void SendMessageDialog::showEvent(QShowEvent *event)
{

    QDialog::showEvent(event);
    createUI();

    // Очікування певного часу перед викликом dbAzsOpen
    const int delayBeforeDbAzsOpen = 100;
    QTimer::singleShot(delayBeforeDbAzsOpen, this, &SendMessageDialog::dbAzsOpen);


}

void SendMessageDialog::on_pushButtonTestConnection_clicked()
{
    ui->labelSatusConnections->setPixmap(QPixmap(":/Images/waiting_icon.png"));
    ui->pushButtonSendMessage->setEnabled(false);
    const int delayBeforeDbAzsOpen = 100;
    QTimer::singleShot(delayBeforeDbAzsOpen, this, &SendMessageDialog::dbAzsOpen);
}

