
#include <iostream>
#include <string>
#include "common.h"
#include "DataAcquisitionApi.h"

#include "common.h"
#include "TrajectoryApi.h"
#include "Trajectory.h"
#include "NIDacCard.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <exception>

#using  "..\x64\Debug\Processor Wrap.dll"

using namespace std;
using namespace System;
using namespace std;

namespace main_prc{
uInt_32 uiBatchFrame = 1;
uInt_32 uiBatchAscan = 10;
uInt_32 uiPrevFrequency = 40;
bool bReslice = false;

uInt_32 uiNumPixel = 1024;
uInt_32 uiNumAscan = 800;
uInt_32 uiNumBscan = 800;
uInt_32 uiNumBcgrFrame = 3; // background frames

uInt_32 uiStartPixel = 15;
uInt_32 uiEndPixel = 480;
uInt_32 alpha = 0.5;
uInt_32 uiGrayLevel = 30;
bool bInterpLinear = true;

bool bGridSearch = true;
float fDisA2 = 0.00909076;
float fDisA3 = 2.44613e-7;
uInt_32 uiDispTests = 100;
uInt_32 uiDispFrame = 100;






	void errorMsg(int returnValue);
int main2()
{
	ProcessorWrap::PrcssrAccess^ prcssrApi=gcnew ProcessorWrap::PrcssrAccess();
	//ProcessorApi::GPUProcessorApi* prcssrApi = new ProcessorApi::GPUProcessorApi();
	prcssrApi->setBatchFrames(uiBatchFrame);
	prcssrApi->setBatchAscans(uiBatchAscan);
	prcssrApi->setPrevFreq(uiPrevFrequency);
	prcssrApi->setReslice(bReslice);

	prcssrApi->setNumAscansPerBscan(uiNumAscan);
	prcssrApi->setNumBScans(uiNumBscan);
	prcssrApi->setNumCameraPixels(uiNumPixel);
	prcssrApi->setNumBgFrame(uiNumBcgrFrame);
	prcssrApi->setStartPixel(uiStartPixel);
	prcssrApi->setEndPixel(uiEndPixel);
	prcssrApi->setAlpha(alpha);
	prcssrApi->setGrayLevel(uiGrayLevel);
	prcssrApi->setInterpLinear(bInterpLinear);

	// What kind of dispersion compensation was selected?
	prcssrApi->setDispModeGS(bGridSearch);
	//if (rb_gridsearch->Checked)
	if (bGridSearch)
	{
		prcssrApi->setDispA2(0.f);
		prcssrApi->setDispA3(0.f);
		prcssrApi->setDispTests(uiDispTests);
		prcssrApi->setDispFrame(uiDispFrame);
	}
	else
	{
		prcssrApi->setDispA2(fDisA2);
		prcssrApi->setDispA3(fDisA3);
		prcssrApi->setDispTests(0);
		prcssrApi->setDispFrame(0);
	}

	System::String^ sPhraseFileName = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";
	System::String^ sBgFileName = "E:\\Data\\2015-05-29_SHR_e11_5_mouse_embryo_heart\\SHR_e11_5_mouse_embryo_OD_U-3D_ 9x 9_R00.bin";
	System::String^ sDataFileName = "E:\\Data\\2015-05-29_SHR_e11_5_mouse_embryo_heart\\SHR_e11_5_mouse_embryo_OD_U-3D_ 9x 9_R01.bin";


	prcssrApi->setFnamePhase(sPhraseFileName);
	prcssrApi->setFnameBg(sBgFileName);
	prcssrApi->setFnameData(sDataFileName);

	int result = 0;
	try
	{	
		prcssrApi->initDLL();
		result = prcssrApi->param();
		if (result != 0) { errorMsg(result); throw; }
		result = prcssrApi->data();
		if (result != 0) { errorMsg(result); throw; }
		result = prcssrApi->interp();
		if (result != 0) { errorMsg(result); throw; }
		result = prcssrApi->disp();
		if (result != 0) { errorMsg(result); throw; }
		result = prcssrApi->bg();
		if (result != 0) { errorMsg(result); throw; }
		result = prcssrApi->output();
		if (result != 0) { errorMsg(result); throw; }
		prcssrApi->destroy();
	}
	catch (exception e)
	{
		prcssrApi->destroy();
		delete prcssrApi;	// Not sure if this does anything
	}

	return 0;
}


void errorMsg(int returnValue)
{
	if  (returnValue == 1) {
		std::cout<<"Calibration File Error"<<std::endl;
		//MessageBox::Show("Calibration File Error");
	} else if (returnValue == 2) {
		std::cout<<"File Open Error (background or dispersion)"<<std::endl;
		//MessageBox::Show("File Open Error (background or dispersion)");
	} else if (returnValue == 3) {
		std::cout<<"Data File Error"<<std::endl;
		//MessageBox::Show("Data File Error");
	} else if (returnValue == 4) {
		std::cout<<"Gridsearch Error"<<std::endl;
		//int fakeCode = 0;//MessageBox::Show("Gridsearch Error"); // don't show an error.
	} else if (returnValue == 5) {
		std::cout<<"CUDA Error"<<std::endl;
		//MessageBox::Show("CUDA Error");
	} else if (returnValue == 6) {
		std::cout<<"CUFFT Error"<<std::endl;
		//MessageBox::Show("CUFFT Error");
	} else if (returnValue == 7) {
		std::cout<<"Unknown Error"<<std::endl;
		//MessageBox::Show("Unknown Error");
	} else if (returnValue == 9) {
		std::cout<<"ROI Error: Enter upper-left point followed by lower-right point."<<std::endl;
		//MessageBox::Show("ROI Error: Enter upper-left point followed by lower-right point.");
	} else  {
		std::cout<<"Really Unknown Error"<<std::endl;
		//MessageBox::Show("Really Unknown Error");
	}
}
}