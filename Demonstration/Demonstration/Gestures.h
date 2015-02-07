/****************************************************************************

Author:Steven Cooper
Project: Computer Vision for Interactive Applications
Version: 1.1 Date:22/02/14 details: Creation of Gestures class
Version: 1.2 Date:26/02/14 details: Creation of Read from XML function
Version: 1.3 Date:26/03/14 details: Creation of multiple gesture types
Version: 1.4 Date:29/03/14 details: Creation of gesture Log array and functions
Version: 1.5 Date:31/03/14 details: Creation of update gesture functions
Version: 1.6 Date:01/03/14 details: Created gets and sets for two handed gestures
Version: 1.7 Date:02/03/14 details: Get function for gesture type created

****************************************************************************/

#include "Matrix3D.h"

#ifndef _GESTURES_H_

#define _GESTURES_H_

using namespace std;
using namespace Matrix;

enum gesture_type
{
	ONE_HAND,
	TWO_HAND
};

struct gesture
{
	string GestureName;

	Matrix2D A;
	Matrix2D centroids;
	Matrix2D B;
	Matrix1D Pi;

	double gestureThreshold;
};

class Gestures
{

public:
	Gestures();
	~Gestures();
	vector<gesture> getGestures();
	void setCurrentGesture(gesture);
	void setCurrentGesture2(gesture);
	gesture getCurrentGesture();
	gesture getCurrentGesture2();
	void setGestureType(gesture_type);
	gesture_type getGestureType();
	void updateGestures();
	bool isTwoHandedGesture(string, string, string&);
protected:

private:
	vector<gesture> gestures;
	void initializeGestures();
	void read_data_from_XML(string, gesture&);
	gesture_type GESTURE_TYPE;
	gesture currentGesture;
	gesture currentGesture2;
};

#endif