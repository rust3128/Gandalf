#include "editpassexcepdialog.h"
#include "ui_editpassexcepdialog.h"

EditPassExcepDialog::EditPassExcepDialog(int termID, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPassExcepDialog)
    , m_terminalID(termID)
{
    ui->setupUi(this);
    createConnections();
    createUI();
}

EditPassExcepDialog::~EditPassExcepDialog()
{
    delete ui;
}

void EditPassExcepDialog::passChanged()
{
    QString text1 = ui->lineEditPass1->text();
    QString text2 = ui->lineEditPass2->text();

    if (text1 == text2) {
        ui->labelInfo->hide();
        // Викликати інші дії або обробку тексту, якщо потрібно
    } else {
        ui->labelInfo->show();
        // Викликати інші дії або обробку тексту, якщо потрібно
    }
}

void EditPassExcepDialog::createUI()
{
    ui->labelInfo->hide();
    if(m_terminalID != 0){
        ui->lineEditTerminalID->setReadOnly(true);
    }
}

void EditPassExcepDialog::createConnections()
{
    connect(ui->lineEditPass1, &QLineEdit::textChanged, this, &EditPassExcepDialog::passChanged);
    connect(ui->lineEditPass2, &QLineEdit::textChanged, this, &EditPassExcepDialog::passChanged);
}
