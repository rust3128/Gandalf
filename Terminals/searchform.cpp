#include "searchform.h"
#include "ui_searchform.h"

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
}

void SearchForm::on_lineEditSearch_returnPressed()
{
    emit signalSendSearchTerminalID(ui->lineEditSearch->text().trimmed().toInt());
}

