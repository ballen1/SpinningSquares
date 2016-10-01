#include <GL/glut.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define VIEWPORT_EXTENT 50

#define STARTX_POS 100
#define STARTY_POS 100

#define MAX_RECTANGLES 15

#define SIM_SPEED 5

struct Rectangle {
  float xOrigin;
  float yOrigin;
  float width;
  float height;
};

void display();
void glInit();
void reshape(int width, int height);
void generateRectangles(Rectangle* rects);
void resetMatrices();
void handleInput();

int main (int argc, char* argv[])
{
 
  Rectangle rects[MAX_RECTANGLES];
 
  glutInit(&argc, argv);
  glInit();

  generateRectangles(rects);

  glutMainLoop();

  return 0;

}

void display()
{

  glClear(GL_COLOR_BUFFER_BIT);
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

}

void reshape(int width, int height)
{

  glViewport(0, 0, width, height);
  resetMatrices();

}

void resetMatrices()
{

  glMatrixMode(GL_PROJECTION);
  glOrtho(-VIEWPORT_EXTENT, VIEWPORT_EXTENT, -VIEWPORT_EXTENT, VIEWPORT_EXTENT, -1.0, 1.0);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void handleInput()
{	

}

void generateRectangles(Rectangle* rects)
{
  for (int i = 0; i < MAX_RECTANGLES; i++)
  {
    
  }
}
