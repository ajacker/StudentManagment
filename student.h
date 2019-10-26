#ifndef STUDENT_H
#define STUDENT_H
#include <QString>
class Student
{
public:
    Student(QString name="",QString sex="男",QString adress="",QString classroom="",QString age="",QString number="",QString tel="");
    QString name,sex,adress,classroom,age,tel,number;
    QList<QString> getList() const;
    friend QDataStream& operator<<(QDataStream& out, const Student& student);
    friend QDataStream& operator>>(QDataStream& in, Student& student);
    QString& operator[] (int i);
    bool operator ==(const Student& s);
};

#endif // STUDENT_H
