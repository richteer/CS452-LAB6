#include <stdio.h>
#include <stdlib.h>
#include "init_stuff.h"

#define LOG_SIZE 1024

int init_shader(char * filename, GLenum type)
{
	size_t filesize;			// Size of the shader source
	char * filebuf;				// Buffer to store the shader source in
	char log[LOG_SIZE + 1];			// Error log
	int log_length;				// Size of the error log
	int ret;				// Return value, shader ID
	FILE * src = fopen(filename,"r");	// Shader file

	if (NULL == src) {
		fprintf(stderr,"Failed to open file '%s'\n",filename);
		return -1;
	}

	// Find get the total size of the source
	fseek(src,0,SEEK_END);
	filesize = ftell(src);
	rewind(src);

	// Create the source buffer, load the source into memory
	filebuf = malloc(sizeof(char) * filesize);
	fread(filebuf,1,filesize,src);

	// Create the shader, returns 0 if error
	ret = glCreateShader(type);

	// Pass in the source for compliation
	glShaderSource(ret, 1, &filebuf, &filesize);
	glCompileShader(ret);

	// Check for errors
	glGetShaderInfoLog(ret,LOG_SIZE,&log_length,log);

	// Return an error code if compilation failed
	if (log_length) {
		fprintf(stderr,"Shader '%s' compile error: %s\n",filename,log);
		return -1;
	}

	// Cleanup: free buffer, close file
	fclose(src);
	free(filebuf);

	return ret;
}

int init_program(int nargs, ...)
{
	unsigned int program;
	GLuint shader;

	va_list vl;
	char * filename;
	GLenum type;
	int i;

	char log[1000];
	int loglen;
	

	// Check to make sure there are the proper number of arguments	
	if (!nargs || (nargs % 2)) {
		fprintf(stderr,"Error: odd number of arguments passed in\n");
		return -1;
	}
	
	program = glCreateProgram();

	va_start(vl,nargs);


	// For each argument pair, pull in the filename then shader type	
	for (i = 0; i < nargs; i += 2) {
		filename = va_arg(vl,char*);
		type = va_arg(vl,GLenum);

		// Compile the shader
		shader = init_shader(filename,type);
		
		// Ensure the shader is ready
		if (shader <= 0) { 
			fprintf(stderr,"Failed to compile shader '%s'!\n",filename);
			continue;
		}
		glAttachShader(program,shader);
	}	

	glLinkProgram(program);

	// Error check
	glGetProgramInfoLog(program, 1000, &loglen,log);
	if (loglen) {
		fprintf(stderr,"Link Program Failed: %s\n",log);
		return -1;
	}

	va_end(vl);

	return program;
}

