#include <glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <glm/glm.hpp>


// Globals.
static unsigned int Boxes; // List index.
static unsigned int space_ship; // List index.

GLfloat LightColor[][3]={1,1,0,   0,1,1,   0,1,0};
GLint CI=0;

// defining the eye, center and up vectors
glm::vec3 eye = glm::vec3(0.0, 0.0, 15);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

static float speed = 1.0;   // a float to determine the speed of camera motion
static float angleSpeed = 5.0 * M_PI / 180.0;   // determines the speed/test of camera rotation
static float angle = 0.0;  // angle of camera rotation measured counter-clockwise with the -ve z axis

// Car position
float carX = 0.0f;
float carY = 0.0f;

// Target position
float targetX = 8.0f;
float targetY = 8.0f;

// Obstacle positions
float obstacle1X = 2.0f;
float obstacle1Y = 2.0f;
float obstacle2X = -4.0f;
float obstacle2Y = -6.0f;
float obstacle3X = 6.0f;
float obstacle3Y = -4.0f;
float obstacle4X = -8.0f;
float obstacle4Y = 6.0f;

// Game state
bool gameOver = false;
bool gameWon = false;

// to move the camera in the forward direction, we simply increment the eye and center vectors in that direction. Here we chose that to be to the front, down the z axis.
void moveForward(void)
{
    glm::vec3 direction = glm::normalize(center - eye);
    eye += speed * direction;
    center += speed * direction;
}

// a change in sign moves you to the opposite direction .
void moveBackward(void)
{
    glm::vec3 direction = glm::normalize(center - eye);
    eye -= speed * direction;
    center -= speed * direction;
}

// cross product of the line of sight vector with the up vector gives us the direction pointing to the right of the camera.
// the eye and center are incremented in that direction
void moveRight(void)
{
    glm::vec3 rightDir = glm::cross(glm::normalize(center-eye), up);
    eye += speed * rightDir;
    center += speed * rightDir;
}

void moveLeft(void)
{
    glm::vec3 rightDir = glm::cross(glm::normalize(center-eye), up);
    eye -= speed * rightDir;
    center -= speed * rightDir;
}

// rotate the camera about the eye.
// to do that, we rotate the center on the perimeter of a circle, whose center is the eye and radius is the distance from the eye to the center
// Here we measured the angle of rotation counter-clockwise with the -ve z axis.
void rotateLeft(void)
{
    float distance = glm::length(center - eye);
    angle += angleSpeed;
    center.x = eye.x - distance * sin(angle);
    center.z = eye.z - distance * cos(angle);
}

void rotateRight(void)
{
    float distance = glm::length(center - eye);
    angle -= angleSpeed;
    center.x = eye.x - distance * sin(angle);
    center.z = eye.z - distance * cos(angle);
}



// Function to handle key presses
void handleKeypress(unsigned char key, int x, int y) {
    if (gameOver) {
        // Reset the game if it's over
        carX = 0.0f;
        carY = 0.0f;
        gameOver = false;
        gameWon = false;
    }
    else {
        // Move the car based on the key pressed
        switch (key)
        {
            case 27:
                exit(0);
                break;
            case 'w':
                carY += 1.0f;
                moveForward();
                glutPostRedisplay();
                break;
            case 's':
                carY -= 1.0f;
                moveBackward();
                glutPostRedisplay();
                break;
            case 'd':
                carX += 1.0f;
                moveRight();
                glutPostRedisplay();
                break;
            case 'a':
                carX -= 1.0f;
                moveLeft();
                glutPostRedisplay();
                break;
            case 'q':
                rotateLeft();
                glutPostRedisplay();
                break;
            case 'e':
                rotateRight();
                glutPostRedisplay();
                break;
            default:
                break;
        }

        // Check for collision with obstacles
        if (carX == obstacle1X && carY == obstacle1Y ||
            carX == obstacle2X && carY == obstacle2Y ||
            carX == obstacle3X && carY == obstacle3Y ||
            carX == obstacle4X && carY == obstacle4Y) {
            gameOver = true;
        }

        // Check for reaching the target
        if (carX == targetX && carY == targetY) {
            gameWon = true;
        }
    }

    glutPostRedisplay();
}

// Function to draw the scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z) ;
    // Draw the car
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(carX, carY, center.z);
    glutSolidCube(1.0f);

    // Draw the target
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(5.0, 6.0, 0.0f);
    glCallList(Boxes); // Execute display list.
    glPopMatrix();

    // Draw the obstacles
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(10.0, 0.0, -5.0);
    glCallList(Boxes); // Execute display list.
    glPopMatrix();



    // Display game over or game won message
    if (gameOver) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-1.0f, 0.0f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"You Lose !");
    }
    else if (gameWon) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-1.0f, 0.0f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"You Win !");
    }

    glFlush();
}

// Function to handle window resizing
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
}

// Initialization routine.
void setup(void)
{
    space_ship = glGenLists(1); // Return a list index.

    // Begin create a display list

    // Begin create a display list.
    Boxes = glGenLists(1); // Return a list index.
    glNewList(Boxes, GL_COMPILE);
    glutSolidCube(1.0f);
    glEndList();
    // End create a display list.
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Car Game");

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);

    setup();

    glutMainLoop();

    return 0;
}