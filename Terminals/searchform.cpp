#include "searchform.h"
#include "ui_searchform.h"
#include <QIntValidator>

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
    createUI();

}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::createUI()
{
    ui->lineEditSearch->addAction(QIcon(":/Images/search_icon.png"), QLineEdit::LeadingPosition);
    QIntValidator* validator = new QIntValidator(ui->lineEditSearch);
    ui->lineEditSearch->setValidator(validator);
}

void SearchForm::on_lineEditSearch_returnPressed()
{
    emit signalSendSearchTerminalID(ui->lineEditSearch->text().trimmed().toInt());
    ui->lineEditSearch->clear();
}

