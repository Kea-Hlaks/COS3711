#include "studentlist.h"

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>

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

void StudentList::saveToXml(const QString &filePath) const
{
    QDomDocument doc;
    QDomElement root = doc.createElement("StudentList");
    doc.appendChild(root);

    for (int i = 0; i < m_students.size(); ++i) {
        Student *s = m_students[i];

        QDomElement studentElem = doc.createElement("student");
        root.appendChild(studentElem);

        QDomElement numberElem = doc.createElement("number");
        numberElem.appendChild(doc.createTextNode(s->getNumber()));
        studentElem.appendChild(numberElem);

        QDomElement modulesElem = doc.createElement("modules");
        studentElem.appendChild(modulesElem);

        QMap<QString,int> mods = s->getModules();
        for (auto it = mods.constBegin(); it != mods.constEnd(); ++it) {
            QDomElement moduleElem = doc.createElement("module");
            modulesElem.appendChild(moduleElem);

            QDomElement codeElem = doc.createElement("code");
            codeElem.appendChild(doc.createTextNode(it.key()));
            moduleElem.appendChild(codeElem);

            QDomElement markElem = doc.createElement("mark");
            markElem.appendChild(doc.createTextNode(QString::number(it.value())));
            moduleElem.appendChild(markElem);
        }
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    doc.save(out, 4); // 4-space indent
    file.close();
}

void StudentList::loadFromXml(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "StudentList")
        return;

    QDomNodeList studentNodes = root.elementsByTagName("student");
    for (int i = 0; i < studentNodes.size(); ++i) {
        QDomElement studentElem = studentNodes.at(i).toElement();

        QString number = studentElem.firstChildElement("number").text().trimmed();
        if (number.isEmpty())
            continue;

        Student *s = new Student(number);

        QDomNodeList moduleNodes = studentElem.firstChildElement("modules")
                                              .elementsByTagName("module");
        for (int j = 0; j < moduleNodes.size(); ++j) {
            QDomElement modElem = moduleNodes.at(j).toElement();
            QString code = modElem.firstChildElement("code").text().trimmed();
            bool ok = false;
            int mark = modElem.firstChildElement("mark").text().trimmed().toInt(&ok);
            if (!code.isEmpty() && ok)
                s->addModule(code, mark);
        }

        m_students.append(s);
    }
}
