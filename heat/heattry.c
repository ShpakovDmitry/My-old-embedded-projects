#include "GL/freeglut.h"
#include "GL/gl.h"

#include <stdlib.h>
#include <stdio.h>

int WindowWidth  = 500;
int WindowHeight = 500;

struct point
{
	float x,y,color_red;
};

float length = 1.0;
float width  = 1.0;
int N = 1;
int M = 1;


void renderfunction()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	
	//struct point a[(N+1)*(M+1)];
	struct point *a;
	a=(struct point *)malloc(sizeof(struct point)*(M+1)*(N+1));
	int i,j;
	float x,y;
	i=0;
	for(y=-width/2.0; y<=width/2.0+(float)width/M/10 ; y+=(float)width/M )
	 for(x=-length/2.0; x<=length/2.0+(float)length/N/10 ; x+=(float)length/N )
	 {
	 	a[i].x = x;
	 	a[i].y = y;
	 	a[i].color_red = (float)rand()/(RAND_MAX + 1.0);
	 	i++;
	 }
	
	glBegin(GL_QUADS);
	 for(j=0; j<M; j++)
	 for(i=0; i< N; i++)
	  {
	  	glColor3f(a[i+j*(N+1)].color_red, 0.0, 0.0);
	 	glVertex2f(a[i+j*(N+1)].x, a[i+j*(N+1)].y);
	 	
	 	glColor3f(a[i+(j+1)*(N+1)].color_red, 0.0, 0.0);
	 	glVertex2f(a[i+(j+1)*(N+1)].x, a[i+(j+1)*(N+1)].y);
	 	
	 	glColor3f(a[i+(j+1)*(N+1)+1].color_red, 0.0, 0.0);
	 	glVertex2f(a[i+(j+1)*(N+1)+1].x, a[i+(j+1)*(N+1)+1].y);
	 	
	 	glColor3f(a[i+j*(N+1)+1].color_red, 0.0, 0.0);
	 	glVertex2f(a[i+j*(N+1)+1].x, a[i+j*(N+1)+1].y);
	  }
	glEnd();
	
	/*for(i=0;i<(N+1)*(M+1);i++)
	 printf("%f %f %f\n", a[i].x, a[i].y, a[i].color_red);
	*/
	/*glBegin(GL_POLYGON);
	 glColor3f(1.0, 0.0, 0.0);
	 glVertex2f(-0.5,  -0.5);
	 glColor3f(1.0, 0.0, 0.0);
	 glVertex2f(-0.5,  0.5);
	 glColor3f(0.0, 0.0, 0.0);
	 glVertex2f( 0.0,  0.5);
	 glColor3f(0.0, 0.0, 0.0);
	 glVertex2f( 0.0, -0.5);
	 
	glEnd();
	
	glBegin(GL_POLYGON);
	 glColor3f(0.0, 0.0, 0.0);
	 glVertex2f( 0.0, -0.5);
	 glColor3f(0.0, 0.0, 0.0);
	 glVertex2f( 0.0,  0.5);
	 glColor3f(0.0, 1.0, 0.0);
	 glVertex2f( 0.5,  0.5);
	 glColor3f(0.0, 1.0, 0.0);
	 glVertex2f( 0.5, -0.5);
	 
	glEnd();
	*/
	
	/*struct point a[10];

	int i;

	for(i=0; i<10; i+=2)
	{
		
		a[i].x = -0.5 + i*0.1;
		a[i].y = 0.0;
		a[i+1].x = a[i].x;
		a[i+1].y = 0.5;
		
		a[i].color_red = (float)rand()/(RAND_MAX + 1.0);
		a[i+1].color_red = (float)rand()/(RAND_MAX + 1.0);
	}
	
	glBegin(GL_QUAD_STRIP);
	 for(i=0; i<10; i++)
	 {
	 	glColor3f(a[i].color_red, 0.0, 0.0);
	 	glVertex2f(a[i].x, a[i].y);
	 }

	 
	glEnd();*/
	
	
	
	
	glFlush();
	
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_SINGLE);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Heat transition");
	
	//glutFullScreen();
	
	glutDisplayFunc(renderfunction);
	
	//glutReshapeFunc(myReshape);
	//glutIdleFunc(renderfunction);
	//glutMouseFunc(mouse);
	
	glutMainLoop();
	
	return 0;
}
