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
Version:2.8 Date: 04/03/2014 details:Rework initialisation of HMM so only have to change details in one place

*****************************************************************************/

#ifndef _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#define _SCL_SECURE_NO_WARNINGS

#include "Viewer.h"
#include "BlobDetection.h"
#include "PathDetection.h"
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <time.h>

#define GL_WIN_SIZE_X	640
#define GL_WIN_SIZE_Y	512
#define TEXTURE_SIZE	512

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

using namespace std;

BlobDetection* bd;
PathDetection* pd;

int mainWindow, sub1, sub2;

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

	bd = new BlobDetection();

	pd = new PathDetection();

	pd->initHiddenMarkovModel(3,10, 7, 2, "delete_50");

	pd->m_pPath = vector<short>(m_width * m_height);

	return initOpenGL(argc, argv);

}
openni::Status PathViewer::run()	//Does not return
{
	glutMainLoop();

	return openni::STATUS_OK;
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

	if(viewerMode == PATH || viewerMode == HAND_DETECTION) m_depthStream.readFrame(&m_depthFrame);
	else if(viewerMode == RGB_HAND_DETECTION || viewerMode == RGB_PATH) 
	{
		m_colorStream.readFrame(&m_colorFrame);
		m_depthStream.readFrame(&m_depthFrame);
	}
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, GL_WIN_SIZE_X, GL_WIN_SIZE_Y, 0, -1.0, 1.0);

	memset(m_pTexMap, 0, m_nTexMapX*m_nTexMapY*sizeof(openni::RGB888Pixel));

	//switch case based on viewer selection

	if(viewerMode == HAND_DETECTION) displayHandDetection();
	else if(viewerMode == PATH) displayPath();
	else if(viewerMode == RGB_HAND_DETECTION) displayRGBHandDetection();
	else if(viewerMode == RGB_PATH) displayRGBPath();


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

void PathViewer::displayHandDetection()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	bd->init(m_depthFrame.getHeight(), m_depthFrame.getWidth(), rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				bd->addToBlob(x,y);
			}
		}
		pDepthRow += rowSize;
	}

	if(bd->hasBlobs())
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

				for(unsigned int i = 0; i < bd->Blobs.size(); i++)
				{
					if(bd->Blobs.at(i).imageMoment.area > 100)
					{
						if(bd->Blobs.at(i).imageMoment.isBorder(x,y))
						{
							pTex->r = bd->Blobs.at(i).blobColour.red;
							pTex->g = bd->Blobs.at(i).blobColour.green;
							pTex->b = bd->Blobs.at(i).blobColour.blue;
						}

						if(bd->Blobs.at(i).imageMoment.isCenter(x,y))
						{
							pTex->r = 0;
							pTex->g = 255;
							pTex->b = 0;
						}
					}
				}
			}
			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}
}

void PathViewer::displayPath()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	pd->init(m_depthFrame.getWidth(),m_depthFrame.getHeight(), rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{ 
			if (*pDepth != 0 && *pDepth < 575)
			{
				pd->addToPath(x,y, *pDepth);
			}
		}
		pDepthRow += rowSize;
	}

	if(pd->hasPath())
	{
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			openni::RGB888Pixel* pTex = pTexRow + m_depthFrame.getCropOriginX();
			const openni::DepthPixel* pDepth = pDepthRow;
			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pTex, ++pDepth)
			{

				//for(unsigned int i = 0; i < bd->Blobs.size(); i++)
				//{
					
						if(pd->m_pPath.at(x + (y * rowSize)) == 1)
						{
							pTex->r = 255;
							pTex->g = 255;
							pTex->b = 0;
						}
				//}
			}
			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}
}

void PathViewer::displayRGBHandDetection()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int d_rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
	int c_rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

	int* colorPixelX = new int(0);
	int* colorPixelY = new int(0);

	bd->init(m_colorFrame.getHeight(), m_colorFrame.getWidth(), c_rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				openni::CoordinateConverter().convertDepthToColor(m_depthStream, m_colorStream, x, y, *pDepth, colorPixelX, colorPixelY);
				bd->addToBlob(*colorPixelX, *colorPixelY);
			}
		}
		pDepthRow += d_rowSize;
	}

	if(bd->hasBlobs())
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

				for(unsigned int i = 0; i < bd->Blobs.size(); i++)
				{
					if(bd->Blobs.at(i).imageMoment.area > 100)
					{
						if(bd->Blobs.at(i).imageMoment.isCenter(x,y))
						{
							pTex->r = 0;
							pTex->g = 255;
							pTex->b = 0;
						}

						if(bd->Blobs.at(i).imageMoment.isBorder(x,y))
						{
							pTex->r = bd->Blobs.at(i).blobColour.red;
							pTex->g = bd->Blobs.at(i).blobColour.green;
							pTex->b = bd->Blobs.at(i).blobColour.blue;
						}
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
}

void PathViewer::displayRGBPath()
{
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int d_rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
	int c_rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

	int* colorPixelX = new int(0);
	int* colorPixelY = new int(0);

	pd->init(m_colorFrame.getHeight(), m_colorFrame.getWidth(), c_rowSize);

	for (int y = 0; y < m_depthFrame.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 600)
			{
				openni::CoordinateConverter().convertDepthToColor(m_depthStream, m_colorStream, x, y, *pDepth, colorPixelX, colorPixelY);
				pd->addToPath(*colorPixelX, *colorPixelY, *pDepth);
			}
		}
		pDepthRow += d_rowSize;
	}

	if(pd->hasPath())
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

				for(unsigned int i = 0; i < bd->Blobs.size(); i++)
				{
					
						if(pd->m_pPath.at(x + (y * c_rowSize)) == 1)
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
	case 53:
		pd->hmm.Train_HMM();
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
	// 	glutFullScreen();
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
