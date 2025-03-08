#include "vector.h"

void vec3_copy(vec3 v1, vec3 v2)
{
	v2[0] = v1[0];
	v2[1] = v1[1];
	v2[2] = v1[2];
}

void vec3_add(vec3 v1, vec3 v2, vec3 r)
{
	r[0] = v1[0] + v2[0];
	r[1] = v1[1] + v2[1];
	r[2] = v1[2] + v2[2];
}

void vec3_scale(vec3 v, float s, vec3 r)
{
	r[0] = v[0] * s;
	r[1] = v[1] * s;
	r[2] = v[2] * s;
}

float vec3_dot(vec3 v1, vec3 v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void vec3_cross(vec3 v1, vec3 v2, vec3 r)
{
	r[0] = v1[1] * v2[2] - v1[2] * v2[1];
	r[1] = v1[2] * v2[0] - v1[0] * v2[2];
	r[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float vec3_length(vec3 v)
{
	float r = 0;

	r += v[0] * v[0];
	r += v[1] * v[1];
	r += v[2] * v[2];
	
	return sqrtf(r);
}

void vec3_normalize(vec3 v, vec3 r)
{
	float l = vec3_length(v);
	
	r[0] = v[0] / l;
	r[1] = v[1] / l;
	r[2] = v[2] / l;
}	

void vec4_copy(vec4 v1, vec4 v2)
{
	v2[0] = v1[0];
	v2[1] = v1[1];
	v2[2] = v1[2];
	v2[3] = v1[3];
}

void vec4_add(vec4 v1, vec4 v2, vec4 r)
{
	r[0] = v1[0] + v2[0];
	r[1] = v1[1] + v2[1];
	r[2] = v1[2] + v2[2];
	r[3] = v1[3] + v2[3];
}

void vec4_scale(vec4 v, float s, vec4 r)
{
	r[0] = v[0] * s;
	r[1] = v[1] * s;
	r[2] = v[2] * s;
	r[3] = v[3] * s;
}

float vec4_length(vec4 v)
{
	float r = 0;

	r += v[0] * v[0];
	r += v[1] * v[1];
	r += v[2] * v[2];
	r += v[3] * v[3];
	
	return sqrtf(r);
}

void vec4_normalize(vec4 v, vec4 r)
{
	float l = vec4_length(v);
	
	r[0] = v[0] / l;
	r[1] = v[1] / l;
	r[2] = v[2] / l;
	r[3] = v[3] / l;
}
