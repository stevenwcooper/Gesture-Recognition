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
*
*****************************************************************************/

#ifndef _BLOBDETECTION_H_

#define _BLOBDETECTION_H_

#include "ImageMoments.h"
#include "PathDetection.h"
#include <vector>
#include <algorithm>

struct Colour{
	int red;
	int green;
	int blue;
} Colours[3];

struct Blob{
	ImageMoment imageMoment;
	//PathDetection Path;
	Colour blobColour;
};

class BlobDetection
{

public:
	BlobDetection();
	~BlobDetection();

	void init(int labelHeight, int labelWidth, int rowSize);

	std::vector<short> labelArray;

	void addToBlob(int x, int y);

	std::vector<Blob> Blobs;

	bool hasBlobs();
protected:

	bool Neighbour(int x, int y, short& label);

	void addToEquivalenceTable(int label);

	void ammendEquivalenceTable(int maxLabel, int minLabel);

	bool Connected(int maxLabel, int minLabel);

	int Max(int a, int b);

	int Min(int a, int b);
	
	void createBlobs();
	
	int BlobCount();
	
	void clearBlobs();
private:
	short labelCount;

	vector<int> EquivalenceTable;

	int labelWidth;
	int labelHeight;
	int labelRowSize;
};


BlobDetection::BlobDetection()
{
	EquivalenceTable = vector<int>();

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

	for(unsigned int i = 0; i < Blobs.size(); i++)
	{
		if(Blobs.at(i).imageMoment.area > 0)
			if(!Blobs.at(i).imageMoment.hasMoments()) hasBlobs = false;
	}

	return hasBlobs;
}

void BlobDetection::clearBlobs()
{
	labelCount = 0;

	for(unsigned int i = 0; i < Blobs.size(); i++)
	{
		Blobs.at(i).imageMoment.~ImageMoment();
		//Blobs.at(i).Path.~PathDetection();
	}

	Blobs.clear();

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


void BlobDetection::addToBlob(int x, int y)
{
	short label = 0;
	if(Neighbour(x, y, label))
	{
		labelArray.at(x + (y * labelRowSize)) = label;
	}
	else
	{
		labelCount++;
		labelArray.at(x + (y * labelRowSize)) = labelCount;
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

		//newBlob.Path = PathDetection();
		//newBlob.Path.init(labelWidth, labelHeight, labelRowSize);

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
				Blobs.at(EquivalenceTable.at(labelArray.at(x + (y * labelRowSize))-1)-1).imageMoment.addToMoments(x,y);
				//Blobs.at(EquivalenceTable.at(labelArray.at(x + (y * labelRowSize))-1)-1).Path.addToPath(x,y);
			}
		}
	}
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
#endif