#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCloseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onLaunchClicked();
    void onProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onDisplayClicked();
    void onAverageClicked();
    void onGraduateClicked();

private:
    QPushButton *m_launchBtn;
    QTextEdit   *m_outputDisplay;
    QProcess    *m_process;

    QLineEdit   *m_queryNumberEdit;
    QPushButton *m_displayBtn;
    QPushButton *m_averageBtn;
    QPushButton *m_graduateBtn;
    QLabel      *m_resultLabel;

    static const QString XML_FILE;
};

#endif // MAINWINDOW_H
