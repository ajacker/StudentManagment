#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <tablemodel.h>
#include <QListIterator>
#include <QDebug>
#include <QMimeData>
#include <mytable.h>
#include <finddialog.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->table->setParent(ui->centralWidget);
    ui->table->move(0,0);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setMinimumSectionSize(100);
    on_newFile_triggered();//打开时创建新文件
    setAcceptDrops(true);//支持拖放操作
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)//拖放进入事件
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }else{
        event->ignore();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug()<<"dropEvent";
    const QMimeData *mimeData = event->mimeData();//获取mime数据
        if(mimeData->hasUrls())
        {
            QList<QUrl> urlList = mimeData->urls();//获取url列表
            QString fileName = urlList.at(0).toLocalFile();//将第一个URL作为本地文件路径
            qDebug()<<"fileName:"<<fileName;
            QFile file(fileName);
            if (!file.open(QIODevice::ReadWrite)) {
                QMessageBox::warning(this, tr("读入文件"),
                                     tr("无法打开文件:\n%1").arg(fileName));
                return;
            }
            QDataStream in(&file);
            QList<Student> data;
            // 检查魔术数字
            quint32 magic;
            in >> magic;
            if (magic != 0xA0B0C0D0) {
                QMessageBox::warning(this, tr("警告"),
                                     tr("文件格式错误:\n%1").arg(fileName));
                file.close();//关闭文件
                return;
            }
            nowpath=fileName;
            QFont font;
            QColor color,textcolor;
            in >> font;//读入字体
            in >> color;//读入颜色
            in >> textcolor;//字体颜色
            in >> data;//读入数据
            file.close();//关闭文件
            //更新表格视图
            TableModel *model = new TableModel(ui->table);
            model->setCurrencyList(data);
            tablemodel=model;
            ui->table->setModel(model);
            ui->table->setFont(font);
            tablemodel->setColor(color);
            tablemodel->setTextColor(textcolor);
            ui->table->resizeColumnsToContents();//自适应列宽
            ui->table->show();
        }
}
void MainWindow::warning(){
    if(tablemodel==NULL) {
        QMessageBox::warning(this, tr("警告"),
                             tr("请先打开或者新建一个文件！"));
        return;
    }
}
void MainWindow::on_btn_confirm_clicked()//点击"添加"按钮
{
    warning();
    tablemodel->insertRows(1);

}

void MainWindow::on_editBox_stateChanged(int arg1)//勾选修改框
{
    switch (arg1) {
    case 0://不勾选
        ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        break;
    case 2://勾选
        QAbstractItemView::EditTriggers triggers=QAbstractItemView::NoEditTriggers;
        triggers|=QAbstractItemView::DoubleClicked;
        triggers|=QAbstractItemView::EditKeyPressed;
        triggers|=QAbstractItemView::AnyKeyPressed;
        ui->table->setEditTriggers(triggers);
        break;
    }
}

void MainWindow::on_btn_delete_clicked()//点击"删除"按钮
{
    warning();
    tablemodel->removeRows(1);
}

void MainWindow::on_openFile_triggered()//点击菜单项"打开"
{
    QString filePath=QFileDialog::getOpenFileName(this, tr("文件对话框"), ".", tr("学生信息文件(*.sti)"));
    if(!filePath.isEmpty()) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadWrite)) {
                QMessageBox::warning(this, tr("读入文件"),
                                     tr("无法打开文件:\n%1").arg(filePath));
                return;
            }
            QDataStream in(&file);
            QList<Student> data;
            // 检查魔术数字
            quint32 magic;
            in >> magic;
            if (magic != 0xA0B0C0D0) {
                QMessageBox::warning(this, tr("警告"),
                                     tr("文件格式错误:\n%1").arg(filePath));
                file.close();//关闭文件
                return;
            }
            nowpath=filePath;
            QFont font;
            QColor color,textcolor;
            in >> font;//读入字体
            in >> color;//读入背景色
            in >> textcolor;//读入字体颜色
            in >> data;//读入数据
            file.close();//关闭文件
            //更新表格视图
            TableModel *model = new TableModel(ui->table);
            model->setCurrencyList(data);
            tablemodel=model;
            ui->table->setModel(model);
            ui->table->setFont(font);
            tablemodel->setColor(color);
            tablemodel->setTextColor(textcolor);
            ui->table->resizeColumnsToContents();//自适应列宽
            ui->table->show();
        }
}


void MainWindow::on_saveFile_triggered()//点击菜单项"保存"
{
    if(nowpath.isEmpty()){
        on_saveAsFile_triggered();
        return;
    }
    QFile file(nowpath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("写入文件"),
                                   tr("无法打开文件:\n%1").arg(nowpath));
        return;
    }
    QDataStream out(&file);
    out << (quint32)0xA0B0C0D0;//写入特定字符表示文件类型
    out << ui->table->font();//写入字体
    out << tablemodel->getColor();//写入背景色
    out << tablemodel->getTextColor();//写入字体颜色
    out << tablemodel->getCurrencyList();//保存表格数据
    file.close();

}

void MainWindow::on_saveAsFile_triggered()//点击菜单项"另存为"
{
    QString path=QFileDialog::getSaveFileName(this,tr("文件对话框"),"StudentInfo", tr("学生信息文件(*.sti)"));
    if (QFileInfo(path).suffix().isEmpty()) path.append(".sti");  //若后缀为空自动添加后缀
    if(!path.isEmpty()) {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::warning(this, tr("写入文件"),
                                           tr("无法打开文件:\n%1").arg(path));
                return;
            }
            QDataStream out(&file);
            out << (quint32)0xA0B0C0D0;//写入特定字符表示文件类型
            out << ui->table->font();//写入字体
            out << tablemodel->getColor();//写入背景色
            out << tablemodel->getTextColor();//写入字体颜色
            out <<tablemodel->getCurrencyList();//保存表格数据
            file.close();
            nowpath=path;
        }
}

void MainWindow::on_newFile_triggered()//点击菜单项"新建"
{
    nowpath="";
    QList<Student> data;
    Student s;
    data.append(s);
    TableModel *model = new TableModel(ui->table);
    tablemodel=model;
    model->setCurrencyList(data);
    ui->table->setModel(model);
    ui->table->show();
}

void MainWindow::on_table_activated(const QModelIndex &index)
{
    qDebug()<<"table_activated";
    QItemSelectionModel *selections =ui->table->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    tablemodel->setCurrentIndexs(selected);
    ui->table->resizeColumnsToContents();//自适应列宽
}

void MainWindow::on_sortbyNumber_triggered()
{
    warning();
    tablemodel->sortByNumber();
}

void MainWindow::on_sortbyAge_triggered()
{

    warning();
    tablemodel->sortByAge();
}

void MainWindow::on_sortbySex_triggered()
{

    warning();
    tablemodel->sortBySex();
}

void MainWindow::on_sortbyName_triggered()
{

    warning();
    tablemodel->sortByName();
}

void MainWindow::on_sortbyClass_triggered()
{
    warning();
    tablemodel->sortByClass();
}

void MainWindow::on_reverse_toggled(bool arg1)
{
    warning();
    tablemodel->setsortFlag(arg1);
}

void MainWindow::on_setFont_triggered()
{
    warning();
    bool ok;
    QFont font=ui->table->font();
    QFont f = QFontDialog::getFont(&ok,font);//选择字体
    if(ok)
    {
        ui->table->setFont(f);
    }
}

void MainWindow::on_setColor_triggered()
{
    warning();
    QColor color=tablemodel->getColor();
    QColor c = QColorDialog::getColor(color);//选择颜色
    tablemodel->setColor(c);
}

void MainWindow::on_setTextColor_triggered()
{
    warning();
    QColor color=tablemodel->getTextColor();
    QColor c = QColorDialog::getColor(color);//选择颜色
    tablemodel->setTextColor(c);
}


void MainWindow::on_findByName_triggered()
{
    warning();
    FindDialog *dialog=new FindDialog(this);
    dialog->setWindowTitle("通过姓名查找");
    dialog->show();
}

TableModel* MainWindow::getTableModel(){
    return tablemodel;
}

void MainWindow::on_findByNumber_triggered()
{
    warning();
    FindDialog *dialog=new FindDialog(this);
    dialog->setWindowTitle("通过学号查找");
    dialog->show();
}

void MainWindow::findNext(QString s){
    QList<int> list=tablemodel->find(s,s);
    int selection=-1;
    int currentindex=ui->table->selectionModel()->currentIndex().row();
    for(int i=0;i<list.count();i++){
        if(currentindex < list.at(i)){
            selection=list.at(i);
            break;
        }
    }
    if(selection!=-1) ui->table->selectRow(selection);
    else {
        QMessageBox::warning(this, tr("警告"),
                                   tr("找不到结果！"));
    }
}

void MainWindow::findPrevious(QString s){
    QList<int> list=tablemodel->find(s,s);
    int selection=-1;
    int currentindex=ui->table->selectionModel()->currentIndex().row();
    for(int i=0;i<list.count();i++){
        if(currentindex > list.at(list.count()-i-1)){
            selection=list.at(list.count()-i-1);
            break;
        }
    }
    if(selection!=-1) ui->table->selectRow(selection);
    else {
        QMessageBox::warning(this, tr("警告"),
                                   tr("找不到结果！"));
    }
}

