#include "rolemanawindow.h"
#include "ui_rolemanawindow.h"

#include "QStandardItemModel.h"
#include "qfile.h"
#include <qdebug.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <roleformwindow.h>
#include <qmessagebox.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

QStandardItemModel *model;
QList<QMap<QString, QString>> list;

QSqlDatabase database;
QString sql;

RoleManaWindow::RoleManaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoleManaWindow)
{
    ui->setupUi(this);
    initTableView();
    initDataBase();

    queryAllRole();
}

/** 初始化表格
 * @brief RoleManaWindow::initTableView
 */
void RoleManaWindow::initTableView()
{
    // 表格
    model = new QStandardItemModel();
    // 表头
    model->setColumnCount(4);   // 列数
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ROLENAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("IF"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("THEN"));
    ui->tvRoles->setModel(model);
    // 属性设置
    ui->tvRoles->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);   // 列宽随内容调节
    ui->tvRoles->setEditTriggers(QAbstractItemView::NoEditTriggers);    // 行内不可编辑
}

/** 初始化数据库
 * @brief RoleManaWindow::initDataBase
 */
void RoleManaWindow::initDataBase() {
    // 判断连接是否存在;qt_sql_default_connection - 默认连接名称（addDatabase时的arg2可以省略）
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 创建数据库连接 - 默认连接名称时可省略arg2
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Roles.db");
        database.setUserName("louie");
        database.setPassword("main");
    }
}

/** 查询所有Role
 * @brief RoleManaWindow::queryAllRole
 */
void RoleManaWindow::queryAllRole() {
    QSqlQuery query;
    list.clear();
    if(database.open()) {
        // 查询数据
        sql = "select * from roles";
        if (query.exec(sql)) {
            while (query.next()) {
                int ID = query.value(0).toInt();
                QString ROLENAME = query.value(1).toString();
                QString IF = query.value(2).toString();
                QString THEN = query.value(3).toString();

                QMap<QString, QString> dataMap;
                dataMap.insert("ID", QString::number(ID));
                dataMap.insert("ROLENAME", ROLENAME + QString::number(ID));
                dataMap.insert("IF", IF);
                dataMap.insert("THEN", THEN);

                list.append(dataMap);
                // 设置表格数据
                setTableData();
            }
        } else {
            qDebug() << database.lastError();
        }
    } else {
        // 否
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    database.close();
}

/** 设置表格数据
 * @brief setTableData
 * @param model
 */
void RoleManaWindow::setTableData() {
    // 数据
    for(int i = 0; i < list.size(); i++)
    {
        QMap<QString, QString> map = list[i];
        QMap<QString, QString>::ConstIterator j = map.constBegin();
        QString ID, ROLENAME, IF, THEN;
        // 遍历
        while (j != map.constEnd()) {
            if(j.key() == "ID"){
                ID = j.value();
            } else
            if(j.key() == "ROLENAME") {
                ROLENAME = j.value();
            } else if(j.key() == "IF") {
                IF = j.value();
            } else if(j.key() == "THEN") {
                THEN = j.value();
            }
            ++j;
        }
        // arg1 - 行数，arg2 - 列数，arg3 - 内容
        model->setItem(i, 0, new QStandardItem(ID));
        // 设置字符颜色
        model->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
        // 设置字符位置
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, 1, new QStandardItem(ROLENAME));
        model->setItem(i, 2, new QStandardItem(IF));
        model->setItem(i, 3, new QStandardItem(THEN));
    }
}

RoleManaWindow::~RoleManaWindow()
{
    delete ui;
}

//** 读取json - roles
// * @brief RoleManaWindow::on_pushButton_clicked
// */
//void RoleManaWindow::on_pushButton_clicked()
//{
//    QFile file;
//    file.setFileName("role.json");
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    QString text = file.readAll();
//    file.close();

//    //    qDebug("Test:%d",id); //（%d表示整数）
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(text.toUtf8());
//    QJsonObject jsonObj = jsonDoc.object();
//    QJsonValue ID = jsonObj.value(QString("ID"));
//    QJsonValue ROLENAME = jsonObj.value(QString("ROLENAME"));
//    QJsonValue CONTENT = jsonObj.value(QString("CONTENT"));

//    QMap<QString, QString> dataMap;
//    dataMap.insert("ID", ID.toString());
//    dataMap.insert("ROLENAME", ROLENAME.toString());
//    dataMap.insert("CONTENT",  CONTENT.toString());

//    list.append(dataMap);
//    // 设置表格数据
//    setTableData();
//}

/** 新增
 * @brief RoleManaWindow::on_actionAdd_triggered
 */
void RoleManaWindow::on_actionAdd_triggered()
{
    RoleFormWindow* rfw = new RoleFormWindow(this);
    rfw->show();
}

/** 清空表格数据
 * @brief RoleManaWindow::on_actionClear_triggered
 */
void RoleManaWindow::on_actionClear_triggered()
{
    model->removeRows(0, model->rowCount());
}

/** 刷新
 * @brief RoleManaWindow::on_actionRefresh_triggered
 */
void RoleManaWindow::on_actionRefresh_triggered()
{
    queryAllRole();
}

/** 删除role
 * @brief RoleManaWindow::on_actionDelete_triggered
 */
void RoleManaWindow::on_actionDelete_triggered()
{
    QSqlQuery query;
    if(database.open()) {
        // 删除数据
        int row = ui->tvRoles->currentIndex().row();
        QModelIndex index = model->index(row, 0);
        QString id = model->data(index).toString();
        // 删除model
        model->removeRow(row);
        sql = "delete from roles where ID = " + id;
        if(query.exec(sql)) {
            QMessageBox::information(this, "温馨提示", "删除成功");
        } else {
            QMessageBox::warning(this, "失败", "删除失败");
        }
    } else {
        // 否
        QMessageBox::warning(this, "失败", "数据库打开失败");
    }
    database.close();
}
