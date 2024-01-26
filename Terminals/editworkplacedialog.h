#ifndef EDITWORKPLACEDIALOG_H
#define EDITWORKPLACEDIALOG_H

#include <QDialog>

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


private:
    void createUI();
private:
    Ui::EditWorkplaceDialog *ui;
    int workplaceID;
    int terminalID;
};

#endif // EDITWORKPLACEDIALOG_H
