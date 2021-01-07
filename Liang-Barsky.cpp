/*#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include<stdio.h>

float xmin, xmax, ymin, ymax;


//对坐标进行处理，使窗口坐标位于0-1之间
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) gluOrtho2D(0.0, 1.0, 0.0, 1.0 * (GLfloat)h / (GLfloat)w);
	else gluOrtho2D(0.0, 1.0 * (GLfloat)w / (GLfloat)h, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}

int Clip(float p, float q, float* tL, float* tU)
{
	//flag为标志变量0表示舍弃1表示可见
	int flag = 1;
	float r;

	if (p < 0.0)
	{
		r = q / p;
		if (r > * tU)
		{
			flag = 0;
		}
		else if (r > * tL) 
		{
			//m取进入点最大参数值
			*tL = r;
		}
	}
	else if (p > 0.0) 
	{
		r = q / p;
		if (r < *tL)
		{
			flag = 0;
		}
		else if (r < *tU) 
		{
			//n取离开点的最小值
			*tU = r;
		}	
	}
	else if (q < 0 && p == 0)
	{
		//平行于边界而且在界外的线
		flag = 0;
	}
	return flag;
}
//Liang-Barsky直线裁剪算法
void myclip()
{
	float dx, dy, x1, x2, y1, y2, tL, tU;
	tL = 0;
	tU = 1.0;

	printf("请输入线段的两个顶点坐标x1,y1,x2,y2:\n");
	scanf_s("%f%f%f%f",&x1,&y1,&x2,&y2);
	x1=x1/10;
	y1=y1/10;
	x2=x2/10;
	y2=y2/10;
	//使用opengl自带的画线方法
	glBegin(GL_LINES);
	glColor4f(0.0, 0.0, 0.0, 0.0);

	glVertex2f(x1, y1); //	起点 
	glVertex2f(x2, y2); //  终点 

	glEnd();

	dx = x2 - x1;

	if (Clip(-dx, x1 - xmin, &tL, &tU))
	{
		if (Clip(dx, xmax - x1, &tL, &tU)) 
		{
			dy = y2 - y1;
			if (Clip(-dy, y1 - ymin, &tL, &tU))
				if (Clip(dy, ymax - y1, &tL, &tU))
				{
					if (tU < 1.0)
					{
						//通过n求得裁剪后的p2端点
						x2 = x1 + tU * dx; 
						y2 = y1 + tU * dy;
					}
					if (tL > 0.0)
					{
						//通过m求得裁剪后的p1端点
						x1 = x1 + tL * dx; 
						y1 = y1 + tL * dy;
					}
					//重新画线
					glBegin(GL_LINES);

					glColor4f(1.0, 0.0, 0.0, 1.0);
					glVertex2f(x1, y1); 
					glVertex2f(x2, y2);

					glEnd();
				}
		}
	}
		

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//画个矩形
	printf("请分别输入矩形对角线上的两个顶点：\n");
	scanf_s("%f%f%f%f",&xmin,&ymin,&xmax,&ymax);
	x1=x1/10;
	y1=y1/10;
	x2=x2/10;
	y2=y2/10;
	glColor4f(1.0, 1.0, 0.0, 0.75);

	//使用GL_POLYGON绘制多边形，给出四个顶点
	glBegin(GL_POLYGON);

	glVertex2f(xmin, ymin);
	glVertex2f(xmax, ymin); 
	glVertex2f(xmax, ymax); 
	glVertex2f(xmin, ymax); 

	glEnd();

	myclip();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Liang-Barsky");

	glShadeModel(GL_FLAT);
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);

	glutMainLoop();

	return 0;
}*/