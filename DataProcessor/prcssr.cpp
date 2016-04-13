#pragma once

#include "prcssr.h"

using namespace std;

Prcssr::Prcssr	(	int batchFrames,		int batchAscans,
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
	Parameters	(	batchFrames,		batchAscans,
					prevFreq,			reslice,
					numAscansPerBscan,	numBScans,
					numCameraPixels,	numBgFrame,
					startPixel,			endPixel,
					alpha,				grayLevel,
					interpLinear,		dispModeGS, 
					dispFrame,			dispTests, 
					dispA2,				dispA3,
					fnamePhase, fnameBg, fnameData
				)
{
	p = NULL;
	data = NULL;
	interp = NULL;
	disp = NULL;
	bg = NULL;
	out = NULL;
}

Prcssr::~Prcssr()
{
	if (bg) { delete bg; bg = NULL;}
	if (disp) { delete disp; disp = NULL;}
	if (interp) { delete interp; interp = NULL;}
	if (data) { delete data; data = NULL;}
	if (p) { delete p; p = NULL;}
}

void Prcssr::runParam()
{
	cudaDeviceReset();

	p = new Parameters(	batchFrames,		batchAscans,
						prevFreq,			reslice,
						numAscansPerBscan,	numBScans,
						numCameraPixels,	numBgFrame,
						startPixel,			endPixel,
						alpha,				grayLevel,
						interpLinear,		dispModeGS, 
						dispFrame,			dispTests, 
						dispA2,				dispA3,
						fnamePhase, fnameBg, fnameData
					  );
}

void Prcssr::runData()
{
	data = new Data(*p);
}

void Prcssr::runInterp()
{
	interp = new Interpolation(*p, *data);
	interp->init();
}

void Prcssr::runDisp()
{
	disp = new Dispersion(*p, *data, *interp);
	disp->process();
}

void Prcssr::runDisp(int dispModeGS, float &a2, float &a3)
{
	disp = new Dispersion(*p, *data, *interp);
	disp->process(dispModeGS, a2, a3);
}

void Prcssr::runBg()
{
	bg = new Background(*p, *data, *interp, *disp); 
	bg->process();
}

void Prcssr::runOutput()
{
	out = new Output(*p, *data, *interp, *disp, *bg);
	out->process("bg");
	
	if (out) { delete out; out = NULL;}
}

void Prcssr::runOutput(string str, std::string datatype)
{
	p->fnameData = str;
	out = new Output(*p, *data, *interp, *disp, *bg);
	out->process(datatype);

	if (out) { delete out; out = NULL;}
}

void Prcssr::freeBg(string str)
{
	if (bg) { delete bg; bg = NULL;}
	p->fnameBg = str;
}