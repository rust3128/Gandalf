#include "passmanagerdialog.h"
#include "ui_passmanagerdialog.h"
#include "LogginCategories/loggincategories.h"
#include <AppParameters/editpassexcepdialog.h>

#include <QSqlQuery>
#include <QSqlError>


PassManagerDialog::PassManagerDialog(MainWindow *mainWin, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PassManagerDialog)
    , mainWindow(mainWin)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

PassManagerDialog::~PassManagerDialog()
{
    delete ui;
}

void PassManagerDialog::createUI()
{
    ui->listView->setModel(model);
}

void PassManagerDialog::createModel()
{
    model = new QStandardItemModel(this);


    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    QString adress;
    QString sqlQuery = "SELECT p.terminal_id FROM pass_exception_vnc p ORDER BY p.terminal_id";
    q.prepare(sqlQuery);
    if (!q.exec()) {
        qCritical(logCritical()) << tr("Не удалось получить список исключений для праролей VNC") << q.lastError().text();
        return;
    }

    while (q.next()) {
        QString terminalID = q.value(0).toString();
        adress = mainWindow->sendAdressAZS(terminalID.toInt());
        QString itemText = terminalID + "|" + adress;

        QStandardItem *item = new QStandardItem(itemText);
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);  // Задайте початковий стан чекбокса
        model->appendRow(item);
    }

}

void PassManagerDialog::on_listView_doubleClicked(const QModelIndex &index)
{
    // Отримати текст вибраного елемента
    QString selectedText = model->data(index, Qt::DisplayRole).toString();

    // Розділити текст за допомогою символу "|"
    QStringList parts = selectedText.split('|');

    if (parts.size() == 2) {
        // Отримати числове значення
        QString terminalIdString = parts[0];
        bool ok;
        int terminalId = terminalIdString.toInt(&ok);

        if (ok) {
            // Здійснюйте подальші дії з числовим значенням (terminalId)
            qDebug() << "Double-clicked on Terminal ID:" << terminalId;
        } else {
            qDebug() << "Неможливо конвертувати в числове значення:" << terminalIdString;
        }
    }
}



void PassManagerDialog::on_toolButtonRemove_clicked()
{
    // Використовуємо match() для пошуку всіх елементів із встановленим чекбоксом
    QModelIndexList checkedIndexes = ui->listView->model()->match(
        ui->listView->model()->index(0, 0),
        Qt::CheckStateRole,
        Qt::Checked,
        -1,
        Qt::MatchExactly | Qt::MatchRecursive
        );

    QStringList selectedTerminals;
    for (const QModelIndex &index : checkedIndexes) {
        QString itemText = model->data(index, Qt::DisplayRole).toString();
        QStringList parts = itemText.split('|');

        if (parts.size() == 2) {
            QString terminalID = parts[0];
            selectedTerminals.append(terminalID);
        }
    }

    qDebug() << "Selected Terminals: " << selectedTerminals.join(", ");
}





void PassManagerDialog::on_toolButtonAdd_clicked()
{
    EditPassExcepDialog *editPass = new EditPassExcepDialog(0);
    editPass->exec();
}

