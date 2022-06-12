#ifndef MYGRAPH_H
#define MYGRAPH_H

#include <QPoint>
#include <QVector>
#include <cstring>
#define MaxVertexNum 20 //最大顶点数设为20
#define GINFINITY 65535 //∞设为双字节无符号整数的最大值65535
#define DRAWTIME 500    //画图时间

/*
 *
 *
 *
 *
 *
 */

class GEdge
{
public:
    int vertex1;
    int vertex2;
    int weight;

    GEdge() {}

    GEdge(int V1, int V2, int W)
    {
        vertex1 = V1;
        vertex2 = V2;
        weight = W;
    }
};

class MGNode
{
public:
    int num;
    QPoint position;
    QString cityInfor;

    MGNode(int n, QString infor, QPoint pos)
    {
        num = n;
        cityInfor = infor;
        position = pos;
    }
};

class MGraph
{
public:
    int vertexNum;                     //顶点数
    int edgeNum;                       //边数
    QVector<MGNode> data;              //存节点信息
    QVector<GEdge> edge;               //存节点信息
    int G[MaxVertexNum][MaxVertexNum]; //邻接矩阵

    //初始化一个有VertexNum个顶点但没有边的图
    MGraph(int VertexNum)
    {
        int V, W;
        vertexNum = VertexNum;
        edgeNum = 0;

        //初始化邻接矩阵; 这里默认顶点编号从0开始，到(Graph->Nv - 1)
        for (V = 0; V < vertexNum; V++)
            for (W = 0; W < vertexNum; W++)
                G[V][W] = GINFINITY;
    }

    void insertEdge(GEdge edge)
    {
        G[edge.vertex1][edge.vertex2] = edge.weight;
        G[edge.vertex2][edge.vertex1] = edge.weight;
    }
};

#endif // MYGRAPH_H
