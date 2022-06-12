#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "connect.h"
#include "admin.h"
#include "student.h"
#include "stu_func.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createConnection();
    void keyPressEvent(QKeyEvent *event);
    void delay(int mSec);
    bool login(QString account, QString password);
    int admin_login(QString account, QString password);
    int student_login(QString account, QString password);


private slots:
    void on_button_login_clicked();

private:
    Ui::MainWindow *ui;
    admin opAdmin;
    student opStudent;
    stu_func opStuFunc;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
