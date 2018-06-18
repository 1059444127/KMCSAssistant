#ifndef ROLEMANAWINDOW_H
#define ROLEMANAWINDOW_H

#include <QMainWindow>

namespace Ui {
class RoleManaWindow;
}

class RoleManaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoleManaWindow(QWidget *parent = 0);
    ~RoleManaWindow();

private slots:
    void initTableView();
    void setTableData();
    void initDataBase();
    void queryAllRole();

    void on_actionAdd_triggered();

    void on_actionClear_triggered();

    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

private:
    Ui::RoleManaWindow *ui;
};

#endif // ROLEMANAWINDOW_H
