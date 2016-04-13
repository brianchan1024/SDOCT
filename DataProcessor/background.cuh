#pragma once

#include "dispersion.cuh"

/* 
 * Background calculates the DC level in the image.
 */

class Background : public Helper
{
public:
	Background(Parameters &p, Data &da, Interpolation &i, Dispersion &di);
	~Background(void);
	
	float *d_bg, *d_bg_noise;		// Output of class.
	float2 *d_bg_mask;				//

	void process();
	
private:
	Data &data;
	Interpolation &interp;
	Dispersion &disp;

	float bgColumnMeanMax;
	float2 *d_bg_frame;

	void genBg();
	void genBgMask();
	void genBgNoise();

	// This version of columnMean doesn't return a column mean max result. 
	void columnMean(int h, int w, float *my_array, float *result_array);

	// filter and filtfilt are based on http://mechatronics.ece.usu.edu/yqchen/filter.c/
	void filter(int ord, vector<float> &a, vector<float> &b, int w, vector<float> &my_vector, vector<float> &result_vector);
	void filtfilt(int ord, vector<float> &a, vector<float> &b, int w, vector<float> &my_vector, float *result_array);
};