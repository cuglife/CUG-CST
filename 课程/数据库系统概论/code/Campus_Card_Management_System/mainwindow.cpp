#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>
#include <QMessageBox>
#include <QSqlDriver>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

void MainWindow::createConnection()
{
    if (QSqlDatabase::contains(HostNameAPort))
        db = QSqlDatabase::database(HostNameAPort);
    else{
        db = QSqlDatabase::addDatabase("QMYSQL", HostNameAPort);
//        db = QSqlDatabase::addDatabase("QODBC");
        db.setHostName(HostName);
        db.setPort(Port);
        db.setDatabaseName(DatabaseName);
        if (!db.open(UserName,Password)){
            QMessageBox::critical(nullptr, QObject::tr("无法打开数据库"), "无法创建数据库连接！ ", QMessageBox::Cancel);
            qDebug()<<db.lastError().text();
        }
        else {
            qDebug()<<"success";
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Return:{       //数字键盘Enter键
        on_button_login_clicked();
        break;
    }
    case Qt::Key_Enter: {       //Enter键
        on_button_login_clicked();
        break;
    }
    default:
        break;
    }
}

void MainWindow::delay(int mSec)                            //延迟登录
{
    QEventLoop loop;
    QTimer::singleShot(mSec, &loop, SLOT(quit()));
    loop.exec();
}

bool MainWindow::login(QString account, QString password)
{
    if(student_login(account, password)==1){
        opStuFunc.show();
        return true;
    }

    int adminType = admin_login(account, password);
    if(adminType==1){
        opStudent.show();
        return true;
    }

    if(adminType==2){
        opAdmin.show();
        return true;
    }

    return false;
}

int MainWindow::admin_login(QString account, QString password)
{
    QSqlQuery *query=new QSqlQuery(db);
    query->prepare("select * from admin_infor where admin_account=:account and admin_password=:password");
    query->bindValue(":account",account);
    query->bindValue(":password",password);
    query->exec();


    if(query->last()&&query->at()>=0&&query->first()){
        QString admin_type = query->value(5).toString();
        if(admin_type=="学生信息管理")
            return 1;
        else
            return 2;
    }

//    if(query->size()>=1&&query->next()){
//        QString admin_type = query->value(5).toString();
//        if(admin_type=="学生信息管理")
//            return 1;
//        else
//            return 2;
//    }
        return 0;



}

int MainWindow::student_login(QString account, QString password)
{
    QSqlQuery *query=new QSqlQuery(db);
    query->prepare("select * from student_infor where student_account=:account and student_password=:password");
    query->bindValue(":account",account);
    query->bindValue(":password",password);
    query->exec();

    if(query->last()&&query->at()>=0)
        return 1;

//    if(query->size()>=1)
//        return 1;
    return 0;
}

void MainWindow::on_button_login_clicked()
{
    QString isLogin=this->ui->button_login->text();

    if(isLogin=="登录"){
        account=this->ui->line_account->text();
        QString password=this->ui->line_password->text();
        if(account.isEmpty()||password.isEmpty()){
            QMessageBox::critical(this,"错误","请您输入账号和密码后再登录","确认");
        }
        else{
//判断账号密码是否相符
           this->ui->button_login->setText("登录中...");
           delay(500);

           if(!login(account,password)){
               QMessageBox::critical(this,"错误","你输入的账户名或密码不正确，原因可能是：\n1、账户名输入有误；\n2、忘记密码；\n3、未区分字母大小写；\n4、未开启小键盘。\n\n如果你的密码丢失或遗忘，可寻找管理员找回密码。\n","确认");
               this->ui->button_login->setText("登录");
           }
           else
              this->hide();
        }
    }
}
