#pragma once

#include <string>
#include "common.h"


class TrajectoryCalibration
{
	friend class TrajectoryGenerator;
private:
    float m_fXMaxV;
    float m_fYMaxV;
    float m_fXScaleVM; // [v/m]
    float m_fYScaleVM; // [v/m]
    //float m_fXOffsetV;
	//float m_fYOffsetV;

	//uInt_32 m_uiNumPixels;
    //uInt_32 m_uiNumAscan;
    //uInt_32 m_uiNumBscan;

    uInt_32 m_uiUpdateRate; //[points/sec]
    float m_fWaveEffi;
    std::string m_sPhyChannel;
    
    float m_fExposureTime;

	float m_fTrajTriVMax;
	float m_fTrajTriAMax;
	//float m_fTrajRestVMax;
	//float m_fTrajRestAMax;
	int NUM_BG_LINES;
	int m_iPreviewAScan;
	int m_iBackgroundAScan;
	int m_iFundusViewAScan;

	float m_fFundusXOffsetV;
	float m_fFundusYOffsetV;

public:
	TrajectoryCalibration() {}

    TrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, //float fXOffsetV, float fYOffsetV,
                            uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
    {
       setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM,
                            uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
    }

	~TrajectoryCalibration() {}

	void setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
                            uInt_32 uiUpdateRate, float fWaveEffi, std::string sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
    {
        m_fXMaxV = fXMaxV;
        m_fYMaxV = fYMaxV;
        m_fXScaleVM = fXScaleVM;
        m_fYScaleVM = fYScaleVM;

        m_uiUpdateRate = uiUpdateRate;
        m_fWaveEffi = fWaveEffi;
        m_sPhyChannel = sPhyChannel;
        
        m_fExposureTime = fExposureTime;

		m_fTrajTriVMax = 12000 * 0.5; // cal max change rate to voltage: 120000 volt/s
		m_fTrajTriAMax = 12000000; // unit: volt/(s*s), not calculated, just set to a predict safe value, original: 1027718330*0.5; 
		
		//m_fTrajRestVMax = 12000 * 0.5;   // no tused
		//m_fTrajRestAMax = 30000000.0*0.5;  // not used

		NUM_BG_LINES = 50;
		m_iPreviewAScan = iPreviewAScan;
		m_iBackgroundAScan = iBackgroundAScan;
		m_iFundusViewAScan = iFundusViewAScan;

		m_fFundusXOffsetV = fFundusXOffsetV;
		m_fFundusYOffsetV = fFundusYOffsetV;
    }
};
