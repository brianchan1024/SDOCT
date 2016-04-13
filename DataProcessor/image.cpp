#include "image.h"
#include <iostream> // why was this included here?

using namespace cv;

Image::Image(int h, int w, float *im) : imageArray(im)
{
	height = w;
	width = h;
}

Image::~Image()
{
}

void Image::dspl()
{
	curImg = Mat(height, width, CV_32FC1, imageArray);
	normImg(curImg);

	namedWindow("Dispersion Result", CV_WINDOW_NORMAL);
	resizeWindow("Dispersion Result", width/1.5, height/1.5);
	moveWindow("Dispersion Result",(80+(width/1.5)),50);
	destroyWindow("Gridsearch");
	imshow("Dispersion Result", curImg);

	// check if esc was pressed
	char k = waitKey(0);
	if (k == 27)
	{
		destroyWindow("Dispersion Result");
		destroyWindow("Select an ROI with high dynamic range");
		throw std::invalid_argument("Gridsearch Error");
	}

	destroyWindow("Dispersion Result");
	destroyWindow("Select an ROI with high dynamic range");
}

void Image::dsplGS()
{
	curImg = Mat(height, width, CV_32FC1, imageArray);
	normImg(curImg);

	namedWindow("Gridsearch", CV_WINDOW_AUTOSIZE);
	imshow("Gridsearch", curImg);
	waitKey(1);
}

void Image::getPts()
{
	curImg = Mat(height, width, CV_32FC1, imageArray);
	normImg(curImg);

	namedWindow("Select an ROI with high dynamic range", CV_WINDOW_NORMAL);
	resizeWindow("Select an ROI with high dynamic range", width/1.5, height/1.5);
	moveWindow("Select an ROI with high dynamic range",50,50);
	setMouseCallback("Select an ROI with high dynamic range", onMouse, this);

	while (1)
	{
		imshow("Select an ROI with high dynamic range", curImg);
		if (ROIpts.size() == 2)
			break;
		cv::waitKey(10);
	}
}

void Image::onMouse(int event, int x, int y, int, void* points)
{
    if (event != CV_EVENT_LBUTTONUP) 
		return;
	// check for null pointer
	Image *thisImage = reinterpret_cast<Image*>(points);
	thisImage->onMouse(event, x, y);
}

void Image::onMouse(int event, int x, int y)
{
	ROIpts.push_back(Point(x,y));
}

void Image::normImg(Mat &image)
{
	double maxVal = 0;
	double minVal = 0;
	Point minLoc = 0;
	Point maxLoc = 0;
	minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);

	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++)
			image.at<float>(i, j) = (image.at<float>(i, j) - (float)minVal)/((float)maxVal - (float)minVal); 
	}
}