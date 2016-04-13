
#pragma once
#include <iostream>
#include "ProcessorCalibration.h"
#include "ProcessorApi.h"
//#include "Processor Wrap.h"

//#using  "..\x64\Debug\Processor Wrap.dll"
using namespace std;

int batchFrames = 1;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
int batchAscans = 3;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
int prevFreq = 1;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
bool reslice = false;						// reslice into enface view								
// Image settings. Same as MATLAB.

int numAscansPerBscan = 15;
int numBScans = 1;
int numCameraPixels = 1024;
int numBgFrame = 0;
int startPixel = 15;
int endPixel = 480;
float alpha = 0.5;
int grayLevel = 30;
bool interpLinear = true;

bool dispModeGS = true;
int dispFrame = 100;
int dispTests = 100;
float dispA2 = 0.00909076;
float dispA3 = 2.44613e-7;

//std::string fnamePhase = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";
//std::string fnameBg = "E:\\test\\0.bin";
//std::string fnameData = "E:\\test\\1.bin";

int main()
{
	std::string fnamePhase = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";
	std::string fnameBg = "E:\\test\\0.bin";
	std::string fnameData = "E:\\test\\1.bin";
	std::string fnameData2 = "E:\\test\\2.bin";

	ProcessorApi* prcsrApi = new ProcessorApi();
	prcsrApi->setProcessorCalibration(batchFrames, batchAscans, prevFreq, reslice,
		numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear,
		dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fnameBg, fnameData);

	prcsrApi->processBg();
	prcsrApi->processFrameData(fnameData2, "test");
	delete prcsrApi;
	return 0;
}
