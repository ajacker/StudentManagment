#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>
#include <student.h>
#include <QDebug>
#include <QMultiMap>
#include <QLocale>
#include <QCollator>
#include <QColor>
class TableModel : public QAbstractTableModel
{
public:
    TableModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setCurrencyList(const QList<Student> &list);
    QList<Student> getCurrencyList(void);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index,const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int count=1, const QModelIndex &parent = QModelIndex());
    static bool compareInt(const int &i1,const int &i2);
    bool removeRows(int count=1, const QModelIndex &parent = QModelIndex());
    void setCurrentIndexs(const QModelIndexList &indexs);
    void setColor(QColor c);
    QColor getColor();
    void setTextColor(QColor c);
    QColor getTextColor();
    QList<int> find(QString name="",QString number="");
    static bool compareNumber(const Student &s1,const Student &s2);
    bool  sortByNumber();
    static bool compareAge(const Student &s1,const Student &s2);
    bool  sortByAge();
    static bool compareName(const Student &s1,const Student &s2);
    bool  sortByName();
    static bool compareSex(const Student &s1,const Student &s2);
    bool  sortBySex();
    static bool compareClass(const Student &s1,const Student &s2);
    bool  sortByClass();
    void setsortFlag(bool arg);
private:
    int currencyAtVertical(int offset) const;
    QString currencyAtHorizontal(int offset) const;
    QList <Student> currencyList;
    QStringList columndata;
    QColor color,textcolor;
    mutable int column;
    mutable int row;
    QModelIndexList currentindexs;
    enum sort{forward,reverse} sorttype=forward;
};
#endif // TABLEMODEL_H
