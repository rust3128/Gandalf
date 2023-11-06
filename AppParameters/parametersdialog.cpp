#include "parametersdialog.h"
#include "ui_parametersdialog.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"
#include "AppParameters/criptpass.h"

#include <QSqlQuery>
#include <QSqlError>

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

void ParametersDialog::createUI()
{
    // Параметри підключення до бази даних МПОС
    ui->lineEditServerName->setText(AppParameters::instance().getParameter("mposHostName"));
    ui->spinBoxPort->setValue(AppParameters::instance().getParameter("mposPort").toInt());
    ui->lineEditDBFileName->setText(AppParameters::instance().getParameter("mposDB"));
    ui->lineEditUser->setText(AppParameters::instance().getParameter("mposUser"));
    CriptPass crP;
    ui->lineEditPass->setText(crP.decriptPass(AppParameters::instance().getParameter("mposPass")));
    QAction *showPasswordAction = new QAction(QIcon(":/Images/eye_icon.png"), tr("Показать пароль"), this);

    // Налаштовуємо обробник події активації QAction
    connect(showPasswordAction, &QAction::triggered, [this] {
        // Змінюємо режим відображення паролю
        ui->lineEditPass->setEchoMode(ui->lineEditPass->echoMode() == QLineEdit::Password ? QLineEdit::Normal : QLineEdit::Password);
    });
    // Додаємо QAction до QLineEdit у кінцевій позиції
    ui->lineEditPass->addAction(showPasswordAction, QLineEdit::TrailingPosition);
}

void ParametersDialog::on_buttonBox_rejected()
{
    this->reject();
}


void ParametersDialog::on_buttonBox_accepted()
{
    // Зміна параметрів та оновлення ъх у БД
    AppParameters::instance().setParameter("mposHostName",ui->lineEditServerName->text().trimmed());
    updateParam("mposHostName");
    AppParameters::instance().setParameter("mposPort",QString::number(ui->spinBoxPort->value()));
    updateParam("mposPort");
    AppParameters::instance().setParameter("mposDB",ui->lineEditDBFileName->text().trimmed());
    updateParam("mposDB");
    AppParameters::instance().setParameter("mposUser",ui->lineEditUser->text().trimmed());
    updateParam("mposUser");
    CriptPass crP;
    AppParameters::instance().setParameter("mposPass",crP.criptPass(ui->lineEditPass->text().trimmed()));
    updateParam("mposPass");

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
