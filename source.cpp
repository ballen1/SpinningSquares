#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 640

#define VIEWPORT_EXTENT 50

#define STARTX_POS 100
#define STARTY_POS 100

#define MAX_RECTANGLES     15
#define MAX_RECT_DIMENSION 15
#define MIN_RECT_DIMENSION 3

#define ROTATE_SPEED        0.025
#define SCALE_SPEED         0.002
#define VELOCITY_MULTIPLIER 0.005

#define SCALE_AMPLITUDE 0.33
#define SCALE_MEDIAN    1.0

#define M_PI 3.14159265358979323846

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

  float movementX;
  float movementY;

  float scalingSeed;
  float scaleSpeed;

  float warpX;
  float warpY;

};

struct Point {

  float x;
  float y;

};

void display();
void glInit();
void reshape(int width, int height);
void generateRectangles();
void resetMatrices();
void handleInput(unsigned char key, int x, int y);
void playScene();
bool hasRectExitedScreen(Rectangle *rect, float scaleFactor);

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

    float scaleFactor = (SCALE_AMPLITUDE * cos(rects[i].scalingSeed)) + SCALE_MEDIAN;

    if (hasRectExitedScreen(&rects[i], scaleFactor))
    {
      rects[i].xOrigin = rects[i].warpX;
      rects[i].yOrigin = rects[i].warpY;
    }

    glPushMatrix();

    glTranslatef(rects[i].xOrigin + (rects[i].width/2.0), rects[i].yOrigin + (rects[i].height/2.0), 0.0);
    glRotatef(rects[i].rotation*rects[i].rotationDir, 0.0, 0.0, 1.0);
    glScalef(scaleFactor, scaleFactor, 1.0);
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
    case 'p':
      glutIdleFunc(NULL);
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
    rects[i].rotation += rects[i].rotationSpeed * ROTATE_SPEED;

    if (rects[i].rotation >= 360.0)
    {
      rects[i].rotation -= 360.0;
    }

    rects[i].scalingSeed += rects[i].scaleSpeed * SCALE_SPEED;

    rects[i].xOrigin += rects[i].movementX * VELOCITY_MULTIPLIER;
    rects[i].yOrigin += rects[i].movementY * VELOCITY_MULTIPLIER;
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

    rects[i].rotation = rand() % (361);

    ((rand() % 2) == 1) ? rects[i].rotationDir = 1 : rects[i].rotationDir = -1;
   
    rects[i].rotationSpeed = ((rand() % 15) + 1) / 10.0;

    rects[i].scalingSeed = rand();
    rects[i].scaleSpeed = ((rand() % 100) + 50) / 100.0;

    // Generate random movement vector components
    rects[i].movementX = (((rand() % 2) == 1) ? -1 : 1) * RAND_MAX;
    rects[i].movementY = (((rand() % 2) == 1) ? -1 : 1) * RAND_MAX;

    // Normalize each vector component
    float magnitude = sqrt(rects[i].movementX*rects[i].movementX + rects[i].movementY*rects[i].movementY);

    rects[i].movementX /= magnitude;
    rects[i].movementY /= magnitude;
    
    // Set up the warp points by walking the movement vector
    rects[i].warpX = rects[i].xOrigin;
    rects[i].warpY = rects[i].yOrigin;

    while (rects[i].warpX < VIEWPORT_EXTENT && (rects[i].warpX + rects[i].width) > -VIEWPORT_EXTENT
	   && rects[i].warpY < VIEWPORT_EXTENT && (rects[i].warpY + rects[i].height) > -VIEWPORT_EXTENT)
    {
	rects[i].warpX += -rects[i].movementX;
	rects[i].warpY += -rects[i].movementY;
    }

  }

}

bool hasRectExitedScreen(Rectangle *rect, float scaleFactor)
{ 

  bool hasExited = false;
  
  Point rectPoints[4];
  Point rectPoints_new[4];

  rectPoints[0].x = rect->xOrigin;
  rectPoints[0].y = rect->yOrigin;

  rectPoints[1].x = rect->xOrigin + rect->width;
  rectPoints[1].y = rect->yOrigin;

  rectPoints[2].x = rect->xOrigin;
  rectPoints[2].y = rect->yOrigin + rect->height;

  rectPoints[3].x = rect->xOrigin + rect->width;
  rectPoints[3].y = rect->yOrigin + rect->height;

  // Convert Rotation into radians
  float rotation = (rect->rotation * M_PI) / 180.0;

  float xCenter = rect->xOrigin + (rect->width/2.0);
  float yCenter = rect->yOrigin + (rect->height/2.0);

  int pointsOutsideBoundary = 0;

  for (int i = 0; i < 4; i++)
  {

    rectPoints[i].x -= xCenter;
    rectPoints[i].y -= yCenter;

    rectPoints[i].x *= scaleFactor;
    rectPoints[i].y *= scaleFactor;

    rectPoints_new[i].x = rectPoints[i].x * cos(rotation) - rectPoints[i].y * sin(rotation);
    rectPoints_new[i].y = rectPoints[i].y * cos(rotation) + rectPoints[i].x * sin(rotation);

    rectPoints_new[i].x += xCenter;
    rectPoints_new[i].y += yCenter;

    if (labs(rectPoints_new[i].x) >= VIEWPORT_EXTENT || labs(rectPoints_new[i].y) >= VIEWPORT_EXTENT)
    {
      pointsOutsideBoundary += 1;
    }

  }

  if (pointsOutsideBoundary == 4)
  {
    hasExited = true;
  }

  return hasExited;

}
