// TerminalListDialog.h
#ifndef TERMINALLISTDIALOG_H
#define TERMINALLISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QSharedPointer>
#include "Terminals/TermData.h" // Припустимо, що у вас є файли заголовків для класу TermData
#include "TerminalListModel.h"

namespace Ui {
class TerminalListDialog;
}

class TerminalListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TerminalListDialog(const QMap<int, QSharedPointer<TermData>>& terminalMap, QWidget *parent = nullptr);
    ~TerminalListDialog();

private:
    void createUI();
private:
    Ui::TerminalListDialog *ui;
    QMap<int, QSharedPointer<TermData>> terminalMap; // Додали поле для зберігання списку терміналів
    TerminalListModel *terminalModel;
};

#endif // TERMINALLISTDIALOG_H
