#include "connectionslistdialog.h"
#include "ui_connectionslistdialog.h"
#include "LogginCategories/loggincategories.h"

ConnectionsListDialog::ConnectionsListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionsListDialog)
{
    ui->setupUi(this);
    createUI();
}
ConnectionsListDialog::~ConnectionsListDialog()
{
    delete ui;
}

void ConnectionsListDialog::createUI()
{
    ui->dateTimeEditFrom->setDate(QDate::currentDate());
    ui->dateTimeEditFrom->setTime(QTime(0,0,0));
    ui->dateTimeEditTo->setDate(QDate::currentDate());
    ui->dateTimeEditTo->setTime(QTime(23,59,59));
    modelLogs = nullptr;
    showHistory();
}

void ConnectionsListDialog::showHistory()
{
    QString strQuery = createSQLHistory();
    if(modelLogs != nullptr){
        qInfo(logInfo()) << "NULLPTR";
        modelLogs->deleteLater();
    }
    modelLogs = new ConnectionsModel();
    modelLogs->setQuery(strQuery);

    // Перевірка, чи є дані у моделі
    if (modelLogs != nullptr && modelLogs->rowCount() > 0) {
        ui->tableView->setVisible(true);  // Робимо видимим QTableView
        ui->labelNoData->setVisible(false);  // Ховаємо текстовий віджет
    } else {
        ui->tableView->setVisible(false);  // Ховаємо QTableView
        ui->labelNoData->setVisible(true);  // Робимо видимим текстовий віджет
    }
    QStringList headers;
    headers << tr("Дата") << tr("АЗС") << tr("POS") << tr("Пользователь") << tr("Статус") << tr("Коментарии");
    for(int i=0; i < headers.size(); ++i){
        modelLogs->setHeaderData(i, Qt::Horizontal, headers[i]);
    }

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(modelLogs);
    proxyModel->setDynamicSortFilter(true);
    ui->tableView->setModel(proxyModel);
    ui->tableView->hideColumn(6);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

QString ConnectionsListDialog::createSQLHistory()
{
    QString strSQL = QString("SELECT l.dat, l.terminal_id, l.pos_id, u.user_fio, l.logtype_id, l.comments, u.user_login FROM logs l "
                             "LEFT JOIN users u ON u.user_id = l.user_id "
                             "WHERE l.dat BETWEEN '%1' and '%2' ").arg(ui->dateTimeEditFrom->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                                  .arg(ui->dateTimeEditTo->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if(!ui->lineEditAZS->text().trimmed().isEmpty()){
        strSQL += QString("AND l.terminal_id = %1 ").arg(ui->lineEditAZS->text().trimmed());
    }
    strSQL += QString("AND l.logtype_id IN (1,2,6) "
                      "order BY l.dat DESC");
    return strSQL;
}

void ConnectionsListDialog::on_pushButtonRefresh_clicked()
{
    showHistory();
}


void ConnectionsListDialog::on_buttonBox_rejected()
{
    this->reject();
}

