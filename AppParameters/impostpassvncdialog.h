#ifndef IMPOSTPASSVNCDIALOG_H
#define IMPOSTPASSVNCDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class ImpostPassVNCDialog;
}

class ImpostPassVNCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImpostPassVNCDialog(QWidget *parent = nullptr);
    ~ImpostPassVNCDialog();
private slots:
    void slotOpenCSVFile();
    void on_comboBoxDelimiter_textActivated(const QString &arg1);

    void on_pushButtonImport_clicked();

    void on_buttonBox_rejected();

private:
    void createUI();
    void loadCSV(const QString &filename);
private:
    Ui::ImpostPassVNCDialog *ui;
    QStandardItemModel *model;
    QString selectedDelimiter;
};

#endif // IMPOSTPASSVNCDIALOG_H
