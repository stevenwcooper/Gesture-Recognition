/****************************************************************************

Author:Steven Cooper
Project: Computer Vision for Interactive Applications
Version: 1.1 Date:22/02/14 details: Creation of Gestures class
Version: 1.2 Date:26/02/14 details: Creation of Read from XML function
Version: 1.3 Date:26/03/14 details: Creation of multiple gesture types
Version: 1.4 Date:29/03/14 details: Creation of gesture Log array and functions
Version: 1.5 Date:31/03/14 details: Creation of update gesture functions

****************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Matrix3D.h"
#include "tinyxml.h"

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
	gesture getCurrentGesture();
	vector<string> getGestureLog();

	void addToGestureLog(string);

	void setGestureType(gesture_type);

	void updateGestures();
protected:
	void initializeGestures();

	void read_data_from_XML(string, gesture&);
private:
	vector<gesture> gestures;

	gesture_type GESTURE_TYPE;

	vector<string> gestureLog;

	gesture currentGesture;
};

Gestures::Gestures()
{
	GESTURE_TYPE = ONE_HAND;
	initializeGestures();
}

Gestures::~Gestures()
{

}

void Gestures::initializeGestures()
{
	gesture newG;
	switch(GESTURE_TYPE)
	{
	case ONE_HAND:
		newG.GestureName = "swipe_left";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "swipe_right";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "wave";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_up";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	case TWO_HAND:
		break;

	default:
		break;
	}

	//newG = gesture();
	//newG.GestureName = "swipe_down";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "swipe_left";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "swipe_right";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "put_down";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);
}

void Gestures::updateGestures()
{
	gestures.clear();
	gesture newG;
	switch(GESTURE_TYPE)
	{
	case ONE_HAND:
		newG.GestureName = "swipe_left";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "swipe_right";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "wave";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_up";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	case TWO_HAND:
		break;

	default:
		break;
	}

	//newG = gesture();
	//newG.GestureName = "swipe_down";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "swipe_left";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "swipe_right";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);

	//newG = gesture();
	//newG.GestureName = "put_down";
	//read_data_from_XML(newG.GestureName, newG);
	//gestures.push_back(newG);
}

vector<gesture> Gestures::getGestures()
{
	return gestures;
}

void Gestures::setCurrentGesture(gesture gest)
{
	currentGesture = gest;
}

gesture Gestures::getCurrentGesture()
{
	return currentGesture;
}

void Gestures::read_data_from_XML(string gesturename, gesture &newGesture)
{
	string filename = "gestures/" + gesturename + ".xml";
	stringstream temp;
	int width, height;

	TiXmlDocument doc(filename.c_str());
	if(!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement *pElem;
	TiXmlHandle hRoot(0);
	TiXmlNode *pNode;

	//gesture node
	pElem = hDoc.Child(1).Element();
	hRoot = TiXmlHandle(pElem);

	//threshold
	pElem = hRoot.FirstChild("Threshold").Element();

	double g;
	temp << pElem->GetText();
	temp >> g;
	newGesture.gestureThreshold  = g;

	//Centroids
	pElem = hRoot.FirstChild("Centroids").Element();
	pElem->QueryIntAttribute("wSize",&width);
	pElem->QueryIntAttribute("hSize", &height);
	Matrix2D Readcentroids = Matrix2D();
	pNode = hRoot.FirstChild("Centroids").FirstChild().Node();
	temp = stringstream();
	temp << pNode->Value();


	for(int  i = 0; i < height; i++)
	{
		Matrix1D mat = Matrix1D(width);
		for(int j = 0; j < width; j++)
		{
			string val;
			double dval;
			if(!getline(temp,val,',')) break;
			istringstream ival(val);
			ival >> dval;
			mat.data.at(j) = dval;
		}
		Readcentroids.data.push_back(mat);
	}
	newGesture.centroids = Readcentroids;

	//A
	pElem = hRoot.FirstChild("A").Element();
	pElem->QueryIntAttribute("wSize",&width);
	pElem->QueryIntAttribute("hSize", &height);
	Matrix2D ReadA = Matrix2D();
	pNode = hRoot.FirstChild("A").FirstChild().Node();
	temp = stringstream();
	temp << pNode->Value();


	for(int  i = 0; i < height; i++)
	{
		Matrix1D mat = Matrix1D(width);
		for(int j = 0; j < width; j++)
		{
			string val;
			double dval;
			if(!getline(temp,val,',')) break;
			istringstream ival(val);
			ival >> dval;
			mat.data.at(j) = dval;
		}
		ReadA.data.push_back(mat);
	}
	newGesture.A = ReadA;

	//B
	pElem = hRoot.FirstChild("B").Element();
	pElem->QueryIntAttribute("wSize",&width);
	pElem->QueryIntAttribute("hSize", &height);
	Matrix2D ReadB = Matrix2D();
	pNode = hRoot.FirstChild("B").FirstChild().Node();
	temp = stringstream();
	temp << pNode->Value();


	for(int  i = 0; i < height; i++)
	{
		Matrix1D mat = Matrix1D(width);
		for(int j = 0; j < width; j++)
		{
			string val;
			double dval;
			if(!getline(temp,val,',')) break;
			istringstream ival(val);
			ival >> dval;
			mat.data.at(j) = dval;
		}
		ReadB.data.push_back(mat);
	}
	newGesture.B = ReadB;

	//A
	pElem = hRoot.FirstChild("Pi").Element();
	int size;
	pElem->QueryIntAttribute("size",&size);
	Matrix1D ReadPi = Matrix1D();
	pNode = hRoot.FirstChild("Pi").FirstChild().Node();
	temp = stringstream();
	temp << pNode->Value();


	for(int  i = 0; i < size; i++)
	{
		string val;
		double dval;
		if(!getline(temp,val,',')) break;
		istringstream ival(val);
		ival >> dval;
		ReadPi.data.push_back(dval);
	}
	newGesture.Pi = ReadPi;
}

void Gestures::setGestureType(gesture_type type)
{
	GESTURE_TYPE = type;
}

vector<string> Gestures::getGestureLog()
{
	return gestureLog;
}

void Gestures::addToGestureLog(string entry)
{
	if(gestureLog.size() >= 5) gestureLog.erase(gestureLog.begin());

	gestureLog.push_back(entry);
}
#endif