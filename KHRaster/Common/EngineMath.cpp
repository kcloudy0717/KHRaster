#include "EngineMath.h"

//////////////////////////////////////////////////////////////////////////
// Common math functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// General Utility functions
//////////////////////////////////////////////////////////////////////////

// Are two floating point numbers equal to each other
// Floating Point Error Safe
//
// IN:		a		The first number
//			b		The second number
//
// RETURN: TRUE iff |a-b| < Tolerance
//
// NOTE:	EPSILON is tolerance
bool IsEqual(float a, float b)
{
	// NOTE: Do not modify.
	return fabs(a - b) < EPSILON;
}

// Is a floating point value equal to zero
// Floating Point Error Safe
//
// IN:		a		The number to check
//
// RETURN:	TRUE iff |a| < Tolerance
//
// NOTE:	Tolerance set by EPSILON
bool IsZero(float a)
{
	// NOTE: Do not modify
	return (fabs(a)) < EPSILON;
}

// RETURN: MAX of two numbers
float Max(float a, float b)
{
	// NOTE: Do not modify.
	return (a > b) ? a : b;
}

// RETURN: MIN of two numbers
float Min(float a, float b)
{
	// NOTE: Do not modify.
	return (a < b) ? a : b;
}

// RETURN: Converts input to radian measure
float Degrees_To_Radians(float Deg)
{
	// NOTE: Do not modify.
	return Deg * PI / 180.0f;
}

// RETURN: Converts input to degree measure
float Radians_To_Degrees(float Rad)
{
	// NOTE: Do not modify.
	return Rad * 180.0f / PI;
}
//////////////////////////////////////////////////////////////////////////
// Vector Functions
//////////////////////////////////////////////////////////////////////////

// Check if two Vec4's are equal to each other
//
// IN:		v		First Vector
//			w		Second Vector
//
// RETURN:  True if v==w, False otherwise
//
// NOTE:	Use's all four components
//			Should be floating point error safe.
bool Vector_IsEqual(Vec4 v, Vec4 w)
{
	if (IsEqual(v.x, w.x) && IsEqual(v.y, w.y) && IsEqual(v.z, w.z) && IsEqual(v.w, w.w))
		return true;
	return false;
}

// ADD two Vec4's togother
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v + w
//
// NOTE:	Use's all four components
Vec4 Vector_Add(Vec4 v, Vec4 w)
{
	Vec4 addVec;
	for (size_t i = 0; i < 4; i++)
	{
		addVec.e[i] = v.e[i] + w.e[i];
	}
	return addVec;
}

// SUBTRACT one Vec4 from another
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v - w
//
// NOTE:	Use's all four components
Vec4 Vector_Sub(Vec4 v, Vec4 w)
{
	Vec4 subtractVec;
	for (size_t i = 0; i < 4; i++)
	{
		subtractVec.e[i] = v.e[i] - w.e[i];
	}
	return subtractVec;
}

// MULTIPLY all four components of a Vec4 by a scalar
//
// IN:		v		The vector to scale
//			s		The value to scale by
//
// RETURN:  s * v
Vec4 Vector_Scalar_Multiply(Vec4 v, float s)
{
	Vec4 scalarMultiplyVector;
	for (size_t i = 0; i < 4; i++)
	{
		scalarMultiplyVector.e[i] = s * v.e[i];
	}
	return scalarMultiplyVector;
}

// NEGATE all the components of a Vec4
//
// IN:		v		The vector to negate
//
// RETURN:	-1 * v
//
// NOTE:	Use's all four components
Vec4 Vector_Negate(Vec4 v)
{
	Vec4 negateVector;
	for (size_t i = 0; i < 4; i++)
	{
		negateVector.e[i] = v.e[i] * -1.0f;
	}
	return negateVector;
}

// Perform a Dot Product on two Vec4's
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v (DOT) w
//
// NOTE:	Use's all four components
float Vector_Dot(Vec4 v, Vec4 w)
{
	float dotProduct = 0.0f;
	for (size_t i = 0; i < 4; i++)
	{
		dotProduct += (v.e[i] * w.e[i]);
	}
	return dotProduct;
}

// Perform a Cross Product on two Vec4's
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v (CROSS) w
//
// NOTE:	The w-component of each vector is not used.
//			The resultant vector will have a w-component of zero.
Vec4 Vector_Cross(Vec4 v, Vec4 w)
{
	Vec4 crossProduct;
	crossProduct.x = (v.y * w.z) - (v.z * w.y);
	crossProduct.y = -((v.x * w.z) - (v.z * w.x));
	crossProduct.z = (v.x * w.y) - (v.y * w.x);
	crossProduct.w = 0.0f;
	return crossProduct;
}

// Find the squared length of a Vec4
//
// IN:		v		The vector to find the squared length of
//
// RETURN:	Squared Length of Vec4
//
// NOTE:	Use's all four components
float Vector_LengthSq(Vec4 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

// Find the length of a Vec4
//
// IN:		v		The vector to find the length of
//
// RETURN:	Length of Vec4
//
// NOTE:	Use's all four components
float Vector_Length(Vec4 v)
{
	return sqrtf(Vector_LengthSq(v));
}

// Normalize a Vec4
//
// IN:		v		The vector to normalize
//
// RETURN:	Normalized version of v
//
// NOTE:	Use's all four components
Vec4 Vector_Normalize(Vec4 v)
{
	Vec4 normalizeVec = {};
	if (IsZero(Vector_Length(v)))
	{
		return normalizeVec;
	}

	for (size_t i = 0; i < 4; i++)
	{
		normalizeVec.e[i] = v.e[i] / Vector_Length(v);
	}

	return normalizeVec;
}

// Makes a Vec4's w-component normalized
//
// IN:		v		The vector (point object) to homogenise
//
// RETURN:	The homogenised vector (point)
//
// NOTE:	If the w-component of the vector is 0 then the
//			function will return a zero vector with a w-component
//			of 0.
Vec4 Vector_Homogenise(Vec4 v)
{
	Vec4 homogeniseVec = {};
	if (IsZero(v.w))
	{
		return homogeniseVec;
	}

	for (size_t i = 0; i < 4; i++)
	{
		homogeniseVec.e[i] = v.e[i] / v.w;
	}

	return homogeniseVec;
}

// Get a Vec4 made from the maximun components of two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A maximized vector
//
// NOTE:	Use's all four components
Vec4 Vector_Maximize(Vec4 v, Vec4 w)
{
	Vec4 maxVector = {};
	for (size_t i = 0; i < 4; i++)
	{
		maxVector.e[i] = Max(v.e[i], w.e[i]);
	}

	return maxVector;
}

// Get a Vec4 made from the minimum components of two Vec4's
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A minimum vector
//
// NOTE:	Use's all four components
Vec4 Vector_Minimize(Vec4 v, Vec4 w)
{
	Vec4 minVector = {};
	for (size_t i = 0; i < 4; i++)
	{
		minVector.e[i] = Min(v.e[i], w.e[i]);
	}

	return minVector;
}

// Get a Vec4 made from the average of two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A vector made from the average of two vectors
//
// NOTE:	Use's all four components

Vec4 Vector_Average(Vec4 v, Vec4 w)
{
	Vec4 avgVector = {};
	for (size_t i = 0; i < 4; i++)
	{
		avgVector.e[i] = (v.e[i] + w.e[i]) / 2.0f;
	}
	return avgVector;
}

// Find the angle between two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:  The angle in degrees between the two vectors
//
// NOTE:	If either vector is a zero vector then the return
//			value will be 0.
float Vector_AngleBetween(Vec4 v, Vec4 w)
{
	float angle = acosf(Vector_Dot(v, w) / ((Vector_Length(v) * Vector_Length(w))));
	return Radians_To_Degrees(angle);
}

// Get the distance one Vec4 points in the direction of another
// Vec4
//
// IN:		v		The first vector
//			w		The direction of the component
//
// RETURN:	The distance that v points in the direction of w.
//
// NOTE:	If w or v is a zero vector then the return value is zero.
float Vector_Component(Vec4 v, Vec4 w)
{
	float compVec = 0.0f;
	if ((IsZero(v.x) && IsZero(v.y) && IsZero(v.z) && IsZero(v.w)) || (IsZero(w.x) && IsZero(w.y) && IsZero(w.z) && IsZero(w.w)))
		return 0.0f;

	compVec = Vector_Dot(v, Vector_Normalize(w));

	return compVec;
}

// Get the Vec4 that represents v projected on w.
//
// IN:		v		The first vector
//			w		The direction of the projection
//
// RETURN:	The projection of v onto w
//
// NOTE:	If w or v is a zero vector then the return value is zero.
Vec4 Vector_Project(Vec4 v, Vec4 w)
{
	// TODO LAB 1: Replace with your implementation.
	Vec4 projVec = {};
	if ((IsZero(v.x) && IsZero(v.y) && IsZero(v.z) && IsZero(v.w)) || (IsZero(w.x) && IsZero(w.y) && IsZero(w.z) && IsZero(w.w)))
	{
		return projVec;
	}

	for (size_t i = 0; i < 4; i++)
	{
		projVec.e[i] = Vector_Component(v, w) * Vector_Normalize(w).e[i];
	}

	return projVec;
}

////////////////////////////////////////////////////////////////////////
// Functions Lab  #2
///////////////////////////////////////////////////////////////////////

// Get the reflection of v across w
//
// IN:		v		The vector to reflect
//			w		The "axis" to reflect across
//
// RETURN:	v reflected across w
//
// NOTE:	If w is a zero vector then return -v.
Vec4 Vector_Reflect(Vec4 v, Vec4 w)
{
	Vec4 reflectVec;
	for (size_t i = 0; i < 4; i++) // reflection of a vector = v - 2(v (DOT) Normalize(w))(Normalize(w))
	{
		reflectVec.e[i] = -(v.e[i] - (2 * Vector_Dot(v, Vector_Normalize(w)) * Vector_Normalize(w).e[i]));
	}
	return reflectVec;
}

//////////////////////////////////////////////////////////////////////////
// Matrix Functions
//////////////////////////////////////////////////////////////////////////

// Get a [0] matrix
//
// RETURN: A 0 4x4 matrix
Matrix4x4 Matrix_Zero(void)
{
	Matrix4x4 zeroMatrix = {};
	return zeroMatrix;
}

// Get a [I] matrix
//
// RETURN: A 4x4 Identity matrix
Matrix4x4 Matrix_Identity(void)
{
	Matrix4x4 identityMatrix = {1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f};
	return identityMatrix;
}

// Get a translation matrix
//
// IN:		x		Amount of translation in the x direction
//			y		Amount of translation in the y direction
//			z		Amount of translation in the z direction
//
// RETURN:	The translation matrix
Matrix4x4 Matrix_Create_Translation(float x, float y, float z)
{
	Matrix4x4 translationMatrix = {1.0f, 0.0f, 0.0f, 0.0f,
								   0.0f, 1.0f, 0.0f, 0.0f,
								   0.0f, 0.0f, 1.0f, 0.0f,
								   x, y, z, 1.0f};
	return translationMatrix;
}

// Create a scale matrix
//
// IN:		x		Amount to scale in the x direction
//			y		Amount to scale in the y direction
//			z		Amount to scale in the z direction
//
// RETURN:	The scale matrix
Matrix4x4 Matrix_Create_Scale(float x, float y, float z)
{
	Matrix4x4 scaleMatrix = {x, 0.0f, 0.0f, 0.0f,
							 0.0f, y, 0.0f, 0.0f,
							 0.0f, 0.0f, z, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f};
	return scaleMatrix;
}

// Get a rotation matrix for rotation about the x-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A X-Rotation Matrix
Matrix4x4 Matrix_Create_Rotation_X(float Deg)
{
	Deg = Degrees_To_Radians(Deg);
	Matrix4x4 RxMatrix = {1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, cosf(Deg), -sinf(Deg), 0.0f,
						  0.0f, sinf(Deg), cosf(Deg), 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f};
	return RxMatrix;
}

// Get a rotation matrix for rotation about the y-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A Y-Rotation Matrix
Matrix4x4 Matrix_Create_Rotation_Y(float Deg)
{
	Deg = Degrees_To_Radians(Deg);
	Matrix4x4 RyMatrix = {cosf(Deg), 0.0f, sinf(Deg), 0.0f,
						  0.0f, 1.0f, 0.0f, 0.0f,
						  -sinf(Deg), 0.0f, cosf(Deg), 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f};
	return RyMatrix;
}

// Get a rotation matrix for rotation about the z-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A Z-Rotation Matrix
Matrix4x4 Matrix_Create_Rotation_Z(float Deg)
{
	Deg = Degrees_To_Radians(Deg);
	Matrix4x4 RzMatrix = {cosf(Deg), -sinf(Deg), 0.0f, 0.0f,
						  sinf(Deg), cosf(Deg), 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f};
	return RzMatrix;
}

// ADD two matrices together
//
// IN:		m		The first matrix
//			n		The second matrix
//
// RETURN: m + n
Matrix4x4 Matrix_Matrix_Add(Matrix4x4 m, Matrix4x4 n)
{
	Matrix4x4 addMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		addMatrix.e[i] = m.e[i] + n.e[i];
	}
	return addMatrix;
}

// SUBTRACT two matrices
//
// IN:		m		The first matrix (left hand side)
//			n		The second matrix (right hand side)
//
// RETURN: m - n
Matrix4x4 Matrix_Matrix_Sub(Matrix4x4 m, Matrix4x4 n)
{
	Matrix4x4 subtractMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		subtractMatrix.e[i] = m.e[i] - n.e[i];
	}
	return subtractMatrix;
}

// Multiply a matrix by a scalar
//
// IN:		m		The matrix to be scaled (right hand side)
//			s		The value to scale by   (left hand side)
//
// RETURN:	The matrix formed by s*[m]
Matrix4x4 Matrix_Scalar_Multiply(Matrix4x4 m, float s)
{
	Matrix4x4 scalarMultiplyMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		scalarMultiplyMatrix.e[i] = s * m.e[i];
	}
	return scalarMultiplyMatrix;
}

// Negate a matrix
//
// IN:		m		The matrix to negate
//
// RETURN:  The negation of m
Matrix4x4 Matrix_Negate(Matrix4x4 m)
{
	Matrix4x4 negateMatrix;
	for (size_t i = 0; i < 16; i++)
	{
		negateMatrix.e[i] = -1.0f * m.e[i];
	}
	return negateMatrix;
}

// Transpose a matrix
//
// IN:		m		The matrix to transpose
//
// RETURN:	The transpose of m
Matrix4x4 Matrix_Transpose(Matrix4x4 m)
{
	Matrix4x4 transposeMatrix = {m._e11, m._e21, m._e31, m._e41,
								 m._e12, m._e22, m._e32, m._e42,
								 m._e13, m._e23, m._e33, m._e43,
								 m._e14, m._e24, m._e34, m._e44};
	return transposeMatrix;
}

// Multipy a matrix and a vector
//
// IN:		m		The matrix (left hand side)
//			v		The vector (right hand side)
//
// RETURN:	[m]*v
Vec4 Matrix_Vector_Multiply(Matrix4x4 m, Vec4 v)
{
	Vec4 MVMultiply = {};
	float product = 0.0f;
	size_t j = 0;
	for (size_t i = 0; i < 4; i++) // The entire matrix
	{
		size_t rowEntry = 0;
		for (; j < 16; j++, rowEntry++) // Multiplying the each rows
		{
			product += (m.e[j] * v.e[rowEntry]);
			if (j == 0) // Ignore first index...
				continue;

			if ((j + 1) % 4 == 0) // Once all the values from one row has been add up and put into new matrix move on to next row
			{
				MVMultiply.e[i] = product;
				product = 0.0f;
				rowEntry = 0;
				j++;
				break;
			}
		}
	}
	return MVMultiply;
}

// Multipy a vector and a matrix
//
// IN:		v		The vector ( left hand side)
//			m		The matrix (right hand side)
//
// RETURN:	v*[m]
Vec4 Vector_Matrix_Multiply(Vec4 v, Matrix4x4 m)
{
	return Matrix_Vector_Multiply(Matrix_Transpose(m), v);
}
// Multiply a matrix by a matrix
//
// IN:		m		First Matrix (left hand side)
//			n		Second Matrix (right hand side)
//
// RETURN:	[m]*[n]
Matrix4x4 Matrix_Matrix_Multiply(Matrix4x4 m, Matrix4x4 n)
{
	Matrix4x4 matrixMultiply = {(m._e11 * n._e11) + (m._e12 * n._e21) + (m._e13 * n._e31) + (m._e14 * n._e41), (m._e11 * n._e12) + (m._e12 * n._e22) + (m._e13 * n._e32) + (m._e14 * n._e42), (m._e11 * n._e13) + (m._e12 * n._e23) + (m._e13 * n._e33) + (m._e14 * n._e43), (m._e11 * n._e14) + (m._e12 * n._e24) + (m._e13 * n._e34) + (m._e14 * n._e44),
								(m._e21 * n._e11) + (m._e22 * n._e21) + (m._e23 * n._e31) + (m._e24 * n._e41), (m._e21 * n._e12) + (m._e22 * n._e22) + (m._e23 * n._e32) + (m._e24 * n._e42), (m._e21 * n._e13) + (m._e22 * n._e23) + (m._e23 * n._e33) + (m._e24 * n._e43), (m._e21 * n._e14) + (m._e22 * n._e24) + (m._e23 * n._e34) + (m._e24 * n._e44),
								(m._e31 * n._e11) + (m._e32 * n._e21) + (m._e33 * n._e31) + (m._e34 * n._e41), (m._e31 * n._e12) + (m._e32 * n._e22) + (m._e33 * n._e32) + (m._e34 * n._e42), (m._e31 * n._e13) + (m._e32 * n._e23) + (m._e33 * n._e33) + (m._e34 * n._e43), (m._e31 * n._e14) + (m._e32 * n._e24) + (m._e33 * n._e34) + (m._e34 * n._e44),
								(m._e41 * n._e11) + (m._e42 * n._e21) + (m._e43 * n._e31) + (m._e44 * n._e41), (m._e41 * n._e12) + (m._e42 * n._e22) + (m._e43 * n._e32) + (m._e44 * n._e42), (m._e41 * n._e13) + (m._e42 * n._e23) + (m._e43 * n._e33) + (m._e44 * n._e43), (m._e41 * n._e14) + (m._e42 * n._e24) + (m._e43 * n._e34) + (m._e44 * n._e44)};
	return matrixMultiply;
}

////////////////////////////////////////////////////////////////////////
// Matrix Functions Lab # 3
///////////////////////////////////////////////////////////////////////

// HELPER FUNCTION  *** NOT GRADED, ONLY SUGGESTED ***
// USE THIS FUNCTION TO FIND THE DETERMINANT OF A 3*3
// MATRIX. IT CAN BE USED IN THE MATRIX DETERMINANT
// AND MATRIX INVERSE FUNCTIONS BELOW
//
// RETURN:	The determinant of a 3x3 matrix
float Matrix_Determinant(float e_11, float e_12, float e_13,
						 float e_21, float e_22, float e_23,
						 float e_31, float e_32, float e_33)
{
	return (e_11 * ((e_22 * e_33) - (e_32 * e_23))) - (e_12 * ((e_21 * e_33) - (e_31 * e_23))) + (e_13 * ((e_21 * e_32) - (e_31 * e_22)));
}

// Get the determinant of a matrix
//
// IN:		m		The ONE!
//
// RETURN:	It's deterinant
float Matrix_Determinant(Matrix4x4 m)
{
	return (m._e11 * (Matrix_Determinant(m._e22, m._e23, m._e24, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44))) - (m._e12 * (Matrix_Determinant(m._e21, m._e23, m._e24, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44))) + (m._e13 * Matrix_Determinant(m._e21, m._e22, m._e24, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44)) - (m._e14 * (Matrix_Determinant(m._e21, m._e22, m._e23, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43)));
}

// Get the inverse of a matrix
//
// IN:		m		The matrix to inverse
//
// RETURN:	The Inverse of [m]
//
// NOTE: Returns the matrix itself if m is not invertable.
Matrix4x4 Matrix_Inverse(Matrix4x4 m)
{
	Matrix4x4 inverseMatrix;
	if (Matrix_Determinant(m) == 0.0f)
		return m;

	inverseMatrix = {Matrix_Determinant(m._e22, m._e23, m._e24, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44), (-1.0f * (Matrix_Determinant(m._e21, m._e23, m._e24, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44))), Matrix_Determinant(m._e21, m._e22, m._e24, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44), (-1.0f * (Matrix_Determinant(m._e21, m._e22, m._e23, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43))),
					 (-1.0f * (Matrix_Determinant(m._e12, m._e13, m._e14, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44))), Matrix_Determinant(m._e11, m._e13, m._e14, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44), (-1.0f * Matrix_Determinant(m._e11, m._e12, m._e14, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44)), Matrix_Determinant(m._e11, m._e12, m._e13, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43),
					 Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e42, m._e43, m._e44), (-1.0f * (Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e41, m._e43, m._e44))), Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e41, m._e42, m._e44), (-1.0f * (Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e41, m._e42, m._e43))),
					 (-1.0f * (Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e32, m._e33, m._e34))), Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e31, m._e33, m._e34), (-1.0f * (Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e31, m._e32, m._e34))), Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e31, m._e32, m._e33)};

	return Matrix_Scalar_Multiply(Matrix_Transpose(inverseMatrix), (1.0f / Matrix_Determinant(m)));
}