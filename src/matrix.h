#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

#include "vector.h"

typedef float mat4[4][4]; 

void mat4_identity(mat4 m);
void mat4_multiply(mat4 m1, mat4 m2, mat4 r);

void mat4_translate(mat4 m, vec3 v, mat4 r);
void mat4_rotate(mat4 m, float angle, vec3 v, mat4 r);
void mat4_scale(mat4 m, vec3 v, mat4 r);

void mat4_lookat(vec3 eye, vec3 center, vec3 up, mat4 r);
void mat4_perspective(float fov, float aspect, float near, float far, mat4 r);

#endif
