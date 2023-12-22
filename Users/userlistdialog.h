#ifndef USERLISTDIALOG_H
#define USERLISTDIALOG_H
#include "Users/UsersModel.h"
#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class UserListDialog;
}

class UserListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserListDialog(QWidget *parent = nullptr);
    ~UserListDialog();
private slots:
    void on_tableViewUsers_doubleClicked(const QModelIndex &index);

    void on_buttonBox_rejected();

private:
    void createUI();
    void createModel();
private:
    Ui::UserListDialog *ui;
    UsersModel *modelUsers;
};

#endif // USERLISTDIALOG_H
