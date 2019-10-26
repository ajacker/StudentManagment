#include "opendialog.h"
#include "ui_opendialog.h"

openDialog::openDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openDialog)
{
    ui->setupUi(this);
}

openDialog::~openDialog()
{
    delete ui;
}
