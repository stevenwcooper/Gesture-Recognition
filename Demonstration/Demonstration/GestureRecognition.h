/**********************************************************************************
*
*
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version: 1.0 Date:26/03/2014 details: Creation of Gesture Recognition class
*	Version: 1.1 Date:27/03/2014 details: Moved hmm from blob detection to class
*	Version: 1.2 Date:01/03/2014 details: Moved condition statements from bd to class
*	Version: 1.3 Date:01/03/2014 details: Implemented two hand gestures and checks
*	
*
***********************************************************************************/

#include <string>
#include "Log.h"
#include "HMM.h"
#include "BlobDetection.h"
#include "OpenNI.h"

namespace CVIA
{

	class GestureRecognition
	{

	public:
		GestureRecognition();
		~GestureRecognition();

		void init(int, int, int);

		void initiateRecognition(vector<int>, vector<int>, vector<int>);

		vector<gesture> returnGestureList();

		vector<string> returnLog();

		void addData(int, int, int);

		void addToLog(string);

		bool dataDetected();

		bool handDetected();

		bool updateLog();

		bool updateGestures();

		vector<Blob> Hands;
	protected:

	private:
		BlobDetection hd_blobDetection;
		HMM c_hmm;
		Log m_log;

		bool log_flag;
		bool gesture_flag;
	};

}