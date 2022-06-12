#include <iostream>
#include "Process_Communication.h"

using namespace std;

int main() {
    int n = 1;
    while (n) {
        cout << "请选择通信方式：" << endl;
        cout << "1.通过管道" << endl
             << "2.通过消息队列" << endl
             << "3.通过共享内存" << endl
             << "0.退出" << endl;
        cout << "请输入：";
        cin >> n;

        if (n == 1) {
            pipe_con();
        } else if (n == 2) {
            msg_queen();
        } else if (n == 3) {
            shared_mem_comm();
        } else if (n == 0) {
            break;
        } else {
            cout << "输入有误，请重新输入：" << endl;
        }
    }

    return 0;
}
