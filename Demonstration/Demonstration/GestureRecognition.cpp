#include "GestureRecognition.h"

using namespace CVIA;

GestureRecognition::GestureRecognition()
{
	c_hmm = HMM();
}

GestureRecognition::~GestureRecognition()
{

}

void GestureRecognition::init(int frameHeight, int frameWidth, int rowSize)
{
	log_flag = false;
	gesture_flag = false;
	hd_blobDetection.init(frameHeight, frameWidth, rowSize);
}

void GestureRecognition::initiateRecognition(vector<int> Xpath, vector<int> Ypath, vector<int> Zpath)
{
	c_hmm.Test_HMM(Xpath, Ypath, Zpath);
}

void GestureRecognition::addData(int x, int y, int z)
{
	hd_blobDetection.addToBlob(x,y,z);
}

bool GestureRecognition::dataDetected()
{
	bool hand_detected = handDetected(); 
	vector<Blob> previousBlobs = hd_blobDetection.oldBlobs;
	bool data_flag = hd_blobDetection.hasBlobs();
	if(data_flag) 
	{
		Hands = hd_blobDetection.Blobs;

		if(hd_blobDetection.oldBlobs.size() != Hands.size())
		{
			if(Hands.size() > 1) c_hmm.aGesture.setGestureType(TWO_HAND);
			else c_hmm.aGesture.setGestureType(ONE_HAND);
			c_hmm.updateGestures();
			gesture_flag = true;
		}

		/*if(c_hmm.aGesture.getGestureType() == TWO_HAND)
		{
			if(Hands.at(0).xPath.size() >= 40) 
			{

				if((Hands.at(0).xPath.size() % 10) == 0)
				{
					for(int i = 0; i < Hands.size(); i++)
					{
						initiateRecognition(hd_blobDetection.Blobs.at(i).xPath,hd_blobDetection.Blobs.at(i).yPath,hd_blobDetection.Blobs.at(i).zPath);
					}
				}
			}
		}*/
		//else if(c_hmm.aGesture.getGestureType() == ONE_HAND)
		//{
			if(Hands.at(0).xPath.size() >= 20) 
			{

				if((Hands.at(0).xPath.size() % 10) == 0)
				{
					for(int i = 0; i < Hands.size(); i++)
					{
						initiateRecognition(hd_blobDetection.Blobs.at(i).xPath,hd_blobDetection.Blobs.at(i).yPath,hd_blobDetection.Blobs.at(i).zPath);
					}
				}
			}
		//}

		if(handDetected() != hand_detected)
		{
			if(hd_blobDetection.Blobs.size() == 1) m_log.addToLog("One Hand Detected");
			else if(hd_blobDetection.Blobs.size() == 2) m_log.addToLog("Two Hands Detected");

			log_flag = true;
		}
	}
	else
	{
		if(previousBlobs.size() > 0)
			if(previousBlobs.at(0).xPath.size() >= 20) 
			{
				if(c_hmm.aGesture.getGestureType() == TWO_HAND)
				{
					string found_gesture = "";
					if(c_hmm.aGesture.isTwoHandedGesture(c_hmm.aGesture.getCurrentGesture().GestureName, c_hmm.aGesture.getCurrentGesture2().GestureName, found_gesture))
					{
						m_log.addToLog("Gesture " + found_gesture + " found!");
					}
					else m_log.addToLog("Gesture Not found!");
					c_hmm.aGesture.setCurrentGesture(gesture());
					c_hmm.aGesture.setCurrentGesture2(gesture());
				}
				else
					m_log.addToLog("Gesture " + c_hmm.returnCurrentGesture().GestureName + " found!");
				c_hmm.aGesture.setCurrentGesture(gesture());
				log_flag = true;
			}
	}
	return data_flag;
}

bool GestureRecognition::handDetected()
{
	return hd_blobDetection.handDetected();
}

bool GestureRecognition::updateLog()
{
	return log_flag;
}

bool GestureRecognition::updateGestures()
{
	return gesture_flag;
}

vector<gesture> GestureRecognition::returnGestureList()
{
	return c_hmm.gestureList;
}

vector<string> GestureRecognition::returnLog()
{
	return m_log.getLog();
}

void GestureRecognition::addToLog(string entry)
{
	m_log.addToLog(entry);
}