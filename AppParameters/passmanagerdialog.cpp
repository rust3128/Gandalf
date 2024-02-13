#include "passmanagerdialog.h"
#include "ui_passmanagerdialog.h"
#include "LogginCategories/loggincategories.h"
#include <AppParameters/editpassexcepdialog.h>
#include "AppParameters/impostpassvncdialog.h"

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

    // Отримати індекс для першого елемента (0-й рядок, 0-й стовпець)
    QModelIndex firstIndex = model->index(0, 0);

    // Отримати модель вибору
    QItemSelectionModel *selectionModel = ui->listView->selectionModel();

    // Створити діапазон вибору для першого елемента
    QItemSelection selection(firstIndex, firstIndex);

    // Встановити діапазон вибору для моделі вибору
    selectionModel->select(selection, QItemSelectionModel::Select);

    // Зробити цей індекс видимим у списку
    ui->listView->scrollTo(firstIndex, QAbstractItemView::PositionAtTop);

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

void PassManagerDialog::handleItemAction(const QModelIndex &index)
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
            EditPassExcepDialog *editPass = new EditPassExcepDialog(terminalId);
            editPass->exec();
        } else {
            qDebug() << "Неможливо конвертувати в числове значення:" << terminalIdString;
        }
    }
}

void PassManagerDialog::on_listView_doubleClicked(const QModelIndex &index)
{
    handleItemAction(index);
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

    if (!selectedTerminals.isEmpty()) {
        QSqlQuery query;

        QString placeholders = selectedTerminals.join(", ");
        QString queryString = "DELETE FROM PASS_EXCEPTION_VNC WHERE TERMINAL_ID IN (" + placeholders + ")";

        query.prepare(queryString);

        if (!query.exec()) {
            qCritical(logCritical()) << "Ошибка удаления паролей VNC " << query.lastError().text();
            // Обробляйте помилку видалення за необхідності
        } else {
            qDebug() << "Записи успішно видалено з таблиці.";
            model->deleteLater();
            createModel();
            createUI();
        }
    } else {
        QMessageBox::information(this, tr("Внимание"), tr("Список терминалов для удаления пуст"));
        qDebug() << "Список вибраних терміналів порожній.";
    }

}

void PassManagerDialog::on_toolButtonAdd_clicked()
{
    EditPassExcepDialog *editPass = new EditPassExcepDialog(0);
    if(editPass->exec() == QDialog::Accepted) {
        model->deleteLater();
        createModel();
        createUI();
    }
}


void PassManagerDialog::on_toolButtonEdit_clicked()
{
    QModelIndex currentIndex = ui->listView->currentIndex();
    if (currentIndex.isValid()) {
        handleItemAction(currentIndex);
    } else {
        QMessageBox::information(this, tr("Ошибка"), tr("Не выбрана АЗС для редактирования!"));
        qDebug() << "Обробка кнопки редагування: вибраного елемента немає.";
    }
}


void PassManagerDialog::on_toolButton_clicked()
{
    ImpostPassVNCDialog *impDlg = new ImpostPassVNCDialog();
    impDlg->exec();
    model->deleteLater();
    createModel();
    createUI();
}


void PassManagerDialog::on_toolButton_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Внимание!"), tr("Вы действительно хотите убрать все исключения для паролей?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM PASS_EXCEPTION_VNC");
        if (query.exec()) {
            // Вдало видалили дані з бази даних
            qDebug() << "Дані успішно видалені з таблиці PASS_EXCEPTION_VNC";
            model->deleteLater();
            createModel();
            createUI();
        } else {
            // Виникла помилка при видаленні
            qDebug() << "Помилка видалення даних з таблиці PASS_EXCEPTION_VNC:" << query.lastError().text();
        }

    }

}


void PassManagerDialog::on_buttonBox_rejected()
{
    this->reject();
}

