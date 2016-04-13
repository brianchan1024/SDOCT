#pragma once

#include <string>

class DacCalibration
{
	friend class NIDacCard;
private:
    float m_fXMaxV;
    float m_fYMaxV;
    float m_fXScaleVM; // [v/m]
    float m_fYScaleVM; // [v/m]
    
    std::string m_sXChanName;
    std::string m_sYChanName;
    std::string m_sSwitchChanName;
    
    bool m_bExterClked; // Externally Clocked
    std::string m_sExterClkChanName; // External Clock Channel Name
    unsigned long m_ulTrajGenRate; //[points/sec]
    
    
public:
	DacCalibration() {};
    DacCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
                        std::string sXChanName, std::string sYChanName, std::string sSwitchChanName,
                        bool bExterClked, std::string sExterClkChanName, unsigned long ulTrajGenRate)
    {
        setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM,
                        sXChanName, sYChanName, sSwitchChanName,
                        bExterClked, sExterClkChanName, ulTrajGenRate);
    }
	~DacCalibration(void)
	{
	}
	void setDacCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM,
                        std::string sXChanName, std::string sYChanName, std::string sSwitchChanName,
                        bool bExterClked, std::string sExterClkChanName, unsigned long ulTrajGenRate)
    {
        m_fXMaxV = fXMaxV;
        m_fYMaxV = fYMaxV;
        m_fXScaleVM = fXScaleVM;
        m_fYScaleVM = fYScaleVM;
        m_sXChanName = sXChanName;
        m_sYChanName = sYChanName;
        m_sSwitchChanName = sSwitchChanName;
        m_bExterClked = bExterClked;
        m_sExterClkChanName = sExterClkChanName;
        m_ulTrajGenRate = ulTrajGenRate;
    }
};

