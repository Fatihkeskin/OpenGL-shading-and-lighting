#include <iostream>

#define PI 3.14159265358979323846264338327950288419716939937510
#define toRadians (PI/180.0)

struct Vector3
{
	float x, y, z;
};
struct Vector2
{
	float x, y;
};
Vector3 Hector3(float x, float y, float z);


/* ------------------------------------------------------------------------------------------------------ */
/* These are the fundamental functions replacing the GLM functions */

/* multiply and update all elements of a vector with a floating number */
Vector3 vectorScalarMultiplication(Vector3 f, float s)
{
	Vector3 result;
	result.x = f.x*s;
	result.y = f.y*s;
	result.z = f.z*s;
	return result;
}

/* simply dot product */
float dotProduct(Vector3 f, Vector3 s)
{
	return f.x*s.x + f.y*s.y + f.z*s.z;
}


/* returns addition of 2 vectors */
Vector3 vectorAdditionWithReturn(Vector3 f, Vector3 s)
{
	Vector3 result;
	result.x = f.x + s.x;
	result.y = f.y + s.y;
	result.z = f.z + s.z;
	return result;

}

/* this function prevents the overload of float as i ask from the piazza */
GLfloat vectorLength(Vector3 vec)
{
	return (GLfloat)(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
}

/* normalize a vector but don't update it*/
Vector3 vectorNormalization(Vector3 vec)
{
	Vector3 result;
	float vecLength = vectorLength(vec);
	result.x = vec.x / vecLength;
	result.y = vec.y / vecLength;
	result.z = vec.z / vecLength;
	return result;
}


/* subtract second vector from the first vector */
Vector3 vectorSubtractionWithReturn(Vector3 f, Vector3 s)
{
	Vector3 result;
	result.x = f.x - s.x;
	result.y = f.y - s.y;
	result.z = f.z - s.z;
	return result;
}

/* cross product 2 vectors but return another */
Vector3 vectorCross(Vector3  f, Vector3  s)
{
	Vector3 result;

	result.x = f.y*s.z - f.z*s.y;
	result.y = f.z*s.x - f.x*s.z;
	result.z = f.x*s.y - f.y*s.x;

	return result;
}

/* add 2 4x4 matrices */
float *matrixAddition(float *a, float *b) {

	float *result = (float*)malloc(sizeof(float) * 16);

	for (int g = 0; g < 16; g++) {
		result[g] = (float)(a[g] + b[g]);
	}

	return result;
}

/* subtract second matrix from the first matrix */
float *matrixSubtraction(float * a, float * b) {

	float *result = (float*)malloc(sizeof(float) * 16);

	for (int g = 0; g < 16; g++) {
		result[g] = (float)(a[g] - b[g]);
	}

	return result;
}


/* check vectors result with print */
void printVectorThree(Vector3 a) {

	std::cout << a.x << ", " << a.y << ", " << a.z;

}

/* check matrix result with print */
void printMatrixFour(float *a) {

	std::cout << "Resulting 4x4 Matrix is =" << std::endl;
	std::cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << std::endl;
	std::cout << a[4] << ", " << a[5] << ", " << a[6] << ", " << a[7] << ", " << std::endl;
	std::cout << a[8] << ", " << a[9] << ", " << a[10] << ", " << a[11] << ", " << std::endl;
	std::cout << a[12] << ", " << a[13] << ", " << a[14] << ", " << a[15] << ", " << std::endl;

}

/* i calculated the lookat() by looking at stackoverflow (lookat function im going crazy) */
float *myLookAtFunction(Vector3 eyeVec, Vector3 centerVec, Vector3 upVec) {

	float *result = (float*)malloc(sizeof(float) * 16);

	Vector3 resultF = vectorNormalization(vectorSubtractionWithReturn( centerVec , eyeVec));
	Vector3 resultU = vectorNormalization(upVec);
	Vector3 resultS = vectorNormalization(vectorCross(resultF, resultU));
	resultU = vectorCross(resultS, resultF);

	result[0] = resultS.x;
	result[1] = resultU.x;
	result[2] = (-1)*resultF.x;
	result[3] = 0.0f;
	result[4] = resultS.y;
	result[5] = resultU.y;
	result[6] = (-1)*resultF.y;
	result[7] = 0.0f;
	result[8] = resultS.z;
	result[9] = resultU.z;
	result[10] = (-1)*resultF.z;
	result[11] = 0.0f; 
	result[12] = (-1)*dotProduct(resultS, eyeVec);
	result[13] = (-1)*dotProduct(resultU, eyeVec);
	result[14] = dotProduct(resultF, eyeVec);
	result[15] = 1.0f;

	return result;
}

/* equalize the first matrix with the second  */
void matrixEqualization(float *a, float *b) {

	for (int t = 0; t < 16; t++) {
		a[t] = b[t];
	}

}

/* create a vector struct with given parameters x,y,z */
Vector3 Hector3(GLfloat x, GLfloat y, GLfloat z)
{
	Vector3 tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}

/* projection function inspired by glm implementation */
float *myProjectionFunction(float angle, float screenRatio, float nearPlane, float farPlane) {

	float *result = (float*)malloc(sizeof(float) * 16);

	float tanFieldofView = tan(angle / 2);

	result[0] = (1 / (screenRatio * tanFieldofView));						//[0][0]
	result[1] = 0.0f;														//[0][1]
	result[2] = 0.0f;														//[0][2]
	result[3] = 0.0f;														//[0][3]
	result[4] = 0.0f;														//[1][0]
	result[5] = (1 / tanFieldofView);										//[1][1]
	result[6] = 0.0f;														//[1][2]
	result[7] = 0.0f;														//[1][3]
	result[8] = 0.0f;														//[2][0]
	result[9] = 0.0f;														//[2][1]
	result[10] = (-1)*(farPlane + nearPlane) / (farPlane - nearPlane);		//[2][2]
	result[11] = -1.0f;														//[2][3]
	result[12] = 0.0f;														//[3][0]
	result[13] = 0.0f;														//[3][1]
	result[14] = (-1)*(2 * nearPlane * farPlane) / (farPlane - nearPlane);	//[3][2]
	result[15] = 0.0f;														//[3][3]

	return result;

}



/* ------------------------------------------------------------------------------------------------------ */
