#include "listazsdialog.h"
#include "ui_listazsdialog.h"

#include <QSqlRelationalDelegate>

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

void ListAzsDialog::onTerminalSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid()) {
        // Отримати значення terminalID з моделі терміналів
        int selectedTerminalID = modelTerminals->data(modelTerminals->index(current.row(), 0)).toInt();

        // Оновіть дані у моделі WORKPLACE
        modelWorkplace->setTable("WORKPLACE");
        modelWorkplace->setFilter(QString("TERMINAL_ID = %1").arg(selectedTerminalID));

        // Додайте відношення для імені версії
        int versionTypeColumnIndex = modelWorkplace->fieldIndex("VERSION_TYPE");
        modelWorkplace->setRelation(versionTypeColumnIndex, QSqlRelation("VERSION_TYPE", "VERSION_TYPE_ID", "VERSION_NAME"));

        // Знову викликайте select, оскільки змінили фільтр
        modelWorkplace->select();

        // Встановлення назв стовбців
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
    }
}

void ListAzsDialog::on_toolButtonAdd_clicked()
{

}

