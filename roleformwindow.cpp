#include "roleformwindow.h"
#include "ui_roleformwindow.h"

#include "qmessagebox.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

QSqlDatabase databaseF;
QString sqlF;

int flag;
QString IF, THEN;

RoleFormWindow::RoleFormWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoleFormWindow)
{
    ui->setupUi(this);

    ui->cbFiled->addItem(QObject::tr("sex"));
    ui->cbFiled->addItem(QObject::tr("ism"));
    ui->cbFiled->addItem(QObject::tr("gdesc"));
    ui->cbFiled->addItem(QObject::tr("micro"));
    ui->cbFiled->addItem(QObject::tr("diagn"));

    ui->cbCondi->addItem("=");
    ui->cbCondi->addItem(QObject::tr("like"));

    ui->cbFiledThen->addItem(QObject::tr("sex"));
    ui->cbFiledThen->addItem(QObject::tr("ism"));
    ui->cbFiledThen->addItem(QObject::tr("gdesc"));
    ui->cbFiledThen->addItem(QObject::tr("micro"));
    ui->cbFiledThen->addItem(QObject::tr("diagn"));

    ui->cbCondiThen->addItem("=");
    ui->cbCondiThen->addItem(QObject::tr("like"));
    ui->cbCondiThen->addItem(QObject::tr("notlike"));
    flag = 0;
    IF = "";
    THEN = "";
}

/** 初始化数据库
 * @brief RoleFormWindow::initDataBase
 */
void RoleFormWindow::initDataBase() {
    // 判断连接是否存在;qt_sql_default_connection - 默认连接名称（addDatabase时的arg2可以省略）
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        databaseF = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 创建数据库连接 - 默认连接名称时可省略arg2
        databaseF = QSqlDatabase::addDatabase("QSQLITE");
        databaseF.setDatabaseName("Roles.db");
        databaseF.setUserName("louie");
        databaseF.setPassword("main");
    }
}

RoleFormWindow::~RoleFormWindow()
{
    delete ui;
}

/** 添加预览
 * @brief RoleFormWindow::on_addThen_clicked
 */
void RoleFormWindow::on_addThen_clicked()
{
    QString ifFiled, ifCondi, ifValue, thenFiled, thenCondi, thenValue, THENR;
    ifValue = ui->leValue->text();
    thenValue = ui->teValueThen->toPlainText();

    if(ifValue == NULL || ifValue == "") {
        QMessageBox::warning(this, "警告", "请输入条件值");
        return;
    } else {
        if(flag == 0) {
            flag = 1;
            // 1.生成IF
            ifFiled = ui->cbFiled->currentText();
            ifCondi = ui->cbCondi->currentText();
            IF = ifFiled + " " + ifCondi + " "  + ifValue;
            // 3.生成review
            ui->teReview->setPlainText(QObject::tr("if") + "\n" + IF + "\n" + QObject::tr("then") + "\n" + THEN);
        }
    }
    // 2.生成THEN
    if(thenValue == NULL || thenValue == "") {
        QMessageBox::warning(this, "警告", "请输入条件值");
    } else {
        thenFiled = ui->cbFiledThen->currentText();
        thenCondi = ui->cbCondiThen->currentText();
        THEN += thenFiled + " " + thenCondi + " "  + thenValue + ";";
        // 去除最后一个;
        THENR = THEN.mid(0, THEN.length() - 1);
        // 3.生成review
        ui->teReview->setPlainText(QObject::tr("if") + "\n" + IF + "\n" + QObject::tr("then") + "\n" + THENR);
    }

}

/** 保存role
 * @brief RoleFormWindow::on_actionSave_S_triggered
 */
void RoleFormWindow::on_actionSave_S_triggered()
{
    initDataBase();
    QSqlQuery query;
    QString THENT;
    if(databaseF.open()) {
        // 插入数据 - 也可以使用addBindValue赋值
        THENT = THEN.mid(0, THEN.length() - 1);
        sqlF = "insert into roles('ROLENAME', 'IFR', 'THENR') values('role', '"+IF+"', '"+THENT+"')";
        if(query.exec(sqlF)) {
            QMessageBox::information(this, "温馨提示", "添加成功");
        } else {
            QMessageBox::warning(this, "失败", "添加失败");
        }
    } else {
        // 否
        QMessageBox::warning(this, "失败", "数据库打开失败");
    }
    databaseF.close();
}
