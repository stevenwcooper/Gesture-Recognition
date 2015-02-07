#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "tinyxml.h"
#include "Gestures.h"

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

		newG = gesture();
		newG.GestureName = "scroll_down";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "delete";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	case TWO_HAND:
		newG.GestureName = "swipe_left";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "swipe_right";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_up";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_down";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	default:
		break;
	}
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

		newG = gesture();
		newG.GestureName = "scroll_down";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "delete";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	case TWO_HAND:
		newG.GestureName = "swipe_left";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "swipe_right";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_up";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);

		newG = gesture();
		newG.GestureName = "scroll_down";
		read_data_from_XML(newG.GestureName, newG);
		gestures.push_back(newG);
		break;

	default:
		break;
	}
}

vector<gesture> Gestures::getGestures()
{
	return gestures;
}

void Gestures::setCurrentGesture(gesture gest)
{
	currentGesture = gest;
}

void Gestures::setCurrentGesture2(gesture gest)
{
	currentGesture2 = gest;
}

gesture Gestures::getCurrentGesture()
{
	return currentGesture;
}

gesture Gestures::getCurrentGesture2()
{
	return currentGesture2;
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

gesture_type Gestures::getGestureType()
{
	return GESTURE_TYPE;
}

bool Gestures::isTwoHandedGesture(string g1, string g2, string& rg)
{
	bool flag = false;

	if(g1 == "")
	{
		g1 = "Not";
	}

	if(g2 == "")
	{
		g2 = "Not";
	}

	// swipe left
	if(g1 == "Not" && g2 == "swipe_left" ||
		g1 == "swipe_left" && g2 == "Not")
	{
		rg = "swipe left";
		flag = true;
	}
	// swipe right
	else if(g1 == "Not" && g2 == "swipe_right" ||
		g1 == "swipe_right" && g2 == "Not")
	{
		rg = "swipe right";
		flag = true;
	}
	// scroll up
	else if(g1 == "Not" && g2 == "scroll_up" ||
		g1 == "scroll_up" && g2 == "Not")
	{
		rg = "scroll up";
		flag = true;
	}
	// scroll down
	else if(g1 == "Not" && g2 == "scroll_down" ||
		g1 == "scroll_down" && g2 == "Not")
	{
		rg = "scroll down";
		flag = true;
	}
	// shrink width
	else if(g1 == "swipe_left" && g2 == "swipe_right" ||
		g1 == "swipe_right" && g2 == "swipe_left")
	{
		rg = "shrink width";
		flag = true;
	}
	// shrink height
	else if(g1 == "scroll_down" && g2 == "scroll_up" ||
		g1 == "scroll_up" && g2 == "scroll_down")
	{
		rg = "shrink height";
		flag = true;
	}
	return flag;
}