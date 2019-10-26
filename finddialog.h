#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <mainwindow.h>
#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();
signals:
    void sendFindNext(QString s);
    void sendFindPrevious(QString s);
private slots:
    void on_findNext_clicked();

    void on_findPrevious_clicked();

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
