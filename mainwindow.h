#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QModelIndex>
#include <QMainWindow>
#include <tablemodel.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    inline void warning();
    TableModel* getTableModel();
public slots:
    void findNext(QString s);
    void findPrevious(QString s);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void on_btn_confirm_clicked();

    void on_openFile_triggered();

    void on_saveAsFile_triggered();

    void on_btn_delete_clicked();

    void on_newFile_triggered();

    void on_saveFile_triggered();

    void on_table_activated(const QModelIndex &index);

    void on_sortbyNumber_triggered();

    void on_sortbyAge_triggered();

    void on_sortbySex_triggered();

    void on_sortbyName_triggered();

    void on_sortbyClass_triggered();

    void on_reverse_toggled(bool arg1);

    void on_editBox_stateChanged(int arg1);

    void on_setFont_triggered();

    void on_setColor_triggered();

    void on_setTextColor_triggered();

    void on_findByName_triggered();

    void on_findByNumber_triggered();

private:
    Ui::MainWindow *ui;
    QString nowpath="";
    TableModel *tablemodel;
};

#endif // MAINWINDOW_H
