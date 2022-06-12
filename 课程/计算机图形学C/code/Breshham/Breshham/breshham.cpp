#define NDEBUG
#include <GL/glut.h>

#include <iostream>

void MidBresehamLine(int x0, int y0, int x1, int y1) {
    int dx, dy, d, UpIncre, DownIncre, x, y;
    if (x0 > x1) {  //调整点0和点1的位置
        x = x1, x1 = x0, x0 = x;
        y = y1, y1 = y0, y0 = y;
    }
    x = x0;
    y = y0;        //从点0开始模拟直线
    dx = x1 - x0;  // x轴增量
    dy = y1 - y0;  // y轴增量

    if (abs(dy) > abs(dx)) {  //判断斜率的绝对值是否大于1
        //扩大2*dy倍防止出现浮点数
        d = dy - 2 * dx;            //初始位置：d0=2*dy*(0.5-(1/k))
        UpIncre = 2 * dy - 2 * dx;  //向右增量: UpIncre=2*dy*(1-(1/k))
        DownIncre = 2 * dx;         //向左增量: DownIncre=2*dy*(1/k)
        while (y <= y1) {           //小于等于y1时
            std::cout << "  x: " << x << "  y: " << y
                      << std::endl;  //输出该点像素位置
            y++;                     // y坐标增加1
            if (d < 0) {  //如果d<0，即中点位置在直线下方，则选取（x+1,y+1）
                          //同时更新d
                x++;
                d += UpIncre;
            } else {  //如果d>0，即中点位置在直线上方，则选取（x,y+1） 同时更新d
                d -= DownIncre;
            }
        }
    } else {
        //扩大2*dx倍防止出现浮点数
        d = dx - 2 * dy;            //初始位置：d=2*dx*（0.5-2k）
        UpIncre = 2 * dx - 2 * dy;  //向上增量: UpIncre=2*dx*(1-k)
        DownIncre = 2 * dy;         //向下增量: DownIncre=2*dx*k

        while (x <= x1) {  //小于等于x1时
            std::cout << "  x: " << x << "  y: " << y
                      << std::endl;  //输出该点像素位置
            x++;                     // x坐标增加1
            if (d < 0) {  //如果d<0，即中点位置在直线下方，则选取（x+1,y+1）
                          //同时更新d
                y++;
                d += UpIncre;
            } else {  //如果d>0，即中点位置在直线上方，则选取（x+1,y） 同时更新d
                d -= DownIncre;
            }
        }
    }
}

void MidBresehamCircle() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPointSize(1.0);
    glBegin(GL_POINTS);

    int r = 160;
    int x, y, d;
    x = 0;
    y = r;
    d = 1 - r;

    while (x <= y) {
        GLfloat X = x / 200.0;
        GLfloat Y = y / 200.0;

        glVertex2f(X, Y);
        glVertex2f(Y, X);
        glVertex2f(-X, Y);
        glVertex2f(-Y, X);
        glVertex2f(X, -Y);
        glVertex2f(Y, -X);
        glVertex2f(-X, -Y);
        glVertex2f(-Y, -X);

        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    glEnd();
    glFlush();
    glutSwapBuffers();
}

void MidBresehamEllipse() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPointSize(1.0);
    glBegin(GL_POINTS);

    int a = 160;
    int b = 120;

    int x, y;
    float d1, d2;
    x = 0;
    y = b;
    d1 = b * b + a * a * (-b + 0.25);

    GLfloat X = x / 200.0;
    GLfloat Y = y / 200.0;
    glVertex2f(X, Y);
    glVertex2f(-X, Y);
    glVertex2f(X, -Y);
    glVertex2f(-X, -Y);

    while (b * b * (x + 1) < a * a * (y - 0.5)) {
        if (d1 <= 0) {
            d1 += b * b * (2 * x + 3);
            x++;
        } else {
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            x++;
            y--;
        }
        GLfloat X = x / 200.0;
        GLfloat Y = y / 200.0;
        glVertex2f(X, Y);
        glVertex2f(-X, Y);
        glVertex2f(X, -Y);
        glVertex2f(-X, -Y);
    }
    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) -
         a * a * b * b;
    while (y > 0) {
        if (d2 <= 0) {
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
            y--;
        } else {
            d2 += a * a * (-2 * y + 3);
            y--;
        }
        GLfloat X = x / 200.0;
        GLfloat Y = y / 200.0;
        glVertex2f(X, Y);
        glVertex2f(-X, Y);
        glVertex2f(X, -Y);
        glVertex2f(-X, -Y);
    }

    glEnd();
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    // MidBresehamLine(0, 0, 8, 5);
    // MidBresehamLine(0, 0, 5, 8);
     MidBresehamLine(0, 0, -8, -5);
    // MidBresehamLine(0, 0, -5, -8);

    glutInit(&argc, argv);  //初始化
    //设置显示方式：RGB颜色，单缓冲
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(200, 200);     //设置在窗口的中心位置
    glutInitWindowSize(400, 400);         //设置窗口大小
    glutCreateWindow("圆");               //创建窗口
    glutDisplayFunc(MidBresehamCircle);   //设置一个函数
    glutCreateWindow("椭圆");             //创建窗口
    glutDisplayFunc(MidBresehamEllipse);  //设置一个函数
    glutMainLoop();                       //进行消息循环

    return 0;
}