

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

float camX = 0, camZ = 0, camY = 50;
float cX, cY, cZ;
int startX, startY, tracking = 0;
float Lx = 0.0, Ly = 0.0, Lz = 0.0;

int alpha = 0, beta = 45, r = 50;

float height, widht;

int strips;

unsigned char* imageData;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float getY(int i, int j) {
	return (float) imageData[j * (strips +1) + i];
}


float hf(float px, float pz) {
	
	int x1 = floor(px);
	int x2 = x1 + 1;
	int z1 = floor(pz);
	int z2 = z1 + 1;

	int fx = px - x1;
	int fz = pz - z1;

	int h_x1_z = getY(x1 + height/2, z1 + widht/2) * (1 - fz) + getY(x1 + height / 2, z2 + widht / 2) * fz;
	int h_x2_z = getY(x2 + height / 2, z1 + widht / 2) * (1 - fz) + getY(x2 + height / 2, z2 + widht / 2) * fz;

	int height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;
	return height_xz;
}


void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
	float step = 1;
	float x = -widht/2;
	float z = -height/2;
	float y = 0;
	for (int i = 0; i < strips; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < strips; j++) {
			y = getY(i, j);
			glVertex3f(x, y, z);
			y = getY(i+1, j);
			glVertex3f(x + step, y, z);
			z += step;
		}
		glEnd();
		z = -height / 2;
		x += step;
	}
}



void renderScene(void) {


	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	float eye = 5;
	float camY = hf(camX, camZ) + eye;
	Lx = camX + cX;
	Lz = camZ + cY;
	Ly = camY + cZ;
	gluLookAt(camX, camY, camZ, 
		      Lx,Ly,Lz,
			  0.0f,1.0f,0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawTerrain();

	// draw trees
	srand(1);
	for (int i = 0; i < 100; i++) {
		float px = -widht / 2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / widht));
		float pz = -height / 2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / height));
		float h = hf(px, pz);
		glPushMatrix();
		glTranslatef(px, h, pz);
		glRotatef(-90, 1, 0, 0);
		glColor3f(0.0f, 0.2f, 0.0f);
		glutSolidCone(1, 7, 5, 5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(px, h+3, pz);
		glRotatef(-90, 1, 0, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glutSolidCone(4, 8, 5, 5);
		glPopMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

	// put code to process regular keys in here
	switch (key)
	{
	default:
	case 'd':
		camZ += 0.2;
		break;
	case 'a':
		camZ -= 0.2;
		break;
	case 's':
		camX -= 0.2;
		break;
	case 'w':
		camX += 0.2;
		break;

		glutPostRedisplay();
	}
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
	
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	cX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	cZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	cY = rAux * 							     sin(betaAux * 3.14 / 180.0);

}


void init() {
	unsigned int t;
	
// 	Load the height map "terreno.jpg"
	ilLoadImage((ILstring)"terreno.jpg");

	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	widht = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	strips = (int)height - 1;
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();
// 	Build the vertex arrays

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(800,800);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	//glEnableClientState(GL_VERTEX_ARRAY);

	ilInit();
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

