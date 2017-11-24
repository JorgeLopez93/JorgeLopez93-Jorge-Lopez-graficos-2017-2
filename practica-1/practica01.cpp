// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT
// Para compilar en ubuntu
// g++ -Wall -o practica01 practica01.cpp -lGL -lglut -lGLU

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif
using namespace std;

#define NUMVTX (10)

static int winwidth,winheight;
static int mx,my;
static int flag = 0;
static float rotx = 0.0f, roty = 0.0f;

GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};
GLfloat position[] = {0.0, 0.0, 4.0, 0.0};

float materialAmient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float materialDifuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };


class Later {
public:
  unsigned int size;
  vector<unsigned int> s;
  Later (unsigned int size_);
  void draw(GLfloat v[][3], GLfloat c[][3], GLfloat n[][3]);
};

Later::Later(unsigned int size_){
  size = size_;
};

void Later::draw(GLfloat v[][3], GLfloat c[][3], GLfloat n[][3]){
  unsigned int i, j;
  for (j = 0; j < s.size() - 1; j++) {
    glBegin(GL_QUAD_STRIP);
    for (i = s[j]; i < s[j + 1]; i++)
    {
      //glNormal3fv(n[i]);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glColor3fv(c[i]);
      glVertex3fv(v[i]);
    }
    glEnd();

  }
}

// J R L C
// J
Later J(10);

GLfloat Jv[10][3]={{-1, 1.1, 1}, {0, 1.1, 1},
                  {-1, 0.9, 1}, {0, 0.9, 1},
                  {-0.6, 0.9, 1}, {-0.4, 0.9, 1},
                  {-0.6, 0.1, 1}, {-0.4, -0.1, 1},
                  {-1, 0.1, 1}, {-1, -0.1, 1} };


/*GLfloat Jv[10][3]={{-1, 1.1, 1}, {0, 1.1, 1},
                   {0, 0.9, 1}, {-0.4, 0.9, 1},
                   {-0.4, -1.1, 1}, {-1, -1.1, 1},
                   {-1, -0.9, 1}, {-0.6, -0.9, 1},
                   {-0.6, 0.9, 1}, {-1, 0.9, 1}};*/

/*GLfloat Jv[12][3]={{-1, 1, 1}, {-1, 1, -1},
                   {0, 1, 1}, {0, 1, -1},
                   {-0.5, 1, 1}, {-0.5, 1, -1},
                   {-0.5, -1, 1}, {-0.5, -1, -1},
                   {-0.5, -1, 1}, {-0.5, -1, -1},
                   {-1, -1, 1}, {-1, -1, -1}};*/

GLfloat Jn[12][3]={{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},
                   {1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f}};

GLfloat Jc[12][3]={{0, 1, 0},{0, 1, 0},
                   {0, 1, 0},{0, 1, 0},
                   {0, 1, 0},{0, 1, 0},
                   {0, 1, 0},{0, 1, 0},
                   {0, 1, 0},{0, 1, 0},
                   {0, 1, 0},{0, 1, 0}};

// R
Later R(18);
GLfloat Rv[18][3]={{-1, 1.1, 1}, {-0.8, 0.9, 1},
                   {0.0, 1.1, 1}, {-0.2, 0.9, 1},
                   {0.0, 0.4, 1}, {-0.2, 0.6, 1},
                   {-1, 0.4, 1},{-0.8, 0.6, 1},
                   {-1, 1.1, 1}, {-0.8, 0.9, 1},
                   {-1, 0.4, 1},{-0.8, 0.4, 1},
                   {-1, -0.1, 1},{-0.8, -0.1, 1},
                   {-0.8, 0.4, 1},{0.0, -0.1, 1},
                   {-0.8, 0.2, 1},{-0.3, -0.1, 1}};

GLfloat Rc[18][3]={{1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0},
                    {1, 0, 0},{1, 0, 0}};
// L
Later L(6);
GLfloat Lv[6][3]={{-1, 1.1, 1}, {-0.8, 1.1, 1},
                   {-1, -0.1, 1}, {-0.8, 0.1, 1},
                   {0, -0.1, 1}, {0, 0.1, 1}};

GLfloat Lc[6][3]={{1, 1, 0},{1, 1, 0},
                  {1, 1, 0},{1, 1, 0},
                  {1, 1, 0},{1, 1, 0}};
// C
Later C(8);
GLfloat Cv[8][3] = {{0, 1.1, 1}, {0, 0.9, 1},
                    {-1, 1.1, 1}, {-0.8, 0.9, 1},
                    {-1, -0.1, 1}, {-0.8, 0.1, 1},
                    {0, -0.1, 1}, {0, 0.1, 1}};

GLfloat Cc[8][3] = {{0, 0, 1},{0, 0, 1},
                    {0, 0, 1},{0, 0, 1},
                    {0, 0, 1},{0, 0, 1},
                    {0, 0, 1},{0, 0, 1}};
void drawgraphix()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);

  glTranslatef(-1.7f,0.0f,-3.2f);
	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);
  J.draw(Jv, Jc, Jn);

  glTranslatef(1.5f,0.0f, 0.0f);
  glNormal3f(0.0f, 1.0f, 1.0f);
  R.draw(Rv, Rc, Jn);

  glTranslatef(1.5f,0.0f, 0.0f);
  glNormal3f(0.0f, 1.0f, 1.0f);
  L.draw(Lv, Lc, Jn);

  glTranslatef(1.5f,0.0f, 0.0f);
  glNormal3f(0.0f, 1.0f, 1.0f);
  C.draw(Cv, Cc, Jn);

	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth = width;
	winheight = height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			flag = 1;
		} else {
			flag = 0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx += 360.0f*(x - mx)/winwidth;
			roty += 360.0f*(y - my)/winheight;
		}

		mx = x;
		my = y;

		drawgraphix();

		flag = 2;
	}vector<GLfloat [3]> v;
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key == 'q' || key == 27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

static void init(){

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHTING);

  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
}

int main(int argc,char **argv)
{
  winwidth = winheight = 512;

  J.s.push_back(0);
  J.s.push_back(4);
  J.s.push_back(10);
  R.s.push_back(0);
  R.s.push_back(10);
  R.s.push_back(14);
  R.s.push_back(18);
  L.s.push_back(0);
  L.s.push_back(6);
  C.s.push_back(0);
  C.s.push_back(8);

  //J.c = &Jc1;
  glutInit(&argc,argv);
  glutInitWindowSize(winwidth,winheight);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutCreateWindow("Practica 1 - Ejercicio 1.1");
  init();
  glutDisplayFunc(drawgraphix);
  glutReshapeFunc(reshapefunc);
  glutMouseFunc(mousefunc);
  glutMotionFunc(motionfunc);
  glutKeyboardFunc(keyboardfunc);
  glutIdleFunc(idlefunc);

  glutMainLoop();

  return(0);
}
