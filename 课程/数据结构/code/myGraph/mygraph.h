#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <QPoint>
#include <QVector>
#include <cstring>
#define MaxVertexNum 10                //最大顶点数设为10
#define GINFINITY 65535                //∞设为双字节无符号整数的最大值65535
#define DRAWTIME 500                   //画图时间
class GEdge{
public:
    int Vertex1,Vertex2;
    int Weight;
    GEdge(){}
    GEdge(int V1,int V2,int W){Vertex1=V1;Vertex2=V2;Weight=W;}
};

class MGNode{
public:
    int num;
    QString information;
    QPoint position;
    MGNode(int n,QString infor,QPoint pos){num=n;information=infor;position=pos;}
};

class MGraph{
public:
    int VertexNum;          //顶点数
    int EdgeNum;            //边数
    QVector<MGNode> data;    //存节点信息
    QVector<GEdge> edge;    //存节点信息
    int G[MaxVertexNum][MaxVertexNum];   //邻接矩阵

    MGraph* CreateGraph( int VertexNum ){//初始化一个有VertexNum个顶点但没有边的图
        int V, W;
        MGraph *Graph=new MGraph();
        Graph->VertexNum = VertexNum;
        Graph->EdgeNum = 0;
        //初始化邻接矩阵; 这里默认顶点编号从0开始，到(Graph->Nv - 1)
        for (V=0; V<Graph->VertexNum; V++)
            for (W=0; W<Graph->VertexNum; W++)
                Graph->G[V][W] = GINFINITY;
        return Graph;
    }

    void insertEdge(GEdge edge){
         G[edge.Vertex1][edge.Vertex2] = edge.Weight;
         G[edge.Vertex2][edge.Vertex1] = edge.Weight;
    }
};

class LGNode{
public:
    int num;
    QString information;
    QPoint position;
    LGNode* next;
    LGNode(){next=nullptr;}
    LGNode(int n,QString infor,QPoint pos){num=n;information=infor;position=pos;next=nullptr;}
};

class LGraph{
public:
    int VertexNum;          //顶点数
    int EdgeNum;            //边数
    QVector<LGNode> AdjList;
    LGraph(){VertexNum=0;EdgeNum=0;}

//    void InsertEdge(GEdge E)
//    {
//        PtrToAdjVNode NewNode;

//        /* 插入边 <V1, V2> */
//        /* 为V2建立新的邻接点 */
//        NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
//        NewNode->AdjV = E->V2;
//        NewNode->Weight = E->Weight;
//        /* 将V2插入V1的表头 */
//        NewNode->Next = Graph->G[E->V1].FirstEdge;
//        Graph->G[E->V1].FirstEdge = NewNode;

//        /* 若是无向图，还要插入边 <V2, V1> */
//        /* 为V1建立新的邻接点 */
//        NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
//        NewNode->AdjV = E->V1;
//        NewNode->Weight = E->Weight;
//        /* 将V1插入V2的表头 */
//        NewNode->Next = Graph->G[E->V2].FirstEdge;
//        Graph->G[E->V2].FirstEdge = NewNode;
//    }

};

#endif // MYGRAPH_H
