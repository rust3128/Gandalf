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
    void changeLanguageToUkrainian();
    void changeLanguageToEnglish();
    void changeLanguageToRussian();


protected:
    void showEvent(QShowEvent *event) override;
private:
    void getUserID();             //Получение информации о пользователе
    void updateUserData();          //Изменение данных пользователя, заполнения данныхпользователя
    void createUI();
    void updateStatusBar();
    void changeLangUI(int langCode);          // Изменение данных пользователя и интерефейса при смене языка
private:
    Ui::MainWindow *ui;
    bool isNewUser;                 //Пользовать первый раз в ситеме - true
    int userRole;
    QTranslator appTranslator;
    QTranslator qtTranslator;
    QLabel *labelFlag;

};
#endif // MAINWINDOW_H
