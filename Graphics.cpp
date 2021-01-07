#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <glut.h>
#include<stdlib.h>
#include<math.h>

int x1, y12;
int x2, y2;
int k = 0;
//交换功能
void swap(GLint& a, GLint& b)
{
    GLint t = a;
    a = b;
    b = t;
}

//DDA算法——digital differentialanalyzer，数字微分析法 
void LineDDA(int x0, int y0, int xEnd, int yEnd)
{
    int dx = xEnd - x0;                             //x的增量
    int dy = yEnd - y0;                             //y的增量
    int steps;
    float xIncrement, yIncrement, x = x0, y = y0;
    if (abs(dx) > abs(dy))                          //谁的增量大
    {
        steps = abs(dx);
    }
    else
    {
        steps = abs(dy);
    }

    xIncrement = float(dx) / float(steps);          //x每步骤增量
    yIncrement = float(dy) / float(steps);          //y的每步增量
;
    //开始画点
    glVertex2f(x0, y0);                          //起点要画下
    for (int k = 0; k <= steps; ++k)
    {
        x += xIncrement;                            //x点+增量
        y += yIncrement;                            //y点+增量
        glVertex2f(x, y);
    }
    glVertex2f(xEnd, yEnd);                      //终点画下
}

//中点画线法
void MidpointLine(int x0, int y0, int x1, int y1)
{
    int a, b, d1, d2, x = 0, y = 0; 
    float m;
    if (x1 < x0) 
    {
        swap(x0, x1);          
        swap(y0, y1);
    }
    a = y0 - y1; 
    b = x1 - x0;

    if (b == 0) 
    {
        m = -1 * a * 100;
    }
    else
    {
        m = (float)a / (x0 - x1); 
        x = x0;
        y = y0;
    }
    //画第一个点
    glVertex2f(x, y);
    int d = 0;
    if (m >= 0 && m <= 1)
    {
        d = 2 * a + b; 
        d1 = 2 * a; 
        d2 = 2 * (a + b);
        while (x < x1)
        {
            if (d <= 0)
            {
                x++;
                y++;
                d += d2;
            }
            else
            {
                x++; 
                d += d1;
            }
            glVertex2f(x, y);
        }
    }
    else if (m <= 0 && m >= -1)
    {
        d = 2 * a - b; 
        d1 = 2 * a - 2 * b; 
        d2 = 2 * a;
        while (x < x1)
        {
            if (d > 0)
            {
                x++;
                y--;
                d += d1;
            }
            else
            {
                x++; 
                d += d2;
            }
            glVertex2f(x, y);
        }
    }
    else if (m > 1)
    {
        d = a + 2 * b; 
        d1 = 2 * (a + b); 
        d2 = 2 * b;
        while (y < y1)
        {
            if (d > 0)
            {
                x++;
                y++;
                d += d1;
            }
            else
            {
                y++; 
                d += d2;
            }
            glVertex2f(x, y);
        }
    }
    else
    {
        d = a - 2 * b; 
        d1 = -2 * b;
        d2 = 2 * (a - b);
        while (y > y1)
        {
            if (d <= 0) 
            {
                x++; 
                y--; 
                d += d2;
            }
            else 
            {
                y--; 
                d += d1;
            }
            glVertex2f(x, y);
        }
    }
}

//B法画线
void BDrawLine(GLint x1, GLint y1, GLint x2, GLint y2)
{
    if (x2 < x1)
    {
        swap(x2, x1);
        swap(y2, y1);
    }
    //画第一个点
    int x, y;
    x = x1;
    y = y1;
    glVertex2f(x, y);
    //首先处理直线平行坐标轴
    if (y1 == y2)
    {
        //平行x轴
        while (x < x2)
        {
            x++;
            glVertex2f(x, y);
        }
        return;
    }
    if (x1 == x2)
    {
        //平行y轴
        while (y < y2)
        {
            y++;
            glVertex2f(x, y);
        }
        return;
    }
    int dx = x2 - x1; 
    int dy = y2 - y1;
    int p;
    int twoDy = 2 * dy, twoMinusDx = 2 * (dy - dx), twoDx = 2 * dx, twoMinusDy = 2 * (dx - dy);
    int twoSum = 2 * (dy + dx);

    double k = (double)dy / (double)dx;

    //0<k<1的情况
    if (k < 1.0 && k>0.0)
    {
        p = 2 * dy - dx;
        while (x < x2)
        {
            x++;
            if (p < 0)
            { 
                p += twoDy;
            }              
            else
            {
                y++;
                p += twoMinusDx;
            }
            glVertex2f(x, y);
        }
    }
    //k>=1的情况
    if (k >= 1.0)
    {
        p = dy;
        while (y < y2)
        {
            y++;
            if (p < 0)
            {
                p += twoDx;
            }          
            else
            {
                x++;
                p += twoMinusDy;
            }
            glVertex2f(x, y);
        }
    }
    //0>k>-1的情况
    if (k > -1 && k < 0)
    {
        p = 2 * dy + dx;
        while (x < x2)
        {
            x++;
            if (p >= 0)
            {
                p += twoDy;
            }             
            else
            {
                y--;
                p += twoSum;
            }
            glVertex2f(x, y);
        }
    }
    //k<-1的情况
    if (k <= -1)
    {
        p = 2 * dx - dy;
        while (y > y2)
        {
            y--;
            if (p >= 0)
            {
                p -= twoDx;
            }              
            else
            {
                x++;
                p -= twoSum;
            }
            glVertex2f(x, y);
        }
    }
}

//对称画圆方法
void CirclePoints(int x, int y, int offx, int offy)//利用对称性画整圆
{
    glVertex2f(x + offx, y + offy);
    glVertex2f(y + offx, x + offy);
    glVertex2f(x + offx, -y + offy);
    glVertex2f(-y + offx, x + offy);
    glVertex2f(-x + offx, y + offy);
    glVertex2f(y + offx, -x + offy);
    glVertex2f(-x + offx, -y + offy);
    glVertex2f(-y + offx, -x + offy);
}

//中点画圆算法
void MidPointCircle(int x1, int y1, int r)//中点画圆
{
    int x, y, e;
    x = 0; 
    y = r; 
    e = 1 - r;
    CirclePoints(x, y, x1, y1);
    while (x <= y)
    {
        if (e < 0)
            e += 2 * x + 3;
        else
        {
            e += 2 * (x - y) + 5;
            y--;
        }
        x++;
        CirclePoints(x, y, x1, y1);
    }
}

//B法画圆
void BDrawCircle(int x1, int y1, int radius)
{

    int mx = x1, my = y1;
    int x = 0, y = radius;
    int d = 1 - radius;    //起点(0,R),下一点中点(1,R-0.5),d=1*1+(R-0.5)*(R-0.5)-R*R=1.25-R,d只参与整数运算，所以小数部分可省略
    while (y > x)    // y>x即第一象限的第1区八分圆
    {
        CirclePoints(x, y, mx, my);    
        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
void circlepoints4(int x0, int x, int y0, int y) {
    glVertex2f(x0 + x, y0 + y);
    glVertex2f(x0 - x, y0 + y);
    glVertex2f(x0 + x, y0 - y);
    glVertex2f(x0 - x, y0 - y);
}
void bresenhamCircle(int x0,int y0,int r0) {
    int x, y, r, detla, direction;
    int detla1;
    x = 0;
    r = r0;
    y = r;
    detla = 2 * (1 - r);

    while (y > 0) {
        circlepoints4(x0, x, y0, y);
        if (detla < 0) {
            detla1 = 2 * (detla + y) - 1;
            if (detla1 <= 0)
                direction = 1;
            else
                direction = 2;
        }
        else if (detla > 0) {
            detla1 = 2 * (detla - x) - 1;
            if (detla1 < 0)
                direction = 2;
            else
                direction = 3;
        }
        else
            direction = 2;

        switch (direction) {
        case 1:
            x++;
            detla += 2 * x + 1;
            break;
        case 2:
            x++;
            y--;
            detla += 2 * (x - y + 1);
            break;
        case 3:
            y--;
            detla += (-2 * y + 1);
            break;
        }
    }
}

//椭圆4路对称
void Ellipsepot(int x0, int y0, int offx, int offy)
{
    // 1
    glVertex2f((x0 + offx), (y0 + offy));
    // 2
    glVertex2f((x0 + offx), (y0 - offy));
    // 3
    glVertex2f((x0 - offx), (y0 - offy));
    // 4
    glVertex2f((x0 - offx), (y0 + offy));
}

//中点画椭圆算法
void MidPointEllipse(int x0, int y0, int a, int b)
{
    double sqa = a * a;
    double sqb = b * b;

    double d = sqb + sqa * (0.25 - b);
    int x = 0;
    int y = b;
    Ellipsepot(x0, y0, x, y);
    // 1
    while (sqb * (x + 1) < sqa * (y - 0.5))
    {
        if (d < 0)
        {
            d += sqb * (2 * x + 3);
        }
        else
        {
            d += (sqb * (2 * x + 3) + sqa * ((-2) * y + 2));
            --y;
        }
        ++x;
        Ellipsepot(x0, y0, x, y);
    }
    d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
    // 2
    while (y > 0)
    {
        if (d < 0)
        {
            d += sqb * (2 * x + 2) + sqa * ((-2) * y + 3);
            ++x;
        }
        else
        {
            d += sqa * ((-2) * y + 3);
        }
        --y;
        Ellipsepot(x0, y0, x, y);
    }
}
//主循环方法呼出OpenGL
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glViewport(0, 0, 500, 500);

    //LineDDA(x1, y12, x2, y2);
    //BDrawLine(x1, y12, x2, y2);
    //MidpointLine(x1, y12, x2, y2);
    //bresenhamCircle(100, 100, 50);
    //MidPointCircle(50,50,20);
    //BDrawCircle(50, 50, 20);
    MidPointEllipse(100,100, 50 ,40);

    glEnd();
    glFlush();
}

//鼠标点击响应方法
void mymouse(int button, int state, int x, int y) {
    // 按下左键，起始坐标
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        x1 = x;
        y12 = 500-y;
    }
    // 松开左键，结束坐标
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        x2 = x;
        y2 = 500-y;
        k++;
        glutPostRedisplay();
    }
}

//鼠标移动响应方法
void mymotion(int x, int y) {
    x2 = x;
    y2 = 500 - y;
    glutPostRedisplay();
}

//OpenGL入口
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("计算机图形学");
    glutMouseFunc(mymouse);
    glutMotionFunc(mymotion);
    glutDisplayFunc(display);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glutMainLoop();
}