// TerminalListDialog.cpp
#include "TerminalListDialog.h"
#include "ui_TerminalListDialog.h"
#include "LogginCategories/loggincategories.h"

TerminalListDialog::TerminalListDialog(const QMap<int, QSharedPointer<TermData>>& terminalMap, QWidget *parent)
    : QDialog(parent), ui(new Ui::TerminalListDialog), terminalMap(terminalMap)
{
    ui->setupUi(this);
    // Тепер можна використовувати terminalMap у вашому діалозі, наприклад, відображати його у віджеті або зберігати внутрішню копію.
    createUI();
}

TerminalListDialog::~TerminalListDialog()
{
    delete ui;
}

void TerminalListDialog::createUI()
{
    qInfo(logInfo()) << terminalMap[1001]->getNameAZS();
    terminalModel = new TerminalListModel(terminalMap, this);
    ui->tableViewTerminals->setModel(terminalModel);
}
