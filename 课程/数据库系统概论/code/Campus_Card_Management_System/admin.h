#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "connect.h"
#include "card_status.h"
#include "deposit.h"
#include "query.h"

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_btn_deposit_clicked();

    void on_btn_status_clicked();

    void on_btn_query_clicked();

private:
    Ui::admin *ui;
    card_status opCardStatus;
    deposit opDeposit;
    query opQuery;

};

#endif // ADMIN_H
