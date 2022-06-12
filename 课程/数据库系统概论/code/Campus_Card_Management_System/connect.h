#ifndef CONNECT_H
#define CONNECT_H

#include <QMessageBox>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>

#include <QRegExp>
#include <QValidator>
#include <QDebug>

const QString HostName = "wecug.cn";
const QString HostNameAPort = "wecug.cn@3306";
const QString UserName = "guest";

//const QString HostName = "localhost";
//const QString HostNameAPort = "localhost@3306";
//const QString UserName = "root";

const QString DatabaseName = "campus_card_system";

const QString Password = "wewings";
const int     Port = 3306;

const int depositCol = 5;
const int operateCol = 4;

const QString depositWay[depositCol] = {"校园卡卡号", "学生学号", "充值人账号","充值时间", "充值金额"};
const QString operateWay[operateCol] = {"管理员账号", "学生学号", "操作时间"  ,"操作类型" };

const QString depositDB[depositCol] = {"校园卡卡号", "充值人账号", "充值金额","充值时间", "余额"};
const QString operateDB[operateCol] = {"管理员账号", "学生学号", "操作时间"  ,"操作类型" };


extern QString account;

#endif // CONNECT_H
