#ifndef POP_H
#define POP_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QEvent>
#include <QPixmap>
#include <QAction>
#include <QMenu>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class Pop;
}

class Pop : public QWidget
{
    Q_OBJECT

public:
    explicit Pop(QWidget *parent = 0);
    ~Pop();
    void setValidState(int val, QString txt);

signals:
    void showMainWindowSignal();
    void validRoleSignal();
    void exitAppSignal();

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    Ui::Pop *ui;
    QPoint relativePos;
    QPixmap *pix;
    QPixmap *pix2;
    QMenu *popMenu;
    QAction *popShowMainWindow;
    QAction *popValid;
    QAction *popHidden;
    QAction *popExit;
    int flag = 1;
    QGraphicsDropShadowEffect *shadow_effect;
    QGraphicsDropShadowEffect *shadow_effect2;

private slots:
    void on_popHiddenAction();
    void on_popExitAction();
    void on_popValidAction();
    void on_popShowMainWindowAction();
};

#endif // POP_H
