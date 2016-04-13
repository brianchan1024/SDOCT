#pragma once

#include "common.h"

class DaqCalibration
{
    friend class DataAcquisition;
private:
    uInt_32 m_uiNumPixels;
    uInt_32 m_uiNumAscan;
    uInt_32 m_uiNumBscan;
	uInt_32 m_uiNumBscanRepeat;

public:
	DaqCalibration() {};
    DaqCalibration(uInt_32 uiNumPixels, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBscanRepeat)
    {
        setDaqCalibration(uiNumPixels, uiNumAscan, uiNumBscan, uiNumBscanRepeat);
    }
	~DaqCalibration() {};
	void setDaqCalibration(uInt_32 uiNumPixels, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBscanRepeat)
	{
		m_uiNumPixels = uiNumPixels;
		m_uiNumAscan = uiNumAscan;
		m_uiNumBscan = uiNumBscan;
		m_uiNumBscanRepeat = uiNumBscanRepeat;
	}	
};

