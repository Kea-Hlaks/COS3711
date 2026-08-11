#include "student.h"

Student::Student(const QString &number)
    : m_number(number)
{
}

void Student::setNumber(const QString &number)
{
    m_number = number;
}

QString Student::getNumber() const
{
    return m_number;
}

void Student::addModule(const QString &code, int mark)
{
    m_modules[code] = mark;
}

QMap<QString,int> Student::getModules() const
{
    return m_modules;
}

double Student::average() const
{
    if (m_modules.isEmpty())
        return 0.0;

    int total = 0;
    for (int mark : m_modules.values())
        total += mark;

    return static_cast<double>(total) / m_modules.size();
}

bool Student::graduate() const
{
    // Must have passed (>=50) at least 5 modules,
    // with at most 2 being 1st-year and at least 1 being 3rd-year.
    int passed = 0, firstYear = 0, thirdYear = 0;

    for (auto it = m_modules.constBegin(); it != m_modules.constEnd(); ++it) {
        if (it.value() >= 50) {
            ++passed;
            // Year digit is the 4th character of the module code (index 3)
            if (it.key().length() >= 4) {
                QChar yr = it.key().at(3);
                if (yr == '1') ++firstYear;
                if (yr == '3') ++thirdYear;
            }
        }
    }

    return (passed >= 5) && (firstYear <= 2) && (thirdYear >= 1);
}
