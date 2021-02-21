#pragma once

#include <cmath>

// Pie is not round, pie are squared :)
#define PI		3.14159f

// Roundoff error to be ignored in floating point number comparisons
#define EPSILON 0.00001f

/**
* Structure represents a
* vector with 4 components
*/
union Vec2
{
	float e[2];

	struct
	{
		float x, y;
	};
};

union Vec3
{
	float e[3];

	struct
	{
		float x, y, z;
	};
};

union Vec4
{
	// array access to vector elements
	float e[4];

	// individual component access
	struct
	{
		float x, y, z, w;
	};
};

/**
* Constant world axis
*/
const Vec4 gc_xAxis = { 1.0f, 0.0f, 0.0f, 0.0f };
const Vec4 gc_yAxis = { 0.0f, 1.0f, 0.0f, 0.0f };
const Vec4 gc_zAxis = { 0.0f, 0.0f, 1.0f, 0.0f };

/**
* Structure represents a 4x4 matrix
* row major.
*/
typedef union Matrix4x4
{
	// NOTE - first 4 are _e11 - _e14
	float e[16];

	// Single elements in row major format
	struct
	{
		float _e11, _e12, _e13, _e14;
		float _e21, _e22, _e23, _e24;
		float _e31, _e32, _e33, _e34;
		float _e41, _e42, _e43, _e44;
	};
};

//////////////////////////////////////////////////////////////////////////
// All math functions. Comments in cpp file
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// General Utility functions
/////////////////////////////////////////////////////////////////////////

bool IsEqual(float a, float b);

bool IsZero(float a);

float Max(float a, float b);

float Min(float a, float b);

float Degrees_To_Radians(float Deg);

float Radians_To_Degrees(float Rad);

//////////////////////////////////////////////////////////////////////////
// Vector Functions
//////////////////////////////////////////////////////////////////////////

bool    Vector_IsEqual(Vec4 v, Vec4 w);

Vec4 Vector_Add(Vec4 v, Vec4 w);

Vec4 Vector_Sub(Vec4 v, Vec4 w);

Vec4 Vector_Scalar_Multiply(Vec4 v, float s);

Vec4 Vector_Negate(Vec4 v);

float   Vector_Dot(Vec4 v, Vec4 w);

Vec4 Vector_Cross(Vec4 v, Vec4 w);

float   Vector_LengthSq(Vec4 v);

float   Vector_Length(Vec4 v);

Vec4 Vector_Normalize(Vec4 v);

Vec4 Vector_Homogenise(Vec4 v);

Vec4 Vector_Maximize(Vec4 v, Vec4 w);

Vec4 Vector_Minimize(Vec4 v, Vec4 w);

Vec4 Vector_Average(Vec4 v, Vec4 w);

float   Vector_AngleBetween(Vec4 v, Vec4 w);

float   Vector_Component(Vec4 v, Vec4 w);

Vec4 Vector_Project(Vec4 v, Vec4 w);

Vec4 Vector_Reflect(Vec4 v, Vec4 w);

//////////////////////////////////////////////////////////////////////////
// Matrix Functions
//////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix_Zero(void);

Matrix4x4 Matrix_Identity(void);

Matrix4x4 Matrix_Create_Translation(float x, float y, float z);

Matrix4x4 Matrix_Create_Scale(float x, float y, float z);

Matrix4x4 Matrix_Create_Rotation_X(float Deg);

Matrix4x4 Matrix_Create_Rotation_Y(float Deg);

Matrix4x4 Matrix_Create_Rotation_Z(float Deg);

Matrix4x4 Matrix_Matrix_Add(Matrix4x4 m, Matrix4x4 n);

Matrix4x4 Matrix_Matrix_Sub(Matrix4x4 m, Matrix4x4 n);

Matrix4x4 Matrix_Scalar_Multiply(Matrix4x4 m, float s);

Matrix4x4 Matrix_Negate(Matrix4x4 m);

Matrix4x4 Matrix_Transpose(Matrix4x4 m);

Vec4 Vector_Matrix_Multiply(Vec4 v, Matrix4x4 m);

Vec4 Matrix_Vector_Multiply(Matrix4x4 m, Vec4 v);

Matrix4x4 Matrix_Matrix_Multiply(Matrix4x4 m, Matrix4x4 n);

float   Matrix_Determinant(Matrix4x4 m);

Matrix4x4 Matrix_Inverse(Matrix4x4 m);