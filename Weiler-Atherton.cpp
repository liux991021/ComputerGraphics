#include<iostream>
#include <GL/glut.h>
#include<vector>

using namespace std;
//多边形的端点
float result[6][2] = 
{ 
	{ 25, 10 },
	{ 40, 150 },
	{ 50, 30 },
	{ 90, 80 },
	{ 100, 30 },
	{ 130, 5 }
	
};
//多边形端点个数
int resultlen = 6;

//矩形的端点
float rx1 = 20, ry1 = 20, rx2 = 160, ry2 = 160;

float re[4][2] = 
{ 
	{ rx1, ry2 },
	{ rx2, ry2 },
	{ rx2, ry1 },
	{ rx1, ry1 }
};
//矩形端点个数
int relen = 4;

//关于标记的枚举
enum mark { L, B, R, T  };

//顶点的属性结构
struct P 
{
	float x;
	float y;
	int judge;
};

//双向链表结构
struct node 
{
	float x;
	float y;
	int judge;
	node* next;
	node* pre;
};

//链表增加操作
void addNode(node*& now, int num1, int num2, int ju) 
{
	node* p = now;
	node* q = NULL;
	q = new node;
	q->x = num1;
	q->y = num2;
	q->judge = ju;
	q->pre = p;
	p->next = q;
	now = q;
}
int count1 = 0;
//链表插入操作
void insertNode(node*& one, node*& two, const P matchPoint, bool change) 
{
	node* tem = new node;
	tem = new node;

	tem->x = matchPoint.x;
	tem->y = matchPoint.y;

	tem->judge = matchPoint.judge;

	tem->next = two;
	two->pre = tem;

	tem->pre = one;
	one->next = tem;
	if (change) {
		two = tem;
	}
	else {
		one = tem;
	}
}

//链表初始化操作
void initNode(node*& head, node*& rec) 
{
	head = new node;
	rec = new node;
	head->x = rec->x = 0;
	head->y = rec->y = 0;
	head->next = rec->next = NULL;
	head->pre = rec->pre = NULL;

	node* p = head;
	//记录被裁减多边形顶点
	for (int i = 0; i < resultlen; ++i)
	{
		addNode(p, result[i][0], result[i][1], 0);
	}
	p->next = head;
	head->pre = p;

	//记录裁剪多边形顶点
	p = rec;
	for (int i = 0; i < relen; ++i) 
	{
		addNode(p, re[i][0], re[i][1], 0);
	}
	p->next = rec;
	rec->pre = p;
}

//检测是否有交点,判断边界条件
int cross(const node* one, const node* two, const int edg) 
{
	int result = 0;
	switch (edg) 
	{
	case T:
		if ((one->y > ry2) && (two->y < ry2) || (one->y < ry2) && (two->y > ry2))
		{
			result = 1;
		}
		break;
	case R:
		if ((one->x > rx2) && (two->x < rx2) || (one->x < rx2) && (two->x > rx2))
		{
			result = 1;
		}
		break;
	case B:
		if ((one->y > ry1) && (two->y < ry1) || (one->y < ry1) && (two->y > ry1))
		{
			result = 1;
		}
		break;
	case L:
		if ((one->x > rx1) && (two->x < rx1) || (one->x < rx1) && (two->x > rx1))
		{
			result = 1;
		}
		break;
	default:
		break;
	}
	return result;
}

//得到交点
void testPoint(vector<P>& matchPoint, node*& first, node*& head, int edg, float xl, float xr, float yb, float yt) 
{
	node* p = first;
	node* q = p->next;
	P tem;
	//依次对每个点处理，把交点存入matchPoint
	while (p != head) 
	{
		int x = 0;
		int y = 0;
		if (q == head) 
		{
			q = q->next;
		}
		//求k值
		float k = 0;
		if (q->x - p->x != 0) 
		{
			k = (q->y - p->y) * 1.0 / (q->x - p->x);
		}

		switch (edg) 
		{
		case L:
			if (cross(p, q, L)) 
			{
				x = xl;
				y = p->y + (x - p->x) * k;
				tem.x = x;
				tem.y = y;
				tem.judge = 1;
				matchPoint.push_back(tem);
			}
			break;
		case R:
			if (cross(p, q, R)) 
			{
				x = xr;
				y = p->y + (x - p->x) * k;
				tem.x = x;
				tem.y = y;
				tem.judge = 1;
				matchPoint.push_back(tem);
			}
			break;
		case B:
			if (cross(p, q, B)) 
			{
				if (k == 0) 
				{    
					//处理两点垂直和平行情况
					if (p->y != q->y) 
					{
						y = yb;
						x = p->x;
					}
					else 
					{
						break;
					}
				}
				if (k != 0)
				{
					y = yb;
					x = p->x + (y - p->y) / k;
				}
				tem.x = x;
				tem.y = y;
				tem.judge = 1;
				matchPoint.push_back(tem);
			}
			break;
		case T:
			if (cross(p, q, T)) 
			{
				if (k == 0) 
				{
					if (p->y != q->y) 
					{
						y = yb;
						x = p->x;
					}
					else 
					{
						break;
					}
				}
				if (k != 0) 
				{
					y = yt;
					x = p->x + (y - p->y) / k;
				}
				tem.x = x;
				tem.y = y;
				tem.judge = 1;
				matchPoint.push_back(tem);
			}
			break;
		default:
			break;
		}
		//对处理点更新
		p = p->next;
		q = p->next;
	}
}

//对每个边进行裁剪，求得交点
void getPoint(vector<P>& matchPoint, node*& head, float xl, float xr, float yb, float yt) 
{
	node* p = head->next;
	//分别对上下左右边进行裁剪
	for (int i = 0; i < 4; ++i) 
	{
		testPoint(matchPoint, p, head, i, xl, xr, yb, yt);
	}
}

//把交点插入数组
void sortPointRec(const vector<P> matchPoint, node*& recPoint, int num) 
{
	node* p = recPoint->next;
	node* q = p->next;
	//用于标记
	bool change = false;
	if (matchPoint[0].y == 20) {
		change = true;
	}
	if (num == 2) {
		change = false;
	}

	for (int i = 0; i < matchPoint.size(); ++i) 
	{
		if (q == recPoint) 
		{
			q = q->next;
		}
		if (matchPoint[i].y == ry2) 
		{
			if ((matchPoint[i].x <= q->x && (q->y == ry2)) || (matchPoint[i].x <= q->x && cross(q, p, T))) 
			{
				insertNode(p, q, matchPoint[i], change);
			}
			else 
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (matchPoint[i].x == rx2) 
		{
			if ((matchPoint[i].y >= q->y && (q->x == rx2)) || (matchPoint[i].y >= p->y && cross(p, q, R))) 
			{
				insertNode(p, q, matchPoint[i], change);

			}
			else 
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (matchPoint[i].y == ry1) 
		{
			if ((matchPoint[i].x <= p->x && (p->y == ry1)) || (matchPoint[i].x <= q->x && cross(p, q, B))) 
			{
				insertNode(p, q, matchPoint[i], change);
			}
			else 
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (matchPoint[i].x == rx1) 
		{
			if ((matchPoint[i].y <= p->y && (p->x == rx1)) || (matchPoint[i].y <= q->y && cross(p, q, L)))
			{
				insertNode(p, q, matchPoint[i], change);
			}
			else 
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
	}
}

//得到混合数组
void getArray(node*& polPoint, const vector<P> matchPoint, node*& recPoint) 
{
	sortPointRec(matchPoint, recPoint, 1);
	sortPointRec(matchPoint, polPoint, 2);
}

//得到输出数组
void draw(node*& polPoint, node*& recPoint) 
{
	vector<P> Q, S;
	node* one = polPoint->next;
	while ( one->next != polPoint)
	{
		cout << "(" << one->x << "," << one->y << "," << one->judge << ")";
		one = one->next;
	}
	node* two = recPoint->next;
	cout << "\n";
	while (two->next != recPoint)
	{
		cout << "(" << two->x << "," << two->y << "," << two->judge << ")";
		two = two->next;
	}
	cout << "\n";
	P tem;
	//标记入点是否遍历完
	int c = 0;

	bool location = true;
	bool begin = false;
	one = polPoint->next;
	two = recPoint->next;
	node* get = one;
	node* ma = polPoint->next;   //检测是否有入点标记的点存在
	while (ma != polPoint)
	{
		if (ma->judge == 1)
		{
			c++;
		}
		ma = ma->next;
	}
	while (c != 0)
	{
		if (get->judge == 1 && location == true)  //在数组一遍历到入点
		{
			tem.x = get->x;
			tem.y = get->y;
			tem.judge = 0;
			S.push_back(tem);   //暂存入点
			begin = true;
			location = true;
		}
		if (get->judge == 2 && location == true && begin == true)  //在数组一遍历到出点
		{
			location = false;
			while ((two->x != get->x) || (two->y != get->y)) //数组跳转
			{
				//cout << "(" << two->x << "," << two->y << ")";
				two = two->next;
			}
			get = two;
		}
		if (get->judge == 1 && location == false)  //数组2遍历到入点
		{
			if ((get->x == S[0].x) && (get->y == S[0].y)) //当前入点等于起点
			{
				while ((one->x != get->x) || (one->y != get->y)) //数组跳转
				{
					one = one->next;
				}
				get = one;
				get->judge = 0;
				//是则输出绘制
				glBegin(GL_LINE_LOOP);
				for (int i = 0; i < Q.size(); ++i)
				{
					//cout << "(" << Q[i].x << "," << Q[i].y << ")";
					glVertex2f(Q[i].x, Q[i].y);
				}
				glEnd();
				Q.clear();
				S.clear();
				begin = false;
				location = true;
				get = polPoint->next; //重新遍历数组一
			}
			else
			{
				//不是则返回数组一
				location = true;
				while ((one->x != get->x) || (one->y != get->y)) //数组跳转
				{
					one = one->next;
				}
				get = one;
				get->judge = 0;
			}

		}
		if (get != recPoint && get != polPoint && begin == true) //防止把头节点压入输出数组
		{
			cout << "(" << get->x << "," << get->y << ")";
			tem.x = get->x;
			tem.y = get->y;
			tem.judge = 0;
			Q.push_back(tem);
		}
		get = get->next;
		//每次循环检测一次是否还有入点
		c = 0;
		ma = polPoint->next;
		while (ma != polPoint)
		{
			if (ma->judge == 1)
			{
				c++;
			}
			ma = ma->next;
		}
	}
}

void drawPol() 
{
	//处理多边形顶点
	node* h = new node;
	node* recPoint = new node;
	initNode(h, recPoint);

	//绘制原多边形
	node* p = h->next;
	glColor3f(1.f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	while (p != h) 
	{
		glVertex2f(p->x, p->y);
		p = p->next;
	}
	glEnd();

	//得到交点数组并进行排序确认出入点
	vector<P> matchPoint;
	getPoint(matchPoint, h, rx1, rx2, ry1, ry2);
	count1 = 0;
	for (int q = 0;q<matchPoint.size();q++)
	{
		
		if (matchPoint[q].y == 20)
		{
			if (count1 % 2 == 0)
			{
				matchPoint[q].judge = 1;
				count1++;
			}
			else
			{
				matchPoint[q].judge = 2;
				count1++;
			}
		}
		if (matchPoint[q].y == 160)
		{
			if (count1 % 2 == 0)
			{
				matchPoint[q].judge = 2;
				count1++;
			}
			else
			{
				matchPoint[q].judge = 1;
				count1++;
			}
		}
		//cout << "(" << matchPoint[q].x << "," << matchPoint[q].y  << ","<<matchPoint[q].judge<< ")" << "\n";
	}
	//得到混合数组并进行绘制
	getArray(h, matchPoint, recPoint);
	//绘制新的图形
	glColor3f(1.0f, 1.0f, 0.0f);
	draw(h, recPoint);
}
void initPlo() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 1.0f, 1.0f);

	//绘制矩形线框
	glBegin(GL_LINE_LOOP);
	glVertex2f(rx1, ry1);
	glVertex2f(rx1, ry2);
	glVertex2f(rx2, ry2);
	glVertex2f(rx2, ry1);

	glEnd();

	drawPol();

	glFlush();
}

void changeSize(int w, int h) 
{
	if (h == 0) 
	{
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) 
	{
		glOrtho(0.0f, 200.0f, 0.0f, 200.0f * h / w, 1.0f, -1.0f);
	}
	else 
	{
		glOrtho(0.0f, 200.0f * w / h, 0.0f, 200.0f, 1.0f, -1.0f);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Weiler-Atherton");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(initPlo);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return 0;
}
