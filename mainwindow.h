#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Terminals/termdata.h"
#include <QMainWindow>
#include <QTranslator>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionUserProfile_triggered();
    void on_actionParametrs_triggered();
    void slotGetTerminalID(int terminalID);
    void on_tabWidgetTerminals_tabCloseRequested(int index);

protected:

private:
    bool openMposDB();
    void createUI();
    void getListAZS();                      //Отримання спису АЗС
    void createConnections();

private:
    Ui::MainWindow *ui;
    QMap<int, QSharedPointer<TermData>> listAzs;            //Список терміналів



};
#endif // MAINWINDOW_H
