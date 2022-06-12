#pragma once

#include <iostream>
using namespace std;
const int processesNumbers = 5;  //进程数量
const int resourcesTypes = 3;    //资源种类

int Available[resourcesTypes] = {2, 3, 3};
int Max[processesNumbers][resourcesTypes] = {
    {5, 5, 9}, {5, 3, 6}, {3, 0, 8}, {4, 2, 5}, {4, 2, 4}};
int Allocation[processesNumbers][resourcesTypes] = {
    {2, 1, 2}, {4, 0, 2}, {3, 0, 5}, {2, 0, 4}, {3, 1, 4}};
int Need[processesNumbers + 1][resourcesTypes] = {
    {3, 4, 7}, {1, 3, 4}, {0, 0, 3}, {2, 2, 1}, {1, 1, 0}};
int Request[resourcesTypes] = {0, 0, 0};

void showStatus()  //显示系统现状
{
    cout << endl << "Available:";
    for (int j = 0; j < resourcesTypes; j++) cout << Available[j] << " ";
    cout << endl;
    for (int i = 1; i <= 5; i++) cout << "     process " << i;

    cout << endl << "Max    ";
    for (int i = 1; i <= processesNumbers; i++) {
        for (int j = 0; j < resourcesTypes; j++) cout << Max[i - 1][j] << " ";
        cout << "        ";
    }

    cout << endl << "Allo   ";
    for (int i = 1; i <= processesNumbers; i++) {
        for (int j = 0; j < resourcesTypes; j++)
            cout << Allocation[i - 1][j] << " ";
        cout << "        ";
    }

    cout << endl << "Need   ";
    for (int i = 1; i <= processesNumbers; i++) {
        for (int j = 0; j < resourcesTypes; j++) cout << Need[i - 1][j] << " ";
        cout << "        ";
    }
    cout << endl;
}

//预先检查输入是否合法
bool beforCheck(int i) {
    for (int k = 0; k < resourcesTypes; k++) {
        if (Request[k] > Need[i][k]) {
            cout << "request quantity is greater than demand quantity! "
                 << endl;
            return false;
        }
        if (Request[k] > Available[k]) {
            cout << "request quantity is greater than available quantity! "
                 << endl;
            return false;
        }
    }
    return true;
}

//当请求被允许时，更新系统各参数
void change(int i) {
    for (int j = 0; j < resourcesTypes; j++) {
        Available[j]     = Available[j] - Request[j];
        Allocation[i][j] = Allocation[i][j] + Request[j];
        Need[i][j]       = Need[i][j] - Request[j];
    }
}

//当有请求时，判断是否会使系统产生死锁，若否，输出当前的安全序列
int check() {
    int Finish[processesNumbers] = {0};
    // Work为系统当前资源矩阵的镜像
    int Work[resourcesTypes];
    // Result记录安全序列
    int Result[processesNumbers] = {0};
    // sum为系统中已经安全执行并释放资源的进程数
    int sum = 0;
    //初始化Work
    for (int i = 0; i < resourcesTypes; i++) Work[i] = Available[i];

    while (true) {
        // isSatisfied 判定系统当前资源量能否满足某进程还需要的资源量
        // isSafeExcu  判定当前系统是否还有进程能安全执行并释放资源
        bool isSatisfied, isSafeExcu = 0;

        for (int i = 0; i < processesNumbers; i++) {
            isSatisfied = 1;
            if (Finish[i] == 0) {
                for (int j = 0; j < resourcesTypes; j++)
                    if (Need[i][j] > Work[j]) isSatisfied = 0;
                //若某进程还需要的资源能安全得到
                if (isSatisfied) {
                    isSafeExcu = 1;
                    //进程执行完毕，释放该进程得到的资源
                    for (int j = 0; j < resourcesTypes; j++)
                        Work[j] = Work[j] + Allocation[i][j];
                    Finish[i] = 1;  //状态矩阵记录当前进程已执行完毕释放
                    Result[sum] = i + 1;  //在安全序列中记录进程号
                    sum++;                //完成的进程数增加1
                }
            }
        }
        //若所有的进程都安全执行完毕，则系统是安全的
        if (sum == processesNumbers) {
            cout << "the security sequence of the current system is:";
            for (int i = 0; i < processesNumbers; i++) cout << Result[i] << " ";
            cout << endl;
            cout << endl;
            return 0;
        }

        //若进程执行完且剩余的进程无法获得足够的资源，则说明系统是不安全的
        if (!isSafeExcu) {
            return 1;
        }
    }
}

void restore(int i) {  //当请求不被允许时，恢复系统各参数
    cout << "The request will cause the system to deadlock and refuse to "
            "execute!"
         << endl;
    for (int j = 0; j < resourcesTypes; j++) {
        Available[j] = Available[j] + Request[j];
        Allocation[i][j] = Allocation[i][j] - Request[j];
        Need[i][j] = Need[i][j] + Request[j];
    }
}

int run(int flag) {
    int i = 0;
    showStatus();
    if (flag == 1) {
        cout << "input the process number to apply for resources:";
        cin >> i;
        cout << "input the resources numbers of process" << i
             << " need to request" << endl;
        i--;
        cout << "resources A,B,C:";
        cin >> Request[0] >> Request[1] >> Request[2];
        cout << endl;
        if (beforCheck(i)) {
            change(i);
            if (check()) {
                restore(i);
                showStatus();
            } else
                showStatus();
        }

        cout << "continue?, 1 for yes，0 for no:";
        cin >> flag;
    }
    return flag;
}

void bankersAlgorithm() {
    int flag = 1;
    while (flag == 1) flag = run(flag);
}
