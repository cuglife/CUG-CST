#ifndef STUDENT_H
#define STUDENT_H

#include <QDialog>
#include "connect.h"

namespace Ui {
class student;
}

class student : public QDialog
{
    Q_OBJECT

public:
    explicit student(QWidget *parent = nullptr);
    ~student();
    void showEvent(QShowEvent *);
    void createConnection();
    void clearInterface();

private slots:
    void on_button_insert_clicked();

    void on_button_revision_clicked();

    void on_button_delete_clicked();

    void on_line_stu_id_textChanged(const QString &account);

private:
    Ui::student *ui;
    QSqlDatabase db;
    bool isback=false;
};

#endif // STUDENT_H
