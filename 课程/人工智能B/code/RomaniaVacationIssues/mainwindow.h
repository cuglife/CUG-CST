#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <algorithm.h>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void receiveToDrawGraph(int isPaintEdge[20][20], int isPaintVertex[20]);
    void receiveRunTime(double nSec);
    void receiveVisitedNodes(int visitedNodes);
    void receivePathLength(int pathLength);
    void drawGraph(QPixmap &pixmap, QColor color);
    void drawEdge(int from, int to, QColor color);
    void drawVertex(int node, QColor color);
    void init();
    void fresh();

private slots:
    void paintEvent(QPaintEvent *);
    void on_btn_run_clicked();

private:
    Ui::MainWindow *ui;
    Algorithm *algo = new Algorithm();
    const int breadthFirst = 1;
    const int depthFirst = 2;
    const int greedyAlgorithm = 3;
    const int AStarAlgorithm = 4;
    MGraph *mGraph;
    QPixmap pixmap;

    QFont font;
    QColor originalColor;
    QColor tryColor;
    QColor finishedColor;
};
#endif // MAINWINDOW_H
