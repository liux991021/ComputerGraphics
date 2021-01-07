/*#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include<stdio.h>

float xmin, xmax, ymin, ymax;


//��������д���ʹ��������λ��0-1֮��
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
	//flagΪ��־������0��ʾ������1��ʾ�ɼ�
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
			//mȡ�����������ֵ
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
			//nȡ�뿪�����Сֵ
			*tU = r;
		}	
	}
	else if (q < 0 && p == 0)
	{
		//ƽ���ڱ߽窡�����ڽ������
		flag = 0;
	}
	return flag;
}
//Liang-Barskyֱ�߲ü��㷨
void myclip()
{
	float dx, dy, x1, x2, y1, y2, tL, tU;
	tL = 0;
	tU = 1.0;

	printf("�������߶ε�������������x1,y1,x2,y2:\n");
	scanf_s("%f%f%f%f",&x1,&y1,&x2,&y2);
	x1=x1/10;
	y1=y1/10;
	x2=x2/10;
	y2=y2/10;
	//ʹ��opengl�Դ��Ļ��߷���
	glBegin(GL_LINES);
	glColor4f(0.0, 0.0, 0.0, 0.0);

	glVertex2f(x1, y1); //	��� 
	glVertex2f(x2, y2); //  �յ� 

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
						//ͨ��n��òü����p2�˵�
						x2 = x1 + tU * dx; 
						y2 = y1 + tU * dy;
					}
					if (tL > 0.0)
					{
						//ͨ��m��òü����p1�˵�
						x1 = x1 + tL * dx; 
						y1 = y1 + tL * dy;
					}
					//���»���
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
	
	//��������
	printf("��ֱ�������ζԽ����ϵ��������㣺\n");
	scanf_s("%f%f%f%f",&xmin,&ymin,&xmax,&ymax);
	x1=x1/10;
	y1=y1/10;
	x2=x2/10;
	y2=y2/10;
	glColor4f(1.0, 1.0, 0.0, 0.75);

	//ʹ��GL_POLYGON���ƶ���Σ������ĸ�����
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