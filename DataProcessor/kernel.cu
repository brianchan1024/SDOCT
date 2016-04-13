#pragma once

//#include <stdio.h>

#include "parameters.h"
#include "data.cuh"
#include "interpolation.cuh"
#include "dispersion.cuh"
#include "background.cuh"
#include "output.cuh"

//using namespace std;

// add file progress output
// test height for each class.


int main()
{
	Parameters *p = new Parameters();
	Data *data = new Data(*p);
	Interpolation *interp = new Interpolation(*p, *data);
	interp->init();

	Dispersion *disp = new Dispersion(*p, *data, *interp);
	disp->process();

	Background *bg = new Background(*p, *data, *interp, *disp);
	bg->process();

	Output *out = new Output(*p, *data, *interp, *disp, *bg);
	out->process("test");

	delete out;
	delete bg;
	delete disp;
	delete interp;
	delete data;
	delete p;

	cudaDeviceReset();
	return 0;
}