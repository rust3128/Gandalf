#ifndef CONNECTIONSLISTDIALOG_H
#define CONNECTIONSLISTDIALOG_H

#include "Logs/connectionsmodel.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

namespace Ui {
class ConnectionsListDialog;
}

class ConnectionsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionsListDialog(QWidget *parent = nullptr);
    ~ConnectionsListDialog();

private slots:
    void on_pushButtonRefresh_clicked();

    void on_buttonBox_rejected();

private:
    void createUI();
    void showHistory();
    QString createSQLHistory();
private:
    Ui::ConnectionsListDialog *ui;
    ConnectionsModel *modelLogs;
    QSortFilterProxyModel *proxyModel;
};

#endif // CONNECTIONSLISTDIALOG_H
