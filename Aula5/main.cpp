#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


float rodar = 0;
float rodarS = 0.2;
void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	// End of frame
	
	
	// put code to draw scene in here
	//Torus
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glutSolidTorus(2, 5, 30, 30);
	glPopMatrix();

	//Arvores em volta
	

	srand(1);
	for (int i = 0; i < 100; i++) {
		int x = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200));
		int z = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200));

		if (pow(x, 2) + pow(z, 2) >= pow(50,2) && x < 100 && z < 100) {
			glPushMatrix();
			glTranslatef(x, 0,z);
			glRotatef(-90, 1, 0, 0);
			glColor3f(0.0f, 0.2f, 0.0f);
			glutSolidCone(2, 7, 5, 5);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(x, 3,z);
			glRotatef(-90, 1, 0, 0);
			glColor3f(0.0f, 1.0f, 0.0f);
			glutSolidCone(6, 8, 5, 5);
			glPopMatrix();
		}
	}

	

	glPushMatrix();
	glRotatef(rodar, 0, 1, 0);

	//1 ronda de Teapots em volta
	glColor3f(1.0f, 0.0f, 1.0f);
	float aStep = 2*M_PI / 8;
	float rotateStep = 360 / 8;
	float a = 0;
	float b = 0;
	float rotate = -90;
	float r = 15;
	for (int i = 0; i < 8; i++) {
		float x = r * cos(b) * sin(a);
		float z = r * cos(b) * cos(a);
		glPushMatrix();
		glTranslatef(x, 2, z);
		glRotatef(rotate, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
		a += aStep;
		rotate += rotateStep;
	}

	//1 ronda de Teapots em volta
	glColor3f(2.0f, 10.0f, 4.0f);
	aStep = 2 * M_PI / 8;
	rotateStep = 360 / 8;
	a = 0;
	b = 0;
	rotate = 0;
	r = 35;
	for (int i = 0; i < 8; i++) {
		float x = r * cos(b) * sin(a);
		float z = r * cos(b) * cos(a);
		glPushMatrix();
		glTranslatef(x, 2, z);
		glRotatef(rotate, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
		a += aStep;
		rotate += rotateStep;
	}
	glPopMatrix();
	rodar += rodarS;
	
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
