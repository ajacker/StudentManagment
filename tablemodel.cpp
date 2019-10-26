#include <tablemodel.h>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)//构造函数
{
    color=QColor(Qt::white);
    textcolor=QColor(Qt::black);
}
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    flag|=Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}
int TableModel::rowCount(const QModelIndex & parent) const//行数
{
    row=currencyList.count();
    return row;
}

int TableModel::columnCount(const QModelIndex & parent) const//列数
{
    column=7;
    return column;
}
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const//返回列名
{
    if (role != Qt::DisplayRole) return QVariant();
    if(orientation == Qt::Horizontal) return currencyAtHorizontal (section);
    else return currencyAtVertical(section);
}

int TableModel::currencyAtVertical(int offset) const//返回offset行的key（行名）
{
    return offset+1;
}
QString TableModel::currencyAtHorizontal(int offset) const//返回offset列的列名
{
    QStringList columndata;
    columndata<<"姓名"<<"年龄"<<"性别"<<"班级"<<"学号"<<"联系方式"<<"家庭住址";
    return columndata[offset];
}
void TableModel::setCurrencyList(const QList<Student> &list)//设置数据的时候刷新模型
{
    beginResetModel();
    currencyList = list;
    endResetModel();
}
QList<Student> TableModel::getCurrencyList(void)
{
    return currencyList;
}
bool TableModel::insertRows(int count,const QModelIndex &parent)
{
    if(currentindexs.isEmpty()){//如果未选择就添加到末尾
        beginInsertRows(parent,row,row+count-1);
        for(int i=0;i<count;i++){//添加count行
                Student s;
                currencyList.append(s);
        }
        endInsertRows();
    }else{//如果选择了就添加到选择的那一行后面
        beginInsertRows(parent,currentindexs.last().row()+1,currentindexs.last().row()+count);
        for(int i=0;i<count;i++){
                Student s;
                currencyList.insert(currentindexs.last().row()+1,s);
        }
        endInsertRows();
    }
    return true;
}
bool TableModel::compareInt(const int &i1,const int &i2){
    return i1<i2;
}
bool TableModel::removeRows(int count,const QModelIndex &parent)
{
    if (currencyList.count()==0) return false;
    if(currentindexs.isEmpty()){//如果未选择就删除末尾
        qDebug()<<"未选择";
        beginResetModel();
        for(int i=0;i<count;i++){//删除count行
                currencyList.removeLast();
        }
        endResetModel();
    }else{//如果选择了就从选择的那一行开始删除
        qDebug()<<currentindexs.first().row()<<currentindexs.last().row()<<currentindexs.count();
        QList<int> list;
        for(int i=0;i<currentindexs.count();i++){//所选行号加入list
            list.append(currentindexs.at(0+i).row());
        }
        qSort(list.begin(),list.end(),compareInt);//从小到大排序list
        qDebug()<<list.first()<<list.last()<<list.count();
        beginResetModel();
        for(int i=0;i<list.count();i++){//选择了多少个就删除多少个
                currencyList.removeAt(list.at(0+i)-i);//删除了某一个以后相对位置-i
        }
        endResetModel();
    }
    return true;
}
QVariant TableModel::data(const QModelIndex &index, int role) const//控制每一格数据显示的内容
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {//对齐方式
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }else if(role == Qt::BackgroundColorRole){
        return color;
    }else if(role == Qt::TextColorRole){
        return textcolor;
    }else if (role == Qt::DisplayRole||role == Qt::EditRole) {//显示内容
        QList<QString> QDataList;//行数据
        switch (sorttype) {
        case reverse:
            qDebug()<<row;
            QDataList = currencyList[row-1-index.row()].getList();//反向获取当前行数据的QList
            break;
        case forward:
            QDataList = currencyList[index.row()].getList();//获取当前行数据的QList
            break;
        }
        if (QDataList[index.column()].isEmpty()) {//数据为空显示为####
            return "####";
        }
        return QDataList[index.column()];//显示
    }
    return QVariant();
}
bool TableModel::setData(const QModelIndex &index,const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QString &currentdata=currencyList[index.row()][index.column()];//获取修改前的数据
        currentdata=value.toString();//修改数据
        return true;
    }
    return false;
}
void TableModel::setCurrentIndexs(const QModelIndexList &indexs){
    currentindexs=indexs;
}
void TableModel::setColor(QColor c){
    beginResetModel();
    color=c;
    endResetModel();
}
QColor TableModel::getColor(){
    return color;
}
void TableModel::setTextColor(QColor c){
    beginResetModel();
    textcolor=c;
    endResetModel();
}
QColor TableModel::getTextColor(){
    return textcolor;
}
QList<int> TableModel::find(QString name,QString number){
    Student stu(name,"","","","",number,"");
    QList<int> list;
    int i=0;
    while(currencyList.indexOf(stu,i)!=-1){//如果有结果
        i=currencyList.indexOf(stu,i)+1;//记录结果索引+1，下次从这开始找
        list.append(i-1);//添加到结果列表
    }
    if (i==0) list.append(-1);//如果一个结果都找不到就返回一个-1
    return list;
}
bool TableModel::compareNumber(const Student &s1,const Student &s2){//用于排序的规则
    return s1.number.toLong()>s2.number.toLong();
}
bool TableModel::compareAge(const Student &s1,const Student &s2){
    return s1.age.toInt()>s2.age.toInt();
}
bool TableModel::compareName(const Student &s1,const Student &s2){
    QLocale cn(QLocale::Chinese);
    QCollator collator(cn);
    return collator(s1.name,s2.name);
}
bool TableModel::compareSex(const Student &s1,const Student &s2){
    return QString::compare(s1.sex,s2.sex)>0;
}
bool TableModel::compareClass(const Student &s1,const Student &s2){
    QLocale cn(QLocale::Chinese);
    QCollator collator(cn);
    return collator(s1.classroom,s2.classroom);
}
bool TableModel::sortByNumber(){
    beginResetModel();
    qSort(currencyList.begin(),currencyList.end(),compareNumber);
    endResetModel();
    return true;
}
bool TableModel::sortByAge(){
    beginResetModel();
    qSort(currencyList.begin(),currencyList.end(),compareAge);
    endResetModel();
    return true;
}
bool TableModel::sortByName(){
    beginResetModel();
    qSort(currencyList.begin(),currencyList.end(),compareName);
    endResetModel();
    return true;
}
bool TableModel::sortBySex(){
    beginResetModel();
    qSort(currencyList.begin(),currencyList.end(),compareSex);
    endResetModel();
    return true;
}
bool TableModel::sortByClass(){
    beginResetModel();
    qSort(currencyList.begin(),currencyList.end(),compareClass);
    endResetModel();
    return true;
}
void TableModel::setsortFlag(bool arg){
    beginResetModel();
    if(arg){sorttype=reverse;}
    else{sorttype=forward;}
    endResetModel();
}
