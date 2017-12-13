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

bool flag_ini = 0, SphereFlag = true, normFlag = 0;


bool ilum = false;

static int window;
static int menu_id;
static int submenu_id;
static int submenu0_id;
static int submenu1_id;
static int submenu2_id;
static int submenu3_id;

static int value = 0;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint programIni;
GLuint program;
GLuint sphere_buffer;
GLuint floor_buffer;
GLuint x_buffer;
GLuint y_buffer;
GLuint z_buffer;

GLfloat  fovy = 45.0;
GLfloat  aspect;
GLfloat  zNear = 0.5, zFar = 15.0;

GLfloat angleX = 0.1;
GLfloat angleZ = 0.1;
GLfloat angle = 0.0;

mat4 rot = Rotate(0, 1, 1, 1);

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
point3 floor_normal[6];

color4 x_colors[3];
point3 x_points[3];
point3 x_normal[3];

color4 y_colors[3];
point3 y_points[3];
point3 y_normal[3];

color4 z_colors[3];
point3 z_points[3];
point3 z_normal[3];

void floor(){
  floor_colors[0] = floor_color; floor_points[0] = point3(5, 0, 8);
	floor_colors[1] = floor_color; floor_points[1] = point3(5, 0, -4);
	floor_colors[2] = floor_color; floor_points[2] = point3(-5, 0, -4);
  floor_normal[0] = point3(0, 1, 0);
  floor_normal[1] = point3(0, 1, 0);
  floor_normal[2] = point3(0, 1, 0);


	floor_colors[3] = floor_color; floor_points[3] = point3(5, 0, 8);
	floor_colors[4] = floor_color; floor_points[4] = point3(-5, 0, -4);
	floor_colors[5] = floor_color; floor_points[5] = point3(-5, 0, 8);
  floor_normal[3] = point3(0, 1, 0);
  floor_normal[4] = point3(0, 1, 0);
  floor_normal[5] = point3(0, 1, 0);

  x_colors[0] = x_color; x_points[0] = point3(0, 0, 0); x_normal[0] = point3(1, 0, 0);
  x_colors[1] = x_color; x_points[1] = point3(0, 0, 0); x_normal[1] = point3(1, 0, 0);
  x_colors[2] = x_color; x_points[2] = point3(10, 0, 0);x_normal[2] = point3(1, 0, 0);

  y_colors[0] = y_color; y_points[0] = point3(0, 0, 0); y_normal[0] = point3(0, 1, 0);
  y_colors[1] = y_color; y_points[1] = point3(0, 0, 0); y_normal[1] = point3(0, 1, 0);
  y_colors[2] = y_color; y_points[2] = point3(0, 10, 0);y_normal[2] = point3(0, 1, 0);

  z_colors[0] = z_color; z_points[0] = point3(0, 0, 0); z_normal[0] = point3(0, 1, 0);
  z_colors[1] = z_color; z_points[1] = point3(0, 0, 0); z_normal[1] = point3(0, 1, 0);
  z_colors[2] = z_color; z_points[2] = point3(0, 0, 10);z_normal[2] = point3(0, 1, 0);
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
    sphere_normFlat.push_back(pn);
    sphere_normFlat.push_back(pn);
    sphere_normFlat.push_back(pn);
  }
  numNodes = sphere_points.size();
  std::cout << numNodes << '\n';
}

void init(){
  unsigned int sizeData = 0;

  glGenBuffers(1, &sphere_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*numNodes*3 + sizeof(color4)*numNodes
                              , NULL, GL_STATIC_DRAW);
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(point3), sphere_points[i]);
    sizeData += sizeof(point3);
  }
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(point3), sphere_normFlat[i]);
    sizeData += sizeof(point3);
  }
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(point3), sphere_normSmooth[i]);
    sizeData += sizeof(point3);
  }
  for (int i = 0; i < numNodes; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, sizeData, sizeof(color4), sphereLines_color);
    sizeData += sizeof(color4);
  }
  floor();
  glGenBuffers(1, &floor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points)*3 + sizeof(floor_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_normal), floor_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points)*2, sizeof(floor_normal), floor_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points)*3, sizeof(floor_colors),
		floor_colors);

  glGenBuffers(1, &x_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, x_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(x_points)*3 + sizeof(x_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_points), x_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points), sizeof(x_points), x_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points)*2, sizeof(x_points), x_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_points)*3, sizeof(x_colors),
		x_colors);

  glGenBuffers(1, &y_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, y_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y_points)*3 + sizeof(y_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(y_points), y_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points), sizeof(y_normal), y_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points)*2, sizeof(y_normal), y_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(y_points)*3, sizeof(y_colors),
		y_colors);

  glGenBuffers(1, &z_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, z_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(z_points)*3 + sizeof(z_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(z_points), z_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points), sizeof(z_normal), z_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points)*2, sizeof(z_normal), z_normal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(z_points)*3, sizeof(z_colors),
		z_colors);

  programIni = InitShader("vshader41.glsl", "fshader41.glsl");
  program = InitShader("vshader42.glsl", "fshader42.glsl");
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.529, 0.807, 0.92, 0.0 );
  glLineWidth(2.0);
}

void drawObj(GLuint buffer, int num_vertices,bool flag)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GLuint vNormal, vPosition, vColor;

    if (flag) {
      vPosition = glGetAttribLocation(program, "vPosition");
      vColor = glGetAttribLocation(program, "vColor");
      vNormal = glGetAttribLocation(program, "vNormal");
      glEnableVertexAttribArray(vPosition);
      glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0) );
      glEnableVertexAttribArray(vNormal);
      if (normFlag) {
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
          BUFFER_OFFSET(sizeof(point3) * num_vertices*2) );
      }else{
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
          BUFFER_OFFSET(sizeof(point3) * num_vertices) );
      }

      glEnableVertexAttribArray(vColor);
      glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(point3) * num_vertices*3) );
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);
        glDisableVertexAttribArray(vPosition);
        glDisableVertexAttribArray(vNormal);
        glDisableVertexAttribArray(vColor);
    }else{
      vPosition = glGetAttribLocation(programIni, "vPosition");
      vColor = glGetAttribLocation(programIni, "vColor");
      glEnableVertexAttribArray(vPosition);
      glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0) );

      glEnableVertexAttribArray(vColor);
      glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(point3) * num_vertices*3) );
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);

        glDisableVertexAttribArray(vPosition);
        glDisableVertexAttribArray(vColor);
    }


}

void valueMenu() {
  switch (value) {
  	case 1:
  	{
      GLfloat angleX = 0.1;
      GLfloat angleZ = 0.1;
      GLfloat angle = 0.0;
      rot = Rotate(0, 1, 1, 1);
      posiAc = A;
      rotFlag = 3;
  		value = 0;

  		break;
  	}
  	case 3:{
      ilum = 0;
  		break;
  	}
  	case 4:{
      ilum = 1;
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
  	case 7:{
      normFlag = 0;
  		break;
  	}
  	case 8:{
      normFlag = 1;
  		break;
  	}
  	default:{

    }
  }
}

void display( void ){

  GLuint  model_view;
  GLuint  projection;
  GLuint imodel;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  if (ilum) {
    glUseProgram(program);
    model_view = glGetUniformLocation(program, "model_view" );
    projection = glGetUniformLocation(program, "projection" );
    imodel = glGetUniformLocation(program, "imodel");
  }else{
    glUseProgram(programIni);
    model_view = glGetUniformLocation(programIni, "model_view" );
    projection = glGetUniformLocation(programIni, "projection" );
    imodel = glGetUniformLocation(program, "imodel");
  }


  mat4  p = Perspective(fovy, aspect, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);


  mat4  v = LookAt(eye, VPN, VUP);

  mat4 m = Translate(posiAc)*rot;

  mat4  im = mat4WithUpperLeftMat3(inverse(upperLeftMat3(m)));

  glUniformMatrix4fv(model_view, 1, GL_TRUE, v*m);
  if(ilum){
    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);
    glUniform3f(glGetUniformLocation(program, "viewPos"), eye[0], eye[1], eye[2]);
    glUniform3f(glGetUniformLocation(program, "material.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.84f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "material.specular"), 1.0f, 0.84f, 0.0f);
    glUniform1f(glGetUniformLocation(program, "shininess"), 125.0f);

    glUniform3f(glGetUniformLocation(program, "dirLight.direction"), -0.638813, -0.238667, 0.738211);
    glUniform3f(glGetUniformLocation(program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "globalAmbientLight"), 1.0f, 1.0f, 1.0f);
  }

  if (SphereFlag == 1)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


  drawObj(sphere_buffer, numNodes, ilum);

  m = Translate(0.0, 0.05, 0.0);
  im = mat4WithUpperLeftMat3(inverse(upperLeftMat3(m)));
  mat4 mv = LookAt(eye, VPN, VUP) * Translate(0.0, 0.05, 0.0);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(ilum){
    glUniformMatrix4fv(imodel, 1, GL_TRUE, im);
    glUniform3f(glGetUniformLocation(program, "material.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(program, "material.specular"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.84f, 0.0f);
  }
	drawObj(x_buffer, 3, ilum);
	drawObj(y_buffer, 3, ilum);
	drawObj(z_buffer, 3, ilum);

  mv = LookAt(eye, VPN, VUP);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  drawObj(floor_buffer, 6, ilum) ;

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
        rot = Rotate(angle, v1.z, 0, -v1.x)*rot;
        if (posiAc.z <= -4 && posiAc.x<=-1 ) {
          rotFlag = 2;
          posiAc = B;
        }
        break;
      }
      case 2: {
        posiAc += v2/V;
        rot = Rotate(angle, v2.z, 0, -v2.x)*rot;
        if (posiAc.z >= -2.5 && posiAc.x >= 3.5){
          rotFlag = 3;
          posiAc = C;
        }
        break;
      }
      case 3: {
        posiAc += v3/V;
        rot = Rotate(angle, v3.z, 0, -v3.x)*rot;
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
	case 033:
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'X': eye[0] += 1.0; break;
	case 'x': eye[0] -= 1.0; break;
	case 'Y': eye[1] += 1.0; break;
	case 'y': eye[1] -= 1.0; break;
	case 'Z': eye[2] += 1.0; break;
	case 'z': eye[2] -= 1.0; break;
  case 'b': case 'B':
		flag_ini = 1;
		break;

  case 'c': case 'C':
		SphereFlag = 1 - SphereFlag;
		break;

	case ' ':
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

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddSubMenu("Enable Lighting", submenu_id);
	glutAddSubMenu("Shading", submenu0_id);
	glutAddSubMenu("Ligth Source", submenu1_id);
	glutAddSubMenu("Wire Frame", submenu2_id);
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
