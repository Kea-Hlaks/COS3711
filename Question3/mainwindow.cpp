#include "mainwindow.h"
#include "student.h"
#include "studentlist.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_process(new QProcess(this))
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // --- Add student group (via GetStudent process) ---
    m_launchBtn    = new QPushButton("Launch GetStudent");
    m_outputDisplay = new QTextEdit;
    m_outputDisplay->setReadOnly(true);
    m_outputDisplay->setPlaceholderText("Received student entries appear here...");

    QGroupBox *addGroup = new QGroupBox("Add Student");
    QVBoxLayout *addLayout = new QVBoxLayout;
    addLayout->addWidget(m_launchBtn);
    addLayout->addWidget(new QLabel("Received entries:"));
    addLayout->addWidget(m_outputDisplay);
    addGroup->setLayout(addLayout);

    // --- Query group ---
    m_queryNumberEdit = new QLineEdit;
    m_queryNumberEdit->setInputMask("0000;_");
    m_queryNumberEdit->setToolTip("Enter 4-digit student number to query");

    m_displayBtn  = new QPushButton("Display Record");
    m_averageBtn  = new QPushButton("Get Average");
    m_graduateBtn = new QPushButton("Check Graduation");
    m_resultLabel = new QLabel;
    m_resultLabel->setWordWrap(true);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_displayBtn);
    btnLayout->addWidget(m_averageBtn);
    btnLayout->addWidget(m_graduateBtn);

    QGroupBox *queryGroup = new QGroupBox("Query Student");
    QFormLayout *queryForm = new QFormLayout;
    queryForm->addRow("Student Number:", m_queryNumberEdit);
    QVBoxLayout *queryLayout = new QVBoxLayout;
    queryLayout->addLayout(queryForm);
    queryLayout->addLayout(btnLayout);
    queryLayout->addWidget(m_resultLabel);
    queryGroup->setLayout(queryLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(addGroup);
    mainLayout->addWidget(queryGroup);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    central->setLayout(mainLayout);
    setWindowTitle("Student Manager");
    setMinimumSize(460, 480);

    connect(m_launchBtn, &QPushButton::clicked,
            this, &MainWindow::onLaunchClicked);

    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &MainWindow::onProcessOutput);

    connect(m_process,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &MainWindow::onProcessFinished);

    connect(m_displayBtn,  &QPushButton::clicked, this, &MainWindow::onDisplayClicked);
    connect(m_averageBtn,  &QPushButton::clicked, this, &MainWindow::onAverageClicked);
    connect(m_graduateBtn, &QPushButton::clicked, this, &MainWindow::onGraduateClicked);
}

MainWindow::~MainWindow()
{
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        m_process->waitForFinished(2000);
    }
}

void MainWindow::onLaunchClicked()
{
    if (m_process->state() != QProcess::NotRunning) {
        QMessageBox::information(this, "Already running",
            "GetStudent is already open. Please use or close that window first.");
        return;
    }

    m_process->start("GetStudent.exe", QStringList());

    if (!m_process->waitForStarted(3000)) {
        QMessageBox::critical(this, "Error",
            "Could not start GetStudent.exe.\n"
            "Make sure it is in the same folder as this application.");
    }
}

void MainWindow::onProcessOutput()
{
    // Line format from GetStudent: "studentNum,moduleCode,mark"
    while (m_process->canReadLine()) {
        const QString line = QString::fromUtf8(m_process->readLine()).trimmed();
        if (line.isEmpty())
            continue;

        m_outputDisplay->append(line);

        QStringList parts = line.split(',');
        if (parts.size() != 3)
            continue;

        const QString number = parts[0].trimmed();
        const QString code   = parts[1].trimmed();
        bool ok = false;
        const int mark = parts[2].trimmed().toInt(&ok);
        if (!ok)
            continue;

        StudentList *sl = StudentList::instance();
        int idx = sl->indexOf(number);

        if (idx == -1) {
            // New student
            Student *s = new Student(number);
            s->addModule(code, mark);
            sl->addStudent(s);
        } else {
            // Existing student — just add the module
            sl->getAt(idx)->addModule(code, mark);
        }
    }
}

void MainWindow::onProcessFinished(int /*exitCode*/, QProcess::ExitStatus /*status*/)
{
    onProcessOutput();
}

// Helper: find student or show error; returns 0 on failure
static Student *findStudent(const QString &number, QLabel *label)
{
    int idx = StudentList::instance()->indexOf(number);
    if (idx == -1) {
        label->setText("Student " + number + " not found.");
        return 0;
    }
    return StudentList::instance()->getAt(idx);
}

void MainWindow::onDisplayClicked()
{
    const QString number = m_queryNumberEdit->text().trimmed();
    Student *s = findStudent(number, m_resultLabel);
    if (!s) return;

    QString info = "Student: " + s->getNumber() + "\n";
    QMap<QString,int> mods = s->getModules();
    for (auto it = mods.constBegin(); it != mods.constEnd(); ++it)
        info += "  " + it.key() + " : " + QString::number(it.value()) + "\n";

    m_resultLabel->setText(info.trimmed());
}

void MainWindow::onAverageClicked()
{
    const QString number = m_queryNumberEdit->text().trimmed();
    Student *s = findStudent(number, m_resultLabel);
    if (!s) return;

    m_resultLabel->setText(
        QString("Average mark for %1: %2").arg(number).arg(s->average(), 0, 'f', 2));
}

void MainWindow::onGraduateClicked()
{
    const QString number = m_queryNumberEdit->text().trimmed();
    Student *s = findStudent(number, m_resultLabel);
    if (!s) return;

    m_resultLabel->setText(
        number + (s->graduate() ? " qualifies for graduation." : " does NOT qualify for graduation."));
}
