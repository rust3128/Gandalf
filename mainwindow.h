#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Users/userprofiledialog.h"
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

protected:

private:
    void createUI();
private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
