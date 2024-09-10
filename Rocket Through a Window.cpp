/*******************************************
  TARIK ANAFARTA

  Additional Functions:

  The tip of the rocket is heating while it goes upwards.
  Left Mouse Click to Close Curtain (Click on the Window)
  Right Mouse Click to Change the Galaxy
  Press Right Arrow to Move Right
  Press Left Arrow to Move Left
  Hold Down Arrow to Slow the Rocket
  Hold Up Arrow to Speed the Rocket
  Press Space to Stop the Rocket
*******************************************/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 900
#define TIMER_PERIOD   20   // Period for the timer

#define PI 3.1415

// Global variables
bool TIMER_ON = true, curtain1 = false, curtain2 = false;
int  winWidth, winHeight; // Current Window width and height
int r = 244, g = 160, b = 7; // Initial color
int xR = 0, yR = 0, xB = 0, yB = 0, yB2 = 0, yB3 = 0; //Rocket's and curtains' initial positions

// To draw a filled circle, centered at (x,y) with radius r
void circle(int x, int y, int r) {
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

// To display text with variables
void vprint(int x, int y, void* font, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

void drawRocket() {
	
	// Background
	glLineWidth(2);
	glBegin(GL_QUADS);
	glColor3b(r, g, b);
	glVertex2f(-winWidth / 2 - 600, winHeight / 2 + 500);
	glColor3f(0, 0, 0.13);
	glVertex2f(-winWidth / 2 + 60, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 60, -winHeight / 2 + 60);
	glColor3f(0, 0, 0.25);
	glVertex2f(winWidth / 2 - 60, winHeight / 2 - 60);
	glEnd();

	for (int i = 0; i < winWidth/30 - 3; i++) // Stars
	{
		for (int j = 0; j <= winHeight/40 ; j++)
		{
			glPointSize(2);
			glBegin(GL_POINTS);
			glColor3f(1, 1, 1);
			glVertex2f(-winWidth / 2 + 70 + i * 30, winHeight / 2 - 65 - j * 40);
			glEnd();
		}
	}

	// Part 1: Body
	glColor3f(0.3, 0.3, 0.3); //Gray
	glBegin(GL_QUADS);
	glVertex2f(-60 * .8 + xB, -360 * .8 + yB);
	glVertex2f(-60 * .8 + xB, -90 * .8 + yB);
	glVertex2f(60 * .8 + xB, -90 * .8 + yB);
	glVertex2f(60 * .8 + xB, -360 * .8 + yB);
	glEnd();

	// Part 2: Heating Tip
	glColor3f(1, 1 - fabs(yB)/512, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(0 * .8 + xB, 0 + yB);
	glVertex2f(-60 * .8 + xB, -90 * .8 + yB);
	glVertex2f(60 * .8 + xB, -90 * .8 + yB);
	glEnd();

	// Part 3 & 4: Right and Left Wings
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(-120 * .8 + xB, -360 * .8 + yB);
	glVertex2f(-60 * .8 + xB, -245 * .8 + yB);
	glVertex2f(-60 * .8 + xB, -360 * .8 + yB);
	glVertex2f(60 * .8 + xB, -360 * .8 + yB);
	glVertex2f(60 * .8 + xB, -245 * .8 + yB);
	glVertex2f(120 * .8 + xB, -360 * .8 + yB);
	glEnd();

	// Part 5: Third Wing
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0 + xB, -245 * .8 + yB);
	glVertex2f(0 + xB, -360 * .8 + yB);
	glEnd();

	// Part 6: Exhauster
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(-60 * .8 + xB, -360 * .8 + yB);
	glVertex2f(-45 * .8 + xB, -400 * .8 + yB);
	glVertex2f(45 * .8 + xB, -400 * .8 + yB);
	glVertex2f(60 * .8 + xB, -360 * .8 + yB);
	glEnd();

	// Part 7: Flames
	glColor3f(1, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-45 * .8 + xB, -400 * .8 + yB);
	glVertex2f(-30 * .8 + xB, -450 * .8 + yB);
	glVertex2f(-20 * .8 + xB, -425 * .8 + yB);
	glVertex2f(0 * .8 + xB, -500 * .8 + yB);
	glVertex2f(20 * .8 + xB, -425 * .8 + yB);
	glVertex2f(30 * .8 + xB, -450 * .8 + yB);
	glVertex2f(45 * .8 + xB, -400 * .8 + yB);
	glEnd();

	// Part 8: Windows
	glColor3b(0, 0, 100);
	circle(0 + xB, -120 + yB, 25);

	glColor3b(25, 25, 112);
	circle(0 + xB, -120 + yB, 20);

	glColor3b(67, 33, 7); // Upper Handle Background
	glBegin(GL_QUADS);
	glVertex2f(-winWidth / 2 + 32, 30);
	glVertex2f(-winWidth / 2 + 32, 0);
	glVertex2f(winWidth / 2 - 32, 0);
	glVertex2f(winWidth / 2 - 32, 30);

	glColor3b(67, 33, 7); // Down Handle Background
	glBegin(GL_QUADS);
	glVertex2f(-winWidth / 2 + 32, 0);
	glVertex2f(-winWidth / 2 + 32, -30);
	glVertex2f(winWidth / 2 - 32, -30);
	glVertex2f(winWidth / 2 - 32, 0);
	glEnd();

	glColor3b(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-30, 13);
	glVertex2f(30, 13);
	glVertex2f(-30, -13);
	glVertex2f(30, -13);
	glEnd();

	glColor3b(0, 0, 0);
	circle(-30, 13, 4);
	circle(30, 13, 4);
	circle(-30, -13, 4);
	circle(30, -13, 4);

	//Inner Window Frames
	glColor3b(67, 33, 7);
	glBegin(GL_QUADS);
	glVertex2f(-winWidth / 2 + 30, winHeight / 2 - 30);
	glVertex2f(-winWidth / 2 + 30, winHeight / 2 - 60);
	glVertex2f(winWidth / 2 - 30, winHeight / 2 - 60);
	glVertex2f(winWidth / 2 - 30, winHeight / 2 - 30);

	glVertex2f(winWidth / 2 - 60, winHeight / 2 - 60);
	glVertex2f(winWidth / 2 - 60, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 30, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 30, winHeight / 2 - 60);

	glVertex2f(winWidth / 2 - 30, -winHeight / 2 + 30);
	glVertex2f(-winWidth / 2 + 30, -winHeight / 2 + 30);
	glVertex2f(-winWidth / 2 + 30, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 30, -winHeight / 2 + 60);

	glVertex2f(-winWidth / 2 + 30, winHeight / 2 - 30);
	glVertex2f(-winWidth / 2 + 30, -winHeight / 2 + 60);
	glVertex2f(-winWidth / 2 + 60, -winHeight / 2 + 60);
	glVertex2f(-winWidth / 2 + 60, winHeight / 2 - 30);

	glColor3f(0.171875, 0.0859375, 0.01953125);
	glVertex2f(-winWidth / 2, winHeight / 2);
	glVertex2f(-winWidth / 2, winHeight / 2 - 30);
	glVertex2f(winWidth / 2, winHeight / 2 - 30);
	glVertex2f(winWidth / 2, winHeight / 2);

	glVertex2f(-winWidth / 2, -winHeight / 2 + 30);
	glVertex2f(-winWidth / 2, -winHeight / 2);
	glVertex2f(winWidth / 2, -winHeight / 2);
	glVertex2f(winWidth / 2, -winHeight / 2 + 30);

	glVertex2f(-winWidth / 2, winHeight / 2);
	glVertex2f(-winWidth / 2, -winHeight / 2);
	glVertex2f(-winWidth / 2 + 30, -winHeight / 2);
	glVertex2f(-winWidth / 2 + 30, winHeight / 2);
	glEnd();

	glColor3f(0, 0, 0); // Upper Background Lines
	glBegin(GL_LINE_LOOP);
	glVertex2f(-winWidth / 2 + 60, winHeight / 2 - 60);
	glVertex2f(-winWidth / 2 + 60, 0);
	glVertex2f(winWidth / 2 - 60, 0);
	glVertex2f(winWidth / 2 - 60, winHeight / 2 - 60);
	glEnd();

	glColor3f(0, 0, 0); // Down Background Lines
	glBegin(GL_LINE_LOOP);
	glVertex2f(-winWidth / 2 + 60, 0);
	glVertex2f(-winWidth / 2 + 60, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 60, -winHeight / 2 + 60);
	glVertex2f(winWidth / 2 - 60, 0);
	glEnd();

	glColor3f(1, 0.02, 0.03); //Upper Curtain
	glBegin(GL_QUADS);
	glVertex2f(-winWidth / 2 + 61, winHeight / 2 - 61);
	glVertex2f(-winWidth / 2 + 61, winHeight / 2 - 61 - yB2);
	glVertex2f(winWidth / 2 - 61, winHeight / 2 - 61 - yB2);
	glVertex2f(winWidth / 2 - 61, winHeight / 2 - 61);
	glEnd();

	glColor3f(1, 0.02, 0.03); //Down Curtain
	glBegin(GL_QUADS);
	glVertex2f(-winWidth / 2 + 61, -winHeight / 2 + 61 + yB3);
	glVertex2f(-winWidth / 2 + 61, -winHeight / 2 + 61);
	glVertex2f(winWidth / 2 - 61, -winHeight / 2 + 61);
	glVertex2f(winWidth / 2 - 61, -winHeight / 2 + 61 + yB3);
	glEnd();
	
	for (int i = 0; i <= 100; i++) // Wall Pattern (Big Dots)
	{
		glPointSize(9);
		glBegin(GL_POINTS);
		glColor3f(0.8, 0.55, 0.5); // Pink
		glVertex2f(-winWidth / 2 + i * 20, winHeight / 2 - 15);
		glVertex2f(-winWidth / 2 + i * 20, -winHeight / 2 + 15);
		glVertex2f(-winWidth / 2 + 15, winHeight / 2 - i * 20);
		glVertex2f(winWidth / 2 - 15, winHeight / 2 - i * 20);
		glEnd();
	}

	for (int i = 0; i <= 100; i++) // Wall Pattern (Small Dots)
	{
		glPointSize(4);
		glBegin(GL_POINTS);
		glColor3f(1, 1, 0); // Yellow
		glVertex2f(-winWidth / 2 + i * 20, winHeight / 2 - 15);
		glVertex2f(-winWidth / 2 + i * 20, -winHeight / 2 + 15);
		glVertex2f(-winWidth / 2 + 15, winHeight / 2 - i * 20);
		glVertex2f(winWidth / 2 - 15, winHeight / 2 - i * 20);
		glEnd();
	}

	glBegin(GL_LINE_LOOP); // Label
	glColor3f(1, 0, 0);
	glVertex2f(-70, winHeight / 2 - 34);
	glColor3f(0, 1, 0);
	glVertex2f(-70, winHeight / 2 - 56);
	glColor3f(0, 0, 1);
	glVertex2f(70, winHeight / 2 - 56);
	glColor3f(1, 1, 1);
	glVertex2f(70, winHeight / 2 - 34);
	glEnd();
}

// To display onto window using OpenGL commands
void display() {

	// Clear window to Brown
	glClearColor(0.171875, 0.0859375, 0.01953125, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawRocket();

	glColor3f(1, 1, 1); // White
	vprint(-55, winHeight / 2 - 48, GLUT_BITMAP_8_BY_13, "Tarik Anafarta"); // Name and Surname

	glutSwapBuffers();
}

// Key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y) {

	// Stop the animation when SPACE is pressed.
	if (key == 32)
		TIMER_ON = !TIMER_ON;

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyDown(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT) //left
		xB -= 5;
	if (key == GLUT_KEY_RIGHT) //right
		xB += 5;
	if (key == GLUT_KEY_DOWN) //slower
		yB -= 0.5;
	if (key == GLUT_KEY_UP) //faster
		yB += 5;

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) // Random Colors
	{
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) // Curtains' hitboxes
	{
		xR = x - winWidth / 2;
		yR = winHeight / 2 - y;
		if (xR >= -winWidth / 2 + 60 && xR <= winWidth / 2 + 60 && yR <= winHeight / 2 + 60 && yR >= 30)
			curtain1 = !curtain1;
		if (xR >= -winWidth / 2 + 60 && xR <= winWidth / 2 + 60 && yR <= -30 && yR >= -winHeight / 2 + 60)
			curtain2 = !curtain2;
	}

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void onResize(int w, int h) {
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // Refresh window
}

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (TIMER_ON)
	{
		if (yB < (winHeight / 2 + 400)) //Animate rocket until it touches the top edge of the window. winHeight is the current height of the window.
			yB++;
		else
			yB = -winHeight / 2;
		
		//to refresh the window it calls display() function
		glutPostRedisplay();
	}

	if (curtain1) // Closing the up curtain
		if (yB2 < winHeight / 2 - 90)
			yB2 += 10;
	if (!curtain1) // Opening the up curtain
		if (yB2 > 0)
			yB2 -= 10;

	if (curtain2) // Closing the down curtain
		if (yB3 < winHeight / 2 - 90)
			yB3 += 10;
	if (!curtain2) // Opening the down curtain
		if (yB3 > 0)
			yB3 -= 10;

	//to refresh the window it calls display() function
	glutPostRedisplay();
}

void init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(500, 50); // Window's Initial Position
	glutCreateWindow("CTIS164 - Homework 1"); // Meaningful Window Title

	// Window Events
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// Keyboard Events
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	// Mouse Events
	glutMouseFunc(onClick);

	// if (TIMER_ON) - Timer Event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	// Initialize random generator
	srand(time(0));

	init();
	glutMainLoop();
	return 0;
}