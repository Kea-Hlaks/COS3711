#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLaunchClicked();
    void onProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);

private:
    QPushButton *m_launchBtn;
    QTextEdit   *m_outputDisplay;
    QProcess    *m_process;
};

#endif // MAINWINDOW_H
