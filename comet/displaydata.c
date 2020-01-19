#include "GL/freeglut.h"
#include "GL/gl.h"

#include <stdio.h>

int WindowWidth  = 640;
int WindowHeight = 480;

double data[8][2001];

void printtext(double x, double y, char *String);


void renderfunction()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	gluOrtho2D(0.0, 2.0, 0.0, 2.0);
	
	int i;
	
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_POINTS);
	for(i=0;i<2001;i++)
	{
		glVertex2d((double)i, data[0][i]);
		//glVertex2f((double)(i+1),data[0][i+1]);
		
	}
	glEnd();
	
	printtext(1.0,1.0,"600 K");
	/*for(i=0; i<256; i++)
	{
		glBegin(GL_LINES);
		 glColor3f(1.0,(float)i*dc,0.0);
		 glVertex2f(0.8,1.0-(float)i*dy-2.0/6.0);
		 glVertex2f(0.7,1.0-(float)i*dy-2.0/6.0);
		glEnd();
	}*/
	
	
	
	
	glFlush();
	glutPostRedisplay();
	
}


int main(int argc, char **argv)
{

	int i,t;
	
	FILE *f = fopen("output.dat","r");
	if(f == NULL)
	{
		printf("Can't open file\n");
		return -1;
	}
	
	for(i=0;i<2001;i++)
	{
		fscanf(f,"%d %lf %lf %lf %lf %lf %lf %lf %lf", &t, &data[0][i],&data[1][i], &data[2][i],&data[3][i], &data[4][i],&data[5][i],\
		&data[6][i],&data[7][i]);

		
	}
	fclose(f);
	printf("Read OK!\n");

	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_SINGLE);
	//glutInitDisplayMode(GLUT_RGB);
	//glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Data");
	
	
	//glutFullScreen();
	
	glutDisplayFunc(renderfunction);
	
	glutReshapeFunc(renderfunction);
	//glutIdleFunc(renderfunction);
	//glutMouseFunc(mouse);
	
	glutMainLoop();
	
	return 0;
}

void printtext(double x, double y, char *String)
{
//(x,y) is from the bottom left of the window
    glColor3f(1.0 ,0.0 ,0.0 );
    /*glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();*/
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2f(x,y);
    int i;
    for (i=0; i<5; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
