/*****************************************************************************

	Author:Steven Cooper
	Project: Computer Vision for Interactive Applications
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
	Matrix3D(int n);
	Matrix3D(int n, int m, int l);
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

Matrix3D::Matrix3D()
{
	data = vector<Matrix2D>();
}

Matrix3D::Matrix3D(int n)
{
	for(int i = 0; i < n; i++) data.push_back(Matrix2D(n));
}

Matrix3D::Matrix3D(int n, int m, int l)
{
	for(int i = 0; i < n; i++) data.push_back(Matrix2D(m,l));
}

Matrix3D::~Matrix3D()
{

}

Matrix3D Matrix3D::operator+(const Matrix3D& b)
{
	Matrix3D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) += b.data.at(i);
			}
			else temp.data.at(i) = b.data.at(i);
		}
	}
	else temp.data = b.data;

	return temp;
}

Matrix3D Matrix3D::operator+(const double b)
{
	Matrix3D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) += b;

	return temp;
}

void Matrix3D::operator+=(const Matrix3D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) += b.data.at(i);
			}
			else data.at(i) = b.data.at(i);
		}
	}
	else data = b.data;
}

void Matrix3D::operator+=(const double b)
{
	for(size_t i = 0; i < data.size(); i++) data.at(i) += b;
}

Matrix3D Matrix3D::operator-(const Matrix3D& b)
{
	Matrix3D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) -= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");

	return temp;
}

Matrix3D Matrix3D::operator-(const double b)
{
	Matrix3D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) -= b;

	return temp;
}

void Matrix3D::operator-=(const Matrix3D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) -= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");
}

void Matrix3D::operator-=(const double b)
{
	for(size_t i = 0; i < data.size(); i++) data.at(i) -= b;
}

Matrix3D Matrix3D::operator*(const Matrix3D& b)
{
	Matrix3D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) *= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");

	return temp;
}

Matrix3D Matrix3D::operator*(const double b)
{
	Matrix3D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) *= b;

	return temp;
}

void Matrix3D::operator*=(const Matrix3D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) *= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");
}

void Matrix3D::operator*=(const double b)
{
	for(size_t i = 0; i < data.size(); i++) data.at(i) *= b;
}

Matrix3D Matrix3D::operator/(const Matrix3D& b)
{
	Matrix3D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) /= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");

	return temp;
}

Matrix3D Matrix3D::operator/(const double b)
{
	Matrix3D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) /= b;

	return temp;
}

void Matrix3D::operator/=(const Matrix3D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) /= b.data.at(i);
			}
			else throw("row matrix has no data");
		}
	}
	else throw("matrix has no data");
}

void Matrix3D::operator/=(const double b)
{
	for(size_t i = 0; i < data.size(); i++) data.at(i) /= b;
}

}



#endif // !_MATRIX3D_
