/*****************************************************************************

Author: Steven Cooper
Project: Computer Vision for Interactive Applications
Version:1.0	Date: 03/11/2013 details:Creation of Viewer
Version:1.1 Date: 21/11/2013 details:implement Gussian Blur into app
Version:1.2 Date: 21/11/2013 details:clean up code, remove colourMode
Version:1.3 Date: 27/11/2013 details:Binary image and Hand detection
Version:1.4 Date: 05/12/2013 details:Clean up code and start image moments imp
Version:1.5	Date: 16/12/2013 details:Path Viewer functions created
Version:1.6 Date: 18/12/2013 details:Views created for Path and RGB Hand Detection 
Version:1.7 Date: 29/12/2013 details:CoOrdinate converter used to convert depth x,y to color x,y
Version:1.8 Date: 05/01/2014 details:ImageMoment class implemented into Viewer
Version:1.9 Date: 06/01/2014 details:Image Moments class implemented for N number of prior moments
Version:2.0 Date: 07/01/2014 details:Blob Detection class starting to implement into viewer
Version:2.1 Date: 14/01/2014 details:Blob Detection code removed to review implementation
Version:2.2 Date: 19/01/2014 details:Multi Blob Detection implemented into Binary hand viewer
Version:2.3 Date: 19/01/2014 details:Removal of functions, now found in relevant classes to make more efficient
Version:2.4 Date: 19/01/2014 details:Blob Detection implemented into RGB hand detection
Version:2.5 Date: 19/01/2014 details:New viewer RGB Path created to map path onto RGB image
Version:2.6 Date: 20/01/2014 details:Incorporated different colours into viewer
Version:2.7 Date: 21/02/2014 details:Set up path detection to take in test data
Version:2.8 Date: 29/03/2014 details:Creation of new subWindows for Instructions and Log
Version:2.9 Date: 29/03/2014 details:Creation of display functions for new sub Windows
Version:3.0 Date: 31/03/2014 details:Created update sub windows functions
Version:3.1 Date: 31/03/2014 details:Implemented new Blob Detection into all views
Version:3.2 Date: 02/04/2014 details:Moved Blob Detection into a new gesture recognititon super class

*****************************************************************************/

#ifndef _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#define _SCL_SECURE_NO_WARNINGS

#include "Viewer.h"
#include "GestureRecognition.h"
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <time.h>

#define GL_WIN_SIZE_X	850
#define GL_WIN_SIZE_Y	700
#define TEXTURE_SIZE	512

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

using namespace std;

CVIA::GestureRecognition* cvia_gestureRecognition;

int mainWindow, sub1, sub2, sub3;

PathViewer* PathViewer::ms_self = NULL;

void PathViewer::glutIdle()
{
	glutPostRedisplay();
}
void PathViewer::glutDisplay()
{
	PathViewer::ms_self->display();
}

void PathViewer::glutKeyboard(unsigned char key, int x, int y)
{
	PathViewer::ms_self->onKey(key, x, y);
}

PathViewer::PathViewer(const char* strSampleName, openni::Device& device, openni::VideoStream& depth, openni::VideoStream& color) :
	m_device(device), m_depthStream(depth),m_colorStream(color), m_streams(NULL), m_pTexMap(NULL), viewerMode(RGB_PATH)

{
	ms_self = this;
	strncpy(m_strSampleName, strSampleName, ONI_MAX_STR);
	viewNumber = 1;
	EXIT_FLAG = false;
	c_width = GL_WIN_SIZE_X;
	c_height = GL_WIN_SIZE_Y;
}

PathViewer::~PathViewer()
{
	delete[] m_pTexMap;

	ms_self = NULL;

	if (m_streams != NULL)
	{
		delete []m_streams;
	}
}

openni::Status PathViewer::init(int argc, char **argv)
{
	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;

	if (m_depthStream.isValid() && m_colorStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		colorVideoMode = m_colorStream.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)
		{
			m_width = depthWidth;
			m_height = depthHeight;
		}
		else
		{
			printf("Error - expect color and depth to be in same resolution: D: %dx%d, C: %dx%d\n",
				depthWidth, depthHeight,
				colorWidth, colorHeight);
			return openni::STATUS_ERROR;
		}
	}
	else if (m_depthStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		m_width = depthVideoMode.getResolutionX();
		m_height = depthVideoMode.getResolutionY();
	}
	else if (m_colorStream.isValid())
	{
		colorVideoMode = m_colorStream.getVideoMode();
		m_width = colorVideoMode.getResolutionX();
		m_height = colorVideoMode.getResolutionY();
	}
	else
	{
		printf("Error - expects at least one of the streams to be valid...\n");
		return openni::STATUS_ERROR;
	}

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &m_depthStream;
	m_streams[1] = &m_colorStream;

	// Texture map init
	m_nTexMapX = MIN_CHUNKS_SIZE(m_width, TEXTURE_SIZE);
	m_nTexMapY = MIN_CHUNKS_SIZE(m_height, TEXTURE_SIZE);
	m_pTexMap = new openni::RGB888Pixel[m_nTexMapX * m_nTexMapY];

	cvia_gestureRecognition = new CVIA::GestureRecognition();

	return initOpenGL(argc, argv);

}
openni::Status PathViewer::run()	//Does not return
{
	glutMainLoop();

	return openni::STATUS_OK;
}

void PathViewer::displayMain()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void PathViewer::displayInstructions()
{
	glClearColor(0.7,0.3,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3d(1,1,1);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,8,0,8);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	float xStart = 1.0f, yStart = 7.5f;
	renderBitmapString(xStart, yStart, "Gestures");
	yStart -= 0.5;
	vector<gesture> gestureList =  cvia_gestureRecognition->returnGestureList();
	for(int i = 0; i < gestureList.size(); i++, yStart -= 0.5)
	{
		char *gestureString = (char*)gestureList.at(i).GestureName.c_str();
		renderBitmapString(xStart, yStart, gestureString);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutSwapBuffers();
}

void PathViewer::displayLog()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3d(1,1,1);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,25,0,8);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	int xStart = 1.0f, ystart = 7.0f;
	renderBitmapString(xStart, ystart, "Log:");
	ystart -= 1.5;
	vector<string> gLog = cvia_gestureRecognition->returnLog();
	for(int i = 0; i < gLog.size(); i++, ystart -= 0.5)
	{
		char * logEntry = (char*)gLog.at(i).c_str();
		renderBitmapString(xStart, ystart, logEntry);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutSwapBuffers();
}

void PathViewer::renderBitmapString(float x, float y, char *string)
{
	glRasterPos2d(x,y);
	for(const char* c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void PathViewer::display()
{
	int changedIndex;
	openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
	if (rc != openni::STATUS_OK)
	{
		printf("Wait failed\n");
		return;
	}

	m_colorStream.readFrame(&m_colorFrame);
	m_depthStream.readFrame(&m_depthFrame);


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, GL_WIN_SIZE_X, GL_WIN_SIZE_Y, 0, -1.0, 1.0);

	memset(m_pTexMap, 0, m_nTexMapX*m_nTexMapY*sizeof(openni::RGB888Pixel));

	//switch case based on viewer selection

	switch(viewNumber)
	{
	case 1:
		displayBlobDetection();
		break;
	case 2:
		displayFeatureExtraction();
		break;
	case 3:
		displayPath();
		break;
	case 4:
		displayClassification();
		break;
	}


	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pTexMap);

	// Display the OpenGL texture map
	glColor4f(1,1,1,1);

	glBegin(GL_QUADS);

	int nXRes = m_width;
	int nYRes = m_height;

	// upper left
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	// upper right
	glTexCoord2f((float)nXRes/(float)m_nTexMapX, 0);
	glVertex2f(GL_WIN_SIZE_X, 0);
	// bottom right
	glTexCoord2f((float)nXRes/(float)m_nTexMapX, (float)nYRes/(float)m_nTexMapY);
	glVertex2f(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	// bottom left
	glTexCoord2f(0, (float)nYRes/(float)m_nTexMapY);
	glVertex2f(0, GL_WIN_SIZE_Y);

	glEnd();


	// Swap the OpenGL display buffers
	glutSwapBuffers();
}

void PathViewer::displayBlobDetection()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	cvia_gestureRecognition->init(m_depthFrame.getHeight(), m_depthFrame.getWidth(), rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				cvia_gestureRecognition->addData(x,y, *pDepth);
			}
		}
		pDepthRow += rowSize;
	}

	if(cvia_gestureRecognition->dataDetected())
	{
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_depthFrame.getCropOriginX();
			const openni::DepthPixel* pDepth = pDepthRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pDepth)
			{
				if(*pDepth != 0 && *pDepth < 600)
				{
					pTex->r = 255;
					pTex->g = 255;
					pTex->b = 255;
				}
			}
			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}
	if(cvia_gestureRecognition->updateLog())
	{
		executeGesture(cvia_gestureRecognition->returnLog().at(cvia_gestureRecognition->returnLog().size()-1));
		updateLog();
	}
	if(cvia_gestureRecognition->updateGestures()) updateGestures();
}

void PathViewer::displayFeatureExtraction()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	cvia_gestureRecognition->init(m_depthFrame.getHeight(), m_depthFrame.getWidth(), rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				cvia_gestureRecognition->addData(x,y, *pDepth);
			}
		}
		pDepthRow += rowSize;
	}

	if(cvia_gestureRecognition->dataDetected())
	{
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_depthFrame.getCropOriginX();
			const openni::DepthPixel* pDepth = pDepthRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pDepth)
			{
				if(*pDepth != 0 && *pDepth < 600)
				{
					pTex->r = 255;
					pTex->g = 255;
					pTex->b = 255;
				}

				for(int i = 0; i < cvia_gestureRecognition->Hands.size(); i++)
				{
					if(cvia_gestureRecognition->Hands.at(i).imageMoment.isBorder(x,y))
					{
						pTex->r = 255;
						pTex->g = 0;
						pTex->b = 0;
					}

					if(cvia_gestureRecognition->Hands.at(i).imageMoment.isCenter(x,y))
					{
						pTex->r = 0;
						pTex->g = 255;
						pTex->b = 255;
					}
				}
			}
			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}
	if(cvia_gestureRecognition->updateLog())
	{
		executeGesture(cvia_gestureRecognition->returnLog().at(cvia_gestureRecognition->returnLog().size()-1));
		updateLog();
	}
	if(cvia_gestureRecognition->updateGestures()) updateGestures();
}

void PathViewer::displayPath()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	cvia_gestureRecognition->init(m_depthFrame.getHeight(), m_depthFrame.getWidth(), rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				cvia_gestureRecognition->addData(x,y, *pDepth);
			}
		}
		pDepthRow += rowSize;
	}

	if(cvia_gestureRecognition->dataDetected())
	{
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_depthFrame.getCropOriginX();
			const openni::DepthPixel* pDepth = pDepthRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pDepth)
			{
				if(*pDepth != 0 && *pDepth < 600)
				{
					pTex->r = 255;
					pTex->g = 255;
					pTex->b = 255;
				}



				for(int i = 0; i < cvia_gestureRecognition->Hands.size(); i++)
				{
					if(!cvia_gestureRecognition->handDetected())
						if(cvia_gestureRecognition->Hands.at(i).imageMoment.isBorder(x,y))
						{
							pTex->r = 255;
							pTex->g = 0;
							pTex->b = 0;
						}

						if(cvia_gestureRecognition->Hands.at(i).m_path.at(x + (y * rowSize)) == 1)
						{
							pTex->r = 0;
							pTex->g = 255;
							pTex->b = 255;
						}
				}
			}
			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}
	if(cvia_gestureRecognition->updateLog())
	{
		executeGesture(cvia_gestureRecognition->returnLog().at(cvia_gestureRecognition->returnLog().size()-1));
		updateLog();
	}
	if(cvia_gestureRecognition->updateGestures()) updateGestures();
}

void PathViewer::displayClassification()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int d_rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
	int c_rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

	int* colorPixelX = new int(0);
	int* colorPixelY = new int(0);

	cvia_gestureRecognition->init(m_colorFrame.getHeight(), m_colorFrame.getWidth(), c_rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				openni::CoordinateConverter().convertDepthToColor(m_depthStream, m_colorStream, x, y, *pDepth, colorPixelX, colorPixelY);
				cvia_gestureRecognition->addData(*colorPixelX, *colorPixelY, *pDepth);
			}
		}
		pDepthRow += d_rowSize;
	}

	if(cvia_gestureRecognition->dataDetected())
	{
		const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		for (int y = 0; y < m_colorFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_colorFrame.getCropOriginX();
			const openni::RGB888Pixel* pImage = pImageRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pImage)
			{
				*pTex = *pImage;

				for(unsigned int i = 0; i < cvia_gestureRecognition->Hands.size(); i++)
				{
					if(!cvia_gestureRecognition->handDetected())
						if(cvia_gestureRecognition->Hands.at(i).imageMoment.isBorder(x, y))
						{
							pTex->r = 0;
							pTex->g = 255;
							pTex->b = 255;
						}

						if(cvia_gestureRecognition->Hands.at(i).m_path.at(x + (y * c_rowSize)) == 1)
						{
							pTex->r = 0;
							pTex->g = 255;
							pTex->b = 255;
						}
				}
			}
			pImageRow += c_rowSize;
			pTexRow += m_nTexMapX;
		}
	}
	else
	{
		const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		for (int y = 0; y < m_colorFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_colorFrame.getCropOriginX();
			const openni::RGB888Pixel* pImage = pImageRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pImage)
			{
				*pTex = *pImage;
			}
			pImageRow += c_rowSize;
			pTexRow += m_nTexMapX;
		}
	}
	if(cvia_gestureRecognition->updateLog())
	{
		executeGesture(cvia_gestureRecognition->returnLog().at(cvia_gestureRecognition->returnLog().size()-1));
		updateLog();
	}
	if(cvia_gestureRecognition->updateGestures()) updateGestures();
}

void PathViewer::onKey(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key)
	{
	case 49:
		viewerMode = HAND_DETECTION;
		break;
	case 50:
		viewerMode = PATH;
		break;
	case 51:
		//viewerMode = RGB_HAND_DETECTION;
		break;
	case 52:
		viewerMode = RGB_PATH;
		break;
	case 27:
		m_depthStream.stop();
		m_depthStream.destroy();
		//m_device.close();
		openni::OpenNI::shutdown();

		exit (1);
	}

}

openni::Status PathViewer::initOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	mainWindow = glutCreateWindow (m_strSampleName);

	glutDisplayFunc(displayMain);
	glutKeyboardFunc(glutKeyboard);
	glutIdleFunc(glutIdle);

	sub2 = glutCreateSubWindow(mainWindow, 650, 5, 195, 512);

	glutDisplayFunc(displayInstructions);
	glutKeyboardFunc(glutKeyboard);
	glutIdleFunc(glutIdle);

	sub3 = glutCreateSubWindow(mainWindow, 5, 523, 840, 170);

	glutDisplayFunc(displayLog);
	glutKeyboardFunc(glutKeyboard);
	glutIdleFunc(glutIdle);

	sub1 = glutCreateSubWindow(mainWindow, 5,5,640,512);

	glutSetWindow(sub1);

	glutSetCursor(GLUT_CURSOR_NONE);

	initOpenGLHooks();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	return openni::STATUS_OK;

}

void PathViewer::initOpenGLHooks()
{
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
}

void PathViewer::updateLog()
{
	glutSetWindow(sub3);
	displayLog();
	glutSetWindow(sub1);
}

void PathViewer::updateGestures()
{
	glutSetWindow(sub2);
	displayInstructions();
	glutSetWindow(sub1);
}

void PathViewer::executeGesture(string temp)
{
	if(temp == "" || temp == "Not" || temp == "One Hand Detected" || temp == "Two Hands Detected") return;
	else
	{
		if(temp.find("swipe_left") != string::npos) 
		{
			if(EXIT_FLAG) 
			{
				m_depthStream.stop();
				m_depthStream.destroy();
				openni::OpenNI::shutdown();

				exit (1);
			}
			else if(viewNumber != MAX_VIEW) viewNumber++;
		}
		else if(temp.find("swipe_right") != string::npos) 
		{
			if(EXIT_FLAG)
			{
				EXIT_FLAG = false;
			}
			else if(viewNumber != 1) viewNumber--;
		}
		else if(temp.find("delete") != string::npos)
		{
			EXIT_FLAG = true;
			cvia_gestureRecognition->addToLog("Exit? Swipe Left ""Yes"", Swipe Right ""No");
		}
		else if(temp.find("wave") != string::npos)
		{
			viewNumber = 1;
			EXIT_FLAG = false;
			showLog();
			glutSetWindow(sub2);
			glutShowWindow();

			glutSetWindow(sub3);
			glutShowWindow();

			glutSetWindow(mainWindow);
			c_height = GL_WIN_SIZE_Y;
			c_width = GL_WIN_SIZE_X;
			glutReshapeWindow(c_width, c_height);

			glutSetWindow(sub1);

			glutPostRedisplay();

		}
		else if(temp.find("scroll_down") != string::npos)
		{
			hideLog();
		}
		else if(temp.find("scroll_up") != string::npos)
		{
			showLog();
		}
		else if(temp.find("swipe left") != string::npos)
		{
			glutSetWindow(sub2);
			glutShowWindow();
			glutSetWindow(sub1);
		}
		else if(temp.find("swipe right") != string::npos)
		{
			glutSetWindow(sub2);
			glutHideWindow();
			glutSetWindow(sub1);
		}
		else if(temp.find("shrink width") != string::npos)
		{
			glutSetWindow(mainWindow);
			c_width *= 0.8;
			glutReshapeWindow(c_width, c_height);
			glutPostRedisplay();
		}
		else if(temp.find("shrink height") != string::npos)
		{
			glutSetWindow(mainWindow);
			c_height *= 0.8;
			glutReshapeWindow(c_width, c_height);
			glutPostRedisplay();
		}
	}
}

void PathViewer::hideLog()
{
	glutSetWindow(sub3);
	glutHideWindow();

	glutSetWindow(sub1);
}

void PathViewer::showLog()
{
	glutSetWindow(sub3);
	glutShowWindow();

	glutSetWindow(sub1);
}
