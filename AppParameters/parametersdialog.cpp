#include "parametersdialog.h"
#include "ui_parametersdialog.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersDialog)
{
    ui->setupUi(this);
    createUI();
}

ParametersDialog::~ParametersDialog()
{
    delete ui;
}

void ParametersDialog::slotOpenClentVNC()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл TightVNC клиента"), "",
                                                    tr("Приложение (*.exe);;Все файлы (*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    if(!fileName.isEmpty())
        ui->lineEditClientTightVNC->setText(fileName);
}

void ParametersDialog::slotOpenClentVNCUltra()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл UltraVNC клиента"), "",
                                                    tr("Приложение (*.exe);;Все файлы (*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    if(!fileName.isEmpty())
        ui->lineEditClientUltraVNC->setText(fileName);
}

void ParametersDialog::slotOpenMPosDBFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл базы данных"), "",
                                                    tr("База данных (*.gdb *.fdb);;Все файлы (*.*)"),nullptr,QFileDialog::DontUseNativeDialog);
    if(!fileName.isEmpty())
        ui->lineEditDBFileName->setText(fileName);
}

void ParametersDialog::createUI()
{
    CriptPass crP;
    // Параметри підключення до бази даних МПОС
    ui->lineEditServerName->setText(AppParameters::instance().getParameter("mposHostName"));
    ui->spinBoxPort->setValue(AppParameters::instance().getParameter("mposPort").toInt());
    QAction *openFileMPosAction = new QAction(QIcon(":/Images/file_open_icon.png"), tr("Открыть файл"), this);
    connect(openFileMPosAction,&QAction::triggered,this,&ParametersDialog::slotOpenMPosDBFile);
    ui->lineEditDBFileName->addAction(openFileMPosAction, QLineEdit::TrailingPosition);
    ui->lineEditDBFileName->setText(AppParameters::instance().getParameter("mposDB"));
    ui->lineEditUser->setText(AppParameters::instance().getParameter("mposUser"));

    ui->lineEditPass->setText(crP.decriptPass(AppParameters::instance().getParameter("mposPass")));
    QAction *showPasswordAction = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показать пароль"), this);
        // Налаштовуємо обробник події активації QAction
    connect(showPasswordAction, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPass->setEchoMode(ui->lineEditPass->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    // Додаємо QAction до QLineEdit у кінцевій позиції
    ui->lineEditPass->addAction(showPasswordAction, QLineEdit::TrailingPosition);
    // Вкладка Общие
    ui->lineEditMinTemID->setText(AppParameters::instance().getParameter("minTerminalID"));
    ui->lineEditMaxTermID->setText(AppParameters::instance().getParameter("maxTerminalID"));
    ui->checkBoxShowKodZem->setChecked(AppParameters::instance().getParameter("showKodZem").toInt());
    ui->spinBoxMaxPC->setValue(AppParameters::instance().getParameter("maxCountPC").toInt());
    ui->lineEditHotlinePhone->setText(AppParameters::instance().getParameter("hotlinePhone"));
    ui->lineEditPassFBTerminals->setText(crP.decriptPass(AppParameters::instance().getParameter("passFBTerminal")));
    // Вивести TEMPLATE_HOSTNAME
    QMap<int, QString> templateHostnameMap = AppParameters::instance().TEMPLATE_HOSTNAME;
    for (auto it = templateHostnameMap.begin(); it != templateHostnameMap.end(); ++it) {
//        qDebug() << "Index:" << it.key() << "Value:" << it.value();
        ui->comboBoxHostTemplate->addItem(it.value(), QVariant(it.key()));
    }
    ui->comboBoxHostTemplate->setCurrentIndex(AppParameters::instance().getParameter("templatеHostname").toInt());
// VNC options
    if (AppParameters::instance().getParameter("typeVNC") == "TightVNC") {
        ui->radioButtonTightVNC->setChecked(true);
     } else  {
        ui->radioButtonUltraVNC->setChecked(true);
    }
    QAction *openVNCFileAction = new QAction(QIcon(":/Images/file_open_icon.png"), tr("Открыть файл"), this);
    connect(openVNCFileAction, &QAction::triggered,this,&ParametersDialog::slotOpenClentVNC);
    ui->lineEditClientTightVNC->addAction(openVNCFileAction, QLineEdit::TrailingPosition);
    ui->lineEditClientTightVNC->setText(AppParameters::instance().getParameter("clientVNCPath"));

    QAction *openUltraVNCFileAction = new QAction(QIcon(":/Images/file_open_icon.png"), tr("Открыть файл"), this);
    connect(openUltraVNCFileAction,&QAction::triggered,this,&ParametersDialog::slotOpenClentVNCUltra);
    ui->lineEditClientUltraVNC->addAction(openUltraVNCFileAction, QLineEdit::TrailingPosition);
    ui->lineEditClientUltraVNC->setText(AppParameters::instance().getParameter("clientUltraVNCPath"));

    ui->lineEditPassDefault->setText(crP.decriptPass(AppParameters::instance().getParameter("defaultVNCPass")));
    QAction *showPasswordVNCAction = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показать пароль"), this);
    connect(showPasswordVNCAction, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPassDefault->setEchoMode(ui->lineEditPassDefault->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    ui->lineEditPassDefault->addAction(showPasswordVNCAction, QLineEdit::TrailingPosition);

    ui->groupBoxTemplatePass->setChecked(AppParameters::instance().getParameter("useTemplatePassVNC").toInt());
    QMap<int, QString> templateVNCPassMap = AppParameters::instance().TEMPLATE_VNC_PASS;
    for(auto it = templateVNCPassMap.begin(); it != templateVNCPassMap.end(); ++it){
        ui->comboBoxTemplatePass->addItem(it.value(), QVariant(it.key()));
    }
    if(!ui->groupBoxTemplatePass->isChecked()){
        ui->comboBoxTemplatePass->setCurrentIndex(-1);
    } else {
        ui->comboBoxTemplatePass->setCurrentIndex(AppParameters::instance().getParameter("templatеVNCPass").toInt());
    }
    ui->lineEditPrefix->setText(AppParameters::instance().getParameter("TemplPassPrefix"));
    ui->lineEditSufix->setText(AppParameters::instance().getParameter("TemplPassSufix"));
    createTemplatePass();
}

void ParametersDialog::createConnections()
{

}

void ParametersDialog::on_buttonBox_rejected()
{
    this->reject();
}


void ParametersDialog::on_buttonBox_accepted()
{
    CriptPass crP;
    // Зміна параметрів та оновлення ъх у БД
    AppParameters::instance().setParameter("mposHostName",ui->lineEditServerName->text().trimmed());
    updateParam("mposHostName");
    AppParameters::instance().setParameter("mposPort",QString::number(ui->spinBoxPort->value()));
    updateParam("mposPort");
    AppParameters::instance().setParameter("mposDB",ui->lineEditDBFileName->text().trimmed());
    updateParam("mposDB");
    AppParameters::instance().setParameter("mposUser",ui->lineEditUser->text().trimmed());
    updateParam("mposUser");

    AppParameters::instance().setParameter("mposPass",crP.criptPass(ui->lineEditPass->text().trimmed()));
    updateParam("mposPass");
    // Зміна параметрів вкладки Загалні
    AppParameters::instance().setParameter("minTerminalID",ui->lineEditMinTemID->text().trimmed());
    updateParam("minTerminalID");
    AppParameters::instance().setParameter("maxTerminalID",ui->lineEditMaxTermID->text().trimmed());
    updateParam("maxTerminalID");
    AppParameters::instance().setParameter("showKodZem",QString::number(ui->checkBoxShowKodZem->isChecked()));
    updateParam("showKodZem");
    AppParameters::instance().setParameter("maxCountPC",QString::number(ui->spinBoxMaxPC->value()));
    updateParam("maxCountPC");
    AppParameters::instance().setParameter("templatеHostname", QString::number(ui->comboBoxHostTemplate->currentIndex()));
    updateParam("templatеHostname");

    if(ui->radioButtonTightVNC->isChecked()){
        AppParameters::instance().setParameter("typeVNC","TightVNC");
    } else {
        AppParameters::instance().setParameter("typeVNC","UltraVNC");
    }
    updateParam("typeVNC");
    AppParameters::instance().setParameter("clientVNCPath", ui->lineEditClientTightVNC->text().trimmed());
    updateParam("clientVNCPath");
    AppParameters::instance().setParameter("clientUltraVNCPath", ui->lineEditClientUltraVNC->text().trimmed());
    updateParam("clientUltraVNCPath");
    AppParameters::instance().setParameter("defaultVNCPass",crP.criptPass(ui->lineEditPassDefault->text().trimmed()));
    updateParam("defaultVNCPass");
    AppParameters::instance().setParameter("useTemplatePassVNC",QString::number(ui->groupBoxTemplatePass->isChecked()));
    updateParam("useTemplatePassVNC");
    AppParameters::instance().setParameter("templatеVNCPass", QString::number(ui->comboBoxTemplatePass->currentIndex()));
    updateParam("templatеVNCPass");
    AppParameters::instance().setParameter("TemplPassPrefix", ui->lineEditPrefix->text().trimmed());
    updateParam("TemplPassPrefix");
    AppParameters::instance().setParameter("TemplPassSufix", ui->lineEditSufix->text().trimmed());
    updateParam("TemplPassSufix");
    AppParameters::instance().setParameter("hotlinePhone", ui->lineEditHotlinePhone->text().trimmed());
    updateParam("hotlinePhone");
    AppParameters::instance().setParameter("passFBTerminal", crP.criptPass(ui->lineEditPassFBTerminals->text().trimmed()));
    updateParam("passFBTerminal");

    this->accept();
}

// Функція для оновлення або додавання параметрів в базу даних
void ParametersDialog::updateOrInsertParameter(const QString& paramName, const QString& paramValue) {
    QSqlQuery query;

    // Використовуємо інструкцію UPDATE OR INSERT
    QString queryString = "UPDATE OR INSERT INTO PARAMETERS (param_name, param_value) VALUES (:paramName, :paramValue) MATCHING (param_name)";

    query.prepare(queryString);
    query.bindValue(":paramName", paramName);
    query.bindValue(":paramValue", paramValue);

    if (query.exec()) {
        qInfo(logInfo()) << QApplication::tr("Параметр %1 успішно оновлено або додано.").arg(paramName);
    } else {
        qWarning(logWarning()) << QApplication::tr("Помилка при оновленні або додаванні параметра %1.").arg(paramName);
    }
}

// Функція для оновлення параметра
void ParametersDialog::updateParam(const QString& paramName)
{
    QString paramValue = AppParameters::instance().getParameter(paramName);
    updateOrInsertParameter(paramName, paramValue);
}



void ParametersDialog::on_comboBoxHostTemplate_activated(int index)
{
    // Отримати об'єкт QVariant для вибраного елемента
    QVariant selectedData = ui->comboBoxHostTemplate->itemData(index);

    // Перевірити, чи отримано правильне значення
    if (selectedData.isValid()) {
        AppParameters::instance().setParameter("templatеHostname", selectedData.toString());
    }
}


void ParametersDialog::on_comboBoxTemplatePass_activated(int index)
{
    QVariant selectedData = ui->comboBoxTemplatePass->itemData(index);
    if(selectedData.isValid()){
        AppParameters::instance().setParameter("templatеVNCPass", selectedData.toString());
    }
}


void ParametersDialog::on_groupBoxTemplatePass_toggled(bool checked)
{
    if(checked) {
        ui->comboBoxTemplatePass->setCurrentIndex(AppParameters::instance().getParameter("templatеVNCPass").toInt());
    } else {
        ui->comboBoxTemplatePass->setCurrentIndex(-1);
    }
}


void ParametersDialog::on_lineEditTermIDTemplate_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    createTemplatePass();
}

void ParametersDialog::createTemplatePass()
{
    QString templatePass;
    QString termID = ui->lineEditTermIDTemplate->text().trimmed();
    templatePass = ui->lineEditPrefix->text().trimmed();
    if(termID.length() == 0) {
        termID="XXXXX";
    } else {
        termID = termID.rightJustified(5,'0');
    }
    templatePass = templatePass+termID+ui->lineEditSufix->text().trimmed();
    ui->labelPassSample->setText(templatePass);
}
void ParametersDialog::on_lineEditPrefix_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    createTemplatePass();
}


void ParametersDialog::on_lineEditSufix_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    createTemplatePass();
}

