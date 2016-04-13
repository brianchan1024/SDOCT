#pragma once

#include <iostream>
#include <fstream>
/* 
 * 
 * To Do: Make variables private and use getters and setters
 */

class Parameters
{
public:
									// Default constructor is for debug and test cases
	Parameters();

									// This constructor takes input from the GUI through the wrapper DLL.
	Parameters	(	int batchFrames,		int batchAscans,
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

	~Parameters(void);
									// Speed Optimization. These go first since batchFrames needs to be initialized before ascan and bscan values.
	int batchFrames;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
	int batchAscans;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
	int prevFreq;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
	bool reslice;						// reslice into enface view								
									// Image settings. Same as MATLAB.
	int numAscansPerBscan;
	int numBScans;
	int numCameraPixels;
	int numBgFrame;
	int startPixel;
	int endPixel;
	float alpha;
	int grayLevel;
	bool interpLinear;					// interpolation mode - linear or cubic spline
									// Dispersion Settings
	bool dispModeGS;					// Gridsearch Mode or Manual Entry Mode
	int dispFrame;						// Frame number to use to calculate A2 & A3
	int dispTests;						// Number of tests to use for gridsearch
	float dispA2;
	float dispA3;
									// Other & File paths
	std::string fnamePhase;
	std::string fnameBg;
	std::string fnameData;
};