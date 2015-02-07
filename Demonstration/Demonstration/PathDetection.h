/***********************************************************************************
*	PathDetection.h
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version:1.0 Date: 17/12/2013 details: Create class for PathDetection
*   Version:1.1 Date: 24/12/2013 details: Create array for storing Path
*	Version:1.2 Date: 29/12/2013 details: function for creating path created, needs tweeking
*	Version:1.3 Date: 30/12/2013 details: removal of code creating path explanation : bad implementation
*	Version:1.4 Date: 19/01/2014 details: removal of redundant code and improved efficiency of the code
*	Version:1.5 Date: 21/02/2014 details: Addition of Z data for gesture recognition
*	Version:1.6 Date: 13/03/2014 details: resize of pixel from one to nine
*	Version:1.7 Date: 26/03/2014 details: implemented Image Moments into the path detection
*	Version:1.8 Date: 29/03/2014 details: implemented add to gesture log function
*	Version:1.9 Date: 31/03/2014 details: Not needed, merged into Blob Detection to increase speed
************************************************************************************/

#ifndef _PATH_DETECTION_
#define _PATH_DETECTION_

#include "OpenNI.h"
#include <vector>
#include <math.h>
#include "HMM.h"
#include "ImageMoment.h"

using namespace std;
class PathDetection
{

public:
	PathDetection();
	~PathDetection();
	void init(int width, int height, int rowSize);
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

	ImageMoment im;
private:
	int pathWidth;
	int pathHeight;
	int pathRowSize;

	int centerX;
	int centerY;
	int centerZ;
};

PathDetection::PathDetection()
{
	hmm = HMM();
}

PathDetection::~PathDetection()
{
	im.~ImageMoment();
	m_pPath = vector<short>(pathWidth * pathHeight);
}

void PathDetection::init(int width, int height, int rowSize)
{
	pathWidth = width;
	pathHeight = height;
	pathRowSize = rowSize;

	im = ImageMoment();
}

void PathDetection::addToPath(int x, int y, int z)
{
	im.addToMoments(x,y,z);
}

void PathDetection::createPath()
{
	im.getCenterPoints(centerX, centerY, centerZ);

	xPath.push_back(centerX);
	yPath.push_back(centerY);
	zPath.push_back(centerZ);

	createPathPixel();

	im.clearMoments();
}

void PathDetection::createPathPixel()
{
	for(int x = centerX-1; x <= centerX+1; x++)
		for(int y = centerY-1; y <= centerY+1; y++)
			m_pPath.at(x + (y * pathRowSize)) = 1;
}

bool PathDetection::hasPath()
{

	if(xPath.size() >= 40)
	{
		hmm.Test_HMM(xPath, yPath, zPath);
		if(im.hasMoments())
		{
			createPath();
			return true;
		}
		else
		{
			clearPath();
			hmm.aGesture.addToGestureLog("Gesture " + hmm.returnCurrentGesture().GestureName + " found!");
			return false;
		}
	}
	else if(im.hasMoments())
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
	im.clearMoments();

	xPath = vector<int>();
	yPath = vector<int>();
	zPath = vector<int>();
	m_pPath = vector<short>(pathWidth * pathHeight);
}
#endif // !1