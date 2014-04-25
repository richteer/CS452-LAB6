#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>
//#include <SOIL/SOIL.h>

#include "img.c"
#include "init_stuff.h"

#define NOPE(a) (exit(a))

#define ANGLE_X 0x0
#define ANGLE_Y 0x1
#define ANGLE_Z 0x2

#define PI 3.141592653589793f

#define TEXTURE_SIZE 256

unsigned int prog;
unsigned int step = 10;
float angle = 0;

typedef struct {
	float x;
	float y;
	float z;
} angle_t;

typedef struct {
	float x;
	float y;
} pos_t;

angle_t rotation = {0};
pos_t pos = {0};
float scale = 1;


int texID;
int num_vertices;

GLint object, shadow;

void init(void)
{
	int vaoID, vertBuf, elemBuf, texBuf;
	int rot_pos, pos_pos, sca_pos;

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	object = init_program(4, "vshader.glsl", GL_VERTEX_SHADER , "fshader.glsl", GL_FRAGMENT_SHADER);
	if (object < 0) {
		fprintf(stderr,"Error: could not initialize program, bailing...\n");
		NOPE(1);
	}
	shadow = init_program(4, "vshadow.glsl", GL_VERTEX_SHADER , "fshadow.glsl", GL_FRAGMENT_SHADER);
	if (shadow < 0) {
		fprintf(stderr,"Error: could not initialize program, bailing...\n");
		NOPE(1);
	}
	
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D,texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image.width, gimp_image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, gimp_image.pixel_data);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture( GL_TEXTURE0 ); 	

	GLfloat vertices[] = {
		-1, -1, -1,
		 1, -1, -1,
		-1, -1,  1,
		 1, -1,  1,
		 1, -1, -1,
		-1, -1,  1,

		-1, 1, -1,
		 1, 1, -1,
		-1, 1,  1,
		 1, 1,  1,
		 1, 1, -1,
		-1, 1,  1,

		-1, -1, -1,
		-1,  1, -1,
		-1, -1,  1,
		-1,  1,  1,
		-1,  1, -1,
		-1, -1,  1,

		 1, -1, -1,
		 1,  1, -1,
		 1, -1,  1,
		 1,  1,  1,
		 1,  1, -1,
		 1, -1,  1,

		-1, -1, -1,
		-1,  1, -1,
		 1, -1, -1,
		 1,  1, -1,
		-1,  1, -1,
		 1, -1, -1,

		-1, -1,  1,
		-1,  1,  1,
		 1, -1,  1,
		 1,  1,  1,
		-1,  1,  1,
		 1, -1,  1,
	};

	num_vertices = sizeof(vertices)/sizeof(GLfloat)/3;

	int i;
	for (i = 0; i < sizeof(vertices)/sizeof(GLfloat); i++)
		vertices[i] *= 0.5;

	GLfloat texCor[] = {
		0, 0,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 1,

		0, 0,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 1,

		0, 0,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 1,

		0, 0,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 1,

		0, 0,
		0, 1,
		1, 0,
		1, 1,
		0, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		1, 1,
		0, 1,
		1, 0,
	};

	
	glGenBuffers(1, &vertBuf);
	glGenBuffers(1, &texBuf);

	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, texBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCor), texCor, GL_STATIC_DRAW);
	
	int tex_loc = glGetAttribLocation(prog, "intexCoords");
	printf("tex_loc = %d\n",tex_loc);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(tex_loc);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, NULL);

}

void on_idle(void) {
	clock_t clk = clock();

	angle += PI/180.0f;
	
	while(clock() - clk <= 10000);

	glutPostRedisplay();
}

void on_key(unsigned char key, int x, int y)
{
	switch(key) {
	}	
	glutPostRedisplay();
}


void renderFromLight(void)
{
	glUseProgram(shading); // See init()
	pseudoBegin();
	-> //Render the object(s) from the point of view of the light(s)
	-> //Save the buffer as a texture 
	-> //Determine the shadows being cast on surfaces obscured by the object (where can the camera at the light not see?)
	pseudoEnd();
}

void renderObject(void)
{
	glUseProgram(object);
	pseudoBegin();
	-> // Render the object normally
	-> // Apply shadow texture, transformed to fit the surfaces to the scene 
	pseudoEnd();
}


void show_stuff(void)
{
	renderFromLight(); // See above
	
	renderObject();
}


int main(int argc, char** argv)
{
	glutInit(&argc,argv);

	glutInitContextVersion(3,3);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Lab 5");

	init();

	glutIdleFunc(on_idle);	
	glutDisplayFunc(show_stuff);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}

