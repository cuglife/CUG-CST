#include "deposit.h"
#include "ui_deposit.h"

deposit::deposit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deposit)
{
    ui->setupUi(this);
}

deposit::~deposit()
{
    delete ui;
    db.close();
}

void deposit::showEvent(QShowEvent *)
{
    createConnection();
}

void deposit::createConnection()
{
    if (QSqlDatabase::contains(HostNameAPort))
        db = QSqlDatabase::database(HostNameAPort);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", HostNameAPort);

    db.setHostName(HostName);
    db.setPort(Port);
    db.setDatabaseName(DatabaseName);

    if (!db.open(UserName,Password))
        QMessageBox::critical(nullptr, QObject::tr("无法打开数据库"), "无法创建数据库连接！ ", QMessageBox::Cancel);
}

void deposit::refresh()
{
    QString account =this->ui->line_stu_id->text();
    QSqlQuery *query=new QSqlQuery(db);
    query->prepare("select * from card_infor where student_account=:account");
    query->bindValue(":account",account);
    query->exec();
    if(query->size()>=1&&query->next()){
        this->ui->line_balance->setText(query->value(3).toString());
        de_card_id = query->value(0).toString();
        if(query->value(2).toString()=="正常"){
            this->ui->button_ok->setEnabled(true);
        }

        if(query->value(2).toString()=="挂失")
            QMessageBox::information(this,"通知","校园卡处于挂失状态，无法充值！","确认");
    }
    else
        QMessageBox::information(this,"通知","未查询到校园卡信息，请办卡！","确认");
}

void deposit::on_button_ok_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString _account = this->ui->line_stu_id->text();
    float count = this->ui->line_count->text().toFloat();

    if(count>0){
        query->prepare("update card_infor set balance=balance+? where student_account=?");

        query->addBindValue(this->ui->line_count->text().toFloat());
        query->addBindValue(this->ui->line_stu_id->text());

        bool isDeposited = query->exec();
        if(isDeposited){
            QMessageBox::information(this,"通知","充值成功！","确认");

            query->prepare("insert into deposit_infor(card_id, admin_account, recharge_amount, recharge_time, balance) "
                              "values (?, ?, ?, ?, ?)");

            qDebug()<<de_card_id;
            qDebug()<<account;
            qDebug()<<this->ui->line_count->text().toFloat();
            qDebug()<<this->ui->line_balance->text().toFloat()+this->ui->line_count->text().toFloat();

            query->addBindValue(de_card_id);
            query->addBindValue(account);
            query->addBindValue(this->ui->line_count->text().toFloat());
            query->addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query->addBindValue(this->ui->line_balance->text().toFloat()+this->ui->line_count->text().toFloat());

            bool isDepositInfo = query->exec();

            if(!isDepositInfo){
                QMessageBox::information(this,"通知","充值信息添加失败！","确认");
            }
            refresh();
        }
        else {
            qDebug()<<query->lastError();
            QMessageBox::information(this,"通知","充值失败！","确认");
        }

    }
    else {
        QMessageBox::critical(this,"错误","充值数额错误，请检查","确定");
    }
}

void deposit::on_line_stu_id_textChanged(const QString &account)
{
    this->ui->button_ok->setDisabled(true);
    if(isback){
        QString _account=account;
        this->ui->line_stu_id->setText(_account);
        isback=false;
    }
    if(account.length()==10){
        isback=true;
        QSqlQuery *query=new QSqlQuery(db);

        query->prepare("select * from student_infor where student_account=:account");
        query->bindValue(":account",account);
        query->exec();
        if(query->size()>=1&&query->next())
            this->ui->line_name->setText(query->value(3).toString());
        else {
            this->ui->button_ok->setDisabled(true);
            QMessageBox::information(this,"通知","未找到相关学生信息无法进行操作！","确认");
            return;
        }
        refresh();
    }
}
