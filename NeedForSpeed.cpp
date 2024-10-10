
//importing the header files
#include <glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <glm/glm.hpp>

static float speed = 1.0;   // a float to determine the speed of camera motion
static float angleSpeed = 5.0 * M_PI / 180.0;   // determines the speed/test of camera rotation
static float angle = 0.0;  // angle of camera rotation measured counter-clockwise with the -ve z axis
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.

// defining the eye, center and up vectors
glm::vec3 eye = glm::vec3(0.0, 0.0, 0.2);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

// Drawing routine.
void drawScene(void)
{
    // clearing the buffer and setting the drawing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Fixed camera.
    gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidSphere(2.0, 15.0, 15.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(5.0, 4.0, 0.0);
    glutSolidSphere(4.0, 15.0, 15.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(1.0, 02.0, 5.0);
    glutSolidSphere(4.0, 15.0, 15.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(7.0, 8.0, 2.0);
    glutSolidSphere(4.0, 15.0, 15.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(3.0, 9.0, 10.0);
    glutSolidSphere(4.0, 15.0, 15.0);
    glPopMatrix();

    // Locate the camera at the tip of the cone and pointing in the direction of the cone.
    gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 10 * cos((M_PI / 180.0) * angle),
              xVal - 11 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 11 * cos((M_PI / 180.0) * angle),
              0.0,
              1.0,
              0.0);

    // execute the drawing
    glFlush();
}

// Initialization routine.
void setup(void)
{
    //the clearing color of the opengl window (background)
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    // drawing the entire window
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // setting the projection matrix
    glOrtho(-50.0, 50.0, -50.0, 50.0, 5.0, 250.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// to move the camera in the forward direction, we simply increment the eye and center vectors in that direction. Here we chose that to be to the front, down the z axis.
void moveForward(void)
{
    glm::vec3 direction = glm::normalize(center - eye);
    eye -= speed * direction;
    center -= speed * direction;
}

// a change in sign moves you to the opposite direction .
void moveBackward(void)
{
    glm::vec3 direction = glm::normalize(center - eye);
    eye += speed * direction;
    center += speed * direction;
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

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'w':
            moveForward();
            glutPostRedisplay();
            break;
        case 's':
            moveBackward();
            glutPostRedisplay();
            break;
        case 'd':
            moveRight();
            glutPostRedisplay();
            break;
        case 'a':
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
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

    // Compute next position.
    if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
    if (key == GLUT_KEY_UP)
    {
        tempxVal = xVal - sin(angle * M_PI / 180.0);
        tempzVal = zVal - cos(angle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_DOWN)
    {
        tempxVal = xVal + sin(angle * M_PI / 180.0);
        tempzVal = zVal + cos(angle * M_PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;
    glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv)
{

    glutInit(&argc, argv);	// initializes the FreeGLUT library.
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // wanting an OpenGL context to support a single-buffered frame, each pixel having red, green, blue and alpha values.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

    // set the initial size of the OpenGL window and the location of its top left corner on the computer screen
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Need For Speed");

    // callback routines � when the OpenGL window is to be drawn, when it is resized,
    // and when keyboard input is received, respectively
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);


    // initializes GLEW (the OpenGL Extension Wrangler Library) which handles the loading of OpenGL extensions,
    // with the switch set so that extensions implemented even in pre-release drivers are exposed.
    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    // begins the event-processing loop, calling registered callback routines as needed
    glutMainLoop();

}