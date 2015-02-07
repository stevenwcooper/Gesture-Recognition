/**************************************************************************

	Author:Steven Cooper
	Project: Computer Vision for size_teractive Applications
	Version:1.0 Date:12/01/14 details: Creation of Matrix2D class
	Version:1.1 Date:13/01/14 details: Creation of +=, -=, *=, /= operator functions
	Version:1.2 Date:15/01/14 details: Created sum function

**************************************************************************/


#include <iostream>
#include <vector>
#include "Matrix1D.h"

#ifndef _MATRIX2D_

#define _MATRIX2D_

using namespace std;

namespace Matrix
{
	class Matrix2D
	{

	public:
		Matrix2D();
		Matrix2D(size_t n);
		Matrix2D(size_t n, size_t m);
		~Matrix2D();

		Matrix2D operator+(const Matrix2D&);
		Matrix2D operator+(const double);
		void operator+=(const Matrix2D&);
		void operator+=(const double);

		Matrix2D operator-(const Matrix2D&);
		Matrix2D operator-(const double);
		void operator-=(const Matrix2D&);
		void operator-=(const double);

		Matrix2D operator*(const Matrix2D&);
		Matrix2D operator*(const double);
		void operator*=(const Matrix2D&);
		void operator*=(const double);

		Matrix2D operator/(const Matrix2D&);
		Matrix2D operator/(const double);
		void operator/=(const Matrix2D&);
		void operator/=(const double);

		Matrix1D sum();

		vector<Matrix::Matrix1D> data;

	protected:

	private:



	};

}
#endif // !_MATRIX2D_
