#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <qdebug.h>
#include <qfile.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载国际化
    QTranslator translator;
    translator.load(":/i18n/zh_CN");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    int firstBoot = 1;

    // 判断setting.ini是否存在
    QFile file("setting.ini");
    // arg1 - 配置文件路径，arg2 - 针对ini文件
    QSettings *configIni = new QSettings("setting.ini", QSettings::IniFormat);
    if(!file.exists())
    {
        // 使用QSetting写配置文件
        qDebug() << "Create setting.ini";

        //向ini文件中写入内容,setValue函数的两个参数是键值对
        configIni->setValue("app/firstBoot", 0);

        //向ini文件的第一个节写入内容,ip节下的第一个参数
        configIni->setValue("app/windowName", "组织病理结果处理");
        //向ini文件的第一个节写入内容,ip节下的第二个参数
        //        configIni->setValue("app/distClassName", "WindowsForms10.EDIT.app.0.2f5a4f0_r9_ad1");
        configIni->setValue("app/distClassName", "WindowsForms10.EDIT.app");
        //向ini文件的第二个节写入内容,port节下的第一个参数
        configIni->setValue("getIndex/diagn", "1");
        configIni->setValue("getIndex/micro", "5");
        configIni->setValue("getIndex/gdesc", "13");
        configIni->setValue("getIndex/sex", "23");
        configIni->setValue("getIndex/ism", "31");
        //写入完成后删除指针
        delete configIni;
    } else {
        firstBoot = configIni->value("app/firstBoot").toInt();
    }

    // 第一次启动, 创建Roles数据库
    if(firstBoot == 1) {
        // 1.创建数据库
        QSqlDatabase database;
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
        // 2.打开连接
        if(!database.open()) {
            // 否
            qDebug() << "Error: Failed to connect database." << database.lastError();
        } else {
            // 是 - 3.操作数据库
            // 创建表
            QSqlQuery query;
            QString createSql = "create table roles (ID INTEGER PRIMARY KEY AUTOINCREMENT, ROLENAME varchar(255), IFR varchar(255), THENR varchar(255))";
            // 执行sql
            if(query.exec(createSql)) {
                qDebug() << "Table roles Created.";
            } else {
                qDebug() << "Error: Fail to create table." << database.lastError();
            }
        }
        database.close();
    }
    return a.exec();
}
