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
*	Version: 2.3 Date:01/03/2014 details: Moved HMM to new Gesture Recognition class
*	Version: 2.4 Date:01/03/2014 details: Moved conditions for updating logs and gestures to GR class
*
*****************************************************************************/

#ifndef _BLOBDETECTION_H_

#define _BLOBDETECTION_H_

using namespace std;

#include "ImageMoment.h"
#include <vector>


struct Blob{
	ImageMoment imageMoment;
	vector<int> xPath;
	vector<int> yPath;
	vector<int> zPath;
	vector<int> m_path;
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

	bool handDetected();
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

	int HandRecTime;
};

#endif