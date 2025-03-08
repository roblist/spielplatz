#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef float vec3[3];
typedef float vec4[4];

void vec3_copy(vec3 v1, vec3 v2);
void vec3_add(vec3 v1, vec3 v2, vec3 r);
void vec3_scale(vec3 v, float s, vec3 r);
float vec3_dot(vec3 v1, vec3 v2);
void vec3_cross(vec3 v1, vec3 v2, vec3 r);
float vec3_length(vec3 v);
void vec3_normalize(vec3 v, vec3 r);

void vec4_copy(vec4 v1, vec4 v2);
void vec4_add(vec4 v1, vec4 v2, vec4 r);
void vec4_scale(vec4 v, float s, vec4 r);
float vec4_length(vec4 v);
void vec4_normalize(vec4 v, vec4 r);

#endif
