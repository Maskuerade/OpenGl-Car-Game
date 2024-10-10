///////////////////////////////////////////////////////////////////////////////////
// This program draws a conical car that can travel and an array of fixed spherical asteroids.
// The view in the left viewport is from a fixed camera; the view in the right viewport is from the car.
// There is approximate collision detection.
// 
// User-defined constants: 
// ROW is the number of rows of asteroids.
// COLUMN is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
//
// cr. code: Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////


// This code simply implements simplified collision detection with a group of objects.
// You can refer to section 4.6.4 in reference: Sumanta Guha - Computer Graphics through OpenGL


#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <glew.h>
#include <freeglut.h> 

#define ROWS 10  // Number of rows of asteroids.
#define COLUMNS 5 // Number of columns of asteroids.
#define FILL_PROBABILITY 50 // Percentage probability that a particular row-column slot will be
                             // filled with an asteroid. It should be an integer between 0 and 100.

#define SIZE 10 // Size of each obstacle.

// Globals.
static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24; // Larger font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the car.
static float xVal = 0, zVal = 120; // Co-ordinates of the car.
static int isCollision = 0; // Is there collision between the car and an asteroid?
static unsigned int car; // Display lists base index.
static int frameCount = 0; // Number of frames

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Asteroid class.
class Asteroid
{
public:
	Asteroid();
	Asteroid(float x, float y, float z, float r, unsigned char colorR,
		unsigned char colorG, unsigned char colorB);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	void draw();

private:
	float centerX, centerY, centerZ, radius;
	unsigned char color[3];
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
	radius = 0.0; // Indicates no asteroid exists in the position.
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR,
	unsigned char colorG, unsigned char colorB)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

// Function to draw asteroid.
void Asteroid::draw()
{
	if (radius > 0.0) // If asteroid exists.
	{
		glPushMatrix();
		glTranslatef(centerX, centerY, centerZ);
		glColor3ubv(color);
        glutSolidCube(radius);
		glPopMatrix();
	}
}

Asteroid arrayAsteroids[ROWS][COLUMNS]; // Global array of asteroids.

// Routine to count the number of frames drawn every second.
void frameCounter(int value)
{
   if (value != 0) // No output the first time frameCounter() is called (from main()).
	  std::cout << "FPS = " << frameCount << std::endl;
   frameCount = 0;
   glutTimerFunc(1000, frameCounter, 1);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
                             float x2, float y2, float z2, float r2)
{
    return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
}

// Function to draw the hitbox of the car.
void drawHitbox(float x, float y, float z, float radius)
{
    glColor3f(1.0, 0.0, 0.0); // Set the color to red.

    // Draw the sphere.
    glPushMatrix();
    glTranslatef(x, y, z);
    glutWireSphere(radius, 16, 16);
    glPopMatrix();
}

int CarCraftCollision(float x, float z, float a)
{
    int i, j;

    // Check for collision with each asteroid.
    for (j = 0; j<COLUMNS; j++)
        for (i = 0; i<ROWS; i++)
            if (arrayAsteroids[i][j].getRadius() > 0) // If asteroid exists.
                if (checkSpheresIntersection(x - 5 * sin((M_PI / 180.0) * a), 0.0,
                                             z - 5 * cos((M_PI / 180.0) * a), 1.5,
                                             arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
                                             arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius()))
                    return 1;
    return 0;
}

// Function to check if the car is off the track.
int isOffTrack(float x, float angle)
{
    // Declare the track boundaries.
    float trackLeftBoundary;
    float trackRightBoundary;

    // Check if the car's x-coordinate is outside the track boundaries.
    if (angle<=30 && angle>= -30){
        trackLeftBoundary = -40.0-SIZE;
        trackRightBoundary = 40.0+SIZE;
    }else{
        trackLeftBoundary = -40.0;
        trackRightBoundary = 40.0;
    }

    // Check if the car's x-coordinate is outside the track boundaries.
    if (x < trackLeftBoundary || x > trackRightBoundary)
    {
        return 1;
    }

    return  0;
}

// Function to check if two cubes intersect.
int checkCubesIntersection(float x1, float y1, float z1, float s1,
                           float x2, float y2, float z2, float s2)
{
    return (abs(x1 - x2) * 2 < (s1 + s2)) &&
           (abs(y1 - y2) * 2 < (s1 + s2)) &&
           (abs(z1 - z2) * 2 < (s1 + s2));
}

// Initialization routine.
void setup(void)
{
    int i, j, k, l;

    car = glGenLists(1);
    glNewList(car, GL_COMPILE);
    //car body
    glPushMatrix();
    glColor3f(41.0 / 255.0, 60.0 / 255.0, 139.0 / 255.0);
    glTranslatef(0.0, 0.0, -2.0);
    glScalef(1.88, 0.5, 0.8); // Scale down the car.
    glutSolidCube(10.0);
    glPopMatrix();

    //car roof
    glPushMatrix();
    glColor3f(43.0 / 255.0, 62.0 / 255.0, 130 / 255.0);
    glTranslatef(-2.0, 5.0, -2.0);
    glScalef(1.5, 0.8, 1.0); // Scale down the car part.
    glutSolidCube(5.0);
    glPopMatrix();

    //car side windows
    glPushMatrix();
    glColor3f(106.0 / 255.0, 178.0 / 255.0, 197.0 / 255.0);
    glTranslatef(0.0, 5.0, -2.0);
    glScalef(0.6, 0.7, 1.1); // Scale down the car part.
    glutSolidCube(5.0);
    glPopMatrix();


    //car side windows
    glPushMatrix();
    glColor3f(106.0 / 255.0, 178.0 / 255.0, 197.0 / 255.0);
    glTranslatef(-4.0, 5.0, -2.0);
    glScalef(0.6, 0.7, 1.1); // Scale down the car part.
    glutSolidCube(5.0);
    glPopMatrix();

    //car front window
    glPushMatrix();
    glColor3f(106.0 / 255.0, 178.0 / 255.0, 197.0 / 255.0);
    glTranslatef(0.0, 5.0, -2.0);
    glScalef(0.85, 0.7, 0.7); // Scale down the car part.
    glutSolidCube(5.0);
    glPopMatrix();


    //car back window
    glPushMatrix();
    glColor3f(106.0 / 255.0, 178.0 / 255.0, 197.0 / 255.0);
    glTranslatef(-3.88, 5.0, -2.0);
    glScalef(0.85, 0.7, 0.7); // Scale down the car part.
    glutSolidCube(5.0);
    glPopMatrix();

    //car wheels
    glPushMatrix();
    glColor3f(82.0 / 255.0, 76.0 / 255.0, 82.0 / 255.0);
    glTranslatef(-5.0, -2.5, 0.5);
    glScalef(0.5, 0.5, 0.5); // Scale down the car part.
    glutSolidCylinder(5.0, 5.0, 16, 16);
    glPopMatrix();


    glPushMatrix();
    glColor3f(82.0 / 255.0, 76.0 / 255.0, 82.0 / 255.0);
    glTranslatef(5.0, -2.5, 0.5);
    glScalef(0.5, 0.5, 0.5); // Scale down the car part.
    glutSolidCylinder(5.0, 5.0, 16, 16);
    glPopMatrix();


    glPushMatrix();
    glColor3f(82.0 / 255.0, 76.0 / 255.0, 82.0 / 255.0);
    glTranslatef(-5.0, -2.5, -7.0);
    glScalef(0.5, 0.5, 0.5); // Scale down the car part.
    glutSolidCylinder(5.0, 5.0, 16, 16);
    glPopMatrix();


    glPushMatrix();
    glColor3f(82.0 / 255.0, 76.0 / 255.0, 82.0 / 255.0);
    glTranslatef(5.0, -2.5, -7.0);
    glScalef(0.5, 0.5, 0.5); // Scale down the car part.
    glutSolidCylinder(5.0, 5.0, 16, 16);
    glPopMatrix();
    glEndList();

    // Initialize global arrayAsteroids.
    for (j = 0; j<COLUMNS; j++)
    {
        for (i = 0; i<ROWS; i++)
        {
            if (rand() % 100 < FILL_PROBABILITY)
            {
                float x, y, z;
                bool intersect;
                do
                {
                    intersect = false;
                    // Generate random positions for the new asteroid.
                    x = rand() % 60 - 30;
                    y = -2.0;
                    z = 80.0 - 30.0*i;

                    for (k = 0; k < i && !intersect; k++) {
                        for (l = 0; l < COLUMNS; l++) {
                            if (arrayAsteroids[k][l].getRadius() > 0) { // If asteroid exists.
                                if (checkSpheresIntersection(arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
                                                             arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius(),
                                                             arrayAsteroids[k][l].getCenterX(), arrayAsteroids[k][l].getCenterY(),
                                                             arrayAsteroids[k][l].getCenterZ(), arrayAsteroids[k][l].getRadius()))
                                {
                                    intersect = true;
                                }
                            }
                        }
                    }
                } while (intersect);

                // Position the asteroid at the generated position.
                arrayAsteroids[i][j] = Asteroid(x, y, z, SIZE,
                                                rand() % 256, rand() % 256, rand() % 256);
            }
        }
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
}

// Function to draw the track.
void drawTrack()
{
    // Draw the track.
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5); // Set the color to grey.
    glBegin(GL_QUADS);
    glVertex3f(-40.0-SIZE, -SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(40.0+SIZE, -SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(40.0+SIZE, -SIZE, 120.0);
    glVertex3f(-40.0-SIZE, -SIZE, 120.0);
    glEnd();
    glPopMatrix();

    // Draw the barriers.
    glColor3f(34.0/255,139.0/255,230.0/255); // Set the color to red.
    glBegin(GL_QUADS);
    glVertex3f(-40.0-SIZE, -SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(-40.0-SIZE, SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(-40.0-SIZE, SIZE, 140.0);
    glVertex3f(-40.0-SIZE, -SIZE, 140.0);
    glVertex3f(40.0+SIZE, -SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(40.0+SIZE, SIZE, -30.0 * ROWS - 20.0);
    glVertex3f(40.0+SIZE, SIZE, 130.0);
    glVertex3f(40.0+SIZE, -SIZE, 130.0);
    glEnd();

    // Draw the starting line as a series of black and white tiles.
    for (int i = -40-SIZE; i < 40+SIZE; i += 2)
    {
        if (i % 4 == 0)
            glColor3f(1.0, 1.0, 1.0); // Set the color to white.
        else
            glColor3f(0.0, 0.0, 0.0); // Set the color to black.

        glBegin(GL_QUADS);
        glVertex3f(i, -SIZE+1, 100.0);
        glVertex3f(i + 2, -SIZE+1, 100.0);
        glVertex3f(i + 2, -SIZE+1, 130.0); // Extend the z-coordinate.
        glVertex3f(i, -SIZE+1, 130.0); // Extend the z-coordinate.
        glEnd();
    }

    // Draw the finish line as a series of black and white tiles.
    for (int i = -40-SIZE; i < 40+SIZE; i += 2)
    {
        if (i % 4 == 0)
            glColor3f(1.0, 1.0, 1.0); // Set the color to white.
        else
            glColor3f(0.0, 0.0, 0.0); // Set the color to black.

        glBegin(GL_QUADS);
        glVertex3f(i, -SIZE+0.1, -30.0 * ROWS - 20.0);
        glVertex3f(i + 2, -SIZE+0.1, -30.0 * ROWS - 20.0);
        glVertex3f(i + 2, -SIZE+0.1, -30.0 * ROWS);
        glVertex3f(i, -SIZE+0.1, -30.0 * ROWS);
        glEnd();
    }

    // Draw start text.
    glColor3f(1.0, 1.0, 1.0); // Set text color to white.
    glRasterPos3f(-5.0, 10, 90.0); // Position for start text.
    const char *startText = "Start Race";
    for (int i = 0; startText[i] != '\0'; ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, startText[i]);
    }

    // Draw finish text.
    glRasterPos3f(-5.0, 10, -30.0 * ROWS - 30.0); // Position for finish text.
    const char *finishText = "Finish Line";
    for (int i = 0; finishText[i] != '\0'; ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, finishText[i]);
    }

}

void restartGame(int value)
{
    // Reset game state.
    xVal = 0;
    zVal = 120;
    angle = 0.0;
    isCollision = 0;

    // Restart the game.
    setup();
}

// Drawing routine.
void drawScene(void)
{
   frameCount++; // Increment number of frames every redraw.

   int i, j;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Begin left viewport.
   glViewport(0, 0, width / 2.0, height);
   glLoadIdentity();


    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    if (isCollision)
    {
        glRasterPos3f(-5.0, 0.0, -30.0); // Position for "GAMEOVER" text.
        writeBitmapString((void*)font, "GAMEOVER");
    }
    glPopMatrix();

    glPopMatrix();

    // third-person POV .
    gluLookAt(xVal, 30.0, zVal + 30.0, xVal, 0.0, zVal, 0.0, 1.0, 0.0);

   // Draw the track.
   drawTrack();

   // Draw all the asteroids in arrayAsteroids.
   for (j = 0; j<COLUMNS; j++)
      for (i = 0; i<ROWS; i++)
         arrayAsteroids[i][j].draw();

    // Draw car and hit-box.
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle+90, 0.0, 1.0, 0.0);
    glCallList(car);
    glPopMatrix();


    // Check if the car has crossed the finish line.
    if (zVal <= -30.0 * ROWS)
    {
        // Write text in isolated (i.e., before gluLookAt) translate block.
        glPushMatrix();
        glColor3f(0.0, 1.0, 0.0); // Set the color to green.
        glRasterPos3f(-5.0, 0.0, -40.0); // Position for "CONGRATULATIONS!" text.
        writeBitmapString((void*)font, "CONGRATULATIONS!");
        glPopMatrix();
        // Wait for 3 seconds before restarting the game.
        glutTimerFunc(3000, restartGame, 0);
    }

    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
//    drawHitbox(0.0, 0.0, 0.0, 10.0); // Assuming the radius of the car is 10.0
    glPopMatrix();
    // End left viewport.

    // Begin right viewport.
   glViewport(width / 2.0, 0, width / 2.0, height);
   glLoadIdentity();

   // Draw a vertical line on the left of the viewport to separate the two viewports
   glColor3f(1.0, 1.0, 1.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
   glVertex3f(-5.0, -5.0, -5.0);
   glVertex3f(-5.0, 5.0, -5.0);
   glEnd();
   glLineWidth(1.0);

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

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    if (isCollision)
    {
        // Position for "GAMEOVER" text relative to the camera's position and orientation.
        glRasterPos3f(xVal-2, 0.0, zVal-15); // Position for "GAMEOVER" text.
        writeBitmapString((void*)font, "GAMEOVER");
        // Wait for 3 seconds before restarting the game.

    }
    glPopMatrix();

   // Draw the track.
   drawTrack();


    // Check if the car has crossed the finish line.
    if (zVal <= -30.0 * ROWS)
    {
        // Write text in isolated (i.e., before gluLookAt) translate block.
        glPushMatrix();
        glColor3f(0.0, 1.0, 0.0); // Set the color to green.
        glRasterPos3f(xVal, 0.0, zVal-15); // Position for "CONGRATULATIONS!" text.
        writeBitmapString((void*)font, "CONGRATULATIONS!");
        glPopMatrix();
        // Wait for 3 seconds before restarting the game.
        glutTimerFunc(3000, restartGame, 0);
    }

   // Draw all the asteroids in arrayAsteroids.
   for (j = 0; j<COLUMNS; j++)
    for (i = 0; i<ROWS; i++)
     arrayAsteroids[i][j].draw();
   // End right viewport.

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	width = w;
	height = h;
}



// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
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
        tempxVal = xVal - sin(tempAngle * M_PI / 180.0);
        tempzVal = zVal - cos(tempAngle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_DOWN)
    {
        tempxVal = xVal + sin(tempAngle * M_PI / 180.0);
        tempzVal = zVal + cos(tempAngle * M_PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;

    // Move car to next position only if there will not be collision with an asteroid or off the track.
    if (!CarCraftCollision(tempxVal, tempzVal, tempAngle) && !isOffTrack(tempxVal, angle))
    {
        isCollision = 0;
        xVal = tempxVal;
        zVal = tempzVal;
        angle = tempAngle;
    }
    else {
        isCollision = 1;
        // Wait for 3 seconds before restarting the game.
        glutTimerFunc(3000, restartGame, 0);
    }

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the left/right arrow keys to turn the craft." << std::endl
		<< "Press the up/down arrow keys to move the craft." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("spaceTravel.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

