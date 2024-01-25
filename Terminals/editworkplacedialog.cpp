#include "editworkplacedialog.h"
#include "ui_editworkplacedialog.h"

EditWorkplaceDialog::EditWorkplaceDialog(int wID, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditWorkplaceDialog)
    , workplaceID(wID)
{
    ui->setupUi(this);
    createUI();
}

EditWorkplaceDialog::~EditWorkplaceDialog()
{
    delete ui;
}

void EditWorkplaceDialog::createUI()
{
    if(workplaceID == 0) {
        this->setWindowTitle(tr("Новое рабочее место"));
    }
}



