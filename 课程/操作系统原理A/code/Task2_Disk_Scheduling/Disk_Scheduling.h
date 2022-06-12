//
// Created by az on 2020/12/10.
//

#ifndef TASK2_DISK_SCHEDULING_DISK_SCHEDULING_H
#define TASK2_DISK_SCHEDULING_DISK_SCHEDULING_H

#include <iostream>
#include <climits>
#include <algorithm>
#include <cstdio>

using namespace std;

/**
 * @author oaeen
 * * 对磁盘调度的相关知识作进一步的了解，明确磁盘调度的原理
 * * 加深理解磁盘调度的主要任务
 * * 通过编程，掌握磁盘调度的主要算法
 *
 * * 对于如下给定的一组磁盘访问进行调度
 * * 请求服务到达	A	B	C	D	E	F	G	H	I	J	K	L	M	N
 * * 访问的磁道号	30	50	100	180	20	90	150	70	80	10	160	120	40	110
 *
 * TODO: 要求分别采用先来先服务、最短寻道优先以及电梯调度算法进行调度。
 * TODO: 要求给出每种算法中磁盘访问的顺序，计算出平均移动道数。
 * TODO: 假定当前读写头在90号，电梯调度算法向磁道号增加的方向移动。
 *
 */

#define MAX 1000

class Disk_Scheduling {
    int tracks_num = 14;
    int tracks[MAX] = {30, 50, 100, 180, 20, 90, 150, 70, 80, 10, 160, 120, 40, 110};
    string tracks_name[MAX] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"};

    // find the average seek length
    double average(int *length, int n) {
        double sum = 0;
        for (int i = 0; i < n; i++)
            sum += length[i];
        return sum / n;
    }

    void output(int next_track[], string next_track_name[], int move_length[], int n) {
        cout << endl << "next_track track     " << "moving distance" << endl;
        for (int i = 0; i < n; i++)
            cout << "    " << next_track_name[i] + ": " << next_track[i] << "\t\t\t\t " << move_length[i] << endl;
        cout << "average seek move length: " << average(move_length, n) << endl << endl;
    }

// FCFS
    void first_come_first_server(int *tracks, int n, int now_track) {
        int i, k = 0;
        int t = n;
        int next_track[MAX], move_length[MAX];
        string next_track_name[MAX];
        bool is_visit[MAX] = {0};
        while (t--) {
            for (i = 0; i < n; i++) {
                if (is_visit[i])
                    continue;
                else {
                    move_length[k] = abs(tracks[i] - now_track);
                    next_track_name[k] = tracks_name[i];
                    next_track[k++] = tracks[i];
                    is_visit[i] = true;
                    now_track = tracks[i];
                }
            }
        }
        output(next_track, next_track_name, move_length, n);
    }

//SSTF
    void shortest_seek_time_first(int *tracks, int n, int now_track) {
        int i, min, k = 0;

        int t = n;
        int next_track[MAX], move_length[MAX];
        string next_track_name[MAX];
        bool is_visit[MAX] = {0};
        // index of the shortest path
        int flag;
        // seek n times
        while (t--) {
            min = INT_MAX;
            // find the nearest track among tracks that have not been visited
            for (i = 0; i < n; i++) {
                if (is_visit[i])
                    continue;
                if (abs(tracks[i] - now_track) < min) {
                    flag = i;
                    min = abs(tracks[i] - now_track);
                }
            }
            move_length[k] = min;
            next_track_name[k] = tracks_name[flag];
            next_track[k++] = tracks[flag];
            is_visit[flag] = true;
            now_track = tracks[flag];
        }
        output(next_track, next_track_name, move_length, n);
    }

    void scan(int *tracks, int n, int now) {
        int i, flag = 0, k = 0;
        int next_track[MAX], move_length[MAX];
        string next_track_name[MAX];
        sort(tracks, tracks + n);

        // find the start track
        for (i = 0; i < n; i++) {
            if (tracks[i] >= now) {
                flag = i;
                break;
            }
        }

        // move in the direction of increasing track number
        for (i = flag; i < n; i++) {
            next_track[k] = tracks[i];
            next_track_name[k] = tracks_name[i];
            move_length[k++] = abs(tracks[i] - now);
            now = tracks[i];
        }

        // back
        for (i = flag - 1; i >= 0; i--) {
            next_track[k] = tracks[i];
            next_track_name[k] = tracks_name[i];
            move_length[k++] = abs(tracks[i] - now);
            now = tracks[i];
        }

        output(next_track, next_track_name, move_length, n);
    }

public:
    void run() {
        cout << "total number of tracks to be used: " << tracks_num << endl;

        int i, flag = 1;

        cout << "track num: " << endl;
        for (i = 0; i < tracks_num; i++) {
            cout << tracks[i] << "    ";
        }
        // start track
        int start = 90;
        cout << endl << "start track: " << start << endl;

        //磁盘调度目录
        while (flag) {
            cout << "select one algorithm：" << endl;;
            cout << "1. FCFS" << endl;
            cout << "2. SSTF" << endl;
            cout << "3. Scan" << endl;
            cout << "0. exit" << endl;
            cin >> flag;
            switch (flag) {
                case 1:
                    first_come_first_server(tracks, tracks_num, start);
                    break;
                case 2:
                    shortest_seek_time_first(tracks, tracks_num, start);
                    break;
                case 3:
                    scan(tracks, tracks_num, start);
                    break;
                case 0:
                    break;
            }
        }
    }
};

#endif //TASK2_DISK_SCHEDULING_DISK_SCHEDULING_H
