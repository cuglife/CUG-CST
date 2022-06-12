#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmap = QPixmap(1280, 640);
    pixmap.fill(QColor(255, 255, 255, 255));

    connect(algo, &Algorithm::sendToDrawGraph, this, &MainWindow::receiveToDrawGraph);
    connect(algo, &Algorithm::sendRunTime, this, &MainWindow::receiveRunTime);
    connect(algo, &Algorithm::sendVisitedNodes, this, &MainWindow::receiveVisitedNodes);
    connect(algo, &Algorithm::sendPathLength, this, &MainWindow::receivePathLength);

    init();
    font.setFamily("华文仿宋");
    font.setPointSize(12);

    originalColor = QColor("#c2ccd0");
    tryColor = QColor("#56004f");
    finishedColor = QColor("#ff3300");

    drawGraph(pixmap, originalColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, 1280, 640, pixmap);
}

void MainWindow::fresh()
{
    pixmap = QPixmap(1280, 640);
    pixmap.fill(QColor(255, 255, 255, 255));
    init();
    drawGraph(pixmap, originalColor);
}

void MainWindow::receiveToDrawGraph(int isPaintEdge[20][20], int isPaintVertex[20])
{
    //qDebug()<<"receive";
    pixmap.fill(QColor(255, 255, 255, 255));
    drawGraph(pixmap, originalColor);
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (isPaintEdge[i][j] == -1)
                drawEdge(i, j, tryColor);
            else if (isPaintEdge[i][j] == 1)
                drawEdge(i, j, finishedColor);
        }
    }
    for (int i = 0; i < 20; i++)
    {
        if (isPaintVertex[i] == -1)
            drawVertex(i, tryColor);
        else if (isPaintVertex[i] == 1)
            drawVertex(i, finishedColor);
    }
    update();
}

void MainWindow::receiveRunTime(double nSec)
{
    this->ui->label_runtime->setText(QString::number(nSec, 10, 0)+" nSec");
}

void MainWindow::receiveVisitedNodes(int nodesNum)
{
    this->ui->label_nodes->setText(QString::number(nodesNum, 10, 0));
}

void MainWindow::receivePathLength(int pathLength)
{
    this->ui->label_path_length->setText(QString::number(pathLength, 10, 0));
}

void MainWindow::init()
{
    mGraph = new MGraph(20);
    mGraph->edgeNum = 23;

    mGraph->data.append(MGNode(0, "Arad", QPoint(72, 160)));
    mGraph->data.append(MGNode(1, "Bucharest", QPoint(720, 425)));
    mGraph->data.append(MGNode(2, "Craiova", QPoint(410, 490)));
    mGraph->data.append(MGNode(3, "Doberta", QPoint(230, 470)));
    mGraph->data.append(MGNode(4, "Eforie", QPoint(1060, 480)));
    mGraph->data.append(MGNode(5, "Fagaras", QPoint(530, 230)));
    mGraph->data.append(MGNode(6, "Glurgiu", QPoint(670, 520)));
    mGraph->data.append(MGNode(7, "Hirsova", QPoint(1000, 390)));
    mGraph->data.append(MGNode(8, "Iasi", QPoint(880, 140)));
    mGraph->data.append(MGNode(9, "Lugoj", QPoint(230, 340)));
    mGraph->data.append(MGNode(10, "Mehadia", QPoint(235, 405)));
    mGraph->data.append(MGNode(11, "Neamt", QPoint(735, 90)));
    mGraph->data.append(MGNode(12, "Oradea", QPoint(158, 34)));
    mGraph->data.append(MGNode(13, "Pitesti", QPoint(557, 363)));
    mGraph->data.append(MGNode(14, "Rimmicu_Vikea", QPoint(370, 292)));
    mGraph->data.append(MGNode(15, "Sibiu", QPoint(320, 215)));
    mGraph->data.append(MGNode(16, "Timisoara", QPoint(80, 290)));
    mGraph->data.append(MGNode(17, "Urziceni", QPoint(840, 390)));
    mGraph->data.append(MGNode(18, "Vaslui", QPoint(950, 235)));
    mGraph->data.append(MGNode(19, "Zerind", QPoint(110, 100)));

    mGraph->edge.append(GEdge(0, 15, 140));
    mGraph->edge.append(GEdge(0, 16, 118));
    mGraph->edge.append(GEdge(0, 19, 75));
    mGraph->edge.append(GEdge(1, 5, 211));
    mGraph->edge.append(GEdge(1, 6, 90));
    mGraph->edge.append(GEdge(1, 13, 101));
    mGraph->edge.append(GEdge(1, 17, 85));
    mGraph->edge.append(GEdge(2, 3, 120));
    mGraph->edge.append(GEdge(2, 13, 138));
    mGraph->edge.append(GEdge(2, 14, 146));
    mGraph->edge.append(GEdge(3, 10, 75));
    mGraph->edge.append(GEdge(4, 7, 86));
    mGraph->edge.append(GEdge(5, 15, 99));
    mGraph->edge.append(GEdge(7, 17, 98));
    mGraph->edge.append(GEdge(8, 11, 87));
    mGraph->edge.append(GEdge(8, 18, 92));
    mGraph->edge.append(GEdge(9, 16, 111));
    mGraph->edge.append(GEdge(9, 10, 70));
    mGraph->edge.append(GEdge(12, 15, 151));
    mGraph->edge.append(GEdge(12, 19, 71));
    mGraph->edge.append(GEdge(13, 14, 97));
    mGraph->edge.append(GEdge(14, 15, 80));
    mGraph->edge.append(GEdge(17, 18, 142));

    for (int i = 0; i < mGraph->edge.size(); i++)
        mGraph->insertEdge(mGraph->edge.at(i));
}

void MainWindow::drawGraph(QPixmap &pixmap, QColor color)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    //画节点
    QBrush brush(Qt::SolidPattern);
    brush.setColor(color);
    painter.setBrush(brush);

    for (int i = 0; i < 20; i++)
    {
        QPoint pos = mGraph->data.at(i).position;
        painter.drawEllipse(pos.x() + 12, pos.y(), 12, 12);
        painter.drawText(pos.x() + 32, pos.y() - 8, 64, 16, Qt::AlignLeft, mGraph->data.at(i).cityInfor);
    }

    //画线
    QPen pen;
    pen.setColor(color);
    painter.setPen(pen);
    for (int i = 0; i < mGraph->vertexNum; i++)
    {
        for (int j = 0; j < mGraph->vertexNum; j++)
        {
            if (mGraph->G[i][j] != GINFINITY)
                painter.drawLine(mGraph->data.at(i).position.x() + 20, mGraph->data.at(i).position.y() + 8, mGraph->data.at(j).position.x() + 20, mGraph->data.at(j).position.y() + 8);
        }
    }

    update();
}

void MainWindow::drawVertex(int node, QColor color)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    //画节点
    QBrush brush(Qt::SolidPattern);
    brush.setColor(color);
    painter.setBrush(brush);

    QPoint pos = mGraph->data.at(node).position;
    painter.drawEllipse(pos.x() + 12, pos.y(), 12, 12);
    painter.drawText(pos.x() + 32, pos.y() - 8, 64, 16, Qt::AlignLeft, mGraph->data.at(node).cityInfor);
}

void MainWindow::drawEdge(int from, int to, QColor color)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setColor(color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(mGraph->data.at(from).position.x() + 20, mGraph->data.at(from).position.y() + 8, mGraph->data.at(to).position.x() + 20, mGraph->data.at(to).position.y() + 8);
}

void MainWindow::on_btn_run_clicked()
{

    this->ui->btn_run->setDisabled(true);

    int index = this->ui->cbb_select->currentIndex();

    bool isRunOk = false;

    if (index == breadthFirst)
    {
        isRunOk = algo->breadthFirstSearch(mGraph);
    }
    else if (index == depthFirst)
    {
        isRunOk = algo->depthFirstSearch(mGraph);
        algo->depthFirstSearchStatistics(mGraph);
    }
    else if (index == greedyAlgorithm)
    {
        isRunOk = algo->greedyAlgorithm(mGraph);
    }
    else if (index == AStarAlgorithm)
    {
        isRunOk = algo->AStarAlgorithm(mGraph);
    }
    else
    {
        QMessageBox::critical(this, "错误", "请选择算法", "确定");
    }

    if (isRunOk)
    {
        QMessageBox::information(this, "运行完毕", "运行完毕请查看", "确定");
    }
    else
    {
        QMessageBox::critical(this, "错误", "未知错误", "确定");
    }

    this->ui->btn_run->setEnabled(true);
}
