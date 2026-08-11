#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onLaunchClicked();
    void onProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onDisplayClicked();
    void onAverageClicked();
    void onGraduateClicked();

private:
    // Process section
    QPushButton *m_launchBtn;
    QTextEdit   *m_outputDisplay;
    QProcess    *m_process;

    // Query section
    QLineEdit   *m_queryNumberEdit;
    QPushButton *m_displayBtn;
    QPushButton *m_averageBtn;
    QPushButton *m_graduateBtn;
    QLabel      *m_resultLabel;
};

#endif // MAINWINDOW_H
