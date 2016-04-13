#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "background.cuh"

/* 
 * Output class runs the actual data processing and handles writing to disk 
 */

class Output : public Helper
{
public:
	Output(Parameters &p, Data &da, Interpolation &i, Dispersion &di, Background &b);
	~Output(void);

	void process(std::string datatype);

private:
	Data &data;
	Interpolation &interp;
	Dispersion &disp;
	Background &bg;

	uint16_t *h_buff_1, *d_raw_data;											// Various buffer arrays
	float2 *d_proc_buff_0, *d_proc_buff_1, *d_proc_buff_2, *d_proc_buff_trm;	// used to process data.
	float *d_proc_buff_db, *d_proc_buff_trns;									//

	int height_ba, width_ba, width_2xba, frames_tot;							// Check documentation for explanation
																				// of different dimension and kernel
	dim3 dimLine_wba, dimLine_w2xba, dimLine_wtba;								// launch parameters
	cufftHandle	plan_w, plan_w2;												
	cudaStream_t stream1, stream2;												

	void initResources();
	void processData(int it, float *proc_data_piece);
	void writeToDisk(float *proc_data_array, std::string datatype);									// reslicing happens here.
	void freeResources();

	size_t mem_avail, mem_total; // used for memory debug
};