#include "card_status.h"
#include "ui_card_status.h"

card_status::card_status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::card_status)
{
    ui->setupUi(this);
}

card_status::~card_status()
{
    delete ui;
    db.close();
}

void card_status::showEvent(QShowEvent *)
{
    createConnection();
}

void card_status::createConnection()
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

void card_status::setButtons(QString account)
{
    QSqlQuery *query=new QSqlQuery(db);

    query->prepare("select * from student_infor where student_account=:account");
    query->bindValue(":account",account);
    query->exec();
    if(query->size()>=1&&query->next()){
        this->ui->btn_getCard->setEnabled(true);
        this->ui->btn_makeUpCard->setEnabled(true);
        this->ui->btn_reportLoss->setEnabled(true);
        this->ui->btn_cancelReportLoss->setEnabled(true);
        this->ui->btn_logoutCard->setEnabled(true);

        this->ui->line_id->setText(query->value(0).toString());
        this->ui->line_stu_id->setText(query->value(1).toString());
        this->ui->line_name->setText(query->value(3).toString());
    }
    else {
        this->ui->btn_getCard->setDisabled(true);
        this->ui->btn_makeUpCard->setDisabled(true);
        this->ui->btn_reportLoss->setDisabled(true);
        this->ui->btn_cancelReportLoss->setDisabled(true);
        this->ui->btn_logoutCard->setDisabled(true);
        QMessageBox::information(this,"通知","未找到相关学生信息无法进行操作！","确认");
        return;
    }

    query->prepare("select * from card_infor where student_account=:account");
    query->bindValue(":account",account);
    query->exec();
    if(query->size()>=1&&query->next()){

        this->ui->btn_getCard->setDisabled(true);
        this->ui->btn_makeUpCard->setEnabled(true);
        this->ui->btn_logoutCard->setEnabled(true);

        if(query->value(2).toString()=="正常"){
            this->ui->btn_reportLoss->setEnabled(true);
            this->ui->btn_cancelReportLoss->setDisabled(true);
        }

        if(query->value(2).toString()=="挂失"){
            this->ui->btn_reportLoss->setDisabled(true);
            this->ui->btn_cancelReportLoss->setEnabled(true);
        }
    }
    else {
        this->ui->btn_getCard->setEnabled(true);
        this->ui->btn_makeUpCard->setDisabled(true);
        this->ui->btn_logoutCard->setDisabled(true);
        this->ui->btn_reportLoss->setDisabled(true);
        this->ui->btn_cancelReportLoss->setDisabled(true);
    }
}

QString card_status::randCardID()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString randCardID;
    int randomx = 0;
    for (int i = 0; i < 10; i++){
        randomx= rand() % (36);
        randCardID += chars[randomx];
    }
    qDebug()<<randCardID;
    return randCardID;
}

bool card_status::insertStatusInfor(QString stuAccount, QString time, QString operateType)
{
    QSqlQuery *query=new QSqlQuery(db);
    query->prepare("insert into card_status_infor(admin_account, student_account, operate_time, operate_type) values (?, ?, ?, ?)");

    query->addBindValue(account);
    query->addBindValue(stuAccount);
    query->addBindValue(time);
    query->addBindValue(operateType);

    return query->exec();

}

void card_status::giveInfor(bool isExec, QString stuAccount, QString time, QString operateType)
{
    if(isExec){
        QMessageBox::information(this,"通知",operateType+"成功！","确认");
        if(!insertStatusInfor(stuAccount, time, operateType))
            QMessageBox::information(this,"通知","校园卡操作信息提交失败！","确认");
        setButtons(stuAccount);
    }
    else {
        QMessageBox::information(this,"通知",operateType+"失败！","确认");
    }
}

void card_status::on_btn_getCard_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString stuAccount = this->ui->line_stu_id->text();

    query->prepare("insert into card_infor(card_id, student_account, card_status, balance) values (?, ?, ?, ?)");
    query->addBindValue(randCardID());
    query->addBindValue(stuAccount);
    query->addBindValue("正常");
    query->addBindValue(0);

    giveInfor(query->exec(),stuAccount,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "办卡");
}

void card_status::on_btn_makeUpCard_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString stuAccount = this->ui->line_stu_id->text();

    query->prepare("update card_infor set card_id=?, card_status=? where student_account=?");
    query->addBindValue(randCardID());
    query->addBindValue("正常");
    query->addBindValue(stuAccount);

    giveInfor(query->exec(),stuAccount,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "补卡");
}

void card_status::on_btn_reportLoss_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString stuAccount = this->ui->line_stu_id->text();

    query->prepare("update card_infor set card_status=? where student_account=?");
    query->addBindValue("挂失");
    query->addBindValue(stuAccount);

    giveInfor(query->exec(),stuAccount,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "挂失");
}

void card_status::on_btn_cancelReportLoss_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString stuAccount = this->ui->line_stu_id->text();

    query->prepare("update card_infor set card_status=? where student_account=?");
    query->addBindValue("正常");
    query->addBindValue(this->ui->line_stu_id->text());

    giveInfor(query->exec(),stuAccount,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "解挂失");
}

void card_status::on_btn_logoutCard_clicked()
{
    QSqlQuery *query=new QSqlQuery(db);
    QString stuAccount = this->ui->line_stu_id->text();
    QString sq=QString("delete from card_infor where student_account=%1").arg(this->ui->line_stu_id->text());

    giveInfor(query->exec(sq),stuAccount,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "注销");
}

void card_status::on_line_stu_id_textChanged(const QString &account)
{
    if(isback){
        QString _account=account;
        this->ui->line_stu_id->setText(_account);
        this->ui->btn_getCard->setDisabled(true);
        this->ui->btn_makeUpCard->setDisabled(true);
        this->ui->btn_reportLoss->setDisabled(true);
        this->ui->btn_cancelReportLoss->setDisabled(true);
        this->ui->btn_logoutCard->setDisabled(true);
        isback=false;
    }

    if(account.length()==10){
        isback=true;
        setButtons(account);
    }
}
