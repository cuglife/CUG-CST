#include "student.h"
#include "ui_student.h"
#include "connect.h"

student::student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::student)
{
    ui->setupUi(this);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->line_id );
    this->ui->line_id->setValidator(validator);                                      //学号只能输入数字
    this->ui->line_stu_id->setValidator(validator);
    this->ui->line_class->setValidator(validator);                                   //班级只能输入数字
}

student::~student()
{
    delete ui;
    db.close();
}

void student::showEvent(QShowEvent *)
{
    createConnection();
}

void student::createConnection()
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

void student::clearInterface()
{
    this->ui->line_id->clear();
    this->ui->line_stu_id->clear();
    this->ui->line_name->clear();
    this->ui->rbt_male->setChecked(true);
    this->ui->cbb_age->setCurrentIndex(0);
    this->ui->cbb_faculty->setCurrentIndex(0);
    this->ui->line_class->clear();
    this->ui->line_stu_id->setFocus();
}



void student::on_button_insert_clicked()
{
    QString id=this->ui->line_id->text();
    QString account=this->ui->line_stu_id->text();
    QString name=this->ui->line_name->text();
    QString sex=this->ui->sexGroup->checkedButton()->text();
    int age=this->ui->cbb_age->currentText().toInt();
    QString faculty=this->ui->cbb_faculty->currentText();
    QString student_class=this->ui->line_class->text();

    if(id.length()!=18||account.length()!=10||name.length()<1||age==0||faculty=="学院"||student_class.length()<6){
        QMessageBox::critical(this,"错误","信息填写不完整或错误，请检查","确定");
    }
    else{
        int ret=QMessageBox::question(this,"请确认","请确认要添加的学生信息","确认","取消");
        if(ret==0){
            clearInterface();
            QSqlQuery *query=new QSqlQuery(db);

            query->prepare("insert into student_infor(id, student_account, student_password, student_name, student_sex, student_age, student_faculty, student_class) "
                              "values (?, ?, ?, ?, ?, ?, ?, ?)");

            query->addBindValue(id);
            query->addBindValue(account);
            query->addBindValue(id.right(6));
            query->addBindValue(name);
            query->addBindValue(sex);
            query->addBindValue(age);
            query->addBindValue(faculty);
            query->addBindValue(student_class);

            bool isInsert = query->exec();
            if(isInsert){
                QMessageBox::information(this,"通知","添加成功！","确认");
                clearInterface();
            }
            else {
                QMessageBox::information(this,"通知","添加失败！","确认");
            }
        }
    }

}

void student::on_button_revision_clicked()
{
    QString id=this->ui->line_id->text();
    QString account=this->ui->line_stu_id->text();
    QString name=this->ui->line_name->text();
    QString sex=this->ui->sexGroup->checkedButton()->text();
    int age=this->ui->cbb_age->currentText().toInt();
    QString faculty=this->ui->cbb_faculty->currentText();
    QString student_class=this->ui->line_class->text();

    if(id.length()!=18||account.length()!=10||name.length()<1||age==0||faculty=="学院"||student_class.length()<6){
        QMessageBox::critical(this,"错误","信息填写不完整或错误，请检查","确定");
    }
    else{
        int ret=QMessageBox::question(this,"请确认","请确认要修改的学生信息","确认","取消");
        if(ret==0){
            QSqlQuery *query=new QSqlQuery(db);

            query->prepare("update student_infor set id=?, student_name=?, student_sex=?, student_age=?, student_faculty=?, student_class=? where student_account=?");

            query->addBindValue(id);
            query->addBindValue(name);
            query->addBindValue(sex);
            query->addBindValue(age);
            query->addBindValue(faculty);
            query->addBindValue(student_class);
            query->addBindValue(account);

            bool isRevision = query->exec();
            if(isRevision){
                QMessageBox::information(this,"通知","修改成功！","确认");
                clearInterface();
            }
            else {
                qDebug()<<query->lastError();
                QMessageBox::information(this,"通知","修改失败！","确认");
            }
        }
    }
}

void student::on_button_delete_clicked()
{
    QString _id = this->ui->line_id->text();
    QString stu_id = this->ui->line_stu_id->text();
    int ret=QMessageBox::question(this,"删除学生信息","确定要删除学号为"+stu_id+"学生的信息吗？\n","是(Y)","否(N)");
    if(ret==0){
        QSqlQuery *query=new QSqlQuery(db);
        QString sq=QString("delete from student_infor where id=%1").arg(_id);
        query->exec(sq);
        bool isDelete = query->exec();
        if(isDelete){
            QMessageBox::information(this,"通知","删除成功！","确认");
            clearInterface();
        }
        else {
            qDebug()<<query->lastError();
            QMessageBox::information(this,"通知","删除失败！","确认");
        }
    }
}

void student::on_line_stu_id_textChanged(const QString &account)
{
    if(isback){
        QString _account=account;
        clearInterface();
        this->ui->line_stu_id->setText(_account);
        this->ui->button_insert->setEnabled(true);
        this->ui->button_revision->setDisabled(true);
        this->ui->button_delete->setDisabled(true);
        isback=false;
    }

    if(account.length()==10){
        isback=true;
        QSqlQuery *query=new QSqlQuery(db);
        query->prepare("select * from student_infor where student_account=:account");
        query->bindValue(":account",account);
        query->exec();
        if(query->size()>=1&&query->next()){
            this->ui->button_insert->setDisabled(true);
            this->ui->button_revision->setEnabled(true);
            this->ui->button_delete->setEnabled(true);

            this->ui->line_id->setText(query->value(0).toString());
            this->ui->line_stu_id->setText(query->value(1).toString());
            this->ui->line_name->setText(query->value(3).toString());
            this->ui->cbb_age->setCurrentText(query->value(5).toString());
            this->ui->cbb_faculty->setCurrentText(query->value(6).toString());
            this->ui->line_class->setText(query->value(7).toString());
            if(query->value(4).toString()=="女"){
                this->ui->rbt_female->setChecked(true);
            }
        }
    }
}
