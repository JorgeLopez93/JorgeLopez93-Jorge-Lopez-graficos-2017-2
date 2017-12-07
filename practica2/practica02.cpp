#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include "Angel-yjc.h"
using namespace std;

#define WIDTH  600
#define HEIGHT 600
#define PI 3.14159265

typedef Angel::vec4  color4;
typedef Angel::vec3  point3;
typedef Angel::vec4  vec4;

bool flag_ini = 0, SphereFlag = true;


bool iluminado = true;

static int window;
static int menu_id;
static int submenu_id;
static int submenu0_id;
static int submenu1_id;
static int submenu2_id;
static int submenu3_id;

static int value = 0;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint program;
GLuint sphere_buffer;
GLuint floor_buffer;
GLuint x_buffer;
GLuint y_buffer;
GLuint z_buffer;

GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 15.0;

GLfloat angleX = 0.1;
GLfloat angleZ = 0.1;
GLfloat angle = 0.0;

GLfloat angle1 = 0;
GLfloat angle2 = 0;
GLfloat angle3 = 0;


vec4 VRP = vec4(7.0, 3.0, -10.0, 1.0);
vec4 VPN = vec4(-7.0, -3.0, 10.0, 0.0);
vec4 VUP = vec4(0.0, 1.0, 0.0, 0.0);

vec4 eye = VRP;

std::vector<point3> sphere_points;
std::vector<point3> sphere_normFlat;
std::vector<point3> sphere_normSmooth;

color4 floor_color = color4( 0, 1, 0, 0);
color4 x_color = color4( 1, 0, 0, 1);
color4 y_color = color4( 1, 0, 1, 1);
color4 z_color = color4( 0, 0, 1, 1);
color4 sphereLines_color = color4( 1.0, 0.84, 0.0, 1);
color4 sky_color = color4( 0.529, 0.807, 0.92);

point3 A = point3(3, 1, 5);
point3 B = point3(-1, 1, -4);
point3 C = point3(3.5, 1, -2.5);

point3 v1, v2, v3;



point3 posiAc = A;


color4 floor_colors[6];
point3 floor_points[6];

color4 x_colors[3];
point3 x_points[3];

color4 y_colors[3];
point3 y_points[3];

color4 z_colors[3];
point3 z_points[3];

void floor(){
  floor_colors[0] = floor_color; floor_points[0] = point3(5, 0, 8);
	floor_colors[1] = floor_color; floor_points[1] = point3(5, 0, -4);
	floor_colors[2] = floor_color; floor_points[2] = point3(-5, 0, -4);

	floor_colors[3] = floor_color; floor_points[3] = point3(5, 0, 8);
	floor_colors[4] = floor_color; floor_points[4] = point3(-5, 0, -4);
	floor_colors[5] = floor_color; floor_points[5] = point3(-5, 0, 8);

  x_colors[0] = x_color; x_points[0] = point3(0, 0, 0);
  x_colors[1] = x_color; x_points[1] = point3(0, 0, 0);
  x_colors[2] = x_color; x_points[2] = point3(10, 0, 0);

  y_colors[0] = y_color; y_points[0] = point3(0, 0, 0);
  y_colors[1] = y_color; y_points[1] = point3(0, 0, 0);
  y_colors[2] = y_color; y_points[2] = point3(0, 10, 0);

  z_colors[0] = z_color; z_points[0] = point3(0, 0, 0);
  z_colors[1] = z_color; z_points[1] = point3(0, 0, 0);
  z_colors[2] = z_color; z_points[2] = point3(0, 0, 10);
}

int numNodes, Index, rotFlag;
float V = 20;

point3 normTri(int p1, int p2, int p3){
  point3 p = normalize(sphere_points[p1] + sphere_points[p2] + sphere_points[p3]);
  return p;
}

void readSphere(string s){
  freopen(s.c_str(),"r", stdin);
  int n, d, i, j;
  float x, y, z;
  cin>> n;
  for (i = 0; i < n; i++)  {
    cin>> d;
    for (j = 0; j < d; j++) {
      cin>> x >> y >> z;
      sphere_points.push_back(point3( x, y, z));
      sphere_normSmooth.push_back(normalize(point3( x, y, z)));
      //std::cout << x << "-" << y << "-" << z << '\n';
    }
    point3 pn = normTri(i*3, (i*3) + 1, (i*3) + 2);
    sphere_normFlat.push_back(point3( x, y, z));
    sphere_normFlat.push_back(point3( x, y, z));
    sphere_normFlat.push_back(point3( x, y, z));
  }
  numNodes = sphere_points.size();
  std::cout << numNodes << '\n';
}

void init(){
  unsigned int sizeData = 0;
  glGenBuffers(1, &sphere_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*numNodes + sizeof(color4)*numNodes, NULL,
               GL_STATIC_DRAW);
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(point3), sphere_points[i]);
    sizeData += sizeof(point3);
  }
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(color4), sphereLines_color);
    sizeData += sizeof(color4);
  }
  floor();
  glGenBuffers(1, &floor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
		floor_colors);

  glGenBuffers(1, &x_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, x_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(x_points) + sizeof(x_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_points), x_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points), sizeof(x_colors),
		x_colors);

  glGenBuffers(1, &y_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, y_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y_points) + sizeof(y_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(y_points), y_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points), sizeof(y_colors),
		y_colors);

  glGenBuffers(1, &z_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, z_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(z_points) + sizeof(z_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(z_points), z_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points), sizeof(z_colors),
		z_colors);

  program = InitShader("vshader42.glsl", "fshader42.glsl");
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.529, 0.807, 0.92, 0.0 );
  glLineWidth(2.0);
}

void drawObj(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(sizeof(point3) * num_vertices) );
      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}

void valueMenu() {
  switch (value) {
  	case 1:
  	{

      GLfloat angleX = 0.1;
      GLfloat angleZ = 0.1;
      GLfloat angle = 0.0;

      GLfloat angle1 = 0;
      GLfloat angle2 = 0;
      GLfloat angle3 = 0;
      posiAc = A;
      rotFlag = 3;
  		//flag_comenzar=true;
  		value = 0;
  		//glutSwapBuffers();

  		break;
  	}
  	case 5:{
      SphereFlag = 1;
  		break;
  	}
  	case 6:{
      SphereFlag = 0;
  		break;
  	}

  	default:{
    }
  }
}

void display( void ){

  GLuint  model_view;
  GLuint  projection;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glUseProgram(program);
  model_view = glGetUniformLocation(program, "model_view" );
  projection = glGetUniformLocation(program, "projection" );


  mat4  p = Perspective(fovy, aspect, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  vec4    at(0.0, 0.0, 0.0, 1.0);
  vec4    up(0.0, 1.0, 0.0, 0.0);

  mat4  mv = LookAt(eye, VPN, VUP);

  mv = mv * Translate(posiAc);//*Rotate(angle, angleX, 0, angleZ);

  mv = mv * Rotate(angle1, v1.z, 0, -v1.x);
  mv = mv * Rotate(angle2, v2.z, 0, -v2.x);
  mv = mv * Rotate(angle3, -v3.z, 0, v3.x);


  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  if (SphereFlag == 1)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  drawObj(sphere_buffer, numNodes);

  mv = LookAt(eye, VPN, VUP) * Translate(0.0, 0.05, 0.0) ;
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawObj(x_buffer, 3);
	drawObj(y_buffer, 3);
	drawObj(z_buffer, 3);

  mv = LookAt(VRP, VPN, VUP);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  drawObj(floor_buffer, 6) ;

  glutSwapBuffers();
  valueMenu() ;
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}

void idle(void)
{
  if (flag_ini) {
    switch (rotFlag) {
      case 1: {
        posiAc += v1/V;
        angle1 += angle;
        if (posiAc.z <= -4 && posiAc.x<=-1 ) {
          rotFlag = 2;
          posiAc = B;
        }

        break;
      }
      case 2: {
        posiAc += v2/V;
        angle2 += angle;
        if (posiAc.z >= -2.5 && posiAc.x >= 3.5){
          rotFlag = 3;
          posiAc = C;
        }
        break;
      }
      case 3: {
        posiAc += v3/V;
        angle3 += angle;
        if (posiAc.z >= 5 && posiAc.x <= 3) {
          rotFlag = 1;
          posiAc = A;
        }
        break;
      }
    }
    glutPostRedisplay();
  }
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'X': eye[0] += 1.0; break;
	case 'x': eye[0] -= 1.0; break;
	case 'Y': eye[1] += 1.0; break;
	case 'y': eye[1] -= 1.0; break;
	case 'Z': eye[2] += 1.0; break;
	case 'z': eye[2] -= 1.0; break;
  case 'b': case 'B': // Toggle between animation and non-animation
		flag_ini = 1;
		break;

  case 'c': case 'C': // Toggle between filled and wireframe cube
		SphereFlag = 1 - SphereFlag;
		break;

	case ' ':  // reset to initial viewer/eye position
		eye = VRP;
		break;
	}
	glutPostRedisplay();
}

void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}

void Menu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("NO", 3);
	glutAddMenuEntry("YES", 4);

	submenu0_id = glutCreateMenu(menu);
	glutAddMenuEntry("flat shading", 7);
	glutAddMenuEntry("smooth shading", 8);

	submenu1_id = glutCreateMenu(menu);
	glutAddMenuEntry("spot light", 9);
	glutAddMenuEntry("point source", 10);

	submenu2_id = glutCreateMenu(menu);
	glutAddMenuEntry("NO", 5);
	glutAddMenuEntry("YES", 6);

	submenu3_id = glutCreateMenu(menu);

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddSubMenu("Enable Lighting", submenu_id);
	glutAddSubMenu("Shading", submenu0_id);
	glutAddSubMenu("Ligth Source", submenu1_id);
	glutAddSubMenu("Wire Frame", submenu2_id);
	glutAddSubMenu("Services", submenu3_id);
	glutAddMenuEntry("Quit", 0);

	glutAttachMenu(GLUT_LEFT_BUTTON);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (flag_ini) {
			flag_ini = 0;
		}
		else {
			flag_ini = 1;
		}
	}
}

int main(int argc, char **argv) {
  angle = (180 / PI)/V;
  v1 = normalize(B - A);
  v2 = normalize(C - B);
  v3 = normalize(A - C);

  rotFlag = 1;
  int err;
  readSphere(argv[1]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Practica02");
  err = glewInit();
  if (GLEW_OK != err)
  { printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(err));
    exit(1);
  }

  Menu();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

  init();
  glutMainLoop();
  return 0;
}
