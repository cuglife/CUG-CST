#ifndef STU_FUNC_H
#define STU_FUNC_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QDateTime>
#include "connect.h"

namespace Ui {
class stu_func;
}

class stu_func : public QDialog
{
    Q_OBJECT

public:
    explicit stu_func(QWidget *parent = nullptr);
    ~stu_func();

    void showEvent(QShowEvent *);
    void init();
    void interfaceSet();
    void freshTableView();
    void createConnection();

private slots:
    void on_btn_deposit_clicked();

    void on_btn_change_password_clicked();

private:
    Ui::stu_func *ui;
    QSqlDatabase db;
    QString de_card_id;
    QStandardItemModel *model;
};

#endif // STU_FUNC_H
