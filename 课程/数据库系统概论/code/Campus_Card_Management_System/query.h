#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <QDateEdit>
#include <QStandardItem>
#include <QStandardItemModel>
#include "connect.h"

namespace Ui {
class query;
}

class query : public QDialog
{
    Q_OBJECT

public:
    explicit query(QWidget *parent = nullptr);
    ~query();
    void showEvent(QShowEvent *);
    void createConnection();
    void reSetModel(bool isDeposit);
    bool writeInfor(int colCount, QSqlQuery *queryResult);
    void queryINdeposit();
    void queryINoperate();

private slots:
    void on_button_search_clicked();

    void on_cbb_findway_currentTextChanged(const QString &arg1);

    void on_rbt_deposit_clicked();

    void on_rbt_operate_clicked();

    void on_line_infor_textChanged(const QString &arg1);

private:
    Ui::query *ui;
    QSqlDatabase db;
    QStandardItemModel *model;
};

#endif // QUERY_H
