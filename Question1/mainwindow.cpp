#include "mainwindow.h"

#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // --- Student number: exactly 4 digits via input mask ---
    m_studentNumberEdit = new QLineEdit;
    m_studentNumberEdit->setInputMask("0000;_");
    m_studentNumberEdit->setToolTip("4-digit student number");

    // --- Module code: max 7 chars; structural validator allows progressive typing ---
    // Full pattern enforced on submit: [A-Z]{3}[123][0-9]{2}[A-Za-z0-9]
    m_moduleCodeEdit = new QLineEdit;
    m_moduleCodeEdit->setMaxLength(7);
    // Intermediate validator lets the user build up the code character by character
    QRegularExpression moduleIntermediateRx("^[A-Z]{0,3}([123][0-9]{0,2}[A-Za-z0-9]?)?$");
    m_moduleCodeEdit->setValidator(
        new QRegularExpressionValidator(moduleIntermediateRx, m_moduleCodeEdit));
    m_moduleCodeEdit->setPlaceholderText("e.g. COS3711");
    m_moduleCodeEdit->setToolTip("3 uppercase letters + year (1/2/3) + 2 digits + alphanumeric");

    // --- Mark: integer constrained to 0–100 ---
    m_markEdit = new QLineEdit;
    m_markEdit->setValidator(new QIntValidator(0, 100, m_markEdit));
    m_markEdit->setPlaceholderText("0 – 100");
    m_markEdit->setToolTip("Integer mark between 0 and 100");

    QPushButton *addBtn = new QPushButton("Add");
    m_statusLabel = new QLabel;
    m_statusLabel->setWordWrap(true);

    QFormLayout *form = new QFormLayout;
    form->addRow("Student Number:", m_studentNumberEdit);
    form->addRow("Module Code:",    m_moduleCodeEdit);
    form->addRow("Mark:",           m_markEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(form);
    mainLayout->addWidget(addBtn);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    central->setLayout(mainLayout);
    setWindowTitle("GetStudent");
    setMinimumWidth(320);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
}

void MainWindow::onAddClicked()
{
    const QString studentNum = m_studentNumberEdit->text().trimmed();
    const QString moduleCode = m_moduleCodeEdit->text().trimmed();
    const QString markStr    = m_markEdit->text().trimmed();

    // Validate student number: exactly 4 digits
    static const QRegularExpression snRx("^[0-9]{4}$");
    if (!snRx.match(studentNum).hasMatch()) {
        m_statusLabel->setText("Error: Student number must be exactly 4 digits.");
        return;
    }

    // Validate module code: [A-Z]{3}[123][0-9]{2}[A-Za-z0-9]
    static const QRegularExpression mcRx("^[A-Z]{3}[123][0-9]{2}[A-Za-z0-9]$");
    if (!mcRx.match(moduleCode).hasMatch()) {
        m_statusLabel->setText(
            "Error: Module code must be 3 uppercase letters, year digit (1/2/3), "
            "2 digits, then one alphanumeric character (e.g. COS3711).");
        return;
    }

    // Validate mark: integer 0–100
    bool ok = false;
    const int mark = markStr.toInt(&ok);
    if (!ok || mark < 0 || mark > 100) {
        m_statusLabel->setText("Error: Mark must be an integer between 0 and 100.");
        return;
    }

    // Output to stdout — parsed by Question 2 via QProcess
    const QString output = QString("%1,%2,%3").arg(studentNum, moduleCode).arg(mark);
    std::cout << output.toStdString() << std::endl;

    m_statusLabel->setText("Added: " + output);

    m_studentNumberEdit->clear();
    m_moduleCodeEdit->clear();
    m_markEdit->clear();
    m_studentNumberEdit->setFocus();
}
