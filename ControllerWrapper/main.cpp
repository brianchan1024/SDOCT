#include "Stdafx.h"
#include <iostream>
#include "ControllerWrapper.h"

using namespace std;

int main()
{
	ControllerWrapper::ControllerAccess controller = gcnew ControllerWrapper::ControllerAccess();
	controller.initiate();

	system("pause");
	

	int returnValue = 0;
	float fXMaxV = 9.9;
	float fYMaxV = 9.9;
	float fXScaleVM = 0.98; // [v/mm]
	float fYScaleVM = 0.98; // [v/mm]

	uInt_32 uiUpdateRate = 4000; //[points/sec]
	float fWaveEffi = 0.85;
	std::string sPhyChannel = "Dev1";

	float fExposureTime = 0.00022;

	int iPreviewAScan = 400;
	int iBackgroundAScan = 400;

	// scanpattern params...
	float fXRangeV = 6;
	float fYRangeV = 6;
	float fXOffsetV = 0;
	float fYOffsetV = 0;

	uInt_32 uiNumPixel = 1024;
	uInt_32 uiNumAscan = 400;
	uInt_32 uiNumBscan = 30;
	uInt_32 uiNumBcgrFrame = 3; // background frames
	uInt_32 uiNumBscanRepeat = 3;

	//dac calinration
	std::string sXChanName = "Dev1/ao1";
	std::string sYChanName = "Dev1/ao2";
	std::string sSwitchChanName = "Dev1/ao0";
	bool bExterClked = false;
	std::string sExterClkChanName = "PFI5";
	unsigned long ulTrajGenRate = 4000;


	// data process calibration
	// data process calibration
	int batchFrames = 1;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
	int batchAscans = 1;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
	int prevFreq = 1;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
	bool reslice = false;						// reslice into enface view								

	int numAscansPerBscan = 400;
	int numBScans = 30;
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

	/*
	std::string fnamePhase = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";

	std::string fAcquireDataSet = "E:\\test\\acquireDataSet";
	std::string fbackground = "E:\\test\\background";
	std::string fpreviewxz = "E:\\test\\previewxz";
	std::string fpreviewyz = "E:\\test\\previewyz";

	//controller = gcnew ControllerWrapper::ControllerAccess();
	controller.setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
	controller.setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan);
	controller.setControllerCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame,  iPreviewAScan, iBackgroundAScan, fbackground, fpreviewxz, fpreviewyz, fAcquireDataSet);
	controller.setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, sXChanName, sYChanName, sSwitchChanName, bExterClked, sExterClkChanName, ulTrajGenRate);
	controller.setPreviewPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, 1, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fbackground+".bin", fpreviewxz+".bin");
	controller.setAcquirePrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fbackground+".bin", fpreviewxz+".bin");

	returnValue = controller.initTrajectory();
	if (0 != returnValue)
	{
		cout<<"initTrajectory error\n";
		system("pause");
		return -1;

	}

	controller.initNIDacCard();


	returnValue = controller.getBackground();
	if (0 != returnValue)
	{
		cout<<"get bcgr error\n";
		system("pause");
		return -1;

	}
	returnValue = controller.processPreviewBcgr();
	if (0 != returnValue)
	{
		cout<<"process bcgr error\n";
		system("pause");
		return -1;

	}
	*/

	return 0;

}