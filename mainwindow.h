#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

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
    void changeEvent(QEvent *);
    void on_activeedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void createActions();
    void createMenu();
    void on_showMainAction();
    void on_exitAppAction();
    void hotKeyActivated();

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
