#include "output.cuh"
//#include "cuda_profiler_api.h"

using namespace std;
using namespace cv;

Output::Output(Parameters &p, Data &da, Interpolation &i, Dispersion &di, Background &b) : Helper(p), data(da), interp(i), disp(di), bg(b)
{
	cout << "****************\nData processing" << endl;
	
	height_ba = p.numAscansPerBscan/p.batchAscans;
	width_ba = p.numCameraPixels*p.batchAscans;
	width_2xba = (p.numCameraPixels*2)*p.batchAscans;
	frames_tot = p.numBScans*p.batchFrames;
	//std::cout<<"Debug: "<<__FUNCTION__<<" frames_tot:"<<frames_tot<<" p.numBScans: "<<p.numBScans<<" p.batchFrames: "<<p.batchFrames<<std::endl;
	dimGrid_w = dim3((width - 1) / TILE_WIDTH + 1, (height_bfr - 1)/TILE_WIDTH + 1, 1);
	dimGrid_w2 = dim3((width_2x - 1) / TILE_WIDTH + 1, (height_bfr - 1)/TILE_WIDTH + 1, 1);
	dimGrid_wt = dim3((width_trm - 1) / TILE_WIDTH + 1, (height_bfr - 1)/TILE_WIDTH + 1, 1);

	dimLine_wba	= dim3((width_ba+THREADS_PER_BLOCK-1)/THREADS_PER_BLOCK, 1, 1);
	dimLine_w2xba = dim3((width_2xba+THREADS_PER_BLOCK-1)/THREADS_PER_BLOCK, 1, 1);
	dimLine_wtba = dim3(((width_trm*p.batchAscans)+THREADS_PER_BLOCK-1)/THREADS_PER_BLOCK, 1, 1);
}

Output::~Output()
{
}

void Output::process(std::string datatype)
{
	cudaMemGetInfo(&mem_avail, &mem_total);
	std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	cv::Mat prevImg;

	// Hold the full processed image stack in memory.
	float *processed_data_array = new float[width_trm * height_bfr * frames];
	initResources();
	//std::cout<<"Debug: "<<__FUNCTION__<<" process 1"<<std::endl;
	//gpuErrchk( cudaProfilerStart() );
	for (int i = 0; i < frames; i++)
	{	
		processData(i, processed_data_array);
	
		/*
		// do not need to show the processed image, since we have a preview UI @brian
		if (i%p.prevFreq == 0)
		{
			prevImg = cv::Mat(width_trm, height_1fr, CV_32F, &processed_data_array[i * width_trm * height_bfr]);
			cv::namedWindow("Preview", cv::WINDOW_AUTOSIZE);
			cv::imshow("Preview", prevImg);
			cv::waitKey(1);
		}
		*/
	}
	//std::cout<<"Debug: "<<__FUNCTION__<<" process 2"<<std::endl;
	//destroyWindow("Preview"); @brian
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"1 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	freeResources();
	//gpuErrchk( cudaProfilerStop() );

	writeToDisk(processed_data_array, datatype);

	delete[] processed_data_array;
	prevImg.release(); 
	//std::cout<<"Debug: "<<__FUNCTION__<<" process 2"<<std::endl;
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"2 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
}

void Output::initResources()
{
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
	//// debug
	//output_csv(p.batchAscans, width_2x, data.d_gauss_win, "gaussWin.csv");
	//output_csv(p.batchAscans,width_2x,interp.d_query_points,"pixel_nonuniform.csv");  // make this private
	//output_csv(p.batchAscans,width_2x,disp.d_fphi,"fphi_imag.csv");
	//output_csv(p.batchAscans,width,bg.d_bg,"bg.csv");
	//output_csv(p.batchAscans,width_2x,bg.d_bg_mask,"bg_mask.csv");
	//output_csv(p.batchAscans,width_trm,bg.d_bg_noise,"bg_noise.csv");
	//if(it==3) output_csv((width_trm*4),(height_bfr/4),d_proc_buff_trns,"4frames.csv");	// scaling might be wrong.
	//if(it==12) output_csv(width_trm,height_bfr,d_proc_buff_trns,"1frames.csv");			// these two need to be moved to process loop if used.

	h_buff_1 = new uint16_t[height_bfr * width]();

	gpuErrchk( cudaHostRegister(h_buff_1, (height_bfr*width*sizeof(uint16_t)), cudaHostRegisterPortable));
	gpuErrchk( cudaPeekAtLastError() );
	gpuErrchk( cudaStreamCreate(&stream1) );
	gpuErrchk( cudaStreamCreate(&stream2) );

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"1 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	int m[2] = {width, height_bfr};
	// mem leak here
	cufftErrchk( cufftPlanMany(&plan_w,1,m,NULL,1,0,NULL,1,0,CUFFT_C2C,height_bfr) );

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"2 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	// mem leak here
	cufftErrchk( cufftSetStream(plan_w,stream1) );

	int n[2] = {width_2x, height_bfr};
	cufftErrchk( cufftPlanMany(&plan_w2,1,n,NULL,1,0,NULL,1,0,CUFFT_C2C,height_bfr) );

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"3 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
	
	cufftErrchk( cufftSetStream(plan_w2,stream1) );
	
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"4 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	// error here betwwn nezt debug info
	gpuErrchk( cudaMalloc((void **)&d_raw_data, (height_bfr * width * sizeof(uint16_t))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"3 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_0, (height_bfr * width * sizeof(float2))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"4 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_1, (height_bfr * width_2x * sizeof(float2))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"5 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	// error occurs here
	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"6 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_2, (height_bfr * width_2x * sizeof(float2))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"7 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_trm, (height_bfr * width_trm * sizeof(float2))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"8 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_db, (height_bfr * width_trm * sizeof(float))) );

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"8.5 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMalloc((void **)&d_proc_buff_trns, (width_trm * height_bfr * sizeof(float))) );
	
	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"9 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	data.loadFile(p.fnameData, (height_1fr * width_2x * p.numBgFrame), (height_bfr*width), h_buff_1);

	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"10 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMemcpy(d_raw_data, h_buff_1, (height_bfr * width * sizeof(uint16_t)), cudaMemcpyHostToDevice) );
	
	//cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"11 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
}

void Output::processData(int it, float *proc_data_piece)
{
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaMemset(d_proc_buff_0, 0, (height_bfr * width * sizeof(float2))) );
	subtract<<<dimLine_wba,dimLine_B,0,stream1>>>(height_ba, width_ba, d_raw_data, bg.d_bg, d_proc_buff_0); gpuErrchk( cudaPeekAtLastError() );
	
	cufftErrchk( cufftExecC2C(plan_w, d_proc_buff_0, d_proc_buff_0, CUFFT_FORWARD) );
	gpuErrchk( cudaMemset(d_proc_buff_1, 0, (height_bfr * width_2x * sizeof(float2))) );
	
	zero_pad<<<dimGrid_w,dimGrid_B,0,stream1>>>(height_bfr, width, d_proc_buff_0, d_proc_buff_1); gpuErrchk( cudaPeekAtLastError() );

	cufftErrchk( cufftExecC2C(plan_w2, d_proc_buff_1, d_proc_buff_1, CUFFT_INVERSE) );
	scale_IFT_x<<<dimGrid_w2,dimGrid_B,0,stream1>>>(height_bfr, width_2x, w2Recip, d_proc_buff_1); gpuErrchk( cudaPeekAtLastError() );

	interp.procInterp(height_ba, width_2xba, dimLine_w2xba, dimLine_B, stream1, d_proc_buff_1, d_proc_buff_2);

	data.loadFile(p.fnameData, ((height_1fr * width_2x) * (p.numBgFrame+((it+1)*p.batchFrames))), (height_bfr*width), h_buff_1);
	
	//std::cout<<"Debug: "<<__FUNCTION__<<" processData 1"<<std::endl;

	mult_divide<<<dimLine_w2xba,dimLine_B,0,stream1>>>(height_ba, width_2xba, data.d_gauss_win, d_proc_buff_2, bg.d_bg_mask, d_proc_buff_2); gpuErrchk( cudaPeekAtLastError() );
	phi_multiply<<<dimLine_w2xba,dimLine_B,0,stream1>>>(height_ba, width_2xba, disp.d_fphi, d_proc_buff_2, d_proc_buff_1); gpuErrchk( cudaPeekAtLastError() );

	gpuErrchk( cudaMemcpyAsync(d_raw_data, h_buff_1, (height_bfr * width * sizeof(uint16_t)), cudaMemcpyHostToDevice, stream2) );

	cufftErrchk( cufftExecC2C(plan_w2,d_proc_buff_1, d_proc_buff_1, CUFFT_INVERSE) );
	scale_IFT<<<dimGrid_w2,dimGrid_B,0,stream1>>>(height_bfr, width_2x, w2Recip, d_proc_buff_1); gpuErrchk( cudaPeekAtLastError() );

	trim_width<<<dimGrid_wt,dimGrid_B,0,stream1>>>(height_bfr, width_2x, p.startPixel, p.endPixel, d_proc_buff_1, d_proc_buff_trm); gpuErrchk( cudaPeekAtLastError() );
	magnitude_db<<<dimGrid_wt,dimGrid_B,0,stream1>>>(height_bfr, width_trm, d_proc_buff_trm, d_proc_buff_db); gpuErrchk( cudaPeekAtLastError() );
	subt_divide<<<dimLine_wtba,dimLine_B,0,stream1>>>(height_ba, (width_trm*p.batchAscans), d_proc_buff_db, bg.d_bg_noise, grayRecip, d_proc_buff_db); gpuErrchk( cudaPeekAtLastError() );
	multiframe_transpose<<<dimGrid_wt,dimGrid_B,0,stream1>>>(height_1fr, width_trm, p.batchFrames, d_proc_buff_db, d_proc_buff_trns); gpuErrchk( cudaPeekAtLastError() );
	
	int write_offset = (it*height_bfr*width_trm);
	gpuErrchk( cudaMemcpy((proc_data_piece+write_offset), d_proc_buff_trns, (width_trm * height_bfr * sizeof(float)), cudaMemcpyDeviceToHost) );

	gpuErrchk( cudaDeviceSynchronize() );
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

}

void Output::writeToDisk(float *proc_data_array, std::string datatype)
{
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	Mat image;
	Mat new_image;
	char fname_opencv[200];
	string path_string; 

	Mat row_mean;
	Mat avg_img;
	
	if (!p.reslice)
	{
		std::cout<<"Debug: "<<__FUNCTION__<<"frames_tot:"<<frames_tot<<std::endl;

		unsigned slash_split = p.fnameData.find_last_of("/\\");
		string path_name = p.fnameData.substr(0,slash_split);
		string f_name = p.fnameData.substr(slash_split+1);
		unsigned period_split = f_name.find_last_of("."); 
		string r_name =  f_name.substr(0, period_split);
		//path_string = (path_name+"\\"+r_name);
		path_string = (path_name+"\\GPU_processed");
		CreateDirectory(path_string.c_str(),NULL);
		path_string = (path_string + "\\" + r_name);
		CreateDirectory(path_string.c_str(),NULL);

		if (datatype == "fundus")
		{	
			sprintf(fname_opencv,"%s\\0.png",path_string.c_str());
			// fundus view result in one image
			for (int i = 0; i < frames_tot; i++)
			{
				image = cv::Mat(width_trm, height_1fr, CV_32F, &proc_data_array[i*width_trm*height_1fr]);
				new_image = cv::Mat(width_trm, height_1fr, CV_8U);
				image.convertTo(new_image,CV_8U,255);

				reduce(new_image,row_mean, 0, CV_REDUCE_AVG);
				avg_img.push_back(row_mean);
			}
			imwrite(fname_opencv, avg_img);
		}
		else {
			// process the other data type
			for (int i = 0; i < frames_tot; i++)
			{
				// This conversion makes the images look like they do in MATLAB. Doing normalization, like in the
				// image class, results on low contrast images. This needs to be confirmed, but I believe all values below 0 are set to 0.
				image = cv::Mat(width_trm, height_1fr, CV_32F, &proc_data_array[i*width_trm*height_1fr]);
				new_image = cv::Mat(width_trm, height_1fr, CV_8U);
				image.convertTo(new_image,CV_8U,255);
				//new_image = cv::Mat(width_trm, height_1fr, CV_16U);
				//image.convertTo(new_image,CV_16U,65535);	

				reduce(new_image,row_mean, 0, CV_REDUCE_AVG);
				avg_img.push_back(row_mean);
				// Try to create the directory on the first iteration.
				//std::cout<<"Debug: "<<__FUNCTION__<<"path_string:"<<path_string<<std::endl;
				//sprintf(fname_opencv,"%s\\%i.tiff",path_string.c_str(),i);
				sprintf(fname_opencv,"%s\\%i.png",path_string.c_str(),i);
				imwrite(fname_opencv, new_image);
			}
		}
		/*
		for (int i = 0; i < frames_tot; i++)
		{

			// This conversion makes the images look like they do in MATLAB. Doing normalization, like in the
			// image class, results on low contrast images. This needs to be confirmed, but I believe all values below
			// 0 are set to 0.
			image = cv::Mat(width_trm, height_1fr, CV_32F, &proc_data_array[i*width_trm*height_1fr]);
			new_image = cv::Mat(width_trm, height_1fr, CV_8U);
			image.convertTo(new_image,CV_8U,255);
			//new_image = cv::Mat(width_trm, height_1fr, CV_16U);
			//image.convertTo(new_image,CV_16U,65535);	

			reduce(new_image,row_mean, 0, CV_REDUCE_AVG);
			avg_img.push_back(row_mean);
			// Try to create the directory on the first iteration.
			if (i == 0) 
			{
				unsigned slash_split = p.fnameData.find_last_of("/\\");
				string path_name = p.fnameData.substr(0,slash_split);
				string f_name = p.fnameData.substr(slash_split+1);
				unsigned period_split = f_name.find_last_of("."); 
				string r_name =  f_name.substr(0, period_split);
				//path_string = (path_name+"\\"+r_name);
				path_string = (path_name+"\\GPU_processed");
				CreateDirectory(path_string.c_str(),NULL);
				path_string = (path_string + "\\" + r_name);
				CreateDirectory(path_string.c_str(),NULL);
			}
			//std::cout<<"Debug: "<<__FUNCTION__<<"path_string:"<<path_string<<std::endl;
			//sprintf(fname_opencv,"%s\\%i.tiff",path_string.c_str(),i);
			sprintf(fname_opencv,"%s\\%i.png",path_string.c_str(),i);
			imwrite(fname_opencv, new_image);
		}
		imwrite("E:\\a.png", avg_img);
		*/
	}

	// reslice before saving to disk
	else
	{
		float *resliced_data_array = new float[height_bfr*width_trm*frames];

		for (int i = 0; i < width_trm; i++)
			for (int j = 0; j < frames_tot; j++) 
				for (int k = 0; k < height_1fr; k++)
					resliced_data_array[i*frames_tot*height_1fr+j*height_1fr+k] = 
						proc_data_array[j*width_trm*height_1fr+i*height_1fr+k];

		for (int i = 0; i < width_trm; i ++)	
		{
			image = Mat(frames_tot, height_1fr, CV_32F, &resliced_data_array[i*frames_tot*height_1fr]);
			new_image = Mat(frames_tot, height_1fr, CV_8U);
			image.convertTo(new_image, CV_8U, 255);
			//new_image = cv::Mat(width_trm, height_1fr, CV_16U);
			//image.convertTo(new_image,CV_16U,65535);

			cv::namedWindow("Preview", cv::WINDOW_AUTOSIZE);
			cv::imshow("Preview", new_image);			
			cv::waitKey(1);
			
			// Try to create the directory on the first iteration.
			if (i == 0)
			{
				unsigned slash_split = p.fnameData.find_last_of("/\\");
				string path_name = p.fnameData.substr(0,slash_split);
				string f_name = p.fnameData.substr(slash_split+1);
				unsigned period_split = f_name.find_last_of("."); 
				string r_name =  f_name.substr(0, period_split);
				//string path_string = (path_name+"\\"+r_name+" - reslice");

				path_string = (path_name+"\\GPU_processed");
				CreateDirectory(path_string.c_str(),NULL);
				path_string = (path_string + "\\" + r_name + " - reslice");
				CreateDirectory(path_string.c_str(),NULL);
			}

			sprintf(fname_opencv,"%s\\%i.tiff",path_string.c_str(),i);
			imwrite(fname_opencv, new_image);
		}
		destroyWindow("Preview");
		delete[] resliced_data_array;
	}

	
	image.release();
	new_image.release();
	cv::waitKey(1);
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"1 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
}

void Output::freeResources()
{
	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"-1 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	// modify here for cuda error @brian
	cufftDestroy(plan_w);
	cufftDestroy(plan_w2);

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"0 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaStreamDestroy(stream1) );
	gpuErrchk( cudaStreamDestroy(stream2) );

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"1 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaFree(d_raw_data) );
	gpuErrchk( cudaFree(d_proc_buff_0) );
	gpuErrchk( cudaFree(d_proc_buff_trm) );
	gpuErrchk( cudaFree(d_proc_buff_db) );
	gpuErrchk( cudaFree(d_proc_buff_trns) );
	gpuErrchk( cudaFree(d_proc_buff_1) );
	gpuErrchk( cudaFree(d_proc_buff_2) );

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"2 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;

	gpuErrchk( cudaHostUnregister(h_buff_1) );
	delete[] h_buff_1;

	cudaMemGetInfo(&mem_avail, &mem_total);
	//std::cout<<"Debug: "<<__FUNCTION__<<"3 mem avail: "<<mem_avail<<" total: "<<mem_total<<std::endl;
}