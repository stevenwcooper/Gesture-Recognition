/*****************************************************************************
                                                                          
	Author: Steven Cooper
	Project: Computer Vision for Interactive Applications
	Version:1.0 Date: 03/11/2013 details: creation of Main function

*****************************************************************************/
#include <OpenNI.h>
#include "Viewer.h"
#include "Viewer.h"

int main(int argc, char** argv)
{
	openni::Status rc = openni::STATUS_OK;

	openni::Device device;
	openni::VideoStream depth;
	openni::VideoStream color;
	const char* deviceURI = openni::ANY_DEVICE;
	if (argc > 1)
	{
		deviceURI = argv[1];
	}

	rc = openni::OpenNI::initialize();

	printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		printf("FYPViewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
		openni::OpenNI::shutdown();
		return 1;
	}

	rc = depth.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		rc = depth.start();
		if (rc != openni::STATUS_OK)
		{
			printf("FYPViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			depth.destroy();
		}
	}
	else
	{
		printf("FYPViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	}

	rc = color.create(device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK)
	{
		rc = color.start();
		if (rc != openni::STATUS_OK)
		{
			printf("FYPViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			color.destroy();
		}
	}
	else
	{
		printf("FYPViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	}

	if (!depth.isValid() && !color.isValid())
	{
		printf("FYPViewer: No valid streams. Exiting\n");
		openni::OpenNI::shutdown();
		return 2;
	}

	PathViewer PathViewer("Simple Viewer", device, depth, color);

	rc = PathViewer.init(argc, argv);
	if (rc != openni::STATUS_OK)
	{
		openni::OpenNI::shutdown();
		return 3;
	}
	PathViewer.run();
}