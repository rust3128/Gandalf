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
private:
    void createUI();
    void createConnections();
private:
    Ui::EditPassExcepDialog *ui;
    int m_terminalID;
};

#endif // EDITPASSEXCEPDIALOG_H
