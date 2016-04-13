#include "ProcessorApi.h"
#include "DP_logger.h"

ProcessorApi::ProcessorApi(void)
{
	prcsrCali = new ProcessorCalibration();
	octPrcssr=new PrcssrAccess();
}


ProcessorApi::~ProcessorApi(void)
{
	
	if (prcsrCali != NULL) { delete prcsrCali; prcsrCali = NULL; }
	if (octPrcssr != NULL) { octPrcssr->destroy(); delete octPrcssr; octPrcssr = NULL; }
}

void ProcessorApi::setProcessorCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice,
	int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
	int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear,
	bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
	std::string fnamePhase, std::string fnameBg, std::string fnameData)
{
	//std::cout<<"Debug: "<<__FUNCTION__<<"batchFrames: "<<batchFrames<<std::endl;

	prcsrCali->setProcessorCalibration(batchFrames, batchAscans, prevFreq, reslice,
		numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear,
		dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fnameBg, fnameData);
	
	setPrames();
}

void ProcessorApi::resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3)
{
	prcsrCali->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3);
	setPrames();
}
void ProcessorApi::setPrames()
{
	// Send settings
	//std::cout<<"Debug: "<<__FUNCTION__<<"prcsrCali->m_batchFrames: "<<prcsrCali->m_batchFrames<<std::endl;

	octPrcssr->setBatchFrames(prcsrCali->m_batchFrames);
	octPrcssr->setBatchAscans(prcsrCali->m_batchAscans);
	octPrcssr->setPrevFreq(prcsrCali->m_prevFreq);
	octPrcssr->setReslice(prcsrCali->m_reslice);

	octPrcssr->setNumAscansPerBscan(prcsrCali->m_numAscansPerBscan);
	octPrcssr->setNumBScans(prcsrCali->m_numBScans);
	octPrcssr->setNumCameraPixels(prcsrCali->m_numCameraPixels);
	octPrcssr->setNumBgFrame(prcsrCali->m_numBgFrame);
	octPrcssr->setStartPixel(prcsrCali->m_startPixel);
	octPrcssr->setEndPixel(prcsrCali->m_endPixel);
	octPrcssr->setAlpha(prcsrCali->m_alpha);
	octPrcssr->setGrayLevel(prcsrCali->m_grayLevel);
	octPrcssr->setInterpLinear(prcsrCali->m_interpLinear);

	// What kind of dispersion compensation was selected?
	octPrcssr->setDispModeGS(prcsrCali->m_dispModeGS);

	octPrcssr->setDispA2(prcsrCali->m_dispA2);
	octPrcssr->setDispA3(prcsrCali->m_dispA3);
	octPrcssr->setDispTests(prcsrCali->m_dispTests);
	octPrcssr->setDispFrame(prcsrCali->m_dispFrame);

	octPrcssr->setFnamePhase(prcsrCali->m_fnamePhase);
	octPrcssr->setFnameBg(prcsrCali->m_fnameBg);
	octPrcssr->setFnameData(prcsrCali->m_fnameData);
	//octPrcssr->setFnamePhase("E:\\Data\\gpu_test_data\\calibration_files\\1024_calibration_file.txt");
	//octPrcssr->setFnameBg("E:\\test\\0.bin");
	//octPrcssr->setFnameData("E:\\test\\1.bin");
}

void ProcessorApi::processBg()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
	cout<<"processbg 0"<<endl;

	int result = 0;
	try
	{
		octPrcssr->initDLL();
		result = octPrcssr->param();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 1"<<endl;

		//std::cout<<"Debug: "<<__FUNCTION__<<" "<<result<<std::endl;
		if (result != 0) { throw invalid_argument("param error"); }

		result = octPrcssr->data();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 2"<<endl;

		//std::cout<<"Debug: "<<__FUNCTION__<<" "<<result<<std::endl;
		if (result != 0) { throw invalid_argument("data error"); }
		result = octPrcssr->interp();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 3"<<endl;

		if (result != 0) { throw invalid_argument("interp error"); }

		result = octPrcssr->disp();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 4"<<endl;

		if (result != 0) { throw invalid_argument("disp error"); }
		result = octPrcssr->bg();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 5"<<endl;

		if (result != 0) { throw invalid_argument("bg error"); }
		//result = octPrcssr->output();

		now = time(0);
		ltm = localtime(&now);
		cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
		cout<<"processbg 6"<<endl;

		if (result != 0) { throw invalid_argument("output error"); }

		DP_LOGGER_INFO << __FUNCTION__<<"\t\tprocessBg done.";

	} catch (std::exception &e)
	{
		//std::cout<<"Error: "<<__FUNCTION__<<" "<<result<<std::endl;
		DP_LOGGER_ERROR << __FUNCTION__<<"\t\tprocessBg error:"<<result<<std::endl;
		octPrcssr->destroy();
		delete octPrcssr; octPrcssr = NULL;	// Not sure if this does anything
	}

}


void ProcessorApi::processBg(int dispModeGS, float &a2, float &a3)
{
	int result = 0;
	
		octPrcssr->initDLL();
		result = octPrcssr->param();
		std::cout<<"Debug: "<<__FUNCTION__<<" param: "<<result<<std::endl;
		if (result != 0) { throw invalid_argument("param error"); }

		cout<<"before data"<<endl;
		result = octPrcssr->data();
		if (result != 0) { throw invalid_argument("data error"); }
		result = octPrcssr->interp();
		if (result != 0) { throw invalid_argument("interp error"); }

		result = octPrcssr->disp(dispModeGS, a2, a3);
		if (result != 0) { throw invalid_argument("disp error"); }
		result = octPrcssr->bg();
		if (result != 0) { throw invalid_argument("bg error"); }
		//result = octPrcssr->output();
		//if (result != 0) { throw invalid_argument("output error"); }

		DP_LOGGER_INFO << __FUNCTION__<<"\t\tprocessBg done."<<result;
		
		//octPrcssr->destroy();
		//delete octPrcssr; octPrcssr = NULL;	// Not sure if this does anything
}

void ProcessorApi::processFrameData(std::string fnameData2, std::string datatype)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
	cout<<"processFrameData 0"<<endl;

	int result;
	result = octPrcssr->output(fnameData2, datatype);
	if (result != 0) { 
		DP_LOGGER_ERROR << __FUNCTION__<<"\t\tprocessFrameData error."<<result;
		throw invalid_argument("output error");
	}

	now = time(0);
	ltm = localtime(&now);
	cout<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<"\t";
	cout<<"processFrameData 1 "<<endl;

	//system("pause");
	DP_LOGGER_INFO << __FUNCTION__<<"\t\tprocessFrameData done."<<result;
	
}