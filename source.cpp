#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define VIEWPORT_EXTENT 50

#define STARTX_POS 100
#define STARTY_POS 100

#define MAX_RECTANGLES 15
#define MAX_RECT_DIMENSION 15
#define MIN_RECT_DIMENSION 3

#define SIM_SPEED 0.025

struct Rectangle {

  float xOrigin;
  float yOrigin;
  float width;
  float height;
 
  unsigned char r;
  unsigned char g;
  unsigned char b;

  float rotation;
  float rotationSpeed;
  int rotationDir;

  float vectorX;
  float vectorY;

  float scalingSeed;
  float scaleSpeed;
};

void display();
void glInit();
void reshape(int width, int height);
void generateRectangles();
void resetMatrices();
void handleInput(unsigned char key, int x, int y);
void playScene();

Rectangle rects[MAX_RECTANGLES];

int main (int argc, char* argv[])
{
 
  glutInit(&argc, argv);
  glInit();

  generateRectangles();

  glutMainLoop();

  return 0;

}

void display()
{

  glClear(GL_COLOR_BUFFER_BIT);

  for (int i = 0; i < MAX_RECTANGLES; i++)
  {
    glPushMatrix();
    glTranslatef(rects[i].xOrigin + (rects[i].width/2.0), rects[i].yOrigin + (rects[i].height/2.0), 0.0);
    glScalef(cos(rects[i].scalingSeed)+1, cos(rects[i].scalingSeed)+1, 1.0);
    //    glRotatef(rects[i].rotation, 0.0, 0.0, rects[i].rotationDir);
     glTranslatef(-(rects[i].xOrigin + (rects[i].width/2.0)), -(rects[i].yOrigin + (rects[i].height/2.0)), 0.0);
    glColor3ub(rects[i].r, rects[i].g, rects[i].b);
    glRectf(rects[i].xOrigin, rects[i].yOrigin, rects[i].xOrigin+rects[i].width, rects[i].yOrigin+rects[i].height);
    glPopMatrix();
  }

  glutSwapBuffers();

}

void glInit()
{

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(STARTX_POS, STARTY_POS);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glutCreateWindow("Assignment 1");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(handleInput);

}

void reshape(int width, int height)
{

  glViewport(0, 0, width, height);
  resetMatrices();

}

void resetMatrices()
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-VIEWPORT_EXTENT, VIEWPORT_EXTENT, -VIEWPORT_EXTENT, VIEWPORT_EXTENT, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void handleInput(unsigned char key, int x, int y)
{
	
  switch (key)
  {
    case 's':
      glutIdleFunc(playScene);
      break;
    case 'q':
      exit(0);
      break;
    default:
      break;

  }
}

void playScene()
{

  for (int i = 0; i < MAX_RECTANGLES; i++)
  {
    rects[i].rotation += rects[i].rotationSpeed * SIM_SPEED;
    rects[i].scalingSeed += rects[i].scaleSpeed * SIM_SPEED;
    printf("Scaling Seed: %f\n", rects[i].scalingSeed);
  }

  glutPostRedisplay();
}

void generateRectangles()
{
  
  srand(time(NULL));

  for (int i = 0; i < MAX_RECTANGLES; i++)
  {
    rects[i].xOrigin = rand() % (VIEWPORT_EXTENT*2 + 1) - VIEWPORT_EXTENT;
    rects[i].yOrigin = rand() % (VIEWPORT_EXTENT*2 + 1) - VIEWPORT_EXTENT;
    
    rects[i].width  = rand() % (MAX_RECT_DIMENSION + 1 - MIN_RECT_DIMENSION) + MIN_RECT_DIMENSION;
    rects[i].height = rand() % (MAX_RECT_DIMENSION + 1 - MIN_RECT_DIMENSION) + MIN_RECT_DIMENSION;
    
    rects[i].r = rand() % 256;
    rects[i].g = rand() % 256;
    rects[i].b = rand() % 256;

    rects[i].rotation = rand() % (360 + 1);

    ((rand() % 2) == 1) ? rects[i].rotationDir = 1 : rects[i].rotationDir = -1;
   
    rects[i].rotationSpeed = (rand() % (10 + 1)) / 10.0;

    rects[i].scalingSeed = rand();
    rects[i].scaleSpeed = ((rand() % 10) + 5) / 100.0;

  }

}
