#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QStack>
#include <QQueue>
#include <QTime>
#include <QElapsedTimer>
#include <algorithm>
#include <vector>
#include <functional>
#include <myGraph.h>
#include <delay.h>

// TODO
// 耗散值
// 生成节点数
// 统计
// 运行时间

class Algorithm : public QObject
{
    Q_OBJECT
public:
    Algorithm() {}
    virtual ~Algorithm() {}

    const int vertexNum = 20;
    QColor originalColor = QColor("#c2ccd0");
    QColor tryColor = QColor("#56004f");
    QColor finishedColor = QColor("#ff3300");

    //预估代价函数
    int h[20] = {366, 0, 160, 242, 161, 176, 77, 151, 226, 244,
                 241, 234, 380, 100, 193, 253, 329, 80, 199, 374};

    //默认为找到从初始地点 Arad 到 目的地点 Bucharest 的最短路径
    bool breadthFirstSearch(MGraph *mGraph)
    {
        breadthFirstSearchStatistics(mGraph);
        int nodesCount = 1;

        int node = 0;
        int isPaintEdge[20][20] = {{0}};
        bool isPaintEdgeStore[20][20] = {{false}};
        int isPaintVertex[20] = {0};

        int fatherNode[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        QQueue<int> _nodeQueue;
        bool isVisited[20] = {false};

        _nodeQueue.enqueue(node);

        isVisited[node] = true;
        isPaintVertex[node] = true;

        while (!_nodeQueue.isEmpty())
        {
            int focus = _nodeQueue.dequeue();
            int i;
            for (i = 0; i < mGraph->vertexNum; i++)
            {
                //如果为邻接点
                if (mGraph->G[focus][i] != GINFINITY)
                {

                    //如果未访问过，标记为已访问
                    if (!isVisited[i])
                    {
                        nodesCount++;
                        isPaintEdge[focus][i] = 1;
                        isPaintEdge[i][focus] = 1;
                        isPaintEdgeStore[focus][i] = true;
                        isPaintEdgeStore[i][focus] = true;
                        isPaintVertex[i] = true;

                        isVisited[i] = true;

                        fatherNode[i] = focus;

                        _nodeQueue.enqueue(i); //访问过的节点入队

                        if (i == 1)
                        {
                            emit sendToDrawGraph(isPaintEdge, isPaintVertex);
                            emit sendVisitedNodes(nodesCount);
                            emit sendPathLength(getPathLength(fatherNode,1,mGraph));
                            //printG(fatherNode);
                            return true;
                        }
                    }
                    else
                    {
                        //已访问过，标记为试探边
                        isPaintEdge[focus][i] = -1;
                        isPaintEdge[i][focus] = -1;
                    }
                }
            }

            emit sendToDrawGraph(isPaintEdge, isPaintVertex);
            emit sendVisitedNodes(nodesCount);

            Delay::run(200);

            for (int i = 0; i < 20; i++)
            {
                //把试探的节点恢复
                for (int j = 0; j < 20; j++)
                {
                    if (isPaintEdge[i][j] == -1)
                        isPaintEdge[i][j] = 0;
                    if (isPaintEdgeStore[i][j])
                        isPaintEdge[i][j] = true;
                }
                if (isVisited[i])
                    isPaintVertex[i] = 1;
            }
        }
        return false;
    }

    bool depthFirstSearch(MGraph *mGraph)
    {
        depthFirstSearchStatistics(mGraph);
        int nodesCount = 1;
        int pathLength = 0;
        int node = 0;
        int isPaintEdge[20][20] = {{0}};
        bool isPaintEdgeStore[20][20] = {{false}};
        int isPaintVertex[20] = {0};

        QStack<int> _nodeStack;
        bool isVisited[20] = {false};

        _nodeStack.push(node);
        int focus = node;
        isVisited[node] = true;
        isPaintVertex[node] = true;

        while (!_nodeStack.isEmpty())
        {
            int i;
            //遍历图内所有节点
            for (i = 0; i < mGraph->vertexNum; i++)
            {
                //如果为邻接点
                if (mGraph->G[focus][i] != GINFINITY)
                {

                    //如果未访问过，标记为已访问
                    if (!isVisited[i])
                    {
                        nodesCount++;
                        isPaintEdge[focus][i] = 1;
                        isPaintEdge[i][focus] = 1;
                        isPaintEdgeStore[focus][i] = true;
                        isPaintEdgeStore[i][focus] = true;
                        isPaintVertex[i] = true;
                        isVisited[i] = true;
                        pathLength += mGraph->G[focus][i];

                        _nodeStack.push(i); //访问过的节点压入栈中
                        focus = i;          //更新当前节点
                        break;
                    }
                    else
                    {
                        //已访问过，标记为试探边
                        isPaintEdge[focus][i] = -1;
                        isPaintEdge[i][focus] = -1;
                    }
                }
            }

            emit sendToDrawGraph(isPaintEdge, isPaintVertex);
            emit sendVisitedNodes(nodesCount);
            Delay::run(200);

            if(i==1){
                emit sendPathLength(pathLength);
                return true;
            }

            for (int i = 0; i < 20; i++)
            { //把试探的节点恢复
                for (int j = 0; j < 20; j++)
                {
                    if (isPaintEdge[i][j] == -1)
                        isPaintEdge[i][j] = 0;
                    if (isPaintEdgeStore[i][j])
                        isPaintEdge[i][j] = true;
                }
                if (isVisited[i])
                    isPaintVertex[i] = 1;
            }

            if (i == mGraph->vertexNum)
            { //节点都试探过，出栈回退
                _nodeStack.pop();
                if (!_nodeStack.isEmpty())
                {
                    focus = _nodeStack.top();
                }
            }
        }
        return true;
    }

    bool greedyAlgorithm(MGraph *mGraph)
    {
        greedyAlgorithmStatistics(mGraph);
        int nodesCount = 1;
        int pathLength = 0;
        int currentNode = 0;
        int targetNode = 1;
        int isPaintEdge[20][20] = {{0}};
        int isPaintVertex[20] = {0};

        bool isVisited[20] = {false};

        isVisited[currentNode] = true;
        isPaintVertex[currentNode] = true;

        int shortestNode;
        int shortestWeight;

        while (currentNode != targetNode)
        {
            // 初始化currentNode可访问到的最短节点和路径长度
            shortestNode = currentNode;
            shortestWeight = 65535;
            // 遍历所有节点
            for (int i = 0; i < mGraph->vertexNum; i++)
            {
                // 如果可访问 & 未访问
                if (mGraph->G[currentNode][i] != GINFINITY && isVisited[i] == false)
                {
                    // 标记为试探访问过的节点
                    isPaintEdge[currentNode][i] = -1;
                    isPaintEdge[i][currentNode] = -1;

                    // 更新可访问到的最短路径长度和节点
                    if (h[i] < shortestWeight)
                    {
                        shortestNode = i;
                        shortestWeight = h[i];
                    }
                }
            }

            // 标记访问过的边和节点
            nodesCount++;
            pathLength+=mGraph->G[currentNode][shortestNode];
            isPaintEdge[currentNode][shortestNode] = 1;
            isPaintEdge[shortestNode][currentNode] = 1;
            isPaintVertex[shortestNode] = 1;
            isVisited[shortestNode] = true;

            // 发送绘图信号
            emit sendToDrawGraph(isPaintEdge, isPaintVertex);
            emit sendVisitedNodes(nodesCount);

            Delay::run(200);

            // 更新当前访问的节点
            currentNode = shortestNode;

            // 将试探边信息清空
            for (int i = 0; i < 20; i++)
            { //把试探的节点恢复
                for (int j = 0; j < 20; j++)
                {
                    if (isPaintEdge[i][j] == -1)
                        isPaintEdge[i][j] = 0;
                }
            }
        }
        emit sendPathLength(pathLength);
        return true;
    }

    bool AStarAlgorithm(MGraph *mGraph)
    {
        AStarAlgorithmStatistics(mGraph);
        int nodesCount = 1;
        int pathLength = 0;
        int currentNode = 0;
        int targetNode = 1;

        int isPaintEdge[20][20] = {{0}};
        int isPaintVertex[20] = {0};
        isPaintVertex[currentNode] = true;

        bool isVisited[20] = {false};
        isVisited[currentNode] = true;

        int shortestNode;
        int shortestWeight;

        while (currentNode != targetNode)
        {
            shortestNode = currentNode;
            shortestWeight = 65535;
            // 遍历所有节点
            for (int i = 0; i < mGraph->vertexNum; i++)
            {
                // 如果可访问 & 未访问
                if (mGraph->G[currentNode][i] != GINFINITY && isVisited[i] == false)
                {
                    // 标记为试探访问过的节点
                    isPaintEdge[currentNode][i] = -1;
                    isPaintEdge[i][currentNode] = -1;

                    // 更新可访问到的最短路径长度和节点
                    if (mGraph->G[currentNode][i] + h[i] < shortestWeight)
                    {
                        shortestNode = i;
                        shortestWeight = mGraph->G[currentNode][i] + h[i];
                    }
                }
            }


            // 标记访问过的边和节点
            nodesCount++;
            pathLength+=mGraph->G[currentNode][shortestNode];
            isPaintEdge[currentNode][shortestNode] = 1;
            isPaintEdge[shortestNode][currentNode] = 1;
            isPaintVertex[shortestNode] = 1;
            isVisited[shortestNode] = true;

            // 发送绘图信号
            emit sendToDrawGraph(isPaintEdge, isPaintVertex);
            emit sendVisitedNodes(nodesCount);
            Delay::run(200);

            // 更新当前访问的节点
            currentNode = shortestNode;

            // 将试探边信息清空
            for (int i = 0; i < 20; i++)
            { //把试探的节点恢复
                for (int j = 0; j < 20; j++)
                {
                    if (isPaintEdge[i][j] == -1)
                        isPaintEdge[i][j] = 0;
                }
            }


        }

        emit sendPathLength(pathLength);
        return true;
    }

    bool breadthFirstSearchStatistics(MGraph *mGraph)
    {
        QElapsedTimer timer;
        timer.start();
        int node = 0;
        QQueue<int> _nodeQueue;
        bool isVisited[20] = {false};

        _nodeQueue.enqueue(node);

        int visitedNodesCount = 1;
        isVisited[node] = true;
        while (!_nodeQueue.isEmpty())
        {
            int focus = _nodeQueue.dequeue();
            int i;
            visitedNodesCount++; //计数已遍历的节点
            for (i = 0; i < mGraph->vertexNum; i++)
            {
                //如果为邻接点
                if (mGraph->G[focus][i] != GINFINITY)
                {
                    //如果未访问过，标记为已访问
                    if (!isVisited[i])
                    {
                        isVisited[i] = true;
                        _nodeQueue.enqueue(i); //访问过的节点入队
                        if (i == 1)
                        {
                            double runTime = timer.nsecsElapsed();
                            qDebug()<<runTime;
                            emit sendRunTime(runTime);
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool depthFirstSearchStatistics(MGraph *mGraph)
    {
        QElapsedTimer timer;
        timer.start();

        int node = 0;

        QStack<int> _nodeStack;
        bool isVisited[20] = {false};

        _nodeStack.push(node);
        int focus = node, count = 1;
        isVisited[node] = true;

        while (!_nodeStack.isEmpty())
        {
            int i;
            //遍历图内所有节点
            for (i = 0; i < mGraph->vertexNum; i++)
            {
                //如果为邻接点
                if (mGraph->G[focus][i] != GINFINITY)
                {
                    //如果未访问过，标记为已访问
                    if (!isVisited[i])
                    {
                        isVisited[i] = true;
                        _nodeStack.push(i); //访问过的节点压入栈中
                        focus = i;          //更新当前节点
                        count++;            //计数已遍历的节点
                        break;
                    }
                }
            }

            if(i==1){
                double runTime = timer.nsecsElapsed();
                qDebug()<<runTime;
                emit sendRunTime(runTime);
                return true;

            }

            if (i == mGraph->vertexNum)
            { //节点都试探过，出栈回退
                _nodeStack.pop();
                if (!_nodeStack.isEmpty())
                {
                    focus = _nodeStack.top();
                }
            }



        }

        if (count != mGraph->vertexNum)
        {
            return false;
        }

        return true;
    }

    bool greedyAlgorithmStatistics(MGraph *mGraph)
    {
        QElapsedTimer timer;
        timer.start();
        int currentNode = 0;
        int targetNode = 1;

        bool isVisited[20] = {false};

        isVisited[currentNode] = true;

        int shortestNode;
        int shortestWeight;

        while (currentNode != targetNode)
        {
            // 初始化currentNode可访问到的最短节点和路径长度
            shortestNode = currentNode;
            shortestWeight = 65535;
            // 遍历所有节点
            for (int i = 0; i < mGraph->vertexNum; i++)
            {
                // 如果可访问 & 未访问
                if (mGraph->G[currentNode][i] != GINFINITY && isVisited[i] == false)
                {
                    // 更新可访问到的最短路径长度和节点
                    if (h[i] < shortestWeight)
                    {
                        shortestNode = i;
                        shortestWeight = h[i];
                    }
                }
            }

            isVisited[shortestNode] = true; // 标记访问过的边和节点
            currentNode = shortestNode;     // 更新当前访问的节点
        }

        double runTime = timer.nsecsElapsed();
        qDebug()<<runTime;
        emit sendRunTime(runTime);

        return true;
    }

    bool AStarAlgorithmStatistics(MGraph *mGraph)
    {
        QElapsedTimer timer;
        timer.start();
        int currentNode = 0;
        int targetNode = 1;

        bool isVisited[20] = {false};

        isVisited[currentNode] = true;

        int shortestNode;
        int shortestWeight;

        while (currentNode != targetNode)
        {
            // 初始化currentNode可访问到的最短节点和路径长度
            shortestNode = currentNode;
            shortestWeight = 65535;
            // 遍历所有节点
            for (int i = 0; i < mGraph->vertexNum; i++)
            {
                // 如果可访问 & 未访问
                if (mGraph->G[currentNode][i] != GINFINITY && isVisited[i] == false)
                {
                    // 更新可访问到的最短路径长度和节点
                    if (mGraph->G[currentNode][i] + h[i] < shortestWeight)
                    {
                        shortestNode = i;
                        shortestWeight = h[i] + mGraph->G[currentNode][i];
                    }
                }
            }

            // 标记访问过的边和节点
            isVisited[shortestNode] = true;
            // 更新当前访问的节点
            currentNode = shortestNode;
        }

        double runTime = timer.nsecsElapsed();
        qDebug()<<runTime;
        emit sendRunTime(runTime);

        return true;
    }


    int getPathLength(int fatherNodes[20],int destination,MGraph *mGraph){
        int pathLength = 0;
        int currentNode=destination;
        while (fatherNodes[currentNode]!=-1) {
            pathLength+=mGraph->G[currentNode][fatherNodes[currentNode]];
            currentNode=fatherNodes[currentNode];
        }
        return pathLength;
    }

    void printG(int g[])
    {
        for (int i = 0; i < 20; i++)
        {
            qDebug() << "g" << i << g[i];
        }
    }

signals:
    void sendToDrawGraph(int isPaintEdge[20][20], int isPaintVertex[20]);

    void sendRunTime(double nSec);
    void sendVisitedNodes(int nodesNum);
    void sendPathLength(int length);
};

#endif // ALGORITHM_H
