#pragma once

#include <fstream>

#include "helper.cuh"

/* 
 * Data has 2 purposes: create the gaussian window array, and load the calibration, background, and data binary files into arrays.
 * 
 * To Do: Retest various file reading methods for speed.
 */

class Data : public Helper
{
public:
	Data(Parameters &p);
	~Data(void);
	
	float *d_gauss_win;			// Output of class.

	//first version is used for calibration file, second is for background and disperion, and last is for the data file.
	//these probably can be combined 
	void loadFile(std::string fname, int length, float *result_array);
	void loadFile(std::string fname, int startLocation, int length, float2 *result_array);
	void loadFile(std::string fname, int startLocation, int length, uint16_t *result_array);

private:
	void genGaussWin();
};