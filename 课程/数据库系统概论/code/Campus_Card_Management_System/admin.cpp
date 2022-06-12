#include "admin.h"
#include "ui_admin.h"

admin::admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}


void admin::on_btn_deposit_clicked()
{
    opDeposit.show();
}

void admin::on_btn_status_clicked()
{
    opCardStatus.show();
}

void admin::on_btn_query_clicked()
{
    opQuery.show();
}
