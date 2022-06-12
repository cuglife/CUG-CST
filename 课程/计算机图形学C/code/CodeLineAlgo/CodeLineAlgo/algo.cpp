#include <conio.h>
#include <graphics.h>

#include <iostream>
#define left 1
#define right 2
#define bottom 4
#define top 8
using namespace std;

//设置裁剪框的大小和位置
double xl = 240, xr = 480, yt = 480, yb = 240;

//编码
void encode(double x, double y, int& code) {
    int c = 0;
    if (x < xl)
        c = c + left;
    else if (x > xr)
        c = c + right;

    if (y < yb)
        c = c + bottom;
    else if (y > yt)
        c = c + top;

    code = c;
}

// Cohen-Sutherland裁剪算法
int CS_LineClip(double x1, double y1, double x2, double y2) {
    int code1, code2, code;
    int x, y;  // tmp
    encode(x1, y1, code1);
    encode(x2, y2, code2);
    //都在里面时，直接画线，否则进行判断
    while (code1 != 0 || code2 != 0) {
        int ttt = (code1 & code2);
        if ((code1 & code2) != 0) return 0;  //都在外面舍弃
        code = code1;
        if (code1 == 0) code = code2;
        if ((left & code) != 0) {
            x = xl;
            y = y1 + (y2 - y1) * (xl - x1) / (x2 - x1);
        } else if ((right & code) != 0) {
            x = xr;
            y = y1 + (y2 - y1) * (xr - x1) / (x2 - x1);
        } else if ((bottom & code) != 0) {
            y = yb;
            x = x1 + (x2 - x1) * (yb - y1) / (y2 - y1);
        } else if ((top & code) != 0) {
            y = yt;
            x = x1 + (x2 - x1) * (yt - y1) / (y2 - y1);
        }
        if (code == code1) {
            x1 = x;
            y1 = y;
            encode(x, y, code1);
        } else {
            x2 = x;
            y2 = y;
            encode(x, y, code2);
        }
    }
    x1;
    y1;
    x2;
    y2;
    setcolor(BLUE);
    line(x1, y1, x2, y2);
    return 0;
}

int main() {
    double x1, y1, x2, y2;

    initgraph(720, 720);
    setbkcolor(0xFFFFFF);
    cleardevice();
    setcolor(0x8B0000);
    POINT points[] = {{xl, yt}, {xr, yt}, {xr, yb}, {xl, yb}};
    polygon(points, 4);
    //用鼠标来获取两个点的坐标
    MOUSEMSG m;  // 定义结构体保存鼠标消息
    while (true) {
    flag:
        m = GetMouseMsg();  // 获取一次鼠标消息
        switch (m.uMsg) {
            case WM_LBUTTONDOWN:
                x1 = m.x;
                y1 = m.y;  //鼠标第一次按下时，获取鼠标当前坐标
                while (true) {
                    m = GetMouseMsg();  // 再次获取一条鼠标消息
                    switch (m.uMsg) {
                        case WM_LBUTTONDOWN:
                            x2 = m.x;
                            y2 = m.y;  //鼠标第二次按下时,得到坐标
                            setcolor(RED);
                            line(x1, y1, x2, y2);
                            CS_LineClip(x1, y1, x2, y2);
                            goto flag;
                    }
                }
        }
    }
    _getch();
    closegraph();
    return 0;
}