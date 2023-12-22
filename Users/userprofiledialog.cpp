#include "userprofiledialog.h"
#include "ui_userprofiledialog.h"
#include "AppParameters/AppParameters.h"
#include <LogginCategories/loggincategories.h>
#include "Logs/logger.h"

#include <QSqlQuery>
#include <QSqlError>

UserProfileDialog::UserProfileDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserProfileDialog),
    m_userID(id)
{
    ui->setupUi(this);
    createUI();
}

UserProfileDialog::~UserProfileDialog()
{
    delete ui;
}

void UserProfileDialog::createUI()
{
//    ui->lineEditLogin->setText(AppParameters::instance().getParameter("userID"));
    QSqlQuery q;
    q.prepare("SELECT u.user_login, u.user_fio, u.mob_phone, u.sip_phone, u.email, u.lang_ui, u.user_role, u.isactive "
              "FROM users u WHERE user_id = ?");
    q.bindValue(0, m_userID);
    if(!q.exec()) {
        qCritical(logCritical()) << tr("Не удалось получить данные пользователя") << q.lastError().text();
        this->reject();
    }
    q.next();
    ui->lineEditLogin->setText(q.value(0).toString());
    ui->lineEditFIO->setText(q.value(1).toString());
    ui->lineEditPhone->setText(q.value(2).toString());
    ui->lineEditSIP->setText(q.value(3).toString());
    ui->lineEditEmail->setText(q.value(4).toString());
    switch (q.value(5).toInt()) {
    case 1:
        ui->radioButtonUA->setChecked(true);
        break;
    case 2:
        ui->radioButtonUS->setChecked(true);
        break;
    case 3:
        ui->radioButtonRU->setChecked(true);
        break;
    default:
        break;
    }
    int userRole = q.value(6).toInt();
    switch (userRole) {
    case 0:
        ui->radioButtonAdminRole->setChecked(true);
        break;
    case 1:
        ui->radioButtonMangerRole->setChecked(true);
        break;
    case 2:
        ui->radioButtonUserRole->setChecked(true);
    default:
        break;
    }
    ui->checkBoxIsWork->setChecked(q.value(7).toBool());
    if(userRole != 0){
        ui->checkBoxIsWork->setDisabled(true);
        ui->checkBoxIsWork->setToolTip(tr("Доступно только администратору"));
        ui->groupBoxRole->setDisabled(true);
        ui->groupBoxRole->setToolTip(tr("Доступно только администратору"));
    }

}



void UserProfileDialog::on_buttonBox_rejected()
{
    this->reject();
}


void UserProfileDialog::on_buttonBox_accepted()
{
    int langUI=0;
    if (ui->radioButtonUA->isChecked()) {
        langUI = 1; // Встановлюємо значення 1, якщо радіокнопка "Українська" вибрана
    } else if (ui->radioButtonUS->isChecked()) {
        langUI = 2; // Встановлюємо значення 2, якщо радіокнопка "Англійська" вибрана
    } else if (ui->radioButtonRU->isChecked()) {
        langUI = 3; // Встановлюємо значення 3, якщо радіокнопка "Російська" вибрана
    }
    int userRole = 0;
    if(ui->radioButtonAdminRole->isChecked()) {
        userRole=0;
    } else if (ui->radioButtonMangerRole->isChecked()){
        userRole=1;
    } else if (ui->radioButtonUserRole->isChecked()) {
        userRole =2;
    }
    QSqlQuery q;
    q.prepare("UPDATE USERS "
              "SET USER_FIO = ?, "
              "MOB_PHONE = ?, "
              "SIP_PHONE = ?, "
              "EMAIL = ?, "
              "LANG_UI = ?, "
              "USER_ROLE = ?, "
              "ISACTIVE = ? "
              "WHERE (USER_ID = ?)");
    q.bindValue(0, ui->lineEditFIO->text().trimmed());
    q.bindValue(1, ui->lineEditPhone->text().trimmed());
    q.bindValue(2, ui->lineEditSIP->text().trimmed());
    q.bindValue(3, ui->lineEditEmail->text().trimmed());
    q.bindValue(4, langUI);
    q.bindValue(5, userRole);
    q.bindValue(6, ui->checkBoxIsWork->isChecked() ? 1 : 0);
    q.bindValue(7, m_userID);
    if(q.exec()){
        qInfo(logInfo()) << tr("Информация о пользователе успешно обновлена. ID=") << m_userID;
        writeLogUpdateUser();
        this->accept();
    } else {
        qCritical(logCritical()) << tr("Не удалось обнвить информацию о пользователе. ID=") << m_userID << q.lastError().text();
        this->reject();
    }
}

void UserProfileDialog::writeLogUpdateUser()
{
    QString strComments = QString(tr("Пользователь ")+AppParameters::instance().getParameter("userLogin")+tr(" обновил профиль ")+ui->lineEditLogin->text());
    LogData logDat(AppParameters::instance().getParameter("userID").toInt(),0,0,AppParameters::LOG_TYPE_UPDATE_USER_PROFILE,strComments);
    Logger logger(logDat);
    logger.writeToLog();

}
