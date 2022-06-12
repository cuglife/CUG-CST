#pragma once
#include <cstring>
#include <iostream>

using namespace std;

//进程控制块
class PCB {
   public:
    PCB() {}
    PCB(string _name, int _need, int _turn) {
        name = _name;
        need = _need;  //需运行的时间
        turn = _turn;  //周转时间
        next = nullptr;
    }
    string name;
    int need;  //需运行的时间
    int turn;  //周转时间
    PCB* next;
};

class RQ {
   public:
    PCB* front = new PCB();
};

RQ* RQ1 = new RQ();
RQ* RQ2 = new RQ();

void readRQ() {
    string RQ1name[5] = {"p1", "p2", "p3", "p4", "p5"};
    int RQ1need[] = {16, 11, 14, 13, 15};  // RQ1运行时间
    int RQ1wait[] = {6, 5, 4, 3, 2};       // RQ1已等待时间

    string RQ2name[5] = {"p6", "p7", "p8", "p9", "p10"};
    int RQ2need[] = {21, 18, 10, 7, 14};  // RQ2运行时间
    int RQ2wait[] = {1, 2, 3, 4, 5};      // RQ2已等待时间

    PCB* p = RQ1->front;
    PCB* q = RQ2->front;
    for (int i = 0; i < 5; i++) {
        p->next = new PCB(RQ1name[i], RQ1need[i], RQ1wait[i]);
        q->next = new PCB(RQ2name[i], RQ2need[i], RQ2wait[i]);
        p = p->next;
        q = q->next;
    }
}

//排序用合并算法
PCB* merge(PCB* l1, PCB* l2) {
    PCB* l = new PCB();
    PCB* p = l;

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->need < l2->need) {
            p->next = l1;
            l1 = l1->next;
        } else {
            p->next = l2;
            l2 = l2->next;
        }
        p = p->next;
    }

    if (l1 != nullptr) p->next = l1;

    if (l2 != nullptr) p->next = l2;

    return l->next;
}

//排序
PCB* sortList(PCB* head) {
    if (head == nullptr || head->next == nullptr) return head;

    // step 1. cut the list to two halves
    PCB *prev = nullptr, *slow = head, *fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    prev->next = nullptr;

    PCB* l1 = sortList(head);
    PCB* l2 = sortList(slow);

    return merge(l1, l2);
}

void multipleFeedback() {
    readRQ();
    int clock = 0;

    const int timeSlice = 7;  //时间片
    PCB* p = RQ1->front;
    // RQ1 没有跑完
    while (p->next != nullptr) {
        while (p->next != nullptr) {
            if (p->next->need > timeSlice) {
                p->next->need -= timeSlice;
                clock += timeSlice;
                p = p->next;
            } else {
                clock += p->next->need;
                p->next->turn = clock + p->next->turn;
                cout << p->next->name << " time count: " << p->next->turn
                     << endl;
                p->next = p->next->next;
            }
        }
        p = RQ1->front;
    }

    // sort
    RQ2->front = sortList(RQ2->front);

    PCB* q = RQ2->front;
    // RQ2 没有跑完
    while (q->next != nullptr) {
        clock += q->next->need;
        q->next->turn += clock;
        cout << q->next->name << " time count: " << q->next->turn << endl;
        q = q->next;
    }
}