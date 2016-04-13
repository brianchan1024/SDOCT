#include "ProcessorWrapper.h"
#include <string>
#include "DP_logger.h"

using namespace std;

void PrcssrAccess::initDLL()
{
	/*string fnamePhase_m = sysStrToStdStr(fnamePhase);
	string fnameBg_m = sysStrToStdStr(fnameBg);
	string fnameData_m = sysStrToStdStr(fnameData);*/
	string fnamePhase_m = fnamePhase;
	string fnameBg_m = fnameBg;
	string fnameData_m = fnameData;

	myPrcssr = new Prcssr(	batchFrames,		batchAscans,
							prevFreq,			reslice,
							numAscansPerBscan*batchFrames,	
							numBScans/batchFrames,
							numCameraPixels,	numBgFrame,
							startPixel,			endPixel,
							alpha,				grayLevel,
							interpLinear,		dispModeGS, 
							dispFrame,			dispTests, 
							dispA2,				dispA3,
							fnamePhase_m, fnameBg_m, fnameData_m
							);


	returnValue = 0;
}

int PrcssrAccess::param()
{
	try { myPrcssr->runParam(); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue); }
	return returnValue; 
}

int PrcssrAccess::data()
{
	try { myPrcssr->runData(); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue); }
	return returnValue;
}

int PrcssrAccess::interp()
{
	try { myPrcssr->runInterp(); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue); }
	return returnValue;
}

int PrcssrAccess::disp()
{
	try { myPrcssr->runDisp(); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue); }
	return returnValue;
}

int PrcssrAccess::disp(int dispModeGS, float &a2, float &a3)
{
	try { myPrcssr->runDisp(dispModeGS, a2, a3); }
	catch (std::exception &ex) 
	{ runErrorCheck(ex, returnValue); }
	return returnValue;
}

int PrcssrAccess::bg()
{
	try { myPrcssr->runBg(); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue); }
	return returnValue;
}

int PrcssrAccess::output()
{
	try { myPrcssr->runOutput(); }
	catch (std::exception &ex) 
	{
			DP_LOGGER_ERROR << __FUNCTION__ <<" output Error";
			runErrorCheck(ex, returnValue); 
	}
	return returnValue;
}

int PrcssrAccess::output(std::string str, std::string datatype)
{
	string cppStr = sysStrToStdStr(str);
	try { myPrcssr->runOutput(cppStr, datatype); }				
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue);}
	return returnValue;
}

// Errors if this conversion is done directly. Some sort of bug in vs2010, workaround with method.
// Not a bug anymore? Not sure, can't find the stack overflow post. I'm leaving it wrapped in the function.
			
string PrcssrAccess::sysStrToStdStr(std::string str)
{
	//string cppString = msclr::interop::marshal_as<std::string>(str);
	string cppString = str;
	return cppString;
}
			
// When batching directories, we need to recalculate the background for each folder. To do this, the background
//  class needs to be freed and recreated. This function handles that, passing in the path to the new background file.
int PrcssrAccess::freeBg(std::string str)
{
	string cppStr = sysStrToStdStr(str);
	try { myPrcssr->freeBg(cppStr); }
	catch (std::exception &ex) 
		{ runErrorCheck(ex, returnValue);}
	return returnValue;
}

void PrcssrAccess::destroy()
{
	returnValue = 0;
	delete myPrcssr;
}

// Turn the error string into an error code which the GUI understands.
void PrcssrAccess::runErrorCheck(std::exception e, int &rv)
{
	std::cout<<"Debug: "<<__FUNCTION__<<" "<<rv<<std::endl;
	if  (e.what() == std::string("Calibration File Error"))
	{
		rv = 1;
		DP_LOGGER_ERROR << __FUNCTION__ <<" Calibration File Error";
	}
	else if (e.what() == std::string("File Open Error"))
	{
		rv = 2;
		DP_LOGGER_ERROR << __FUNCTION__ <<" File Open Error";
	}
	else if (e.what() == std::string("; File Error"))
	{
		rv = 3;
		DP_LOGGER_ERROR << __FUNCTION__ <<" File Error";
	}
	else if (e.what() == std::string("Gridsearch Error"))
	{
		rv = 4;
		DP_LOGGER_ERROR << __FUNCTION__ <<" Gridsearch Error";
	}
	else if (e.what() == std::string("CUDA Error"))
	{
		rv = 5;
		DP_LOGGER_ERROR << __FUNCTION__ <<" CUDA Error";
	}
	else if (e.what() == std::string("CUFFT Error"))
	{
		rv = 6;
		DP_LOGGER_ERROR << __FUNCTION__ <<" CUFFT Error";
	}
	else if (e.what() == std::string("ROI Error"))
	{
		rv = 9;
		DP_LOGGER_ERROR << __FUNCTION__ <<" ROI Error";
	}
	else
	{
		rv = 7;
		DP_LOGGER_ERROR << __FUNCTION__ <<" Unknown Error";
	}
}

// getters and setters.
void PrcssrAccess::setBatchFrames(int batchFrames){this->batchFrames = batchFrames;}
void PrcssrAccess::setBatchAscans(int batchAscans){this->batchAscans = batchAscans;}
void PrcssrAccess::setPrevFreq(int prevFreq){this->prevFreq = prevFreq;}
void PrcssrAccess::setReslice(bool reslice){this->reslice = reslice;}

void PrcssrAccess::setNumAscansPerBscan(int numAscansPerBscan){this->numAscansPerBscan = numAscansPerBscan;}
void PrcssrAccess::setNumBScans(int numBScans){this->numBScans = numBScans;}
void PrcssrAccess::setNumCameraPixels(int numCameraPixels){this->numCameraPixels = numCameraPixels;}
void PrcssrAccess::setNumBgFrame(int numBgFrame){this->numBgFrame = numBgFrame;}
void PrcssrAccess::setStartPixel(int startPixel){this->startPixel = startPixel;}
void PrcssrAccess::setEndPixel(int endPixel){this->endPixel = endPixel;}
void PrcssrAccess::setAlpha(float alpha){this->alpha = alpha;}
void PrcssrAccess::setGrayLevel(int grayLevel){this->grayLevel = grayLevel;}
void PrcssrAccess::setInterpLinear(bool interpLinear){this->interpLinear = interpLinear;}

void PrcssrAccess::setDispModeGS(bool dispModeGS){this->dispModeGS = dispModeGS;}
void PrcssrAccess::setDispFrame(int dispFrame){this->dispFrame = dispFrame;}
void PrcssrAccess::setDispTests(int dispTests){this->dispTests = dispTests;}
void PrcssrAccess::setDispA2(float dispA2){this->dispA2 = dispA2;}
void PrcssrAccess::setDispA3(float dispA3){this->dispA3 = dispA3;}

void PrcssrAccess::setFnamePhase(std::string fnamePhase){this->fnamePhase = fnamePhase;}
void PrcssrAccess::setFnameBg(std::string fnameBg){this->fnameBg = fnameBg;}
void PrcssrAccess::setFnameData(std::string fnameData){this->fnameData = fnameData;}

int PrcssrAccess::getBatchFrames(){return batchFrames;}
int PrcssrAccess::getBatchAscans(){return batchAscans;}
int PrcssrAccess::getPrevFreq(){return prevFreq;}
bool PrcssrAccess::getReslice(){return reslice;}

int PrcssrAccess::getNumAscansPerBscan(){return numAscansPerBscan;}
int PrcssrAccess::getNumBScans(){return numBScans;}
int PrcssrAccess::getNumCameraPixels(){return numCameraPixels;}
int PrcssrAccess::getNumBgFrame(){return numBgFrame;}
int PrcssrAccess::getStartPixel(){return startPixel;}
int PrcssrAccess::getEndPixel(){return endPixel;}
float PrcssrAccess::getAlpha(){return alpha;}
int PrcssrAccess::getGrayLevel(){return grayLevel;}
bool PrcssrAccess::getInterpLinear(){return interpLinear;}

bool PrcssrAccess::getDispModeGS(){return dispModeGS;}
int PrcssrAccess::getDispFrame(){return dispFrame;}
int PrcssrAccess::getDispTests(){return dispTests;}
float PrcssrAccess::getDispA2(){return dispA2;}
float PrcssrAccess::getDispA3(){return dispA3;}

std::string PrcssrAccess::getFnamePhase(){return fnamePhase;}
std::string PrcssrAccess::getFnameBg(){return fnameBg;}
std::string PrcssrAccess::getFnameData(){return fnameData;}
