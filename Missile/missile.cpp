/*
* Napisati kratko uputstvo za koriscenje igrice komentar
*
*/
#include "missile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Glut.h"
#include <iostream>
#include <list>
#include <math.h>
#include <Windows.h>
#include <Mmsystem.h>
#include <list>
#include <cstdlib>
#include <time.h>

using namespace std;

const double Xmin = 0.0, Xmax = 70.0;
const double Ymin = 0.0, Ymax = 50.0;

double groundHeight=1.0,botomToGround=2.0;
double gunHeight = 2.0, gunTopWidth = 1.5, gunBottomWidth = 4.0;

double paddleX = Xmax / 2, paddleY = Ymax / 2, paddleW = 1.0, paddleH = 1.0;
double paddleVelX = 0.7;
double paddleVelY = 0.7;

double cityHeight = 2.0;
double cityWidht = 7.0;
int frame = 0;
class city {
public:
	double width, height, X, Y;
	bool destroyed;
	city() {
		width = 0;
		height = 0;
		X = 0;
		Y = 0;
		destroyed = false;
	}
	city(double width, double height, double X, double Y) {
		this->width = width;
		this->height = height;
		this->X = X;
		this->Y = Y;
		this->destroyed = false;
	}
	double center() {
		return X + width/2;
	}
	void draw() {
		if (this->destroyed) {
			return;
		}
		glColor3f(0.0, 0.0, 0.9);
		glBegin(GL_POLYGON);
		glVertex3f(X,Y,0.0);
		glVertex3f(X+width,Y,0.0);
		glVertex3f(X+width,Y+height,0.0);
		glVertex3f(X,Y+height,0.0);
		glEnd();
		
	}
	void destroy() {
		this->destroyed = true;
	}
};
city cities[6];
class bullet {
private:
	double X, Y;
	double Xvel, Yvel;
	double angle;
	double r = 1;
public:
	bool toDestroy = false;
	bullet() {
		Y = Ymax-1;
		X = ((double)rand() / RAND_MAX)*Xmax;
		int targetIndex = rand() % 6;
		double cityXCenter = cities[targetIndex].center();
		Yvel = -0.002;
		double time = (Y - (botomToGround + groundHeight + cityHeight))/Yvel;
		Xvel = (X-cityXCenter) / time;
	}
	bullet(double X,double Y,double Xvel,double Yvel,double angle) {
		this->X = X;
		this->Y = Y;
		this->Xvel = Xvel;
		this->Yvel = Yvel;
		this->angle = angle;

	}
	void draw() {
		glColor3f(0.8, 0.3, 0.2);
		glBegin(GL_POLYGON);
		for (int ii = 0; ii < 30; ii++)
		{
			float theta = 2.0f * 3.1415926f * float(ii) / float(30);//get the current angle

			float x = r * cosf(theta);//calculate the x component
			float y = r * sinf(theta);//calculate the y component

			glVertex2f(x + X, y + Y);//output vertex

		}
		glEnd();
	}
	void update() {
		X += Xvel;
		Y += Yvel;
	}
	void checkColision(){
		if (Y < botomToGround + groundHeight + r) {
			toDestroy = true;
			return;
		}
		for (int i = 0; i < 6; i++) {
			city c = cities[i];
			if (c.destroyed) {
				continue;
			}
			if (X>c.X-r && X<c.X+c.width+r && Y>c.Y-r && Y<c.Y+c.height+r) {
				cities[i].destroy();
				this->toDestroy = true;
			}

		}
	}


};

list<bullet> bullets;
std::list<bullet>::iterator it;
void drawPaddle() {
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex3f(paddleX-paddleW/2,paddleY-paddleH/2,0.0);
	glVertex3f(paddleX+paddleW/2,paddleY-paddleH/2,0.0);
	glVertex3f(paddleX+paddleW/2,paddleY+paddleH/2,0.0);
	glVertex3f(paddleX-paddleW/2,paddleY+paddleH/2,0.0);
	glEnd();
}
void drawEnv(){
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex3f(Xmax / 2 + gunBottomWidth / 2, botomToGround + groundHeight, 0.0);
	glVertex3f(Xmax / 2 + gunTopWidth / 2, botomToGround + groundHeight + gunHeight, 0.0);
	glVertex3f(Xmax / 2 - gunTopWidth / 2, botomToGround + groundHeight + gunHeight, 0.0);
	glVertex3f(Xmax / 2 - gunBottomWidth / 2, botomToGround + groundHeight, 0.0);
	glVertex3f(Xmin, botomToGround + groundHeight, 0.0);
	glVertex3f(Xmin, botomToGround, 0.0);
	glVertex3f(Xmax, botomToGround, 0.0);
	glVertex3f(Xmax, botomToGround + groundHeight, 0.0);
	glEnd();
}
void removeBullet() {
	for (it = bullets.begin(); it != bullets.end(); it++) {
		it->update();
		it->checkColision();
		it->draw();
	}
}
void drawCities() {
	for (int i = 0; i < 6; i++) {
		cities[i].draw();
	}
}
void drawBullets() {
	for (it = bullets.begin(); it != bullets.end(); it++) {
		it->update();
		it->checkColision();
		it->draw();
	}
}

bool destroyed(bullet& b) { return b.toDestroy; }
void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(1);
	};

}
void mySpecialKeyFunc(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		paddleX -= paddleVelX;
		break;
	case GLUT_KEY_RIGHT:
		paddleX += paddleVelX;
		break;
	case GLUT_KEY_UP:
		paddleY += paddleVelY;
		break;
	case GLUT_KEY_DOWN:
		paddleY -= paddleVelY;
		break;
	};
}
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glLoadIdentity();
	glPushMatrix();
	drawEnv();//crta pozadinu
	drawPaddle();//crta onu belu kockicu
	drawCities();//crta gradove
	drawBullets();//crta metkove iz liste
	bullets.remove_if(destroyed);//uklanja unistene metkove iz liste
	glPopMatrix();
	if (frame++ > 10000) {//svaki 10000 frejm ce dodati novi metak u listu sa metkovima
		frame = 0;
		bullets.push_back(bullet());
	}



	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

}
void initRendering()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glShadeModel(GL_SMOOTH);

}
void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	glViewport(0, 0, w, h);

	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;
	if ((Xmax - Xmin) / w < (Ymax - Ymin) / h) {
		scale = ((Ymax - Ymin) / h) / ((Xmax - Xmin) / w);
		center = (Xmax + Xmin) / 2;
		windowXmin = center - (center - Xmin) * scale;
		windowXmax = center + (Xmax - center) * scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax - Xmin) / w) / ((Ymax - Ymin) / h);
		center = (Ymax + Ymin) / 2;
		windowYmin = center - (center - Ymin) * scale;
		windowYmax = center + (Ymax - center) * scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(windowXmin, windowXmax, windowYmin, windowYmax, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	
	for (int i = 0; i < 6; i++) {
		int ind = i;
		if (i > 2) { ind++; }
		cities[i] = city(cityWidht, cityHeight, ind * 10.0 + 1.5, botomToGround + groundHeight);
	}
	srand(time(0));
	bullets.push_back(bullet());


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(50, 60);
	glutInitWindowSize(700, 500);

	glutCreateWindow("Missile command");

	initRendering();

	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialKeyFunc);


	glutReshapeFunc(resizeWindow);


	glutDisplayFunc(drawScene);
	glutMainLoop();

	return(0);
}
