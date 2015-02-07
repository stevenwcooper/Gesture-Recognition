#include "Matrix2D.h"

using namespace Matrix;
// constructor
Matrix2D::Matrix2D()
{
	data = vector<Matrix::Matrix1D>();
}

// constructor, input size n x n
Matrix2D::Matrix2D(size_t n)
{
	for(size_t i = 0; i < n; i++) data.push_back(Matrix1D(n));
}

// constructor, input size n x m
Matrix2D::Matrix2D(size_t n, size_t m)
{
	for(size_t i = 0; i < n; i++) data.push_back(Matrix1D(m));
}

Matrix2D::~Matrix2D()
{

}

// operator function for addition, input 2-D matrix
Matrix2D Matrix2D::operator+(const Matrix2D& b)
{
	Matrix2D temp;
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

// operator function for addition, input double
Matrix2D Matrix2D::operator+(const double b)
{
	Matrix2D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++)
		temp.data.at(i) += b;

	return temp;
}
// operator function for addition to self, input 2-D matrix

void Matrix2D::operator+=(const Matrix2D& b)
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

// operator function for addition to self, input double
void Matrix2D::operator+=(const double b)
{
	for(size_t i = 0; i < data.size(); i++) data.at(i) += b;
}

// operator function for subtraction, input 2-D matrix
Matrix2D Matrix2D::operator-(const Matrix2D& b)
{
	Matrix2D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) -= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");
	return temp;
}

// operator function for subtraction, input double
Matrix2D Matrix2D::operator-(const double b)
{
	Matrix2D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++)
		temp.data.at(i) -= b;

	return temp;
}

// operator function for subtraction to self, input 2-D matrix
void Matrix2D::operator-=(const Matrix2D& b)
{

	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) -= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");
}

// operator function for subtraction to self, input double
void Matrix2D::operator-=(const double b)
{

	for(size_t i = 0; i < data.size(); i++)
		data.at(i) -= b;

}

// operator function for multiplication, input 2-D matrix
Matrix2D Matrix2D::operator*(const Matrix2D& b)
{
	Matrix2D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) *= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");

	return temp;
}

// operator function for multiplication, input double
Matrix2D Matrix2D::operator*(const double b)
{
	Matrix2D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++)
		temp.data.at(i) *= b;

	return temp;
}

// operator function for multiplication to self, input 2-D matrix
void Matrix2D::operator*=(const Matrix2D& b)
{

	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) *= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");

}

// operator function for multiplication to self, input double
void Matrix2D::operator*=(const double b)
{

	for(size_t i = 0; i < data.size(); i++)
		data.at(i) *= b;
}

// operator function for divison, input 2-D matrix
Matrix2D Matrix2D::operator/(const Matrix2D& b)
{
	Matrix2D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			if(temp.data.at(i).data.size() > 0)
			{
				temp.data.at(i) /= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");

	return temp;
}

// operator function for division, input double
Matrix2D Matrix2D::operator/(const double b)
{
	Matrix2D temp;
	temp.data = data;
	for(size_t i = 0; i < temp.data.size(); i++)
		temp.data.at(i) /= b;

	return temp;
}

// operator function for division, input 2-D matrix
void Matrix2D::operator/=(const Matrix2D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			if(data.at(i).data.size() > 0)
			{
				data.at(i) /= b.data.at(i);
			}
			else throw("no data in column vector");
		}
	}
	else throw("no data in row vector");
}

// operator function for division to self, input double
void Matrix2D::operator/=(const double b)
{
	for(size_t i = 0; i < data.size(); i++)
		data.at(i) /= b;
}

// operator for sum of matrix
Matrix1D Matrix2D::sum()
{
	Matrix1D temp = Matrix1D(((size_t)data.at(0).data.size()));
	for(size_t i = 0; i < data.at(0).data.size(); i++)
		for(size_t j = 0; j < data.size(); j++) temp.data.at(i) += data.at(j).data.at(i);

	return temp;
}