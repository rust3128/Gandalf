#ifndef EDITWORKPLACEDIALOG_H
#define EDITWORKPLACEDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class EditWorkplaceDialog;
}

class EditWorkplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditWorkplaceDialog(int wID, int termID, QWidget *parent = nullptr);
    ~EditWorkplaceDialog();
private slots:


    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void createUI();
    void createModel();
private:
    Ui::EditWorkplaceDialog *ui;
    int workplaceID;
    int terminalID;
    QSqlQueryModel *modelMposType;
};

#endif // EDITWORKPLACEDIALOG_H
