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
    explicit EditWorkplaceDialog(int wID, QWidget *parent = nullptr);
    ~EditWorkplaceDialog();
private slots:


private:
    void createUI();
private:
    Ui::EditWorkplaceDialog *ui;
    int workplaceID;
};

#endif // EDITWORKPLACEDIALOG_H
