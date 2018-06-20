#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qwindow.h"
#include "qmessagebox.h"
#include "windows.h"
#include "qfile.h"
#include <QSettings>
#include <QDebug>

#include "rolemanawindow.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QKeyEvent>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

HWND hwnd;
MainWindow *m;
int index = 0;
QString windowName;
QString distClassName;
int diagnIndex;
int microIndex;
int gdescIndex;
int sexIndex;
int ismIndex;

QSqlDatabase databaseR;
QString sqlR;
QList<QMap<QString, QString>> listR;

QSystemTrayIcon *mSysTrayIcon;
QAction *showMainAction;
QAction *exitAppAction;
QMenu *mMenu;

/** 获取窗口
 * @brief getWindow
 */
BOOL getWindow()
{
    // QString 转 LPCWSTR
    hwnd = ::FindWindowW(NULL, (const wchar_t*)windowName.utf16());
    if(hwnd != NULL) {
        m->ui->statusBar->showMessage(QObject::tr("Getting window success"));
        return TRUE;
    } else {
        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("The window is not started. Please try to restart or quit and start again."));
        m->ui->statusBar->showMessage(QObject::tr("Getting window failure"), 3000);
        return FALSE;
    }
}

/** 获取所有Role
 * @brief MainWindow::queryAllRole
 */
void MainWindow::queryAllRole()
{
    // 判断连接是否存在;qt_sql_default_connection - 默认连接名称（addDatabase时的arg2可以省略）
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        databaseR = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 创建数据库连接 - 默认连接名称时可省略arg2
        databaseR = QSqlDatabase::addDatabase("QSQLITE");
        databaseR.setDatabaseName("Roles.db");
        databaseR.setUserName("louie");
        databaseR.setPassword("main");
    }
    QSqlQuery query;
    listR.clear();
    if(databaseR.open()) {
        // 查询数据
        sqlR = "select * from roles";
        if (query.exec(sqlR)) {
            while (query.next()) {
                int ID = query.value(0).toInt();
                QString ROLENAME = query.value(1).toString();
                QString IF = query.value(2).toString();
                QString THEN = query.value(3).toString();

                QMap<QString, QString> dataMap;
                dataMap.insert("ID", QString::number(ID));
                dataMap.insert("ROLENAME", ROLENAME);
                dataMap.insert("IF", IF);
                dataMap.insert("THEN", THEN);

                listR.append(dataMap);
            }
        } else {
            qDebug() << databaseR.lastError();
        }
    } else {
        // 否
        qDebug() << "Error: Failed to connect database." << databaseR.lastError();
    }
    databaseR.close();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m = this;
    // 设置作者信息
    QLabel *qLabel = new QLabel(this);
    qLabel->setText("Powered by Louiema1n.");
    ui->statusBar->addPermanentWidget(qLabel);

    // 读取配置文件
    QFile file("setting.ini");
    if(file.exists())
    {
        QSettings *configIniRead = new QSettings("setting.ini", QSettings::IniFormat);
        //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
        windowName = configIniRead->value("app/windowName").toString();
        distClassName = configIniRead->value("app/distClassName").toString();
        diagnIndex = configIniRead->value("getIndex/diagn").toInt();
        microIndex = configIniRead->value("getIndex/micro").toInt();
        gdescIndex = configIniRead->value("getIndex/gdesc").toInt();
        sexIndex = configIniRead->value("getIndex/sex").toInt();
        ismIndex = configIniRead->value("getIndex/ism").toInt();
        //读入入完成后删除指针
        delete configIniRead;
    }

    // 获取窗口数据
    getWindow();

    // 1.从数据库查询出所有规则
    queryAllRole();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** 获取并设置相应数据
 * @brief initData
 * @return
 */
BOOL CALLBACK initData(HWND hWnd, LPARAM param)
{
    if(hWnd != NULL)
    {
        // 获取当前句柄的classname
        char class_name[255];
        ::GetClassNameA(hWnd, class_name, 255);
        QString className = QString::fromLocal8Bit(class_name);
        // 匹配字符串，成功返回true, 第二个参数代表大小写敏感
        if(className.contains(distClassName, Qt::CaseSensitive))
        {
            // 获取编辑类型的句柄
            index++;
            char text[2000];
            ::SendMessageA(hWnd, WM_GETTEXT, sizeof(text), (LPARAM)text);
            QString result = QString::fromLocal8Bit(text, sizeof(text));
            if (index == diagnIndex) {
                // 病理诊断
                if(result != NULL || result != "")
                {
                    m->ui->teDiagn->setPlainText(result);
                } else {
                    m->ui->teDiagn->setPlaceholderText("未获取到数据");
                }
            }
            else if(index == microIndex){
                // 镜下所见
                if(result != NULL || result != "")
                {
                    m->ui->teMicro->setPlainText(result);
                } else {
                    m->ui->teMicro->setPlaceholderText("未获取到数据");
                }
            }
            else if(index == gdescIndex){
                // 肉眼所见
                if(result != NULL || result != "")
                {
                    m->ui->teGdesc->setPlainText(result);
                } else {
                    m->ui->teGdesc->setPlaceholderText("未获取到数据");
                }
            }
            else if(index == sexIndex){
                // 性别
                if(result != NULL || result != "")
                {
                    m->ui->leSex->setText(result);
                } else {
                    m->ui->leSex->setPlaceholderText("未获取到数据");
                }
            }
            else if(index == ismIndex){
                // 送检材料
                if(result != NULL || result != "")
                {
                    m->ui->leIsm->setText(result);
                } else {
                    m->ui->leIsm->setPlaceholderText("未获取到数据");
                }
            }
        }
        return TRUE;
    }
    return FALSE;
}

/** 获取数据
 * @brief MainWindow::on_actionGet_triggered
 */
void MainWindow::on_actionGet_triggered()
{
    index = 0;
    on_actionClear_triggered();
    if(getWindow())
    {
        ui->statusBar->showMessage("开始获取窗口数据");
        // 枚举所有句柄
        ::EnumChildWindows(hwnd, initData, (LPARAM)"param");
        ui->statusBar->showMessage("数据获取完成");
    }

}

/** 清空界面
 * @brief MainWindow::on_actionClear_triggered
 */
void MainWindow::on_actionClear_triggered()
{
    ui->leIsm->clear();
    ui->leSex->clear();
    ui->teDiagn->clear();
    ui->teGdesc->clear();
    ui->teMicro->clear();
}

/** 显示作者信息
 * @brief MainWindow::on_actionAuthor_triggered
 */
void MainWindow::on_actionAuthor_triggered()
{

    QMessageBox::information(NULL, "作者", "Louiema1n");
}

/** 规则管理
 * @brief MainWindow::on_actionRoleMana_triggered
 */
void MainWindow::on_actionRoleMana_triggered()
{
    RoleManaWindow* rmw = new RoleManaWindow(this);
    rmw->show();
}

/** 验证规则
 * @brief MainWindow::on_actionValidRole_triggered
 */
void MainWindow::on_actionValidRole_triggered()
{
    // 获取所有界面获取的数据
    QString sex, ism, diagn, gdesc, micro;
    QMap<QString, QString> map;
    QMap<QString, QString>::ConstIterator j;
    QString ID, ROLENAME, IF, THEN;
    QStringList ifs, thens;
    QString filed, condi, value;
    QString validValue;

    sex = ui->leSex->text();
    ism = ui->leIsm->text();
    diagn = ui->teDiagn->toPlainText();
    gdesc = ui->teGdesc->toPlainText();
    micro = ui->teMicro->toPlainText();
    // 2.遍历规则
    for(int i = 0; i < listR.size(); i++) {
        map = listR[i];
        j = map.constBegin();
        // 遍历Map
        while (j != map.constEnd()) {
            if(j.key() == "ID"){
                ID = j.value();
            } else if(j.key() == "ROLENAME") {
                ROLENAME = j.value();
            } else if(j.key() == "IF") {
                IF = j.value();
            } else if(j.key() == "THEN") {
                THEN = j.value();
            }
            ++j;
        }
        // 处理IF
        if(IF.isEmpty() || IF == "") {
            goto validThen;
        }
        ifs = IF.split(" ");
        filed = ifs[0];     // 字段
        condi = ifs[1];     // 条件
        value = ifs[2];     // 值
        // 处理filed
        if(filed == "sex" || filed == "性别") {
            validValue = sex;
        }
        if(filed == "ism" || filed == "送检材料") {
            validValue = ism;
        }
        if(filed == "diagn" || filed == "病理诊断") {
            validValue = diagn;
        }
        if(filed == "gdesc" || filed == "肉眼所见") {
            validValue = gdesc;
        }
        if(filed == "micro" || filed == "镜下所见") {
            validValue = micro;
        }
        ui->statusBar->showMessage("恭喜，未触发任何规则。");

        // 验证IF like
        if(condi == "like" || condi == "包含") {
            if(validValue.contains(value)) {
                // 2.1 满足IF, 验证THEN
                goto validThen;
            }
        }
        // 验证IF =
        if(condi == "=") {
            if(validValue == value) {
                // 2.1 满足IF, 验证THEN
                validThen:
                thens = THEN.split(";");
                QString singelThen, singelValue;
                QStringList singelThens, singelValues;
                for(int k = 0; k < thens.size(); k++) {
                    singelThen = thens[k];
                    singelThens = singelThen.split(" ");
                    filed = singelThens[0];     // 字段
                    condi = singelThens[1];     // 条件
                    value = singelThens[2];     // 值

                    singelValues = value.split(",");

                    // 处理filed
                    if(filed == "sex" || filed == "性别") {
                        validValue = sex;
                    }
                    if(filed == "ism" || filed == "送检材料") {
                        validValue = ism;
                    }
                    if(filed == "diagn" || filed == "病理诊断") {
                        validValue = diagn;
                    }
                    if(filed == "gdesc" || filed == "肉眼所见") {
                        validValue = gdesc;
                    }
                    if(filed == "micro" || filed == "镜下所见") {
                        validValue = micro;
                    }
                    // 验证THEN =
                    for(int l = 0; l < singelValues.size(); l++) {
                        singelValue = singelValues[l];
                        if(condi == "=" || condi == "等于") {
                            if(validValue != singelValue) {
                                // 2.1.1 满足THEN, 触发当前规则，弹出提示
                                QMessageBox::warning(this, "警告", filed + " 未等于 " +  singelValue);
                                ui->statusBar->showMessage("失败！触发" + ROLENAME + ID);
                            }
                        }
                        if(condi == "!=" || condi == "不等于") {
                            if(validValue == singelValue) {
                                // 2.1.1 满足THEN, 触发当前规则，弹出提示
                                QMessageBox::warning(this, "警告", filed + " 等于了 " +  singelValue);
                                ui->statusBar->showMessage("失败！触发" + ROLENAME + ID);
                            }
                        }
                        // 验证THEN like
                        if(condi == "like" || condi == "包含") {
                            if(!validValue.contains(singelValue)) {
                                // 2.1.1 满足THEN, 触发当前规则，弹出提示
                                QMessageBox::warning(this, "警告", filed + " 未" +  condi + " " +  singelValue);
                                ui->statusBar->showMessage("失败！触发" + ROLENAME + ID);
                            }
                        }
                        // 验证THEN notlike
                        if(condi == "notlike" || condi == "不包含") {
                            if(validValue.contains(singelValue)) {
                                // 2.1.1 满足THEN, 触发当前规则，弹出提示
                                QMessageBox::warning(this, "警告", filed + " 包含了 " +  singelValue);
                                ui->statusBar->showMessage("失败！触发" + ROLENAME + ID);
                            }
                        }

                    }
                }
            }
        }
  }
}

/** 处理最小化窗口
 * @brief MainWindow::changeEvent
 */
void MainWindow::changeEvent(QEvent *) {
    if(this->windowState() == Qt::WindowMinimized) {
        // 最小化
        // 隐藏主界面
        this->hide();
        // 创建QSystemTrayIcon对象
        mSysTrayIcon = new QSystemTrayIcon(this);
        // 设置图标
        QIcon icon = QIcon(":/icon/appIcon");
        mSysTrayIcon->setIcon(icon);
        // 当鼠标移动到托盘上的图标时，会显示此处设置的内容
        mSysTrayIcon->setToolTip(QObject::tr("KMCSAssistant running hidden"));

        // 添加槽函数
        connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activeedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
        createActions();
        createMenu();

        // 显示
        mSysTrayIcon->show();

        // 显示消息球 - 第一个参数是标题 - 第二个参数是消息内容 - 第三个参数图标 - 第四个参数是超时毫秒数
        mSysTrayIcon->showMessage("Tips",
                                  "KMCSAssistant will running in the background.Please click the icon to display the main window.",
                                  QSystemTrayIcon::Information,
                                  1000);
    }
}

/** 槽函数 - 托盘图标操作
 * @brief MainWindow::on_activeedSysTrayIcon
 * @param reason
 */
void MainWindow::on_activeedSysTrayIcon(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        // 单击 - 显示主界面
        this->showNormal();
        this->activateWindow();
        mSysTrayIcon->hide();
        break;
    case QSystemTrayIcon::DoubleClick:
        // 双击
        break;
    default:
        break;
    }
}

/** 创建菜单动作
 * @brief MainWindow::createActions
 */
void MainWindow::createActions() {
    showMainAction = new QAction(QObject::tr("Show MainWindow"));
    connect(showMainAction, SIGNAL(triggered()), this, SLOT(on_showMainAction()));

    exitAppAction = new QAction(QObject::tr("Exit App"));
    connect(exitAppAction, SIGNAL(triggered()), this, SLOT(on_exitAppAction()));
}

/** 创建菜单
 * @brief MainWindow::createMenu
 */
void MainWindow::createMenu() {
    mMenu = new QMenu(this);
    // 增加显示主界面
    mMenu->addAction(showMainAction);
    // 添加分隔符
    mMenu->addSeparator();
    // 添加退出程序
    mMenu->addAction(exitAppAction);
    // 添加菜单
    mSysTrayIcon->setContextMenu(mMenu);
}

/** 显示主界面
 * @brief MainWindow::on_showMainAction
 */
void MainWindow::on_showMainAction() {
    this->showNormal();
    this->activateWindow();
    mSysTrayIcon->hide();
}

/** 退出程序
 * @brief MainWindow::on_exitAppAction
 */
void MainWindow::on_exitAppAction() {
    mSysTrayIcon->hide();
    exit(0);
}

/** 处理全局热键
 * @brief MainWindow::hotKeyActivated
 * @param event
 */
void MainWindow::hotKeyActivated() {
    qDebug() << "你按下了快捷键";
}
