#ifndef EDITPASSEXCEPDIALOG_H
#define EDITPASSEXCEPDIALOG_H

#include <QDialog>

namespace Ui {
class EditPassExcepDialog;
}

class EditPassExcepDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPassExcepDialog(int termID, QWidget *parent = nullptr);
    ~EditPassExcepDialog();
private slots:
    void passChanged();
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void createUI();
    void createConnections();
private:
    Ui::EditPassExcepDialog *ui;
    int m_terminalID;
    bool passOK;
};

#endif // EDITPASSEXCEPDIALOG_H
