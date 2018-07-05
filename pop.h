#ifndef POP_H
#define POP_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPixmap>
#include <QAction>
#include <QMenu>

namespace Ui {
class Pop;
}

class Pop : public QWidget
{
    Q_OBJECT

public:
    explicit Pop(QWidget *parent = 0);
    ~Pop();

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    Ui::Pop *ui;
    QPoint relativePos;
    QPixmap *pix;
    QMenu *popMenu;
    QAction *popShowMainWindow;
    QAction *popValid;
    QAction *popHidden;
    QAction *popExit;


private slots:
    void on_popHiddenAction();
    void on_popExitAction();
    void on_popValidAction();
    void on_popShowMainWindowAction();
};

#endif // POP_H
