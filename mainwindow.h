#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionGet_triggered();

    void on_actionClear_triggered();

    void on_actionAuthor_triggered();

    void on_actionRoleMana_triggered();

    void on_actionValidRole_triggered();
    void queryAllRole();

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
