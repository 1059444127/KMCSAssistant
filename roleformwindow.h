#ifndef ROLEFORMWINDOW_H
#define ROLEFORMWINDOW_H

#include <QMainWindow>

namespace Ui {
class RoleFormWindow;
}

class RoleFormWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoleFormWindow(QWidget *parent = 0);
    ~RoleFormWindow();

private slots:
    void on_addThen_clicked();

    void on_actionSave_S_triggered();
    void initDataBase();

private:
    Ui::RoleFormWindow *ui;
};

#endif // ROLEFORMWINDOW_H
