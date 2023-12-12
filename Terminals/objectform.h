// objectform.h
#ifndef OBJECTFORM_H
#define OBJECTFORM_H

#include "Terminals/termdata.h"
#include "Terminals/connectiondata.h"
#include "Terminals/pingmodel.h"
#include "Terminals/connstatusvnc.h"
#include "Terminals/buttonvnc.h"
#include "Terminals/tanksinfomodel.h"
#include "Terminals/dispenserproperty.h"
#include "Terminals/punpproperty.h"

#include <QDialog>
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QProcess>
#include <QSqlDatabase>
#include <QProgressBar>



namespace Ui {
class ObjectForm;
}

class ObjectForm : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectForm(QSharedPointer<TermData> tData, QWidget *parent = nullptr);
    ~ObjectForm();
signals:
    void signalStartPing();
    void signalSendHost(QString *);
public slots:
    void slotGetQueryTanks(QList<TankProperty> list);
    void slotGetQueryDisp(QList<DispenserProperty> disp, QList<PunpProperty> pump);
private slots:
    void on_toolButtonClipboard_clicked();
    void on_toolButtonAdres2Clip_clicked();
    void slotGetPingString(QByteArray output);
    void on_toolButtonPingAddres_clicked();
    void slotVNCButtonClicked();
    void slotGetStatus(const ConnStatus &status);
    void slotFinishConStatus();
    void slotVNCProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotStartGetTanksInfo();
    void slotFinishGetTanks();
    void slotStartGetDispInfo();
    void slotFinishGetDispInfo();
 private:
    void createUI();
    void createConnList();
    void connListAvias();
    QString getVNCPassword();
    void сheckingСonnections();
    void createConnections();
    void addButtonConnections();
    void tanksTabShow();
    void trkTabShow();

private:
    Ui::ObjectForm *ui;
    QSharedPointer<TermData> m_termData;
    QList<QSharedPointer<ConnectionData>> connList;
    PingModel* pingModel = nullptr; // Ініціалізація вказівника
    bool aviableConnections;
    QVBoxLayout *layoutButton;
    QMap<int, ButtonVNC*> buttonMap;
    int threadCount;
    QProcess *myProcess;
    QSqlDatabase dbCenter;
    TanksInfoModel *modelTanks;


};

#endif // OBJECTFORM_H

