#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = nullptr);
    ~SearchForm();
signals:
    void signalSendSearchTerminalID(int);
private slots:
    void on_lineEditSearch_returnPressed();

private:
    void createUI();
private:
    Ui::SearchForm *ui;
};

#endif // SEARCHFORM_H
