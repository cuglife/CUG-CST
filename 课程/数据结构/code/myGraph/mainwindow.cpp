#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QThreadPool::globalInstance()->setMaxThreadCount(10);
    font.setFamily("华文仿宋");
    font.setPointSize(17);

    originalColor=QColor("#c2ccd0");
    tryColor=QColor("#56004f");
    finishedColor=QColor("#ff3300");

    QImage *testPic=new QImage("://GraphExm.png");
    this->ui->label_testPic->setPixmap(QPixmap::fromImage(*testPic));
    this->model = new QStandardItemModel;
    this->ui->tableView->setModel(model);
    //this->ui->tableView->verticalHeader()->setVisible(false);

    pixmapDFS = QPixmap(420, 720);              pixmapDFS.fill(QColor(255,255,255,255));
    pixmapPrim = QPixmap(420, 720);             pixmapPrim.fill(QColor(255,255,255,255));
    pixmapDijkstra = QPixmap(420, 720);         pixmapDijkstra.fill(QColor(255,255,255,255));

    init(mGraph);
    drawGraph(pixmapDFS,originalColor,mGraph);
    drawGraph(pixmapPrim,originalColor,mGraph);
    drawGraph(pixmapDijkstra,originalColor,mGraph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter Painter(this);
    Painter.drawPixmap(20,128, 420, 720,pixmapDFS);
    Painter.drawPixmap(440,128, 420, 720,pixmapPrim);
    Painter.drawPixmap(860,128, 420, 720,pixmapDijkstra);
}

void MainWindow::delay(int mSec)
{
    QEventLoop loop;
    QTimer::singleShot(mSec, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::init(MGraph *&mGraph)
{
    mGraph = mGraph->CreateGraph(10);
    mGraph->EdgeNum=17;

    mGraph->data.append(MGNode(0,"辽中南",QPoint(248,10)));
    mGraph->data.append(MGNode(1,"京津冀",QPoint(177,80)));
    mGraph->data.append(MGNode(2,"山东",QPoint(310,100)));
    mGraph->data.append(MGNode(3,"关中",QPoint(60,180)));
    mGraph->data.append(MGNode(4,"中原",QPoint(180,180)));
    mGraph->data.append(MGNode(5,"长三角",QPoint(329,180)));
    mGraph->data.append(MGNode(6,"成渝",QPoint(10,300)));
    mGraph->data.append(MGNode(7,"重庆",QPoint(95,320)));
    mGraph->data.append(MGNode(8,"珠三角",QPoint(209,400)));
    mGraph->data.append(MGNode(9,"长中游",QPoint(198,270)));

    mGraph->edge.append(GEdge(0,1,750));
    mGraph->edge.append(GEdge(0,2,680));
    mGraph->edge.append(GEdge(1,2,800));
    mGraph->edge.append(GEdge(1,3,1140));
    mGraph->edge.append(GEdge(1,4,650));
    mGraph->edge.append(GEdge(2,4,820));
    mGraph->edge.append(GEdge(2,5,960));
    mGraph->edge.append(GEdge(3,4,570));
    mGraph->edge.append(GEdge(4,5,1200));
    mGraph->edge.append(GEdge(3,6,840));
    mGraph->edge.append(GEdge(4,9,530));
    mGraph->edge.append(GEdge(5,9,680));
    mGraph->edge.append(GEdge(6,7,340));
    mGraph->edge.append(GEdge(5,8,2600));
    mGraph->edge.append(GEdge(7,9,900));
    mGraph->edge.append(GEdge(7,8,2500));
    mGraph->edge.append(GEdge(8,9,1380));

    for (int i=0;i<mGraph->edge.size();i++)
        mGraph->insertEdge(mGraph->edge.at(i));

    int PrimList=1,DijkstraList=12;
    this->model->setHorizontalHeaderItem(0,new QStandardItem(""));
    this->model->setHorizontalHeaderItem(11,new QStandardItem(""));
    this->model->setHorizontalHeaderItem(12,new QStandardItem(""));
    for (int i=0;i<10;i++) {
        this->model->setHorizontalHeaderItem(i+PrimList,new QStandardItem(mGraph->data.at(i).information));
        this->model->setHorizontalHeaderItem(i+DijkstraList,new QStandardItem(mGraph->data.at(i).information));
        this->ui->tableView->setColumnWidth(i+PrimList,60);
        this->ui->tableView->setColumnWidth(i+DijkstraList,60);
    }
    this->ui->tableView->setRowHeight(0,50);
    this->ui->tableView->setRowHeight(1,50);

    this->ui->tableView->setColumnWidth(0,72);
    this->ui->tableView->setColumnWidth(11,72);
    this->ui->tableView->setColumnWidth(11,72);
    this->model->setItem(0,0,new QStandardItem("Parent"));
    this->model->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->model->setItem(0,11,new QStandardItem("Path"));
    this->model->item(0,11)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->model->setItem(1,0,new QStandardItem("Dist"));
    this->model->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->model->setItem(1,11,new QStandardItem("Dist"));
    this->model->item(1,11)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void MainWindow::drawGraph(QPixmap &pixmap,QColor color,MGraph *mGraph)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    QPen pen;
    pen.setColor(color);
    painter.setPen(pen);

    for (int i=0;i<10;i++) {
        QPoint pos=mGraph->data.at(i).position;
        painter.drawRect(pos.x(),pos.y(),80,32);                           //节点
        painter.drawText(pos.x(),pos.y(),80,32,Qt::AlignCenter,mGraph->data.at(i).information);
    }

    for (int i=0; i<mGraph->VertexNum; i++){
        for (int j=0; j<mGraph->VertexNum; j++){
            if(mGraph->G[i][j] != GINFINITY)
                painter.drawLine(mGraph->data.at(i).position.x()+40,mGraph->data.at(i).position.y()+16,mGraph->data.at(j).position.x()+40,mGraph->data.at(j).position.y()+16);
        }
    }
}

void MainWindow::drawVertex(QPixmap &pixmap,QColor color,MGraph *mGraph,int node)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    QPen pen;
    pen.setColor(color);
    painter.setPen(pen);

    QPoint pos=mGraph->data.at(node).position;
    painter.drawRect(pos.x(),pos.y(),80,32);
    painter.drawText(pos.x(),pos.y(),80,32,Qt::AlignCenter,mGraph->data.at(node).information);
}

void MainWindow::drawEdge(QPixmap &pixmap,QColor color,MGraph *mGraph,int NodeA, int NodeB)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setColor(color);
    painter.setPen(pen);

    painter.drawLine(mGraph->data.at(NodeA).position.x()+40,mGraph->data.at(NodeA).position.y()+16,mGraph->data.at(NodeB).position.x()+40,mGraph->data.at(NodeB).position.y()+16);
}

void MainWindow::drawTry(int isPaintEdge[10][10],int isPaintVertex[10], QPixmap &pixmap,MGraph *mGraph)
{
    pixmap.fill(QColor(255,255,255,255));
    drawGraph(pixmap,originalColor,mGraph);
    for (int i=0;i<10;i++) {
        for (int j=0;j<10;j++){
            if(isPaintEdge[i][j]==-1)
                drawEdge(pixmap,tryColor,mGraph,i,j);
            else if(isPaintEdge[i][j]==1)
                drawEdge(pixmap,finishedColor,mGraph,i,j);
        }
    }
    for (int i=0;i<10;i++) {
        if(isPaintVertex[i]==-1)
            drawVertex(pixmap,tryColor,mGraph,i);
        else if (isPaintVertex[i]==1)
            drawVertex(pixmap,finishedColor,mGraph,i);
    }
}

void MainWindow::display(int *arr, int arrLen, int row, int col)
{
    for (int i=0;i<arrLen;i++){
        if(arr[i]!=GINFINITY){
            this->model->setItem(row,col+i,new QStandardItem(QString::number(arr[i])));
            this->model->item(row,col+i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        else {
            this->model->setItem(row,col+i,new QStandardItem("∞"));
            this->model->item(row,col+i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    this->show();
}

void MainWindow::DFS(MGraph *mGraph, int node)
{
    int isPaintEdge[10][10]={{0}};      bool isPaintEdgeStore[10][10]={{false}};
    int isPaintVertex[10]={0};

    QStack<int> _nodeStack;
    bool isVisited[10]={false};

    _nodeStack.push(node);
    int focus=node,count=1;
    isVisited[node]=true;
    isPaintVertex[node]=true;
    int travel[10]={node,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int travelOrder=1;

    while(!_nodeStack.isEmpty()){
        int i;
        for (i=0; i<mGraph->VertexNum; i++){//遍历图内所有节点
            if(mGraph->G[focus][i] != GINFINITY){//如果为邻接点
                if(!isVisited[i]){//如果未访问过，标记为已访问
                    isPaintEdge[focus][i]=1;    isPaintEdgeStore[focus][i]=true;
                    isPaintEdge[i][focus]=1;    isPaintEdgeStore[i][focus]=true;
                    isPaintVertex[i]=true;
                    isVisited[i]=true;
                    travel[travelOrder++]=i;
                    _nodeStack.push(i);    //访问过的节点压入栈中
                    focus=i;               //更新当前节点
                    count++;               //计数已遍历的节点
                    break;
                }
                else {//已访问过，标记为试探边
                    isPaintEdge[focus][i]=-1;
                    isPaintEdge[i][focus]=-1;
                }
            }
        }

        drawTry(isPaintEdge,isPaintVertex,pixmapDFS,mGraph);
        myUpdate();

        for (int i=0;i<10;i++) {                             //把试探的节点恢复
            for (int j=0;j<10;j++){
                if(isPaintEdge[i][j]==-1)
                    isPaintEdge[i][j]=0;
                if(isPaintEdgeStore[i][j])
                    isPaintEdge[i][j]=true;
            }
            if(isVisited[i])
                isPaintVertex[i]=1;
        }

        if(i==mGraph->VertexNum){                   //节点都试探过，出栈回退
            _nodeStack.pop();
            if(!_nodeStack.isEmpty()){
                focus=_nodeStack.top();
            }
        }
    }

    if(count!=mGraph->VertexNum){
        QMessageBox::critical(this,"错误","未知错误","确定");
    }
}

void MainWindow::Prim(MGraph *mGraph, int node)
{
    int isPaintEdge[10][10]={{0}};
    int isPaintVertex[10]={0};

    int dist[10],parent[10];
    for (int i=0; i<mGraph->VertexNum;i++) {
        dist[i] = mGraph->G[node][i];
        parent[i] = node;
    }

    dist[node] = 0;             //将初始点node收录
    parent[node] = -1;          //标记树根node
    int count=1;                //记录收入的节点数
    isPaintVertex[node]=1;

    while (true) {
        int MinVertex=-1,MinDist = GINFINITY;
        int tryVertex=-1;
        for (int i=0; i<mGraph->VertexNum; i++) {
            if ( dist[i]!=0 && dist[i]<GINFINITY) {
                tryVertex = i;
                isPaintEdge[parent[tryVertex]][tryVertex]=-1;
                isPaintEdge[tryVertex][parent[tryVertex]]=-1;
            }
            if ( dist[i]!=0 && dist[i]<MinDist) {   //若i未被收录，且dist[i]更小, 更新最小距离和对应顶点
                MinDist = dist[i];
                MinVertex = i;

                isPaintEdge[parent[MinVertex]][MinVertex]=-1;
                isPaintEdge[MinVertex][parent[MinVertex]]=-1;
            }
        }

        if(MinVertex==-1)
            break;
        isPaintVertex[MinVertex]=1;
        isPaintEdge[parent[MinVertex]][MinVertex]=1;
        isPaintEdge[MinVertex][parent[MinVertex]]=1;
        dist[MinVertex] = 0;

        count++;

        for(int i=0; i<mGraph->VertexNum; i++ ){//对图中的每个顶点i}
            if ( dist[i]!=0 && mGraph->G[MinVertex][i]<GINFINITY ) {//若i是MinVertex的邻接点并且未被收录
                if ( mGraph->G[MinVertex][i] < dist[i] ) {          //若收录MinVertex使得dist[i]变小
                    dist[i] = mGraph->G[MinVertex][i];              //更新dist[i]
                    parent[i] = MinVertex;                          //更新树
                }
            }
        }
        drawTry(isPaintEdge,isPaintVertex,pixmapPrim,mGraph);
        display(parent,10,0,1);
        display(dist,10,1,1);
        myUpdate();
        for (int i=0;i<10;i++) {                             //把试探的节点恢复
            for (int j=0;j<10;j++){
                if(isPaintEdge[i][j]==-1)
                    isPaintEdge[i][j]=0;
            }
        }
    }

    if(count<mGraph->VertexNum)
        QMessageBox::critical(this,"错误","生成树不存在","确定");
}

void MainWindow::Dijkstra(MGraph *mGraph, int node)
{
    int isPaintEdge[10][10]={{0}};          //记录每个节点间的绘画情况
    int isPaintVertex[10]={0};

    bool isCollected[10]={false};       //节点是否被收录
    int dist[10],path[10];
    for (int i=0; i<mGraph->VertexNum;i++) {    //初始化dist和path
        dist[i] = mGraph->G[node][i];
        if ( dist[i]<GINFINITY )
            path[i] = node;
        else
            path[i] = -1;
       }
       dist[node] = 0;
       isCollected[node] = true;
       isPaintVertex[node]=1;

       while (true) {
           int MinVertex=-1, MinDist = GINFINITY;
           for (int i=0; i<mGraph->VertexNum; i++) {
               if ( isCollected[i]==false && dist[i]<MinDist) {//若V未被收录，且dist[V]更小 更新最小距离和对应顶点
                   MinDist = dist[i];
                   MinVertex = i;
                   isPaintEdge[path[MinVertex]][MinVertex]=-1;      //所有试探的顶点数组标记为-1
                   isPaintEdge[MinVertex][path[MinVertex]]=-1;
               }
           }

           if(MinVertex==-1)
               break;

           isPaintEdge[path[MinVertex]][MinVertex]=1;               //最小的数组标记为0
           isPaintEdge[MinVertex][path[MinVertex]]=1;

           this->ui->label_di->setText(mGraph->data.at(MinVertex).information);

           isCollected[MinVertex] = true;
           isPaintVertex[MinVertex]=1;

           for( int i=0; i<mGraph->VertexNum; i++ ){
               if ( isCollected[i]==false && mGraph->G[MinVertex][i]<GINFINITY ) {
                   if ( mGraph->G[MinVertex][i]<0 ){
                       QMessageBox::critical(this,"错误","有负边","确定");
                       return;
                   }
                   if ( dist[MinVertex]+mGraph->G[MinVertex][i] < dist[i] ) {   //若收录V使得dist[i]变小
                       dist[i] = dist[MinVertex]+mGraph->G[MinVertex][i];       //更新dist[i]
                       path[i] = MinVertex;                                     //更新node到i的路径
                   }
               }
           }
           drawTry(isPaintEdge,isPaintVertex,pixmapDijkstra,mGraph);
           display(path,10,0,12);
           display(dist,10,1,12);
           myUpdate();
           for (int i=0;i<10;i++) {                             //把试探的节点恢复
               for (int j=0;j<10;j++){
                   if(isPaintEdge[i][j]==-1)
                       isPaintEdge[i][j]=0;
               }
           }


       }
}

void MainWindow::on_btn_play_clicked()
{
    QString node=this->ui->lineEdit->text();
    QFuture<void> tDFS =QtConcurrent::run(this,&MainWindow::DFS,mGraph,node.toInt());
    QFuture<void> tPrim =QtConcurrent::run(this,&MainWindow::Prim,mGraph,node.toInt());
    QFuture<void> tDijkstra =QtConcurrent::run(this,&MainWindow::Dijkstra,mGraph,node.toInt());
    delay(100);
    this->model->setItem(0,0,new QStandardItem("Parent"));
    this->model->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->ui->tableView->setColumnWidth(0,72);
}

void MainWindow::on_btn_clear_clicked()
{
    pixmapDFS.fill(QColor(255,255,255,255));
    pixmapPrim.fill(QColor(255,255,255,255));
    pixmapDijkstra.fill(QColor(255,255,255,255));
    drawGraph(pixmapDFS,originalColor,mGraph);
    drawGraph(pixmapPrim,originalColor,mGraph);
    drawGraph(pixmapDijkstra,originalColor,mGraph);
    update();
}

void MainWindow::on_btn_flag_clicked()
{
    flag=true;
    this->model->setItem(0,0,new QStandardItem("Parent"));
    this->model->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->ui->tableView->setColumnWidth(0,72);
}
