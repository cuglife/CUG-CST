//
// Created by az on 2020/12/10.
//

#ifndef TASK6_PROCESS_COMMUNICATION_PROCESS_COMMUNICATION_H
#define TASK6_PROCESS_COMMUNICATION_PROCESS_COMMUNICATION_H

/**
 * 编写一主程序可以由用户选择如下三种进程通信方式：
 * 1. 使用管道来实现父子进程之间的进程通信
 *    子进程向父进程发送自己的进程标识符，以及字符串“is sending a message toparent”。
 *    父进程则通过管道读出子进程发来的消息，将消息显示在屏幕上，然后终止。
 * 2. 使用消息缓冲队列来实现 client 进程和 server 进程之间的通信
 *    server 进程先建立一个关键字为 SVKEY（如 75）的消息队列，然后等待接收类型为 REQ(例如 1)的消息；
 *    在收到请求消息后，它便显示字符串“serving for client”和接收到的 client 进程的进程标识数，
 *    表示正在为 client 进程服务；然后再向 client 进程发送应答消息，
 *    该消息的类型是 client 进程的进程标识数，而正文则是 server 进程自己的标识ID
 *    client 进程则向消息队列发送类型为 REQ 的消息（消息的正文为自己的进程标识 ID） 以取得 sever 进程的服务，
 *    并等待 server 进程发来的应答；然后显示字符串“receive replyfrom”和接收到的 server 进程的标识 ID
 * 3. 使用共享存储区来实现两个进程之间的进程通信
 *    进程 A 创建一个长度为 512 字节的共享内存，并显示写入该共享内存的数据；
 *    进程 B 将共享内存附加到自己的地址空间，并向共享内存中写入数据。
 */


#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

using namespace std;

//采用管道通信
void pipe_con() {
    int status;
    int fd[2];
    if (pipe(fd) == -1) {
        cout << "创建管道错误" << endl;
        return;
    }
    pid_t pid = fork();
    if (pid == -1) {
        cout << "创建子进程失败" << endl;
        return;
    } else if (pid == 0) {
        auto id = getpid();
        string s = to_string(id);
        s += " is sending a message to parent";
        //char *m="is sending a message to parent";
        char *m = new char[s.size() + 2];
        s.copy(m, s.size(), 0);
        m[s.size()] = 0;
        close(fd[0]);
        write(fd[1], m, strlen(m));
        close(fd[1]);
        return;
        //exit(0);
    } else {
        close(fd[1]);
        char p[100];
        int len = read(fd[0], p, sizeof(p));
        p[len] = 0;
        cout << p << endl;
        close(fd[0]);
        wait(&status);
    }
    return;
}

//采用消息队列
#define SVKEY 75 //定义关键字SVKEY
struct msgform  //消息结构
{
    long mtype;
    char mtext[250];  //文本长度
} msg;

int msgqid, pid, *pint, i;

void client() {
    msgqid = msgget(SVKEY, 0777);  //打开 75#消息队列
    pid = getpid();  //获取client进程标识符
    pint = (int *) msg.mtext;  //把正文的内容传给 pint，并强制转换类型
    *pint = pid;  //pint指针指向client进程标识符
    msg.mtype = 1;  //消息类型为 1
    msgsnd(msgqid, &msg, sizeof(int), 0);  //发送消息msg入msgqid消息队列
    msgrcv(msgqid, &msg, 250, pid, 0);  //从队列msgqid接收消息msg
    cout << "(client):receive reply from pid=" << *pint << endl;//显示 server进程标识数
    exit(0);
}

void server() {
    msgqid = msgget(SVKEY, 0777 | IPC_CREAT);  //创建 75#消息队列
    msgrcv(msgqid, &msg, 250, 1, 0);  //接收client进程标识数消息
    pint = (int *) msg.mtext;  //把正文的内容传给 pint，并强制转换类型
    pid = *pint;  //获得 cilent 进程标识数
    cout << "(server):serving for client pid=" << pid << endl;
    msg.mtype = pid;  //消息类型为 client 进程标识数
    *pint = getpid();  //获取 server 进程标识数
    msgsnd(msgqid, &msg, sizeof(int), 0);  //发送消息
    exit(0);
}

void msg_queen() {
    while ((i = fork()) == -1);  //创建进程 1
    if (!i)server();
    while ((i = fork()) == -1); //创建进程 2
    if (!i) client();
    wait(0);
    wait(0);
}

void shared_mem_comm() {
    int id;
    char *addr;
    char message[512];
    id = shmget(75, 512, 0777 | IPC_CREAT);
    if (fork() == 0) {
        sprintf(message, "%d is sending a message to parent", getpid());
        cout << "mess " << message << endl;
        addr = (char *) shmat(id, 0, 0);
        strcpy(addr, message);
        shmdt(addr);
    } else {
        wait(0);
        addr = (char *) shmat(id, 0, 0);
        cout << "addr " << addr << endl;
        shmdt(addr);
        shmctl(id, IPC_RMID, 0);
    }
}

#endif //TASK6_PROCESS_COMMUNICATION_PROCESS_COMMUNICATION_H
