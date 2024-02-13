#ifndef PASSMANAGERDIALOG_H
#define PASSMANAGERDIALOG_H

#include "mainwindow.h"
#include <QDialog>
#include <QStandardItem>


namespace Ui {
class PassManagerDialog;
}

class PassManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassManagerDialog(MainWindow *mainWin, QWidget *parent = nullptr);
    ~PassManagerDialog();
private slots:
    void on_listView_doubleClicked(const QModelIndex &index);



    void on_toolButtonRemove_clicked();

    void on_toolButtonAdd_clicked();

    void on_toolButtonEdit_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_buttonBox_rejected();

private:
    void createUI();
    void createModel();
    void handleItemAction(const QModelIndex &index);
private:
    Ui::PassManagerDialog *ui;
    QStandardItemModel *model;
    MainWindow *mainWindow;
};

#endif // PASSMANAGERDIALOG_H
