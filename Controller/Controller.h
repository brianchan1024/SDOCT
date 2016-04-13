#ifndef CONTROLLER_h
#define CONTROLLER_h


#ifdef CONTROLLERAPIDLL_EXPORTS
#define CONTROLLERAPIDLL __declspec(dllexport) 
#else
#define CONTROLLERAPIDLL __declspec(dllimport) 
#endif


#include "common.h"
#include "Util.h"
#include "DataAcquisitionApi.h"
#include "TrajectoryApi.h"
#include "Trajectory.h"
#include "NIDacCard.h"
#include "DacCalibration.h"
#include "ProcessorApi.h"


/*
enum WorkingModel_e
{
	IDEL = 0,
	BACKGROUND,
	ACQUIRE,
	PREVIEWXZ,
	PREVIEWYZ,
	FUNDUS_VIEW
};
*/

/*!
 * @brief This class is used as central controller which call the DAC, DAQ, DataProcessor and called by UI
 */

class CONTROLLERAPIDLL Controller
{
private:
	TrajectoryApi* m_pTtrajApi;

	Trajectory* m_pAcqTraj;			//!< pointer of acquire trajectory
    Trajectory* m_pPrevTrajXZ;		//!< pointer of XZ preview trajectory
	Trajectory* m_pPrevTrajYZ;		//!< pointer of YZ preview trajectory
    Trajectory* m_pFundusTraj;		//!< pointer of fundus view trajectory
	Trajectory* m_pBackgroundTraj;	//!< pointer of background trajectory
	DacCalibration* m_pDacCali;		
	NIDacCard* m_pDacCard;			
	ProcessorApi* previewPrcsrApi;	//!< pointer of ProcessorApi, used to process preview data
	ProcessorApi* acquirePrcsrApi;	//!< pointer of ProcessorApi, used to process acquire data
	ProcessorApi* fundusPrcsrApi;	//!< pointer of ProcessorApi, used to process fundus data
	uInt_32 m_uiNumPixel;			//!< number of pixel
	uInt_32 m_uiNumAscan;			//!< number of A scan in acquire mode
	uInt_32 m_uiNumBscan;			//!< number of B scan in acquire mode
	uInt_32 m_uiNumBscanRepeat;     //!< number of B scan repeat in acquire mode

	int m_iPreviewAScan;			//!< number of A scan in preview mode
	int m_iBackgroundAScan;			//!< number of A scan in get background data mode
	int m_iFundusViewAScan;

	std::string m_fAcquireDataSet;	//!< file to save acquire data set
	std::string m_fFundusDataSet;	//!< file to save fundus view data set
	std::string m_fbackground;		//!< file to save background data
	std::string m_fpreviewxz;		//!< file to save preview XZ
	std::string m_fpreviewyz;		//!< file to save preview YZ
	std::vector<short> m_vData;

	int returnValue;
	int runExpCheck(std::exception e);
	void runRetValCheck(bool re, string job);
	void initGetSettingByMode(WorkingModel_e mode, int& aScan, int& bScan, int& bScanRepeat, std::string& fileName, Trajectory*& traj);
public:
	Controller();
	~Controller();

	void setControllerCalibration(uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, std::string fbackground, std::string fpreviewxz, std::string fpreviewyz, std::string fAcquireDataSet, std::string fFundusDataSet);
	void resetAcquireDataSet(std::string fAcquireDataSet);

	// Need to init trajectory again after call resetScanPattern, but not needed for setScanPattern.
	void setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat);
	void resetScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat);

	// Need to init trajectory again after call resetTrajectoryCalibration, but not needed for setTrajectoryCalibration.
	void setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV);
	void resetTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV);
	
	//int initTrajectory();
	void initTrajectory();
	
	void setDacCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, std::string sXChanName, std::string sYChanName, std::string sSwitchChanName, bool bExterClked, std::string sExterClkChanName, uInt_32 ulTrajGenRate);
	void initNIDacCard();

	// set PrcssrCalibration for preview
	void setPreviewPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
		int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
		std::string fnamePhase, std::string fnameBg, std::string fnameData);

	// set PrcssrCalibration for fundus view
	void setFundusPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
		int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
		std::string fnamePhase, std::string fnameBg, std::string fnameData);

	// set PrcssrCalibration for acquire mode
	void setAcquirePrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame, int numBScansRepeat, 
		int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
		std::string fnamePhase, std::string fnameBg, std::string fnameData);

	void resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3);
	int getAcquireDataSet(); // get the scan data of the whole sample
	int getFundusDataSet();
	int getPreviewXZ();
	int getPreviewYZ();
	int getBackground();
	bool getData(WorkingModel_e mode);

	int processPreviewBcgr();
	int processPreviewBcgr(bool dispModeGS, float &a2, float &a3);
	int processPreviewFrameData(std::string datafile);

	int processFundusBcgr();
	int processFundusFrameData(std::string datafile);

	int processAcquireBcgr();
	int processAcquireFrameData(std::string datafile);
	std::vector<short> getSpectrum();
};

#endif