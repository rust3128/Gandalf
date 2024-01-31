#include "editworkplacedialog.h"
#include "ui_editworkplacedialog.h"
#include "LogginCategories/loggincategories.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

EditWorkplaceDialog::EditWorkplaceDialog(int wID, int termID, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditWorkplaceDialog)
    , workplaceID(wID)
    , terminalID(termID)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

EditWorkplaceDialog::~EditWorkplaceDialog()
{
    delete ui;
}

void EditWorkplaceDialog::createUI()
{

    ui->lineEditTerminalID->setText(QString::number(terminalID));
    ui->comboBoxVersionType->setModel(modelMposType);
    ui->comboBoxVersionType->setModelColumn(1);
    if(workplaceID == 0) {
        this->setWindowTitle(tr("Новое рабочее место"));
        ui->comboBoxVersionType->setCurrentIndex(-1);
    } else {
        QSqlQuery q;
        q.prepare("select w.version_type, w.pos_id, w.ipadr, w.passvnc, w.portvnc from workplace w where w.workplace_id = :worplaceID");
        q.bindValue(":worplaceID", workplaceID);
        if(!q.exec()){
            qCritical(logCritical()) << tr("Не удалось получить данные о рабочем местe.");
            MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось получить данные о рабочем местe. ")+QString::number(terminalID),
                                        "", q.lastError().text());
            this->reject();
        } else {
            q.next();
            int versionTypeId = q.value(0).toInt();
            for (int row = 0; row < modelMposType->rowCount(); ++row) {
                int currentVersionTypeId = modelMposType->data(modelMposType->index(row, 0)).toInt();
                if (currentVersionTypeId == versionTypeId) {
                    ui->comboBoxVersionType->setCurrentIndex(row);
                    break;
                }
            }
            ui->spinBoxPosID->setValue(q.value(1).toInt());
            ui->lineEditIP->setText(q.value(2).toString());
            ui->lineEditPassVNC->setText(q.value(3).toString());
            ui->spinBoxPort->setValue(q.value(4).toInt());
        }
    }
}

void EditWorkplaceDialog::createModel()
{
    modelMposType = new QSqlQueryModel();
    modelMposType->setQuery("select v.version_type_id, v.version_name from version_type v");


}

void EditWorkplaceDialog::on_buttonBox_rejected()
{
    this->reject();
}


void EditWorkplaceDialog::on_buttonBox_accepted()
{
    int versionTypeIdInt = 0;
    // Отримати індекс обраного елемента
    int selectedIndex = ui->comboBoxVersionType->currentIndex();

    // Впевнитися, що індекс є в межах розміру моделі
    if (selectedIndex >= 0 && selectedIndex < modelMposType->rowCount()) {
        QVariant versionTypeId = modelMposType->data(modelMposType->index(selectedIndex, 0));
        versionTypeIdInt = versionTypeId.toInt();
    } else {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не верный тип версии МПос."));
        return;
    }
    if(terminalID != ui->lineEditTerminalID->text().trimmed().toInt()){
        QMessageBox::critical(this, tr("Ошибка"), tr("Не верный номер терминала."));
        return;
    }
    QString hostIP = ui->lineEditIP->text().trimmed();
    if(hostIP.length() ==8) {
        QMessageBox::critical(this, tr("Ошибка"), tr("IP-адрес или имя хоста поустое."));
        return;
    }
    QString passVNC = ui->lineEditPassVNC->text().trimmed();
    if(passVNC.length() !=8) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не верный пароль VNC.\nДлина пароля - 8 символов."));
        return;
    }
    QSqlQuery q;
    if(workplaceID == 0){
        q.prepare("INSERT INTO WORKPLACE (TERMINAL_ID, VERSION_TYPE, POS_ID, IPADR, PASSVNC, PORTVNC) "
                  "VALUES (:terminal_id, :version_type, :pos_id, :ipadr, :passvnc, :portvnc)");

        q.bindValue(":terminal_id", terminalID);
        q.bindValue(":version_type", versionTypeIdInt);
        q.bindValue(":pos_id", ui->spinBoxPosID->value());
        q.bindValue(":ipadr", hostIP);
        q.bindValue(":passvnc", passVNC);
        q.bindValue(":portvnc", ui->spinBoxPort->value());

        if (q.exec()) {
            qInfo(logInfo()) << tr("Рабочее место успешно добавлено");
            this->accept();
        } else {
            // Помилка виконання запиту
            qCritical(logCritical()) << "Ошибка добавления рабочего места" << q.lastError().text();
            MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось добавить рабочее место для терминала ")+QString::number(terminalID),
                                        "", q.lastError().text());

        }
    } else {
        q.prepare("UPDATE WORKPLACE SET VERSION_TYPE = :version_type, POS_ID = :pos_id, IPADR = :ipadr, PASSVNC = :passvnc, PORTVNC =:portvnc "
                  "WHERE (WORKPLACE_ID = :workplaceID)");
        q.bindValue(":version_type", versionTypeIdInt);
        q.bindValue(":pos_id", ui->spinBoxPosID->value());
        q.bindValue(":ipadr", hostIP);
        q.bindValue(":passvnc", passVNC);
        q.bindValue(":portvnc", ui->spinBoxPort->value());
        q.bindValue(":workplaceID", workplaceID);
        if (q.exec()) {
            qInfo(logInfo()) << tr("Рабочее место успешно обновлено");
            this->accept();
        } else {
            // Помилка виконання запиту
            qCritical(logCritical()) << "Ошибка обновления рабочего места" << q.lastError().text();
            MyMessage::showNotification(QMessageBox::Critical,tr("Ошибка"),tr("Не удалось обновить рабочее место для терминала ")+QString::number(terminalID),
                                        "", q.lastError().text());

        }
    }
}

