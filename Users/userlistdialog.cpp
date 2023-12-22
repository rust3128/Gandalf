#include "userlistdialog.h"
#include "qsqlquery.h"
#include "ui_userlistdialog.h"
#include "Users/userprofiledialog.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/AppParameters.h"

#include <QMessageBox>

UserListDialog::UserListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserListDialog)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

UserListDialog::~UserListDialog()
{
    delete ui;
}

void UserListDialog::createUI()
{
    ui->tableViewUsers->setModel(modelUsers);

    ui->tableViewUsers->resizeColumnsToContents();

}

void UserListDialog::createModel()
{
    modelUsers = new UsersModel();
    modelUsers->setQuery("select u.user_id, u.user_login, u.user_fio, u.mob_phone, u.sip_phone, u.email, u.user_role, u.isactive from users u");
    // Встановлення заголовків стовпців для QTableView
    QMap<int, QString> headersMap;
    headersMap.insert(0, tr("ID"));
    headersMap.insert(1, tr("Логин"));
    headersMap.insert(2, tr("ФИО"));
    headersMap.insert(3, tr("Моб.Телефон"));
    headersMap.insert(4, tr("SIP"));
    headersMap.insert(5, tr("E-mail"));
    headersMap.insert(6, tr("Роль"));
    headersMap.insert(7, tr("Работает"));
    // Проходження по карті і встановлення заголовків для кожного стовпця
    QMapIterator<int, QString> headersIter(headersMap);
    while (headersIter.hasNext()) {
        headersIter.next();
        modelUsers->setHeaderData(headersIter.key(), Qt::Horizontal, headersIter.value());
    }
}

void UserListDialog::on_tableViewUsers_doubleClicked(const QModelIndex &idx)
{
    if(AppParameters::instance().getParameter("userRole").toInt()!=0)
    {
        QString messageText = tr("Доступно только администратору!\n\n")
                              + tr("Свой профиль Вы можете изменить на панели инструментов главного окна программы.");

        QMessageBox::information(this, tr("Важная информация"), messageText);
        return;
    }
    if (idx.isValid()) {
        // Отримано значення першого стовпця
        UserProfileDialog *userProfile = new UserProfileDialog(idx.siblingAtColumn(0).data().toInt());
        if(userProfile->exec() == QDialog::Accepted){
            modelUsers->setQuery(modelUsers->query().lastQuery());
        }

    }
}


void UserListDialog::on_buttonBox_rejected()
{
    this->reject();
}

