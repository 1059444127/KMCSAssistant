#include "pop.h"
#include "ui_pop.h"
#include "QMouseEvent"
#include "QPixmap"
#include <QAction>
#include <QMenu>
#include <QDesktopWidget>
#include <QRect>
#include <QDebug>
#include <QEvent>
#include <QGraphicsDropShadowEffect>

Pop::Pop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pop)
{
    // 获取屏幕分辨率
    QDesktopWidget *desk = QApplication::desktop();
    QRect screen = desk->screenGeometry();
    setGeometry(screen.width() * 0.85, screen.height() * 0.1, 64, 64);
    ui->setupUi(this);
    // 取消窗体边框
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::WindowMinimizeButtonHint|Qt::Tool);
    // 设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    // 设置鼠标样式为手型
    this->setCursor(QCursor(Qt::PointingHandCursor));

    // 设置阴影
    shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(5, 5);
    shadow_effect->setColor(QColor("#888888"));
    shadow_effect->setBlurRadius(16);
    shadow_effect2 = new QGraphicsDropShadowEffect(this);
    shadow_effect2->setOffset(5, 5);
    shadow_effect2->setColor(QColor("#888888"));
    shadow_effect2->setBlurRadius(16);
    ui->popLabel2->setGraphicsEffect(shadow_effect);
    ui->popLabel->setGraphicsEffect(shadow_effect2);

    setValidState(2, "点我验证");

    // 创建一个菜单
    popMenu = new QMenu;
    popValid = new QAction(QObject::tr("Valid"), this);
    connect(popValid, SIGNAL(triggered()), this, SLOT(on_popGetAndValidAction()));
    popGetAndValid = new QAction(QObject::tr("GetAndValid"), this);
    connect(popGetAndValid, SIGNAL(triggered()), this, SLOT(on_popValidAction()));
    popShowMainWindow = new QAction(QObject::tr("ShowMainWindow"), this);
    QObject::connect(popShowMainWindow, SIGNAL(triggered()), this, SLOT(on_popShowMainWindowAction()));
    popHidden = new QAction(QObject::tr("Hidden"), this);
    connect(popHidden, SIGNAL(triggered()), this, SLOT(on_popHiddenAction()));
    popExit = new QAction(QObject::tr("Exit"), this);
    QObject::connect(popExit, SIGNAL(triggered()), this, SLOT(on_popExitAction()));

    popMenu->addAction(popValid);
    popMenu->addAction(popGetAndValid);
    popMenu->addAction(popShowMainWindow);
    popMenu->addAction(popHidden);
    popMenu->addAction(popExit);
}

Pop::~Pop()
{
    delete ui;
}

/** 重写鼠标按下事件
 * @brief Pop::mousePressEvent
 * @param event
 */
void Pop::mousePressEvent(QMouseEvent *event) {
   if(event->button() == Qt::LeftButton) {
       // 左键
       relativePos = event->globalPos() - this->pos();
   }
   if(event->button() == Qt::RightButton) {
       // 显示右键菜单
       popMenu->exec(event->globalPos());
   }
}

/** 重写鼠标移动事件
 * @brief Pop::mouseMoveEvent
 * @param event
 */
void Pop::mouseMoveEvent(QMouseEvent *event) {
    this->move(event->globalPos() - relativePos);
}

/** 重写鼠标释放事件
 * @brief Pop::mouseReleaseEvent
 * @param event
 */
void Pop::mouseReleaseEvent(QMouseEvent *event) {
    on_popValidAction();
}

/** 重写鼠标移入事件
 * @brief Pop::enterEvent
 * @param event
 */
void Pop::enterEvent(QEvent *event) {
    if(flag == 1) {
        setValidState(2, "点我验证");
    }
}

/** 重写鼠标移除事件
 * @brief Pop::leaveEvent
 * @param event
 */
void Pop::leaveEvent(QEvent *event) {
    flag = 1;
}

/** 隐藏Pop
 * @brief Pop::on_popHiddenAction
 */
void Pop::on_popHiddenAction() {
    this->hide();
}

/** 显示主界面
 * @brief Pop::on_popShowMainWindowAction
 */
void Pop::on_popShowMainWindowAction() {
    emit showMainWindowSignal();
}

/** 退出
 * @brief Pop::on_popExitAction
 */
void Pop::on_popExitAction() {
    emit exitAppSignal();
}

/** 规则校验
 * @brief Pop::on_popValidAction
 */
void Pop::on_popValidAction() {
    emit validRoleSignal();
}

/** 获取数据并规则校验
 * @brief Pop::on_popGetAndValidAction
 */
void Pop::on_popGetAndValidAction() {
    emit getAndValidRoleSignal();
}

/** 设置验证状态
 * @brief Pop::setValidState
 * @param val
 */
void Pop::setValidState(int val, QString txt) {
    pix = new QPixmap(ui->popLabel->size());
    QString borderColor, fontColor;
    switch (val) {
    case 1:
        pix->load(":/icon/right");
        borderColor = "#1296db";
        fontColor = "#1296db";
        break;
    case 0:
        pix->load(":/icon/left");
        borderColor = "#d81e06";
        fontColor = "#d81e06";
        break;
    default:
        pix->load(":/icon/check");
        borderColor = "#d4237a";
        fontColor = "#d4237a";
        break;
    }
    flag = 0;
    ui->popLabel->setPixmap(*pix);
    // 设置样式
    ui->popLabel->setStyleSheet("background-color: #dbdbdb;"
                                "border:1px solid "+borderColor+";"
                                "border-right-style: none;"
                                "border-top-left-radius:32px;"
                                "border-bottom-left-radius:32px");

    ui->popLabel2->setStyleSheet("background-color: #dbdbdb;"
                                 "border:1px solid "+borderColor+";"
                                 "border-left-style: none;"
                                 "border-top-right-radius:32px;"
                                 "border-bottom-right-radius:32px;"
                                 "font-size:16px;"
                                 "font-weight:bold;"
                                 "font-family: SimHei,Microsoft YaHei,Serif;"
                                 "color:"+fontColor);
    // 设置内容
//    ui->popLabel2->setText("<p ><font color=#d4237a>点我验证</font></p>");
    ui->popLabel2->setText(txt);
    ui->popLabel2->adjustSize();
}
