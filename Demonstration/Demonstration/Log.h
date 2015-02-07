/************************************************************************
*
*	Author: Steven Cooper
*	Project: Computer Vision for Interactive Applications
*	Version: 1.0 Date:30/03/2014 details: Log moved from gesture class to own class
*
**************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

class Log
{

public:
	Log();
	~Log();

	void addToLog(string);
	vector<string> getLog();
protected:

private:
	vector<string> gestureLog;
};