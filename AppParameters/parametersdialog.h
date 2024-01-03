#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>

namespace Ui {
class ParametersDialog;
}

class ParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersDialog(QWidget *parent = nullptr);
    ~ParametersDialog();
private slots:
    void slotOpenClentVNC();
    void slotOpenClentVNCUltra();
    void slotOpenMPosDBFile();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_comboBoxHostTemplate_activated(int index);

    void on_comboBoxTemplatePass_activated(int index);

    void on_groupBoxTemplatePass_toggled(bool checked);

    void on_lineEditTermIDTemplate_textChanged(const QString &arg1);

    void on_lineEditPrefix_textChanged(const QString &arg1);

    void on_lineEditSufix_textChanged(const QString &arg1);

private:
    void createUI();
    void createConnections();
    void updateOrInsertParameter(const QString& paramName, const QString& paramValue);  // Функція для оновлення або додавання параметрів в базу даних
    void updateParam(const QString& paramName);        // Функція для оновлення параметра
    void createTemplatePass();
private:
    Ui::ParametersDialog *ui;
    QString passSample;
};

#endif // PARAMETERSDIALOG_H
