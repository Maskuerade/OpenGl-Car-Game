// This code creates simple camera movements (forward, backward, right, left, rotateLeft, rotateRight) using gluLookAt as well as simple collision detection

#include <iostream>

#include <glew.h>
#include <freeglut.h>
#include <glm/glm.hpp>

static float speed = 1.0;   // a float to determine the speed of camera motion
static float angleSpeed = 5.0 * M_PI / 180.0;   // determines the speed/test of camera rotation
static float angle = 0.0;  // angle of camera rotation measured counter-clockwise with the -ve z axis

// defining the eye, center and up vectors
glm::vec3 eye = glm::vec3(0.0, 0.0, 15);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

// function for drawing bitmapped text
void writeBitmapString(void *font, char *string)
{
char *c;
for (c = string; *c != '\0'; c++)
    glutBitmapCharacter(font, *c);
}

// simple collision detection
void detectCollision(void)
{
    // measuring distance between the centers of the two spheres
    // If the distance is equal to or less than the sum of their radii, they collided
    float distance = glm::length(center - glm::vec3(10.0, 0.0, -5.0));
    if (distance <= 4.0)
    {
        // drawing collision text message
        glRasterPos3f(0.0, 5.0, -10.0);
        writeBitmapString((long*)GLUT_BITMAP_8_BY_13, (char*)"You Collided !");
    }
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z) ;
        
    // the first sphere: it moves along with the camera, as if we're driving it
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);
    glutSolidSphere(2.0, 15.0, 15.0);
    glPopMatrix();
       
    // The second sphere: drawn at center (0,0,-5)
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(10.0, 0.0, -5.0);
    glutSolidSphere(2.0, 15, 15);

    // check if the two spheres collide
    detectCollision();
    
    glutSwapBuffers();
}

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


// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
   
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    
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



// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("boxWithLookAt.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}

