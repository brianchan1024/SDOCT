#pragma once

#ifndef ProcessorWrap_h
#define ProcessorWrap_h

#include "prcssr.h"

//using namespace System;
//using namespace System::Runtime::InteropServices;

class PrcssrAccess
{
private:
	Prcssr *myPrcssr;

	int batchFrames;
	int batchAscans;
	int prevFreq;
	bool reslice;

	int numAscansPerBscan;
	int numBScans;
	int numCameraPixels;
	int numBgFrame;
	int startPixel;
	int endPixel;
	float alpha;
	int grayLevel;
	bool interpLinear;

	bool dispModeGS;
	int dispFrame;
	int dispTests;
	float dispA2;
	float dispA3;

	std::string fnamePhase;
	std::string fnameBg;
	std::string fnameData;

	int returnValue;

public:

	void initDLL();

	int param();

	int data();

	int interp();

	int disp();
	int disp(int dispModeGS, float &a2, float &a3);

	int bg();

	int output();

	int output(std::string str, std::string datatype);

	// Errors if this conversion is done directly. Some sort of bug in vs2010, workaround with method.
	// Not a bug anymore? Not sure, can't find the stack overflow post. I'm leaving it wrapped in the function.

	string sysStrToStdStr(std::string str);

	// When batching directories, we need to recalculate the background for each folder. To do this, the background
	//  class needs to be freed and recreated. This function handles that, passing in the path to the new background file.
	int freeBg(std::string str);

	void destroy();
	// Turn the error string into an error code which the GUI understands.
	void runErrorCheck(std::exception e, int &rv);

	// getters and setters.
	void setBatchFrames(int batchFrames);
	void setBatchAscans(int batchAscans);
	void setPrevFreq(int prevFreq);
	void setReslice(bool reslice);

	void setNumAscansPerBscan(int numAscansPerBscan);
	void setNumBScans(int numBScans);
	void setNumCameraPixels(int numCameraPixels);
	void setNumBgFrame(int numBgFrame);
	void setStartPixel(int startPixel);
	void setEndPixel(int endPixel);
	void setAlpha(float alpha);
	void setGrayLevel(int grayLevel);
	void setInterpLinear(bool interpLinear);

	void setDispModeGS(bool dispModeGS);
	void setDispFrame(int dispFrame);
	void setDispTests(int dispTests);
	void setDispA2(float dispA2);
	void setDispA3(float dispA3);

	void setFnamePhase(std::string fnamePhase);
	void setFnameBg(std::string fnameBg);
	void setFnameData(std::string fnameData);

	int getBatchFrames();
	int getBatchAscans();
	int getPrevFreq();
	bool getReslice();

	int getNumAscansPerBscan();
	int getNumBScans();
	int getNumCameraPixels();
	int getNumBgFrame();
	int getStartPixel();
	int getEndPixel();
	float getAlpha();
	int getGrayLevel();
	bool getInterpLinear();

	bool getDispModeGS();
	int getDispFrame();
	int getDispTests();
	float getDispA2();
	float getDispA3();

	std::string getFnamePhase();
	std::string getFnameBg();
	std::string getFnameData();
};

#endif