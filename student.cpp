#include "student.h"
#include <QList>
#include <QDataStream>
Student::Student(QString name,QString sex,QString adress,QString classroom,QString age,QString number,QString tel):
    name(name),sex(sex),adress(adress),classroom(classroom),age(age),tel(tel),number(number)
{

}
 QList<QString> Student::getList() const{
    QList<QString> temp;
    temp.append(name);
    temp.append(age);
    temp.append(sex);
    temp.append(classroom);
    temp.append(number);
    temp.append(tel);
    temp.append(adress);

    return temp;
}
bool Student::operator ==(const Student& s)
{
    return (this->name==s.name || this->number==s.number);
}
QDataStream& operator<<(QDataStream& out, const Student& student)//重载DataStream的流运算符
{
     out<<student.name<<student.age<<student.sex<<student.adress<<student.classroom<<
          student.number<<student.tel;
     return out;
}

QDataStream& operator>>(QDataStream& in, Student& student)//重载DataStream的流运算符
{
    in>>student.name;
    in>>student.age;
    in>>student.sex;
    in>>student.adress;
    in>>student.classroom;
    in>>student.number;
    in>>student.tel;
    return in;
}

QString& Student::operator[] (int i)    //返回引用，这样可以直接对返回值赋值
{
    switch (i) {
    case 0:
        return name;
        break;
    case 1:
        return age;
        break;
    case 2:
        return sex;
        break;
    case 3:
        return classroom;
        break;
    case 4:
        return number;
        break;
    case 5:
        return tel;
        break;
    case 6:
        return adress;
        break;
    default:
        break;
    }
}
