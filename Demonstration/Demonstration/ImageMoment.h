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

private:
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

#endif // !_IMAGEMOMENT_H_