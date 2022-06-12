#include <iomanip>
#include <iostream>

using namespace std;

enum class status { freeZone, usedZone };

class block {
   public:
    block() {}
    block(int _address, int _size, int _id, status _state, block* _pre,
          block* _next) {
        address = _address;
        size = _size;
        id = _id;
        state = _state;
        pre = _pre;
        next = _next;
    }
    int address;
    int size;
    int id;
    status state;  // 状态
    block* pre;    // 前向指针
    block* next;   // 后向指针
};

class distrabuteArea {
   public:
    distrabuteArea() {}
    block* head = new block();
};

distrabuteArea* blocks = new distrabuteArea();

void intdisArea() {
    blocks->head->pre = nullptr;
    blocks->head->next =
        new block(0, 512, -1, status::freeZone, nullptr, nullptr);
    blocks->head->next->pre = blocks->head;
}

bool firstFit(int id, int size) {
    block* p = blocks->head->next;
    while (p != nullptr) {
        //当前分配空间足够，分配大小为size的区间
        if (p->state == status::freeZone && p->size >= size) {
            block* node = new block(p->address + size, p->size - size, -1,
                                    status::freeZone, p, p->next);
            // 修改分区链节点指针
            if (p->next != nullptr) p->next->pre = node;

            p->next = node;
            // 分配空闲区间
            p->size = size;
            p->state = status::usedZone;
            p->id = id;

            return true;
        }
        p = p->next;
    }
    return false;
}

bool bestFit(int id, int size) {
    block* tar = nullptr;
    int tarSize = 512 + 1;
    block* p = blocks->head->next;
    // 寻找最佳空闲区间
    while (p != nullptr) {
        if (p->state == status::freeZone && p->size >= size &&
            p->size < tarSize) {
            tar = p;
            tarSize = p->size;
        }
        p = p->next;
    }

    // 找到要分配的空闲分区，分配大小为size的区间
    if (tar != nullptr) {
        block* node = new block(tar->address + size, tar->size - size, -1,
                                status::freeZone, tar, tar->next);
        if (tar->next != nullptr) tar->next->pre = node;
        // 分配空闲区间
        tar->next = node;
        tar->size = size;
        tar->state = status::usedZone;
        tar->id = id;
        return true;
    } else {
        return false;
    }
}

int worstFit(int id, int size) {
    block* tar = nullptr;
    int tarSize = 512 + 1;
    block* p = blocks->head->next;
    int tempsize = 0;
    // 寻找最大空闲区间
    while (p != nullptr) {
        
        if (p->state == status::freeZone && p->size >= tempsize) {
            tar = p;
            tarSize = p->size;
            tempsize = p->size;
        }
        p = p->next;
    }

    // 找到要分配的空闲分区
    if (tar != nullptr && tarSize >= size) {
        block* node = new block(tar->address + size, tar->size - size, -1,
                                status::freeZone, tar, tar->next);
        if (tar->next != nullptr) tar->next->pre = node;
        // 分配空闲区间
        tar->next = node;
        tar->size = size;
        tar->state = status::usedZone;
        tar->id = id;
        return true;
    } else {
        return false;
    }
}

int release(int id) {
    bool isFound = false;
    bool up = false;
    bool down = false;
    block *p = blocks->head->next, *focus;
    while (p != nullptr) {
        //是否找到已使用的id号
        if (p->state == status::usedZone && p->id == id) {
            isFound = true;

            // 上面是空闲分区
            if (p->pre != blocks->head && p->pre->state == status::freeZone) {
                up = true;
                focus = p;
                p = p->pre;
                p->size = p->size + focus->size;
                p->next = focus->next;
                if (focus->next != nullptr) focus->next->pre = p;
                free(focus);
            }

            // 下面是空闲分区
            if (p->next != nullptr && p->next->state == status::freeZone) {
                down = true;
                focus = p->next;
                p->size = p->size + focus->size;
                p->state = status::freeZone;
                p->id = -1;
                p->next = focus->next;
                if (focus->next != nullptr) focus->next->pre = p;
                free(focus);
            }

            if (!up && !down) {  // 上下都是占用分区
                p->state = status::freeZone;
                p->id = -1;
            }
        }
        p = p->next;
    }
    return isFound;
}

void showMemory() {
    cout << "当前的内存分配情况如下：" << endl;
    cout << " 地址  "
         << " 大小   "
         << " 状态 "
         << " 作业号  " << endl;
    block* p = blocks->head->next;
    while (p != nullptr) {
        cout << setw(4) << 512-p->address << setw(8) << p->size << setw(8);
        if (p->state == status::freeZone)
            cout << "空闲";
        else
            cout << "占用";
        cout << setw(5) << p->id << endl;
        p = p->next;
    }
}

void showMenu() {
    cout << "请选择要采用的放置策略" << endl;
    cout << "1.首次适应算法" << endl;
    cout << "2.最佳适应算法" << endl;
    cout << "3.最坏适应算法" << endl;
}

void showAction() {
    cout << "请输入要进行的操作" << endl;
    cout << "1. 申请分区\n2. 释放分区\n0. 退出" << endl;
}

void showExcuFailed() {
    cout << "Execution Failed！" << endl;
}

void dynamicPartition() {
    int selection, option;
    int id, size = 0;
    intdisArea();
    showMenu();
    cin >> selection;
    while (true) {
        showAction();
        cin >> option;
        if (option == 0) break;
        if (option == 1) {
            cout << "请输入作业号和申请的内存大小：" << endl;
            cin >> id >> size;
            size++;
            if (selection == 1 && !firstFit(id, size))showExcuFailed();
            if (selection == 2 && !bestFit(id, size)) showExcuFailed();
            if (selection == 3 && !worstFit(id, size))showExcuFailed();
        }
        if (option == 2) {
            cout << "请输入要回收的作业号：" << endl;
            cin >> id;
            release(id);
        }
        showMemory();
    }
}
