#include "Matrix1D.h"

using namespace Matrix;

// Constructor
Matrix1D::Matrix1D()
{
	data = vector<double>();
}

// Constructor , input Matrix size
Matrix1D::Matrix1D(size_t n)
{
	data = vector<double>(n);
}

Matrix1D::~Matrix1D()
{

}

// operator function for addition, input 1-D matrix
Matrix1D Matrix1D::operator+(const Matrix1D& b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) += b.data.at(i); 
	}
	else temp.data = b.data;

	return temp;
}

// operator function for addition, input double
Matrix1D Matrix1D::operator+(const double b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) += b;
	}
	else throw("No vector data");

	return temp;
}

// operator for additon to self, input 1-D matrix
void Matrix1D::operator+=(const Matrix1D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) += b.data.at(i);
	}
	else data = b.data;
}

// operator for additon to self, input double
void Matrix1D::operator+=(const double b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) += b; 
	}
	else throw("No vector data");
}

// operator for subtraction, input 1-D matrix
Matrix1D Matrix1D::operator-(const Matrix1D& b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) -= b.data.at(i); 
	}
	else throw("No vector data");

	return temp;
}

// operator for subtraction, input double
Matrix1D Matrix1D::operator-(const double b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) -= b;
	}
	else throw("No vector data");

	return temp;
}

// operator for subtraction to self, input 1-D matrix
void Matrix1D::operator-=(const Matrix1D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) -= b.data.at(i);
	}
	else data = b.data;
}

// operator for subtraction to self, input double
void Matrix1D::operator-=(const double b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) -= b; 
	}
	else throw("No vector data");
}

// operator for multiplication, input 1-D matrix
Matrix1D Matrix1D::operator*(const Matrix1D& b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) *= b.data.at(i); 
	}
	else throw("No vector data");

	return temp;
}

// operator for multiplication, input double
Matrix1D Matrix1D::operator*(const double b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) temp.data.at(i) *= b;
	}
	else throw("No vector data");

	return temp;
}

// operator for multiplication to self, input 1-D matrix
void Matrix1D::operator*=(const Matrix1D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) *= b.data.at(i);
	}
	else data = b.data;
}

// operator for multiplication to self, input double
void Matrix1D::operator*=(const double b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) *= b; 
	}
	else throw("No vector data");
}

// operator for division, input 1-D matrix
Matrix1D Matrix1D::operator/(const Matrix1D& b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++) 
		{
			temp.data.at(i) /= b.data.at(i); 
			if(temp.data.at(i) < 0.00001) temp.data.at(i) = 0.0;
		}
	}
	else throw("No vector data");

	return temp;
}

// operator for divison, input double
Matrix1D Matrix1D::operator/(const double b)
{
	Matrix1D temp;
	temp.data = data;
	if(temp.data.size() > 0)
	{
		for(size_t i = 0; i < temp.data.size(); i++)
		{
			temp.data.at(i) /= b;
			if(temp.data.at(i) < 0.00001) temp.data.at(i) = 0.0;
		}
	}
	else throw("No vector data");

	return temp;
}

// operator for divsion to self, input 1-D matrix
void Matrix1D::operator/=(const Matrix1D& b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++)
		{
			data.at(i) /= b.data.at(i);
			if(data.at(i) < 0.00001) data.at(i) = 0.0;
		}
	}
	else data = b.data;
}

// operator for divsion to self, input double
void Matrix1D::operator/=(const double b)
{
	if(data.size() > 0)
	{
		for(size_t i = 0; i < data.size(); i++) 
		{
			data.at(i) /= b; 
			if(data.at(i) < 0.00001) data.at(i) = 0.0;
		}
	}
	else throw("No vector data");
}

// operator to sum matrix
double Matrix1D::sum()
{
	double temp = 0.0;
	for(size_t i = 0; i < data.size(); i++) temp += data.at(i);
	return temp;
}