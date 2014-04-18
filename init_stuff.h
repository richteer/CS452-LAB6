#ifndef _INIT_SHADER_H_
#define _INIT_SHADER_H_
#include <GL/gl.h>
#include <stdarg.h>

int init_shader(char * filename, GLenum type);
int init_program(int nargs, ...);
#endif
