#include "query.h"
#include "ui_query.h"

query::query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::query)
{
    ui->setupUi(this);
    this->ui->button_search->setDisabled(true);
    this->ui->line_infor->setVisible(true);
    this->ui->dateEdit->setVisible(false);
    this->ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    this->ui->dateEdit->setDate(QDate::currentDate());
    this->model = new QStandardItemModel;
    this->ui->tableView->setModel(model);
    reSetModel(true);
}

query::~query()
{
    delete ui;
    db.close();
}

void query::showEvent(QShowEvent *)
{
    createConnection();
}

void query::createConnection()
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

void query::reSetModel(bool isDeposit)
{
    this->model->clear();
    this->ui->tableView->resize(1081,591);

    if(isDeposit){
        for (int i = 0; i < 5; i++){
            this->model->setHorizontalHeaderItem(i,new QStandardItem(depositDB[i]));
            this->ui->tableView->setColumnWidth(i,216);
        }

    }
    else {
        for (int i = 0; i < 4; i++){
            this->model->setHorizontalHeaderItem(i,new QStandardItem(operateDB[i]));
            this->ui->tableView->setColumnWidth(i,270);
        }
    }
}

bool query::writeInfor(int colCount, QSqlQuery *query)
{
    if(query->size()>0){
        for (int i =0; query->next(); i++) {
            for (int k = 0; k < colCount; k++){
                this->model->setItem(i,k,new QStandardItem(query->value(k).toString()));
                this->model->item(i,k)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
        return true;
    }
    else
        return false;
}

void query::on_button_search_clicked()
{
    bool isDepositChecked = this->ui->rbt_deposit->isChecked();
    int index = this->ui->cbb_findway->currentIndex();
    if(index==0){
        QMessageBox::information(this,"通知","请选择查询方式！","确认");
        return;
    }

    if(isDepositChecked)
        queryINdeposit();
    else
        queryINoperate();
}

void query::queryINdeposit()
{
    QSqlQuery *query=new QSqlQuery(db);
    int findway=this->ui->cbb_findway->currentIndex();
    QString queryInfor = this->ui->line_infor->text();
    QString queryTime = this->ui->dateEdit->text();

    if(findway==1)
        query->exec("select * from deposit_infor where card_id like '%"+ queryInfor +"%'");
    if(findway==2)
        query->exec("select * from deposit_infor where deposit_infor.card_id =  (select card_id from card_infor where student_account like '%"+ queryInfor +"%')");
    if(findway==3)
        query->exec("select * from deposit_infor where admin_account like '%"+ queryInfor +"%'");
    if(findway==4)
        query->exec("select * from deposit_infor where recharge_time like '%"+ queryTime +"%'");
    if(findway==5)
        query->exec("select * from deposit_infor where recharge_amount = "+ queryInfor);

    if(!writeInfor(depositCol, query))
        QMessageBox::information(this,"通知","未查询到充值信息！","确认");
}

void query::queryINoperate()
{
    QSqlQuery *query=new QSqlQuery(db);
    int findway=this->ui->cbb_findway->currentIndex();
    QString queryInfor = this->ui->line_infor->text();
    QString queryTime = this->ui->dateEdit->text();

    if(findway==1)
        query->exec("select * from card_status_infor where admin_account like '%"+ queryInfor +"%'");
    if(findway==2)
        query->exec("select * from card_status_infor where student_account like '%"+ queryInfor +"%'");
    if(findway==3)
        query->exec("select * from card_status_infor where operate_time like '%"+ queryTime +"%'");
    if(findway==4)
        query->exec("select * from card_status_infor where operate_type like '%"+ queryInfor +"%'");

    if(!writeInfor(operateCol, query))
        QMessageBox::information(this,"通知","未查询到校园卡操作信息！","确认");
}

void query::on_cbb_findway_currentTextChanged(const QString &index)
{
    this->ui->line_infor->setVisible(true);
    this->ui->dateEdit->setVisible(false);

    if(index=="充值时间"||index=="操作时间"){
        this->ui->button_search->setEnabled(true);
        this->ui->line_infor->setVisible(false);
        this->ui->dateEdit->setVisible(true);
        this->ui->dateEdit->setCalendarPopup(true);
    }
}

void query::on_rbt_deposit_clicked()
{
    reSetModel(true);
    this->ui->cbb_findway->clear();
    this->ui->cbb_findway->addItem("请选择查询方式");
    for (int i = 0; i < 5; i++)
        this->ui->cbb_findway->addItem(depositWay[i]);

}

void query::on_rbt_operate_clicked()
{
    reSetModel(false);
    this->ui->cbb_findway->clear();
    this->ui->cbb_findway->addItem("请选择查询方式");
    for (int i = 0; i < 4; i++)
        this->ui->cbb_findway->addItem(operateWay[i]);

}

void query::on_line_infor_textChanged(const QString &queryInfor)
{
    if(queryInfor.length()!=0)
        this->ui->button_search->setEnabled(true);
    else
        this->ui->button_search->setDisabled(true);
}
