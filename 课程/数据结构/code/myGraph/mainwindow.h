#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QFuture>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QString>
#include <QStack>
#include <QQueue>
#include <QStandardItem>
#include <QStandardItemModel>
#include "mygraph.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init(MGraph *&mGraph);

    void myUpdate(){update();while (!flag) {delay(100);}delay(200);flag=false;delay(200);}
    void drawGraph(QPixmap &pixmap,QColor color,MGraph *mGraph);
    void drawTry(int isPaintEdge[10][10],int isPaintVertex[10], QPixmap &pixmap,MGraph *mGraph);

    void drawEdge(QPixmap &pixmap,QColor color,MGraph *mGraph,int nodeA,int nodeB);
    void drawVertex(QPixmap &pixmap,QColor color,MGraph *mGraph,int node);

    void delay(int mSec);

    void display(int arr[],int arrLen,int row,int col);

    void DFS(MGraph *mGraph,int node);
    void Prim(MGraph *mGraph,int node);
    void Dijkstra(MGraph *mGraph,int node);

private slots:
    void paintEvent(QPaintEvent *);

    void on_btn_play_clicked();

    void on_btn_clear_clicked();

    void on_btn_flag_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *model;
    MGraph *mGraph;
    bool flag=false;
    QFont font;

    QColor originalColor;
    QColor tryColor;
    QColor finishedColor;

    QPixmap pixmapDFS;
    QPixmap pixmapPrim;
    QPixmap pixmapDijkstra;
};

#endif // MAINWINDOW_H
