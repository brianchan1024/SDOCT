#pragma once
#include <string>

class ProcessorCalibration
{
	friend class ProcessorApi;

private:
	int m_batchFrames;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
	int m_batchAscans;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
	int m_prevFreq;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
	bool m_reslice;						// reslice into enface view								
	// Image settings. Same as MATLAB.

	int m_numAscansPerBscan;
	int m_numBScans;
	int m_numCameraPixels;
	int m_numBgFrame;

	int m_startPixel;
	int m_endPixel;
	float m_alpha;
	int m_grayLevel;
	bool m_interpLinear;

	bool m_dispModeGS;
	int m_dispFrame;
	int m_dispTests;
	float m_dispA2;
	float m_dispA3;

	std::string m_fnamePhase;
	std::string m_fnameBg;
	std::string m_fnameData;
	std::string m_fnameData2;

public:
	ProcessorCalibration(void) {};
	~ProcessorCalibration(void) {};
	void setProcessorCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice,
		int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
		int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear,
		bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
		std::string fnamePhase, std::string fnameBg, std::string fnameData)
	{
		m_batchFrames = batchFrames;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
		m_batchAscans = batchAscans;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
		m_prevFreq = prevFreq;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
		m_reslice = reslice;						// reslice into enface view								
		// Image settings. Same as MATLAB.

		m_numAscansPerBscan = numAscansPerBscan;
		m_numBScans = numBScans;
		m_numCameraPixels = numCameraPixels;
		m_numBgFrame = numBgFrame;

		m_startPixel = startPixel;
		m_endPixel = endPixel;
		m_alpha = alpha;
		m_grayLevel = grayLevel;
		m_interpLinear = interpLinear;

		m_dispModeGS = dispModeGS;
		m_dispFrame = dispFrame;
		m_dispTests = dispTests;
		m_dispA2 = dispA2;
		m_dispA3 = dispA3;

		m_fnamePhase = fnamePhase;
		m_fnameBg = fnameBg;
		m_fnameData = fnameData;

	}

	void resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3)
	{
		m_dispModeGS = dispModeGS;
		m_dispFrame = dispFrame;
		m_dispTests = dispTests;
		m_dispA2 = dispA2;
		m_dispA3 = dispA3;
	}
};

