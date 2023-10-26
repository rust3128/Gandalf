#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AppParameters/AppParameters.h"
#include "LogginCategories/loggincategories.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    QString strTitle = tr("Маг не приходит поздно, %1, и рано тоже не приходит. Он появляется тогда, когда положено.")
                           .arg(AppParameters::instance().getParameter("userFIO"));
    QLabel *labelStatus =new QLabel(strTitle);
    ui->statusbar->addWidget(labelStatus);
}


