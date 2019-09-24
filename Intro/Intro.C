#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <sstream>
#include <unistd.h>

//Nuovi Include
#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include "GL/freeglut.h"
#include "GL/gl.h"

using namespace std;

struct Cerchio;

double rapp = 1;
Cerchio * arr;

struct Cerchio{
	double Ux;
	double Uy;
	double Ur;
	double R;
	double G;
	double B;
	double A;
	Cerchio(double Ux=0, double Uy=0, double Ur=0.05, double R=1, double G=0, double B=0, double A=1): Ux(Ux), Uy(Uy), Ur(Ur), R(R), G(G), B(B), A(A){}
};

void drawCircol(Cerchio c){
	double ang = 0;
	while (ang<2*3.14){
	    glColor4f(c.R, c.G, c.B, c.A);
        glBegin(GL_TRIANGLES);
                glVertex2f(cos(ang)*c.Ur+c.Ux, sin(ang)*c.Ur*rapp+c.Uy);
                glVertex2f(cos(ang+0.1)*c.Ur+c.Ux, sin(ang+0.1)*c.Ur*rapp+c.Uy);
                glVertex2f(c.Ux, c.Uy);
        glEnd();
        ang+=0.1;
	}
}
double c = 0, cc = 0;
void render(int i){
	glutTimerFunc(25, render, 0);
	glClearColor(0, 0.025, 0.045, 1);
    glClear(GL_COLOR_BUFFER_BIT);
   	if (c>=2*3.14){
    	c = 0;
    	cc = cc+0.0005;
    }
    for (int i=0;i<10;i++){
    	arr[i].Uy=sin((c+i*0.1));
    	arr[i].Ux= arr[i].Ux+cc;
    	drawCircol(arr[i]);
    }
    //cout << sin(c) << endl;
    c = c+0.03;
	glutSwapBuffers();
}
void draw(){
	rapp=glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT);
	render(0);
}
int main(int argc, char * argv[]){
	arr = new Cerchio[10];
	for (int i=0;i<10;i++){
		arr[i].Ux = -1+i*0.1;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
  	glutInitWindowSize(400, 400);
  	glutInitWindowPosition(0, 0);
  	glutCreateWindow("Nut Planet");
  	glutDisplayFunc(draw);
  	glutTimerFunc(25, render, 0);
  	glutMainLoop();
}
