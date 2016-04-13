
#include <iostream>
#include <string>

#include "Controller.h"
#include "Util.h"

#include "common.h"
#include "DataAcquisitionApi.h"

#include "common.h"
#include "TrajectoryApi.h"
#include "Trajectory.h"
#include "NIDacCard.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include <ctime>
#include <cstdio>
#include "Ctr_logger.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


using namespace std;


// trajectory calibration params...
float fXMaxV = 9.9;
float fYMaxV = 9.9;
float fXScaleVM = 0.98; // [v/mm]
float fYScaleVM = 0.98; // [v/mm]

uInt_32 uiUpdateRate = 4000; //[points/sec]
float fWaveEffi = 0.85;
std::string sPhyChannel = "Dev1";

float fExposureTime = 0.00022;

int iPreviewAScan = 400;
int iBackgroundAScan = 200;
int iFundusViewAScan = 100;

// scanpattern params...
float fXRangeV = 6;
float fYRangeV = 6;
float fXOffsetV = 0;
float fYOffsetV = 0;

uInt_32 uiNumPixel = 1024;
uInt_32 uiNumAscan = 400;
uInt_32 uiNumBscan = 400;
uInt_32 uiNumBcgrFrame = 0; // background frames
uInt_32 uiNumBscanRepeat = 3;
//dac calinration
std::string sXChanName = "Dev1/ao1";
std::string sYChanName = "Dev1/ao2";
std::string sSwitchChanName = "Dev1/ao0";
bool bExterClked = false;
std::string sExterClkChanName = "PFI5";
unsigned long ulTrajGenRate = 4000;
float fFundusXOffsetV = 2 * fXScaleVM;
float fFundusYOffsetV = 2 * fYScaleVM;

// data process calibration
// data process calibration
int batchFrames = 1;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
int batchAscans = 1;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
int prevFreq = 1;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
bool reslice = false;						// reslice into enface view								

int numAscansPerBscan = 400;
int numBScans = 400;
int numCameraPixels = 1024;
int numBgFrame = 0;
int startPixel = 15;
int endPixel = 480;
float alpha = 0.5;
int grayLevel = 30;
bool interpLinear = true;

bool dispModeGS = true; // not used as switch any more since hard coded in Contoller::resetDispersionParams, but should be kept to init GPU code
int dispFrame = 100;
int dispTests = 100;
float dispA2 = 0.00909076;
float dispA3 = 2.44613e-7;

std::string fnamePhase = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";

std::string fAcquireDataSet = "E:\\test\\acquireDataSet";
std::string fbackground = "E:\\test\\background";
std::string fpreviewxz = "E:\\test\\previewxz";
std::string fpreviewyz = "E:\\test\\previewyz";
std::string fFundusDataSet = "E:\\test\\fundusDataSet";

int main()
{
	int returnValue;

	//INFO2 << "hello log";

	std::remove("sdoct_0.log");
	//freopen("main.log", "w", stdout);
	std::cout<<"TRACE: "<<__FUNCTION__<<std::endl;
	Controller* controller = new Controller();
	controller->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
	controller->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
	controller->setControllerCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fbackground, fpreviewxz, fpreviewyz, fAcquireDataSet, fFundusDataSet);
	controller->setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, sXChanName, sYChanName, sSwitchChanName, bExterClked, sExterClkChanName, ulTrajGenRate);
	controller->setPreviewPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, /*numBScans*/ 1, numCameraPixels, numBgFrame,
										startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
										fnamePhase, fbackground+".bin", fpreviewxz+".bin");
	controller->setFundusPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, iFundusViewAScan, /*numBScans*/ iFundusViewAScan, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fbackground+".bin", fpreviewxz+".bin");

	controller->setAcquirePrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame, uiNumBscanRepeat, 
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fbackground+".bin", fpreviewxz+".bin");

	returnValue = controller->initTrajectory();
	if (0 != returnValue)
	{
		cout<<"initTrajectory error\n";
		system("pause");
		return -1;

	}

	CTR_LOGGER_INFO <<"-----------------------------------------------";
	controller->initNIDacCard();
	CTR_LOGGER_INFO <<"-----------------------------------------------";

	
	returnValue = controller->getBackground();
	if (0 != returnValue)
	{
		cout<<"get bcgr error\n";
		system("pause");
		return -1;

	}
	CTR_LOGGER_INFO <<"-----------------------------------------------";
	controller->processPreviewBcgr();
	CTR_LOGGER_INFO <<"-----------------------------------------------";
	
	int i = 0;
	std::clock_t start = std::clock();
	while (i<1)
	{
		i++;
		returnValue = controller->getPreviewXZ();
		controller->processPreviewFrameData(fpreviewxz+".bin");

		//std::vector<short> acquireData = controller->getAcquireDataSet();
		//
		std::cout<<"DEBUG: "<<__FUNCTION__<< (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	}
	
	//system("pause");
	
	//controller->processPreviewBcgr(); // process background data
	//controller->processPreviewFrameData(fpreviewxz+".bin");
	
	/*
	// acquire data set process test
	returnValue = controller->getAcquireDataSet();
	controller->processAcquireBcgr();
	controller->processAcquireFrameData(fAcquireDataSet+".bin");
	*/

	
	// fundus data set process test
	returnValue = controller->getFundusDataSet();
	controller->processFundusBcgr();
	controller->processFundusFrameData(fFundusDataSet+".bin");
	

	delete controller; controller = NULL;

	std::cout<<"INFO: "<<"All Done."<<std::endl;
	system("pause");
	return 0;
}
