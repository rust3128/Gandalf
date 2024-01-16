#include "impostpassvncdialog.h"
#include "ui_impostpassvncdialog.h"

#include "AppParameters/criptpass.h"

#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

ImpostPassVNCDialog::ImpostPassVNCDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImpostPassVNCDialog)
    , model(new QStandardItemModel(this)),
    selectedDelimiter(";")
{
    ui->setupUi(this);
    createUI();
}

ImpostPassVNCDialog::~ImpostPassVNCDialog()
{
    delete ui;
}

void ImpostPassVNCDialog::slotOpenCSVFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл импорта паролей VNC клиента"), "",
                                                    tr("CSV файл (*.csv);;Все файлы (*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    if(!fileName.isEmpty()){
        ui->lineEditFileName->setText(fileName);
        loadCSV(fileName);
    }
}

void ImpostPassVNCDialog::createUI()
{
    QAction *openCSVFile = new QAction(QIcon(":/Images/file_open_icon.png"), tr("Открыть файл"), this);
    connect(openCSVFile,&QAction::triggered,this,&ImpostPassVNCDialog::slotOpenCSVFile);
    ui->lineEditFileName->addAction(openCSVFile, QLineEdit::TrailingPosition);

    // Додамо роздільники до комбо-бокса
    QStringList delimiters = {";", ",", "\t", " "};
    for (const QString& delimiter : delimiters) {
        QString displayText = delimiter;
        if (delimiter == "\t") {
            displayText = "Tab";
        } else if (delimiter == " ") {
            displayText = "Space";
        }
        ui->comboBoxDelimiter->addItem(displayText, delimiter);
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->progressBar->hide();
    ui->plainTextEditErrors->setVisible(false);
    ui->groupBoxData->hide();

}

void ImpostPassVNCDialog::loadCSV(const QString &filename)
{
    // Очищаємо модель перед завантаженням нових даних
    model->clear();
    ui->groupBoxData->hide();
    // Очищаємо QPlainTextEdit перед завантаженням нових даних
    ui->plainTextEditErrors->clear();

    QFile file(filename);
    bool hasErrors = false;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();

            // Використовуємо вже відповідно оброблений роздільник
            QStringList fields = line.split(selectedDelimiter);

            // Перевірка кількості полів
            if (fields.size() != 2) {
                QString errorText = tr("Невірна кількість полів у рядку:");
                ui->plainTextEditErrors->appendHtml("<font color='red'>" + errorText + "</font><br>" + line);
                hasErrors = true;
                continue;
            }

            QString terminal = fields.at(0);
            QString password = fields.at(1);

            // Перевірка, чи термінал є цілим числом у діапазоні від 100 до 99999
            bool isTerminalOk = false;
            int terminalValue = terminal.toInt(&isTerminalOk);
            if (!isTerminalOk || terminalValue < 100 || terminalValue > 99999) {
                QString errorText = tr("Невірний номер терміналу у рядку:");
                ui->plainTextEditErrors->appendHtml("<font color='red'>" + errorText + "</font><br>" + line );
                hasErrors = true;
                continue;
            }

            // Перевірка, чи пароль має довжину 8 символів
            if (password.length() != 8) {
                QString errorText = tr("Невірна довжина пароля у рядку:");
                ui->plainTextEditErrors->appendHtml("<font color='red'>" + errorText + "</font><br>" + line);
                hasErrors = true;
                continue;
            }

            // Всі перевірки пройдені, можна додати дані до моделі
            QList<QStandardItem*> rowItems;
            QStandardItem *terminalItem = new QStandardItem(terminal);
            QStandardItem *passwordItem = new QStandardItem(password);
            rowItems.append(terminalItem);
            rowItems.append(passwordItem);
            model->appendRow(rowItems);
        }

        file.close();

        // Перевірка, чи є дані для відображення
        if (model->rowCount() == 0) {
            hasErrors = true;
            ui->plainTextEditErrors->appendPlainText(tr("Немає даних для завантаження. Перевірте вміст початкового файлу або встановіть роздільник."));
        }
    } else {
        hasErrors = true;
        ui->plainTextEditErrors->appendPlainText(tr("Помилка відкриття файлу."));
    }

    // Встановлення видимості QPlainTextEdit в залежності від наявності помилок
    ui->plainTextEditErrors->setVisible(hasErrors);

    // Перевірка, чи є дані для відображення
    if (!hasErrors && model->rowCount() > 0) {
        // Встановлення заголовків стовбців
        model->setHorizontalHeaderLabels({tr("Термінал"), tr("Пароль")});
        // Показуємо QTableView після завантаження даних
        ui->groupBoxData->show();
    }
}


void ImpostPassVNCDialog::on_comboBoxDelimiter_textActivated(const QString &arg1)
{
    // Зберегти вибраний роздільник
    selectedDelimiter = arg1;

    // Перетворити "Tab" та "Space" на відповідні роздільники
    if (selectedDelimiter == "Tab") {
        selectedDelimiter = "\t";
    } else if (selectedDelimiter == "Space") {
        selectedDelimiter = " ";
    }

    // Перезавантажуємо дані при зміні роздільника
    if (!ui->lineEditFileName->text().isEmpty()) {
        loadCSV(ui->lineEditFileName->text());
    }
}


void ImpostPassVNCDialog::on_pushButtonImport_clicked()
{
    int rowCount = model->rowCount();
    ui->progressBar->show();
    // Перевірка, чи модель має дані
    if (rowCount == 0) {
        qDebug() << "Помилка: модель не має даних для імпорту";
        return;
    }
    ui->progressBar->setRange(0, rowCount);

    ui->progressBar->setValue(0);
    // Підготовка SQL-запиту
    QString sqlQuery = "UPDATE OR INSERT INTO PASS_EXCEPTION_VNC (TERMINAL_ID, PASSVNC) VALUES (?, ?) MATCHING (TERMINAL_ID);";

    // Підготовка SQL-запитувача
    QSqlQuery query;
    query.prepare(sqlQuery);
    CriptPass crP;
    // Перегляд усіх рядків у моделі
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex terminalIndex = model->index(row, 0); // індекс стовпця "Термінал"
        QModelIndex passwordIndex = model->index(row, 1); // індекс стовпця "Пароль"

        auto termialValue = terminalIndex.data().toString();
        auto passwordValue = crP.cryptVNCPass(termialValue,passwordIndex.data().toString());
        // Встановлення значень у параметри запиту
        query.bindValue(0, termialValue);
        query.bindValue(1, passwordValue);

        // Виконання SQL-запиту
        if (!query.exec()) {
            qDebug() << "Помилка SQL-запиту:" << query.lastError().text();
            // Ви можете вивести повідомлення про помилку або зробити інші дії в разі невдалого виконання запиту.
        }
        ui->progressBar->setValue(row +1);
    }
    this->accept();

}



void ImpostPassVNCDialog::on_buttonBox_rejected()
{
    this->reject();
}

