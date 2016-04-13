#include "dispersion.cuh"

using namespace std;

Dispersion::Dispersion(Parameters &p, Data &d, Interpolation &i) : Helper(p), data(d), interp(i)
{
	cout << "Dispersion" << endl;
	d_fphi = NULL;

	dispParam.resize(4);

	dimGrid_w = dim3((width - 1) / TILE_WIDTH + 1, (height_1fr - 1)/TILE_WIDTH + 1, 1);
	dimGrid_w2 = dim3((width_2x - 1) / TILE_WIDTH + 1, (height_1fr - 1)/TILE_WIDTH + 1, 1);
	dimGrid_wt = dim3((width_trm - 1) / TILE_WIDTH + 1, (height_1fr - 1)/TILE_WIDTH + 1, 1);

	// half A-scan length parameters for ROI selection.
	width_h = p.numCameraPixels / 2;
	dimGrid_wh = dim3((width_h - 1) / TILE_WIDTH + 1, (height_1fr - 1)/TILE_WIDTH + 1, 1);
}

Dispersion::~Dispersion()
{
	if (d_fphi) gpuErrchk( cudaFree(d_fphi) );
}

void Dispersion::process()
{
	gpuErrchk( cudaMalloc((void **)&d_fphi, (p.batchAscans*width_2x*sizeof(float2))) );

	if (p.dispModeGS)
	{
		getROI();
		gridsearch();
		previewComp();
	}
	else
		dispModeMan();
}

void Dispersion::process(int dispModeGS, float &a2, float &a3)
{
	gpuErrchk( cudaMalloc((void **)&d_fphi, (p.batchAscans*width_2x*sizeof(float2))) );

	if (dispModeGS)
	{
		getROI();
		gridsearch();
		previewComp();
		// return calculation result of dispersion params
		a2 = dispParam[0];
		a3 = dispParam[1];
	}
	else
	{
		dispModeMan();
	}

}

void Dispersion::dispModeMan()
{
	fphi = new float2[width_2x];
	vector<float> phi(width_2x, 0.0);
	
	dispParam[0] = p.dispA2;
	dispParam[1] = p.dispA3;
	dispParam[2] = dispParam[3] = 0;
	
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < width_2x; j++)
			phi[j] = phi[j] + dispParam[i] * powf((j+1.f), (i+2));
	
	double temp = 0;
	for (int i = 0; i < width_2x; i++)
	{
		temp = -phi[i] / width_2x;
		fphi[i].x = cos(temp);
		fphi[i].y = sin(temp);
	}
	gpuErrchk( cudaMemcpy(d_fphi, fphi, (width_2x * sizeof(float2)), cudaMemcpyHostToDevice) );

	repmat<<<dimLine_w2,dimLine_B>>>(p.batchAscans, width_2x, d_fphi); gpuErrchk( cudaPeekAtLastError() );
	delete[] fphi;
}

void Dispersion::getROI()
{
	float2 *initial_frame = new float2[height_1fr * width]();
	//data.loadFile(p.fnameData, 0, (height_1fr*width), initial_frame);
	data.loadFile(p.fnameBg, 0, (height_1fr*width), initial_frame);

	float2 *middle_frame = new float2[height_1fr * width]();
	data.loadFile(p.fnameData, (height_1fr * width_2x * (p.numBgFrame + p.dispFrame-1)), (height_1fr*width), middle_frame);

	cout << "	- Initialized dispesion parameters" << endl;
	cout << "file: " << p.fnameData << " height_1fr : " << height_1fr << " width: " << width << " width_2x: " << width_2x << " p.numBgFrame: " << p.numBgFrame << " p.dispFrame: " << p.dispFrame << endl;

	for(int i = 0; i < width_2x; i++)
	{
		cout << middle_frame[i].x << " ";
	}
	cout << endl;

	float *column_mean = new float[width];
	float column_mean_max = 0;
	columnMean(height_1fr, width, initial_frame, column_mean, column_mean_max);
	delete[] initial_frame;

	float *d_column_mean;
	float2 *d_middle_frame;
	float2 *d_IFT_data;
	gpuErrchk( cudaMalloc((void **)&d_column_mean, (width * sizeof(float))) );
	gpuErrchk( cudaMalloc((void **)&d_middle_frame, (height_1fr * width * sizeof(float2))) );
	gpuErrchk( cudaMalloc((void **)&d_IFT_data, (height_1fr * width * sizeof(float2))) );
	gpuErrchk( cudaMemcpy(d_middle_frame, middle_frame, (height_1fr *  width * sizeof(float2)), cudaMemcpyHostToDevice) );
	gpuErrchk( cudaMemcpy(d_column_mean, column_mean, (width * sizeof(float)), cudaMemcpyHostToDevice) );
	gpuErrchk( cudaMemset(d_IFT_data, 0, (height_1fr * width * sizeof(float2))) );
	delete[] middle_frame;
	delete[] column_mean;

	subtract<<<dimLine_w,dimLine_B>>>(height_1fr, width, d_middle_frame, d_column_mean, d_IFT_data); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_column_mean) );
	gpuErrchk( cudaFree(d_middle_frame) );

	float2 *d_fringe_zp;
	gpuErrchk( cudaMalloc((void **)&d_fringe_zp, (height_1fr * width_2x * sizeof(float2))) );
	gpuErrchk( cudaMemset(d_fringe_zp, 0, (height_1fr * width_2x * sizeof(float2))) );  //memset for zeropad

	FFT(height_1fr, width, d_IFT_data, d_IFT_data);

	zero_pad<<<dimGrid_w,dimGrid_B>>>(height_1fr, width, d_IFT_data, d_fringe_zp); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_IFT_data) );

	IFT(height_1fr, width_2x, dimGrid_w2, dimGrid_B, d_fringe_zp, d_fringe_zp);
	
	gpuErrchk( cudaMalloc((void **)&d_fringe_interp, (height_1fr * width_2x * sizeof(float2))) );
	gpuErrchk( cudaMemset(d_fringe_interp, 0, (height_1fr * width_2x * sizeof(float2))) );
	
	interp.procInterp(height_1fr, width_2x, dimLine_w2, dimLine_B, d_fringe_zp, d_fringe_interp);
	gpuErrchk( cudaFree(d_fringe_zp) );
	
	IFT(height_1fr, width_2x, dimGrid_w2, dimGrid_B, d_fringe_interp, d_fringe_interp);
		
	float2 *d_img_trim;
	gpuErrchk( cudaMalloc((void **)&d_img_trim, (height_1fr * width_h * sizeof(float2))) );
	trim_width<<<dimGrid_wh,dimGrid_B>>>(height_1fr, width_2x, 1, width_h, d_fringe_interp, d_img_trim); gpuErrchk( cudaPeekAtLastError() );

	float *d_img_mag;
	gpuErrchk( cudaMalloc((void **)&d_img_mag, (height_1fr * width_h * sizeof(float))) );
	magnitude_db<<<dimGrid_wh,dimGrid_B>>>(height_1fr, width_h, d_img_trim, d_img_mag); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_img_trim) );

	float *d_img_transpose;
	gpuErrchk( cudaMalloc((void **)&d_img_transpose, (width_h * height_1fr * sizeof(float))) );
	transpose<<<dimGrid_wh,dimGrid_B>>>(height_1fr, width_h, d_img_mag, d_img_transpose); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_img_mag) );

	float *TD = new float[width_h * height_1fr];
	gpuErrchk( cudaMemcpy(TD, d_img_transpose, (width_h * height_1fr * sizeof(float)), cudaMemcpyDeviceToHost) );
	gpuErrchk( cudaFree(d_img_transpose) );

	Image selectROI(height_1fr, width_h, TD);
	selectROI.getPts();

	ROIbounds.resize(4);
	ROIbounds[0] = selectROI.ROIpts[0].x+1;  
	ROIbounds[1] = selectROI.ROIpts[0].y+1;		// Add plus one to these since rest of code was written
	ROIbounds[2] = selectROI.ROIpts[1].x+1;		// with matlab test values, which are 1 indexed.
	ROIbounds[3] = selectROI.ROIpts[1].y+1;		// Doesn't matter too much though.

	//ROIbounds[0] = 101;		// This is for gridsearch testing, if it's necessary to compare
	//ROIbounds[1] = 101,		// with matlab the same values can be entered into matlab as
	//ROIbounds[2] = 284,		// well (might need +/- 1). In general though, using manual
	//ROIbounds[3] = 222;		// mode and writing in a2/a3 in MATLAB is easier.

	delete[] TD;

	width_ROI = ROIbounds[3] - ROIbounds[1] + 1; //122
	height_ROI = ROIbounds[2] - ROIbounds[0] + 1; //184

	if ((width_ROI < 0) || (height_ROI < 0))
	{
		throw std::invalid_argument("ROI Error");
	}

	dimGrid_wf = dim3((width_2x - 1)/TILE_WIDTH + 1, (height_ROI - 1)/TILE_WIDTH + 1, 1);
	dimGrid_wROI = dim3((width_ROI - 1)/TILE_WIDTH + 1, (height_ROI - 1)/TILE_WIDTH + 1, 1);

	gpuErrchk( cudaMalloc((void **)&d_fringe_frame, (height_ROI * width_2x * sizeof(float2))) );
	trim_height<<<dimGrid_w2,dimGrid_B>>>(height_1fr, width_2x, ROIbounds[0], ROIbounds[2], d_fringe_interp, d_fringe_frame); gpuErrchk( cudaPeekAtLastError() );

	//cudaFree(d_fringe_interp); // Keep these in memory for now
	//cudaFree(d_fringe_frame);  // as they are used later.
}

void Dispersion::gridsearch()
{
	w.resize(width_2x);
	for (int i = 0; i < width_2x; i++)
		w[i] = i - width + 1;

	FFT(height_ROI, width_2x, d_fringe_frame, d_fringe_frame);
	
	fphi = new float2[width_2x];

	// prep a fft plan for getParam call inside secondOrder() and thirdOrder().
	int w2ROIh[2] = {width_2x, height_ROI};
	cufftPlanMany(&plan_ROIw,1,w2ROIh,NULL,1,0,NULL,1,0,CUFFT_C2C,height_ROI);

	secondOrder();
	thirdOrder();
	cufftDestroy(plan_ROIw);
	
	genFphi();

	gpuErrchk( cudaFree(d_fringe_frame) );
	delete[] fphi;

	cout << "	- Calculated gridsearch parameters" << endl;
}

void Dispersion::secondOrder()
{
	float temp = 0;
	a2.resize(p.dispTests);
	vector<float> R_a2(p.dispTests,0);
	linSpace(-0.3f, 0.3f, p.dispTests, a2);

	for (int i = 0; i < p.dispTests; i++)
	{
		for (int j = 0; j < width_2x; j++)
		{
			temp = (-a2[i] * powf(w[j],2)) / width_2x;
			fphi[j].x = cos(temp);
			fphi[j].y = sin(temp);
		}
		gpuErrchk( cudaMemcpy(d_fphi, fphi, (width_2x * sizeof(float2)), cudaMemcpyHostToDevice) );
		getParam(i, R_a2); // d_fphi is used in here
	}

	// Get the index of the min value.
	int na2 = 0;
    for (int i = 1; i < p.dispTests; i++)
        if(R_a2[i] < R_a2[na2])
            na2 = i;              
	dispParam[0] = a2[na2]; 

	cout << "	- Calculated a2:			" << dispParam[0] << endl;
}

void Dispersion::thirdOrder()
{
	float temp = 0;
	vector<float> a3(p.dispTests);
	linSpace(-0.1f, 0.1f, p.dispTests, a3);
	for (int i = 0; i < p.dispTests; i++)
		a3[i] /= width_2x;
	vector<float> R_a3(p.dispTests, 0);

	for (int i = 0; i < p.dispTests; i++)
	{
		for (int j = 0; j < width_2x; j++)
		{
			//temp =  dispParam[0] *  w[j] * w[j] + (a3[i] * w[j] * w[j] * w[j]);
			temp = (dispParam[0] * powf(w[j],2)) + (a3[i] * powf(w[j],3));
			temp = -temp / width_2x;
			fphi[j].x = cos(temp);
			fphi[j].y = sin(temp);
		}
		gpuErrchk( cudaMemcpy(d_fphi, fphi, (width_2x * sizeof(float2)), cudaMemcpyHostToDevice) );
		getParam(i, R_a3); // d_fphi is used in here
	}

	// Get the index of the min value.
	int na3 = 0;
    for (int i = 1; i < p.dispTests; i++)
        if(R_a3[i] < R_a3[na3])
            na3 = i;              

	dispParam[1] = a3[na3];
	dispParam[2] = dispParam[3] = 0;
	
	cout << "	- Calculated a3:			" << dispParam[1] << endl;
}

void Dispersion::genFphi()
{
	vector<float> phi(width_2x, 0.0);
	
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < width_2x; j++)
			phi[j] = phi[j] + dispParam[i] * powf((j+1.f), (i+2));

	float temp = 0;
	for (int i = 0; i < width_2x; i++)
	{
		temp = -phi[i] / width_2x;
		fphi[i].x = cos(temp);
		fphi[i].y = sin(temp);
	}
	gpuErrchk( cudaMemcpy(d_fphi, fphi, (width_2x * sizeof(float2)), cudaMemcpyHostToDevice) );

	repmat<<<dimLine_w2,dimLine_B>>>(p.batchAscans, width_2x, d_fphi); gpuErrchk( cudaPeekAtLastError() );
}

void Dispersion::previewComp()
{
	
	FFT(height_1fr, width_2x, d_fringe_interp, d_fringe_interp);
	
	float2 *d_comp;
	gpuErrchk( cudaMalloc((void **)&d_comp, (height_1fr * width_2x * sizeof(float2))) );

	// matlab scales both real and imag compenets of fhi with the real component of S
	phi_multiply<<<dimLine_w2,dimLine_B>>>(height_1fr, width_2x, d_fphi, d_fringe_interp, d_comp); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_fringe_interp) );

	IFT(height_1fr, width_2x, dimGrid_w2, dimGrid_B, d_comp, d_comp);

	float2 *d_comp_trim;
	gpuErrchk( cudaMalloc((void **)&d_comp_trim, (height_1fr * width_h * sizeof(float2))) );
	trim_width<<<dimGrid_w2,dimGrid_B>>>(height_1fr, width_2x, 1, width_h, d_comp, d_comp_trim); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_comp) );

	float *d_comp_mag;
	gpuErrchk( cudaMalloc((void **)&d_comp_mag, (height_1fr * width_h * sizeof(float))) );
	magnitude_db<<<dimGrid_wh,dimGrid_B>>>(height_1fr, width_h, d_comp_trim, d_comp_mag); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_comp_trim) );

	float *d_comp_transpose;
	gpuErrchk( cudaMalloc((void **)&d_comp_transpose, (width_h * height_1fr * sizeof(float))) );
	transpose<<<dimGrid_wh,dimGrid_B>>>(height_1fr, width_h, d_comp_mag, d_comp_transpose); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_comp_mag) );

	float *TD_comp_preview = new float[width_h * height_1fr];
	gpuErrchk( cudaMemcpy(TD_comp_preview, d_comp_transpose, (width_h * height_1fr * sizeof(float)), cudaMemcpyDeviceToHost) );
	gpuErrchk( cudaFree(d_comp_transpose) );

	Image comp_image(height_1fr,width_h,TD_comp_preview);
	comp_image.dspl();

	delete[] TD_comp_preview;
	cout << "	- Generated compensated preview image" << endl;
}

void Dispersion::getParam(int it, vector<float> &result_vector)
{
	// matlab scales both real and imag compenets of fphi with the real component of S.
	float2 *d_S;
	gpuErrchk( cudaMalloc((void **)&d_S, (height_ROI * width_2x * sizeof(float2))) );
	phi_multiply<<<dimLine_w2,dimLine_B>>>(height_ROI, width_2x, d_fphi, d_fringe_frame, d_S); gpuErrchk( cudaPeekAtLastError() );

	cufftErrchk( cufftExecC2C(plan_ROIw, d_S, d_S, CUFFT_INVERSE) );
	scale_IFT<<<dimGrid_w2,dimGrid_B>>>(height_ROI, width_2x, w2Recip, d_S);

	float2 *d_S_ROI;
	gpuErrchk( cudaMalloc((void **)&d_S_ROI, (height_ROI * width_ROI * sizeof(float2))) );
	trim_width<<<dimGrid_wf,dimGrid_B>>>(height_ROI, width_2x, ROIbounds[1], ROIbounds[3], d_S, d_S_ROI); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_S) );

	float *d_S_abs;
	gpuErrchk( cudaMalloc((void **)&d_S_abs, (height_ROI * width_ROI * sizeof(float))) );
	magnitude<<<dimGrid_wROI,dimGrid_B>>>(height_ROI, width_ROI, d_S_ROI, d_S_abs); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_S_ROI) );	

	float s = 0;
	sumElements(height_ROI, width_ROI, d_S_abs, s);
	divide<<<dimGrid_wROI,dimGrid_B>>>(height_ROI, width_ROI, d_S_abs, s, d_S_abs); gpuErrchk( cudaPeekAtLastError() );

	float *d_S_log;
	gpuErrchk( cudaMalloc((void **)&d_S_log, (height_ROI * width_ROI * sizeof(float))) );
	d_log<<<dimGrid_wROI,dimGrid_B>>>(height_ROI, width_ROI, d_S_abs, d_S_log); gpuErrchk( cudaPeekAtLastError() );
	
	multiply<<<dimGrid_wROI,dimGrid_B>>>(height_ROI, width_ROI, d_S_abs, d_S_log, d_S_abs); gpuErrchk( cudaPeekAtLastError() );
	sumElements(height_ROI, width_ROI, d_S_abs, result_vector[it]);
	gpuErrchk( cudaFree(d_S_abs) );

	result_vector[it] = -result_vector[it];

	/*********************************************************************************************************************/
																											  /* Preview */
	float *d_S_transpose;
	gpuErrchk( cudaMalloc((void **)&d_S_transpose, (width_ROI * height_ROI * sizeof(float))) );
	transpose<<<dimGrid_wROI,dimGrid_B>>>(height_ROI, width_ROI, d_S_log, d_S_transpose); gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaFree(d_S_log) );

	float *compPrev = new float[width_ROI * height_ROI];
	gpuErrchk( cudaMemcpy(compPrev, d_S_transpose, (width_ROI * height_ROI * sizeof(float)), cudaMemcpyDeviceToHost) );
	gpuErrchk( cudaFree(d_S_transpose) );

	Image dispCompImg(height_ROI, width_ROI, compPrev);
	dispCompImg.dsplGS();
	
	delete[] compPrev;
}

void Dispersion::linSpace(float min, float max, int pts, vector<float> &result_vector)
{
	float space = (max - min) / (pts - 1);
	result_vector[0] = min;
	
	for (int i = 1; i < pts; i++)
		result_vector[i] = result_vector[i - 1] + space;
}

void Dispersion::sumElements(int height, int width, float *initial_array, float &sum)
{	
		float *d_result_array;
		gpuErrchk( cudaMalloc((void **)&d_result_array, (height * width * sizeof(float))) );
		gpuErrchk( cudaMemcpy(d_result_array, initial_array, (height * width * sizeof(float)), cudaMemcpyDeviceToDevice) );	
	
		float *d_temp_sum;
		gpuErrchk( cudaMalloc((void **)&d_temp_sum, (height * width * sizeof(float))) );

		int sharedMemSize = TILE_WIDTH * TILE_WIDTH * sizeof(float);
		float sumHeight = height; 
		float sumWidth = width;
		dim3 dimGrid((width - 1)/TILE_WIDTH + 1, (height - 1)/TILE_WIDTH + 1, 1);
		dim3 dimBlock(TILE_WIDTH, TILE_WIDTH, 1);
		int k = (height * width);

		while (k != 0)
		{
    		d_sum_elements<<<dimGrid, dimBlock, sharedMemSize>>>(sumHeight, sumWidth, d_result_array, d_temp_sum); gpuErrchk( cudaPeekAtLastError() );
    		gpuErrchk( cudaMemcpy(d_result_array, d_temp_sum, (sumHeight * sumWidth * sizeof(float)), cudaMemcpyDeviceToDevice) );
   
    		sumHeight = ceil(sumHeight/TILE_WIDTH);
    		sumWidth = ceil(sumWidth/TILE_WIDTH);;
    		dimGrid.x = (sumWidth - 1) / TILE_WIDTH + 1; 
    		dimGrid.y = (sumHeight - 1) / TILE_WIDTH + 1;
    		k /= TILE_WIDTH*TILE_WIDTH;
		}

		gpuErrchk( cudaMemcpy(&sum, &d_result_array[0], sizeof(float), cudaMemcpyDeviceToHost) );

		gpuErrchk( cudaFree(d_temp_sum) );
		gpuErrchk( cudaFree(d_result_array) );
}

vector<float> Dispersion::getDispersionCalResult()
{
	return dispParam;
}