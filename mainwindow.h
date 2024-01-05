#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Terminals/termdata.h"
#include "Deploys/deploydata.h"
#include "Deploys/deploysmodel.h"
#include <QMainWindow>
#include <QTranslator>
#include <QLabel>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString sendAdressAZS(int termID);
private slots:
    void on_actionUserProfile_triggered();
    void on_actionParametrs_triggered();
    void slotGetTerminalID(int terminalID);
    void on_tabWidgetTerminals_tabCloseRequested(int index);
    void on_action_AboutQt_triggered();
    void on_actionAbout_triggered();
    void deploysShow();                                  //Неотзвон
    void slotErrorGetDeploys(QString message);
    void slotStartGetDeploys();
    void slotGetDeploys(QVector<DeployData> dp);
    void slotFinishGetDeploys();
    void on_pushButtonRefreshDeploys_clicked();

    void on_actionUsers_triggered();

    void on_actionPassManager_triggered();

protected:

private:
    bool openMposDB();
    void createUI();
    void getListAZS();                      //Отримання спису АЗС
    void createConnections();
    void showDeploysData(bool show);
    void setMarqueeText(const QString& text, const QColor& textColor);
private:
    Ui::MainWindow *ui;
    QMap<int, QSharedPointer<TermData>> listAzs;            //Список терміналів
    QVector<DeployData> deploys;                            // Список неотзвонов
    DeploysModel *depModel;                                 // Модель для отображения
    QSortFilterProxyModel *proxyDep;                        // Модель для сортировки неотзвонов
    QTimer *timer;                                          // Тайсер для обновления
    QTimer *marqueeTimer;
};
#endif // MAINWINDOW_H
