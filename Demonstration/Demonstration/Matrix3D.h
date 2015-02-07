/*****************************************************************************

	Author:Steven Cooper
	Project: Computer Vision for size_teractive Applications
	Version:1.0 Date:13/01/14 Details: Creation of Matrix3D class
	Verison:1.1 Date:13/01/14 Details: Creation of +=, -=, *=, /= operator functions

*****************************************************************************/

#include <iostream>
#include "Matrix2D.h"

using namespace std;

#ifndef _MATRIX3D_

#define _MATRIX3D_

namespace Matrix
{

class Matrix3D
{

public:
	Matrix3D();
	Matrix3D(size_t n);
	Matrix3D(size_t n, size_t m, size_t l);
	~Matrix3D();

	Matrix3D operator+(const Matrix3D&);
	Matrix3D operator+(const double);
	void operator+=(const Matrix3D&);
	void operator+=(const double);

	Matrix3D operator-(const Matrix3D&);
	Matrix3D operator-(const double);
	void operator-=(const Matrix3D&);
	void operator-=(const double);

	Matrix3D operator*(const Matrix3D&);
	Matrix3D operator*(const double);
	void operator*=(const Matrix3D&);
	void operator*=(const double);

	Matrix3D operator/(const Matrix3D&);
	Matrix3D operator/(const double);
	void operator/=(const Matrix3D&);
	void operator/=(const double);

	vector<Matrix2D> data;
protected:

private:

};

}



#endif // !_MATRIX3D_
