#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include "student.h"
#include <QList>
#include <QString>

class StudentList
{
public:
    static StudentList *instance();

    void            addStudent(Student *student);
    QList<Student*> getList() const;

    // Returns index of student with given number, or -1 if not found
    int      indexOf(const QString &number) const;
    Student *getAt(int index) const;
    int      size() const;

    void saveToXml(const QString &filePath) const;
    void loadFromXml(const QString &filePath);

private:
    StudentList();
    ~StudentList();

    static StudentList *m_instance;
    QList<Student*>     m_students;
};

#endif // STUDENTLIST_H
