/****************************************************************************
*
*	Author: Steven Cooper
*	Project: Computer Vision for Computer Applications
*	Version: 1.0 Date:04/01/2014 details: Creation of Image Moments Class
*	Version: 1.1 Date:05/01/2014 details: Functions created to detect centoid and border
*	Version: 1.2 Date:26/03/2014 details: Re-Work of code to for the implementation into path Detection
*	Version: 1.3 Date:26/03/2014 details: Created getCentrePoints function
*	Version: 1.4 Date:31/03/2014 details: removed redundant code for creating height, width and orientation
*	
****************************************************************************/

#ifndef _IMAGEMOMENT_H_

#define _IMAGEMOMENT_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

class ImageMoment
{
public:
	ImageMoment();
	~ImageMoment();

	bool isCenter(int x, int y);

	bool isBorder(int x, int y);

	float orientation();

	void addToMoments(int x, int y, int z);

	void clearMoments();

	bool hasMoments();
	
	void getCenterPoints(int&, int&, int&);

	int area;
	float length;
	float width;
protected:
	int M00();
	int M01();
	int M10();
	int M11();
	int M02();
	int M20();
	int cM00();
	int cM01();
	int cM10();
	int cM11();
	int cM02();
	int cM20();
	float cMI11();
	float cMI02();
	float cMI20();	
private:
	
	int xValueTotal;
	int yValueTotal;
	int zValueTotal;
	long xsqrdValueTotal;
	long ysqrdValueTotal;
	long xyValueTotal;

	float calculateEigenVector(std::string value);

	float eigenvectorMin;
	float eigenvectorMax;

	float v_orientation;
};

ImageMoment::ImageMoment()
{
	area = 0;
	xValueTotal = 0;
	yValueTotal = 0;
	zValueTotal = 0;
	xsqrdValueTotal = 0;
	ysqrdValueTotal = 0;
	xyValueTotal = 0;
	eigenvectorMax = 0;
	eigenvectorMin = 0;
	v_orientation = 0;
}

ImageMoment::~ImageMoment()
{
	area = 0;
	xValueTotal = 0;
	yValueTotal = 0;
	zValueTotal = 0;
	xsqrdValueTotal = 0;
	ysqrdValueTotal = 0;
	xyValueTotal = 0;
	eigenvectorMax = 0;
	eigenvectorMin = 0;
	v_orientation = 0;
}

void ImageMoment::clearMoments()
{
	area = 0;
	xValueTotal = 0;
	yValueTotal = 0;
	zValueTotal = 0;
	xsqrdValueTotal = 0;
	ysqrdValueTotal = 0;
	xyValueTotal = 0;
	eigenvectorMax = 0;
	eigenvectorMin = 0;
	v_orientation = 0;
}

bool ImageMoment::hasMoments()
{
	if(area > 0)
	{
		/*eigenvectorMax = calculateEigenVector("MAX");
		eigenvectorMin = calculateEigenVector("MIN");
		width = 2 * 2 * sqrt(eigenvectorMin);
		length = 2 * 2 * sqrt(eigenvectorMax);

		float valueA = float(0.5 * atan((2 * cMI11()) / (cMI20() - cMI02())));

		v_orientation = (float)((valueA * 180) / 3.14159265);*/ 

		return true;
	}
	else return false;
}

void ImageMoment::addToMoments(int x, int y, int z)
{
	area++;

	xValueTotal += x;
	yValueTotal += y;
	zValueTotal += z;

	xsqrdValueTotal += (x * x);
	ysqrdValueTotal += (y * y);

	xyValueTotal += (x * y);
}

int ImageMoment::M00()
{
	return area;
}

int ImageMoment::M01()
{
	return yValueTotal;
}

int ImageMoment::M10()
{
	return xValueTotal;
}

int ImageMoment::M11()
{
	return xyValueTotal;
}

int ImageMoment::M02()
{
	return ysqrdValueTotal;
}

int ImageMoment::M20()
{
	return xsqrdValueTotal;
}

int ImageMoment::cM00()
{
	return M00();
}

int ImageMoment::cM01()
{
	return 0;
}

int ImageMoment::cM10()
{
	return 0;
}

int ImageMoment::cM11()
{
	return M11() - ((xValueTotal / area) * M01());
}

int ImageMoment::cM02()
{
	return M02() - ((yValueTotal / area) * M01());
}

int ImageMoment::cM20()
{
	return M20() - ((xValueTotal / area) * M10());
}

float ImageMoment::cMI11()
{
	return (float)(cM11() / cM00());
}

float ImageMoment::cMI02()
{
	return (float)(cM02() / cM00());
}

float ImageMoment::cMI20()
{
	return (float)(cM20() / cM00());
}

float ImageMoment::orientation()
{
	return v_orientation;
}

float ImageMoment::calculateEigenVector(std::string value)
{
	float valueA, valueB;

	valueA = ((cMI20() + cMI02()) / 2) + (sqrt(4 * (cMI11() * cMI11()) + ((cMI20() - cMI02()) * (cMI20() - cMI02()))) / 2);

	valueB = ((cMI20() + cMI02()) / 2) - (sqrt(4 * (cMI11() * cMI11()) + ((cMI20() - cMI02()) * (cMI20() - cMI02()))) / 2);

	if(value == "MAX") return (valueA > valueB) ? valueA : valueB;
	else if(value == "MIN") return (valueA < valueB) ? valueA : valueB;
	else return float(0);
}

void ImageMoment::getCenterPoints(int& x, int& y, int& z)
{
	x = xValueTotal / area;
	y = yValueTotal / area;
	z = zValueTotal / area;
}

bool ImageMoment::isCenter(int x, int y)
{
	int tx,ty,tz;
	getCenterPoints(tx,ty,tz);
	return ((x == tx && y == ty) ||
		(x == tx-1 && y == ty) ||
		(x == tx+1 && y == ty) ||
		(x == tx && y == ty-1) ||
		(x == tx && y == ty+1));
}

bool ImageMoment::isBorder(int x, int y)
{
	int tx,ty,tz;
	getCenterPoints(tx,ty,tz);
	return ( (x == (tx - (int)(0.5 * width)) && y >= (ty - (int)(0.5 * length)) && y <= (ty + (int)(0.5 * length))) ||
		(x == (tx + (int)(0.5 * width)) && y >= (ty - (int)(0.5 * length)) && y <= (ty + (int)(0.5 * length))) ||
		(y == (ty - (int)(0.5 * length)) && x >= (tx - (int)(0.5 * width)) && x <= (tx + (int)(0.5 * width))) ||
		(y == (ty + (int)(0.5 * length)) && x >= (tx - (int)(0.5 * width)) && x <= (tx + (int)(0.5 * width))));
}
#endif // !_IMAGEMOMENT_H_