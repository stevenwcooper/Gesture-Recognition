/*****************************************************************************

Author:Steven Cooper
Project: Computer Vision for size_teractive Applications
Version:1.0 Date:12/01/14 details: Creation of Matrix1D class
Verison:1.1 Date:13/01/14 details: Creation of +=, -=, *=, /= operator functions
Version:1.2 Date:15/01/14 details: Created sum function

*****************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

#ifndef _MATRIX1D_

#define _MATRIX1D_

namespace Matrix
{

	class Matrix1D
	{

	public:
		Matrix1D();
		Matrix1D(size_t n);
		~Matrix1D();
		vector<double> data;

		Matrix1D operator+(const Matrix1D&);
		Matrix1D operator+(const double);
		void operator+=(const Matrix1D&);
		void operator+=(const double);

		Matrix1D operator-(const Matrix1D&);
		Matrix1D operator-(const double);
		void operator-=(const Matrix1D&);
		void operator-=(const double);

		Matrix1D operator*(const Matrix1D&);
		Matrix1D operator*(const double);
		void operator*=(const Matrix1D&);
		void operator*=(const double);

		Matrix1D operator/(const Matrix1D&);
		Matrix1D operator/(const double);
		void operator/=(const Matrix1D&);
		void operator/=(const double);

		double sum();

	protected:

	private:

	};

}
#endif // !_MATRIX1D
