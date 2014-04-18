#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>
#include <SOIL/SOIL.h>

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

void init(void)
{
	prog = init_program(4, "vshader.glsl", GL_VERTEX_SHADER , "fshader.glsl", GL_FRAGMENT_SHADER);
	if (prog < 0) {
		fprintf(stderr,"Error: could not initialize program, bailing...\n");
		NOPE(1);
	}

	glUseProgram(prog);
	glEnable(GL_DEPTH_TEST);

//	glGenTextures(1,&texID);

//	pos.x = 0.5f;
//	pos.y = 0.5f;
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

	// MOAR init
	int vaoID, vertBuf, elemBuf, texBuf;
	int rot_pos, pos_pos, sca_pos;

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	texID = SOIL_load_OGL_texture("img.png",SOIL_LOAD_AUTO,	SOIL_CREATE_NEW_ID,	
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	if (texID == 0) {
        printf("SOIL derped\n");
		NOPE(-1);
	}

	glBindTexture(GL_TEXTURE_2D,texID);

/*
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
*/
	glActiveTexture( GL_TEXTURE0 ); 	

	GLfloat vertices[] = {
		-1, -1, -1,
		-1, -1,  1,
		 1, -1, -1,
		 1, -1, -1,
		-1, -1,  1,
		 1, -1,  1,

		-1,  1, -1,
		 1,  1, -1,
		-1, -1, -1,
		-1, -1, -1,
		 1,  1, -1,
		 1, -1, -1,

		-1, -1, -1,
		-1, -1,  1,
		-1,  1, -1,
		-1,  1, -1,
		-1, -1,  1,
		-1,  1,  1,

		 1, -1, -1,
		 1, -1,  1,
		 1,  1,  1,
		 1,  1,  1,
		 1, -1,  1,
		 1,  1,  1,

		-1, -1,  1,
		 1, -1,  1,
		-1,  1,  1,
		-1,  1,  1, 
		 1, -1,  1,
		 1,  1,  1,	

		-1,  1, -1,
		 1,  1, -1,
		-1,  1,  1,
		-1,  1,  1,
		 1,  1, -1,
		 1,  1,  1

	};

	num_vertices = sizeof(vertices)/sizeof(GLfloat)/3;

	int i;
	for (i = 0; i < sizeof(vertices)/sizeof(GLfloat); i++)
		vertices[i] *= 0.5;

	GLfloat texCor[] = {
		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0,

		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0,

		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0,

		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0,

		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0,

		0,0,
		1,0,
		0,1,
		1,1,
		0,1,
		1,0

	};


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, NULL);
	
	glGenBuffers(1, &vertBuf);
	glGenBuffers(1, &texBuf);

	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, texBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCor), texCor, GL_STATIC_DRAW);
	
	int tex_loc = glGetAttribLocation(prog, "intexCoords");
	printf("tex_loc = %d\n",tex_loc);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(tex_loc);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, NULL);


}

void on_idle(void) {
	clock_t clk = clock();

	angle += PI/180.0f;
	
	while(clock() - clk <= 10000);

	glutPostRedisplay();
}

// TODO: Write this
void on_key(unsigned char key, int x, int y)
{
	switch(key) {
		
		// Rotation
	
		// Translation

		// Scale

	}
	glutPostRedisplay();
}

void show_stuff(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1fv(glGetUniformLocation(prog, "angle", 0), 1, &angle);
	glUniform1iv(glGetUniformLocation(prog, "texture", 0), 1, &texID);

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutCreateWindow("Lab 5");

	glutInitContextVersion(4,3);
	glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);

	init();

	glutIdleFunc(on_idle);	
	glutDisplayFunc(show_stuff);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}

