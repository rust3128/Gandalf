#include "dbsettingsdialog.h"
#include "ui_dbsettingsdialog.h"
#include "AppParameters/AppParameters.h"
#include "AppParameters/criptpass.h"

#include <QSettings>
#include <QFileDialog>

DbSettingsDialog::DbSettingsDialog(bool isNew, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbSettingsDialog)
{
    ui->setupUi(this);
    createUI();
    if(!isNew){
        readSettings();
    }
}

DbSettingsDialog::~DbSettingsDialog()
{
    delete ui;
}

void DbSettingsDialog::openDatabaseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл базы данных"), "",
                                                    tr("База данных (*.gdb *.fdb);;Все файлы (*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    ui->lineEditDBFileName->setText(fileName);
}

void DbSettingsDialog::createUI()
{
    QAction *openFileAction = new QAction(QIcon(":/Images/file_open_icon.png"), tr("Открыть файл"), this);

    // Налаштовуємо обробник події активації QAction
    connect(openFileAction, &QAction::triggered, this, &DbSettingsDialog::openDatabaseFile);
    // Додаємо QAction до QLineEdit у кінцевій позиції
    ui->lineEditDBFileName->addAction(openFileAction, QLineEdit::TrailingPosition);
    QAction *showPasswordAction = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показати пароль"), this);

    // Налаштовуємо обробник події активації QAction
    connect(showPasswordAction, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPass->setEchoMode(ui->lineEditPass->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    // Додаємо QAction до QLineEdit у кінцевій позиції
    ui->lineEditPass->addAction(showPasswordAction, QLineEdit::TrailingPosition);
}

void DbSettingsDialog::readSettings()
{
    ui->lineEditPass->setEchoMode(QLineEdit::Password);
    QSettings settings(AppParameters::CONFIG_FILE_NAME, QSettings::IniFormat);
    ui->lineEditServerName->setText(settings.value("Database/HostName").toString());
    ui->spinBoxPort->setValue(settings.value("Database/Port").toInt());
    ui->lineEditDBFileName->setText(settings.value("Database/DatabaseName").toString());
    ui->lineEditUser->setText(settings.value("Database/UserName").toString());
    CriptPass *crP = new CriptPass();
    ui->lineEditPass->setText(crP->decriptPass(settings.value("Database/Password").toString()));
}

void DbSettingsDialog::writeSettings()
{
    QSettings settings(AppParameters::CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.setValue("Database/HostName", ui->lineEditServerName->text());
    settings.setValue("Database/Port", ui->spinBoxPort->value());
    settings.setValue("Database/DatabaseName", ui->lineEditDBFileName->text());
    settings.setValue("Database/UserName", ui->lineEditUser->text());
    CriptPass *crP = new CriptPass();
    settings.setValue("Database/Password", crP->criptPass(ui->lineEditPass->text()));
}

void DbSettingsDialog::on_buttonBox_accepted()
{
    writeSettings();
    this->accept();
}


void DbSettingsDialog::on_buttonBox_rejected()
{
    this->reject();
}

