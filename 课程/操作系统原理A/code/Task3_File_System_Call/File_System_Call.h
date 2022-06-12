//
// Created by az on 2020/12/10.
//

#ifndef TASK3_FILE_SYSTEM_CALL_FILE_SYSTEM_CALL_H
#define TASK3_FILE_SYSTEM_CALL_FILE_SYSTEM_CALL_H


#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>
#include <string.h>

using namespace std;

/**
 * @author oaeen
 * * 实验目的
 * * 1. 掌握 linux 提供的文件系统调用的使用方法；
 * * 2. 熟悉文件系统的系统调用用户接口；
 * * 3. 了解操作系统文件系统的工作原理和工作方式。
 *
 * TODO: 使用文件系统调用编写一个文件工具 filetools，使其具有以下功能：
 * TODO: 1.创建新文件
 * TODO: 2.写文件
 * TODO: 3.读文件
 * TODO: 4.修改文件权限
 * TODO: 5.查看当前文件权限
 * TODO: 0.退出
 *
 * TODO: 提示用户输入功能号，并根据用户输入的功能选择相应的功能。
 * TODO: 文件按可变记录文件组织，具体记录内容自行设计。
 *
 * ! 1. 必须用系统调用实现，不能使用C库函数
 * ! 2. 文件按可变记录文件组织，具体记录内容自行设计
 * ! 3. 文件系统调用详见：http://www.linuxidc.com/Linux/2016-11/137612.htm
 * ! 4. 改变文件权限的系统调用为chmod，详见：https://www.cnblogs.com/nufangrensheng/p/3502457.html , https://www.cnblogs.com/chengJAVA/p/4319420.html
 * ! 备注：系统调用提供的函数需包含头文件unistd.h
 *
 */



class File_System {
public:
    void create_file(const char *fileName) {
        umask(0000);
        int file = open(fileName, O_RDWR | O_CREAT | O_EXCL, 0666);

        if (file == -1) {
            cout << "file exists! " << endl;
            return;
        }

        cout << "write content to the file? (y/n)" << endl;
        string s;
        cin >> s;

        if (s == "y" || s == "Y") {
            //向文件中写入内容
            cout << "input data you want to write, finished with enter" << endl;
            string s;
            cin >> s;
            write_data(fileName, s);
            cout << "write successfully" << endl;
        }

        close(file);
    }

    void write_data(const char *fileName, const string &s) {
        int file = open(fileName, O_RDWR | O_APPEND);
        if (file == -1) {
            cout << "open file error" << endl;
        }
        // write s
        const char *k = s.c_str();
        int does_write_ok = write(file, k, strlen(k));
        if (does_write_ok == -1) {
            cout << "write error" << endl;
            return;
        }
        close(file);
    }

    void read_file(const char *fileName) {
        int fd = open(fileName, O_RDONLY);
        if (fd == -1) {
            cout << "read file error" << endl;
        }
        cout << "the content of the file is：" << endl;
        char buf[1024];
        int re = read(fd, buf, 1024);
        close(fd);
        for (int i = 0; i < re; i++) {
            if (buf[i] <= 0)
                buf[i] = 0;
        }
        buf[re] = 0;
        printf("%s", buf);
        cout << endl;
    }

    void display_permissions(char *fileName) {
        int status;
        pid_t pid = fork();
        if (pid == 0) {
            char *execv_str[] = {"ls", "-l", fileName, NULL};
            if (execv("/bin/ls", execv_str) < 0) {
                cout << "file not found" << endl;
            }
        } else
            wait(&status);
    }

    void change_permissions(char *fileName) {
        cout << "current permission: " << endl;
        display_permissions(fileName);

        cout << "enter new permissions: " << endl;
        int p;
        cin >> p;

        int hundreds = p / 100;
        int tens = (p / 10) % 10;
        int ones = p % 10;

        bool is_format_wrong = p < 0 || p > 777 || tens > 7 || ones > 7;

        if (is_format_wrong) {
            cout << "enter permissions: error! " << endl;
            return;
        } else {
            // trans to dec
            int permissions = hundreds * 8 * 8 + tens * 8 + ones;
            cout << "permissions trans to decimal: " << permissions << endl;
            chmod(fileName, permissions);
            cout << "file permissions after modification: " << endl;
            display_permissions(fileName);
        }

    }

    void run() {
        int flag = 1;
        while (flag) {

            cout << "请输入对应的功能" << endl;
            cout << "1. 创建新文件" << endl;
            cout << "2. 写文件" << endl;
            cout << "3. 读文件" << endl;
            cout << "4. 修改文件权限" << endl;
            cout << "5. 查看文件权限" << endl;
            cout << "0. 退出" << endl;
            cin >> flag;
            if (flag != 0) {
                string fileName;
                cout << "input file name" << endl;
                cin >> fileName;
                auto p = fileName.c_str();
                switch (flag) {
                    case 1: {
                        create_file(p);
                        break;
                    }
                    case 2: {
                        cout << "请输入要输入的内容" << endl;
                        string data;
                        cin >> data;
                        write_data(p, data);
                        break;
                    }
                    case 3: {
                        read_file(p);
                        break;
                    }
                    case 4: {
                        auto p = new char[fileName.length() + 1];
                        strcpy(p, fileName.c_str());
                        change_permissions(p);
                        break;
                    }
                    case 5: {
                        auto p = new char[fileName.length() + 1];
                        strcpy(p, fileName.c_str());
                        display_permissions(p);
                    }
                    case 0:
                        break;
                }
            }

        }
    }
};


#endif //TASK3_FILE_SYSTEM_CALL_FILE_SYSTEM_CALL_H
