//
// Created by az on 2020/12/10.
//

#ifndef TASK5_REPLACEMENT_ALGORITHM_REPLACEMENT_ALGORITHM_H
#define TASK5_REPLACEMENT_ALGORITHM_REPLACEMENT_ALGORITHM_H

/**
 *
 * * 实验目的
 *  1. 了解虚拟存储技术的特点；
 *  2. 掌握请求分页系统的页面置换算法。
 * * 实验内容
 *  1.通过如下方法产生一指令序列，共 320 条指令。
 *      A. 在[1，32k-2]的指令地址之间随机选取一起点,访问 M；
 *      B. 顺序访问M+1；
 *      C. 在[0，M-1]中随机选取M1，访问 M1；
 *      D. 顺序访问M1+1；
 *      E. 在[M1+2，32k-2]中随机选取M2，访问 M2；
 *      F. 顺序访问M2+1；
 *      G. 重复 A—F，直到执行 320 次指令。
 * 2. 指令序列变换成页地址流设：
 *      1)页面大小为 1K；
 *      2) 分配给用户的内存页块个数为 4 页到 32 页,步长为1页；
 *      3)用户虚存容量为 32K。
 * 3. 计算并输出下述各种算法在不同内存页块下的命中率。
 *      A. 先进先出（FIFO）页面置换算法
 *      B. 最近最久未使用（LRU）页面置换算法
 *      C. 最佳（Optimal）页面置换算法
 * * 预备知识：
 *  1. 先进先出（FIFO）页面置换算法
 *      该算法总是淘汰最新进入内存的页面，即选择在内存中驻留时间最久的页面予以淘汰。
 *      该算法实现简单，只需把一个进程已调入内存的页面，按先后次序链接成一个队列，
 *      并设置一个指针，称为替换指针，使它总是指向最老的页面。
 *  2. 最近最久未使用（LRU）页面置换算法
 *      最近最久未使用（LRU）页面置换算法，是根据页面调入内存后的使用情况进行决策的。
 *      由于无法预测各页面将来的使用情况，只能利用“最近的过去”作为“最近的将来”的近似，
 *      因此，LRU 置换算法是选择最近最久未使用的页面予以淘汰。该算法赋予每个页面一个访问字段，
 *      用来记录一个页面自上次被访问以来所经历的时间 t，当需淘汰一个页面时，
 *      选择现有页面中其 t 值最大的，即最近最久未使用的页面予以淘汰。
 *  3. 最佳（Optimal）页面置换算法
 *      该算法选择的被淘汰页面，将是以后永远不使用的，或许是在最长（未来）时间内不再被访问的页面。
 *      采用该算法，通常可保证获得最低的缺页率。
 *      但由于人们目前还无法预知一个进程在内存的若干个页面中，
 *      哪一个页面是未来最长时间内不再被访问的，
 *      因而该算法是无法实现的，但可以利用该算法去评价其他算法。
 *      提示：A.命中率=1-页面失效次数/页地址流长度
 *           B.本实验中，页地址流长度为 320，页面失效次数为每次访问相应指令时，该指令所对应的页不在内存的次数。
 *  4.关于随机数产生方法，采用系统提供函数 srand(）设定种子和 rand()来产生。

 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

#define PRINT false

void printVec(vector<int> vec) {
    for (int s:vec) {
        cout << s << "  ";
    }
    cout << endl << endl;
}

vector<int> generate_instruction() {
    vector<int> tasks;
    int MIN = 1;      //最小值
    int MAX = 32766;  //最大值
    srand(time(NULL));
    while (tasks.size() < 320) {
        //在1到32k-2产生随机数
        // A.	在[1，32k-2]的指令地址之间随机选取一起点,访问 M；
        int M = rand() % MAX + MIN;
        tasks.push_back(M);
        // B.	顺序访问M+1；
        tasks.push_back(M + 1);
        // C.	在[0，M-1]中随机选取M1，访问 M1；
        int M1 = rand() % M;
        tasks.push_back(M1);
        // D.	顺序访问M1+1；
        tasks.push_back(M1 + 1);
        // E.	在[M1+2，32k-2]中随机选取M2，访问 M2；
        int M2 = rand() % (MAX - M1 - 2) + M1 + 2;
        tasks.push_back(M2);
        // F.	顺序访问M2+1；
        tasks.push_back(M2 + 1);

        // G.	重复 A—F，直到执行 320 次指令。
    }

    // 把超过数量的 pop 出去
    while (tasks.size() > 320) {
        tasks.pop_back();
    }

    return tasks;
}

//将指令序列转换成页地址
vector<int> trans_to_page(vector<int> tasks) {
    vector<int> pages;
    for (int s:tasks) {
        int page = s / 1024 + 1;
        pages.push_back(page);
    }
    return pages;
}

//获得使用 FIFO 算法的命中率，pageNum 表示分配给用户的内存页块个数，pages 表示页地址流
double FIFO_hit_rate(int pageNum, const vector<int> &pages) {
    int nHitNum = 0;//表示没有命中的个数
    vector<int> pageQueue;//内存中用户页块
    for (int s:pages) {
        auto iter = find(pageQueue.begin(), pageQueue.end(), s);
        if (iter == pageQueue.end()) {
            //内存块中没有这个页面
            //判断已有页块数是否等于pageNum
            //如果没有直接推入，否则去掉头部再推入
            nHitNum++;
            if (pageQueue.size() == pageNum) {
#if PRINT
                cout<<"FIFO out"<<*pageQueue.begin()<<"  ";
#endif
                pageQueue.erase(pageQueue.begin());
            }
            pageQueue.push_back(s);
        }
    }
    return 1 - (double) nHitNum / pages.size();
}

//定义用于LRU算法的结构
class LRUStruct {
public:
    int pageID = 0;//页块号
    int hitTime = 0;//表示访问这个页块时的时间
    LRUStruct() = default;

    LRUStruct(int page, int time) {
        pageID = page;
        hitTime = time;
    }

    bool operator<(const LRUStruct &ls) {//用于比较排序
        return hitTime < ls.hitTime;
    }

    bool operator==(int page) {
        return page == pageID;
    }
};


//使用LRU算法
double LRU_hit_rate(int pageNum, const vector<int> &pages) {
    int nHitNum = 0;//用以表示没有命中的次数
    int time = 0;//初始时间
    vector<LRUStruct> pageInMem;//内存中的，分给用户的页块区域
    for (int s:pages) {
        time++;
        //cout<<"LRU out"<<s.pageNum<<endl;
        //sort(pageInMem.begin(),pageInMem.end());//首先排序
        auto iter = find(pageInMem.begin(), pageInMem.end(), s);
        if (iter == pageInMem.end()) {
            //表示缺页
            nHitNum++;
            //查看是否装满
            if (pageInMem.size() == pageNum) {
                //这里表示装满了，所以需要进行替换
                sort(pageInMem.begin(), pageInMem.end());//首先排序
                //然后剔除第一个，再将当前的推入
#if PRINT
                cout<<"LRU out"<<pageInMem.begin()->pageID<<endl;
#endif
                pageInMem.erase(pageInMem.begin());
            }
            //推入
            LRUStruct l(s, time);
            pageInMem.push_back(l);

        } else {
            //找到了，修改hit time即可
            iter->hitTime = time;
#if PRINT
            cout<<"iter pagenum"<<iter->pageID<<"  "<<"time"<<iter->hitTime<<endl;
#endif
        }
    }
    return 1 - (double) nHitNum / pages.size();
}

//定义用于OPT算法的结构
class OPTStruct {
public:
    int pageID = 0;//页块号
    int firstHit = 0;//表示后面的第一个访问这个页块当到当前的距离
    OPTStruct() = default;

    OPTStruct(int page, int time) {
        pageID = page;
        firstHit = time;
    }

    bool operator<(const OPTStruct &ls) {//用于比较排序
        return firstHit > ls.firstHit;
    }

    bool operator==(int page) {
        return page == pageID;
    }
};

//opt算法
double OPT_hit_rate(int pageNum, const vector<int> &pages) {
    int nHitNum = 0;//没有命中的序列
    vector<OPTStruct> pageInMem;
    auto iter = pages.begin();
    for (; iter != pages.end(); iter++) {
        int currPage = *iter;
        auto iterOfInMem = find(pageInMem.begin(), pageInMem.end(), currPage);
        if (iterOfInMem == pageInMem.end()) {
            //没有找到
            nHitNum++;
            //首先考查是不是装满了
            if (pageInMem.size() == pageNum) {
                //装满了，就要移出去一个，但是首先需要更新信息
                //对在内存中的每一块都要更新
                for (OPTStruct &s:pageInMem) {
                    //在指令队列中找到第一个出现的
                    auto tarIter = find(iter, pages.end(), s.pageID);
                    if (tarIter == pages.end()) {
                        //表示后面没有这个元素，将他的值置为最大
                        s.firstHit = 1000;
                    } else {
                        int fi = tarIter - iter;
                        // cout<<fi<<"  "<<s.pageID<<endl;
                        s.firstHit = fi;
                    }
                }
                // cout<<endl;
                //然后排序
                sort(pageInMem.begin(), pageInMem.end());
                //排好序后第一个就是在未来最长时间内不会访问的
#if PRINT
                cout<<"OPT out"<<pageInMem.begin()->pageID<<endl;
#endif
                pageInMem.erase(pageInMem.begin());
            }
            OPTStruct opt(currPage, 0);
            pageInMem.push_back(opt);
        }

    }
    return 1 - (double) nHitNum / pages.size();
}


#endif //TASK5_REPLACEMENT_ALGORITHM_REPLACEMENT_ALGORITHM_H
