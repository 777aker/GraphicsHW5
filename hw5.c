/*
	HW5 Lighting

	Copy of HW4 but now there is...*lighting*

	Key bindings:
	Space: rerandomize everything
	Pageup/Pagedown change dim
	Arrow keys change view
	0 Resets view
	ESC Exit
	p change camera mode
	f/F change FOV

*/
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
// include my helper file
#include "hw5functions.h"

// not sure I'll use this but called when nothing else to do
void idle();
// copied from ex8 called when window resized
void reshape();
// called when a key is pressed
void key(unsigned char ch, int x, int y);
// called when special key pressed like arrows
void special(int key, int x, int y);
// GLUT calls to display scene
void display();
// reproject
void Project();

// ok, finally, lets draw a tree
void tree(int firstrand);
struct point {
	double x;
	double y;
	double z;
};
// I explain this later
void branch(struct point base[4], double dy, int first, double dx, double dz, int iterations);
// I want an icosahedron for leaves so I'm copying this from ex8
static void icosahedron1(float x, float y, float z, float s, float th);
// ok, I want random trees but if we did srand everytime
// we'd get a totally wild tree each update, so
// populate global randoms at beginning then
// pull from here for consitent random value
// value is from 0-99
double randoms[50];

// important variablessszzzz
int th = 35; // um, view angle stuff
int ph = 35; // the other view angle rotation
double dim = 50.0; // size of world / distance of camera?
double asp = 1; // aspect ratio
double ground = -3; // I want the scene to start more towards
// the bottom of the screen so move everything down some
// this is an int that will color our middle tree
// different than the rest
int maintree = 1;
// field of view variable and camera mode
int fov=55;
const int ortho = 0;
const int pers = 1;
const int fp = 2;
int mode = ortho;
// for storing first person info
struct fpstuff {
	double x;
	double y;
	double z;
	double angle;
} firstperson;

// lighting global variables
int light = 1;

int ambient   =  10;  // Ambient intensity (%)
int diffuse   =  50;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)

int zh        = 90;  // Light azimuth
float ylight  = 30;  // Elevation of light
int distance  = 60;  // Light distance

int local     =   0;  // Local Viewer Model

float shiny   =   1;  // Shininess (value)

// light intensity
float lf = 0.05;
// toggle light motion
int lightmotion = 1;

// displays the scene
void display() {
	// clear the window and the depth buffer
	// I want a cool background color so here
	glClearColor(0.06, .92, .89, .5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enable z buffering
	glEnable(GL_DEPTH_TEST);
	// undo previous nonsense
	glLoadIdentity();
	if(mode == ortho) {
		// rotate the world
		glRotatef(ph, 1, 0, 0);
		glRotatef(th, 0, 1, 0);
	} else if(mode == pers) {
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim*Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
	} else if(mode == fp) {

		gluLookAt(firstperson.x,firstperson.y,firstperson.z, 
			firstperson.x+Cos(firstperson.angle),firstperson.y,firstperson.z+Sin(firstperson.angle), 
			0,1,0);
	}

	// we gonna use flat shading because the type of scene
	glShadeModel(GL_SMOOTH);

	// light switch
	if(light) {
		glDisable(GL_CULL_FACE);
		//  Translate intensity to color vectors
      float Ambient[]   = {lf*ambient ,lf*ambient ,lf*ambient ,1.0};
      float Diffuse[]   = {lf*diffuse ,lf*diffuse ,lf*diffuse ,1.0};
      float Specular[]  = {lf*specular,lf*specular,lf*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 5);
      glEnable(GL_CULL_FACE);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
	} else {
		glDisable(GL_LIGHTING);
	}
	
	glDisable(GL_CULL_FACE);
	// draw the grass
	// it's flat so don't do face
	// culling on it
	grass();
	glEnable(GL_CULL_FACE);

	// draw, the grouuunndddd
	// also, if you change the starting dim
	// remember to also change these points @me
	glDisable(GL_CULL_FACE);
	//  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

	Color(15, 86, 7);
	glBegin(GL_QUADS);
	// ground was too big lighting kinda weird so splitting into 4
	// a little weird when close but not too bad
	glNormal3f(0, +1, 0);

	glVertex3f(-50, ground, 50);
	glVertex3f(0, ground, 50);
	glVertex3f(0, ground, 0);
	glVertex3f(-50, ground, 0);

	glVertex3f(0, ground, 50);
	glVertex3f(50, ground, 50);
	glVertex3f(50, ground, 0);
	glVertex3f(0, ground, 0);

	glVertex3f(-50, ground, 0);
	glVertex3f(0, ground, 0);
	glVertex3f(0, ground, -50);
	glVertex3f(-50, ground, -50);

	glVertex3f(0, ground, 0);
	glVertex3f(50, ground, 0);
	glVertex3f(50, ground, -50);
	glVertex3f(0, ground, -50);
	
	glEnd();
	glEnable(GL_CULL_FACE);

	// I want a fence, for, idk, protect my property?
	// from the nothingness?
	//fence();
	//picket();

	// I probably spent way too long on decorations for the main thing
	// but whatever, makes it much more dramatic and cool
	// so uh, finally, the main subject of this composition
	// a tree
	/*
	maintree = 1;
	int first = 1;
	tree(first);
	maintree = 0;
	
	// trees along the diagonal
	first++;
	glPushMatrix();
	glTranslatef(10+randoms[first]/5, 0, 10+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(10+randoms[first]/5), 0, 10+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(10+randoms[first]/5), 0, -(10+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(10+randoms[first]/5, 0, -(10+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	// trees in one ish direction
	first++;
	glPushMatrix();
	glTranslatef(10+randoms[first]/5, 0, -(randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(10+randoms[first]/5), 0, randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(randoms[first]/5, 0, -(10+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-randoms[first]/5, 0, 10+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	// same thing but further out
	first++;
	glPushMatrix();
	glTranslatef(20+randoms[first]/5, 0, 20+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(20+randoms[first]/5), 0, 20+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(20+randoms[first]/5), 0, -(20+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(20+randoms[first]/5, 0, -(20+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	// trees in one ish direction
	first++;
	glPushMatrix();
	glTranslatef(20+randoms[first]/5, 0, (randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-(20+randoms[first]/5), 0, -randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(-randoms[first]/5, 0, -(20+randoms[first*2]/5));
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();

	first++;
	glPushMatrix();
	glTranslatef(randoms[first]/5, 0, 20+randoms[first*2]/5);
	glScalef(.5+randoms[first]/100, .5+randoms[first]/100, .5+randoms[first]/100);
	glRotatef(randoms[first]*3, 0, 1, 0);
	tree(first);
	glPopMatrix();
	*/

	// Display keys
	// Became too crowded turning them off
	/*
	glWindowPos2i(5, 5);
	Print("Arrow Keys: Change view angle");
	glWindowPos2i(5, 25);
	Print("Pageup, Pagedwn: zoom in/out");
	glWindowPos2i(5, 45);
	Print("Space: randomize trees");
	glWindowPos2i(5, 65);
	Print("p: change perspective");
	glWindowPos2i(5, 85);
	Print("f/F: change FOV");
	glWindowPos2i(5, 105);
	Print("WASD: Move, EQ: rotate");
	*/


	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}

// called when special key pressed
void special(int key, int x, int y) {
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP) 
		dim += 1;
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
		dim -= 1;
	th %= 360;
	ph %= 360;
	Project();
	glutPostRedisplay();
}


// called when a key is pressed
// so I can find it easier
// ctrlf: keyplz
void key(unsigned char ch, int x, int y) {
	// exit on esc
	if(ch==27)
		exit(0);
	// got tired of having to exit and rerun
	// to see new sets of random trees
	// so, made a space key that redoes
	// the random array which is what all the
	// randoms are based on
	if(ch==' ') {
		int i;
		for(i = 0; i < 50; i++) {
			randoms[i] = rand()%100;
			//printf("%f,", randoms[i]);
		}
	}
	// change camera mode
	if(ch=='p') {
		mode++;
		mode %= 3;
	}
	// change field of view
	if(ch=='f') {
		fov++;
	}
	if(ch=='F') {
		fov--;
	}
	if(ch=='-') {
		ylight-=1;
	}
	if(ch=='+') {
		ylight+=1;
	}
	if(ch=='<') {
		lf -= .01;
	}
	if(ch=='>') {
		lf += .01;
	}
	if(ch=='0') {
		lightmotion = !lightmotion;
	}
	// walk around
	if(mode == fp) {
		if(ch=='w' || ch=='W') {
			firstperson.x += Cos(firstperson.angle);
			firstperson.z += Sin(firstperson.angle);
			// make sure they can't walk out of bounds
			if(firstperson.x > 50)
				firstperson.x = 50;
			if(firstperson.z > 50)
				firstperson.z = 50;
			if(firstperson.x < -50)
				firstperson.x = -50;
			if(firstperson.z < -50)
				firstperson.z = -50;
			// keep them from going in fence
			if(firstperson.x < 15 && firstperson.x > -15) {
				if(firstperson.z < 15 && firstperson.z > -15) {
					firstperson.x -= Cos(firstperson.angle);
					firstperson.z -= Sin(firstperson.angle);
				}
			}
		}
		if(ch=='s' || ch=='S') {
			firstperson.x -= Cos(firstperson.angle);
			firstperson.z -= Sin(firstperson.angle);
			// make sure they can't walk out of bounds
			if(firstperson.x > 50)
				firstperson.x = 50;
			if(firstperson.z > 50)
				firstperson.z = 50;
			if(firstperson.x < -50)
				firstperson.x = -50;
			if(firstperson.z < -50)
				firstperson.z = -50;
			// keep them from going in fence
			if(firstperson.x < 15 && firstperson.x > -15) {
				if(firstperson.z < 15 && firstperson.z > -15) {
					firstperson.x += Cos(firstperson.angle);
					firstperson.z += Sin(firstperson.angle);
				}
			}
		}
		if(ch=='e' || ch=='E') {
			firstperson.angle += 2;
		}
		if(ch=='q' || ch=='Q') {
			firstperson.angle -= 2;
		}
		if(ch=='d' || ch=='D') {
			firstperson.x += Cos(firstperson.angle+90);
			firstperson.z += Sin(firstperson.angle+90);
			// make sure they can't walk out of bounds
			if(firstperson.x > 50)
				firstperson.x = 50;
			if(firstperson.z > 50)
				firstperson.z = 50;
			if(firstperson.x < -50)
				firstperson.x = -50;
			if(firstperson.z < -50)
				firstperson.z = -50;
			// keep them from going in fence
			if(firstperson.x < 15 && firstperson.x > -15) {
				if(firstperson.z < 15 && firstperson.z > -15) {
					firstperson.x -= Cos(firstperson.angle+90);
					firstperson.z -= Sin(firstperson.angle+90);
				}
			}

		}
		if(ch=='a' || ch=='A') {
			firstperson.x += Cos(firstperson.angle-90);
			firstperson.z += Sin(firstperson.angle-90);
			// make sure they can't walk out of bounds
			if(firstperson.x > 50)
				firstperson.x = 50;
			if(firstperson.z > 50)
				firstperson.z = 50;
			if(firstperson.x < -50)
				firstperson.x = -50;
			if(firstperson.z < -50)
				firstperson.z = -50;
			// keep them from going in fence
			if(firstperson.x < 15 && firstperson.x > -15) {
				if(firstperson.z < 15 && firstperson.z > -15) {
					firstperson.x -= Cos(firstperson.angle-90);
					firstperson.z -= Sin(firstperson.angle-90);
				}
			}
		}
	}
	Project();
	glutPostRedisplay();
}

// GLUT calls when nothing else to do
void idle() {
	if(lightmotion) {
		//  Elapsed time in seconds
   	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   	zh = fmod(90*t,360.0);
	}
	glutPostRedisplay();
}

// when window is resized do this stuff
void reshape(int width, int height) {
	//  Set the viewport to the entire window
	glViewport(0,0, RES*width,RES*height);
	
	//  Orthogonal projection
	asp = (height>0) ? (double)width/height : 1;
	
	Project();
}

// start up GLUT and tell it what to do
// copied from ex8
int main(int argc, char* argv[]) {
	// I'm gonna use random a lot in this so
	// initialize
	time_t t;
	srand((unsigned) time(&t));
	// initialize grass
	save_grass();
	// populate our global randoms
	int i;
	for(i = 0; i < 50; i++) {
		randoms[i] = rand()%100;
		//printf("%f,", randoms[i]);
	}
	firstperson.x = 40;
	firstperson.y = ground+4;
	firstperson.z = 0;
	firstperson.angle = 180;
	// initialize GLUT
	glutInit(&argc, argv);
	// window size
	// double buffer with z buffering
	// and I'm gonna go ahead and do face culling also
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// name it Kelley Kelley HW3
	glutCreateWindow("Kelley Kelley HW5");
	#ifdef USEGLEW
	//  Initialize GLEW
	if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
	#endif
	// I don't think I'm going to use idle but might as well
	// have it available
	glutIdleFunc(idle);
	// GLUT this is our display function
	glutDisplayFunc(display);
	// reshape function
	glutReshapeFunc(reshape);
	// special key function
	glutSpecialFunc(special);
	// keys
	glutKeyboardFunc(key);
	// start GLUT baby
	glutMainLoop();
	return 0;
}

void Project() {
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   if(mode == ortho) {
      // orthogonal project
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   } else if(mode == pers) {
      // perspective camera
      gluPerspective(fov, asp, dim/4, 4*dim);
   } else if(mode == fp) {
      // first person camera
      gluPerspective(fov, asp, (dim-30)/4, 4*(dim-10));
   }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

// ok, everything else is great and works fine
// now lets just focus on a tree and once this
// is done submit it
// wow, if only that's how it actually worked out
//-----------------------------

// I guess I should explain this coplicated mess
// parameters: base ie where it's connected to
// dy: is change in height, how long the branch is upward
// first: is a variable to add randomness from side to side, ie
// iterations is what usually adds randomness but those will be
// the same for the branches called at the same point
// it'll make more sense later?
// dx, dz: are the change in branch position, adds some randomness
// iterations: how many more times we call this function
void branch(struct point base[4], double dy, int first, double dx, double dz, int iterations) {
	int i;
	// first set the color
	Color(79, 28, 3);
	// make a new base that will be where this branch left off
	// to pass to the next branches
	struct point newbase[4];
	// draw the branch
	glBegin(GL_QUAD_STRIP);
	// loop through and draw our quads
	for(i = 4; i >= 0; i--) {
		glVertex3f(base[i%4].x, base[i%4].y, base[i%4].z);
		
		// the top of this branch which will be our next base
		newbase[i%4].x = (base[i%4].x+dx);
		newbase[i%4].y = (base[i%4].y+dy+(dy*randoms[(int)(dy*first)%100]/50.0)/2);
		newbase[i%4].z = (base[i%4].z+dz);

		glVertex3f(newbase[i%4].x, newbase[i%4].y, newbase[i%4].z);
	}
	glEnd();

	// draw some nice lines bc otherwise 3D sucks without lighting
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	for(i = 0; i <= 4; i++) {
		glVertex3f(base[i%4].x, base[i%4].y, base[i%4].z);
		glVertex3f(newbase[i%4].x, newbase[i%4].y, newbase[i%4].z);
	}
	glEnd();
	// so, decrease iterations because this stops once there are no
	// more iterations
	iterations--;
	if(iterations > 0) {
		// then, we randomize the negative values
		int negx, negz, negx2, negz2;
		negx = negz = negx2 = negz2 = 1;
		if(randoms[(iterations*first*2)%100] < 50)
			negx = -1;
		if(randoms[(iterations*first*6+80)%100] < 50)
			negz = -1;
		if(randoms[((iterations*first+200)/6)%100] < 50)
			negx2 = -1;
		if(randoms[(iterations*first*28)%100] < 50)
			negz2 = -1;
		//printf("%d, %d, %d, %d\n", negx, negz, negx2, negz2);
		// then we call our next branch and we randomize some values
		// all this first and iterations nonsense math rather than
		// just srand is so that it's consistent across display calls
		// if we just did srand branches would move all over the place 
		// which would look insane
		// there is also a lot of really cheaky random math here for the
		// tree to come out nicely
		branch(newbase, dy/1.01, (int)randoms[(first+1)%100], negx*randoms[(iterations*first*4-3)%100]/10.0*pow(iterations, .5), negz*randoms[(iterations*first*25/14-8)%100]/10.0*pow(iterations, .5), iterations);
		branch(newbase, dy/1.01, (int)randoms[first%100], negx2*randoms[(iterations*first+62)%100]/10.0*pow(iterations, .5), negz2*randoms[((iterations*first+60)/3)%100]/10.0*pow(iterations, .5), iterations);
	} else {
		// it's the end of our tree so lets draw some leaves
		int negx, negz, negy;
		negx = negz = negy = 1;
		if(randoms[(first*2)%100] < 50)
			negx = -1;
		if(randoms[(first*6+80)%100] < 50)
			negz = -1;
		if(randoms[((first+200)/6)%100] < 50)
			negy = -1;
		icosahedron1(newbase[0].x+negx*(randoms[first%100]/50), newbase[0].y+negy*(randoms[first*2%100]/50), newbase[0].z+negz*(randoms[first*3%100]/50), 
			5+randoms[first%100]/20, (int)(randoms[first%100]*10)%360);

	}
}

// draw a treeee
void tree(int firstrand) {
	glPushMatrix();

	glTranslatef(0, ground, 0);
	glScalef(.5, .5, .5);
	// I guess we'll start with the trunk first
	Color(79, 28, 3);
	int i;
	glBegin(GL_POLYGON);
	for(i = 360; i >= 0; i -= 60)
		Vertex(i, 0);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for(i = 360; i >= 0; i -= 60) {
		Vertexflat(i, 0);
		Vertexflat(i, 10);
	}
	glEnd();
	// now some branches?
	// how should I do this?
	// I decided recursion is the best way
	struct point base[4];
	for(i = 0; i < 4; i++) {
		base[i].x = Sin(i*60);
		base[i].y = 10;
		base[i].z = Cos(i*60);
	}
	// start one side of the tree
	branch(base, 5, 1*firstrand, 2, 2, 4);
	for(i = 3; i < 7; i++) {
		base[i%4].x = Sin(i*60);
		base[i%4].y = 10;
		base[i%4].z = Cos(i*60);
	}
	// start the other side of the tree
	branch(base, 5, 4*firstrand, -1, -1.5, 4);

	// lines
	Color(0, 0, 0);
	glBegin(GL_LINES);
	for(i = 0; i <= 360; i += 60) {
		Vertexflat(i, 0);
		Vertexflat(i, 10);
	}
	glEnd();
	glTranslatef(0,5,0);
	/*
	glBegin(GL_QUAD_STRIP);
	for(i = 360; i >= 0; i -= 60) {
		Vertex(i, 0);
		Vertex(i, 60);
	}
	glEnd();
	*/
	// doesn't matter which tree we are drawing
	// if we drew a tree no more trees can possibly
	// be the main tree so set to 0
	//maintree = 0;
	glPopMatrix();
}

static void icosahedron1(float x,float y,float z,float s,float th) {
   //  Vertex index list
   const int N=60;
   const unsigned char index[] =
      {
       2, 1, 0,    3, 2, 0,    4, 3, 0,    5, 4, 0,    1, 5, 0,
      11, 6, 7,   11, 7, 8,   11, 8, 9,   11, 9,10,   11,10, 6,
       1, 2, 6,    2, 3, 7,    3, 4, 8,    4, 5, 9,    5, 1,10,
       2, 7, 6,    3, 8, 7,    4, 9, 8,    5,10, 9,    1, 6,10,
      };
   //  Vertex coordinates
   const float xyz[] =
      {
       0.000, 0.000, 1.000,
       0.894, 0.000, 0.447,
       0.276, 0.851, 0.447,
      -0.724, 0.526, 0.447,
      -0.724,-0.526, 0.447,
       0.276,-0.851, 0.447,
       0.724, 0.526,-0.447,
      -0.276, 0.851,-0.447,
      -0.894, 0.000,-0.447,
      -0.276,-0.851,-0.447,
       0.724,-0.526,-0.447,
       0.000, 0.000,-1.000
      };
   //  Vertex colors
   // except, I want different colors bc trees so I'll be changing this
   
   float rgb[] =
      {
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      0.0,1.0,0.2,
      };
   float mainrgb[] =
      {
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      1.0,0.2,0.2,
      };

   int i;
   for(i = 0; i < 36; i++) {
   	rgb[i] -= randoms[(int)(i*x)%100]/100;
   	mainrgb[i] -= randoms[(int)(i*x)%100]/100;
   }

  	const float black[] =
      {
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      0.0,0.0,0.0,
      };
   //  Define vertexes
   glDisable(GL_CULL_FACE);
   glVertexPointer(3,GL_FLOAT,0,xyz);
   glEnableClientState(GL_VERTEX_ARRAY);
   //  Define colors for each vertex
   if(maintree == 1)
   	glColorPointer(3,GL_FLOAT,0,mainrgb);
   else
   	glColorPointer(3,GL_FLOAT,0,rgb);
   glEnableClientState(GL_COLOR_ARRAY);
   //  Draw icosahedron
   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(th,1,0,0);
   glScalef(s,s,s);
   glDrawElements(GL_TRIANGLES,N,GL_UNSIGNED_BYTE,index);
   //  draw some linnnzzzz
   glLineWidth(1);
   glColorPointer(3,GL_FLOAT,0,black);
   glEnableClientState(GL_COLOR_ARRAY);
   glDrawElements(GL_LINE_STRIP,N,GL_UNSIGNED_BYTE,index);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
   //  Disable vertex array
   glDisableClientState(GL_VERTEX_ARRAY);
   //  Disable color array
   glDisableClientState(GL_COLOR_ARRAY);
}

//-----------------------------