// objectform.h
#ifndef OBJECTFORM_H
#define OBJECTFORM_H

#include "Terminals/termdata.h"
#include "Terminals/connectiondata.h"
#include <QDialog>
#include <QSharedPointer>  // Додайте цей імпорт

namespace Ui {
class ObjectForm;
}

class ObjectForm : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectForm(QSharedPointer<TermData> tData, QWidget *parent = nullptr);
    ~ObjectForm();
private slots:
    void on_toolButtonClipboard_clicked();
private:
    void createUI();
    void createConnList();
    void connListAvias();
private:
    Ui::ObjectForm *ui;
    QSharedPointer<TermData> m_termData;
    QList<ConnectionData> connList;
};

#endif // OBJECTFORM_H

