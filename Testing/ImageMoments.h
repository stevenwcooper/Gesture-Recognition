/*************************************************************************
*
*	Author: Steven Cooper
*	Project: Computer Vision for Iteractive Applications
*	Version:1.0 date:05/01/2014 details: Creation of Image Moments class
*	Version:1.1 date:06/01/2014	details: Completion of Image Moments class
*
*************************************************************************/

#ifndef _IMAGEMOMENTS_H_

#include "ImageMoment.h"
#include <vector>

class ImageMoments
{
public:
	ImageMoments();
	~ImageMoments();

	void addMoment(ImageMoment iM);

	std::vector<ImageMoment> ImageMomentList;

	void clearStack();

protected:

private:

};

ImageMoments::ImageMoments()
{
	ImageMomentList.clear();
}

ImageMoments::~ImageMoments()
{
	ImageMomentList.clear();
}

void ImageMoments::addMoment(ImageMoment iM)
{
	ImageMomentList.insert(ImageMomentList.begin(), iM);

	if(ImageMomentList.size() > 5) 
	{
		ImageMomentList.pop_back();
	}

	for(unsigned int i = 1; i < ImageMomentList.size(); i++)
	{
	ImageMomentList.at(i).length =  (float)(ImageMomentList.at(i).length * 0.75);
	ImageMomentList.at(i).width = (float)(ImageMomentList.at(i).width * 0.75);
	}
}

void ImageMoments::clearStack()
{
	ImageMomentList.clear();
}

#endif // !1