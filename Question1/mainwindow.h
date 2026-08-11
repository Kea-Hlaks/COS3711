#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void onAddClicked();

private:
    QLineEdit *m_studentNumberEdit;
    QLineEdit *m_moduleCodeEdit;
    QLineEdit *m_markEdit;
    QLabel    *m_statusLabel;
};

#endif // MAINWINDOW_H
