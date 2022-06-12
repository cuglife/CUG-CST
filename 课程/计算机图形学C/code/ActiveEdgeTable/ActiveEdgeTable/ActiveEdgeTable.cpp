#include <Windows.h>
#include <gl/glut.h>
#include <malloc.h>
#include <stdio.h>

#include <iostream>
#define EPSILON 0.000001  // 最小浮点数

// 点结构体
struct Point {
    int x;  // x 坐标
    int y;  // y 坐标
};
// 线结构体
struct Line {
    Point high_point;  // 高端点
    Point low_point;   // 低端点
    int is_active;  // 是否为有效边，水平边（ 0 ），非水平边（ 1 ）
    double inverse_k;  // 斜率 k 的倒数
};
// 边结点
struct EdgeNode {
    double x;  // 扫描线与边交点的 x 坐标（边的低端点的 x 坐标）
    int y_max;         // 边的高端点的 y 坐标 ymax
    double inverse_k;  // 斜率 k 的倒数
    EdgeNode* next;    // 下一个边结点的指针
};
// 有效边表
struct ActiveEdgeTable {
    int y;           // 扫描线 y
    EdgeNode* head;  // 边链表的头指针
};
// 桶结点
typedef struct Bucket {
    int y;           // 扫描线 y
    EdgeNode* head;  // 边链表的头指针
    Bucket* next;    // 下一个桶的指针
} EdgeTable;
int compare(Point p1, Point p2);
Line* create_lines(Point points[], int n);
Point get_lowest_point(Line lines[], int n);
Point get_highest_point(Line lines[], int n);
void swap(Line& l1, Line& l2);
void sort(Line lines[], int n);
EdgeTable* create_edge_table(Line lines[], int n);
ActiveEdgeTable* init_active_table(EdgeTable* edge_table);
void delete_edge(ActiveEdgeTable* active_table, int y_max);
void add_edge(ActiveEdgeTable* active_table, EdgeNode edge);
ActiveEdgeTable* update_active_table(ActiveEdgeTable* active_table,
                                     EdgeTable* edge_table);
void DrawPolygon(Point points, int n);
void DrawGrid(int x, int y);
void Fill(Point points[], int n);
void Initial();
void Display();
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 120);
    glutCreateWindow("Polygon Filling");
    glutDisplayFunc(Display);
    Initial();
    glutMainLoop();
    return 0;
}
// 比较 2 个点的高度
int compare(Point p1, Point p2) {
    if (p1.y > p2.y)
        return 1;
    else if (p1.y == p2.y)
        return 0;
    return -1;
}
// 由点数组生成线段数组
Line* create_lines(Point points[], int n) {
    Line* lines = (Line*)malloc(n * sizeof(Line));
    for (int i = 0; i < n; ++i) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];
        int result = compare(p1, p2);
        if (result == 0)
            lines[i].is_active = 0;
        else
            lines[i].is_active = 1;
        lines[i].high_point = result > 0 ? p1 : p2;
        lines[i].low_point = result < 0 ? p1 : p2;
        lines[i].inverse_k = (double)(p2.x - p1.x) / (double)(p2.y - p1.y);
    }
    return lines;
}
// 获取线数组中最低的端点
Point get_lowest_point(Line lines[], int n) {
    Point lowest_point = lines[0].low_point;
    for (int i = 1; i < n; ++i) {
        Point low_point = lines[i].low_point;
        if (compare(lowest_point, low_point) > 0) lowest_point = low_point;
    }
    return lowest_point;
}
// 获取线数组中最高的端点
Point get_highest_point(Line lines[], int n) {
    Point highest_point = lines[0].high_point;
    for (int i = 1; i < n; ++i) {
        Point high_point = lines[i].high_point;
        if (compare(highest_point, high_point) < 0) highest_point = high_point;
    }
    return highest_point;
}
// 交换 2 个 Line 对象
void swap(Line& l1, Line& l2) {
    Line temp = l1;
    l1 = l2;
    l2 = temp;
}
// 对线数组进行排序
void sort(Line lines[], int n) {
    // 先按低端点的 y 坐标进行升序排序
    for (int i = 0; i < n; ++i) {
        int min_index = i;
        for (int j = i + 1; j < n; ++j) {
            if (lines[j].low_point.y < lines[min_index].low_point.y)
                min_index = j;
        }
        swap(lines[i], lines[min_index]);
    }
    // 再将有序数组按低端点的 x 坐标升序排列，若 x 坐标相等，按 inverse_k 升序
    for (int i = 0; i < n; ++i) {
        int min_index = i;
        for (int j = i + 1; lines[j].low_point.y == lines[i].low_point.y; ++j) {
            if (lines[j].low_point.x < lines[min_index].low_point.x)
                min_index = j;
        }
        swap(lines[i], lines[min_index]);
        if (i > 0 && lines[i].low_point.x == lines[i - 1].low_point.x) {
            if (lines[i].is_active == 1 && lines[i - 1].is_active == 1) {
                if (lines[i].inverse_k < lines[i - 1].inverse_k)
                    swap(lines[i], lines[i - 1]);
            }
        }
    }
}
// 创建一个边表
EdgeTable* create_edge_table(Line lines[], int n) {
    EdgeTable* edge_table = (EdgeTable*)malloc(sizeof(EdgeTable));
    edge_table->head = NULL;
    edge_table->next = NULL;
    sort(lines, n);
    Point lowest_point = get_lowest_point(lines, n);
    Point highest_point = get_highest_point(lines, n);
    EdgeTable* s = edge_table;
    for (int i = lowest_point.y; i <= highest_point.y; ++i) {
        Bucket* bucket = (Bucket*)malloc(sizeof(Bucket));
        bucket->y = i;
        bucket->next = NULL;
        bucket->head = (EdgeNode*)malloc(sizeof(EdgeNode));
        bucket->head->next = NULL;
        EdgeNode* p = bucket->head;
        for (int j = 0; j < n; ++j) {
            if (lines[j].is_active == 0) continue;
            if (lines[j].low_point.y == i) {
                EdgeNode* q = (EdgeNode*)malloc(sizeof(EdgeNode));
                q->x = lines[j].low_point.x;
                q->y_max = lines[j].high_point.y;
                q->inverse_k = lines[j].inverse_k;
                q->next = NULL;
                p->next = q;
                p = q;
            }
        }
        s->next = bucket;
        s = bucket;
    }
    return edge_table;
}
// 从边表中取出第一个不为空的桶初始化有效边表
ActiveEdgeTable* init_active_table(EdgeTable* edge_table) {
    ActiveEdgeTable* active_table =
        (ActiveEdgeTable*)malloc(sizeof(ActiveEdgeTable));
    active_table->y = edge_table->next->y;
    active_table->head = (EdgeNode*)malloc(sizeof(EdgeNode));
    active_table->head->next = NULL;
    EdgeNode* p = edge_table->next->head;
    EdgeNode* q = active_table->head;
    while (p->next != NULL) {
        EdgeNode* s = (EdgeNode*)malloc(sizeof(EdgeNode));
        s->x = p->next->x;
        s->y_max = p->next->y_max;
        s->inverse_k = p->next->inverse_k;
        s->next = NULL;
        q->next = s;
        q = s;
        p = p->next;
    }
    return active_table;
}
// 从有效边表中删除指定 y_max 的边结点
void delete_edge(ActiveEdgeTable* active_table, int y_max) {
    EdgeNode* p = active_table->head;
    while (p->next != NULL) {
        EdgeNode* q = p->next;
        if (q->y_max == y_max) {
            p->next = q->next;
            free(q);
        } else
            p = p->next;
    }
}
// 将一个边结点按次序添加到有效边表中
void add_edge(ActiveEdgeTable* active_table, EdgeNode edge) {
    EdgeNode* t = (EdgeNode*)malloc(sizeof(EdgeNode));
    t->x = edge.x;
    t->y_max = edge.y_max;
    t->inverse_k = edge.inverse_k;
    t->next = NULL;
    EdgeNode* p = active_table->head;
    while (p->next != NULL) {
        EdgeNode* q = p->next;
        if ((edge.x < q->x) ||
            (edge.x == q->x && edge.inverse_k < q->inverse_k)) {
            p->next = t;
            t->next = q;
            return;
        }
        p = p->next;
    }
    p->next = t;
}
// 更新有效边表，并与边表中对应的桶合并
ActiveEdgeTable* update_active_table(ActiveEdgeTable* active_table,
                                     EdgeTable* edge_table) {
    // 更新扫描线 y
    ++active_table->y;
    // 删除 y=ymax 的边
    delete_edge(active_table, active_table->y);

    // 更新边结点的数据
    EdgeNode* p = active_table->head->next;
    while (p != NULL) {
        p->x += p->inverse_k;
        p = p->next;
    }
    // 找到边表中对应的桶
    EdgeTable* q = edge_table;
    while ((q = q->next) != NULL && q->y != active_table->y)
        ;
    // 如果找到，则进行合并
    if (q != NULL) {
        EdgeNode* s = q->head;
        while ((s = s->next) != NULL) {
            add_edge(active_table, *s);
        }
    }
    return active_table;
}
// 画出多边形的边框
void DrawPolygon(Point points[], int n) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; ++i) glVertex2i(points[i].x, points[i].y);
    glEnd();
}
// 画出 x * y 的网格
void DrawGrid(int x, int y) {
    glBegin(GL_LINES);
    // 横线
    for (int i = 0; i <= y; ++i) {
        glVertex2d(0, i);
        glVertex2d(x, i);
    }
    // 竖线
    for (int i = 0; i <= x; ++i) {
        glVertex2d(i, 0);
        glVertex2d(i, y);
    }
    glEnd();
}
// 用指定的像素大小填充多边形
void Fill(Point points[], int n) {
    Line* lines = create_lines(points, n);
    EdgeTable* edge_table = create_edge_table(lines, n);

    EdgeTable* edge_table_out = edge_table;
    std::cout << "EdgeTable:\n";
    while (edge_table_out != nullptr) {
        EdgeNode* node = edge_table_out->head;

        if (node != nullptr) {
            std::cout << "Y: " << edge_table_out->y << "\n";
        }

        while (node != nullptr) {
            if (node = node->next) {
                std::cout << "→";
                std::cout << node->x << "  " << node->y_max << "  "
                          << node->inverse_k << "\n";
            }
            // node = node->next;
        }
        edge_table_out = edge_table_out->next;
        std::cout << "\n\n";
    }

    ActiveEdgeTable* active_table = init_active_table(edge_table);

    while (active_table->head->next != NULL) {
        EdgeNode* p = active_table->head;
        int b = -1;
        while (p->next != NULL) {
            if (b > 0) {
                int left = p->x;
                int right = p->next->x;
                // 如果不是局部最低点，则进行边界处理
                if (!(p->x - p->next->x >= -EPSILON &&
                      p->x - p->next->x <= EPSILON)) {
                    // 处理左边界
                    if (!(p->x - left >= -EPSILON && p->x - left <= EPSILON))
                        left += 1;
                    // 处理右边界
                    if (p->next->x - right >= -EPSILON &&
                        p->next->x - right <= EPSILON)
                        right -= 1;
                }
                for (int i = left; i <= right; ++i) {
                    glBegin(GL_POINTS);
                    glVertex2d(i, active_table->y);
                    glEnd();
                    glFlush();
                    // Sleep(50);
                }
            }
            p = p->next;
            b = -b;
        }
        active_table = update_active_table(active_table, edge_table);

        ActiveEdgeTable* active_table_out = active_table;

        if (active_table_out->y == 4) {
            EdgeNode* anode = active_table_out->head;
            if (anode != nullptr) {
                std::cout << "Y=4时,ActiveEdgeTable:\n";
                while (anode->next != nullptr) {
                    anode = anode->next;
                    std::cout << "→x:" << anode->x << "  Ymax:" << anode->y_max
                              << "  1/k:" << anode->inverse_k << "\n";
                }
            }
        }
    }
}
// 初始化窗口， x 和 y 指定窗口的坐标大小
void Initial() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 20.0, 0.0, 15.0);
}
// 窗口的显示回调函数
void Display() {
    // 使用当前背景色填充窗口
    glClear(GL_COLOR_BUFFER_BIT);

    // 使用灰色画出网格线
    glColor3f(0.75f, 0.75f, 0.75f);
    DrawGrid(20, 14);
    glFlush();
    // 多边形的顶点坐标
    Point points[] = {{1, 1}, {7, 1}, {7, 5}, {6, 4}, {4, 6}, {3, 2}, {2, 3}};
    // 计算顶点个数
    int n = sizeof(points) / sizeof(Point);
    // 使用黑色画出多边形的边框
    glColor3f(0.0f, 0.0f, 0.0f);
    DrawPolygon(points, n);
    glFlush();
    // 指定点大小
    glPointSize(6.0f);
    // 使用红色填充多边形
    glColor3f(1.0f, 0.0f, 1.0f);
    Fill(points, n);
    glFlush();
}
