#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>

class Student
{
public:
    Student(const QString &number = QString());

    void    setNumber(const QString &number);
    QString getNumber() const;

    void             addModule(const QString &code, int mark);
    QMap<QString,int> getModules() const;

    double average() const;
    bool   graduate() const;

private:
    QString           m_number;
    QMap<QString,int> m_modules; // module code -> mark
};

#endif // STUDENT_H
