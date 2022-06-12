#include "stu_func.h"
#include "ui_stu_func.h"


stu_func::stu_func(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stu_func)
{
    ui->setupUi(this);
    this->model = new QStandardItemModel;
    this->ui->tableView->setModel(model);
}

stu_func::~stu_func()
{
    delete ui;
    db.close();
}


void stu_func::showEvent(QShowEvent *)
{
    createConnection();
    init();
}

void stu_func::createConnection()
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

void stu_func::init()
{
    interfaceSet();
    freshTableView();
}

void stu_func::interfaceSet()
{
    this->ui->btn_deposit->setDisabled(true);

    QSqlQuery *query=new QSqlQuery(db);
    query->exec("select * from card_infor where student_account = " + account);
    if(query->size()>0 && query->next()){
        this->ui->line_balance->setText(query->value(3).toString());
        de_card_id = query->value(0).toString();

        if(query->value(2).toString()=="正常")
            this->ui->btn_deposit->setEnabled(true);
        if(query->value(2).toString()=="挂失")
            QMessageBox::information(this,"通知","校园卡处于挂失状态，无法充值！","确认");

    }
    else
        QMessageBox::information(this,"通知","未查询到校园卡信息，请办卡！","确认");
}

void stu_func::freshTableView()
{
    this->model->clear();
    this->ui->tableView->resize(1081,591);

    for (int i = 0; i < 5; i++){
        this->model->setHorizontalHeaderItem(i,new QStandardItem(depositDB[i]));
        this->ui->tableView->setColumnWidth(i,216);
    }

    QSqlQuery *query=new QSqlQuery(db);
    query->exec("select * from deposit_infor where deposit_infor.card_id = (select card_id from card_infor where student_account = " + account+")");

    if(query->size()>0){
        for (int i =0; query->next(); i++) {
            for (int k = 0; k < depositCol; k++){
                this->model->setItem(i,k,new QStandardItem(query->value(k).toString()));
                this->model->item(i,k)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
    else
        QMessageBox::information(this,"通知","未查询到充值信息！","确认");
}

void stu_func::on_btn_deposit_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);

    float rechargeAmount = this->ui->line_count->text().toFloat();
    float _balance = this->ui->line_balance->text().toFloat();

    query->prepare("update card_infor set balance=balance+? where student_account=?");
    query->addBindValue(rechargeAmount);
    query->addBindValue(account);
    if(query->exec()){
        QMessageBox::information(this,"通知","充值成功！","确认");

        query->prepare("insert into deposit_infor(card_id, admin_account, recharge_amount, recharge_time, balance) values (?, ?, ?, ?, ?)");

        query->addBindValue(de_card_id);
        query->addBindValue(account);
        query->addBindValue(rechargeAmount);
        query->addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query->addBindValue(_balance + rechargeAmount);

        if(query->exec())
            freshTableView();
        else
            QMessageBox::information(this,"通知","充值信息添加失败！","确认");

        interfaceSet();
    }
    else {
        qDebug()<<query->lastError();
        QMessageBox::information(this,"通知","充值失败！","确认");
    }

}

void stu_func::on_btn_change_password_clicked()
{
    QString password = this->ui->line_password->text();
    QString passwordConfirm = this->ui->line_password_confirm->text();
    if(password.length()<6||passwordConfirm.length()<6){
        QMessageBox::critical(nullptr, QObject::tr("警告"),
        "密码较弱！ ", QMessageBox::Cancel);
        return;
    }

    if(password!=passwordConfirm){
        QMessageBox::critical(nullptr, QObject::tr("警告"),
        "输入的两次密码不同！ ", QMessageBox::Cancel);
        return;
    }

    int ret=QMessageBox::question(this,"请确认","请确认修改密码","确认","取消");
    if(ret==0){
        QSqlQuery *query=new QSqlQuery(db);

        query->prepare("update student_infor set student_password=? where student_account=?");

        query->addBindValue(password);
        query->addBindValue(account);

        if(query->exec()){
            QMessageBox::information(this,"通知","修改密码成功！","确认");
        }
        else {
            qDebug()<<query->lastError();
            QMessageBox::information(this,"通知","修改密码失败！","确认");
        }
    }

}
