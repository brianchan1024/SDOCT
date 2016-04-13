#include "Controller.h"
#include "Ctr_logger.h"

Controller::Controller()
{
	m_pTtrajApi = new TrajectoryApi();
	m_pDacCali = new DacCalibration();
	m_pDacCard = new NIDacCard();
	previewPrcsrApi = new ProcessorApi();
	acquirePrcsrApi = new ProcessorApi();
	fundusPrcsrApi = new ProcessorApi();
	returnValue = 0;
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tController generate.";
}

Controller::~Controller()
{
	if(m_pTtrajApi != NULL) { delete m_pTtrajApi; m_pTtrajApi = NULL; }
	if(m_pDacCali != NULL) { delete m_pDacCali; m_pDacCali = NULL; }
	if(m_pDacCard != NULL) { delete m_pDacCard; m_pDacCard = NULL; }
	
	if(m_pAcqTraj != NULL) { delete m_pAcqTraj; m_pAcqTraj = NULL; }
	if(m_pPrevTrajXZ != NULL) { delete m_pPrevTrajXZ; m_pPrevTrajXZ = NULL; }
	if(m_pPrevTrajYZ != NULL) { delete m_pPrevTrajYZ; m_pPrevTrajYZ = NULL; }
	if(m_pFundusTraj != NULL) { delete m_pFundusTraj; m_pFundusTraj = NULL; }
	if(m_pBackgroundTraj != NULL) { delete m_pBackgroundTraj; m_pBackgroundTraj = NULL; }

	//if(previewPrcsrApi != NULL) { delete previewPrcsrApi; previewPrcsrApi = NULL; } // should delete here, but exception occurs with it
	//if(acquirePrcsrApi != NULL) { delete acquirePrcsrApi; acquirePrcsrApi = NULL; }

	if(m_pDacCard != NULL) { delete m_pDacCard; m_pDacCard = NULL; }

}

void Controller::setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
{
	// init setting
	// pay attention here, the unit of range value should be voltage rather than mm
	m_pTtrajApi->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);

}

void Controller::setControllerCalibration(uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, std::string fbackground, std::string fpreviewxz, std::string fpreviewyz, std::string fAcquireDataSet, std::string fFundusDataSet)
{
	m_uiNumPixel = uiNumPixel;
	m_uiNumAscan = uiNumAscan;
	m_uiNumBscan = uiNumBscan;

	m_uiNumBscanRepeat = uiNumBscanRepeat;

	m_iPreviewAScan = iPreviewAScan;
	m_iBackgroundAScan = iBackgroundAScan;
	m_iFundusViewAScan = iFundusViewAScan;

	m_fbackground = fbackground;
	m_fpreviewxz = fpreviewxz;
	m_fpreviewyz = fpreviewyz;
	m_fAcquireDataSet = fAcquireDataSet;
	m_fFundusDataSet = fFundusDataSet;
}

void Controller::resetAcquireDataSet(std::string fAcquireDataSet)
{
	m_fAcquireDataSet = fAcquireDataSet;
}
void Controller::setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
{
	// init setting
	m_pTtrajApi->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
}

void Controller::resetScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
{
	// click apply, reset scan pattern params, should update trajectory
	setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
	initTrajectory();
}

void Controller::resetTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
{
	// click apply, reset trajectory calibration params, should update trajectory
	setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
	initTrajectory();
}

void Controller::initTrajectory()
{
	m_pAcqTraj = m_pTtrajApi->getAcquireTraj();
	//m_pAcqTraj->printTrajectory();
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet acquire trajectory done.";

	m_pPrevTrajXZ = m_pTtrajApi->getPreviewTraj(0);
	//m_pPrevTrajXZ->printTrajectory();
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet preview xz trajectory done.";

	m_pPrevTrajYZ = m_pTtrajApi->getPreviewTraj(1);
	//m_pPrevTrajYZ->printTrajectory();
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet preview yz trajectory done.";

	m_pFundusTraj = m_pTtrajApi->getFundusViewTraj();
	//m_pFundusTraj->printTrajectory();
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet fundus trajectory done.";

	m_pBackgroundTraj = m_pTtrajApi->getBackgroundTraj();
	//m_pBackgroundTraj->printTrajectory();
	CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet background trajectory done.";
}
/*
int Controller::initTrajectory()
{
	try{
		m_pAcqTraj = m_pTtrajApi->getAcquireTraj();
		//m_pAcqTraj->printTrajectory();
		CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet acquire trajectory done.";

		m_pPrevTrajXZ = m_pTtrajApi->getPreviewTraj(0);
		//m_pPrevTrajXZ->printTrajectory();
		CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet preview xz trajectory done.";

		m_pPrevTrajYZ = m_pTtrajApi->getPreviewTraj(1);
		//m_pPrevTrajYZ->printTrajectory();
		CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet preview yz trajectory done.";

		m_pFundusTraj = m_pTtrajApi->getFundusViewTraj();
		//m_pFundusTraj->printTrajectory();
		CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet fundus trajectory done.";

		m_pBackgroundTraj = m_pTtrajApi->getBackgroundTraj();
		//m_pBackgroundTraj->printTrajectory();
		CTR_LOGGER_INFO << __FUNCTION__<<"\t\tGet background trajectory done.";
	}
	catch (std::exception &ex) 
	{ returnValue = runExpCheck(ex); }
	return returnValue;
}
*/

void Controller::setDacCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
									std::string sXChanName, std::string sYChanName, std::string sSwitchChanName, bool bExterClked, std::string sExterClkChanName, uInt_32 ulTrajGenRate)
{
	m_pDacCali->setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, sXChanName, sYChanName, sSwitchChanName, bExterClked, sExterClkChanName, ulTrajGenRate);
}

void Controller::initNIDacCard()
{
	m_pDacCard->setNIDacCard(m_pDacCali);
}

void Controller::setPreviewPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
	int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
	std::string fnamePhase, std::string fnameBg, std::string fnameData)
{
	previewPrcsrApi->setProcessorCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fnameBg, fnameData);
}

void Controller::setFundusPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
	int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
	std::string fnamePhase, std::string fnameBg, std::string fnameData)
{
	fundusPrcsrApi->setProcessorCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fnameBg, fnameData);
}

void Controller::setAcquirePrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame, int numBScansRepeat,
	int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
	std::string fnamePhase, std::string fnameBg, std::string fnameData)
{
	acquirePrcsrApi->setProcessorCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans * (1 + numBScansRepeat), numCameraPixels, numBgFrame,
		startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
		fnamePhase, fnameBg, fnameData);
}

void Controller::resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3)
{
	previewPrcsrApi->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3);
	acquirePrcsrApi->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3);
	fundusPrcsrApi->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3);
}

void Controller::runRetValCheck(bool re, string job)
{
	if (re == true)
	{
		CTR_LOGGER_INFO << __FUNCTION__ << "\t\t" << job << " done.";
	} else {
		CTR_LOGGER_ERROR << __FUNCTION__ << "\t\t" << job << " error.";
	}
}

int Controller::getAcquireDataSet()
{
	CTR_LOGGER_TRACE << __FUNCTION__;
	WorkingModel_e mode = ACQUIRE;
	bool re = getData(mode);
	runRetValCheck(re, "get acquire data");
	return re;
}

int Controller::getFundusDataSet()
{
	CTR_LOGGER_TRACE << __FUNCTION__;
	WorkingModel_e mode = FUNDUS_VIEW;
	bool re = getData(mode);
	runRetValCheck(re, "get fundus data");
	return re;
}

int Controller::getPreviewXZ()
{
	WorkingModel_e mode = PREVIEWXZ;
	bool re = getData(mode);
	runRetValCheck(re, "get preview xz data");
	return re;
}

int Controller::getPreviewYZ()
{
	WorkingModel_e mode = PREVIEWYZ;
	bool re = getData(mode);
	runRetValCheck(re, "get preview yz data");
	return re;
}

int Controller::getBackground()
{
	WorkingModel_e mode = BACKGROUND;
	CTR_LOGGER_TRACE << __FUNCTION__;
	bool re = getData(mode);
	runRetValCheck(re, "get background data");
	return re;
}

void Controller::initGetSettingByMode(WorkingModel_e mode, int& aScan, int& bScan, int& bScanRepeat, std::string& fileName, Trajectory*& traj)
{
	if (mode == ACQUIRE)
	{
		aScan = m_uiNumAscan;
		bScan = m_uiNumBscan;
		fileName = m_fAcquireDataSet;
		traj = m_pAcqTraj;
		bScanRepeat = m_uiNumBscanRepeat;
	}
	else if (mode == PREVIEWXZ)
	{
		aScan = m_iPreviewAScan;
		bScan = 1;  // m_uiNumBscan = 1 in PREVIEWXZ
		fileName = m_fpreviewxz;
		traj = m_pPrevTrajXZ;
		bScanRepeat = 0;
	}
	else if (mode == PREVIEWYZ)
	{
		aScan = m_iPreviewAScan;
		bScan = 1; // m_uiNumBscan = 1 in PREVIEWYZ
		fileName = m_fpreviewyz;
		traj = m_pPrevTrajYZ;
		bScanRepeat = 0;
	}
	else if (mode == BACKGROUND)
	{
		aScan = m_iBackgroundAScan;
		bScan = 1;  // m_uiNumBscan = 1 in BACKGROUND
		fileName = m_fbackground;
		traj = m_pBackgroundTraj;
		bScanRepeat = 0;
	}
	else if (mode == FUNDUS_VIEW)
	{
		aScan = m_iFundusViewAScan;
		bScan = m_iFundusViewAScan;  // m_uiNumBscan = 1 in BACKGROUND
		fileName = m_fFundusDataSet;
		traj = m_pFundusTraj;
		bScanRepeat = 0;
	}
}
/*!
 * the main entrance of getting data, steps as following:
 * 1 get trajectory for given mode by calling TrajectoryApi, (done in initTrajectory)
 * 2 set the trajectory to the DacCard,
 * 3 prepare data acquire model by calling DataAcquisitionApi
 * 4 trigger data acquiring by calling DacCard
 * 5 copy the acquired data to local by calling DataAcquisitionApi
 * 6 save the data to disk
 */
bool Controller::getData(WorkingModel_e mode)
{
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 0";
	int aScan, bScan, bScanRepeat;
	std::string fileName;
	Trajectory * traj;
	initGetSettingByMode(mode, aScan, bScan, bScanRepeat, fileName, traj);
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 1";
	
	//m_pDacCard->setSafePosition(); // when call setSafePosition, the trjacoty output will be set to 0, which causes trajectory output value sudden change, so commented out here
		
	m_pDacCard->setTrajectory(traj);
	DataBuffer* pBuf = new DataBuffer(m_uiNumPixel, aScan, bScan, bScanRepeat);
	DataAcquisitionApi* daqApi = new DataAcquisitionApi();
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 2";

	daqApi->setDaqCalibration(m_uiNumPixel, aScan, bScan, bScanRepeat);

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 3";

	daqApi->startDataAcquire();

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 4";

	m_pDacCard->triggerTrajectory();

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 5";

	daqApi->copyData(pBuf);	

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 6";

	daqApi->stopDataAcquire();
		
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 7";

	m_pDacCard->close();

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 8";

	this->m_vData = pBuf->getFrameData();

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 9";

	Util::saveFrameData2File(m_vData, fileName + ".txt", m_uiNumPixel);
	Util::saveFrameData2BinFile(m_vData, fileName + ".bin");

	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 10";

	delete pBuf; pBuf =NULL;
	delete daqApi; daqApi = NULL;
	
	return true;
}

/*
int Controller::getData(WorkingModel_e mode)
{
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 0";
	int aScan, bScan, bScanRepeat;
	std::string fileName;
	Trajectory * traj;
	initGetSettingByMode(mode, aScan, bScan, bScanRepeat, fileName, traj);
	CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 1";
	try
	{
		//m_pDacCard->setSafePosition(); // when call setSafePosition, the trjacoty output will be set to 0, which causes trajectory output value sudden change, so commented out here
		
		m_pDacCard->setTrajectory(traj);
		DataBuffer* pBuf = new DataBuffer(m_uiNumPixel, aScan, bScan, bScanRepeat);
		DataAcquisitionApi* daqApi = new DataAcquisitionApi();
		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 2";

		daqApi->setDaqCalibration(m_uiNumPixel, aScan, bScan, bScanRepeat);

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 3";

		daqApi->startDataAcquire();

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 4";

		m_pDacCard->triggerTrajectory();

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 5";

		daqApi->copyData(pBuf);	

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 6";

		daqApi->stopDataAcquire();
		
		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 7";

		m_pDacCard->close();

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 8";

		this->m_vData = pBuf->getFrameData();

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 9";

		Util::saveFrameData2File(m_vData, fileName + ".txt", m_uiNumPixel);

		Util::saveFrameData2BinFile(m_vData, fileName + ".bin");

		CTR_LOGGER_TRACE << __FUNCTION__<<"\t\t" << "getData 10";

		delete pBuf; pBuf =NULL;
		delete daqApi; daqApi = NULL;
	} catch (std::exception &ex) 
	{ returnValue = runExpCheck(ex); }

	return returnValue;
}
*/

int Controller::processPreviewBcgr()
{
	float a2, a3;
	return processPreviewBcgr(false, a2, a3);
}

// processBackground, dispModeGS: true->update dispersion params a2,a3
//								  false->no updating
int Controller::processPreviewBcgr(bool dispModeGS, float &a2, float &a3)
{
	/*
	try
	{
		previewPrcsrApi->processBg(dispModeGS, a2, a3);
	} catch (std::exception &ex)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tProcess preview background Error.";
		returnValue = 1;
	}
	return returnValue;
	*/
	bool re = previewPrcsrApi->processBg(dispModeGS, a2, a3);
}

int Controller::processPreviewFrameData(std::string datafile)
{
	try
	{
		previewPrcsrApi->processFrameData(datafile, "preview");
	} catch (std::exception &ex)
	{
		CTR_LOGGER_INFO << __FUNCTION__<<" process preview data " << datafile << " done.";
		returnValue = 1;
	}	
	return returnValue;
}

int Controller::processFundusBcgr()
{
	try
	{
		float a2, a3;
		fundusPrcsrApi->processBg(false, a2, a3); // set first param false, not no update a2, a3
	} catch (std::exception &ex)
	{
		std::cout<<"ERROR: "<<__FUNCTION__<<" Process fundus background Error"<<std::endl;
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tProcess fundus background Error.";
		returnValue = 1;
	}
	return returnValue;
}

int Controller::processFundusFrameData(std::string datafile)
{
	try
	{
		fundusPrcsrApi->processFrameData(datafile, "fundus");
		CTR_LOGGER_INFO << __FUNCTION__<<" process fundus frame data done.";
	} catch(std::exception &ex)
	{
		CTR_LOGGER_INFO << __FUNCTION__<<" process fundus frame data error.";
		returnValue = 1;
	}
	return returnValue;
}

int Controller::processAcquireBcgr()
{
	try
	{
		float a2, a3;
		acquirePrcsrApi->processBg(false, a2, a3); // set first param false, not no update a2, a3
	} catch (std::exception &ex)
	{
		std::cout<<"ERROR: "<<__FUNCTION__<<" Process acquire background Error"<<std::endl;
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tProcess acquire background Error.";
		returnValue = 1;
	}
	return returnValue;
}

int Controller::processAcquireFrameData(std::string datafile)
{
	try
	{
		acquirePrcsrApi->processFrameData(datafile, "acquire");
		CTR_LOGGER_INFO << __FUNCTION__<<" process acquire frame data done.";
	} catch(std::exception &ex)
	{
		CTR_LOGGER_INFO << __FUNCTION__<<" process acquire frame data error.";
		returnValue = 1;
	}
	return returnValue;
}

std::vector<short> Controller::getSpectrum()
{
	return m_vData;
}
/*!
 * error type
 * 1: 1?? error from TrajectoryApi
 * 2: 2?? error from NIDacCard
 * 3: 3?? error from DataAcquisition
 */
int Controller::runExpCheck(std::exception e)
{
	CTR_LOGGER_ERROR << __FUNCTION__<<"\t\trunErrorCheck error.";
	int re;
	if (std::string("TrajectoryApi getting acquire trajectory Error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tTrajectoryApi getting acquire trajectory Error.";
		re = 100;
	} else if (std::string("TrajectoryApi getting preview trajectory Error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tTrajectoryApi getting preview trajectory Error.";
		re = 101;
	} else if (std::string("TrajectoryApi getting fundus trajectory Error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<" TrajectoryApi getting fundus trajectory Error.";
		re = 102;
	} else if (std::string("TrajectoryApi getting background trajectory Error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tTrajectoryApi getting background trajectory Error.";
		re = 103;
	} else if (std::string("NIDacCard initCard error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tNIDacCard initCard error.";
		re = 200;
	} else if (std::string("NIDacCard createTraj error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tNIDacCard createTraj error.";
		re = 201;
	} else if (std::string("NIDacCard triggerTrajectory error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tNIDacCard triggerTrajectory error.";
		re = 202;
	} else if (std::string("NIDacCard setSafePosition error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tNIDacCard setSafePosition error.";
		re = 203;
	} else if (std::string("DataAcquisition InitializeSession error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition InitializeSession error.";
		re = 300;
	} else if (std::string("DataAcquisition AllocateACQMemory error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition AllocateACQMemory error.";
		re = 301;
	} else if (std::string("DataAcquisition StartGrabImage error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition StartGrabImage error.";
		re = 302;
	} else if (std::string("DataAcquisition StopGrabImage error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition StopGrabImage error.";
		re = 303;
	} else if (std::string("DataAcquisition stopAcquire error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition stopAcquire error.";
		re = 304;
	} else if (std::string("DataAcquisition QueImage error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition QueImage error.";
		re = 305;
	} else if (std::string("DataAcquisition SaveQueImage error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tProcess acquire background Error.";
		re = 306;
	} else if (std::string("DataAcquisition bufferCopyIn error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tDataAcquisition bufferCopyIn error.";
		re = 307;
	} else if (std::string("Util saveFrameData2File error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tUtil saveFrameData2File error.";
		re = 400;
	} else if (std::string("Util saveFrameData2BinFile error").compare(e.what()) == 0)
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tUtil saveFrameData2BinFile error.";
		re = 401;
	} else
	{
		CTR_LOGGER_ERROR << __FUNCTION__<<"\t\tUnknown error: "<<e.what();
		re = 500;
	}
	return re;
}