// objectform.cpp
#include "objectform.h"
#include "ui_objectform.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"


#include <QClipboard>
#include <QToolTip>


ObjectForm::ObjectForm(QSharedPointer<TermData> tData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectForm),
    m_termData(tData)
{
    ui->setupUi(this);
    createConnList();
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
    ui->lineEditPingAdress->setText(connList[0]->getHostName());
}

void ObjectForm::createConnList()
{
    switch (AppParameters::instance().getParameter("templatеHostname").toInt()) {
    case 0:
        //Avias
        connListAvias();
        break;
    default:
        break;
    }
    for (const auto &connectionData : connList) {
        qDebug() << "Terminal ID:" << connectionData->getTernID();
        qDebug() << "Host Name:" << connectionData->getHostName();
        qDebug() << "Port:" << connectionData->getPort();
        qDebug() << "Pass VNC:" << connectionData->getPassVNC();
    }
}

void ObjectForm::connListAvias()
{
    qInfo(logInfo()) << "Generate Avias. Term ID" << m_termData->getTerminalID();
    int termID=m_termData->getTerminalID();
    for(int i=0; i<AppParameters::instance().getParameter("maxCountPC").toInt(); ++i){
        QSharedPointer<ConnectionData> conTemp = QSharedPointer<ConnectionData>::create(termID);
        QString tempHostname = "azs" + QString::number(m_termData->getOwnerID()) + ".unit.local";
        conTemp->setHostName(tempHostname);
        conTemp->setPort(15901 + i);
        conTemp->setPassVNC(getVNCPassword());
        connList.append(conTemp);
    }
    return;
}

QString ObjectForm::getVNCPassword()
{
    CriptPass crP;
    return crP.decriptPass(AppParameters::instance().getParameter("defaultVNCPass"));
}

void ObjectForm::on_toolButtonClipboard_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipTitle = ui->labelAZSNumber->text()+"\n"+ui->labelAddress->text()+"\n"+ui->labelPhone->text();
    clipboard->setText(clipTitle);
    QToolTip::showText(ui->toolButtonClipboard->mapToGlobal(ui->toolButtonClipboard->rect().center()), tr("Адрес скопирован\nв буфер обмена."));
}


void ObjectForm::on_toolButtonAdres2Clip_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->lineEditPingAdress->text().trimmed());
    QToolTip::showText(ui->toolButtonAdres2Clip->mapToGlobal(ui->toolButtonAdres2Clip->rect().center()), tr("Адрес скопирован\nв буфер обмена."));
}

