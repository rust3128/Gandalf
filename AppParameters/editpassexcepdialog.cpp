#include "editpassexcepdialog.h"
#include "ui_editpassexcepdialog.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"


#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

EditPassExcepDialog::EditPassExcepDialog(int termID, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPassExcepDialog)
    , m_terminalID(termID)
{
    ui->setupUi(this);
    passOK =false;
    createConnections();
    createUI();
}

EditPassExcepDialog::~EditPassExcepDialog()
{
    delete ui;
}

void EditPassExcepDialog::passChanged()
{
    QString text1 = ui->lineEditPass1->text();
    QString text2 = ui->lineEditPass2->text();
    if (text1.length() != 8 || text2.length() != 8){
        ui->labelInfo->setText(tr("Пароли должны состоять\nиз 8 символов."));
        return;
    }

    if (text1 == text2) {
        ui->labelInfo->hide();
        passOK =true;
        // Викликати інші дії або обробку тексту, якщо потрібно
    } else {
        ui->labelInfo->setText(tr("Пароли не совпадают."));
        // Викликати інші дії або обробку тексту, якщо потрібно
    }
}

void EditPassExcepDialog::createUI()
{
    ui->labelInfo->setText("");
    QAction *showPass1Action = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показать пароль"), this);
    connect(showPass1Action, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPass1->setEchoMode(ui->lineEditPass1->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    ui->lineEditPass1->addAction(showPass1Action, QLineEdit::TrailingPosition);

    QAction *showPass2Action = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показать пароль"), this);
    connect(showPass2Action, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPass2->setEchoMode(ui->lineEditPass2->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    ui->lineEditPass2->addAction(showPass2Action, QLineEdit::TrailingPosition);

    if(m_terminalID != 0){
        this->setWindowTitle(tr("Редактирование пароля"));
        ui->lineEditTerminalID->setReadOnly(true);
        ui->lineEditTerminalID->setStyleSheet("background-color: lightgray;");
        ui->lineEditTerminalID->setText(QString::number(m_terminalID));
        QSqlQuery q;
        q.prepare("SELECT p.passvnc FROM pass_exception_vnc p WHERE p.terminal_id =?");
        q.bindValue(0,m_terminalID);
        if(!q.exec()) {
            qCritical(logCritical()) << tr("Не удалось получить не шаблонный пароль VNC") << q.lastError().text();
            MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось получить не шаблонный пароль VNC"),"",q.lastError().text());
        }
        q.next();
        CriptPass crP;
        QString passVNC = crP.decryptVNCPass(q.value(0).toString());
        ui->lineEditPass1->setText(passVNC);
        ui->lineEditPass2->setText(passVNC);
    } else {
        this->setWindowTitle(tr("Новый пароль VNC"));
    }
}

void EditPassExcepDialog::createConnections()
{
    connect(ui->lineEditPass1, &QLineEdit::textChanged, this, &EditPassExcepDialog::passChanged);
    connect(ui->lineEditPass2, &QLineEdit::textChanged, this, &EditPassExcepDialog::passChanged);
}

void EditPassExcepDialog::on_buttonBox_rejected()
{
    this->reject();
}


void EditPassExcepDialog::on_buttonBox_accepted()
{
    CriptPass crP;
    int termID = ui->lineEditTerminalID->text().trimmed().toInt();
    if(termID < AppParameters::instance().getParameter("minTerminalID").toInt() ||
        termID > AppParameters::instance().getParameter("maxTerminalID").toInt()){
        QMessageBox::critical(this,tr("Ошибка"),tr("Не верный номер терминала!"));
        return;
    }
    if(!passOK) {
        return;
    }
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    QString strQuery ="UPDATE OR INSERT INTO PASS_EXCEPTION_VNC (TERMINAL_ID, PASSVNC) "
        "VALUES (?, ?) "
        "MATCHING (TERMINAL_ID);";
    q.prepare(strQuery);
    q.bindValue(0,ui->lineEditTerminalID->text().trimmed());
    q.bindValue(1,crP.cryptVNCPass(ui->lineEditTerminalID->text().trimmed(), ui->lineEditPass1->text().trimmed()));
    if(!q.exec()){
        qCritical(logCritical()) << tr("Не удалось установить исключение для пароля VNC.") << q.lastError().text();
        MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось установить исключение для пароля VNC."),"",q.lastError().text().trimmed());
    } else {
        qInfo(logInfo()) << tr("Добалено исключение для пароля VNC. Терминал:") << termID;
    }
    this->accept();
}

