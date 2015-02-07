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


	Matrix2D::Matrix2D()
	{
		data = vector<Matrix::Matrix1D>();
	}

	Matrix2D::Matrix2D(size_t n)
	{
		for(size_t i = 0; i < n; i++) data.push_back(Matrix1D(n));
	}

	Matrix2D::Matrix2D(size_t n, size_t m)
	{
		for(size_t i = 0; i < n; i++) data.push_back(Matrix1D(m));
	}

	Matrix2D::~Matrix2D()
	{

	}

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

	Matrix2D Matrix2D::operator+(const double b)
	{
		Matrix2D temp;
		temp.data = data;
		for(size_t i = 0; i < temp.data.size(); i++)
			temp.data.at(i) += b;

		return temp;
	}

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

	void Matrix2D::operator+=(const double b)
	{
		for(size_t i = 0; i < data.size(); i++) data.at(i) += b;
	}

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

	Matrix2D Matrix2D::operator-(const double b)
	{
		Matrix2D temp;
		temp.data = data;
		for(size_t i = 0; i < temp.data.size(); i++)
			temp.data.at(i) -= b;

		return temp;
	}

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

	void Matrix2D::operator-=(const double b)
	{

		for(size_t i = 0; i < data.size(); i++)
			data.at(i) -= b;

	}

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

	Matrix2D Matrix2D::operator*(const double b)
	{
		Matrix2D temp;
		temp.data = data;
		for(size_t i = 0; i < temp.data.size(); i++)
			temp.data.at(i) *= b;

		return temp;
	}

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

	void Matrix2D::operator*=(const double b)
	{

		for(size_t i = 0; i < data.size(); i++)
			data.at(i) *= b;
	}

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

	Matrix2D Matrix2D::operator/(const double b)
	{
		Matrix2D temp;
		temp.data = data;
		for(size_t i = 0; i < temp.data.size(); i++)
			temp.data.at(i) /= b;

		return temp;
	}
	
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

	void Matrix2D::operator/=(const double b)
	{
		for(size_t i = 0; i < data.size(); i++)
			data.at(i) /= b;
	}

	Matrix1D Matrix2D::sum()
	{
		Matrix1D temp = Matrix1D(((size_t)data.at(0).data.size()));
		for(size_t i = 0; i < data.at(0).data.size(); i++)
			for(size_t j = 0; j < data.size(); j++) temp.data.at(i) += data.at(j).data.at(i);

		return temp;
	}
}
#endif // !_MATRIX2D_
