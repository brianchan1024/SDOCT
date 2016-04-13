#ifndef TRAJECTORYAPI_H
#define TRAJECTORYAPI_H

#ifdef TRAJECTORYAPIDLL_EXPORTS
#define TRAJECTORYAPIDLL __declspec(dllexport) 
#else
#define TRAJECTORYAPIDLL __declspec(dllimport) 
#endif

#include "common.h"
#include "TrajectoryCalibration.h"
#include "ScanPattern.h"
#include "Trajectory.h"

class TRAJECTORYAPIDLL TrajectoryApi
{
private:
    TrajectoryCalibration* m_pTrajCali;
    ScanPattern* m_pScanPatt;
    TrajectoryGenerator* m_pTrajGenor;
	void initTrajGener();

public:
	TrajectoryApi();
	~TrajectoryApi();

    void setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
                                  uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV);
    
    void setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV,
                        uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat);
    
    Trajectory* getAcquireTraj();
    Trajectory* getPreviewTraj(int previewType);
    Trajectory* getFundusViewTraj();
	Trajectory* getBackgroundTraj();
};

#endif