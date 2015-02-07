/***********************************************************************************
*	BinaryHandDetection.h
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version:1.0 date:05/12/2013 details: Create class for BinaryHandDetection
*
************************************************************************************/

#ifndef _BINARY_HAND_DETECTION_
#define _BINARY_HAND_DETECTION_

#include "OpenNI.h"
#include <vector>

using namespace std;
class BinaryHandDetection
{

public:
	BinaryHandDetection();
	~BinaryHandDetection();
	void setCenter(int x, int y);
	int getCenterX() { return centerX; }
	int getCenterY() { return centerY; }
	void addHandCenterToStack(int center[2]);	
	vector<int>*pixelX;
	vector<int>*pixelY;
	bool test;
protected:

private:
	int centerX;
	int centerY;

};

BinaryHandDetection::BinaryHandDetection()
{
	pixelX = new vector<int>();
	pixelY = new vector<int>();
	test = false;
}

void BinaryHandDetection::setCenter(int x, int y)
{
	centerX = x;
	centerY = y;
}

void BinaryHandDetection::addHandCenterToStack(int center[2])
{
	if(pixelX->size() == 5)
	{
		for(int i = 0; i < pixelX->size() - 1; i++)
		{
			pixelX->at(i) = pixelX->at(i+1);
			pixelY->at(i) = pixelY->at(i+1);
		}
		pixelX->at(pixelX->size() - 1) = center[0];
		pixelY->at(pixelX->size() - 1) = center[1];
	}
	else
	{
		pixelX->push_back(center[0]);
		pixelY->push_back(center[1]);
	}

}
#endif // !1