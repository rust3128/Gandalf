#ifndef SENDMESSAGEDIALOG_H
#define SENDMESSAGEDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class SendMessageDialog;
}

class SendMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendMessageDialog(int termID, QString dbHost, QWidget *parent = nullptr);
    ~SendMessageDialog();
signals:
    void signalOpenDB();
private slots:
    void dbAzsOpen();
    void on_buttonBox_rejected();
    void on_plainTextEditMessage_textChanged();
    void on_checkBox_stateChanged(int state);

    void on_pushButtonSendMessage_clicked();

    void on_pushButtonTestConnection_clicked();

private:
    void createUI();
    void createDBconnections();
    void readDBConnections();
    void dbConnAvias();
    void dbConnUrknafta();
    void dbConnMarshal();
    void dbConnDatabase();
    void writeDBParametrs();
private:
    Ui::SendMessageDialog *ui;
    QString defMessage;
    int terminalID;
    QString dbHostname;
    int dbPort;
    QString dbDatabase = "D:/MPos3/m_pos.gdb";
    QString dbUser ="SYSDBA";
    QString dbPass;
    QTimer *timer;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // SENDMESSAGEDIALOG_H
