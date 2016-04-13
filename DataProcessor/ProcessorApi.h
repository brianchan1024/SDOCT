#pragma once



#include "ProcessorCalibration.h"
#include "ProcessorWrapper.h"

class ProcessorApi
{
private:
	ProcessorCalibration* prcsrCali;
	PrcssrAccess* octPrcssr;
	void setPrames();

public:
	ProcessorApi(void);
	~ProcessorApi(void);
	void setProcessorCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice,
		int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
		int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear,
		bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
		std::string fnamePhase, std::string fnameBg, std::string fnameData);

	void resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3);

	void processBg();
	void processBg(int dispModeGS, float &a2, float &a3);
	void processFrameData(std::string fnameData2, std::string datatype);
};

