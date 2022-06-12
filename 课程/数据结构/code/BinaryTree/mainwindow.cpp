#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->rbt_pre->setChecked(true);
    pixmap = QPixmap(1000, 640);
    pixmap.fill(QColor(240,255,255,128));
    this->ui->rbt_pre->setVisible(true);
    this->ui->rbt_post->setVisible(true);
    this->ui->cbb_travelWay->setVisible(true);
    this->ui->btn_exmAll->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter Painter(this);
    Painter.drawPixmap(160,240, 1000, 640,pixmap);
    update();
}

void MainWindow::delay(int mSec)
{
    QEventLoop loop;
    QTimer::singleShot(mSec, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::messageBox(int mSec, QString text)
{

    QMessageBox *box = new QMessageBox(QMessageBox::Information,"信息",text);
    QTimer::singleShot(mSec,box,SLOT(accept()));
    box->exec();
    clear();
}

void MainWindow::clear()
{
    tree->destroy(tree->root);
    tree->root=nullptr;
    pixmap.fill(QColor(240,255,255,128));
    update();
}

bool MainWindow::isTree(QString inText, QString xText)
{
    int inLen=inText.length();
    int xLen=xText.length();
    if(inLen!=xLen||inLen==0||xLen==0)
        return false;
    else {
        QList<QString> inOrder=inText.split(" ");
        QList<QString> xOrder=xText.split(" ");

//判断两个链表之间是否有相同的字符以及单个链表内否有重复的字符
        QList<QString> inOrderExm=inOrder;
        QList<QString> xOrderExm=xOrder;
        inOrderExm.sort();
        xOrderExm.sort();
        for (int i=0;i<inOrder.size()-1;i++) {
            if(inOrderExm.at(i)==inOrderExm.at(i+1)||xOrderExm.at(i)==xOrderExm.at(i+1)||inOrderExm.at(i)!=xOrderExm.at(i))
                return false;
        }
        for (int i=0;i<inOrder.size();i++) {
            if(inOrderExm.at(i)!=xOrderExm.at(i))
                return false;
        }

        plantTree(xOrder,inOrder);

        if(isPlantRight(tree,inOrder)){
            tree->setNum();             //设置树每个节点的位置
            return true;
        }
        else{
            tree->destroy(tree->root);
            tree->root=nullptr;
            return false;
        }
    }
}

void MainWindow::plantTree(QList<QString> xOrder, QList<QString> inOrder)
{
//利用一个栈来实现根据前序/后序和中序构建二叉树
    if(isPre){
        QStack<BinTreeNode<QString>*> *S = new QStack<BinTreeNode<QString>*>;
        int pre = 0, in = 0;
        BinTreeNode<QString>* curRoot = new BinTreeNode<QString>(xOrder.at(pre++));
        BinTreeNode<QString>* root = curRoot;
        S->push(curRoot);
//通过遍历前序遍历的链表，并结合中序遍历，判断是否为左子树或右子树
        while (pre < xOrder.length()) {
            if (curRoot->data != inOrder.at(in)) {//如果前序遍历当前节点和中序遍历当前节点值不相等，则为左子树
                curRoot->leftChild = new BinTreeNode<QString>(xOrder.at(pre++));
                curRoot = curRoot->leftChild;
                S->push(curRoot);
            }
            else {//正序遍历中序遍历的链表，倒着遍历已经遍历过的根节点，找到最后一次相等的位置，把它作为该节点的右子树
                while (!S->isEmpty() && S->top()->data == inOrder.at(in)) {
                    curRoot = S->top();
                    S->pop();
                    in++;
                }
                curRoot->rightChild = new BinTreeNode<QString>(xOrder.at(pre++));
                curRoot = curRoot->rightChild;
                S->push(curRoot);
            }
        }
        tree->root=root;
    }
    else{//后序+中序同理
        QStack<BinTreeNode<QString>*> *S = new QStack<BinTreeNode<QString>*>;
        int post = xOrder.length() - 1, in = inOrder.length() - 1;
        BinTreeNode<QString>* curRoot = new BinTreeNode<QString>(xOrder.at(post--));
        BinTreeNode<QString>* root = curRoot;
        S->push(curRoot);
        while (post >=  0) {
            if (curRoot->data != inOrder.at(in)) {
                curRoot->rightChild = new BinTreeNode<QString>(xOrder.at(post--));
                curRoot = curRoot->rightChild;
                S->push(curRoot);
            }
            else {
                while (!S->isEmpty() && S->top()->data == inOrder.at(in)) {
                    curRoot = S->top();
                    S->pop();
                    in--;
                }
                curRoot->leftChild = new BinTreeNode<QString>(xOrder.at(post--));
                curRoot = curRoot->leftChild;
                S->push(curRoot);
            }
        }
            tree->root=root;
    }
}

bool MainWindow::isPlantRight(BinaryTree<QString> *tree, QList<QString> inOrder)
{
//将已构造的树中序遍历一遍与输入的中序遍历比较
    QStack<BinTreeNode<QString>*> S;
    BinTreeNode<QString> *p = tree->getRoot();
    int i=0;
    do {
        while (p != nullptr) {
            S.push(p);
            p = p->leftChild;
        }
        if (!S.isEmpty()) {
            p=S.pop ();
            if(p->data!=inOrder.at(i++))
                return false;
            p = p->rightChild;
        }
  } while (p != nullptr || !S.isEmpty());
    return true;
}


void MainWindow::drawTree(BinaryTree<QString> *tree,int travelWay)
{
    this->ui->btn_clear->setEnabled(false);
    switch (travelWay) {
    case 0:{
        PreOrder(tree);
        break;
    }
    case 1:{
        InOrder(tree);
        break;
    }
    case 2:{
        PostOrder(tree);
        break;
    }
    case 3:{
        levelOrder(tree);
        break;
    }
    }
    this->ui->btn_clear->setEnabled(true);
}

void MainWindow::drawNode(BinTreeNode<QString> *node)
{
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
//设置字体
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(17);
        painter.setFont(font);
//设置画笔
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor (255,165,0));
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    setPos(node->num);

//画图，通过两种style画两遍以动态显示
    painter.drawEllipse(pos.x()-16,pos.y()-16,32,32);                           //节点
    painter.drawText(pos.x()-7,pos.y()+8,node->data);
    if(node->leftChild)
        painter.drawLine(pos.x(),pos.y()+16,pos.x()-curWid,pos.y()+80);         //左树枝
    if(node->rightChild)
        painter.drawLine(pos.x(),pos.y()+16,pos.x()+curWid,pos.y()+80);         //右树枝
    update();

    delay(160);

    pen.setColor(QColor (0,128,128));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawEllipse(pos.x()-16,pos.y()-16,32,32);
    painter.drawText(pos.x()-7,pos.y()+8,node->data);
    if(node->leftChild)
        painter.drawLine(pos.x(),pos.y()+16,pos.x()-curWid,pos.y()+80);
    if(node->rightChild)
        painter.drawLine(pos.x(),pos.y()+16,pos.x()+curWid,pos.y()+80);

    update();
    delay(180);
}

QPoint MainWindow::setPos(int num)
{
    const int maxNode=32;
    const int height=96;
    if(num>0&&num<maxNode){
        int depth=int(ceil(log2(double(num+1))));
        int rank=int(num-(pow(2,depth-1)-1));
        int xLeft=0,Tmp=depth-1;
        while (Tmp){
            xLeft+=256/int(pow(Tmp,1.6));
            Tmp--;
        }
        pos.setX(500+RankWid[depth][rank]);
        pos.setY((depth-1)*height+48);
        curWid=depWid[depth];
    }
    return pos;
}

void MainWindow::PreOrder(BinaryTree<QString> *tree)
{
    QStack<BinTreeNode<QString>*> S;
    BinTreeNode<QString> *p = tree->getRoot();
    S.push(nullptr);
     while (p != nullptr) {
         drawNode(p);
         if (p->rightChild != nullptr)
             S.push (p->rightChild);      //预留右指针在栈中
         if (p->leftChild != nullptr)
             p = p->leftChild;            //进左子树
         else
             p=S.pop();                   //左子树为空，由堆栈弹出
     }
}

void MainWindow::InOrder(BinaryTree<QString> *tree)
{
    QStack<BinTreeNode<QString>*> S;
    BinTreeNode<QString> *p = tree->getRoot();
    do {
        while (p != nullptr) {              //遍历指针向左下移动
            S.push(p);                      //该子树沿途结点进栈
            p = p->leftChild;
        }
        if (!S.isEmpty()) {                 //栈不空时退栈
            p=S.pop ();                     //退栈, 访问
            drawNode(p);
            p = p->rightChild;              //遍历指针进到右子女
        }
    } while (p != nullptr || !S.isEmpty());
}

//非递归后序遍历
void MainWindow::PostOrder(BinaryTree<QString> *tree)
{
    QStack<BinTreeNode<QString>*> S;
    BinTreeNode<QString> *p = tree->getRoot();
    BinTreeNode<QString>* tag = nullptr;
    while(p || !S.isEmpty()){
        while(p){
            S.push(p);
            p = p->leftChild;
        }
        p = S.top();
//若右节点已经访问过或者没有右节点,则输出该节点值
        if(p->rightChild==nullptr || tag == p->rightChild){
            S.pop();
            drawNode(p);
            tag = p;
            p = nullptr;
        }
        else{
            p = p->rightChild;
            tag = nullptr;
        }
    }
}

//层次序遍历
void MainWindow::levelOrder (BinaryTree<QString> *tree)
{
    BinTreeNode<QString> *p = tree->getRoot();
    if (p == nullptr)
        return;
     QQueue<BinTreeNode<QString> * > Q;
     drawNode(p);
     Q.enqueue(p);
     while (!Q.isEmpty()) {
         p=Q.dequeue();
         if (p->leftChild != nullptr) {
             drawNode(p->leftChild);
             Q.enqueue(p->leftChild);
         }
         if (p->rightChild != nullptr) {
             drawNode(p->rightChild);
             Q.enqueue(p->rightChild);
         }
     }
}

void MainWindow::on_btn_draw_clicked()
{
    int travelWay=this->ui->cbb_travelWay->currentIndex();
    if(tree->IsEmpty()){
        QString inText=this->ui->lineEdit_in->text();
        QString xText=this->ui->lineEdit_prexpost->text();
        if(isTree(inText,xText)){
            drawTree(tree,travelWay);
        }
        else
            QMessageBox::critical(this,"错误","不能构成二叉树，请检查","确定");
    }
    else{
        drawTree(tree,travelWay);
    }
}

void MainWindow::on_rbt_pre_clicked()
{
    this->ui->label_x->setText("前序");
    isPre=this->ui->rbt_pre->isChecked();
    this->ui->lineEdit_in->clear();
    this->ui->lineEdit_prexpost->clear();
    clear();
}

void MainWindow::on_rbt_post_clicked()
{
    this->ui->label_x->setText("后序");
    isPre=this->ui->rbt_pre->isChecked();
    this->ui->lineEdit_in->clear();
    this->ui->lineEdit_prexpost->clear();
    clear();
}

void MainWindow::on_btn_exm_clicked()
{
    this->ui->lineEdit_in->setText("B F D G A C E H");
    if(isPre)
        this->ui->lineEdit_prexpost->setText("A B D F G C E H");
    else
        this->ui->lineEdit_prexpost->setText("F G D B H E C A");

    QImage *testPic=new QImage("://treeExm.png");
    this->ui->label_testPic->setPixmap(QPixmap::fromImage(*testPic));
}

void MainWindow::on_btn_clear_clicked()
{
    clear();
}

void MainWindow::on_btn_exmAll_clicked()
{
    on_btn_exm_clicked();
    messageBox(1500, "将由中序+前序构建二叉树");
    for (int i=0;i<4;i++) {
        this->ui->cbb_travelWay->setCurrentIndex(i);
        on_btn_draw_clicked();
        delay(1200);
        QString travelWay=this->ui->cbb_travelWay->currentText()+"遍历结束，将进行下一种遍历!";
        messageBox(1000, travelWay);
    }
    messageBox(1500, "将由中序+后序构建二叉树");
    this->ui->rbt_post->setChecked(true);
    on_btn_exm_clicked();
    for (int i=0;i<4;i++) {
        this->ui->cbb_travelWay->setCurrentIndex(i);
        on_btn_draw_clicked();
        delay(1200);
        QString travelWay=this->ui->cbb_travelWay->currentText()+"遍历结束，将进行下一种遍历!";
        messageBox(1000, travelWay);
    }
    messageBox(2000, "全部遍历结束！");
}
