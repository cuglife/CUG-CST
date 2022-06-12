#ifndef LISTDEMO_H
#define LISTDEMO_H

#include <QMainWindow>
#include "myList.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include<QStandardItem>
#include<QStandardItemModel>

namespace Ui {
class ListDemo;
}



class ListDemo : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListDemo(QWidget *parent = nullptr);
    ~ListDemo();

private slots:
    void on_cbb_operate_currentIndexChanged(int index);

    void on_button_ok_clicked();
    void keyPressEvent(QKeyEvent *event);
    void on_rbt_seq_clicked();

    void on_rbt_link_clicked();

    void clear();
    void display();
    void displayControl();


    void on_lineEdit_num_textChanged(const QString &arg1);

    void on_lineEdit_text_textChanged(const QString &arg1);

    void on_button_Debug_clicked();

    void on_cbb_insertType_currentIndexChanged(int index);

    void on_cbb_deleteType_currentIndexChanged(int index);

private:
    Ui::ListDemo *ui;

    SeqList<QString> mySeq;
    LinkList<QString> myLink;

    bool isSeqChecked;
    bool isSeqCreate=false;
    bool isLinkCreate=false;

    int operateType;
    int insertType;
    int deleteType;

    QStandardItemModel *model;
};

#endif // LISTDEMO_H
