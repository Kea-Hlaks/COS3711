#include "studentlist.h"

StudentList *StudentList::m_instance = 0;

StudentList::StudentList()
{
}

StudentList::~StudentList()
{
    qDeleteAll(m_students);
}

StudentList *StudentList::instance()
{
    if (!m_instance)
        m_instance = new StudentList;
    return m_instance;
}

void StudentList::addStudent(Student *student)
{
    m_students.append(student);
}

QList<Student*> StudentList::getList() const
{
    return m_students;
}

int StudentList::indexOf(const QString &number) const
{
    for (int i = 0; i < m_students.size(); ++i) {
        if (m_students[i]->getNumber() == number)
            return i;
    }
    return -1;
}

Student *StudentList::getAt(int index) const
{
    if (index < 0 || index >= m_students.size())
        return 0;
    return m_students[index];
}

int StudentList::size() const
{
    return m_students.size();
}
