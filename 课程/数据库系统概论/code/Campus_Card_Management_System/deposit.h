#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>
#include <QDateTime>

#include "connect.h"

namespace Ui {
class deposit;
}

class deposit : public QDialog
{
    Q_OBJECT

public:
    explicit deposit(QWidget *parent = nullptr);
    ~deposit();
    void showEvent(QShowEvent *);
    void createConnection();
    void refresh();

private slots:
    void on_button_ok_clicked();

    void on_line_stu_id_textChanged(const QString &arg1);

private:
    Ui::deposit *ui;
    QSqlDatabase db;
    QString de_card_id;
    bool isback = false;
};

#endif // DEPOSIT_H
