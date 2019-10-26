#ifndef MYTABLE_H
#define MYTABLE_H
#include <QTableView>
#include <QMouseEvent>

class myTable : public QTableView
{
public:
    myTable(QWidget *parent = 0);
    ~myTable();
protected:
    void mouseReleaseEvent(QMouseEvent *event);//松开鼠标
   // void keyPressEvent(QKeyEvent *event);    //键盘响应
};

#endif // MYTABLE_H
