#include "hw5functions.h"
// a bunch of includes
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
#include <time.h>

// some global variables I need
double gr = -3;

// um, outer rim thingy...idk what these are called
// wow, this is getting really long
// I'mma move it to the bottom
// there was definitely a better way to do this
void picket() {
   glPushMatrix();
   glEnable(GL_CULL_FACE);
   glTranslatef(0, gr, 0);
   glColor3f(1, 1, 1);
   glBegin(GL_QUADS);

   // front of the fence

   glVertex3f(9.5, 1, -9.5);
   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(9.5, 1, 9.5);

   glVertex3f(9.5, 2, -9.5);
   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(9.5, 2, 9.5);
   
   glVertex3f(-9.5, 2, -9.5);
   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(9.5, 2, -9.5);

   glVertex3f(-9.5, 1, -9.5);
   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(9.5, 1, -9.5);

   glVertex3f(9.5, 2, 9.5);
   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(-9.5, 2, 9.5);

   glVertex3f(9.5, 1, 9.5);
   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(-9.5, 1, 9.5);

   glVertex3f(-9.5, 2, 9.5);
   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2, -9.5);

   glVertex3f(-9.5, 1, 9.5);
   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1, -9.5);

   // back of the fence

   glVertex3f(-9, 1, -9);
   glVertex3f(-9, 1.5, -9);
   glVertex3f(-9, 1.5, 9);
   glVertex3f(-9, 1, 9);

   glVertex3f(-9, 2, -9);
   glVertex3f(-9, 2.5, -9);
   glVertex3f(-9, 2.5, 9);
   glVertex3f(-9, 2, 9);

   glVertex3f(9, 1, 9);
   glVertex3f(9, 1.5, 9);
   glVertex3f(9, 1.5, -9);
   glVertex3f(9, 1, -9);
   
   glVertex3f(9, 2, 9);
   glVertex3f(9, 2.5, 9);
   glVertex3f(9, 2.5, -9);
   glVertex3f(9, 2, -9);
   
   glVertex3f(9, 2, -9);
   glVertex3f(9, 2.5, -9);
   glVertex3f(-9, 2.5, -9);
   glVertex3f(-9, 2, -9);
   
   glVertex3f(9, 1, -9);
   glVertex3f(9, 1.5, -9);
   glVertex3f(-9, 1.5, -9);
   glVertex3f(-9, 1, -9);
   
   glVertex3f(-9, 2, 9);
   glVertex3f(-9, 2.5, 9);
   glVertex3f(9, 2.5, 9);
   glVertex3f(9, 2, 9);
   
   glVertex3f(-9, 1, 9);
   glVertex3f(-9, 1.5, 9);
   glVertex3f(9, 1.5, 9);
   glVertex3f(9, 1, 9);

   // top of the fence

   glVertex3f(-9, 1.5, 9);
   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(9, 1.5, 9);

   glVertex3f(-9, 2.5, 9);
   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(9, 2.5, 9);

   glVertex3f(-9, 1.5, -9);
   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(-9, 1.5, 9);

   glVertex3f(-9, 2.5, -9);
   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(-9, 2.5, 9);

   glVertex3f(9, 1.5, -9);
   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(-9, 1.5, -9);

   glVertex3f(9, 2.5, -9);
   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(-9, 2.5, -9);

   glVertex3f(9, 2.5, 9);
   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(9, 2.5, -9);

   glVertex3f(9, 1.5, 9);
   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(9, 1.5, -9);

   // bottom of the fence

   glVertex3f(9, 1, 9);
   glVertex3f(9.5, 1, 9.5);
   glVertex3f(-9.5, 1, 9.5);
   glVertex3f(-9, 1, 9);
   
   glVertex3f(9, 2, 9);
   glVertex3f(9.5, 2, 9.5);
   glVertex3f(-9.5, 2, 9.5);
   glVertex3f(-9, 2, 9);
   
   glVertex3f(-9, 1, 9);
   glVertex3f(-9.5, 1, 9.5);
   glVertex3f(-9.5, 1, -9.5);
   glVertex3f(-9, 1, -9);
   
   glVertex3f(-9, 2, 9);
   glVertex3f(-9.5, 2, 9.5);
   glVertex3f(-9.5, 2, -9.5);
   glVertex3f(-9, 2, -9);
   
   glVertex3f(-9, 1, -9);
   glVertex3f(-9.5, 1, -9.5);
   glVertex3f(9.5, 1, -9.5);
   glVertex3f(9, 1, -9);
   
   glVertex3f(-9, 2, -9);
   glVertex3f(-9.5, 2, -9.5);
   glVertex3f(9.5, 2, -9.5);
   glVertex3f(9, 2, -9);
   
   glVertex3f(9, 2, -9);
   glVertex3f(9.5, 2, -9.5);
   glVertex3f(9.5, 2, 9.5);
   glVertex3f(9, 2, 9);
   
   glVertex3f(9, 1, -9);
   glVertex3f(9.5, 1, -9.5);
   glVertex3f(9.5, 1, 9.5);
   glVertex3f(9, 1, 9);

   glEnd();

   glLineWidth(3);
   glColor3f(0, 0, 0);
   glBegin(GL_LINE_STRIP);

   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(9.5, 2.5, 9.5);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9, 2.5, 9);
   glVertex3f(9, 2.5, -9);
   glVertex3f(-9, 2.5, -9);
   glVertex3f(-9, 2.5, 9);
   glVertex3f(9, 2.5, 9);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(9.5, 1.5, 9.5);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9, 1.5, 9);
   glVertex3f(9, 1.5, -9);
   glVertex3f(-9, 1.5, -9);
   glVertex3f(-9, 1.5, 9);
   glVertex3f(9, 1.5, 9);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9.5, 2, 9.5);
   glVertex3f(9.5, 2, -9.5);
   glVertex3f(-9.5, 2, -9.5);
   glVertex3f(-9.5, 2, 9.5);
   glVertex3f(9.5, 2, 9.5);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9, 2, 9);
   glVertex3f(9, 2, -9);
   glVertex3f(-9, 2, -9);
   glVertex3f(-9, 2, 9);
   glVertex3f(9, 2, 9);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9.5, 1, 9.5);
   glVertex3f(9.5, 1, -9.5);
   glVertex3f(-9.5, 1, -9.5);
   glVertex3f(-9.5, 1, 9.5);
   glVertex3f(9.5, 1, 9.5);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(9, 1, 9);
   glVertex3f(9, 1, -9);
   glVertex3f(-9, 1, -9);
   glVertex3f(-9, 1, 9);
   glVertex3f(9, 1, 9);

   glEnd();
   glBegin(GL_LINES);

   glVertex3f(9.5, 2.5, 9.5);
   glVertex3f(9.5, 2, 9.5);

   glVertex3f(9.5, 1.5, 9.5);
   glVertex3f(9.5, 1, 9.5);

   glVertex3f(-9.5, 2.5, 9.5);
   glVertex3f(-9.5, 2, 9.5);

   glVertex3f(-9.5, 1.5, 9.5);
   glVertex3f(-9.5, 1, 9.5);

   glVertex3f(-9.5, 2.5, -9.5);
   glVertex3f(-9.5, 2, -9.5);

   glVertex3f(-9.5, 1.5, -9.5);
   glVertex3f(-9.5, 1, -9.5);

   glVertex3f(9.5, 2.5, -9.5);
   glVertex3f(9.5, 2, -9.5);

   glVertex3f(9.5, 1.5, -9.5);
   glVertex3f(9.5, 1, -9.5);

   glEnd();

   glPopMatrix();
}

// draw a nice fence
void fence() {
   glPushMatrix();
   glEnable(GL_CULL_FACE);

   glTranslatef(10, gr, 9);
   fence_line();
   glTranslatef(18, 0, -18);
   fence_line();
   glTranslatef(-1, 0, -1);
   glRotatef(90, 0, 1, 0);
   fence_line();
   glTranslatef(18, 0, 18);
   fence_line();
   /*
   glTranslatef(dim, ground, dim-1);
   fence_line();
   glTranslatef(dim*2-2, 0, -dim*2+2);
   fence_line();
   glTranslatef(-1, 0, -1);
   glRotatef(90, 0, 1, 0);
   fence_line();
   glTranslatef(dim*2-2, 0, dim*2-2);
   fence_line();*/

   glDisable(GL_CULL_FACE);
   glPopMatrix();
}

void fence_line() {
   int i;
   for(i = 0; i < 9; i++) {
      glTranslatef(-2, 0, 0);
      fence_post();
   }
}

void fence_post() {
   glColor3f(1, 1, 1);
   glBegin(GL_QUADS);

   glVertex3f(-.5, 0, .25);
   glVertex3f(-.5, 0, -.25);
   glVertex3f(.5, 0, -.25);
   glVertex3f(.5, 0, .25);

   glVertex3f(-.5, 0, .25);
   glVertex3f(.5, 0, .25);
   glVertex3f(.5, 3, .25);
   glVertex3f(-.5, 3, .25);

   glVertex3f(.5, 0, .25);
   glVertex3f(.5, 0, -.25);
   glVertex3f(.5, 3, -.25);
   glVertex3f(.5, 3, .25);

   glVertex3f(.5, 0, -.25);
   glVertex3f(-.5, 0, -.25);
   glVertex3f(-.5, 3, -.25);
   glVertex3f(.5, 3, -.25);

   glVertex3f(-.5, 0, -.25);
   glVertex3f(-.5, 0, .25);
   glVertex3f(-.5, 3, .25);
   glVertex3f(-.5, 3, -.25);

   glVertex3f(-.5, 3, -.25);
   glVertex3f(-.5, 3, .25);
   glVertex3f(0, 4, .25);
   glVertex3f(0, 4, -.25);

   glVertex3f(0, 4, -.25);
   glVertex3f(0, 4, .25);
   glVertex3f(.5, 3, .25);
   glVertex3f(.5, 3, -.25);

   glEnd();

   glBegin(GL_TRIANGLES);

   glVertex3f(-.5, 3, .25);
   glVertex3f(.5, 3, .25);
   glVertex3f(0, 4, .25);

   glVertex3f(0, 4, -.25);
   glVertex3f(.5, 3, -.25);
   glVertex3f(-.5, 3, -.25);

   glEnd();

   glColor3f(0, 0, 0);
   glLineWidth(3);
   glBegin(GL_LINE_STRIP);

   glVertex3f(.5, 0, .25);
   glVertex3f(.5, 3, .25);
   glVertex3f(0, 4, .25);
   glVertex3f(-.5, 3, .25);
   glVertex3f(-.5, 0, .25);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(.5, 0, -.25);
   glVertex3f(.5, 3, -.25);
   glVertex3f(0, 4, -.25);
   glVertex3f(-.5, 3, -.25);
   glVertex3f(-.5, 0, -.25);

   glEnd();
   glBegin(GL_LINE_STRIP);

   glVertex3f(-.5, 0, .25);
   glVertex3f(.5, 0, .25);
   glVertex3f(.5, 0, -.25);
   glVertex3f(-.5, 0, -.25);

   glEnd();
   glBegin(GL_LINES);

   glVertex3f(.5, 3, .25);
   glVertex3f(.5, 3, -.25);

   glVertex3f(-.5, 3, .25);
   glVertex3f(-.5, 3, -.25);

   glVertex3f(0, 4, .25);
   glVertex3f(0, 4, -.25);

   glEnd();
}

// I forgot the openGL method and this is easier
// than finding it
void Color(double r, double g, double b) {
   glColor3f(r/255, g/255, b/255);
}

/*
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where) {
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...) {
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...) {
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
