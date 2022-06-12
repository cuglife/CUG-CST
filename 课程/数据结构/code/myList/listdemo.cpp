#include "listdemo.h"
#include "ui_listdemo.h"
#include <QList>
ListDemo::ListDemo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListDemo)
{
    ui->setupUi(this);

    this->ui->textEdit->setVisible(false);
    this->ui->tableView->setVisible(false);
    this->ui->button_ok->setDisabled(true);

    this->ui->lineEdit_num->setVisible(false);
    this->ui->lineEdit_text->setVisible(false);

    this->ui->cbb_insertType->setVisible(false);
    this->ui->cbb_deleteType->setVisible(false);

    this->ui->rbt_seq->setChecked(true);

    this->model = new QStandardItemModel;
    this->ui->tableView->setModel(model);
    this->ui->tableView->verticalHeader()->setVisible(false);
}

ListDemo::~ListDemo()
{
    delete ui;
}

//响应Enter键
void ListDemo::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Return:{       //数字键盘Enter键
        on_button_ok_clicked();
        break;
    }
    case Qt::Key_Enter: {       //Enter键
        on_button_ok_clicked();
        break;
    }
    default:
        break;
    }
}

void ListDemo::displayControl(){
//是否显示控件及button是否可以按下
    this->ui->lineEdit_num->setVisible(false);
    this->ui->lineEdit_text->setVisible(false);


    if((isSeqChecked&&isSeqCreate)||(!isSeqChecked&&isLinkCreate)){
        if(operateType==1){
            this->ui->textEdit->setVisible(false);
            this->ui->tableView->setVisible(true);
            this->ui->button_ok->setDisabled(true);
        }
        else if(operateType==3){
            this->ui->button_ok->setEnabled(true);
        }

    }
    else {
        if(operateType==1){
            this->ui->textEdit->setVisible(true);
            this->ui->tableView->setVisible(false);
            this->ui->button_ok->setEnabled(true);
        }
        else if(operateType==3)
            this->ui->button_ok->setDisabled(true);
    }

    switch (operateType) {
    case 0:{
        this->ui->cbb_insertType->setVisible(false);
        this->ui->cbb_deleteType->setVisible(false);
        break;
    }
    case 1:{
        this->ui->cbb_insertType->setVisible(false);
        this->ui->cbb_deleteType->setVisible(false);
        display();
        break;
    }
    case 2:{
        this->ui->textEdit->setVisible(false);
        this->ui->tableView->setVisible(true);
        this->ui->cbb_insertType->setVisible(true);
        this->ui->cbb_deleteType->setVisible(false);
        display();
        break;
    }
    case 3:{
        this->ui->textEdit->setVisible(false);
        this->ui->tableView->setVisible(true);
        this->ui->cbb_insertType->setVisible(false);
        this->ui->cbb_deleteType->setVisible(true);
        display();
        break;
    }
    case 4:case 5:{
        this->ui->button_ok->setEnabled(true);
        this->ui->textEdit->setVisible(false);
        this->ui->tableView->setVisible(true);
        this->ui->cbb_insertType->setVisible(false);
        this->ui->cbb_deleteType->setVisible(false);
        display();
        break;
    }
    }
}

void ListDemo::clear(){
    this->ui->cbb_operate->setCurrentIndex(0);
    this->ui->cbb_insertType->setCurrentIndex(0);
    this->ui->cbb_deleteType->setCurrentIndex(0);
    this->ui->lineEdit_num->clear();
    this->ui->lineEdit_text->clear();
    this->ui->textEdit->clear();
    this->model->clear();
}

void ListDemo::display(){
    this->model->clear();
    int row=0;

    if(isSeqChecked){
        this->model->setHorizontalHeaderItem(0,new QStandardItem("序号"));
        this->model->setHorizontalHeaderItem(1,new QStandardItem("顺序表"));
        this->ui->tableView->setColumnWidth(0,200);
        this->ui->tableView->setColumnWidth(1,591);
        int ListLen=mySeq.length();
        for (row=0;row<ListLen;row++) {
            QString strRow=QString::number(row);
            this->model->setItem(row,0,new QStandardItem(strRow));
            this->model->setItem(row,1,new QStandardItem(mySeq.at(row)));
            this->model->item(row,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            this->model->item(row,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    else{
        this->model->setHorizontalHeaderItem(0,new QStandardItem("序号"));
        this->model->setHorizontalHeaderItem(1,new QStandardItem("链表"));
        this->ui->tableView->setColumnWidth(0,200);
        this->ui->tableView->setColumnWidth(1,591);
        int ListLen=myLink.length();
        for (row=0;row<ListLen;row++) {
            QString strRow=QString::number(row);
            this->model->setItem(row,0,new QStandardItem(strRow));
            this->model->setItem(row,1,new QStandardItem(myLink.at(row)));
            this->model->item(row,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            this->model->item(row,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

void ListDemo::on_button_Debug_clicked()
{
    QString arg1=this->ui->lineEdit_num->text();
    QString arg2=this->ui->lineEdit_text->text();
}

void ListDemo::on_button_ok_clicked()
{
    if(isSeqChecked){
        switch (operateType) {
        case 1:{
            QString input = ui->textEdit->toPlainText();
            QStringList inputSplit=input.split("\n");
            int i=0,inputLen=inputSplit.length();
            for (i=0;i<inputLen;i++) {
                if(inputSplit.at(i)!="@")
                    mySeq._insert(i,inputSplit.at(i));
            }

            isSeqCreate=true;
            this->ui->button_ok->setDisabled(true);
            this->ui->textEdit->setVisible(false);
            this->ui->tableView->setVisible(true);
            display();
            break;
        }
        case 2:{
            QString arg1=this->ui->lineEdit_num->text();
            QString arg2=this->ui->lineEdit_text->text();
            int num=arg1.toInt();
            switch (insertType) {
            case 1:{
                mySeq._insert(num,arg2);
                break;
            }
            case 2:{
                mySeq.insert(num,arg2);
                break;
            }
            case 3:{
                mySeq._insert(arg1,arg2);
                break;
            }
            case 4:{
                mySeq.insert(arg1,arg2);
                break;
            }
            case 5:{
                mySeq._insertAll(arg1,arg2);
                break;
            }
            case 6:{
                mySeq.insertAll(arg1,arg2);
                break;
            }
            }
            display();
            break;
        }
        case 3:{
            QString arg1=this->ui->lineEdit_num->text();
            QString arg2=this->ui->lineEdit_text->text();
            int num=arg1.toInt();
            switch (deleteType) {
            case 1:{
                mySeq.removeAt(num);
                break;
            }
            case 2:{
                mySeq.removeValue(arg2);
               break;
            }
            case 3:{
                mySeq.removeValueAll(arg2);
                break;
            }
            }
            display();
            break;
        }
        case 4:{
            mySeq.sort();
            display();
            break;
        }
        case 5:{
            mySeq.inversion();
            display();
            break;
        }
        }
    }
    else {
        switch (operateType) {
        case 1:{
            QString input = ui->textEdit->toPlainText();
            QStringList inputSplit=input.split("\n");
            int i=0,inputLen=inputSplit.length();
            for (i=0;i<inputLen;i++) {
                if(inputSplit.at(i)!="@")
                    myLink._insert(i,inputSplit.at(i));
            }
            isLinkCreate=true;
            this->ui->button_ok->setDisabled(true);
            this->ui->textEdit->setVisible(false);
            this->ui->tableView->setVisible(true);
            display();
            break;
        }
        case 2:{
            QString arg1=this->ui->lineEdit_num->text();
            QString arg2=this->ui->lineEdit_text->text();
            int num=arg1.toInt();
            switch (insertType) {
            case 1:{
                myLink._insert(num,arg2);
                break;
            }
            case 2:{
                myLink.insert(num,arg2);
                break;
            }
            case 3:{
                myLink._insert(arg1,arg2);
                break;
            }
            case 4:{
                myLink.insert(arg1,arg2);
                break;
            }
            case 5:{
                myLink._insertAll(arg1,arg2);
                break;
            }
            case 6:{
                myLink.insertAll(arg1,arg2);
                break;
            }
            }
        }
        display();
        break;
        case 3:{
            QString arg1=this->ui->lineEdit_num->text();
            QString arg2=this->ui->lineEdit_text->text();
            int num=arg1.toInt();
            switch (deleteType) {
            case 1:{
                myLink.removeAt(num);
                break;
            }
            case 2:{
                myLink.removeValue(arg2);
               break;
            }
            case 3:{
                myLink.removeValueAll(arg2);
                break;
            }
            }
            display();
            break;
        }
        case 4:{
            myLink.sort(myLink);
            display();
            break;
        }
        case 5:{
            myLink.inversion();
            display();
            break;
        }
        }
    }
}





























void ListDemo::on_lineEdit_num_textChanged(const QString &arg1)
{
    this->ui->button_ok->setEnabled(true);
    QString arg2=this->ui->lineEdit_text->text();
    int arg1Length=arg1.length();
    int arg2Length=arg2.length();
    if(operateType==3){
        if(deleteType==1&&arg1Length==0)
            this->ui->button_ok->setDisabled(true);
        if((deleteType==2||deleteType==3)&&arg2Length==0)
            this->ui->button_ok->setDisabled(true);
    }
}

void ListDemo::on_lineEdit_text_textChanged(const QString &arg2)
{
    this->ui->button_ok->setEnabled(true);
    QString arg1=this->ui->lineEdit_num->text();
    int arg1Length=arg1.length();
    int arg2Length=arg2.length();
    if(operateType==3){
        if(deleteType==1&&arg1Length==0)
            this->ui->button_ok->setDisabled(true);
        if((deleteType==2||deleteType==3)&&arg2Length==0)
            this->ui->button_ok->setDisabled(true);
    }
}

void ListDemo::on_rbt_seq_clicked()
{
    isSeqChecked=this->ui->rbt_seq->isChecked();
    displayControl();
    clear();
}

void ListDemo::on_rbt_link_clicked()
{
    isSeqChecked=this->ui->rbt_seq->isChecked();
    displayControl();
    clear();
}

void ListDemo::on_cbb_operate_currentIndexChanged(int index)
{
    operateType=index;
    if(index==1)
        this->ui->button_ok->setDisabled(true);
    displayControl();
    this->ui->lineEdit_num->clear();
    this->ui->lineEdit_text->clear();
}

void ListDemo::on_cbb_insertType_currentIndexChanged(int index)
{
    insertType=index;

    if(index!=0){
        this->ui->lineEdit_num->setVisible(true);
        this->ui->lineEdit_text->setVisible(true);
    }

    if(operateType==2){
        if(index==0)
            this->ui->button_ok->setDisabled(true);
        else if(index==1||index==2){
            this->ui->lineEdit_num->setPlaceholderText("序号");
            this->ui->lineEdit_text->setPlaceholderText("内容");
            this->ui->button_ok->setEnabled(true);
        }
        else if(index==3||index==4||index==5||index==6){
            this->ui->lineEdit_num->setPlaceholderText("要查找的值");
            this->ui->lineEdit_text->setPlaceholderText("要插入的内容");
            this->ui->button_ok->setEnabled(true);
        }
    }
    this->ui->lineEdit_num->clear();
    this->ui->lineEdit_text->clear();
}

void ListDemo::on_cbb_deleteType_currentIndexChanged(int index)
{
    deleteType=index;

    if(operateType==3){
        switch (index) {
        case 0:{
            this->ui->button_ok->setDisabled(true);
            break;
        }
        case 1:{
            this->ui->lineEdit_num->setPlaceholderText("序号");
            this->ui->lineEdit_text->setPlaceholderText("");
            this->ui->lineEdit_num->setVisible(true);
            this->ui->lineEdit_text->setVisible(false);
            this->ui->button_ok->setEnabled(true);
            break;
        }
        case 2:case 3:{
            this->ui->lineEdit_num->setPlaceholderText("");
            this->ui->lineEdit_text->setPlaceholderText("内容");
            this->ui->lineEdit_num->setVisible(false);
            this->ui->lineEdit_text->setVisible(true);
            this->ui->button_ok->setEnabled(true);
            break;
        }
        }
    }
    this->ui->lineEdit_num->clear();
    this->ui->lineEdit_text->clear();
}
