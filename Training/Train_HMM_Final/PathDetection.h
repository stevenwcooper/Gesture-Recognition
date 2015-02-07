/***********************************************************************************
*	PathDetection.h
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version:1.0 Date: 17/12/2013 details: Create class for PathDetection
*   Version:1.1 Date: 24/12/2013 details: Create array for storing Path
*	Version:1.2 Date: 29/12/2013 details: function for creating path created, needs tweeking
*	Version:1.3 Date: 30/12/2013 details: removal of code creating path exlanation : bad implementation
*	Version:1.4 Date: 19/01/2014 details: removal of redundant code and improved efficiency of the code
*	Version:1.5 Date: 21/02/2014 details: Addition of Z data for gesture recognition
*	Version:1.6 Date: 04/03/2014 details: ReWork of HMM variable initialisation
*	Version:1.7 Date: 10/03/2014 details: Pixel size increase from one to nine
************************************************************************************/

#ifndef _PATH_DETECTION_
#define _PATH_DETECTION_

#include "OpenNI.h"
#include <vector>
#include <math.h>
#include "HMM.h"

using namespace std;
class PathDetection
{

public:
	PathDetection();
	~PathDetection();
	void init(int width, int height, int rowSize);
	void initHiddenMarkovModel(int, int, int, int, string);
	void addToPath(int x, int y, int z);

	bool hasPath();

	vector<int> xPath;
	vector<int> yPath;
	vector<int> zPath;

	vector<short>m_pPath;

	HMM hmm;
protected:
	void clearPath();
	void createPath();
	void createPathPixel();

private:
	int pathWidth;
	int pathHeight;
	int pathRowSize;

	int xTotal;
	int yTotal;
	int zTotal;
	int pixelTotal;

	int centerX;
	int centerY;
	int centerZ;

	time_t now;
	time_t newtime;
};

PathDetection::PathDetection()
{

}

PathDetection::~PathDetection()
{
	xTotal = 0;
	yTotal = 0;
	zTotal = 0;
	pixelTotal = 0;
	m_pPath = vector<short>(pathWidth * pathHeight);
}

void PathDetection::init(int width, int height, int rowSize)
{
	pathWidth = width;
	pathHeight = height;
	pathRowSize = rowSize;

	xTotal = 0;
	yTotal = 0;
	zTotal = 0;
	pixelTotal = 0;

	newtime = time(&now)+1;
}

void PathDetection::initHiddenMarkovModel(int D, int M, int N, int LR, string gesture)
{
	hmm = HMM(D, M, N, LR, gesture);
}

void PathDetection::addToPath(int x, int y, int z)
{
	xTotal += x;
	yTotal += y;
	zTotal += z;
	pixelTotal++;
}

void PathDetection::createPath()
{
	centerX = xTotal / pixelTotal;
	centerY = yTotal / pixelTotal;
	centerZ = zTotal / pixelTotal;

	xPath.push_back(centerX);
	yPath.push_back(centerY);
	zPath.push_back(centerZ);

	createPathPixel();
}

void PathDetection::createPathPixel()
{
	for(int x = centerX-1; x <= centerX+1; x++)
		for(int y = centerY-1; y <= centerY+1; y++)
			m_pPath.at(x + (y * pathRowSize)) = 1;
}

bool PathDetection::hasPath()
{

	if(xPath.size() == 70)
	{
		hmm.add_Data(xPath, yPath, zPath);
		clearPath();
		return false;
	}
	else if(pixelTotal > 0)
	{
		createPath();
		return true;
	}
	else
	{
		clearPath();
		return false;
	}
}

void PathDetection::clearPath()
{
	xTotal = 0;
	yTotal = 0;
	pixelTotal = 0;

	xPath.clear();
	yPath.clear();
	zPath.clear();
	m_pPath = vector<short>(pathWidth * pathHeight);
}
#endif // !1