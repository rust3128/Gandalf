#ifndef USERPROFILEDIALOG_H
#define USERPROFILEDIALOG_H

#include <QDialog>

namespace Ui {
class UserProfileDialog;
}

class UserProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserProfileDialog(int id, QWidget *parent = nullptr);
    ~UserProfileDialog();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void createUI();
    void writeLogUpdateUser();
private:
    Ui::UserProfileDialog *ui;
    int m_userID;
};

#endif // USERPROFILEDIALOG_H
