#include "Log.h"

Log::Log()
{

}

Log::~Log()
{

}

void Log::addToLog(string entry)
{
	if(gestureLog.size() >= 5) gestureLog.erase(gestureLog.begin());

	gestureLog.push_back(entry);
}

vector<string> Log::getLog()
{
	return gestureLog;
}