/*****************************************************************************

Author:Steven Cooper
Project: Computer Vision for Interactive Applications
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
		Matrix1D(int n);
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

	Matrix1D::Matrix1D()
	{
		data = vector<double>();
	}

	Matrix1D::Matrix1D(int n)
	{
		data = vector<double>(n);
	}

	Matrix1D::~Matrix1D()
	{

	}

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

	void Matrix1D::operator+=(const Matrix1D& b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) += b.data.at(i);
		}
		else data = b.data;
	}

	void Matrix1D::operator+=(const double b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) += b; 
		}
		else throw("No vector data");
	}

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

	void Matrix1D::operator-=(const Matrix1D& b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) -= b.data.at(i);
		}
		else data = b.data;
	}

	void Matrix1D::operator-=(const double b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) -= b; 
		}
		else throw("No vector data");
	}

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

	void Matrix1D::operator*=(const Matrix1D& b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) *= b.data.at(i);
		}
		else data = b.data;
	}

	void Matrix1D::operator*=(const double b)
	{
		if(data.size() > 0)
		{
			for(size_t i = 0; i < data.size(); i++) data.at(i) *= b; 
		}
		else throw("No vector data");
	}

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

	double Matrix1D::sum()
	{
		double temp = 0.0;
		for(size_t i = 0; i < data.size(); i++) temp += data.at(i);
		return temp;
	}

}
#endif // !_MATRIX1D
