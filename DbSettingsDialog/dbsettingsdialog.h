#ifndef DBSETTINGSDIALOG_H
#define DBSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class DbSettingsDialog;
}

class DbSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DbSettingsDialog(bool isNew, QWidget *parent = nullptr);
    ~DbSettingsDialog();

private slots:
    void openDatabaseFile();
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void createUI();
    void readSettings();
    void writeSettings();
private:
    Ui::DbSettingsDialog *ui;
};

#endif // DBSETTINGSDIALOG_H
