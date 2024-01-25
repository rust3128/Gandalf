#ifndef LISTAZSDIALOG_H
#define LISTAZSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QSharedPointer>
#include <QSqlRelationalTableModel>

#include "Terminals/TermData.h"
#include "TerminalListModel.h"

namespace Ui {
class ListAzsDialog;
}

class ListAzsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListAzsDialog(const QMap<int, QSharedPointer<TermData>>& terminalMap, QWidget *parent = nullptr);
    ~ListAzsDialog();
private slots:
    void onTerminalSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_toolButtonAdd_clicked();

private:
    void createUI();
    void createModel();
private:
    Ui::ListAzsDialog *ui;
    QMap<int, QSharedPointer<TermData>> terminalMap; // Додали поле для зберігання списку терміналів
    TerminalListModel *modelTerminals;
    QSqlRelationalTableModel *modelWorkplace;
};

#endif // LISTAZSDIALOG_H
