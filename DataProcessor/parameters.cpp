#include "parameters.h"

using namespace std;

// Debug or Test Case Constructor
Parameters::Parameters()
{	
	cout << "Initialization" << endl;

	int selection = 0;					// select which test case to run. 1 is 2048 pixels, 2 is 1024 pixels.

	if (selection == 1)
	{	
		batchFrames = 1;
		batchAscans = 10;
		prevFreq = 40;
		reslice = false;

		numAscansPerBscan = batchFrames*800;
		numBScans = 800/batchFrames;
		numCameraPixels = 1024;
		numBgFrame = 3;
		startPixel = 15;
		endPixel = 480;
		alpha = 0.5;
		grayLevel = 30;
		interpLinear = true;

		dispModeGS = true;
		dispFrame = 100;
		dispTests = 100;
		dispA2 = 0.00909076;
		dispA3 = 2.44613e-7;

		fnamePhase = "E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt";
		fnameBg = "E:\\Data\\2015-05-29_SHR_e11_5_mouse_embryo_heart\\SHR_e11_5_mouse_embryo_OD_U-3D_ 9x 9_R00.bin";
		fnameData = "E:\\Data\\2015-05-29_SHR_e11_5_mouse_embryo_heart\\SHR_e11_5_mouse_embryo_OD_U-3D_ 9x 9_R01.bin";
	}
	else if (selection == 2)
	{
		batchFrames = 4;
		batchAscans = 10;
		prevFreq = 10;
		reslice = false;

		numAscansPerBscan = batchFrames*800;
		numBScans = 800/batchFrames;
		numCameraPixels = 1024;
		numBgFrame = 2;
		startPixel = 6;
		endPixel = 480;
		alpha = 0.5;
		grayLevel = 25;
		interpLinear = true;

		dispModeGS = false;
		dispFrame = 400;
		dispTests = 100;
		dispA2 = 0; //0.00909076;
		dispA3 = 0; //2.44613e-7;
		
		fnamePhase = "C:\\Users\\andre\\Desktop\\OCT_dataset\\1024_calibration_file.txt";
		fnameBg = "C:\\Users\\andre\\Desktop\\OCT_dataset\\2015-03-18_SHR_mouse_embryo_heart_3disoc_4.5h\\SHR_mouse_embryo_heart_3disoc_4.5h_OD_U-3D_16x16_R00.bin";
		fnameData = "C:\\Users\\andre\\Desktop\\OCT_dataset\\2015-03-18_SHR_mouse_embryo_heart_3disoc_4.5h\\SHR_mouse_embryo_heart_3disoc_4.5h_OD_U-3D_16x16_R03.bin";
	}
	else if (selection == 3)
	{
		batchFrames = 10;
		batchAscans = 2;
		prevFreq = 2;
		reslice = false;

		numAscansPerBscan = batchFrames*800;
		numBScans = 800/batchFrames;
		numCameraPixels = 1024;
		numBgFrame = 2;
		startPixel = 10;
		endPixel = 480;
		alpha = 0.5;
		grayLevel = 25;
		interpLinear = true;

		dispModeGS = false;
		dispFrame = 400;
		dispTests = 100;
		dispA2 = 0.00909076; //0.00909076;
		dispA3 = 2.44613e-7; //2.44613e-7;

		fnamePhase = "C:\\Users\\andre\\Desktop\\OCT_dataset\\1024_calibration_file.txt";
		fnameBg = "C:\\Users\\andre\\Desktop\\OCT_dataset\\beef\\SHR_beef_3disco_OD_U-3D_33x33_R00.bin";
		fnameData = "C:\\Users\\andre\\Desktop\\OCT_dataset\\beef\\SHR_beef_3disco_OD_U-3D_33x33_R03.bin";
	}
	cout << "	- Settings loaded" << endl;
}

// Constructor used by the wrapper.
Parameters::Parameters(	int batchFrames,		int batchAscans,
						int prevFreq,			bool reslice,
						int numAscansPerBscan,	int numBScans,
						int numCameraPixels,	int numBgFrame,
						int startPixel,			int endPixel,
						float alpha,			float grayLevel,
						bool interpLinear,		bool dispModeGS, 
						int dispFrame,			int dispTests, 
						float dispA2,			float dispA3,
						string fnamePhase, string fnameBg, string fnameData
					  ) :
						batchFrames(batchFrames),				batchAscans(batchAscans),
						prevFreq(prevFreq),						reslice(reslice),
						numAscansPerBscan(numAscansPerBscan),	numBScans(numBScans),
						numCameraPixels(numCameraPixels),		numBgFrame(numBgFrame),
						startPixel(startPixel),					endPixel(endPixel),
						alpha(alpha),							grayLevel(grayLevel),
						interpLinear(interpLinear),				dispModeGS(dispModeGS), 
						dispFrame(dispFrame),					dispTests(dispTests), 
						dispA2(dispA2),							dispA3(dispA3),
						fnamePhase(fnamePhase), fnameBg(fnameBg), fnameData(fnameData)						
{
}

Parameters::~Parameters()
{	
}