#include "matrix.h"

void mat4_zero(mat4 m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			m[i][j] = 0.0f;
		}
	}
}
 
void mat4_identity(mat4 m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
}

void mat4_multiply(mat4 m1, mat4 m2, mat4 r)
{
	for(int i = 0; i < 4; i++)
	{	
		for(int j = 0; j < 4; j++)
		{
			r[i][j] = 0.0;

			for(int k = 0; k < 4; k++){
				r[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}

void mat4_translate(mat4 m, vec3 v, mat4 r)
{
	vec4_copy(m[0], r[0]);
	vec4_copy(m[1], r[1]);
	vec4_copy(m[2], r[2]);

	vec4 m3 = { 0 };
	
	for(int i = 0; i < 3; i++)
	{
		vec4 tmp;
		vec4_scale(m[i], v[i], tmp);
		vec4_add(m3, tmp, m3);
	}

	vec4_add(m3, m[3], m3);
	vec4_copy(m3, r[3]);	 
}
	
void mat4_rotate(mat4 m, float angle, vec3 v, mat4 r)
{
	float c = cos(angle);
	float s = sin(angle);

	vec3 axis, tmp;
	vec3_normalize(v, axis);
	vec3_scale(axis, 1.0f - c, tmp);

	mat4 rot = { 0 };

	rot[0][0] = c + tmp[0] * axis[0];
	rot[0][1] = tmp[0] * axis[1] + s * axis[2];
	rot[0][2] = tmp[0] * axis[2] - s * axis[1];

	rot[1][0] = tmp[1] * axis[0] - s * axis[2];
	rot[1][1] = c + tmp[1] * axis[1];
	rot[1][2] = tmp[1] * axis[2] + s * axis[0];

	rot[2][0] = tmp[2] * axis[0] + s * axis[1];
	rot[2][1] = tmp[2] * axis[1] - s * axis[0];
	rot[2][2] = c + tmp[2] * axis[2];

	for(int i = 0; i < 3; i++)
	{
		vec4 col = { 0 };
		
		for(int j = 0; j < 3; j++)
		{
			vec4 cell = { 0 };
			vec4_scale(m[j], rot[i][j], cell);
			vec4_add(col, cell, col);
		}

		vec4_copy(col, r[i]);
	}
	vec4_copy(m[3], r[3]);
} 

void mat4_scale(mat4 m, vec3 v, mat4 r)
{
	vec4_scale(m[0], v[0], r[0]);
	vec4_scale(m[1], v[1], r[1]);
	vec4_scale(m[2], v[2], r[2]);
	vec4_copy(m[3], r[3]);
}

void mat4_lookat(vec3 eye, vec3 center, vec3 up, mat4 r)
{
	vec3 f, s, u, tmp;

	vec3_scale(eye, -1.0f, tmp);
	vec3_add(center, tmp, tmp);
	vec3_normalize(tmp, f);

	vec3_cross(up, f, tmp);
	vec3_normalize(tmp, s);
	
	vec3_cross(f, s, u);

 	r[0][0] = s[0];
	r[1][0] = s[1];
	r[2][0] = s[2];
	r[3][0] = -vec3_dot(s, eye);

	r[0][1] = u[0];
	r[1][1] = u[1];
	r[2][1] = u[2];
	r[3][1] = -vec3_dot(u, eye);

	r[0][2] = f[0];
	r[1][2] = f[1];
	r[2][2] = f[2];
	r[3][2] = -vec3_dot(f, eye);
	
	r[0][3] = 0.0f;
	r[1][3] = 0.0f;	
	r[2][3] = 0.0f;
	r[3][3] = 1.0f;
}

void mat4_perspective(float fov, float aspect, float near, float far, mat4 r)
{
	r[0][0] = 1.0f / (aspect * tanf(fov / 2.0));
	r[0][1] = 0.0f;
	r[0][2] = 0.0f;
	r[0][3] = 0.0f;

	r[1][0] = 0.0f;
	r[1][1] = 1.0f / tanf(fov / 2.0);
	r[1][2] = 0.0f;
	r[1][3] = 0.0f;

	r[2][0] = 0.0f;
	r[2][1] = 0.0f;
	r[2][2] = (far + near) / (far - near);
	r[2][3] = -1.0f;
	
	r[3][0] = 0.0f;
	r[3][1] = 0.0f;	
	r[3][2] = -(2.0f * far * near) / (far - near);
	r[3][3] = 0.0f;
}
