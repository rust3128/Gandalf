// objectform.cpp
#include "objectform.h"
#include "ui_objectform.h"
#include "AppParameters/AppParameters.h"

#include <QClipboard>

ObjectForm::ObjectForm(QSharedPointer<TermData> tData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectForm),
    m_termData(tData)
{
    ui->setupUi(this);
    createUI();
}

ObjectForm::~ObjectForm()
{
    delete ui;
}

void ObjectForm::createUI()
{
    ui->labelAZSNumber->setText(("АЗС № "+QString::number(m_termData->getTerminalID())));
    if(AppParameters::instance().getParameter("showKodZem").toInt()){
        ui->labelAddress->setText(m_termData->getAdress()+tr("\nКод земельного участка: ")+QString::number(m_termData->getOwnerID()));
    } else {
        ui->labelAddress->setText(m_termData->getAdress())    ;
    }
    ui->labelPhone->setText(m_termData->getPhone());
}

void ObjectForm::on_toolButtonClipboard_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipTitle = ui->labelAZSNumber->text()+"\n"+ui->labelAddress->text()+"\n"+ui->labelPhone->text();
    clipboard->setText(clipTitle);
}

