#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(sendFindNext(QString)),parent,SLOT(findNext(QString)));
    connect(this,SIGNAL(sendFindPrevious(QString)),parent,SLOT(findPrevious(QString)));
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_findNext_clicked()
{
    QString f=ui->textEdit->toPlainText();
    emit sendFindNext(f);
}

void FindDialog::on_findPrevious_clicked()
{
    QString f=ui->textEdit->toPlainText();
    emit sendFindPrevious(f);
}

