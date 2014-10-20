#ifndef CONNECTION_H
#define CONNECTION_H

#include<QtSql/QSql>
#include <QtSql>
#include <QDebug>
#include "qdom.h"

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName("easybook-3313b0");
    db.setDatabaseName("data.db");
    db.setUserName("PowerSystem");
    db.setPassword("123456");
    if(!db.open()){
        //提示出错
        return false;
    }

    QSqlQuery query;

    //创建系统表
    query.exec("create table systems(id varchar primary key,name varchar)");
    query.exec(QString("insert into systems values('0','请选择系统')"));
    query.exec(QString("insert into systems values('01','24节点系统')"));
    query.exec(QString("insert into systems values('02','30节点系统')"));
    query.exec(QString("insert into systems values('03','118节点系统')"));

    //创建策略表
    query.exec("create table brand(id varchar primary key,name varchar,systems varchar,price int,sum int,sell int,last int)");
    query.exec(QString("insert into brand values('01','穷举','24节点系统',36,50,10,40)"));
    query.exec(QString("insert into brand values('05','穷举','30节点系统',39,50,15,35)"));
    query.exec(QString("insert into brand values('06','重载','30节点系统',28,60,10,50)"));
    query.exec(QString("insert into brand values('07','裕度','30节点系统',27,70,20,50)"));
    query.exec(QString("insert into brand values('08','穷举','118节点系统',25,75,25,50)"));
//    query.exec(QString("insert into brand values('09','帕萨特','118节点系统',27,65,20,45)"));

    //创建密码表
    query.exec("create table password(pwd varchar primary key)");
    query.exec("insert into password values('123456')");

    return true;
}

static bool createXml()
{
    QFile file("data.xml");
    if(file.exists())return true;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return false;
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement(QString("创建系统"));
    doc.appendChild(root);
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return true;
}

#endif // CONNECTION_H
