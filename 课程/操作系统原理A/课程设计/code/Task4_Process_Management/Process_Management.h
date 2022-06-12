//
// Created by az on 2020/12/10.
//

#ifndef TASK4_PROCESS_MANAGEMENT_PROCESS_MANAGEMENT_H
#define TASK4_PROCESS_MANAGEMENT_PROCESS_MANAGEMENT_H

#include <iostream>

using namespace std;

/**
 * @author oaeen
 * * 实验目的
 * * 1. 理解进程的概念，明确进程和程序的区别。
 * * 2.	理解并发执行的实质。
 * * 3.	掌握进程的同步、撤销等进程控制方法。
 *
 * TODO: 实验内容
 * TODO: 父进程使用系统调用pipe()建立一个管道，然后使用系统调用fork()创建两个子进程：子进程1和子进程2
 * TODO: 子进程1每隔1秒通过管道向子进程2发送数据：I send message x times.（x初值为1，以后发送一次后做加一操作）
 * TODO: 子进程2从管道读出信息，并显示在屏幕上
 * TODO: 父进程用系统调用signal()来捕捉来自键盘的中断信号SIGINT（即按Ctrl+C键,）；当捕捉到中断信号后，父进程用系统调用kill()向两个子进程发出信号
 * TODO: 子进程捕捉到信号后分别输出如下信息后终止： Child Process 1 is killed by Parent! Child Process 2 is killed by Parent!
 * TODO: 父进程等待两个子进程终止后，释放管道并输出如下的信息后终止 Parent Process is Killed!

 */


#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

pid_t pid1;
pid_t pid2;
int fd[2];

void child_process1(int s) {
    cout << endl << "child process 1 is killed by parent! " << endl;
    exit(0);
}

void child_process2(int s) {
    cout << endl << "child process 2 is killed by parent! " << endl;
    exit(0);
}

void parent_kill(int s) {
    int status1;
    int status2;

    kill(pid1, SIGTERM);
    wait(&status1);
    kill(pid2, SIGTERM);

    wait(&status2);
    close(fd[0]);
    close(fd[1]);
    cout << endl << "parent process is killed!" << endl;
    exit(0);
}

void run() {

    if (pipe(fd) == -1) {
        cout << "error creating pipeline" << endl;
        return;
    }

    while((pid1=fork())==-1);

    if (pid1 == 0) {
        int n = 1;
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, child_process1);
        close(fd[0]);

        while (true) {
            string str = "I send message " + to_string(n) + " times";
            const char *p = str.c_str();

            write(fd[1], p, strlen(p));
            sleep(1);
            n++;
        }

        close(fd[1]);
        exit(0);
    }

    while((pid2=fork())==-1);
    if (pid2 == 0) {
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, child_process2);
        close(fd[1]);

        while (true) {
            char p[100];
            int len = read(fd[0], p, sizeof(p));
            cout << p << endl;
        }

        close(fd[0]);
        exit(0);
    } else {
        sleep(1);
        signal(SIGINT, SIG_IGN);
        while (1)
            signal(SIGINT, parent_kill);
    }

    return;
}


#endif //TASK4_PROCESS_MANAGEMENT_PROCESS_MANAGEMENT_H
