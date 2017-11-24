#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/glut.h"

using namespace std;

void shadeInit ();

static int winwidth,winheight;
int shaderProgram, fragmentShader, vertexShader;
static int mx = 0,my = 0;
static int flag = 0;
static float rotx = 0.0f, roty = 0.0f;

float vertices[]={-1, 1.1, 1, 0, 1.1, 1,
                  -1, 0.9, 1, 0, 0.9, 1,
                  -0.6, 0.9, 1, -0.4, 0.9, 1,
                  -0.6, 0.1, 1, -0.4, -0.1, 1,
                  -1, 0.1, 1, -1, -0.1, 1,
                  //R
                  -1, 1.1, 1, -0.8, 0.9, 1,
                   0.0, 1.1, 1, -0.2, 0.9, 1,
                   0.0, 0.4, 1, -0.2, 0.6, 1,
                   -1, 0.4, 1,-0.8, 0.6, 1,
                   -1, 1.1, 1, -0.8, 0.9, 1,
                   -1, 0.4, 1,-0.8, 0.4, 1,
                   -1, -0.1, 1,-0.8, -0.1, 1,
                   -0.8, 0.4, 1,0.0, -0.1, 1,
                   -0.8, 0.2, 1,-0.3, -0.1, 1,
                  //L
                  -1, 1.1, 1, -0.8, 1.1, 1,
                  -1, -0.1, 1, -0.8, 0.1, 1,
                  0, -0.1, 1, 0, 0.1, 1,
                  //C
                  0, 1.1, 1, 0, 0.9, 1,
                  -1, 1.1, 1, -0.8, 0.9, 1,
                  -1, -0.1, 1, -0.8, 0.1, 1,
                  0, -0.1, 1, 0, 0.1, 1
                  };

unsigned int indices[] = {
    0, 1, 3, 2,
    4, 5, 7, 6,
    6, 7, 9, 8,
//R
    10, 11, 13, 12,
    12, 13, 15, 14,
    14, 15, 17, 16,
    16, 17, 19, 18,
    20, 21, 23, 22,
    24, 25, 27, 26,
//L
    28, 29, 31, 30,
    30, 31, 33, 32,
    //32, 33, 35, 34,
//C
    34, 35, 37, 36,
    36, 37, 39, 38,
    38, 39, 41, 40,

};

unsigned int VBO, VAO, EBO;

void move(){
  int i;
  for (i = 0; i < 10; i++) {
    vertices[(i*3)] += -1.1;
  }

  for (i = 28; i < 34; i++) {
    vertices[(i*3)] += 1.1;
  }

  for (i = 34; i < 42; i++) {
    vertices[(i*3)] += 2.2;
  }
}

const char *vertexShaderSource = "#version 450 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 r;\n"
    "float escala = 0.4;"
    "float rx = r.x;"
    "float ry = r.y;"
    "mat3 Rx = mat3(vec3(1, 0, 0),\n"
    "              vec3(0, cos(rx), sin(rx)),\n"
    "              vec3(0, -sin(rx), cos(rx)));\n"
    "mat3 Ry = mat3(vec3(cos(ry), 0, -sin(ry)),\n"
    "               vec3(0, 1, 0),\n"
    "               vec3(-sin(ry), 0, cos(ry)));\n"
    "void main()\n"
    "{\n"
    "vec3 aPosf = Rx*Ry*aPos;\n"

    "   gl_Position = vec4(escala*aPosf.x, escala*aPosf.y, escala*aPosf.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";


void drawgraphix()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(VAO);
  glUseProgram(shaderProgram);
  glDrawElements(GL_QUADS, 58, GL_UNSIGNED_INT, 0);

  glutSwapBuffers();

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
			rotx += 5.0f*(x - mx)/winwidth;
			roty += 5.0f*(y - my)/winheight;
		}

		mx = x;
		my = y;
    glUseProgram(shaderProgram);
    glVertexAttrib2f(1, roty, -rotx);

		flag = 2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key == 'q' || key == 27) exit(0);
}

int main(int argc,char **argv)
{
  winwidth = winheight = 800;
  move();
  glutInit(&argc,argv);
  glutInitWindowSize(winwidth,winheight);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutCreateWindow("Practica 1 - Ejercicio 1.2");

  if (glewInit() != GLEW_OK)
  {
    cout << "failed initializing glew" << endl;
    return -1;
  }

  shadeInit ();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glutDisplayFunc(drawgraphix);
  glutMouseFunc(mousefunc);
  glutMotionFunc(motionfunc);
  glutKeyboardFunc(keyboardfunc);
  glutIdleFunc(drawgraphix);
  glutMainLoop();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return 0;
}

void shadeInit (){
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
