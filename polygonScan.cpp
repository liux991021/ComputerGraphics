#include <iostream>
#include <vector>
#include "GL/glut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  //��Ҫʲô�任���͵���ʲô�ļ����������ȥ������
#include <glm/gtc/type_ptr.hpp>

using namespace std;

//�������ڱ߱�ET�ͻ�߱�AET��ͨ����Edge
class Edge
{
public:
    float ymax;
    float x;
    float dx;
    Edge* next;
};

//�������ڱ�ʾ���ص��������Point
class Point
{
public:
    int x;
    int y;
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};
//������
//Demo1
//const int windowWidth = 18;
//const int windowHeight = 12;

//Demo2
//const int windowWidth = 180;
//const int windowHeight = 120;

//Demo3��Demo4��Demo5
const int windowWidth = 1800;
const int windowHeight = 1200;

int x1, y12;

//����ζ���
//Demo1
//vector<Point> vertices = { Point(2, 5), Point(2, 10), Point(9, 6), Point(16, 11), Point(16, 4), Point(12, 2), Point(7, 2) };

//Demo2
//vector<Point> vertices = { Point(20, 50), Point(20, 100), Point(90, 60), Point(160, 110), Point(160, 40), Point(120, 20), Point(70, 20) };

//Demo3 �����
//vector<Point> vertices = { Point(200, 500), Point(200, 1000), Point(900, 600), Point(1600, 1100), Point(1600, 400), Point(1200, 200), Point(700, 200) };

//Demo4 ��ͷ
//vector<Point> vertices = { Point(395, 887), Point(479, 998), Point(1199, 433), Point(1101, 867), Point(1294, 715), Point(1417, 171), Point(857, 163), Point(668, 314), Point(1111, 321) };

//Demo5 ����
vector<Point> vertices = { Point(566, 970), Point(685, 1020), Point(754, 683), Point(985, 768), Point(1037, 481), Point(1208, 546), Point(1233, 179), Point(1140, 440), Point(951, 386), Point(899, 662), Point(668, 562) };

//�߱�
Edge* ET[windowHeight];
//��߱�
Edge* AET;

void polygonScan()
{
    //������ߵ��y����
    int maxY = 0;
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].y > maxY)
        {
            maxY = vertices[i].y;
        }
    }

    //��ʼ��ET��AET
    Edge* pET[windowHeight];
    for (int i = 0; i < maxY; i++)
    {
        pET[i] = new Edge();
        pET[i]->next = nullptr;
    }

    AET = new Edge();
    AET->next = nullptr;

    //�����ʾ���ڲ����û�����ɫΪ��ɫ
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);

    //�����߱�ET
    for (int i = 0; i < vertices.size(); i++)
    {
        //ȡ����ǰ��1ǰ�����ڵĹ�4���㣬��1���2��������Ϊ����ѭ������ıߣ������������0�͵�3���ڼ������
        int x0 = vertices[(i - 1 + vertices.size()) % vertices.size()].x;
        int x1 = vertices[i].x;
        int x2 = vertices[(i + 1) % vertices.size()].x;
        int x3 = vertices[(i + 2) % vertices.size()].x;
        int y0 = vertices[(i - 1 + vertices.size()) % vertices.size()].y;
        int y1 = vertices[i].y;
        int y2 = vertices[(i + 1) % vertices.size()].y;
        int y3 = vertices[(i + 2) % vertices.size()].y;

        //ˮƽ��ֱ������
        if (y1 == y2)
        {
            continue;
        }

        //�ֱ�����¶˵�y���ꡢ�϶˵�y���ꡢ�¶˵�x�����б�ʵ���
        int ymin = y1 > y2 ? y2 : y1;
        int ymax = y1 > y2 ? y1 : y2;
        float x = y1 > y2 ? x2 : x1;
        float dx = (x1 - x2) * 1.0f / (y1 - y2);

        //������⴦������2->1->0��y���굥���ݼ���y1Ϊ��㣬����1->2->3��y���굥���ݼ���y2Ϊ���
        if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3)))
        {
            ymin++;
            x += dx;
        }

        //�����±ߣ�����߱�ET
        Edge* p = new Edge();
        p->ymax = ymax;
        p->x = x;
        p->dx = dx;
        p->next = pET[ymin]->next;
        pET[ymin]->next = p;

    }

    //ɨ���ߴ�������ɨ�裬y����ÿ�μ�1
    for (int i = 0; i < maxY; i++)
    {
        //ȡ��ET�е�ǰɨ���е����б߲���x�ĵ���˳����x�����dx�ĵ���˳�򣩲���AET
        while (pET[i]->next)
        {
            //ȡ��ET�е�ǰɨ���б�ͷλ�õı�
            Edge* pInsert = pET[i]->next;
            Edge* p = AET;
            //��AET���������ʵĲ���λ��
            while (p->next)
            {
                if (pInsert->x > p->next->x)
                {
                    p = p->next;
                    continue;
                }
                if (pInsert->x == p->next->x && pInsert->dx > p->next->dx)
                {
                    p = p->next;
                    continue;
                }
                //�ҵ�λ��
                break;
            }
            //��pInsert��ET��ɾ����������AET�ĵ�ǰλ��
            pET[i]->next = pInsert->next;
            pInsert->next = p->next;
            p->next = pInsert;
        }

        //AET�еı�������Բ�ѡ����ɫ
        Edge* p = AET;
        while (p->next && p->next->next)
        {
            //���
            glVertex2i(p->next->x, i);
            glVertex2i(p->next->next->x, i);

            //��ɫ
            /*for (int x = p->next->x; x < p->next->next->x; x++)
            {
                glVertex2i(x, i);
            }*/

            p = p->next->next;
        }

        //ɾ��AET������y=ymax�ı�
        p = AET;
        while (p->next)
        {
            if (p->next->ymax == i)
            {
                Edge* pDelete = p->next;
                p->next = pDelete->next;
                pDelete->next = nullptr;
                delete pDelete;
            }
            else
            {
                p = p->next;
            }
        }

        //����AET�бߵ�xֵ��������һѭ��
        p = AET;
        while (p->next)
        {
            p->next->x += p->next->dx;
            p = p->next;
        }

    }

    glEnd();
    glFlush();
}
/*
int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Polygon Scan");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
    glutDisplayFunc(polygonScan);
    glutMainLoop();
    return 0;
}*/
