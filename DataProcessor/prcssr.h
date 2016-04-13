#pragma once

#include "output.cuh"

class Prcssr : public Parameters
{
public:
	Prcssr		(	int batchFrames,		int batchAscans,
					int prevFreq,			bool reslice,
					int numAscansPerBscan,	int numBScans,
					int numCameraPixels,	int numBgFrame,
					int startPixel,			int endPixel,
					float alpha,			float grayLevel,
					bool interpLinear,		bool dispModeGS, 
					int dispFrame,			int dispTests, 
					float dispA2,			float dispA3,
					std::string fnamePhase, std::string fnameBg, std::string fnameData
				);

	~Prcssr(void);

	void runParam();
	void runData();
	void runInterp();
	void runDisp();
	void runDisp(int dispModeGS, float &a2, float &a3);

	void runBg();
	void runOutput();					// The version of output that takes a string runs when batching is used,
	void runOutput(std::string str, std::string datatype);	// str contains the path to the next data file.
	
	// When batching directories, we need to recalculate the background for each folder. To do this, the background
	// class needs to be freed and recreated. This function handles that, passing in the path to the new background file.	
	void freeBg(std::string str);

private:
	Parameters *p;
	Data *data;
	Interpolation *interp;
	Dispersion *disp;
	Background *bg;
	Output *out;	
};
