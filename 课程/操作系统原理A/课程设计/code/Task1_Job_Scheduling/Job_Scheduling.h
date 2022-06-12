//
// Created by az on 2020/12/07.
//

#ifndef TASK1_JOBSCHEDULING_JOB_SCHEDULING_H
#define TASK1_JOBSCHEDULING_JOB_SCHEDULING_H

#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

/**
 * @author oaeen
 * * 假设系统中可同时运行 *两道* 作业，给出每道作业的到达时间和运行时间
 * * 作业名	A	B	C	D	E	F	G	H	I	J
 * * 到达时间	0	2	5	7	12	15	4	6	8	10
 * * 运行时间	7	10	20	30	40	8	8	20	10	12
 *
 * * 预备知识
 * * 响应比=等待时间/运行时间+1
 * * 周转时间=完成时间-到达时间
 * * 带权周转时间=周转时间/运行时间
 *
 * TODO: 分别用先来先服务算法、短作业优先和响应比高者优先三种算法给出作业的调度顺序
 * TODO: 计算每一种算法的平均周转时间及平均带权周转时间并比较不同算法的优劣
 *
 */

class Process {
public:
    Process(string _processName, int _comeTime, int _runTime) {
        processName = std::move(_processName);
        comeTime = _comeTime;
        runTime = _runTime;
        current_time = 0;
    }

    string processName;
    int comeTime;
    int runTime;
    int current_time;
};

class Output {
public:
    void static printResult(vector<string> result) {
        int processNum = result.size();
        for (int i = 0; i < processNum; i++)
            cout << result.at(i) << " ";
        cout << endl;
    }

    void static output(double *turnaround_time, double *weighted_turnaround_time, int lenth) {
        double sum_turnaround_time = 0;
        double sum_weighted_turnaround_time = 0;
        for (int i = 0; i < lenth; i++) {
            sum_turnaround_time += turnaround_time[i];
            sum_weighted_turnaround_time += weighted_turnaround_time[i];
        }
        cout << "average turnaround time: " << sum_turnaround_time / lenth << endl;
        cout << "average weighted turnaround time: " << sum_weighted_turnaround_time / lenth << endl;
    }
};

// FCFS - First Come First Serve
class FCFS {
    bool static CometimeCmp(Process &p1, Process &p2) {
        return p1.comeTime < p2.comeTime;
    }

public:
    void static exec(vector<Process> processes) {

        vector<string> result;
        int processNum = processes.size();
        sort(processes.begin(), processes.end(), CometimeCmp);
        for (int i = 0; i < processNum; i++)
            result.push_back(processes.at(i).processName);

        cout << "Scheduling sequence in FCFS: " << endl;
        Output::printResult(result);

        Process task_empty = Process("null", 999999, -1);
        processes.push_back(task_empty);



        // processing
        // 响应比
        // 周转时间
        // 带权周转时间
        int current_time = 0;
        int wait_time = 0;
        int cur_point = 0;
        int time_point = 0;
        double turnaround_time[10] = {};
        double weighted_turnaround_time[10] = {};
        //double *turnaround_time = new double[processNum];
        //double *weighted_turnaround_time = new double[processNum];
        Process task1 = processes.at(cur_point);
        Process task2 = task_empty;
        int task1_runtime = task1.runTime;
        int task2_runtime = task2.runTime;
        while (true) {

//            cout << "current run task: " << endl
//                 << "process " << task1.processName << "  remain time: " << task1.runTime << endl
//                 << "process " << task2.processName << "  remain time: " << task2.runTime << endl;

            current_time++;
            wait_time++;
            task1.runTime--;
            task2.runTime--;

            if (task1.runTime <= 0) {
                if (task1.runTime == 0) {
                    turnaround_time[time_point] = current_time - task1.comeTime;
                    weighted_turnaround_time[time_point] = turnaround_time[time_point] / task1_runtime;
                    cout << "process " << task1.processName << " executed at: " << current_time << endl << endl;
                    time_point++;
                }
                if (current_time >= processes.at(cur_point + 1).comeTime) {
                    task1 = processes.at(++cur_point);
                    task1_runtime = task1.runTime;
                    wait_time = 0;
                }
            }

            if (task2.runTime <= 0) {
                if (task2.runTime == 0) {
                    turnaround_time[time_point] = current_time - task2.comeTime;
                    weighted_turnaround_time[time_point] = turnaround_time[time_point] / task2_runtime;
                    cout << "process " << task2.processName << " executed at: " << current_time << endl << endl;
                    time_point++;
                }
                if (current_time >= processes.at(cur_point + 1).comeTime) {
                    task2 = processes.at(++cur_point);
                    task2_runtime = task2.runTime;
                    wait_time = 0;
                }
            }

            if (wait_time > 50) {
                break;
            }
        }

        Output::output(turnaround_time, weighted_turnaround_time, processNum);

        // cout << "FCFS runtime: " << current_time << endl;
    }
};

// SJF - Shortest Job First
class SJF {
    bool static cometime_Cmp(Process &p1, Process &p2) {
        return p1.comeTime < p2.comeTime;
    }

    bool static runtime_cmp(Process &p1, Process &p2) {
        return p1.runTime < p2.runTime;
    }

    void static swap(Process &p1, Process &p2) {
        Process tmp = p1;
        p1 = p2;
        p2 = tmp;
    }

    int static get_run_time(vector<Process> &processes, string process_name) {
        vector<Process>::iterator it;
        for (it = processes.begin(); it != processes.end(); ++it) {
            if (it->processName == process_name) {
                return it->runTime;
            }
        }
        return -1;
    }

    bool static eraseProcess(vector<Process> &processes_pool, Process &task) {
        vector<Process>::iterator it;
        for (it = processes_pool.begin(); it != processes_pool.end(); ++it) {
            if (it->processName == task.processName) {
                processes_pool.erase(it);
                return true;
            }
        }
        return false;
    }

public:
    static void exec(vector<Process> processes) {
        int processNum = processes.size();
        sort(processes.begin(), processes.end(), cometime_Cmp);

        // take a backup to show algo performance
        vector<Process> backup_processes = processes;

        vector<Process> processes_pool;
        Process task_empty = Process("null", 999999, -1);
        processes.push_back(task_empty);

        // processing
        // 响应比
        // 周转时间
        // 带权周转时间
        int current_time = 0;
        int wait_time = 0;
        double turnaround_time[10] = {};
        double weighted_turnaround_time[10] = {};
        //double *turnaround_time = new double[processNum];
        //double *weighted_turnaround_time = new double[processNum];
        int cur_point = 0;
        int time_point = 0;
        Process task1 = processes.at(cur_point);
        Process task2 = task_empty;

        while (true) {

//            cout << "current run task: " << endl
//                 << "process " << task1.processName << "  remain time: " << task1.runTime << endl
//                 << "process " << task2.processName << "  remain time: " << task2.runTime << endl;

            current_time++;
            wait_time++;
            task1.runTime--;
            task2.runTime--;

            // update process pool
            while (current_time >= processes.at(cur_point + 1).comeTime) {
                processes_pool.push_back(processes.at(++cur_point));
            }

            sort(processes_pool.begin(), processes_pool.end(), runtime_cmp);


            if (task1.runTime == 0) {
                turnaround_time[time_point] = current_time - task1.comeTime;
                weighted_turnaround_time[time_point] =
                        turnaround_time[time_point] / get_run_time(backup_processes, task1.processName);
                cout << "process " << task1.processName << " executed at: " << current_time << endl << endl;
                time_point++;
                // modify to prevent output more than one time
                task1.runTime = -1;
                wait_time = 0;
            }

            if (task2.runTime == 0) {
                turnaround_time[time_point] = current_time - task2.comeTime;
                weighted_turnaround_time[time_point] =
                        turnaround_time[time_point] / get_run_time(backup_processes, task2.processName);
                cout << "process " << task2.processName << " executed at: " << current_time << endl << endl;
                time_point++;
                // modify to prevent output more than one time
                task2.runTime = -1;
                wait_time = 0;
            }

            if (!processes_pool.empty()) {
                if (task1.runTime <= 0) {
                    task1 = processes_pool.at(0);
                    processes_pool.erase(processes_pool.begin());
                    wait_time = 0;
                } else if (task1.runTime > processes_pool.at(0).runTime) {
                    swap(task1, processes_pool.at(0));
                    wait_time = 0;
                }
            }
            if (!processes_pool.empty()) {
                if (task2.runTime <= 0) {
                    task2 = processes_pool.at(0);
                    processes_pool.erase(processes_pool.begin());
                    wait_time = 0;
                } else if (task2.runTime > processes_pool.at(0).runTime) {
                    swap(task2, processes_pool.at(0));
                    wait_time = 0;
                }
            }

            if (wait_time > 50) {
                break;
            }
        }

        Output::output(turnaround_time, weighted_turnaround_time, processNum);
    }
};

// HRRN - Highest Response Ratio Next
class HRRN {
    bool static cometime_Cmp(Process &p1, Process &p2) {
        return p1.comeTime < p2.comeTime;
    }

    bool static response_cmp(Process &p1, Process &p2) {
        return ((p1.current_time - p1.comeTime) / (p1.runTime * 1.0)) >
               ((p2.current_time - p2.comeTime) / (p2.runTime * 1.0));
    }

    void static update_current_time(vector<Process> &processes, int current_time) {
        for (auto iter = processes.begin(); iter != processes.end(); iter++) {
            iter->current_time = current_time;
        }
    }

    void static swap(Process &p1, Process &p2) {
        Process tmp = p1;
        p1 = p2;
        p2 = tmp;
    }

    int static get_run_time(vector<Process> &processes, string process_name) {
        vector<Process>::iterator it;
        for (it = processes.begin(); it != processes.end(); ++it) {
            if (it->processName == process_name) {
                return it->runTime;
            }
        }
        return -1;
    }

public:
    void static exec(vector<Process> processes) {
        cout << "Scheduling sequence in HRRN: " << endl;
        int processNum = processes.size();
        sort(processes.begin(), processes.end(), cometime_Cmp);

        // take a backup to show algo performance
        vector<Process> backup_processes = processes;

        vector<Process> processes_pool;
        Process task_empty = Process("null", 999999, -1);
        processes.push_back(task_empty);

        // processing
        // 响应比
        // 周转时间
        // 带权周转时间
        int current_time = 0;
        int wait_time = 0;
        double turnaround_time[10] = {};
        double weighted_turnaround_time[10] = {};
        //double *turnaround_time = new double[processNum];
        //double *weighted_turnaround_time = new double[processNum];
        int cur_point = 0;
        int time_point = 0;
        Process task1 = processes.at(cur_point);
        Process task2 = task_empty;

        while (true) {

//            cout << "current run task: " << endl
//                 << "process " << task1.processName << "  remain time: " << task1.runTime << endl
//                 << "process " << task2.processName << "  remain time: " << task2.runTime << endl;

            current_time++;
            wait_time++;
            task1.runTime--;
            task2.runTime--;

            // update process pool
            while (current_time >= processes.at(cur_point + 1).comeTime) {
                processes_pool.push_back(processes.at(++cur_point));
            }

            update_current_time(processes_pool, current_time);

            sort(processes_pool.begin(), processes_pool.end(), response_cmp);


            if (task1.runTime == 0) {
                turnaround_time[time_point] = current_time - task1.comeTime;
                weighted_turnaround_time[time_point] =
                        turnaround_time[time_point] / get_run_time(backup_processes, task1.processName);
                cout << "process " << task1.processName << " executed at: " << current_time << endl << endl;
                time_point++;
                // modify to prevent output more than one time
                task1.runTime = -1;
                wait_time = 0;
            }

            if (task2.runTime == 0) {
                turnaround_time[time_point] = current_time - task2.comeTime;
                weighted_turnaround_time[time_point] =
                        turnaround_time[time_point] / get_run_time(backup_processes, task2.processName);
                cout << "process " << task2.processName << " executed at: " << current_time << endl << endl;
                time_point++;
                // modify to prevent output more than one time
                task2.runTime = -1;
                wait_time = 0;
            }

            double task1_response = (current_time - task1.comeTime) / (task1.runTime * 1.0);
            double task2_response = (current_time - task2.comeTime) / (task2.runTime * 1.0);
            if (!processes_pool.empty()) {
                double pool_response = (current_time - processes_pool.at(0).comeTime) /
                                       (processes_pool.at(0).runTime * 1.0);
                if (task1.runTime <= 0) {
                    task1 = processes_pool.at(0);
                    processes_pool.erase(processes_pool.begin());
                    wait_time = 0;
                } else if (task1_response < pool_response) {
                    swap(task1, processes_pool.at(0));
                    wait_time = 0;
                }
            }
            if (!processes_pool.empty()) {
                double pool_response = (current_time - processes_pool.at(0).comeTime) /
                                       (processes_pool.at(0).runTime * 1.0);
                if (task2.runTime <= 0) {
                    task2 = processes_pool.at(0);
                    processes_pool.erase(processes_pool.begin());
                    wait_time = 0;
                } else if (task2_response < pool_response) {
                    swap(task2, processes_pool.at(0));
                    wait_time = 0;
                }
            }

            if (wait_time > 50) {
                break;
            }
        }

        Output::output(turnaround_time, weighted_turnaround_time, processNum);
    }
};

class Job_Scheduling {
    static vector<Process> getProcesses() {
        string processName[10] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
        int comeTime[10] = {0, 2, 5, 7, 12, 15, 4, 6, 8, 10};
        int runTime[10] = {7, 10, 20, 30, 40, 8, 8, 20, 10, 12};

        vector<Process> processes;
        for (int i = 0; i < 10; i++)
            processes.emplace_back(processName[i], comeTime[i], runTime[i]);
        return processes;
    }

public:
    static void run() {
        // init processes
        const vector<Process> processes = getProcesses();
        int flag = 1;
        while (flag) {
            cout << "input a number to choose algorithm: " << endl
                 << "1. FCFS" << endl
                 << "2. SJF" << endl
                 << "3. HRRN" << endl
                 << "0. exit" << endl;
            cin >> flag;
            // output the result
            switch (flag) {
                case 1:
                    FCFS::exec(processes);
                    break;
                case 2:
                    SJF::exec(processes);
                    break;
                case 3:
                    HRRN::exec(processes);
                    break;
                case 0:
                    break;
                default:
                    cout << "input error, please input again!" << endl;
                    break;
            }
        }
    }
};


#endif //TASK1_JOBSCHEDULING_JOB_SCHEDULING_H
