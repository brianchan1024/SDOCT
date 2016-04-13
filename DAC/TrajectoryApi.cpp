
#include "TrajectoryApi.h"
#include "DAC_logger.h"

TrajectoryApi::TrajectoryApi(void)
{
	m_pScanPatt = new ScanPattern();
	m_pTrajCali = new TrajectoryCalibration();
	m_pTrajGenor = new TrajectoryGenerator();
}


TrajectoryApi::~TrajectoryApi(void)
{
	delete m_pScanPatt;
	m_pScanPatt = NULL;

	delete m_pTrajCali;
	m_pTrajCali = NULL;

	delete m_pTrajGenor;
	m_pTrajGenor = NULL;

}

void TrajectoryApi::initTrajGener()
{
	m_pTrajGenor->setTrajectoryGenerator(m_pTrajCali, m_pScanPatt);
}

void TrajectoryApi::setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
                                uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
{
    //m_pTrajCali = new TrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM,
    //                                    uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime);
	m_pTrajCali->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM,
                                        uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
}
    
void TrajectoryApi::setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV,
                    uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
{
    //m_pScanPatt = new ScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV,
    //                                uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame);
	 m_pScanPatt->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV,
                                    uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
}
    
Trajectory* TrajectoryApi::getAcquireTraj()
{
	Trajectory* traj = NULL;
	try
	{
		initTrajGener();
		traj = m_pTrajGenor->initAcquireTraj();
	} catch (std::exception &ex)
	{
		throw std::invalid_argument("TrajectoryApi getting acquire trajectory Error");
	}
	return traj;
}
    
Trajectory* TrajectoryApi::getPreviewTraj(int previewType)
{
	Trajectory* traj = NULL;
	try
	{
		initTrajGener();
		traj = m_pTrajGenor->initPreviewTraj(previewType);
	} catch (std::exception &ex)
	{
		throw std::invalid_argument("TrajectoryApi getting preview trajectory Error");
	}
	return traj;
}
    
Trajectory* TrajectoryApi::getFundusViewTraj()
{
	Trajectory* traj = NULL;
	try
	{
		initTrajGener();
		traj = m_pTrajGenor->initFundusViewTraj();
	} catch (std::exception &ex)
	{
		throw std::invalid_argument("TrajectoryApi getting fundus trajectory Error");
	}
	return traj;
}

Trajectory* TrajectoryApi::getBackgroundTraj()
{
	Trajectory* traj = NULL;
	try
	{
		initTrajGener();
		traj = m_pTrajGenor->initBackgoundTraj();
	} catch (std::exception &ex)
	{
		throw std::invalid_argument("TrajectoryApi getting background trajectory Error");
	}
	return traj;
}
