#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_process(new QProcess(this))
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    m_launchBtn = new QPushButton("Add Student");

    m_outputDisplay = new QTextEdit;
    m_outputDisplay->setReadOnly(true);
    m_outputDisplay->setPlaceholderText("Student data will appear here...");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Click the button to open the GetStudent form:"));
    layout->addWidget(m_launchBtn);
    layout->addWidget(new QLabel("Received entries:"));
    layout->addWidget(m_outputDisplay);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    central->setLayout(layout);
    setWindowTitle("Student Manager");
    setMinimumSize(420, 300);

    connect(m_launchBtn, &QPushButton::clicked,
            this, &MainWindow::onLaunchClicked);

    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &MainWindow::onProcessOutput);

    connect(m_process,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &MainWindow::onProcessFinished);
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

    // GetStudent.exe must be placed in the same directory as this executable
    m_process->start("GetStudent.exe", QStringList());

    if (!m_process->waitForStarted(3000)) {
        QMessageBox::critical(this, "Error",
            "Could not start GetStudent.exe.\n"
            "Make sure it is in the same folder as this application.");
    }
}

void MainWindow::onProcessOutput()
{
    // Each line written by GetStudent is "studentNum,moduleCode,mark"
    while (m_process->canReadLine()) {
        const QString line = QString::fromUtf8(m_process->readLine()).trimmed();
        if (!line.isEmpty())
            m_outputDisplay->append(line);
    }
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus /*status*/)
{
    Q_UNUSED(exitCode)
    // Read any remaining output after the process ends
    onProcessOutput();
}
