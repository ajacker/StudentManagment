#include "mytable.h"
#include <QDebug>
#include <tablemodel.h>
myTable::myTable(QWidget *parent)
{

}
myTable::~myTable(){

}
void myTable::mouseReleaseEvent(QMouseEvent *event){
    QItemSelectionModel *selections =this->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    reinterpret_cast<TableModel *>(this->model())->setCurrentIndexs(selected);
}
/*void myTable::keyPressEvent(QKeyEvent *event){//键盘输入事件
    this->resizeColumnsToContents();//自适应列宽
}*/
