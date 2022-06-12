#ifndef CARD_STATUS_H
#define CARD_STATUS_H

#include <QDialog>
#include <QtMath>
#include <QDateTime>
#include "connect.h"

namespace Ui {
class card_status;
}

class card_status : public QDialog
{
    Q_OBJECT

public:
    explicit card_status(QWidget *parent = nullptr);
    ~card_status();
    void showEvent(QShowEvent *);
    void createConnection();
    void setButtons(QString account);
    bool insertStatusInfor(QString stuAccount, QString time, QString operateType);
    void giveInfor(bool isExec, QString stuAccount, QString time, QString operateType);
    QString randCardID();

private slots:
    void on_line_stu_id_textChanged(const QString &arg1);

    void on_btn_getCard_clicked();

    void on_btn_makeUpCard_clicked();

    void on_btn_reportLoss_clicked();

    void on_btn_cancelReportLoss_clicked();

    void on_btn_logoutCard_clicked();

private:
    Ui::card_status *ui;
    QSqlDatabase db;
    bool isback = false;
};

#endif // CARD_STATUS_H
