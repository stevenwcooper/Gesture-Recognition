/*****************************************************************************
*
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version: 1.0 Date:06/01/2014 details: Creation of BlobDetection class
*	Version: 1.1 Date:07/01/2014 details: Creation of first check functions
*	Version: 1.2 Date:14/01/2014 details: Creation of second check functions
*	Version: 1.3 Date:14/01/2014 details: removal of second check functions explanation: bad implementation
*	Version: 1.4 Date:16/01/2014 details: Creation of new second check functions
*	Version: 1.5 Date:19/01/2014 details: Multi Blob Detection class completed
*	Version: 1.6 Date:19/01/2014 details: Improved efficiency of code and moved some functions to protected
*	Version: 1.7 Date:20/01/2014 details: Added different colours to different blobs
*	Version: 1.8 Date:26/03/2014 details: Small amendment to creation of blobs to align with image moments changes
*	Version: 1.9 Date:30/03/2014 details: Created x,y,z and full paths for each Blob
*	Version: 2.0 Date:30/03/2014 details: Created function to add path from previous blobs to current blobs
*	Version: 2.1 Date:31/03/2014 details: Created functions for updating gesture list
*	Version: 2.2 Date:31/03/2014 details: Implemented Hmm for single hand
*
*****************************************************************************/

#ifndef _BLOBDETECTION_H_

#define _BLOBDETECTION_H_

#include "ImageMoments.h"
#include "HMM.h"
#include <vector>
#include <limits>
#include <algorithm>

struct Colour{
	int red;
	int green;
	int blue;
} Colours[3];

struct Blob{
	ImageMoment imageMoment;
	vector<int> xPath;
	vector<int> yPath;
	vector<int> zPath;
	vector<int> m_path;
	Colour blobColour;
};

class BlobDetection
{

public:
	BlobDetection();
	~BlobDetection();

	void init(int labelHeight, int labelWidth, int rowSize);

	vector<short> labelArray;

	void addToBlob(int x, int y, int z);

	vector<Blob> Blobs;
	vector<Blob> oldBlobs;

	bool hasBlobs();

	HMM hmm;

	vector<gesture> getGestures();
protected:

	bool Neighbour(int x, int y, short& label);

	void addToEquivalenceTable(int label);

	void ammendEquivalenceTable(int maxLabel, int minLabel);

	bool Connected(int maxLabel, int minLabel);

	int Max(int a, int b);

	int Min(int a, int b);

	void createBlobs();

	int BlobCount();

	void clearData();

	void createPath();

	void clearBlobs();

	void addPathsToBlobs();
private:
	short labelCount;

	vector<int> zValues;
	vector<int> EquivalenceTable;

	int labelWidth;
	int labelHeight;
	int labelRowSize;
};


BlobDetection::BlobDetection()
{
	EquivalenceTable = vector<int>();

	hmm = HMM();

	Colours[0].red = 255;
	Colours[0].green = 0;
	Colours[0].blue = 0;

	Colours[1].red = 255;
	Colours[1].green = 255;
	Colours[1].blue = 0;

	Colours[2].red = 0;
	Colours[2].green = 255;
	Colours[2].blue = 255;

}

BlobDetection::~BlobDetection()
{
	labelCount = 0;

	for(unsigned int i = 0; i < Blobs.size(); i++)
	{
		Blobs.at(i).imageMoment.~ImageMoment();
		//Blobs.at(i).Path.~PathDetection();
	}

	Blobs.clear();
	labelArray.clear();
}

void BlobDetection::init(int height, int width, int rowSize)
{
	clearBlobs();

	labelCount = 0;
	labelWidth = width;
	labelHeight = height;
	labelRowSize = rowSize;
	zValues = vector<int>(labelWidth * labelHeight);
	labelArray = vector<short>(labelWidth * labelHeight);
}

int BlobDetection::Min(int a, int b)
{
	return (a < b) ? a : b;
}

int BlobDetection::Max(int a, int b)
{
	return (a > b) ? a : b;
}

bool BlobDetection::hasBlobs()
{
	bool hasBlobs = true;

	createBlobs();
	vector<Blob> newBlobs;
	for(unsigned int i = 0; i < Blobs.size(); i++)
	{
		if(Blobs.at(i).imageMoment.area > 1500)
		{
			int cx, cy, cz;
			Blobs.at(i).imageMoment.hasMoments();
			Blobs.at(i).imageMoment.getCenterPoints(cx,cy,cz);
			Blobs.at(i).xPath.push_back(cx);
			Blobs.at(i).yPath.push_back(cy);
			Blobs.at(i).zPath.push_back(cz);
			Blobs.at(i).m_path = vector<int>(labelHeight * labelWidth);
			for(int x = cx-1; x <= cx+1; x++)
				for(int y = cy-1; y <= cy+1; y++)
					Blobs.at(i).m_path.at(x + (y * labelRowSize)) = 1;
			newBlobs.push_back(Blobs.at(i));

		}
	}
	Blobs = newBlobs;
	if(Blobs.size() == 0)
	{
		if(oldBlobs.size() > 0)
			if(oldBlobs.at(0).xPath.size() >= 20) hmm.aGesture.addToGestureLog("Gesture " + hmm.returnCurrentGesture().GestureName + " found!");
		hasBlobs = false;
		clearData();
	}
	else
	{
		addPathsToBlobs();
		if(oldBlobs.size() != Blobs.size())
		{
			if(Blobs.size() > 1) hmm.aGesture.setGestureType(TWO_HAND);
			else hmm.aGesture.setGestureType(ONE_HAND);
			hmm.updateGestures();
		}
		if(Blobs.at(0).xPath.size() >= 20) 
		{
			if((Blobs.at(0).xPath.size() % 10) == 0)
				hmm.Test_HMM(Blobs.at(0).xPath,Blobs.at(0).yPath,Blobs.at(0).zPath);
		}
		oldBlobs = Blobs;
	}

	return hasBlobs;
}

void BlobDetection::clearBlobs()
{
	labelCount = 0;
	for(unsigned int i = 0; i < Blobs.size(); i++)
	{
		Blobs.at(i).imageMoment.~ImageMoment();
	}

	Blobs.clear();

	zValues.clear();
	EquivalenceTable = vector<int>();
}

bool BlobDetection::Neighbour(int x, int y, short& label)
{
	if(labelArray.at((x - 1) + (y * labelRowSize)) != 0)
	{
		if(labelArray.at(x + ((y -1) * labelRowSize)) != 0 && labelArray.at(x + ((y -1) * labelRowSize)) != labelArray.at((x - 1) + (y * labelRowSize)))
		{
			ammendEquivalenceTable(Max(EquivalenceTable.at(labelArray.at(x + ((y -1) * labelRowSize))-1), EquivalenceTable.at(labelArray.at((x - 1) + (y * labelRowSize))-1)),
				Min(EquivalenceTable.at(labelArray.at(x + ((y -1) * labelRowSize))-1), EquivalenceTable.at(labelArray.at((x - 1) + (y * labelRowSize))-1)));
			label = short(Min(labelArray.at(x + ((y -1) * labelRowSize)), labelArray.at((x - 1) + (y * labelRowSize))));
		}
		else
		{
			label = short(labelArray.at((x - 1) + (y * labelRowSize)));
		}
		return true;
	}
	else if(labelArray.at(x + ((y -1) * labelRowSize)) != 0)
	{
		label = short(labelArray.at(x + ((y -1) * labelRowSize)));
		return true;
	}
	else return false;
}

bool BlobDetection::Connected(int maxLabel, int minLabel)
{
	return (EquivalenceTable.at(maxLabel-1) == minLabel);
}

void BlobDetection::addToBlob(int x, int y, int z)
{
	short label = 0;
	if(Neighbour(x, y, label))
	{
		labelArray.at(x + (y * labelRowSize)) = label;
		zValues.at(x + (y * labelRowSize)) = z;
	}
	else
	{
		labelCount++;
		labelArray.at(x + (y * labelRowSize)) = labelCount;
		zValues.at(x + (y * labelRowSize)) = z;
		addToEquivalenceTable(labelCount);

	}
}

void BlobDetection::createBlobs()
{
	vector<int> distinctLabels = EquivalenceTable;

	std::sort(distinctLabels.begin(), distinctLabels.end());
	distinctLabels.erase(std::unique(distinctLabels.begin(), distinctLabels.end()), distinctLabels.end());

	// re value the blob labels belong to
	for(unsigned int i = 0; i < EquivalenceTable.size(); i++)
	{
		for(unsigned int j = 0; j < distinctLabels.size(); j++)
		{
			if(distinctLabels.at(j) == EquivalenceTable.at(i))
			{
				EquivalenceTable.at(i) = j + 1;
			}
		}
	}

	// create N number of empty blobs
	for(unsigned int i = 0, col=0; i < distinctLabels.size(); i++)
	{
		Blob newBlob;
		newBlob.imageMoment = ImageMoment();
		newBlob.blobColour = Colours[col];
		Blobs.push_back(newBlob);
		(col == 2) ? col = 0 : col++;
	}

	// fill blobs
	for(int y = 0; y < labelHeight; y++)
	{
		for(int x = 0; x < labelWidth; x++)
		{
			if(labelArray.at(x + (y * labelRowSize)) != 0)
			{
				Blobs.at(EquivalenceTable.at(labelArray.at(x + (y * labelRowSize))-1)-1).imageMoment.addToMoments(x,y, zValues.at(x + (y * labelRowSize)));
			}
		}
	}
	//addPathsToBlobs();
}

void BlobDetection::addToEquivalenceTable(int label)
{
	EquivalenceTable.push_back(label);
}

void BlobDetection::ammendEquivalenceTable(int maxLabel, int minLabel)
{
	int tableVal = 0;
	if(!Connected(maxLabel, minLabel))
	{
		if(EquivalenceTable.at(maxLabel-1) > minLabel)
		{
			tableVal = EquivalenceTable.at(maxLabel-1);
			for(unsigned int i = 0; i < EquivalenceTable.size(); i++)
			{
				if(EquivalenceTable.at(i) == tableVal) EquivalenceTable.at(i) = minLabel;
			}
		}
	}
}

int BlobDetection::BlobCount()
{
	return labelCount;
}

void BlobDetection::addPathsToBlobs()
{
	if(oldBlobs.size() != 0)
	{
		for(int i = 0; i < Blobs.size(); i++)
		{
			int blobId = -1;
			int xVal = 0;
			int yVal = 0;
			int currentDistance = INT_MAX;
			for(int j = 0; j < oldBlobs.size(); j++)
			{
				int Distance;
				xVal = Blobs.at(i).xPath.at(0) - oldBlobs.at(j).xPath.at(oldBlobs.at(j).xPath.size()-1);
				yVal = Blobs.at(i).yPath.at(0) - oldBlobs.at(j).yPath.at(oldBlobs.at(j).yPath.size()-1);
				Distance = (int)(sqrt(xVal * xVal) + sqrt(yVal * yVal));
				if(Distance < currentDistance)
				{
					blobId = j;
					currentDistance = Distance;
				}
			}
			if(blobId != -1)
			{
				vector<int>newPath = oldBlobs.at(blobId).m_path;
				for(int x = Blobs.at(i).xPath.at(0)-1; x <= Blobs.at(i).xPath.at(0)+1; x++)
					for(int y = Blobs.at(i).yPath.at(0)-1; y <= Blobs.at(i).yPath.at(0)+1; y++)
						newPath.at(x + (y * labelRowSize)) = 1;
				Blobs.at(i).m_path = newPath;

				vector<int> newXPath = oldBlobs.at(blobId).xPath;
				newXPath.push_back(Blobs.at(i).xPath.at(0));
				Blobs.at(i).xPath = newXPath;

				vector<int> newYPath = oldBlobs.at(blobId).yPath;
				newYPath.push_back(Blobs.at(i).yPath.at(0));
				Blobs.at(i).yPath = newYPath;

				vector<int> newZPath = oldBlobs.at(blobId).zPath;
				newZPath.push_back(Blobs.at(i).zPath.at(0));
				Blobs.at(i).zPath = newZPath;

				Blobs.at(i).blobColour = oldBlobs.at(blobId).blobColour;
			}
		}
	}
}

void BlobDetection::clearData()
{
	oldBlobs.clear();
}

void BlobDetection::createPath()
{

}

vector<gesture> BlobDetection::getGestures()
{
	return hmm.gestureList;
}
#endif