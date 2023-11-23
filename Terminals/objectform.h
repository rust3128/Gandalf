// objectform.h
#ifndef OBJECTFORM_H
#define OBJECTFORM_H

#include "Terminals/termdata.h"
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
private:
    Ui::ObjectForm *ui;
    QSharedPointer<TermData> m_termData;
};

#endif // OBJECTFORM_H

