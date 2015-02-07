#include "Matrix3D.h"

using namespace Matrix;

Matrix3D::Matrix3D()
{
	data = vector<Matrix2D>();
}

Matrix3D::Matrix3D(size_t n)
{
	for(size_t i = 0; i < n; i++) data.push_back(Matrix2D(n));
}

Matrix3D::Matrix3D(size_t n, size_t m, size_t l)
{
	for(size_t i = 0; i < n; i++) data.push_back(Matrix2D(m,l));
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