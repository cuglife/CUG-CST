#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QPoint>
#include <QTimer>

#include <vector>

#include "binarytree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isTree(QString inText,QString xText);
    void plantTree(QList<QString> xOrder, QList<QString> inOrder);
    bool isPlantRight(BinaryTree<QString> *tree,QList<QString> inOrder);

    void drawTree(BinaryTree<QString> *tree,int travelWay);
    void drawNode(BinTreeNode<QString> *node);
    QPoint setPos(int num);

    void clear();
    void messageBox(int mSec,QString text);

    void levelOrder(BinaryTree<QString> *tree);
    void PreOrder(BinaryTree<QString> *tree);
    void InOrder(BinaryTree<QString> *tree);
    void PostOrder(BinaryTree<QString> *tree);


private slots:
    void on_btn_draw_clicked();

    void on_rbt_pre_clicked();

    void on_rbt_post_clicked();

    void on_btn_exm_clicked();

    void on_btn_clear_clicked();

    void paintEvent(QPaintEvent *);

    void delay(int mSec);

    void on_btn_exmAll_clicked();

private:
    Ui::MainWindow *ui;
    BinaryTree<QString> *tree=new BinaryTree<QString>;
    QPixmap pixmap;
    QPoint pos;
    int depWid[5]={0,256,84,44,28};
    int RankWid[6][17]={{-1},{0},{0,-256,256},{0,-340,-172,172,340},{0,-384,-296,-216,-128,128,216,296,384},{0,-412,-356,-324,-272,-240,-188,-156,-100,100,156,188,240,272,324,356,412}};
    int curWid=0;
    bool isPre=true;
};


#endif // MAINWINDOW_H
