#include "listazsdialog.h"
#include "ui_listazsdialog.h"
#include "Terminals/editworkplacedialog.h"
#include "LogginCategories/loggincategories.h"

#include <QSqlRelationalDelegate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

ListAzsDialog::ListAzsDialog(const QMap<int, QSharedPointer<TermData> > &terminalMap, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListAzsDialog), terminalMap(terminalMap)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

ListAzsDialog::~ListAzsDialog()
{
    delete ui;
}



void ListAzsDialog::createUI()
{
    ui->tableViewTerminals->setModel(modelTerminals);
    ui->tableViewTerminals->resizeColumnsToContents();
    ui->tableViewTerminals->horizontalHeader()->setStretchLastSection(true);

    // Отримати модель вибору
    QItemSelectionModel *selectionModel = ui->tableViewTerminals->selectionModel();

    // Створити діапазон вибору для першого елемента (0-й рядок)
    QItemSelection selection(modelTerminals->index(0, 0), modelTerminals->index(0, modelTerminals->columnCount() - 1));

    // Встановити вибір для першого рядка
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    // Зробити перший індекс видимим у списку
    ui->tableViewTerminals->scrollTo(modelTerminals->index(0, 0, QModelIndex()), QAbstractItemView::PositionAtTop);

    // Підключення слоту для обробки зміни поточного рядка в QTableView для терміналів
    connect(ui->tableViewTerminals->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ListAzsDialog::onTerminalSelectionChanged);

    ui->splitter->setStyleSheet(
        "QSplitter::handle:vertical { background-color: #666666; border: 1px solid #000000; }"
        "QSplitter::handle:horizontal { background-color: #666666; border: 1px solid #000000; }"
        );
}

void ListAzsDialog::createModel()
{
    modelTerminals = new TerminalListModel(terminalMap, this);
    modelWorkplace = new QSqlRelationalTableModel(this);
    modelWorkplace->setTable("WORKPLACE");
    modelWorkplace->setRelation(modelWorkplace->fieldIndex("VERSION_TYPE"), QSqlRelation("VERSION_TYPE", "VERSION_TYPE_ID", "VERSION_NAME"));
    modelWorkplace->select();
}

void ListAzsDialog::updateWorkplaceModel(int terminalID)
{
    // Оновіть дані у моделі WORKPLACE
    modelWorkplace->setTable("WORKPLACE");
    modelWorkplace->setFilter(QString("TERMINAL_ID = %1").arg(terminalID));

    // Додайте відношення для імені версії
    int versionTypeColumnIndex = modelWorkplace->fieldIndex("VERSION_TYPE");
    modelWorkplace->setRelation(versionTypeColumnIndex, QSqlRelation("VERSION_TYPE", "VERSION_TYPE_ID", "VERSION_NAME"));

    // Знову викликайте select, оскільки змінили фільтр
    modelWorkplace->select();

    // Оновлення заголовків стовбців
    modelWorkplace->setHeaderData(1, Qt::Horizontal, tr("Терминал"));
    modelWorkplace->setHeaderData(2, Qt::Horizontal, tr("АРМ"));
    modelWorkplace->setHeaderData(3, Qt::Horizontal, tr("POS ID"));
    modelWorkplace->setHeaderData(4, Qt::Horizontal, tr("IP Адрес"));

    // Відображення данних у QTableView для робочих місць
    ui->tableViewWorkplace->setModel(modelWorkplace);
    ui->tableViewWorkplace->setItemDelegate(new QSqlRelationalDelegate(ui->tableViewWorkplace));

    // Приховання непотрібних стовбців
    ui->tableViewWorkplace->hideColumn(0);
    ui->tableViewWorkplace->hideColumn(5);
    ui->tableViewWorkplace->hideColumn(6);
    ui->tableViewWorkplace->resizeColumnsToContents();

    // Отримати модель вибору
    QItemSelectionModel *selectionModel = ui->tableViewWorkplace->selectionModel();

    // Створити діапазон вибору для першого елемента (0-й рядок)
    QItemSelection selection(modelWorkplace->index(0, 0), modelWorkplace->index(0, modelWorkplace->columnCount() - 1));

    // Встановити вибір для першого рядка
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    // Зробити перший індекс видимим у списку
    ui->tableViewWorkplace->scrollTo(modelWorkplace->index(0, 0, QModelIndex()), QAbstractItemView::PositionAtTop);
}


void ListAzsDialog::onTerminalSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid()) {
        // Отримати значення terminalID з моделі терміналів
        int selectedTerminalID = modelTerminals->data(modelTerminals->index(current.row(), 0)).toInt();

        // Викликати функцію для оновлення моделі
        updateWorkplaceModel(selectedTerminalID);
    }
}

void ListAzsDialog::on_toolButtonAdd_clicked()
{
    // Отримати об'єкт моделі вибору з таблиці терміналів
    QItemSelectionModel *selectionModel = ui->tableViewTerminals->selectionModel();

    // Отримати вибраний рядок (або перший вибраний рядок, якщо є багато)
    QModelIndexList selectedIndexes = selectionModel->selectedRows();

    // Перевірка чи є взагалі вибрані рядки
    if (!selectedIndexes.isEmpty()) {
        // Отримати значення з першої ячейки першого вибраного рядка
        int selectedTerminalID = modelTerminals->data(selectedIndexes.first().siblingAtColumn(0)).toInt();

        // Тепер ви можете використати selectedTerminalID за необхідністю
        EditWorkplaceDialog *wrpDlg = new EditWorkplaceDialog(0, selectedTerminalID);
        if(wrpDlg->exec() == QDialog::Accepted) {
            updateWorkplaceModel(selectedTerminalID);
        }

    } else {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбран терминал для добаления рабочего места."));
    }
}


void ListAzsDialog::on_tableViewWorkplace_doubleClicked(const QModelIndex &index)
{
    int selectedWorkplaceID = modelWorkplace->data(index.siblingAtColumn(0)).toInt();
    int selectedTerminalID = modelWorkplace->data(index.siblingAtColumn(1)).toInt();



    // Тепер ви можете використати selectedTerminalID за необхідністю
    EditWorkplaceDialog *wrpDlg = new EditWorkplaceDialog(selectedWorkplaceID, selectedTerminalID);
    if(wrpDlg->exec() == QDialog::Accepted) {
        updateWorkplaceModel(selectedTerminalID);
    }
}


void ListAzsDialog::on_toolButtonDelete_clicked()
{
    QModelIndex currentIndex = ui->tableViewWorkplace->currentIndex();
    if(currentIndex.isValid()){
        int removeWorkplaceID = modelWorkplace->data(currentIndex.siblingAtColumn(0)).toInt();
        QSqlQuery q;
        q.prepare("DELETE FROM workplace w WHERE w.workplace_id = :workplaceID");
        q.bindValue(":workplaceID", removeWorkplaceID);
        if(!q.exec()){
            qCritical(logCritical()) << "Ошибка удаления рабочего места" << q.lastError().text();
            MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось удалить рабочее место для терминала "),
                                        "", q.lastError().text());
        } else {
            qInfo(logInfo()) << tr("Рабочее место успешно удалено.");
            // Отримати об'єкт моделі вибору з таблиці терміналів
            QItemSelectionModel *selectionModel = ui->tableViewTerminals->selectionModel();

            // Отримати вибраний рядок (або перший вибраний рядок, якщо є багато)
            QModelIndexList selectedIndexes = selectionModel->selectedRows();
            if (!selectedIndexes.isEmpty()) {
                // Отримати значення з першої ячейки першого вибраного рядка
                int selectedTerminalID = modelTerminals->data(selectedIndexes.first().siblingAtColumn(0)).toInt();
                updateWorkplaceModel(selectedTerminalID);
            }
        }
    } else {
        QMessageBox::information(this, tr("Внимание"), tr("Не выбрано рабочее место для удаления!"));
    }

}


void ListAzsDialog::on_buttonBox_rejected()
{
    this->reject();
}

